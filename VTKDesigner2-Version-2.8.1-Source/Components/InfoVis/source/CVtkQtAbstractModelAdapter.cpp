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

#include "CVtkQtAbstractModelAdapter.h"
#include "InfoVisComponent.h"

#include "IVtkTableIOData.h"
#include "CVtkQtAbstractModelAdapterData.h"
#include "vtkTable.h"
#include "vtkQtAbstractModelAdapter.h"

DEFINE_VIS_NODE(CVtkQtAbstractModelAdapter, CGenericVisNodeBase)
{
    pDesc->setNodeClassCategory("InfoVis");
    pDesc->setNodeClassName("vtkQtAbstractModelAdapter");
    pDesc->setNodeClassDescription("vtkQtAbstractModelAdapter");
    pDesc->setNodeIcon( InfoVisComponent::instance().nodeIcon() );

    // Uncomment and use the following code template to add input/output paths
    pDesc->addConnectionPath(
        new CGenericVisNodeConnectionPath(
                "ModelOutput",                              // Name of the path
                IVisSystemNodeConnectionPath::OutputPath,   // Path type can be OutputPath or InputPath
                "vtkQtAbstractModelAdapter",                // Data type of the path
                0,                                          // Path index (don't change)
                false                                       // Allow Multiple Inputs Flag
            )
        );

    pDesc->addConnectionPath(
        new CGenericVisNodeConnectionPath(
                "TableInput",                               // Name of the path
                IVisSystemNodeConnectionPath::InputPath,    // Path type can be OutputPath or InputPath
                "vtkTable",                                 // Data type of the path
                0,                                          // Path index (don't change)
                false                                       // Allow Multiple Inputs Flag
            )
        );

    pDesc->setCreateFunction(0);
}

struct CVtkQtAbstractModelAdapterPrivate
{
    vtkQtAbstractModelAdapter *qtAbstractModelAdapter;
    CVtkQtAbstractModelAdapterData outputData;
};

CVtkQtAbstractModelAdapter::CVtkQtAbstractModelAdapter()
{
    CVtkQtAbstractModelAdapter::InitializeNodeDesc();
    d = new CVtkQtAbstractModelAdapterPrivate;

    d->qtAbstractModelAdapter = 0;
}

CVtkQtAbstractModelAdapter::~CVtkQtAbstractModelAdapter()
{
    // This would be a good time to delete the backend object. If the backend object is a vtkObject
    // subclass, then you have to delete it now.
    // d->VtkObjectPointer->Delete();

    delete d;
}

bool CVtkQtAbstractModelAdapter::hasInput(IVisSystemNodeConnectionPath* path)
{
    if(!path)
        return false;

    if(path->pathName() == "TableInput")
        return d->qtAbstractModelAdapter->GetVTKDataObject() != 0;

    return CGenericVisNodeBase::hasInput(path);
}

bool CVtkQtAbstractModelAdapter::setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    if(path->pathName() == "TableInput")
    {
        IVtkTableIOData* data = 0;
        bool success = inputData->queryInterface("IVtkTableIOData", (void**)&data);
        if(success && data)
        {
            vtkTable* table = data->getVtkTable();
            d->qtAbstractModelAdapter->SetVTKDataObject(table);
            return true;
        }

        return false;
    }

    return CGenericVisNodeBase::setInput(path, inputData);
}

bool CVtkQtAbstractModelAdapter::removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    if(path->pathName() == "TableInput")
    {
        IVtkTableIOData* data = 0;
        bool success = inputData->queryInterface("IVtkTableIOData", (void**)&data);
        if(success && data &&
           data->getVtkTable() ==
            vtkTable::SafeDownCast(d->qtAbstractModelAdapter->GetVTKDataObject()))
        {
            d->qtAbstractModelAdapter->SetVTKDataObject(0);
            return true;
        }

        return false;
    }

    return CGenericVisNodeBase::removeInput(path, inputData);
}

bool CVtkQtAbstractModelAdapter::fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData)
{
    if(!path || !outputData)
        return false;

    if(path->pathName() == "ModelOutput")
    {
        d->outputData.setAbstractModelAdapter( d->qtAbstractModelAdapter );
        *outputData = &d->outputData;
        return true;
    }

    return CGenericVisNodeBase::fetchOutput(path, outputData);
}

bool CVtkQtAbstractModelAdapter::outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData)
{
    if(!path || !outputData)
        return false;

    if(path->pathName() == "ModelOutput")
        return true;

    return CGenericVisNodeBase::outputDerefed(path, outputData);
}

#ifdef ENABLE_ADVANCED_PROPERTIES

int CVtkQtAbstractModelAdapter::propertyCount()
{
    return 0;
}

QString CVtkQtAbstractModelAdapter::propertyName(int index)
{
    Q_UNUSED(index);
    return QString();
}

QString CVtkQtAbstractModelAdapter::propertyGroup(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant CVtkQtAbstractModelAdapter::propertyValue(int index)
{
    Q_UNUSED(index);
    return QVariant();
}

void CVtkQtAbstractModelAdapter::setPropertyValue(int index, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(value);
}

bool CVtkQtAbstractModelAdapter::hasEditor(int index)
{
    Q_UNUSED(index);
    return false;
}

QWidget* CVtkQtAbstractModelAdapter::createEditor(int index)
{
    Q_UNUSED(index);
    return 0;
}

void CVtkQtAbstractModelAdapter::setEditorValue(int index, QWidget* widget, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(value);
}

QVariant CVtkQtAbstractModelAdapter::editorValue(int index, QWidget* widget)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
}

void CVtkQtAbstractModelAdapter::connectEditor(int index, QWidget* widget, QObject* receiver, const char* member)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(receiver);
    Q_UNUSED(member);
}

QString CVtkQtAbstractModelAdapter::propertyValueString(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant CVtkQtAbstractModelAdapter::propertyValueFromString(int index, QString valueStr)
{
    Q_UNUSED(index);
    Q_UNUSED(valueStr);

    return QVariant();
}

bool CVtkQtAbstractModelAdapter::canLoadSaveProperty(int index)
{
    Q_UNUSED(index);

    return false;
}

#endif

void CVtkQtAbstractModelAdapter::setVtkObject(vtkObject* ptr)
{
    Q_UNUSED(ptr);
}

void CVtkQtAbstractModelAdapter::setModelAdapter(vtkQtAbstractModelAdapter* adapter)
{
    d->qtAbstractModelAdapter = adapter;
    if(d->qtAbstractModelAdapter)
        d->qtAbstractModelAdapter->setParent(this);
}






