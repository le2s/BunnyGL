/****************************************************************************
**
** Copyright (C) VCreate Logic Private Limited, Bangalore
**
** Use of this file is limited according to the terms specified by
** VCreate Logic Private Limited, Bangalore. Details of those terms
** are listed in licence.txt included as part of the distribution package
** of this file. This file may not be distributed without including the
** licence.txt file.
**
** Contact info@vcreatelogic.com if any conditions of this licensing are
** not clear to you.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include "CVtkUnstructuredGridVolumeRayCastMapper.h"
#include "VTKVolumeRenderingSystemComponent.h"
#include "vtkUnstructuredGridVolumeRayCastMapper.h"

DEFINE_VIS_NODE(CVtkUnstructuredGridVolumeRayCastMapper, CVtkUnstructuredGridVolumeMapper)
{
    pDesc->setNodeClassCategory("VTKVolumeRenderingSystem");
    pDesc->setNodeClassName("vtkUnstructuredGridVolumeRayCastMapper");
    pDesc->setNodeClassDescription("vtkUnstructuredGridVolumeRayCastMapper");
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

struct CVtkUnstructuredGridVolumeRayCastMapperData
{
    vtkUnstructuredGridVolumeRayCastMapper *unstructuredGridVolumeRayCastMapper;
};

CVtkUnstructuredGridVolumeRayCastMapper::CVtkUnstructuredGridVolumeRayCastMapper()
{
    CVtkUnstructuredGridVolumeRayCastMapper::InitializeNodeDesc();
    d = new CVtkUnstructuredGridVolumeRayCastMapperData;

    d->unstructuredGridVolumeRayCastMapper = 0;
    setVtkObject( vtkUnstructuredGridVolumeRayCastMapper::New() );
}

CVtkUnstructuredGridVolumeRayCastMapper::~CVtkUnstructuredGridVolumeRayCastMapper()
{
    // This would be a good time to delete the backend object. If the backend object is a vtkObject
    // subclass, then you have to delete it now.
    // d->VtkObjectPointer->Delete();
    d->unstructuredGridVolumeRayCastMapper->Delete();
    delete d;
}

bool CVtkUnstructuredGridVolumeRayCastMapper::hasInput(IVisSystemNodeConnectionPath* path)
{
    if(!path)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CVtkUnstructuredGridVolumeMapper::hasInput(path);
}

bool CVtkUnstructuredGridVolumeRayCastMapper::setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CVtkUnstructuredGridVolumeMapper::setInput(path, inputData);
}

bool CVtkUnstructuredGridVolumeRayCastMapper::removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CVtkUnstructuredGridVolumeMapper::removeInput(path, inputData);
}

bool CVtkUnstructuredGridVolumeRayCastMapper::fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData)
{
    if(!path || !outputData)
        return false;

    /*
    If you have added output paths in the description block at the header of this file,
    then you will have to handle outputs here
    */

    return CVtkUnstructuredGridVolumeMapper::fetchOutput(path, outputData);
}

bool CVtkUnstructuredGridVolumeRayCastMapper::outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData)
{
    if(!path || !outputData)
        return false;

    /*
    If you have added output paths in the description block at the header of this file,
    then you will have to handle outputs here
    */

    return CVtkUnstructuredGridVolumeMapper::outputDerefed(path, outputData);
}

#ifdef ENABLE_ADVANCED_PROPERTIES

int CVtkUnstructuredGridVolumeRayCastMapper::propertyCount()
{
    return 0;
}

QString CVtkUnstructuredGridVolumeRayCastMapper::propertyName(int index)
{
    Q_UNUSED(index);
    return QString();
}

QString CVtkUnstructuredGridVolumeRayCastMapper::propertyGroup(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant CVtkUnstructuredGridVolumeRayCastMapper::propertyValue(int index)
{
    Q_UNUSED(index);
    return QVariant();
}

void CVtkUnstructuredGridVolumeRayCastMapper::setPropertyValue(int index, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(value);
}

bool CVtkUnstructuredGridVolumeRayCastMapper::hasEditor(int index)
{
    Q_UNUSED(index);
    return false;
}

QWidget* CVtkUnstructuredGridVolumeRayCastMapper::createEditor(int index)
{
    Q_UNUSED(index);
    return 0;
}

void CVtkUnstructuredGridVolumeRayCastMapper::setEditorValue(int index, QWidget* widget, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(value);
}

QVariant CVtkUnstructuredGridVolumeRayCastMapper::editorValue(int index, QWidget* widget)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
}

void CVtkUnstructuredGridVolumeRayCastMapper::connectEditor(int index, QWidget* widget, QObject* receiver, const char* member)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(receiver);
    Q_UNUSED(member);
}

QString CVtkUnstructuredGridVolumeRayCastMapper::propertyValueString(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant CVtkUnstructuredGridVolumeRayCastMapper::propertyValueFromString(int index, QString valueStr)
{
    Q_UNUSED(index);
    Q_UNUSED(valueStr);

    return QVariant();
}

bool CVtkUnstructuredGridVolumeRayCastMapper::canLoadSaveProperty(int index)
{
    Q_UNUSED(index);

    return false;
}

#endif

void CVtkUnstructuredGridVolumeRayCastMapper::setVtkObject(vtkObject* ptr)
{
    d->unstructuredGridVolumeRayCastMapper = dynamic_cast<vtkUnstructuredGridVolumeRayCastMapper*>(ptr);
    CVtkUnstructuredGridVolumeMapper::setVtkObject(ptr);
}





