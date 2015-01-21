######################################################################
# GCF - Extract contents from a compressed update file
######################################################################

TEMPLATE    = app
TARGET      = ExtractGVZ
DEPENDPATH  += .
INCLUDEPATH += . ../CreateUpdate
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
               ../CreateUpdate/UpdateFileUncompressor.cpp

HEADERS      = ../CreateUpdate/UpdateFileUncompressor.h \
               ../CreateUpdate/UpdateFileCompressorCommon.h

QT          += xml
CONFIG      += console
