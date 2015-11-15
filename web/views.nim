import templates, layout, jester, os
let files_path = "savedscreens/*.png"

proc index*(request: Request): string =
  tmpli  html"""
      <div>
          <ul>
            $for screen in walkFiles($files_path) {
              <li><img src="$screen"></li>
            }
          </ul>
      </div>
      """

  return layout.layout("Screenshots", result)
