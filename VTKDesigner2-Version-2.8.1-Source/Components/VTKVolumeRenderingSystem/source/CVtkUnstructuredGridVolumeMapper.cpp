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

#include "CVtkUnstructuredGridVolumeMapper.h"
#include "VTKVolumeRenderingSystemComponent.h"
#include "vtkUnstructuredGridVolumeMapper.h"

DEFINE_VIS_NODE(CVtkUnstructuredGridVolumeMapper, CVtkAbstractVolumeMapper)
{
    pDesc->setNodeClassCategory("VTKVolumeRenderingSystem");
    pDesc->setNodeClassName("vtkUnstructuredGridVolumeMapper");
    pDesc->setNodeClassDescription("vtkUnstructuredGridVolumeMapper");
    pDesc->setNodeIcon( VTKVolumeRenderingSystemComponent::instance().nodeIcon() );

    pDesc->setPropertyInfo("BlendMode", "SetBlendMode", "GetBlendMode", QStringList() << "int", QStringList(), "void", "int");
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

    pDesc->setCreateFunction(0);
}

struct CVtkUnstructuredGridVolumeMapperData
{
    vtkUnstructuredGridVolumeMapper *unstructuredGridVolumeMapper;
};

CVtkUnstructuredGridVolumeMapper::CVtkUnstructuredGridVolumeMapper()
{
    CVtkUnstructuredGridVolumeMapper::InitializeNodeDesc();
    d = new CVtkUnstructuredGridVolumeMapperData;

    d->unstructuredGridVolumeMapper = 0;
}

CVtkUnstructuredGridVolumeMapper::~CVtkUnstructuredGridVolumeMapper()
{
    // This would be a good time to delete the backend object. If the backend object is a vtkObject
    // subclass, then you have to delete it now.
    // d->VtkObjectPointer->Delete();

    delete d;
}

void CVtkUnstructuredGridVolumeMapper::setBlendMode(CVtkUnstructuredGridVolumeMapper::BlendMode mode)
{
    d->unstructuredGridVolumeMapper->SetBlendMode( int(mode) );
}

CVtkUnstructuredGridVolumeMapper::BlendMode CVtkUnstructuredGridVolumeMapper::blendMode() const
{
    return CVtkUnstructuredGridVolumeMapper::BlendMode(
                d->unstructuredGridVolumeMapper->GetBlendMode()
            );
}

bool CVtkUnstructuredGridVolumeMapper::hasInput(IVisSystemNodeConnectionPath* path)
{
    if(!path)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CVtkAbstractVolumeMapper::hasInput(path);
}

bool CVtkUnstructuredGridVolumeMapper::setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CVtkAbstractVolumeMapper::setInput(path, inputData);
}

bool CVtkUnstructuredGridVolumeMapper::removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CVtkAbstractVolumeMapper::removeInput(path, inputData);
}

bool CVtkUnstructuredGridVolumeMapper::fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData)
{
    if(!path || !outputData)
        return false;

    /*
    If you have added output paths in the description block at the header of this file,
    then you will have to handle outputs here
    */

    return CVtkAbstractVolumeMapper::fetchOutput(path, outputData);
}

bool CVtkUnstructuredGridVolumeMapper::outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData)
{
    if(!path || !outputData)
        return false;

    /*
    If you have added output paths in the description block at the header of this file,
    then you will have to handle outputs here
    */

    return CVtkAbstractVolumeMapper::outputDerefed(path, outputData);
}

#ifdef ENABLE_ADVANCED_PROPERTIES

int CVtkUnstructuredGridVolumeMapper::propertyCount()
{
    return 0;
}

QString CVtkUnstructuredGridVolumeMapper::propertyName(int index)
{
    Q_UNUSED(index);
    return QString();
}

QString CVtkUnstructuredGridVolumeMapper::propertyGroup(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant CVtkUnstructuredGridVolumeMapper::propertyValue(int index)
{
    Q_UNUSED(index);
    return QVariant();
}

void CVtkUnstructuredGridVolumeMapper::setPropertyValue(int index, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(value);
}

bool CVtkUnstructuredGridVolumeMapper::hasEditor(int index)
{
    Q_UNUSED(index);
    return false;
}

QWidget* CVtkUnstructuredGridVolumeMapper::createEditor(int index)
{
    Q_UNUSED(index);
    return 0;
}

void CVtkUnstructuredGridVolumeMapper::setEditorValue(int index, QWidget* widget, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(value);
}

QVariant CVtkUnstructuredGridVolumeMapper::editorValue(int index, QWidget* widget)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
}

void CVtkUnstructuredGridVolumeMapper::connectEditor(int index, QWidget* widget, QObject* receiver, const char* member)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(receiver);
    Q_UNUSED(member);
}

QString CVtkUnstructuredGridVolumeMapper::propertyValueString(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant CVtkUnstructuredGridVolumeMapper::propertyValueFromString(int index, QString valueStr)
{
    Q_UNUSED(index);
    Q_UNUSED(valueStr);

    return QVariant();
}

bool CVtkUnstructuredGridVolumeMapper::canLoadSaveProperty(int index)
{
    Q_UNUSED(index);

    return false;
}

#endif

void CVtkUnstructuredGridVolumeMapper::setVtkObject(vtkObject* ptr)
{
    d->unstructuredGridVolumeMapper = dynamic_cast<vtkUnstructuredGridVolumeMapper*>(ptr);
    CVtkAbstractVolumeMapper::setVtkObject(ptr);
}





