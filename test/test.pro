#-------------------------------------------------
#
# Project created by QtCreator 2013-10-11T21:11:47
#
#-------------------------------------------------

QT       += network xml testlib

QT       -= gui

TARGET = test
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += \
    ../commander.cpp \
    ../config.cpp \
    ../localfile.cpp \
    ../downloader.cpp \
    ../productversion.cpp \
    ../product.cpp \
    ../osdetection.cpp \
    ../message.cpp \
    ../settings.cpp \
    ../update.cpp \
    ../xmlparser.cpp \
    ../wincommander.cpp \
    ../version.cpp \
    tst_clienttest.cpp
DEFINES += SRCDIR=..

HEADERS += \
    ../commander.h \
    ../config.h \
    ../localfile.h \
    ../downloader.h \
    ../productversion.h \
    ../product.h \
    ../osdetection.h \
    ../message.h \
    ../settings.h \
    ../update.h \
    ../xmlparser.h \
    ../wincommander.h \
    ../version.h

win32{
LIBS+= Shell32.lib Advapi32.lib
}