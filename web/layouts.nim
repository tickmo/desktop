# File with page layouts.
import templates, views, jester, os

# All screens layout
proc index*(request: Request): string =
  let files_path = "savedscreens/*.png"
  tmpli  html"""
    <div class="list-screens">
      <div class="row">
        <ul>
          $for screen in walkFiles($files_path) {
            <li><a href="$screen" class="thumbnail"><img src="$screen" alt="screen not available"></a></li>
          }
        </div>
      </div>
    </div>
      """
  return views.allScreensView("Screenshots", result)
