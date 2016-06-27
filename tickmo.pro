QT += widgets \
      network
TARGET = tickmo
CONFIG += staticlib
CONFIG += link_pkgconfig
HEADERS     = \
    src/idledetector/idle.h \
    src/mainimage.h \
    src/mainwindow.h \
    src/settings/settingsdialog.h \
    src/timer/timer.h \
    src/uploader/imguploader.h
SOURCES     = main.cpp \
    src/idledetector/idle_mac.cpp \
    src/idledetector/idle_win.cpp \
    src/idledetector/idle_x11.cpp \
    src/idledetector/idle.cpp \
    src/mainimage.cpp \
    src/mainwindow.cpp \
    src/settings/settingsdialog.cpp \
    src/timer/timer.cpp \
    src/uploader/imguploader.cpp
FORMS += \
    src/mainwindow.ui \
    src/settings/settingsdialog.ui

OBJECTS_DIR = qtcache/obj
MOC_DIR = qtcache/moc
RCC_DIR = qtcache/rcc
UI_DIR = qtcache/ui

RESOURCES = tickmo.qrc

CONFIG(debug, release|debug): DEFINES += DEBUG

windows: INCLUDEPATH += C:/OpenSSL-Win32/include
unix: PKGCONFIG += Qt5X11Extras
unix: PKGCONFIG += xcb-screensaver
unix: PKGCONFIG += xcb
