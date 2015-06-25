import x, xlib, xkblib

include "xsspragma.nim"

type
  PXScreenSaverInfo = ptr TXScreenSaverInfo
  TXScreenSaverInfo* = object
    window*: TWindow          # screen saver window
    state*: cint              # ScreenSaver{Off,On,Disabled}
    kind*: cint               # ScreenSaver{Blanked,Internal,External}
    til_or_since*: culong     # milliseconds
    idle*: culong             # milliseconds
    event_mask*: culong       # events

  PXScreenSaverNotifyEvent = ptr TXScreenSaverNotifyEvent
  TXScreenSaverNotifyEvent* = object
    `type`*: cint             # of event
    serial*: culong           # # of last request processed by server
    send_event*: bool         # true if this came frome a SendEvent request
    display*: PDisplay        # Display the event was read from
    window*: TWindow          # screen saver window
    root*: TWindow            # root window of event screen
    state*: cint              # ScreenSaver{Off,On,Cycle}
    kind*: cint               # ScreenSaver{Blanked,Internal,External}
    forced*: bool             # extents of new region
    time*: TTime              # event timestamp


proc XScreenSaverQueryExtension*(dpy: PDisplay; event_basep: ptr cint; error_basep: ptr cint): bool{.libxssc, importc: "XScreenSaverQueryExtension".}

proc XScreenSaverQueryVersion*(dpy: PDisplay; major_versionp: ptr cint; minor_versionp: ptr cint): TStatus{.libxssc, importc: "XScreenSaverQueryVersion".}

proc XScreenSaverAllocInfo*(): TXScreenSaverInfo{.libxssc, importc: "XScreenSaverAllocInfo".}

proc XScreenSaverQueryInfo*(dpy: PDisplay; drawable: TDrawable; saver_info: PXScreenSaverInfo): TStatus{.libxssc, importc: "XScreenSaverQueryInfo".}

proc XScreenSaverSelectInput*(dpy: PDisplay; drawable: TDrawable; mask: culong){.libxssc, importc: "XScreenSaverSelectInput".}

proc XScreenSaverSetAttributes*(dpy: PDisplay; drawable: TDrawable; x: cint; y: cint; width: cuint; height: cuint; border_width: cuint; depth: cint; class: cuint; visual: PVisual; valuemask: culong; attributes: PXSetWindowAttributes){.libxssc, importc: "XScreenSaverSetAttributes".}

proc XScreenSaverUnsetAttributes*(dpy: PDisplay; drawable: TDrawable){.libxssc, importc: "XScreenSaverUnsetAttributes".}

# Maybe deprecated
# proc XScreenSaverSaverRegister*(dpy: PDisplay; screen: cint; xid: TXID; `type`: TAtom){.libxssc, importc: "XScreenSaverSaverRegister".}

proc XScreenSaverUnregister*(dpy: PDisplay; screen: cint): TStatus{.libxssc, importc: "XScreenSaverUnregister".}

proc XScreenSaverGetRegistered*(dpy: PDisplay; screen: cint; xid: PXID; `type`: PAtom): TStatus{.libxssc, importc: "XScreenSaverGetRegistered".}

proc XScreenSaverSuspend*(dpy: PDisplay; suspend: bool){.libxssc, importc: "XScreenSaverSuspend".}
