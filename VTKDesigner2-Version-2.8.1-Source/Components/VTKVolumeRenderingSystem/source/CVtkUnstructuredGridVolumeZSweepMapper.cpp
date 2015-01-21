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

#include "CVtkUnstructuredGridVolumeZSweepMapper.h"
#include "VTKVolumeRenderingSystemComponent.h"
#include "vtkUnstructuredGridVolumeZSweepMapper.h"

DEFINE_VIS_NODE(CVtkUnstructuredGridVolumeZSweepMapper, CVtkUnstructuredGridVolumeMapper)
{
    pDesc->setNodeClassCategory("VTKVolumeRenderingSystem");
    pDesc->setNodeClassName("vtkUnstructuredGridVolumeZSweepMapper");
    pDesc->setNodeClassDescription("vtkUnstructuredGridVolumeZSweepMapper");
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

struct CVtkUnstructuredGridVolumeZSweepMapperData
{
    vtkUnstructuredGridVolumeZSweepMapper *unstructuredGridVolumeZSweepMapper;
};

CVtkUnstructuredGridVolumeZSweepMapper::CVtkUnstructuredGridVolumeZSweepMapper()
{
    CVtkUnstructuredGridVolumeZSweepMapper::InitializeNodeDesc();
    d = new CVtkUnstructuredGridVolumeZSweepMapperData;

    d->unstructuredGridVolumeZSweepMapper = 0;
    setVtkObject( vtkUnstructuredGridVolumeZSweepMapper::New() );
}

CVtkUnstructuredGridVolumeZSweepMapper::~CVtkUnstructuredGridVolumeZSweepMapper()
{
    // This would be a good time to delete the backend object. If the backend object is a vtkObject
    // subclass, then you have to delete it now.
    // d->VtkObjectPointer->Delete();
    d->unstructuredGridVolumeZSweepMapper->Delete();
    delete d;
}

bool CVtkUnstructuredGridVolumeZSweepMapper::hasInput(IVisSystemNodeConnectionPath* path)
{
    if(!path)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CVtkUnstructuredGridVolumeMapper::hasInput(path);
}

bool CVtkUnstructuredGridVolumeZSweepMapper::setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CVtkUnstructuredGridVolumeMapper::setInput(path, inputData);
}

bool CVtkUnstructuredGridVolumeZSweepMapper::removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CVtkUnstructuredGridVolumeMapper::removeInput(path, inputData);
}

bool CVtkUnstructuredGridVolumeZSweepMapper::fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData)
{
    if(!path || !outputData)
        return false;

    /*
    If you have added output paths in the description block at the header of this file,
    then you will have to handle outputs here
    */

    return CVtkUnstructuredGridVolumeMapper::fetchOutput(path, outputData);
}

bool CVtkUnstructuredGridVolumeZSweepMapper::outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData)
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

int CVtkUnstructuredGridVolumeZSweepMapper::propertyCount()
{
    return 0;
}

QString CVtkUnstructuredGridVolumeZSweepMapper::propertyName(int index)
{
    Q_UNUSED(index);
    return QString();
}

QString CVtkUnstructuredGridVolumeZSweepMapper::propertyGroup(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant CVtkUnstructuredGridVolumeZSweepMapper::propertyValue(int index)
{
    Q_UNUSED(index);
    return QVariant();
}

void CVtkUnstructuredGridVolumeZSweepMapper::setPropertyValue(int index, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(value);
}

bool CVtkUnstructuredGridVolumeZSweepMapper::hasEditor(int index)
{
    Q_UNUSED(index);
    return false;
}

QWidget* CVtkUnstructuredGridVolumeZSweepMapper::createEditor(int index)
{
    Q_UNUSED(index);
    return 0;
}

void CVtkUnstructuredGridVolumeZSweepMapper::setEditorValue(int index, QWidget* widget, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(value);
}

QVariant CVtkUnstructuredGridVolumeZSweepMapper::editorValue(int index, QWidget* widget)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
}

void CVtkUnstructuredGridVolumeZSweepMapper::connectEditor(int index, QWidget* widget, QObject* receiver, const char* member)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(receiver);
    Q_UNUSED(member);
}

QString CVtkUnstructuredGridVolumeZSweepMapper::propertyValueString(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant CVtkUnstructuredGridVolumeZSweepMapper::propertyValueFromString(int index, QString valueStr)
{
    Q_UNUSED(index);
    Q_UNUSED(valueStr);

    return QVariant();
}

bool CVtkUnstructuredGridVolumeZSweepMapper::canLoadSaveProperty(int index)
{
    Q_UNUSED(index);

    return false;
}

#endif

void CVtkUnstructuredGridVolumeZSweepMapper::setVtkObject(vtkObject* ptr)
{
    d->unstructuredGridVolumeZSweepMapper = dynamic_cast<vtkUnstructuredGridVolumeZSweepMapper*>(ptr);
    CVtkUnstructuredGridVolumeMapper::setVtkObject(ptr);
}





