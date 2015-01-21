/****************************************************************************
**
** Copyright (C) VCreate Logic Pvt Ltd
**
** Use of this file is limited according to the terms specified by
** VCreate Logic Pvt Ltd.
**
** Details of those terms are listed in licence.txt included as
** part of the distribution package of this file. This file may not
** be distributed without including the licence.txt file.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include "CVtkOpenGLVolumeTextureMapper3D.h"
#include "VTKVolumeRenderingSystemComponent.h"
#include "vtkOpenGLVolumeTextureMapper3D.h"

DEFINE_VIS_NODE(CVtkOpenGLVolumeTextureMapper3D, CVtkVolumeTextureMapper3D)
{
    pDesc->setNodeClassCategory("VTKVolumeRenderingSystem");
    pDesc->setNodeClassName("vtkOpenGLVolumeTextureMapper3D");
    pDesc->setNodeClassDescription("vtkOpenGLVolumeTextureMapper3D");
    pDesc->setNodeIcon( VTKVolumeRenderingSystemComponent::instance().nodeIcon() );

    // Uncomment and use the following code template to add input/output paths
    /*
    pDesc->addConnectionPath(
        new CGenericVisNodeConnectionPath(
                "PathName",                                 // Name of the path
                IVisSystemNodeConnectionPath::OutputPath,   // Path type can be OutputPath or InputPath
                "PathType",                                 // Data type of the path
                0,                                          // Path index (don't change)
                false                                       // Allow Multiple Inputs Flag
            )
        );
    */
}

struct CVtkOpenGLVolumeTextureMapper3DData
{
    vtkOpenGLVolumeTextureMapper3D *openGLVolumeTextureMapper3D;
};

CVtkOpenGLVolumeTextureMapper3D::CVtkOpenGLVolumeTextureMapper3D()
{
    CVtkOpenGLVolumeTextureMapper3D::InitializeNodeDesc();
    d = new CVtkOpenGLVolumeTextureMapper3DData;

    d->openGLVolumeTextureMapper3D = 0;
    setVtkObject( vtkOpenGLVolumeTextureMapper3D::New() );
}

CVtkOpenGLVolumeTextureMapper3D::~CVtkOpenGLVolumeTextureMapper3D()
{
    // This would be a good time to delete the backend object. If the backend object is a vtkObject
    // subclass, then you have to delete it now.
    // d->VtkObjectPointer->Delete();

    d->openGLVolumeTextureMapper3D->Delete();
    delete d;
}

bool CVtkOpenGLVolumeTextureMapper3D::hasInput(IVisSystemNodeConnectionPath* path)
{
    if(!path)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CVtkVolumeTextureMapper3D::hasInput(path);
}

bool CVtkOpenGLVolumeTextureMapper3D::setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CVtkVolumeTextureMapper3D::setInput(path, inputData);
}

bool CVtkOpenGLVolumeTextureMapper3D::removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CVtkVolumeTextureMapper3D::removeInput(path, inputData);
}

bool CVtkOpenGLVolumeTextureMapper3D::fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData)
{
    if(!path || !outputData)
        return false;

    /*
    If you have added output paths in the description block at the header of this file,
    then you will have to handle outputs here
    */

    return CVtkVolumeTextureMapper3D::fetchOutput(path, outputData);
}

bool CVtkOpenGLVolumeTextureMapper3D::outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData)
{
    if(!path || !outputData)
        return false;

    /*
    If you have added output paths in the description block at the header of this file,
    then you will have to handle outputs here
    */

    return CVtkVolumeTextureMapper3D::outputDerefed(path, outputData);
}

#ifdef ENABLE_ADVANCED_PROPERTIES

int CVtkOpenGLVolumeTextureMapper3D::propertyCount()
{
    return 0;
}

QString CVtkOpenGLVolumeTextureMapper3D::propertyName(int index)
{
    Q_UNUSED(index);
    return QString();
}

QString CVtkOpenGLVolumeTextureMapper3D::propertyGroup(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant CVtkOpenGLVolumeTextureMapper3D::propertyValue(int index)
{
    Q_UNUSED(index);
    return QVariant();
}

void CVtkOpenGLVolumeTextureMapper3D::setPropertyValue(int index, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(value);
}

bool CVtkOpenGLVolumeTextureMapper3D::hasEditor(int index)
{
    Q_UNUSED(index);
    return false;
}

QWidget* CVtkOpenGLVolumeTextureMapper3D::createEditor(int index)
{
    Q_UNUSED(index);
    return 0;
}

void CVtkOpenGLVolumeTextureMapper3D::setEditorValue(int index, QWidget* widget, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(value);
}

QVariant CVtkOpenGLVolumeTextureMapper3D::editorValue(int index, QWidget* widget)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
}

void CVtkOpenGLVolumeTextureMapper3D::connectEditor(int index, QWidget* widget, QObject* receiver, const char* member)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(receiver);
    Q_UNUSED(member);
}

QString CVtkOpenGLVolumeTextureMapper3D::propertyValueString(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant CVtkOpenGLVolumeTextureMapper3D::propertyValueFromString(int index, QString valueStr)
{
    Q_UNUSED(index);
    Q_UNUSED(valueStr);

    return QVariant();
}

bool CVtkOpenGLVolumeTextureMapper3D::canLoadSaveProperty(int index)
{
    Q_UNUSED(index);

    return false;
}

#endif

void CVtkOpenGLVolumeTextureMapper3D::setVtkObject(vtkObject* ptr)
{
    d->openGLVolumeTextureMapper3D = dynamic_cast<vtkOpenGLVolumeTextureMapper3D*>(ptr);
    d->openGLVolumeTextureMapper3D->Register(NULL);
    nodeData()["vtkObject"] = qVariantFromValue<vtkObject*>(d->openGLVolumeTextureMapper3D);
    CVtkVolumeTextureMapper3D::setVtkObject(ptr);
}





