TEMPLATE=subdirs
CONFIG += ordered
include(common.pri)
SUBDIRS += plugin

!win32 {
    error( "This component will only work on Windows. No makefile generated." )
}
