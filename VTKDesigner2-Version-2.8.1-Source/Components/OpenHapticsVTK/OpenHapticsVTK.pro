######################################################################
# VTK Designer 2 - OpenHapticsVTK component
# 
# Provides OpenHaptics integration for VTK. Using the classes provided
# in this component, one can quickly haptics enable visualization
# scenarios designed in VTK Designer
#
######################################################################

include(../CommercialComponents.pri)
include(../../VTK.pri)

MOC_DIR         = $${PROJECT_BASE}/tmp/OpenHapticsVTK
OBJECTS_DIR     = $${PROJECT_BASE}/tmp/OpenHapticsVTK
UI_HEADERS_DIR  = $${PROJECT_BASE}/tmp/OpenHapticsVTK
RCC_DIR         = $${PROJECT_BASE}/tmp/OpenHapticsVTK
UI_DIR          = $${PROJECT_BASE}/tmp/OpenHapticsVTK

# Additional includes from the VTK system
INCLUDEPATH     += $${PROJECT_BASE}/Components/VTKSystem/Interfaces \
                   $${PROJECT_BASE}/Components/VTKSystem/IOData \
                   $${PROJECT_BASE}/Common

# OpenHapticsVTK Configuration
exists( DeviceSerialNo.h ) {
    DEFINES         += USE_DEVICE_SERIAL_NO
    INCLUDEPATH     += .
}

# OpenHaptics Stuff
win32:OH_BASE = "C:/Program Files/SensAble/3DTouch"
win32:INCLUDEPATH += $${OH_BASE}/include $${OH_BASE}/utilities/include
win32:LIBS += $${OH_BASE}/lib/win32/hl.lib $${OH_BASE}/lib/win32/hd.lib \
              $${OH_BASE}/utilities/lib/Win32/Release/hlu.lib \
              $${OH_BASE}/utilities/lib/Win32/Release/hdu.lib

unix:OH_BASE = "/usr/"
unix:INCLUDEPATH += $${OH_BASE}/include $${OH_BASE}/utilities/include
unix:LIBS += -L$${OH_BASE} -lHL -lHD -lHLU -lHDU -lrt
unix:DEFINES += linux NDEBUG

# Input
HEADERS += source/COpenHapticsVTKComponent.h \
           source/CVtkOpenHapticsActor.h \
           source/CVtkOpenHapticsCursorActor.h \
           source/CVtkOpenHapticsProperty.h \
           source/CVtkOpenHapticsRenderer.h \
           source/CVtkOpenHapticsEffect.h \
           source/IVtkOpenHapticsPropertyIOData.h \
           source/vtkOpenHapticsActor.h \
           source/vtkOpenHapticsCursorActor.h \
           source/vtkOpenHapticsProperty.h \
           source/vtkOpenHapticsRenderer.h \
           source/vtkOpenHapticsEffect.h \
           source/CShapeRenderModeSelector.h \
           source/CVtkOpenHapticsEffectIOData.h
           
SOURCES += source/COpenHapticsVTKComponent.cpp \
           source/CVtkOpenHapticsActor.cpp \
           source/CVtkOpenHapticsCursorActor.cpp \
           source/CVtkOpenHapticsProperty.cpp \
           source/CVtkOpenHapticsRenderer.cpp \
           source/CVtkOpenHapticsEffect.cpp \
           source/vtkOpenHapticsActor.cpp \
           source/vtkOpenHapticsCursorActor.cpp \
           source/vtkOpenHapticsProperty.cpp \
           source/vtkOpenHapticsRenderer.cpp \
           source/vtkOpenHapticsEffect.cpp \
           source/CShapeRenderModeSelector.cpp \
           source/CVtkOpenHapticsEffectIOData.cpp
           
RESOURCES += OpenHapticsVTK.qrc

FORMS     += CShapeRenderModeSelector.ui

# Other classes
HEADERS   +=  $${PROJECT_BASE}/Common/CGenericVisNodeDesc.h \
              $${PROJECT_BASE}/Common/CGenericVisNodeBase.h \
              $${PROJECT_BASE}/Components/VTKSystem/IOData/CVtkActorData.h \
              $${PROJECT_BASE}/Components/VTKSystem/IOData/CVtkRendererData.h \

SOURCES   +=  $${PROJECT_BASE}/Common/CGenericVisNodeDesc.cpp \
              $${PROJECT_BASE}/Common/CGenericVisNodeBase.cpp \
              $${PROJECT_BASE}/Components/VTKSystem/IOData/CVtkActorData.cpp \
              $${PROJECT_BASE}/Components/VTKSystem/IOData/CVtkRendererData.cpp


