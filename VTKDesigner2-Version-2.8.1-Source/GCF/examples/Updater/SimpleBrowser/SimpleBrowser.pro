TEMPLATE = app
TARGET = SimpleBrowser
DESTDIR = ../../../bin

# You can also use CONFIG += gcf_app, if you have set QMAKEFEATURES
# to point to GCF_SDK_ROOT/qmake_prf directory
include(../../../qmake_prf/gcf_app.prf)

QT  += webkit

# Input
SOURCES = Main.cpp BrowserWindow.cpp BrowserWindowComponent.cpp
HEADERS = BrowserWindow.h BrowserWindowComponent.h
FORMS = BrowserWindow.ui

CONFIG(debug, debug|release) {
    CONFIG += console
} else {
    CONFIG -= console
}

OTHER_FILES = \
    GUIXML/UpdaterComponent.xml \
    GUIXML/SimpleBrowser.xml \
    GUIXML/BrowserWindowComponent.xml
