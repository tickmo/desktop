#ifndef _SCRNSAVER_H_
#define _SCRNSAVER_H_

#include <X11/Xfuncproto.h>
#include <X11/Xlib.h>
#include <X11/extensions/saver.h>

typedef struct {
    Window window;                /* screen saver window */
    int state;                    /* ScreenSaver{Off,On,Disabled} */
    int kind;                     /* ScreenSaver{Blanked,Internal,External} */
    unsigned long til_or_since;   /* milliseconds */
    unsigned long idle;           /* milliseconds */
    unsigned long event_mask;     /* events */
} XScreenSaverInfo;

typedef struct {
    int type;               /* of event */
    unsigned long serial;   /* # of last request processed by server */
    Bool send_event;        /* true if this came frome a SendEvent request */
    Display *display;       /* Display the event was read from */
    Window window;          /* screen saver window */
    Window root;            /* root window of event screen */
    int state;              /* ScreenSaver{Off,On,Cycle} */
    int kind;               /* ScreenSaver{Blanked,Internal,External} */
    Bool forced;            /* extents of new region */
    Time time;              /* event timestamp */
} XScreenSaverNotifyEvent;

Bool XScreenSaverQueryExtension(Display *dpy, int *event_basep, int *error_basep);

Status XScreenSaverQueryVersion(Display *dpy, int *major_versionp, int *minor_versionp);

XScreenSaverInfo *XScreenSaverAllocInfo(void);

Status XScreenSaverQueryInfo(Display *dpy, Drawable drawable, XScreenSaverInfo *saver_info);

void XScreenSaverSelectInput(Display *dpy, Drawable drawable, unsigned long mask);

void XScreenSaverSetAttributes(Display *dpy, Drawable drawable, int x, int y, unsigned int width, unsigned int height, unsigned int border_width, int depth, unsigned int class, Visual *visual, unsigned long valuemask, XSetWindowAttributes *attributes);

void XScreenSaverUnsetAttributes(Display *dpy, Drawable drawable);

void XScreenSaverSaverRegister(Display *dpy, int screen, XID xid, Atom type);

Status XScreenSaverUnregister(Display *dpy, int screen);

Status XScreenSaverGetRegistered(Display *dpy, int screen, XID *xid, Atom *type);

void XScreenSaverSuspend(Display *dpy, Bool suspend);

#endif /* _SCRNSAVER_H_ */
