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

#include "QAbstractItemViewVisNode.h"
#include "InfoVisComponent.h"

#include <QAbstractItemView>
#include "IVtkQtAbstractModelAdapterIOData.h"
#include "vtkQtAbstractModelAdapter.h"

#include <QDialog>
#include <QMainWindow>
#include <QHBoxLayout>
#include <GCF/AbstractComponent>
#include <QMessageBox>

#include <QListView>
#include <QTableView>
#include <QColumnView>
#include <QTreeView>

#include <QMetaObject>
#include <QMetaProperty>

DEFINE_VIS_NODE(QAbstractItemViewVisNode, CGenericVisNodeBase)
{
    pDesc->setNodeClassCategory("InfoVis");
    pDesc->setNodeClassName("QAbstractItemView");
    pDesc->setNodeClassDescription("QAbstractItemView");
    pDesc->setNodeIcon( InfoVisComponent::instance().nodeIcon() );

    // Uncomment and use the following code template to add input/output paths
    pDesc->addConnectionPath(
        new CGenericVisNodeConnectionPath(
                "ModelAdapterInput",                        // Name of the path
                IVisSystemNodeConnectionPath::InputPath,    // Path type can be OutputPath or InputPath
                "vtkQtAbstractModelAdapter",                // Data type of the path
                0,                                          // Path index (don't change)
                false                                       // Allow Multiple Inputs Flag
            )
        );

    pDesc->setCreateFunction(0);
}

struct QAbstractItemViewVisNodeData
{
    QAbstractItemView* itemView;
};

QAbstractItemViewVisNode::QAbstractItemViewVisNode()
{
    QAbstractItemViewVisNode::InitializeNodeDesc();
    d = new QAbstractItemViewVisNodeData;

    d->itemView = 0;
}

QAbstractItemViewVisNode::~QAbstractItemViewVisNode()
{
    // This would be a good time to delete the backend object. If the backend object is a vtkObject
    // subclass, then you have to delete it now.
    // d->VtkObjectPointer->Delete();
    if(d->itemView)
        delete d->itemView;

    delete d;
}

void QAbstractItemViewVisNode::setView(QAbstractItemView* view)
{
    d->itemView = view;
}

QAbstractItemView* QAbstractItemViewVisNode::view() const
{
    return d->itemView;
}

bool QAbstractItemViewVisNode::hasInput(IVisSystemNodeConnectionPath* path)
{
    if(!path)
        return false;

    if(path->pathName() == "ModelAdapterInput")
        return d->itemView && d->itemView->model();

    return CGenericVisNodeBase::hasInput(path);
}

bool QAbstractItemViewVisNode::setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    if(path->pathName() == "ModelAdapterInput")
    {
        if(!d->itemView)
            return false;

        IVtkQtAbstractModelAdapterIOData* data = 0;
        bool success = inputData->queryInterface("IVtkQtAbstractModelAdapterIOData", (void**)&data);
        if(success && data)
        {
            vtkQtAbstractModelAdapter* adapter = data->getVtkQtAbstractModelAdapter();
            d->itemView->setModel( adapter );
            return true;
        }

        return false;
    }

    return CGenericVisNodeBase::setInput(path, inputData);
}

bool QAbstractItemViewVisNode::removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    if(path->pathName() == "ModelAdapterInput")
    {
        if(!d->itemView)
            return false;

        IVtkQtAbstractModelAdapterIOData* data = 0;
        bool success = inputData->queryInterface("IVtkQtAbstractModelAdapterIOData", (void**)&data);
        vtkQtAbstractModelAdapter* adapter = 0;
        if(success && data)
            adapter = data->getVtkQtAbstractModelAdapter();

        if(d->itemView->model() == adapter)
        {
            d->itemView->setModel(0);
            return true;
        }

        return false;
    }

    return CGenericVisNodeBase::removeInput(path, inputData);
}

bool QAbstractItemViewVisNode::fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData)
{
    if(!path || !outputData)
        return false;

    /*
    If you have added output paths in the description block at the header of this file,
    then you will have to handle outputs here
    */

    return CGenericVisNodeBase::fetchOutput(path, outputData);
}

bool QAbstractItemViewVisNode::outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData)
{
    if(!path || !outputData)
        return false;

    /*
    If you have added output paths in the description block at the header of this file,
    then you will have to handle outputs here
    */

    return CGenericVisNodeBase::outputDerefed(path, outputData);
}

#ifdef ENABLE_ADVANCED_PROPERTIES

int QAbstractItemViewVisNode::propertyCount()
{
    return 0;
}

QString QAbstractItemViewVisNode::propertyName(int index)
{
    Q_UNUSED(index);
    return QString();
}

QString QAbstractItemViewVisNode::propertyGroup(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant QAbstractItemViewVisNode::propertyValue(int index)
{
    Q_UNUSED(index);
    return QVariant();
}

void QAbstractItemViewVisNode::setPropertyValue(int index, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(value);
}

bool QAbstractItemViewVisNode::hasEditor(int index)
{
    Q_UNUSED(index);
    return false;
}

QWidget* QAbstractItemViewVisNode::createEditor(int index)
{
    Q_UNUSED(index);
    return 0;
}

void QAbstractItemViewVisNode::setEditorValue(int index, QWidget* widget, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(value);
}

QVariant QAbstractItemViewVisNode::editorValue(int index, QWidget* widget)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
}

void QAbstractItemViewVisNode::connectEditor(int index, QWidget* widget, QObject* receiver, const char* member)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(receiver);
    Q_UNUSED(member);
}

QString QAbstractItemViewVisNode::propertyValueString(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant QAbstractItemViewVisNode::propertyValueFromString(int index, QString valueStr)
{
    Q_UNUSED(index);
    Q_UNUSED(valueStr);

    return QVariant();
}

bool QAbstractItemViewVisNode::canLoadSaveProperty(int index)
{
    Q_UNUSED(index);

    return false;
}

#endif

void QAbstractItemViewVisNode::showView(QDialog* dialog, QLayout* layout)
{
    QAbstractItemView* newView = 0;

    // Create a new view widget
    if( qobject_cast<QTableView*>(d->itemView) )
        newView = new QTableView(dialog);
    else if( qobject_cast<QColumnView*>(d->itemView) )
        newView = new QColumnView(dialog);
    else if( qobject_cast<QListView*>(d->itemView) )
        newView = new QListView(dialog);
    else if( qobject_cast<QTreeView*>(d->itemView) )
        newView = new QTreeView(dialog);
    
    if(!newView)
        return;

    // Set the same model as the existing view
    newView->setModel( d->itemView->model() );

    // Share the selection model as well
    newView->setSelectionModel( d->itemView->selectionModel() );

    // Copy properties from the old view to the new view
    const QMetaObject* mo = d->itemView->metaObject();
    for(int i=mo->propertyCount()-1; i>=0; i--)
    {
        QMetaProperty prop = mo->property(i);
        QVariant propValue = prop.read(d->itemView);
        prop.write(newView, propValue);
    }

    // Show the new view in the dialog
    layout->addWidget(newView);
    newView->show();

    Q_UNUSED(dialog);
}

void QAbstractItemViewVisNode::command_ShowView()
{
    if(!d->itemView)
    {
        QMessageBox::information(GCF::AbstractComponent::mainWindow(),
            "No ItemView", "No item-view widget associated with this node");
        return;
    }

    // Create a dialog box
    QDialog dialog( GCF::AbstractComponent::mainWindow() );
    QHBoxLayout* layout = new QHBoxLayout(&dialog);

    // Request the subclass to load the view widget into the dialog box
    this->showView(&dialog, layout);

    // Show the dialog box to the user
    dialog.setWindowTitle(this->nodeName() + " - Item View");
    dialog.resize(640, 480);
    dialog.exec();
}




