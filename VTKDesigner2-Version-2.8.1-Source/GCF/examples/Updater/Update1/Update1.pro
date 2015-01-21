TEMPLATE = lib
TARGET = Update1
DESTDIR = Binary

# You can also use CONFIG += gcf_component, if you have set QMAKEFEATURES
# to point to GCF_SDK_ROOT/qmake_prf directory
include(../../../qmake_prf/gcf_component.prf)

QT          += webkit

# Input
INCLUDEPATH += ../SimpleBrowser
SOURCES = SnapshotCapturerComponent.cpp
HEADERS = SnapshotCapturerComponent.h
OTHER_FILES = Binary/SnapshotCapturerComponent.xml \
              Binary/Install.js
