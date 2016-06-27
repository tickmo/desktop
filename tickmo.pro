QT += widgets \
      network
TARGET = tickmo
CONFIG += staticlib
CONFIG += link_pkgconfig
HEADERS     = \
    mainwindow.h \
    settingsdialog.h \
    timer.h \
    imguploader.h \
    mainimage.h
SOURCES     = \
              main.cpp \
    mainwindow.cpp \
    settingsdialog.cpp \
    timer.cpp\
    imguploader.cpp \
    mainimage.cpp
FORMS += \
    mainwindow.ui \
    settingsdialog.ui

OBJECTS_DIR = qtcache/obj
MOC_DIR = qtcache/moc
RCC_DIR = qtcache/rcc
UI_DIR = qtcache/ui

RESOURCES = timer.qrc

CONFIG(debug, release|debug): DEFINES += DEBUG

unix: PKGCONFIG += Qt5X11Extras
unix: PKGCONFIG += xcb-screensaver
unix: PKGCONFIG += xcb
