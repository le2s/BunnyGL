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

#include "CVtkFixedPointVolumeRayCastMapper.h"
#include "VTKVolumeRenderingSystemComponent.h"
#include "vtkFixedPointVolumeRayCastMapper.h"

DEFINE_VIS_NODE(CVtkFixedPointVolumeRayCastMapper, CVtkVolumeMapper)
{
    pDesc->setNodeClassCategory("VTKVolumeRenderingSystem");
    pDesc->setNodeClassName("vtkFixedPointVolumeRayCastMapper");
    pDesc->setNodeClassDescription("vtkFixedPointVolumeRayCastMapper");
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

struct CVtkFixedPointVolumeRayCastMapperData
{
    vtkFixedPointVolumeRayCastMapper *fixedPointVolumeRayCastMapper;
};

CVtkFixedPointVolumeRayCastMapper::CVtkFixedPointVolumeRayCastMapper()
{
    CVtkFixedPointVolumeRayCastMapper::InitializeNodeDesc();
    d = new CVtkFixedPointVolumeRayCastMapperData;

    d->fixedPointVolumeRayCastMapper = 0;
    setVtkObject( vtkFixedPointVolumeRayCastMapper::New() );
}

CVtkFixedPointVolumeRayCastMapper::~CVtkFixedPointVolumeRayCastMapper()
{
    // This would be a good time to delete the backend object. If the backend object is a vtkObject
    // subclass, then you have to delete it now.
    // d->VtkObjectPointer->Delete();
    d->fixedPointVolumeRayCastMapper->Delete();
    delete d;
}

bool CVtkFixedPointVolumeRayCastMapper::hasInput(IVisSystemNodeConnectionPath* path)
{
    if(!path)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CVtkVolumeMapper::hasInput(path);
}

bool CVtkFixedPointVolumeRayCastMapper::setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CVtkVolumeMapper::setInput(path, inputData);
}

bool CVtkFixedPointVolumeRayCastMapper::removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CVtkVolumeMapper::removeInput(path, inputData);
}

bool CVtkFixedPointVolumeRayCastMapper::fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData)
{
    if(!path || !outputData)
        return false;

    /*
    If you have added output paths in the description block at the header of this file,
    then you will have to handle outputs here
    */

    return CVtkVolumeMapper::fetchOutput(path, outputData);
}

bool CVtkFixedPointVolumeRayCastMapper::outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData)
{
    if(!path || !outputData)
        return false;

    /*
    If you have added output paths in the description block at the header of this file,
    then you will have to handle outputs here
    */

    return CVtkVolumeMapper::outputDerefed(path, outputData);
}

#ifdef ENABLE_ADVANCED_PROPERTIES

int CVtkFixedPointVolumeRayCastMapper::propertyCount()
{
    return 0;
}

QString CVtkFixedPointVolumeRayCastMapper::propertyName(int index)
{
    Q_UNUSED(index);
    return QString();
}

QString CVtkFixedPointVolumeRayCastMapper::propertyGroup(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant CVtkFixedPointVolumeRayCastMapper::propertyValue(int index)
{
    Q_UNUSED(index);
    return QVariant();
}

void CVtkFixedPointVolumeRayCastMapper::setPropertyValue(int index, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(value);
}

bool CVtkFixedPointVolumeRayCastMapper::hasEditor(int index)
{
    Q_UNUSED(index);
    return false;
}

QWidget* CVtkFixedPointVolumeRayCastMapper::createEditor(int index)
{
    Q_UNUSED(index);
    return 0;
}

void CVtkFixedPointVolumeRayCastMapper::setEditorValue(int index, QWidget* widget, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(value);
}

QVariant CVtkFixedPointVolumeRayCastMapper::editorValue(int index, QWidget* widget)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
}

void CVtkFixedPointVolumeRayCastMapper::connectEditor(int index, QWidget* widget, QObject* receiver, const char* member)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(receiver);
    Q_UNUSED(member);
}

QString CVtkFixedPointVolumeRayCastMapper::propertyValueString(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant CVtkFixedPointVolumeRayCastMapper::propertyValueFromString(int index, QString valueStr)
{
    Q_UNUSED(index);
    Q_UNUSED(valueStr);

    return QVariant();
}

bool CVtkFixedPointVolumeRayCastMapper::canLoadSaveProperty(int index)
{
    Q_UNUSED(index);

    return false;
}

#endif

void CVtkFixedPointVolumeRayCastMapper::setVtkObject(vtkObject* ptr)
{
    d->fixedPointVolumeRayCastMapper = dynamic_cast<vtkFixedPointVolumeRayCastMapper*>(ptr);
    CVtkVolumeMapper::setVtkObject(ptr);
}





