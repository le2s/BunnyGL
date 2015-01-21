######################################################################
# Generic Component Framework Library from VCreate Logic
#
# Provides a Math and XML utils implementation
######################################################################

include (../components.pri)

DEPENDPATH     += . interfaces source
INCLUDEPATH    += . interfaces source

# Input
HEADERS  = UtilsComponent.h
SOURCES  = UtilsComponent.cpp

OTHER_FILES += \
    UtilsComponent.xml

RESOURCES += \
    UtilsComponent.qrc
