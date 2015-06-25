# Module timer
import times, xscreenshot, xlib, xutil, x, keysym, tables, scrnsaver

const
  WINDOW_WIDTH = 300
  WINDOW_HEIGHT = 150
  WINDOW_BG_COLOR = "#C5C2C5"
  IDLE_TIME = 10 # idle time 10 seconds

let
  interval: float = 5 # time interval in seconds

var
  width, height: cuint
  display: PDisplay
  screen: cint
  depth: int
  win: TWindow
  sizeHints: TXSizeHints
  wmDeleteMessage: TAtom
  running: bool
  xev: TXEvent
  lasttime: float = getTime().toSeconds()
  status: string
  timer_running: bool = true
  colors = initTable[string, TXColor]()
  info: TXScreenSaverInfo
  idle: float = 0.0

proc getColor(name: string): TXColor =
  discard XParseColor(display, DefaultColormap(display, 0), name, result.addr)
  discard XAllocColor(display, DefaultColormap(display, 0), result.addr)
  discard colors.hasKeyOrPut(name, result)

proc create_window =
  width = WINDOW_WIDTH
  height = WINDOW_HEIGHT

  display = XOpenDisplay(nil)
  if display == nil:
    quit "Failed to open display"

  screen = XDefaultScreen(display)
  depth = XDefaultDepth(display, screen)
  var rootwin = XRootWindow(display, screen)
  win = XCreateSimpleWindow(display, rootwin, 100, 10,
                            width, height, 5,
                            XBlackPixel(display, screen),
                            getColor(WINDOW_BG_COLOR).pixel)
  size_hints.flags = PSize or PMinSize or PMaxSize
  size_hints.min_width =  width.cint
  size_hints.max_width =  width.cint
  size_hints.min_height = height.cint
  size_hints.max_height = height.cint
  discard XSetStandardProperties(display, win, "Simple time tracker", "window",
                         0, nil, 0, addr(size_hints))
  discard XSelectInput(display, win, ButtonPressMask or KeyPressMask or
                                     PointerMotionMask or ExposureMask)
  discard XMapWindow(display, win)

  wmDeleteMessage = XInternAtom(display, "WM_DELETE_WINDOW", false.TBool)
  discard XSetWMProtocols(display, win, wmDeleteMessage.addr, 1)
  running = true
  info = XScreenSaverAllocInfo();

proc close_window =
  discard XDestroyWindow(display, win)
  discard XCloseDisplay(display)

proc timer_start* =
  timer_running = true

proc timer_stop* =
  timer_running = false

proc timer_active*: bool =
  return timer_running

proc drawButton(x, y, w, h: cint, pressed: bool = false) =
  # todo: inner shadow was broken =(
  if pressed:
    discard XSetForeground(display, DefaultGC(display,screen), XBlackPixel(display, screen))
    discard XDrawLine(display, win, DefaultGC(display,screen), x, y, x + w, y)
    discard XDrawLine(display, win, DefaultGC(display,screen), x, y, x, y + h)
    discard XSetForeground(display, DefaultGC(display,screen), XWhitePixel(display, screen))
    discard XDrawLine(display, win, DefaultGC(display,screen), x + w, y + h, x, y + h)
    discard XDrawLine(display, win, DefaultGC(display,screen), x + w, y + h, x + w, y)
    # discard XSetForeground(display, DefaultGC(display,screen), getColor("#838183").pixel)
    # discard XDrawLine(display, win, DefaultGC(display,screen), x + 2, y + 1, x + w - 3, y + 1)
    # discard XDrawLine(display, win, DefaultGC(display,screen), x + 1, y + 1, x + 1, y + h - 2)
  else:
    discard XSetForeground(display, DefaultGC(display,screen), XWhitePixel(display, screen))
    discard XDrawLine(display, win, DefaultGC(display,screen), x, y, x + w, y)
    discard XDrawLine(display, win, DefaultGC(display,screen), x, y, x, y + h)
    discard XSetForeground(display, DefaultGC(display,screen), XBlackPixel(display, screen))
    discard XDrawLine(display, win, DefaultGC(display,screen), x + w, y + h, x, y + h)
    discard XDrawLine(display, win, DefaultGC(display,screen), x + w, y + h, x + w, y)
    # discard XSetForeground(display, DefaultGC(display,screen), getColor("#838183").pixel)
    # discard XDrawLine(display, win, DefaultGC(display,screen), x + w - 2, y + h - 1, x + 1, y + h - 1)
    # discard XDrawLine(display, win, DefaultGC(display,screen), x + w - 1, y + h - 2, x + w - 1, y + 1)

proc draw_screen =
  # Clear draw area
  discard XSetForeground(display, DefaultGC(display, screen), getColor("#d3d3d3").pixel)
  discard XFillRectangle(display, win, DefaultGC(display, screen), 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT)

  # Set text color
  discard XSetForeground(display, DefaultGC(display, screen), XBlackPixel(display, screen))

  # Display timer status
  var timer_status: string
  if timer_active():
    timer_status = "Active"
  else:
    timer_status = "Inactive"

    # Display idle time
    var idleString: string = "Your idle time is " & $idle.int & " seconds"
    discard XDrawString(display, win, DefaultGC(display,screen), 40, 140, idleString.cstring, idleString.len.cint)

  discard XDrawString(display, win, DefaultGC(display,screen), 110, 85, timer_status.cstring, timer_status.len.cint)

  # Dispay time
  discard XDrawString(display, win, DefaultGC(display,screen), 40, 40, status.cstring, status.len.cint)

  # Display pause button
  drawButton(80, 60, 100, 40, timer_running)

proc handle_event =
  discard XNextEvent(display, xev.addr);
  case xev.theType
  of Expose:
    discard
  of ClientMessage:
    # Stop app if user close window
    if cast[TAtom](xev.xclient.data[0]) == wmDeleteMessage:
      running = false
  of KeyPress:
    var key = XLookupKeysym(cast[PXKeyEvent](xev.addr), 0)
    if key == 0x20:
      if timer_active():
        timer_stop()
      else:
        timer_start()
      draw_screen()
  else:
    discard

proc tick =
  discard XScreenSaverQueryInfo(display, XDefaultRootWindow(display), info.addr);
  if info.idle.int > idle.int or info.idle.int < idle.int:
    idle = info.idle.int / 1000
    draw_screen()

  if idle.int > IDLE_TIME:
    timer_stop()
  else:
    timer_start()

  var cur_time = getTime().toSeconds()
  if timer_running and lasttime <= cur_time: # Takes first screen with start program
    lasttime = cur_time + interval
    take_screenshot("screens/screenshot-" & $cur_time & ".png")
    status = "Tick - " & $fromSeconds(lasttime)
    draw_screen()

when isMainModule:
  create_window()
  while running:
    tick()
    while XPending(display) > 0:
      handle_event()
  close_window()
