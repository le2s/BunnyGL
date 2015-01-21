BASE_DIR = $$PWD/../
!isEmpty(ROOT_DIR): BASE_DIR = $$ROOT_DIR

GCF_TEMP_DIR = $$BASE_DIR/tmp/

INCLUDEPATH += . $$BASE_DIR/GCF/include $$BASE_DIR/include

win32-msvc* {
    CONFIG(debug, debug|release) {
        LIBS += $$BASE_DIR/lib/GCFd2.lib
    } else {
        LIBS += $$BASE_DIR/lib/GCF2.lib
    }
}

win32-g++ {
    CONFIG(debug, debug|release) {
        LIBS += $$BASE_DIR/lib/libGCFd2.a
    } else {
        LIBS += $$BASE_DIR/lib/libGCF2.a
    }
}

unix {
    CONFIG(debug, debug|release) {
        LIBS += -L$$BASE_DIR/bin -L$$BASE_DIR/lib -lGCFd2
    } else {
        LIBS += -L$$BASE_DIR/bin -L$$BASE_DIR/lib -lGCF2
    }
}

win32-msvc* {
 DEFINES    += -D_CRT_SECURE_NO_DEPRECATE
 CONFIG     += embed_manifest_exe warn_on dll
} else {
 DEFINES    += _CRT_SECURE_NO_DEPRECATE
 CONFIG     += warn_off
}

TEMPLATE = lib
CONFIG += shared
DLLDESTDIR = $$BASE_DIR/bin
DESTDIR = $$BASE_DIR/lib
QT += xml

CONFIG(debug, debug|release) {
    TARGET = $$join(TARGET,,,d)
}
