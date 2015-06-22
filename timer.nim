# Module timer
import times, xscreenshot

let
  interval: float = 5 # time interval in seconds

var
  lasttime: float = getTime().toSeconds()

proc tick =
  while true:
    if lasttime < getTime().toSeconds(): # Takes first screen with start program
      lasttime += interval
      echo("Tick - ", fromSeconds(lasttime))
      take_screenshot("screenshot.png")

when isMainModule:
  tick()
