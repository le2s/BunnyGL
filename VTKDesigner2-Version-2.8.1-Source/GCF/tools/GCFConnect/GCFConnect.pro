QT -= gui
QT += core script network
TEMPLATE = lib
CONFIG += static

isEmpty(ROOT_DIR) {
    DESTDIR     = ../../lib
} else {
    DESTDIR     = $$ROOT_DIR/lib
}

TARGET = GCFConnect
CONFIG(debug, debug|release) {
    TARGET = $$join(TARGET,,,d)
}

HEADERS += \
    RemoteApplication.h \
    RemoteApplicationCall.h

SOURCES += \
    RemoteApplication.cpp \
    RemoteApplicationCall.cpp

