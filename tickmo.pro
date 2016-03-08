QT += widgets \
      network
TARGET = tickmo
CONFIG += staticlib
HEADERS     = \
    mainwindow.h \
    settingsdialog.h \
    timer.h \
    imguploader.h
SOURCES     = \
              main.cpp \
    mainwindow.cpp \
    settingsdialog.cpp \
    timer.cpp\
    imguploader.cpp
FORMS += \
    mainwindow.ui \
    settingsdialog.ui

OBJECTS_DIR = qtcache/obj
MOC_DIR = qtcache/moc
RCC_DIR = qtcache/rcc
UI_DIR = qtcache/ui

RESOURCES = timer.qrc

CONFIG(debug, release|debug): DEFINES += DEBUG
