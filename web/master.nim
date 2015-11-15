import future, strutils, sequtils, asyncdispatch
import jester, templates
import views

routes:
  get "/":    resp views.index(request)
  post "/":    resp views.save(request)

runForever()
