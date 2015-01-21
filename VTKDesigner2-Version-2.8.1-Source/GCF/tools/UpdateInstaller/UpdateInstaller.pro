######################################################################
# GCF - Merges one file into another
######################################################################

TEMPLATE    = app
TARGET      = UINST
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
SOURCES  = FileObject.cpp FileSystemObject.cpp SystemObject.cpp UpdateInstaller.cpp Main.cpp
HEADERS  = FileObject.h FileSystemObject.h SystemObject.h UpdateInstaller.h
FORMS    = UpdateInstaller.ui LicenseDialog.ui
QT      += xml script

# Uncompressor stuff..
INCLUDEPATH += ../CreateUpdate
SOURCES     += ../CreateUpdate/UpdateFileUncompressor.cpp
HEADERS     += ../CreateUpdate/UpdateFileUncompressor.h \
               ../CreateUpdate/UpdateFileCompressorCommon.h

# Logger stuff
DEFINES     += VCL_USE_STATIC_LIB
INCLUDEPATH += ../../include
HEADERS     += ../../include/GCF/Logger.h
SOURCES     += ../../source/core/Logger.cpp


