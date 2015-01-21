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

#include "CVtkQtTreeModelAdapter.h"
#include "InfoVisComponent.h"
#include "vtkQtTreeModelAdapter.h"

DEFINE_VIS_NODE(CVtkQtTreeModelAdapter, CVtkQtAbstractModelAdapter)
{
    pDesc->setNodeClassCategory("InfoVis");
    pDesc->setNodeClassName("vtkQtTreeModelAdapter");
    pDesc->setNodeClassDescription("vtkQtTreeModelAdapter");
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

struct CVtkQtTreeModelAdapterData
{
    vtkQtTreeModelAdapter *qtTreeModelAdapter;
};

CVtkQtTreeModelAdapter::CVtkQtTreeModelAdapter()
{
    CVtkQtTreeModelAdapter::InitializeNodeDesc();
    d = new CVtkQtTreeModelAdapterData;

    d->qtTreeModelAdapter = 0;
    setModelAdapter( new vtkQtTreeModelAdapter );
}

CVtkQtTreeModelAdapter::~CVtkQtTreeModelAdapter()
{
    // This would be a good time to delete the backend object. If the backend object is a vtkObject
    // subclass, then you have to delete it now.
    // d->VtkObjectPointer->Delete();

    // d->qtTreeModelAdapter->Delete();
    delete d;
}

bool CVtkQtTreeModelAdapter::hasInput(IVisSystemNodeConnectionPath* path)
{
    if(!path)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CVtkQtAbstractModelAdapter::hasInput(path);
}

bool CVtkQtTreeModelAdapter::setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CVtkQtAbstractModelAdapter::setInput(path, inputData);
}

bool CVtkQtTreeModelAdapter::removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CVtkQtAbstractModelAdapter::removeInput(path, inputData);
}

bool CVtkQtTreeModelAdapter::fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData)
{
    if(!path || !outputData)
        return false;

    /*
    If you have added output paths in the description block at the header of this file,
    then you will have to handle outputs here
    */

    return CVtkQtAbstractModelAdapter::fetchOutput(path, outputData);
}

bool CVtkQtTreeModelAdapter::outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData)
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

int CVtkQtTreeModelAdapter::propertyCount()
{
    return 0;
}

QString CVtkQtTreeModelAdapter::propertyName(int index)
{
    Q_UNUSED(index);
    return QString();
}

QString CVtkQtTreeModelAdapter::propertyGroup(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant CVtkQtTreeModelAdapter::propertyValue(int index)
{
    Q_UNUSED(index);
    return QVariant();
}

void CVtkQtTreeModelAdapter::setPropertyValue(int index, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(value);
}

bool CVtkQtTreeModelAdapter::hasEditor(int index)
{
    Q_UNUSED(index);
    return false;
}

QWidget* CVtkQtTreeModelAdapter::createEditor(int index)
{
    Q_UNUSED(index);
    return 0;
}

void CVtkQtTreeModelAdapter::setEditorValue(int index, QWidget* widget, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(value);
}

QVariant CVtkQtTreeModelAdapter::editorValue(int index, QWidget* widget)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
}

void CVtkQtTreeModelAdapter::connectEditor(int index, QWidget* widget, QObject* receiver, const char* member)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(receiver);
    Q_UNUSED(member);
}

QString CVtkQtTreeModelAdapter::propertyValueString(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant CVtkQtTreeModelAdapter::propertyValueFromString(int index, QString valueStr)
{
    Q_UNUSED(index);
    Q_UNUSED(valueStr);

    return QVariant();
}

bool CVtkQtTreeModelAdapter::canLoadSaveProperty(int index)
{
    Q_UNUSED(index);

    return false;
}

#endif

void CVtkQtTreeModelAdapter::setVtkObject(vtkObject* ptr)
{
    Q_UNUSED(ptr);
}

void CVtkQtTreeModelAdapter::setModelAdapter(vtkQtAbstractModelAdapter* adapter)
{
    d->qtTreeModelAdapter = qobject_cast<vtkQtTreeModelAdapter*>(adapter);
    CVtkQtAbstractModelAdapter::setModelAdapter(adapter);
}




