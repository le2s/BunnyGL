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

#include "QTreeViewVisNode.h"
#include "InfoVisComponent.h"

#include <QTreeView>

DEFINE_VIS_NODE(QTreeViewVisNode, QAbstractItemViewVisNode)
{
    pDesc->setNodeClassCategory("InfoVis");
    pDesc->setNodeClassName("QTreeView");
    pDesc->setNodeClassDescription("QTreeView");
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

struct QTreeViewVisNodeData
{
    QTreeView* treeView;
};

QTreeViewVisNode::QTreeViewVisNode()
{
    QTreeViewVisNode::InitializeNodeDesc();
    d = new QTreeViewVisNodeData;

    d->treeView = new QTreeView;
    setView(d->treeView);
}

QTreeViewVisNode::~QTreeViewVisNode()
{
    // This would be a good time to delete the backend object. If the backend object is a vtkObject
    // subclass, then you have to delete it now.
    // d->VtkObjectPointer->Delete();


    delete d;
}

bool QTreeViewVisNode::hasInput(IVisSystemNodeConnectionPath* path)
{
    if(!path)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return QAbstractItemViewVisNode::hasInput(path);
}

bool QTreeViewVisNode::setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return QAbstractItemViewVisNode::setInput(path, inputData);
}

bool QTreeViewVisNode::removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return QAbstractItemViewVisNode::removeInput(path, inputData);
}

bool QTreeViewVisNode::fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData)
{
    if(!path || !outputData)
        return false;

    /*
    If you have added output paths in the description block at the header of this file,
    then you will have to handle outputs here
    */

    return QAbstractItemViewVisNode::fetchOutput(path, outputData);
}

bool QTreeViewVisNode::outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData)
{
    if(!path || !outputData)
        return false;

    /*
    If you have added output paths in the description block at the header of this file,
    then you will have to handle outputs here
    */

    return QAbstractItemViewVisNode::outputDerefed(path, outputData);
}

#ifdef ENABLE_ADVANCED_PROPERTIES

int QTreeViewVisNode::propertyCount()
{
    return 0;
}

QString QTreeViewVisNode::propertyName(int index)
{
    Q_UNUSED(index);
    return QString();
}

QString QTreeViewVisNode::propertyGroup(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant QTreeViewVisNode::propertyValue(int index)
{
    Q_UNUSED(index);
    return QVariant();
}

void QTreeViewVisNode::setPropertyValue(int index, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(value);
}

bool QTreeViewVisNode::hasEditor(int index)
{
    Q_UNUSED(index);
    return false;
}

QWidget* QTreeViewVisNode::createEditor(int index)
{
    Q_UNUSED(index);
    return 0;
}

void QTreeViewVisNode::setEditorValue(int index, QWidget* widget, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(value);
}

QVariant QTreeViewVisNode::editorValue(int index, QWidget* widget)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
}

void QTreeViewVisNode::connectEditor(int index, QWidget* widget, QObject* receiver, const char* member)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(receiver);
    Q_UNUSED(member);
}

QString QTreeViewVisNode::propertyValueString(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant QTreeViewVisNode::propertyValueFromString(int index, QString valueStr)
{
    Q_UNUSED(index);
    Q_UNUSED(valueStr);

    return QVariant();
}

bool QTreeViewVisNode::canLoadSaveProperty(int index)
{
    Q_UNUSED(index);

    return false;
}

#endif





