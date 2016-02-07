{.link:"libTickMo.a" passL:"-lQt5Core -lQt5Gui -lQt5Widgets".}
proc initQt: cint {.importC.}

# Module timer
import times, xscreenshot, xlib, xutil, x, keysym, tables, scrnsaver, screensync

const
  IDLE_TIME = 10 # idle time 10 seconds

let
  interval: float = 5 # time interval in seconds
  syncInterval: float = 5 # time period to sync files with server

var
  display: PDisplay
  running: bool = true
  timerRunning: bool = false
  timerStopedManual: bool = true
  lastframe, lasttime: float = getTime().toSeconds()
  synctime: float = getTime().toSeconds() + syncInterval
  info: TXScreenSaverInfo = XScreenSaverAllocInfo()
  idle: float = 0.0
  fps: int = 30

proc timerActive*: bool {.exportC.} =
  return timerRunning

proc timerStart* {.exportC.} =
  timerRunning = true

proc timerStop* {.exportC.} =
  timerRunning = false

proc timerToggle* {.exportC.} =
  if timerActive():
    timerStop()
    timerStopedManual = true
  else:
    timerStart()
    timerStopedManual = false

proc tick*: cstring {.exportC.} =
  var curTime = getTime().toSeconds()
  let filename = "screens/screenshot-" & $curTime & ".png"
  if not timerStopedManual:
    discard XScreenSaverQueryInfo(display, XDefaultRootWindow(display), info.addr);
    if (info.idle.int > idle.int or info.idle.int < idle.int) and lastframe <= curTime:
      idle = info.idle.int / 1000

    if timerActive() and idle.int > IDLE_TIME:
      timerStop()
      timerStopedManual = false
    elif not timerActive() and idle.int < IDLE_TIME:
      timerStart()


  if timerRunning:
    if lasttime <= curTime: # Takes first screen with start program
      lasttime = curTime + interval
      result = filename
      take_screenshot(filename)
      files.push(filename)

    if synctime <= curTime: # Sync files with remote server
      synctime = curTime + syncInterval

  if lastframe <= curTime: # Takes first screen with start program
    lastframe = curTime + 1/fps

proc init =
  display = XOpenDisplay(nil)
  if display == nil:
    quit "Failed to open display"

when isMainModule:
  init()
  discard initQt()
