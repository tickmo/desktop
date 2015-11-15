import templates, layout, jester

proc index*(request: PRequest): string =
  tmpli html"""
      <div>
          <ul>
              <li>screnshot</li>
          </ul>
      </div>
      """

  return layout.layout("Screenshots", result)

proc save*(request: PRequest): string =
  writeFile("filename.png", request.formData.mget("images[]").body)
