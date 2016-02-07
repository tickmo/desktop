QT += widgets
TARGET = TickMo
TEMPLATE = lib
CONFIG += release
CONFIG += staticlib
HEADERS     = \
    mainwindow.h
SOURCES     = \
              main.cpp \
    mainwindow.cpp
FORMS += \
    mainwindow.ui

OBJECTS_DIR = qtcache/obj
MOC_DIR = qtcache/moc
RCC_DIR = qtcache/rcc
UI_DIR = qtcache/ui
