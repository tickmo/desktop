# Module timer
import times, xscreenshot, xlib, xutil, x, keysym, tables, scrnsaver, screensync

const
  WINDOW_WIDTH = 300
  WINDOW_HEIGHT = 150
  WINDOW_BG_COLOR = "#C5C2C5"
  WINDOW_TITLE = "Simple time tracker"
  IDLE_TIME = 10 # idle time 10 seconds
  BUTTON_POSITION_X = 80
  BUTTON_POSITION_Y = 60
  BUTTON_WIDTH = 100
  BUTTON_HEIGHT = 40
  BUTTON_SHADOW_COLOR = "#838183"

let
  interval: float = 5 # time interval in seconds
  syncInterval: float = 15 # time period to sync files with server

var
  width, height: cuint
  display: PDisplay
  screen: cint
  depth: int
  win: TWindow
  sizeHints: TXSizeHints
  wmDeleteMessage: TAtom
  running, timerRunning, timerStopedManual: bool = false
  xev: TXEvent
  lastframe, lasttime: float = getTime().toSeconds()
  synctime: float = getTime().toSeconds() + syncInterval
  colors = initTable[string, TXColor]()
  info: TXScreenSaverInfo
  idle: float = 0.0
  fps: int = 30 # frames per second will be drawn

proc getColor(name: string): TXColor =
  discard XParseColor(display, DefaultColormap(display, 0), name, result.addr)
  discard XAllocColor(display, DefaultColormap(display, 0), result.addr)
  discard colors.hasKeyOrPut(name, result)

proc createWindow =
  width = WINDOW_WIDTH
  height = WINDOW_HEIGHT

  display = XOpenDisplay(nil)
  if display == nil:
    quit "Failed to open display"

  screen = XDefaultScreen(display)
  depth = XDefaultDepth(display, screen)
  var rootwin = XRootWindow(display, screen)
  win = XCreateSimpleWindow(display, rootwin, 100, 10, width, height, 5, XBlackPixel(display, screen), getColor(WINDOW_BG_COLOR).pixel)
  sizeHints.flags = PSize or PMinSize or PMaxSize
  sizeHints.min_width =  width.cint
  sizeHints.max_width =  width.cint
  sizeHints.min_height = height.cint
  sizeHints.max_height = height.cint
  discard XSetStandardProperties(display, win, WINDOW_TITLE, "window", 0, nil, 0, addr(sizeHints))
  discard XSelectInput(display, win, ButtonPressMask or KeyPressMask or PointerMotionMask or ExposureMask)
  discard XMapWindow(display, win)

  wmDeleteMessage = XInternAtom(display, "WM_DELETE_WINDOW", false.TBool)
  discard XSetWMProtocols(display, win, wmDeleteMessage.addr, 1)
  running = true
  info = XScreenSaverAllocInfo();

proc closeWindow =
  discard sync()
  discard XDestroyWindow(display, win)
  discard XCloseDisplay(display)

proc timerActive*: bool =
  return timerRunning

proc clearRectangle(x, y, w, h: cint) = # Clear draw area
  discard XSetForeground(display, DefaultGC(display, screen), getColor(WINDOW_BG_COLOR).pixel)
  discard XFillRectangle(display, win, DefaultGC(display, screen), x, y, w.cuint, h.cuint)
  discard XSetForeground(display, DefaultGC(display, screen), XBlackPixel(display, screen))

proc drawButton(x, y, w, h: cint, pressed: bool = false) =
  clearRectangle(x, y, w + 2, h + 2)
  var timerStatus: string
  if pressed:
    discard XSetForeground(display, DefaultGC(display,screen), XBlackPixel(display, screen))
    discard XDrawLine(display, win, DefaultGC(display,screen), x, y, x + w, y)
    discard XDrawLine(display, win, DefaultGC(display,screen), x, y, x, y + h)
    discard XSetForeground(display, DefaultGC(display,screen), XWhitePixel(display, screen))
    discard XDrawLine(display, win, DefaultGC(display,screen), x + w, y + h, x, y + h)
    discard XDrawLine(display, win, DefaultGC(display,screen), x + w, y + h, x + w, y)
    discard XSetForeground(display, DefaultGC(display,screen), getColor(BUTTON_SHADOW_COLOR).pixel)
    discard XDrawLine(display, win, DefaultGC(display,screen), x + 2, y + 1, x + w - 3, y + 1)
    discard XDrawLine(display, win, DefaultGC(display,screen), x + 1, y + 1, x + 1, y + h - 2)
    discard XSetForeground(display, DefaultGC(display,screen), XBlackPixel(display, screen))
    timerStatus = "Active"
    discard XDrawString(display, win, DefaultGC(display,screen), 110, 85, timerStatus.cstring, timerStatus.len.cint)
  else:
    discard XSetForeground(display, DefaultGC(display,screen), XWhitePixel(display, screen))
    discard XDrawLine(display, win, DefaultGC(display,screen), x, y, x + w, y)
    discard XDrawLine(display, win, DefaultGC(display,screen), x, y, x, y + h)
    discard XSetForeground(display, DefaultGC(display,screen), XBlackPixel(display, screen))
    discard XDrawLine(display, win, DefaultGC(display,screen), x + w, y + h, x, y + h)
    discard XDrawLine(display, win, DefaultGC(display,screen), x + w, y + h, x + w, y)
    discard XSetForeground(display, DefaultGC(display,screen), getColor(BUTTON_SHADOW_COLOR).pixel)
    discard XDrawLine(display, win, DefaultGC(display,screen), x + w, y + h + 1, x + 1, y + h + 1)
    discard XDrawLine(display, win, DefaultGC(display,screen), x + w + 1, y + h, x + w + 1, y + 1)
    discard XSetForeground(display, DefaultGC(display,screen), XBlackPixel(display, screen))
    timerStatus = "Inactive"
    discard XDrawString(display, win, DefaultGC(display,screen), 110, 85, timerStatus.cstring, timerStatus.len.cint)

proc timerStart* =
  timerRunning = true
  drawButton(BUTTON_POSITION_X, BUTTON_POSITION_Y, BUTTON_WIDTH, BUTTON_HEIGHT, timerRunning)

proc timerStop* =
  timerRunning = false
  drawButton(BUTTON_POSITION_X, BUTTON_POSITION_Y, BUTTON_WIDTH, BUTTON_HEIGHT, timerRunning)

proc drawIdleTime(x, y: cint) = # Dispay idle time
  var time = idle.int - IDLE_TIME
  var
    idleString: cstring = cstring("Your idle time is " & $time & " seconds")
    len: cint = idleString.len.cint
  clearRectangle( x, y - 20, WINDOW_WIDTH, y)

  if time > 0:
    discard XSetForeground(display, DefaultGC(display,screen), XBlackPixel(display, screen))
    discard XDrawString(display, win, DefaultGC(display,screen), x, y, idleString, len)

proc drawLastScreenshotTime(x, y: cint, status: string) = # Display last screenshot time
  clearRectangle( x, y - 20, WINDOW_WIDTH, y)
  discard XSetForeground(display, DefaultGC(display,screen), XBlackPixel(display, screen))
  discard XDrawString(display, win, DefaultGC(display,screen), x, y, status.cstring, status.len.cint)

proc timerTogle =
  if timerActive():
    timerStop()
    timerStopedManual = true
  else:
    timerStart()
    timerStopedManual = false

proc handleEvent =
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
      timerTogle()
  of ButtonPress:
    var
      buttonEvent = cast[PXButtonEvent](xev.addr)
      x = buttonEvent.x
      y = buttonEvent.y
    if x >= BUTTON_POSITION_X and x <= BUTTON_POSITION_X + BUTTON_WIDTH and y >= BUTTON_POSITION_Y and y <= BUTTON_POSITION_Y + BUTTON_HEIGHT and buttonEvent.button == 1:
      timerTogle()
  else:
    discard

proc tick =
  var curTime = getTime().toSeconds()
  let filename = "screens/screenshot-" & $curTime & ".png"
  if not timerStopedManual:
    discard XScreenSaverQueryInfo(display, XDefaultRootWindow(display), info.addr);
    if (info.idle.int > idle.int or info.idle.int < idle.int) and lastframe <= curTime:
      idle = info.idle.int / 1000
      drawIdleTime(40, 140)

    if timerActive() and idle.int > IDLE_TIME:
      timerStop()
      timerStopedManual = false
    elif not timerActive() and idle.int < IDLE_TIME:
      timerStart()

  if timerRunning:
    if lasttime <= curTime: # Takes first screen with start program
      lasttime = curTime + interval
      take_screenshot(filename)
      files.push(filename)
      drawLastScreenshotTime(5, 40, "Last screenshot time - " & $fromSeconds(lasttime))

    if synctime <= curTime: # Sync files with remote server
      synctime = curTime + syncInterval
      discard sync()

  if lastframe <= curTime: # Takes first screen with start program
    lastframe = curTime + 1/fps
    drawButton(BUTTON_POSITION_X, BUTTON_POSITION_Y, BUTTON_WIDTH, BUTTON_HEIGHT, timerRunning)

when isMainModule:
  createWindow()
  timerStart()
  while running:
    tick()
    while XPending(display) > 0:
      handleEvent()
  closeWindow()
