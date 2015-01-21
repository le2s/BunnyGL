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

#include "CGenericVisNodeBase.h"

struct CGenericVisNodeBaseData
{
    CGenericVisNodeBaseData() : factory(0) { }

    CGenericVisNodeBase* nodeBase;
    IVisSystemNodeFactory* factory;
    GCF::DataStore dataStore;
    QRectF nodeRect;
    QMap<IVisSystemNodeConnectionPath*, QRectF> pathRectMap;

    void calculateRects();
};

DEFINE_VIS_NODE1(CGenericVisNodeBase)
{
    pDesc->setNodeClassName("CGenericVisNode");
    pDesc->setNodeClassDescription("Generic Visualization Node");
    pDesc->setVisSystemName("None");
    pDesc->setVisSystemDescription("None");
    pDesc->setVisSystemVersion("None");
    pDesc->setCreateFunction(0);
}

CGenericVisNodeBase::CGenericVisNodeBase()
{
    d = new CGenericVisNodeBaseData;
    d->nodeBase = this;
    static int count = 0;
    setObjectName( QString("GenericNode%1").arg(count++) );
}

CGenericVisNodeBase::~CGenericVisNodeBase()
{
    delete d;
}

void CGenericVisNodeBase::setNodeFactory(IVisSystemNodeFactory* factory)
{
    d->factory = factory;
}

QObject* CGenericVisNodeBase::containerObject()
{
    return this;
}

void CGenericVisNodeBase::setNodeName(QString name)
{
    if(objectName() == name)
        return;

    setObjectName(name);
    d->calculateRects();

    emit nodeNameChanged();
    emit propertyChanged("NodeName");
}

QString CGenericVisNodeBase::nodeName()
{
    return objectName();
}

IVisSystemNodeFactory* CGenericVisNodeBase::nodeFactory()
{
    return d->factory;
}

GCF::DataStore CGenericVisNodeBase::nodeData()
{
    return d->dataStore;
}

bool CGenericVisNodeBase::hasInput(IVisSystemNodeConnectionPath* path)
{
    Q_UNUSED(path);
    return false;
}

bool CGenericVisNodeBase::setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    Q_UNUSED(path);
    Q_UNUSED(inputData);
    return false;
}

bool CGenericVisNodeBase::removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    Q_UNUSED(path);
    Q_UNUSED(inputData);
    return false;
}

bool CGenericVisNodeBase::fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData)
{
    Q_UNUSED(path);
    Q_UNUSED(outputData);
    return false;
}

bool CGenericVisNodeBase::outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData)
{
    Q_UNUSED(path);
    Q_UNUSED(outputData);
    return true;
}

bool CGenericVisNodeBase::fetchControlData(IVisSystemNodeConnectionPath* path, IVisSystemNodeControlData** ctrlData)
{
    Q_UNUSED(path);
    Q_UNUSED(ctrlData);
    return false;
}

bool CGenericVisNodeBase::connectControl(IVisSystemNodeConnectionPath* path, IVisSystemNodeControlData* ctrlData)
{
    Q_UNUSED(path);
    Q_UNUSED(ctrlData);
    return false;
}

bool CGenericVisNodeBase::disconnectControl(IVisSystemNodeConnectionPath* path, IVisSystemNodeControlData* ctrlData)
{
    Q_UNUSED(path);
    Q_UNUSED(ctrlData);
    return false;
}

bool CGenericVisNodeBase::executeCommand(QString cmdName, QString& result)
{
    QString slotName = QString("command_%1").arg(cmdName);
    bool success = QMetaObject::invokeMethod(this, slotName.toAscii().data(), Qt::DirectConnection);
    if(!success)
        result = "Command execution was not successful";
    return success;
}

QRectF CGenericVisNodeBase::nodeRect()
{
    return d->nodeRect;
}

QRectF CGenericVisNodeBase::pathRect(IVisSystemNodeConnectionPath* path)
{
    if(d->pathRectMap.contains(path))
        return d->pathRectMap[path];
    return QRectF();
}

void CGenericVisNodeBase::paintNode(QPainter* p, QRectF rect, const QStyleOption& opt)
{
    Q_UNUSED(p);
    Q_UNUSED(rect);
    Q_UNUSED(opt);
}

void CGenericVisNodeBase::paintConnectionPath(IVisSystemNodeConnectionPath* path, QPainter* p, QRectF rect, const QStyleOption& opt)
{
    Q_UNUSED(path);
    Q_UNUSED(p);
    Q_UNUSED(rect);
    Q_UNUSED(opt);
}

void CGenericVisNodeBase::initializeNode()
{
    d->calculateRects();

    if(1)
    {
        static int count = 0;
        setNodeName( QString("%1_%2").arg(this->nodeDesc()->nodeClassName()).arg(count++) );
    }
}

void CGenericVisNodeBase::finalizeNode()
{
    delete this;
}

void CGenericVisNodeBase::recalculatePathRects()
{
    d->calculateRects();
}

void CGenericVisNodeBaseData::calculateRects()
{
    QFontMetrics fm = QApplication::fontMetrics();

    // This is the least width and height required to draw the node label.
    double textWidth = (double)fm.width(QString("_%1__").arg(nodeBase->nodeName()));
    double textHeight = (double)fm.height();

    // This is the least width and height required to draw the icon
    QPixmap normalIcon = nodeBase->nodeDesc()->nodeIcon().pixmap(30, 30);
    double iconWidth = (double)( normalIcon.width() );
    double iconHeight = (double)( normalIcon.height() );

    // This is the least width/height we will need to drawing the node class name
    double ncNameWidth = (double)fm.width(QString("_%1__").arg(nodeBase->nodeDesc()->nodeClassName()));
    double ncNameHeight = (double)fm.height();

    // Lets measure the number of input, output and control paths.
    IVisSystemNodeDesc* desc = nodeBase->nodeDesc();
    QList<IVisSystemNodeConnectionPath*> inputs, outputs, controls;
    for(int i=0; i<desc->connectionPathCount(); i++)
    {
        IVisSystemNodeConnectionPath* path = desc->connectionPath(i);
        if(path->pathType() == IVisSystemNodeConnectionPath::InputPath)
            inputs.append(path);
        else if(path->pathType() == IVisSystemNodeConnectionPath::OutputPath)
            outputs.append(path);
        else if(path->pathType() == IVisSystemNodeConnectionPath::ControlPath)
            controls.append(path);
    }

    const double pathRectWidth = 8.0f;
    const double pathRectHeight = 8.0f;
    const double pathRectDist = 4.0f;

    // For each path we need a rectangle atleast 4x4 units in size and 2 pixels
    // distance between them.
    double inputRectWidth = inputs.count() ? inputs.count()*pathRectWidth + (inputs.count()-1)*pathRectDist : 0.0f;
    double outputRectWidth = outputs.count() ? outputs.count()*pathRectWidth + (outputs.count()-1)*pathRectDist : 0.0f;
    double controlRectHeight = controls.count() ? controls.count()*pathRectHeight + (controls.count()-1)*pathRectDist : 0.0f;

    // now lets calculate the actual width and height of the node.
    double width = textWidth > ncNameWidth ? textWidth : ncNameWidth;
    double height = textHeight+ncNameHeight;
    width += iconWidth;
    height = height > iconHeight ? height : iconHeight;

    if(inputRectWidth > width)
        width = inputRectWidth;
    if(outputRectWidth > width)
        width = outputRectWidth;
    if(controlRectHeight > height)
        height = controlRectHeight;
    width += pathRectWidth + pathRectDist;
    height += pathRectHeight + pathRectDist;

    const double minWidth = 150.0f;
    const double minHeight = 50.0f;
    if(width < minWidth)
        width = minWidth;
    if(height < minHeight)
        height = minHeight;

    QRectF retRect(-width/2.0f, -height/2.0f, width, height);

    // Now lets calculate the path rects and store them in the cache.
    double inputX = -inputRectWidth/2.0f;
    double outputX = -outputRectWidth/2.0f;
    double controlRectY = -controlRectHeight/2.0f;
    for(int i=0; i<inputs.count(); i++)
    {
        QRectF pathRect(inputX, retRect.top(), pathRectWidth, pathRectHeight);
        pathRectMap[inputs[i]] = pathRect;
        inputX += (pathRectWidth+pathRectDist);
    }
    for(int i=0; i<outputs.count(); i++)
    {
        QRectF pathRect(outputX, retRect.bottom()-pathRectHeight, pathRectWidth, pathRectHeight);
        pathRectMap[outputs[i]] = pathRect;
        outputX += (pathRectWidth+pathRectDist);
    }
    for(int i=0; i<controls.count(); i++)
    {
        QRectF pathRect(retRect.left(), controlRectY, pathRectWidth, pathRectHeight);
        pathRectMap[controls[i]] = pathRect;
        controlRectY += (pathRectHeight+pathRectDist);
    }

    nodeRect = retRect;
}

