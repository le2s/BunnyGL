/****************************************************************************
**
** Copyright (C) UNO and VCreate Logic
**
** Use of this file is limited according to the terms specified by
** UNO and VCreate Logic.
**
** Details of those terms are listed in licence.txt included as
** part of the distribution package of this file. This file may not
** be distributed without including the licence.txt file.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include "CVtkQtTableModelAdapter.h"
#include "InfoVisComponent.h"
#include "vtkQtTableModelAdapter.h"

DEFINE_VIS_NODE(CVtkQtTableModelAdapter, CVtkQtAbstractModelAdapter)
{
    pDesc->setNodeClassCategory("InfoVis");
    pDesc->setNodeClassName("vtkQtTableModelAdapter");
    pDesc->setNodeClassDescription("vtkQtTableModelAdapter");
    pDesc->setNodeIcon( InfoVisComponent::instance().nodeIcon() );

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

struct CVtkQtTableModelAdapterData
{
    vtkQtTableModelAdapter *qtTableModelAdapter;
};

CVtkQtTableModelAdapter::CVtkQtTableModelAdapter()
{
    CVtkQtTableModelAdapter::InitializeNodeDesc();
    d = new CVtkQtTableModelAdapterData;

    d->qtTableModelAdapter = 0;
    setModelAdapter( new vtkQtTableModelAdapter );
}

CVtkQtTableModelAdapter::~CVtkQtTableModelAdapter()
{
    // This would be a good time to delete the backend object. If the backend object is a vtkObject
    // subclass, then you have to delete it now.
    // d->VtkObjectPointer->Delete();

    // d->qtTableModelAdapter->Delete();
    delete d;
}

bool CVtkQtTableModelAdapter::hasInput(IVisSystemNodeConnectionPath* path)
{
    if(!path)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CVtkQtAbstractModelAdapter::hasInput(path);
}

bool CVtkQtTableModelAdapter::setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CVtkQtAbstractModelAdapter::setInput(path, inputData);
}

bool CVtkQtTableModelAdapter::removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CVtkQtAbstractModelAdapter::removeInput(path, inputData);
}

bool CVtkQtTableModelAdapter::fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData)
{
    if(!path || !outputData)
        return false;

    /*
    If you have added output paths in the description block at the header of this file,
    then you will have to handle outputs here
    */

    return CVtkQtAbstractModelAdapter::fetchOutput(path, outputData);
}

bool CVtkQtTableModelAdapter::outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData)
{
    if(!path || !outputData)
        return false;

    /*
    If you have added output paths in the description block at the header of this file,
    then you will have to handle outputs here
    */

    return CVtkQtAbstractModelAdapter::outputDerefed(path, outputData);
}

#ifdef ENABLE_ADVANCED_PROPERTIES

int CVtkQtTableModelAdapter::propertyCount()
{
    return 0;
}

QString CVtkQtTableModelAdapter::propertyName(int index)
{
    Q_UNUSED(index);
    return QString();
}

QString CVtkQtTableModelAdapter::propertyGroup(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant CVtkQtTableModelAdapter::propertyValue(int index)
{
    Q_UNUSED(index);
    return QVariant();
}

void CVtkQtTableModelAdapter::setPropertyValue(int index, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(value);
}

bool CVtkQtTableModelAdapter::hasEditor(int index)
{
    Q_UNUSED(index);
    return false;
}

QWidget* CVtkQtTableModelAdapter::createEditor(int index)
{
    Q_UNUSED(index);
    return 0;
}

void CVtkQtTableModelAdapter::setEditorValue(int index, QWidget* widget, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(value);
}

QVariant CVtkQtTableModelAdapter::editorValue(int index, QWidget* widget)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
}

void CVtkQtTableModelAdapter::connectEditor(int index, QWidget* widget, QObject* receiver, const char* member)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(receiver);
    Q_UNUSED(member);
}

QString CVtkQtTableModelAdapter::propertyValueString(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant CVtkQtTableModelAdapter::propertyValueFromString(int index, QString valueStr)
{
    Q_UNUSED(index);
    Q_UNUSED(valueStr);

    return QVariant();
}

bool CVtkQtTableModelAdapter::canLoadSaveProperty(int index)
{
    Q_UNUSED(index);

    return false;
}

#endif

void CVtkQtTableModelAdapter::setVtkObject(vtkObject* ptr)
{
    Q_UNUSED(ptr);
}

void CVtkQtTableModelAdapter::setModelAdapter(vtkQtAbstractModelAdapter* adapter)
{
    d->qtTableModelAdapter = qobject_cast<vtkQtTableModelAdapter*>(adapter);
    CVtkQtAbstractModelAdapter::setModelAdapter(adapter);
}




