# included from xlib bindings


when defined(use_pkg_config) or defined(use_pkg_config_static):
    {.pragma: libxss, cdecl, importc.}
    {.pragma: libxssc, cdecl.}
    when defined(use_pkg_config_static):
        {.passl: gorge("pkg-config xss --static --libs").}
    else:
        {.passl: gorge("pkg-config xss --libs").}
else:
    when defined(macosx):
        const
          libxss* = "libxss.dylib"
    else:
        const
          libxss* = "libXss.so"

    {.pragma: libxss, cdecl, dynlib: libxss, importc.}
    {.pragma: libxssc, cdecl, dynlib: libxss.}
