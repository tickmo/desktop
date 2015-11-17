# Server file. Provides routes and working with URLs.
import future, asyncdispatch, strutils ,sequtils, asyncdispatch
import jester, templates
import layouts

proc getImages(request: PRequest): string =
  var screenshots = request.formData["screenshots[]"]
  let filesPath = "savedscreens/" & screenshots.fields["filename"]
  writeFile(filesPath, screenshots.body)
  return ""

routes:
  post "/": resp getImages(request)
  get "/":    resp layouts.index(request)

runForever()
