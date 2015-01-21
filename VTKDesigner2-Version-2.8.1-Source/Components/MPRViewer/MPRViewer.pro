######################################################################
# MPRViewer from VCreateLogic
######################################################################

include(../../GCF/qmake_prf/gcf_component.prf)
include(../../VTK.pri)

DESTDIR = ../../bin
DEPENDPATH += . interfaces source
INCLUDEPATH += . interfaces source ../../Interfaces ../../Common

HEADERS         = source/MPRViewerComponent.h \
                  source/MPRViewerWidget.h \
                  source/MPRViewerVisNode.h
                  
SOURCES         = source/MPRViewerComponent.cpp \
                  source/MPRViewerWidget.cpp \
                  source/MPRViewerVisNode.cpp
                  
FORMS          += source/MPRViewerForm.ui

# CONFIG += opengl

# vtkQtGraphicsViewRenderWindow implementation
HEADERS         += ../../3rdParty/vtkQtGraphicsViewRenderWindow/vtkQtGraphicsViewRenderWindow.h
SOURCES         += ../../3rdParty/vtkQtGraphicsViewRenderWindow/vtkQtGraphicsViewRenderWindow.cpp
INCLUDEPATH     += ../../3rdParty/vtkQtGraphicsViewRenderWindow/
DEPENDPATH      += ../../3rdParty/vtkQtGraphicsViewRenderWindow/

# VNFComponentGen has generated an empty resource file. If you add
# new objects to the resource file, then you can uncomment the line
# below
# RESOURCES += QtNodes.qrc

# Additional includes from the VTK system
INCLUDEPATH     += ../../Components/VTKSystem/Interfaces \
                   ../../Components/VTKSystem/IOData \
                   ../../Common \
                   ./interfaces

# Nodes in VTK Designer 2 have to implement one or more node interfaces
# Implementing all of them from scratch would be a tough task. So, we make
# use of ready made implementations and then subclass from them.
HEADERS   +=  ../../Common/CGenericVisNodeDesc.h \
              ../../Common/CGenericVisNodeBase.h 

SOURCES   +=  ../../Common/CGenericVisNodeDesc.cpp \
              ../../Common/CGenericVisNodeBase.cpp 


# IOData classes. These classes are so small, that they can actually be inline implementations.
# So there is really no need to worry about code size
HEADERS +=  ../../Components/VTKSystem/IOData/CVtk3DWidgetData.h \
            ../../Components/VTKSystem/IOData/CVtkAbstractMapper3DData.h \
            ../../Components/VTKSystem/IOData/CVtkAbstractMapperData.h \
            ../../Components/VTKSystem/IOData/CVtkAbstractPickerData.h \
            ../../Components/VTKSystem/IOData/CVtkAbstractTransformData.h \
            ../../Components/VTKSystem/IOData/CVtkActor2DData.h \
            ../../Components/VTKSystem/IOData/CVtkActorData.h \
            ../../Components/VTKSystem/IOData/CVtkAlgorithmData.h \
            ../../Components/VTKSystem/IOData/CVtkCameraData.h \
            ../../Components/VTKSystem/IOData/CVtkDataObject.h \
            ../../Components/VTKSystem/IOData/CVtkDataSet.h \
            ../../Components/VTKSystem/IOData/CVtkDataSetMapperData.h \
            ../../Components/VTKSystem/IOData/CVtkImageData.h \
            ../../Components/VTKSystem/IOData/CVtkImplicitFunctionData.h \
            ../../Components/VTKSystem/IOData/CVtkImporterData.h \
            ../../Components/VTKSystem/IOData/CVtkInteractorObserverData.h \
            ../../Components/VTKSystem/IOData/CVtkLocatorData.h \
            ../../Components/VTKSystem/IOData/CVtkLookupTableData.h \
            ../../Components/VTKSystem/IOData/CVtkMapper2DData.h \
            ../../Components/VTKSystem/IOData/CVtkMapperData.h \
            ../../Components/VTKSystem/IOData/CVtkObjectIOData.h \
            ../../Components/VTKSystem/IOData/CVtkPiecewiseFunctionData.h \
            ../../Components/VTKSystem/IOData/CVtkPointSet.h \
            ../../Components/VTKSystem/IOData/CVtkPolyData.h \
            ../../Components/VTKSystem/IOData/CVtkPolyDataMapperData.h \
            ../../Components/VTKSystem/IOData/CVtkProp3DData.h \
            ../../Components/VTKSystem/IOData/CVtkPropertyData.h \
            ../../Components/VTKSystem/IOData/CVtkRectilinearGrid.h \
            ../../Components/VTKSystem/IOData/CVtkRendererData.h \
            ../../Components/VTKSystem/IOData/CVtkRenderWindowData.h \
            ../../Components/VTKSystem/IOData/CVtkRenderWindowInteractorData.h \
            ../../Components/VTKSystem/IOData/CVtkScalarsToColorsData.h \
            ../../Components/VTKSystem/IOData/CVtkStructuredGrid.h \
            ../../Components/VTKSystem/IOData/CVtkStructuredPoints.h \
            ../../Components/VTKSystem/IOData/CVtkTextureData.h \
            ../../Components/VTKSystem/IOData/CVtkUniformGrid.h \
            ../../Components/VTKSystem/IOData/CVtkUnstructuredGrid.h 

SOURCES +=  ../../Components/VTKSystem/IOData/CVtk3DWidgetData.cpp \
            ../../Components/VTKSystem/IOData/CVtkAbstractMapper3DData.cpp \
            ../../Components/VTKSystem/IOData/CVtkAbstractMapperData.cpp \
            ../../Components/VTKSystem/IOData/CVtkAbstractPickerData.cpp \
            ../../Components/VTKSystem/IOData/CVtkAbstractTransformData.cpp \
            ../../Components/VTKSystem/IOData/CVtkActor2DData.cpp \
            ../../Components/VTKSystem/IOData/CVtkActorData.cpp \
            ../../Components/VTKSystem/IOData/CVtkAlgorithmData.cpp \
            ../../Components/VTKSystem/IOData/CVtkCameraData.cpp \
            ../../Components/VTKSystem/IOData/CVtkDataObject.cpp \
            ../../Components/VTKSystem/IOData/CVtkDataSet.cpp \
            ../../Components/VTKSystem/IOData/CVtkDataSetMapperData.cpp \
            ../../Components/VTKSystem/IOData/CVtkImageData.cpp \
            ../../Components/VTKSystem/IOData/CVtkImplicitFunctionData.cpp \
            ../../Components/VTKSystem/IOData/CVtkImporterData.cpp \
            ../../Components/VTKSystem/IOData/CVtkInteractorObserverData.cpp \
            ../../Components/VTKSystem/IOData/CVtkLocatorData.cpp \
            ../../Components/VTKSystem/IOData/CVtkLookupTableData.cpp \
            ../../Components/VTKSystem/IOData/CVtkMapper2DData.cpp \
            ../../Components/VTKSystem/IOData/CVtkMapperData.cpp \
            ../../Components/VTKSystem/IOData/CVtkObjectIOData.cpp \
            ../../Components/VTKSystem/IOData/CVtkPiecewiseFunctionData.cpp \
            ../../Components/VTKSystem/IOData/CVtkPointSet.cpp \
            ../../Components/VTKSystem/IOData/CVtkPolyData.cpp \
            ../../Components/VTKSystem/IOData/CVtkPolyDataMapperData.cpp \
            ../../Components/VTKSystem/IOData/CVtkProp3DData.cpp \
            ../../Components/VTKSystem/IOData/CVtkPropertyData.cpp \
            ../../Components/VTKSystem/IOData/CVtkRectilinearGrid.cpp \
            ../../Components/VTKSystem/IOData/CVtkRendererData.cpp \
            ../../Components/VTKSystem/IOData/CVtkRenderWindowData.cpp \
            ../../Components/VTKSystem/IOData/CVtkRenderWindowInteractorData.cpp \
            ../../Components/VTKSystem/IOData/CVtkScalarsToColorsData.cpp \
            ../../Components/VTKSystem/IOData/CVtkStructuredGrid.cpp \
            ../../Components/VTKSystem/IOData/CVtkStructuredPoints.cpp \
            ../../Components/VTKSystem/IOData/CVtkTextureData.cpp \
            ../../Components/VTKSystem/IOData/CVtkUniformGrid.cpp \
            ../../Components/VTKSystem/IOData/CVtkUnstructuredGrid.cpp 
            
