TEMPLATE = lib
TARGET = Update2
DESTDIR = Binary

# You can also use CONFIG += gcf_component, if you have set QMAKEFEATURES
# to point to GCF_SDK_ROOT/qmake_prf directory
include(../../../qmake_prf/gcf_component.prf)

QT          += webkit

# Input
INCLUDEPATH += ../SimpleBrowser
SOURCES = BookmarkComponent.cpp
HEADERS = BookmarkComponent.h
OTHER_FILES = Binary/BookmarkComponent.xml \
              Binary/Install.js
