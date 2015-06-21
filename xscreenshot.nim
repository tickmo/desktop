# Module xscreenshot
import xlib, xutil, x, cairo, cairoxlib

var
  display: PDisplay
  screen: cint

proc take_screenshot*(filename: string) =
  display = XOpenDisplay(nil)
  if display == nil:
    quit "Failed to open display"
  
  screen = XDefaultScreen(display)
  var rootwin = XRootWindow(display, screen)

  var surface = xlib_surface_create(display, rootwin, DefaultVisual(display, screen), DisplayWidth(display, screen), DisplayHeight(display, screen));
  discard write_to_png(surface, filename);
  destroy(surface);

when isMainModule:
  take_screenshot("screenshot.png")