# Module screensync
import httpclient, os, queues

const
  SYNC_URL = "http://localhost:5000/"

var
  files*, old: Queue[string] = initQueue[string]()

proc push*[T](q: var Queue[T]; item: T) =
    add(q, item)

proc pop*[T](q: var Queue[T]): T =
    result = dequeue(q)

proc sync*(): string =
  var data = newMultipartData()
  while files.len() > 0:
    var filename = files.pop()
    data.addFiles({"screenshots[]": filename})
    old.push(filename)

  try:
    result = postContent(SYNC_URL, multipart=data)
  except:
    while old.len() > 0:
      files.push(old.pop())

  while old.len() > 0:
    var filename = old.pop()
    removeFile(filename)
