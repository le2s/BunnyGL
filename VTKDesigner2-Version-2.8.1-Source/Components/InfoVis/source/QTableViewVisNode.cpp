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

#include "QTableViewVisNode.h"
#include "InfoVisComponent.h"

#include <QTableView>
#include <QHeaderView>

DEFINE_VIS_NODE(QTableViewVisNode, QAbstractItemViewVisNode)
{
    pDesc->setNodeClassCategory("InfoVis");
    pDesc->setNodeClassName("QTableView");
    pDesc->setNodeClassDescription("QTableView");
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

struct QTableViewVisNodeData
{
    QTableView* tableView;
};

QTableViewVisNode::QTableViewVisNode()
{
    QTableViewVisNode::InitializeNodeDesc();
    d = new QTableViewVisNodeData;

    d->tableView = new QTableView;
    setView(d->tableView);
}

QTableViewVisNode::~QTableViewVisNode()
{
    // This would be a good time to delete the backend object. If the backend object is a vtkObject
    // subclass, then you have to delete it now.
    // d->VtkObjectPointer->Delete();


    delete d;
}

void QTableViewVisNode::setVerticalHeaderVisible(bool val)
{
    d->tableView->verticalHeader()->setVisible(val);
}

bool QTableViewVisNode::isVerticalHeaderVisible() const
{
    return d->tableView->verticalHeader()->isVisible();
}

void QTableViewVisNode::setHorizontalHeaderVisible(bool val)
{
    d->tableView->horizontalHeader()->setVisible(val);
}

bool QTableViewVisNode::isHorizontalHeaderVisible() const
{
    return d->tableView->horizontalHeader()->isVisible();
}

void QTableViewVisNode::setGridVisible(bool val)
{
    d->tableView->setShowGrid(val);
}

bool QTableViewVisNode::isGridVisible() const
{
    return d->tableView->showGrid();
}

void QTableViewVisNode::setSortingEnabled(bool val)
{
    d->tableView->setSortingEnabled(val);
}

bool QTableViewVisNode::isSortingEnabled() const
{
    return d->tableView->isSortingEnabled();
}

void QTableViewVisNode::setWordWrapEnabled(bool val)
{
    d->tableView->setWordWrap(val);
}

bool QTableViewVisNode::isWordWrapEnabled() const
{
    return d->tableView->wordWrap();
}

bool QTableViewVisNode::hasInput(IVisSystemNodeConnectionPath* path)
{
    if(!path)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return QAbstractItemViewVisNode::hasInput(path);
}

bool QTableViewVisNode::setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return QAbstractItemViewVisNode::setInput(path, inputData);
}

bool QTableViewVisNode::removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return QAbstractItemViewVisNode::removeInput(path, inputData);
}

bool QTableViewVisNode::fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData)
{
    if(!path || !outputData)
        return false;

    /*
    If you have added output paths in the description block at the header of this file,
    then you will have to handle outputs here
    */

    return QAbstractItemViewVisNode::fetchOutput(path, outputData);
}

bool QTableViewVisNode::outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData)
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

int QTableViewVisNode::propertyCount()
{
    return 0;
}

QString QTableViewVisNode::propertyName(int index)
{
    Q_UNUSED(index);
    return QString();
}

QString QTableViewVisNode::propertyGroup(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant QTableViewVisNode::propertyValue(int index)
{
    Q_UNUSED(index);
    return QVariant();
}

void QTableViewVisNode::setPropertyValue(int index, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(value);
}

bool QTableViewVisNode::hasEditor(int index)
{
    Q_UNUSED(index);
    return false;
}

QWidget* QTableViewVisNode::createEditor(int index)
{
    Q_UNUSED(index);
    return 0;
}

void QTableViewVisNode::setEditorValue(int index, QWidget* widget, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(value);
}

QVariant QTableViewVisNode::editorValue(int index, QWidget* widget)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
}

void QTableViewVisNode::connectEditor(int index, QWidget* widget, QObject* receiver, const char* member)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(receiver);
    Q_UNUSED(member);
}

QString QTableViewVisNode::propertyValueString(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant QTableViewVisNode::propertyValueFromString(int index, QString valueStr)
{
    Q_UNUSED(index);
    Q_UNUSED(valueStr);

    return QVariant();
}

bool QTableViewVisNode::canLoadSaveProperty(int index)
{
    Q_UNUSED(index);

    return false;
}

#endif





