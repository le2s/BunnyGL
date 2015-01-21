PROJECT(vtkQtGraphicsViewRenderWindowTest)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

FIND_PACKAGE(Qt4
             COMPONENTS QtCore QtGui QtOpenGl
             REQUIRED)
IF(QT4_FOUND)
    # (QT_USE_FILE is a variable defined by FIND_PACKAGE( Qt4 )·
    # that contains a path to CMake script)
    INCLUDE( ${QT_USE_FILE} )
ELSE(QT4_FOUND)
    MESSAGE(FATAL_ERROR
    "Cannot build without Qt4.  Please set Qt4_DIR.")
ENDIF(QT4_FOUND)

FIND_PACKAGE(VTK)

IF(VTK_FOUND)
    INCLUDE( ${VTK_USE_FILE} )
ELSE(VTK_FOUND)
    MESSAGE(FATAL_ERROR
    "Cannot build without VTK.")

ENDIF(VTK_FOUND)

SET(VTK_LIBRARIES
    vtkCommon
    vtkGraphics
    vtkFiltering
    vtkGenericFiltering
    vtkRendering
    vtkVolumeRendering
    vtkImaging
    vtkIO
    QVTK
    vtkWidgets
    vtkHybrid
)

INCLUDE_DIRECTORIES(. /usr/local/include/vtk-5.4 $ENV{QTDIR}/src/opengl)

SET(MY_SOURCE
    vtkQtGraphicsViewRenderWindow.cpp
    vtkFractalTerrainSource.cpp
    Main.cpp)
    
QT4_WRAP_CPP(MY_MOC vtkQtGraphicsViewRenderWindow.h)
    
ADD_EXECUTABLE(vtkQtGraphicsViewRenderWindowTest ${MY_SOURCE} ${MY_MOC})
TARGET_LINK_LIBRARIES(vtkQtGraphicsViewRenderWindowTest ${VTK_LIBRARIES}  ${QT_LIBRARIES})
