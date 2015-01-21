QT -= gui
QT += xml
CONFIG += console
TARGET = lgupdate
INCLUDEPATH += .
DEPENDPATH += .

isEmpty(ROOT_DIR) {
    DESTDIR     = ../../bin
} else {
    DESTDIR     = $$ROOT_DIR/bin
}

CONFIG(debug, debug|release) {
    TARGET = $$join(TARGET,,,d)
}

SOURCES += \
    LGCFUpdateMain.cpp

