# Module timer
import times, xscreenshot

let
  interval: float = 5 # time interval in seconds

var
  lasttime: float = getTime().toSeconds()

proc tick =
  if lasttime + interval < getTime().toSeconds():
    lasttime += interval
    echo("Tick - ", fromSeconds(lasttime))
    take_screenshot("screenshot.png")

when isMainModule:
  while true:
    tick()