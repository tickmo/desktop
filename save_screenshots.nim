import jester, future, asyncdispatch

proc save(request: PRequest): string =
  var screenshots = request.formData["screenshots[]"]
  writeFile(screenshots.fields["filename"], screenshots.body)
  return ""

routes:
  post "/": resp save(request)

runForever()
