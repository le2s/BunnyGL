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

#include "QColumnViewVisNode.h"
#include "InfoVisComponent.h"

#include <QColumnView>

DEFINE_VIS_NODE(QColumnViewVisNode, QAbstractItemViewVisNode)
{
    pDesc->setNodeClassCategory("InfoVis");
    pDesc->setNodeClassName("QColumnView");
    pDesc->setNodeClassDescription("QColumnView");
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

struct QColumnViewVisNodeData
{
    QColumnView* columnView;
};

QColumnViewVisNode::QColumnViewVisNode()
{
    QColumnViewVisNode::InitializeNodeDesc();
    d = new QColumnViewVisNodeData;

    d->columnView = new QColumnView;
    setView(d->columnView);
}

QColumnViewVisNode::~QColumnViewVisNode()
{
    // This would be a good time to delete the backend object. If the backend object is a vtkObject
    // subclass, then you have to delete it now.
    // d->VtkObjectPointer->Delete();


    delete d;
}

bool QColumnViewVisNode::hasInput(IVisSystemNodeConnectionPath* path)
{
    if(!path)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return QAbstractItemViewVisNode::hasInput(path);
}

bool QColumnViewVisNode::setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return QAbstractItemViewVisNode::setInput(path, inputData);
}

bool QColumnViewVisNode::removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return QAbstractItemViewVisNode::removeInput(path, inputData);
}

bool QColumnViewVisNode::fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData)
{
    if(!path || !outputData)
        return false;

    /*
    If you have added output paths in the description block at the header of this file,
    then you will have to handle outputs here
    */

    return QAbstractItemViewVisNode::fetchOutput(path, outputData);
}

bool QColumnViewVisNode::outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData)
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

int QColumnViewVisNode::propertyCount()
{
    return 0;
}

QString QColumnViewVisNode::propertyName(int index)
{
    Q_UNUSED(index);
    return QString();
}

QString QColumnViewVisNode::propertyGroup(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant QColumnViewVisNode::propertyValue(int index)
{
    Q_UNUSED(index);
    return QVariant();
}

void QColumnViewVisNode::setPropertyValue(int index, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(value);
}

bool QColumnViewVisNode::hasEditor(int index)
{
    Q_UNUSED(index);
    return false;
}

QWidget* QColumnViewVisNode::createEditor(int index)
{
    Q_UNUSED(index);
    return 0;
}

void QColumnViewVisNode::setEditorValue(int index, QWidget* widget, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(value);
}

QVariant QColumnViewVisNode::editorValue(int index, QWidget* widget)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
}

void QColumnViewVisNode::connectEditor(int index, QWidget* widget, QObject* receiver, const char* member)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(receiver);
    Q_UNUSED(member);
}

QString QColumnViewVisNode::propertyValueString(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant QColumnViewVisNode::propertyValueFromString(int index, QString valueStr)
{
    Q_UNUSED(index);
    Q_UNUSED(valueStr);

    return QVariant();
}

bool QColumnViewVisNode::canLoadSaveProperty(int index)
{
    Q_UNUSED(index);

    return false;
}

#endif





