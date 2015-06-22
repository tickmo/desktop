# Module timer
import times, xscreenshot, xlib, xutil, x, keysym

const
  WINDOW_WIDTH = 400
  WINDOW_HEIGHT = 50

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
                            XWhitePixel(display, screen))
  size_hints.flags = PSize or PMinSize or PMaxSize
  size_hints.min_width =  width.cint
  size_hints.max_width =  width.cint
  size_hints.min_height = height.cint
  size_hints.max_height = height.cint
  discard XSetStandardProperties(display, win, "Simple Window", "window",
                         0, nil, 0, addr(size_hints))
  discard XSelectInput(display, win, ButtonPressMask or KeyPressMask or 
                                     PointerMotionMask or ExposureMask)
  discard XMapWindow(display, win)

  wmDeleteMessage = XInternAtom(display, "WM_DELETE_WINDOW", false.TBool)
  discard XSetWMProtocols(display, win, wmDeleteMessage.addr, 1)
  running = true

proc close_window =
  discard XDestroyWindow(display, win)
  discard XCloseDisplay(display)

proc timer_start* =
  timer_running = true

proc timer_stop* =
  timer_running = false

proc timer_active*: bool =
  return timer_running

proc draw_screen =
  # Clear draw area
  discard XSetForeground(display, DefaultGC(display, screen), XWhitePixel(display, screen))
  discard XFillRectangle(display, win, DefaultGC(display, screen), 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT)
  
  # Set text color
  discard XSetForeground(display, DefaultGC(display, screen), XBlackPixel(display, screen))
  
  # Dispay time
  discard XDrawString(display,win, DefaultGC(display,screen), 10,40, status.cstring, status.len.cint)
  
  # Display timer status
  var timer_status: string
  if timer_active():
    timer_status = "Active"
  else:
    timer_status = "Inactive"
  timer_status &= " (Press [space] to togle)"
  discard XDrawString(display,win, DefaultGC(display,screen), 10,20, timer_status.cstring, timer_status.len.cint)

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
