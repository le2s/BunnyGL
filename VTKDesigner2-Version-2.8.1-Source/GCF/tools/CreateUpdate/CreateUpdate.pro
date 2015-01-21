######################################################################
# GCF - Create a compressed update file.
######################################################################

TEMPLATE    = app
TARGET      = CreateGVZ
DEPENDPATH  += .
INCLUDEPATH += .

isEmpty(ROOT_DIR) {
    DESTDIR     = ../../bin
} else {
    DESTDIR     = $$ROOT_DIR/bin
}

CONFIG(debug, debug|release) {
    TARGET = $$join(TARGET,,,d)
}

# Input
SOURCES      = Main.cpp \
               UpdateFileCompressor.cpp

HEADERS      = UpdateFileCompressor.h \
               UpdateFileCompressorCommon.h

QT          += xml
QT          -= gui
CONFIG      += console
