import future, strutils, sequtils, asyncdispatch
import jester, templates
import views

proc save(request: PRequest): string =
  var screenshots = request.formData["screenshots[]"]
  let filesPath = "savedscreens/" & screenshots.fields["filename"]
  writeFile(filesPath, screenshots.body)
  return ""

routes:
  post "/": resp save(request)
  get "/":    resp views.index(request)

runForever()
