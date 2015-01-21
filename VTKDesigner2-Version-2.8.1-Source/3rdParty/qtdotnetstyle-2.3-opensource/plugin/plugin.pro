TEMPLATE = lib
CONFIG += qt plugin
win32|mac:!wince*:!win32-msvc:!macx-xcode:CONFIG += debug_and_release build_all
TARGET = qtdotnet
VERSION = 2.3.0

xtestbuild {
 CONFIG -= debug debug_and_release build_all
 CONFIG += release
}

TARGET = $$qtLibraryTarget($$TARGET)
DESTDIR = $$[QT_INSTALL_PLUGINS]/styles

include(../src/qtdotnetstyle.pri)
SOURCES += qtdotnetstyleplugin.cpp

target.path += $$[QT_INSTALL_PLUGINS]/styles
INSTALLS += target
