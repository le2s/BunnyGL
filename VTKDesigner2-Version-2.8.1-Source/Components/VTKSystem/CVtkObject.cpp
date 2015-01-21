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

#include "CVtkObject.h"
#include "CVtkObjectDesc.h"
#include "CVtkObjectControlData.h"
#include "vtkObject.h"
#include "vtkCommand.h"
#include "ITimerNode.h"
#include "ITimerNodeControlData.h"

#include <QTimer>
#include <QMetaObject>
#include <QMetaMethod>
#include <QApplication>
#include <QFontMetrics>

#include "IVisSystemNodeConnectionPath.h"

#include <GCF/IContainer>

Q_DECLARE_METATYPE(vtkObject*)

class CVtkObjectCommand : public vtkCommand
{
public:
    CVtkObjectCommand(CVtkObject* parent) : mParentObject(parent) { }
    ~CVtkObjectCommand() { }

    void Execute(vtkObject* caller, unsigned long event, void* callData) {
        if(mParentObject.isNull() || mParentObject->getVtkObject() != caller)
            return;

        if(event == vtkCommand::DeleteEvent)
            mParentObject = 0;
        else
            mParentObject->handleVtkEvent(event, callData);
    }

private:
    QPointer<CVtkObject> mParentObject;
};

struct CVtkObjectData
{
    CVtkObjectData() : vtkObjectPtr(0), command(0), nodeFactory(0) { }

    CVtkObject* object;
    vtkObject* vtkObjectPtr;
    CVtkObjectCommand* command;

    QString nodeName;
    GCF::DataStore dataStore;
    CVtkObjectControlData controlData;
    IVisSystemNodeFactory* nodeFactory;

    QRectF nodeRect;
    QMap<IVisSystemNodeConnectionPath*, QRectF> pathRectMap;

    void calculateRects();
};

CVtkObjectDesc CVtkObject::ObjectDesc;
void CVtkObject_InitObjectDesc(CVtkObjectDesc* pDesc);

CVtkObject::CVtkObject()
{
    CVtkObject::InitializeObjectDesc();
    d = new CVtkObjectData;
    d->object = this;
}

CVtkObject::~CVtkObject()
{
    delete d;
}

IVisSystemNodeDesc* CVtkObject::nodeDesc()
{
    return &CVtkObject::ObjectDesc;
}

void CVtkObject::InitializeObjectDesc()
{
    static bool initialized = false;
    if(initialized)
        return;
    CVtkObject::ObjectDesc.setQMetaObject(&CVtkObject::staticMetaObject);
    CVtkObject::ObjectDesc.setParentNodeDesc(0);
    CVtkObject::ObjectDesc.setCreateFunction(0);
    CVtkObject_InitObjectDesc(&CVtkObject::ObjectDesc);
    initialized = true;
}

void CVtkObject_InitObjectDesc(CVtkObjectDesc* pDesc)
{
    if(qApp)
    {
        pDesc->setNodeFont(qApp->font());
        pDesc->setNodeColor(qApp->palette().button().color());
    }

    pDesc->setNodeClassName("vtkObject");
    pDesc->setNodeClassCategory("Generic Object");
    pDesc->setNodeClassDescription("Base class for all VTK Objects");

    // Timer Control Path
    pDesc->addConnectionPath(
        new CVtkObjectConnectionPath("Timer", IVisSystemNodeConnectionPath::ControlPath, "vtkObject", 0)
        );
}

void CVtkObject::setVtkObject(vtkObject* object)
{
    static int count=0;
    if(d->vtkObjectPtr == vtkObject::SafeDownCast(object))
        return;

    if(d->vtkObjectPtr)
    {
        d->vtkObjectPtr->Delete();
        d->command = 0;
    }

    d->vtkObjectPtr = vtkObject::SafeDownCast(object);

    if(d->vtkObjectPtr)
    {
        d->command = new CVtkObjectCommand(this);
        d->vtkObjectPtr->AddObserver(vtkCommand::AnyEvent, d->command);
        d->command->Delete();
    }

    d->nodeName = QString("%1%2").arg(nodeDesc()->nodeClassName()).arg(count++);
    QObject::setObjectName(d->nodeName);
    d->calculateRects();

    d->dataStore["vtkObject"] = qVariantFromValue<vtkObject*>(d->vtkObjectPtr);
}

vtkObject* CVtkObject::getVtkObject()
{
    return d->vtkObjectPtr;
}

void CVtkObject::setNodeFactory(IVisSystemNodeFactory* factory)
{
    d->nodeFactory = factory;
}

QObject* CVtkObject::containerObject()
{
    return this;
}

void CVtkObject::setNodeName(QString name)
{
    if(d->nodeName == name)
        return;

    d->nodeName = name;
    d->calculateRects();
    QObject::setObjectName(name);
    emit nodeNameChanged();
    emit propertyChanged("NodeName");
}

QString CVtkObject::nodeName()
{
    return d->nodeName;
}

IVisSystemNodeFactory* CVtkObject::nodeFactory()
{
    return d->nodeFactory;
}

GCF::DataStore CVtkObject::nodeData()
{
    return d->dataStore;
}

bool CVtkObject::hasInput(IVisSystemNodeConnectionPath* path)
{
    Q_UNUSED(path);
    return false;
}

bool CVtkObject::setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    Q_UNUSED(path);
    Q_UNUSED(inputData);
    return false;
}

bool CVtkObject::removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    Q_UNUSED(path);
    Q_UNUSED(inputData);
    return false;
}

bool CVtkObject::fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData)
{
    Q_UNUSED(path);
    Q_UNUSED(outputData);
    return false;
}

bool CVtkObject::outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData)
{
    Q_UNUSED(path);
    Q_UNUSED(outputData);
    return true;
}

bool CVtkObject::fetchControlData(IVisSystemNodeConnectionPath* path, IVisSystemNodeControlData** ctrlData)
{
    if(!path || !ctrlData)
        return false;

    if(path->pathName() == "Timer")
    {
        d->controlData.setObject(this);
        *ctrlData = &d->controlData;
        return true;
    }

    return false;
}

bool CVtkObject::connectControl(IVisSystemNodeConnectionPath* path, IVisSystemNodeControlData* ctrlData)
{
    if(!path || !ctrlData)
        return false;

    if(path->pathName() == "Timer")
    {
        IContainer* container = 0;
        if(ctrlData->queryInterface("IContainer", (void**)&container) && container)
        {
            QObject* object = container->containerObject();
            if(object)
                return connect(object, SIGNAL(OnTimeout()), this, SLOT(on_timeout()));
        }

        ITimerNodeControlData* timerCtrl = 0;
        if(ctrlData->queryInterface("ITimerNodeControlData", (void**)&timerCtrl) && timerCtrl)
        {
            ITimerNode* node = timerCtrl->timerNode();
            return connect(node->containerObject(), SIGNAL(OnTimeout()), this, SLOT(on_timeout()));
        }
    }

    return false;
}

bool CVtkObject::disconnectControl(IVisSystemNodeConnectionPath* path, IVisSystemNodeControlData* ctrlData)
{
    if(!path || !ctrlData)
        return false;

    if(path->pathName() == "Timer")
    {
        IContainer* container = 0;
        if(ctrlData->queryInterface("IContainer", (void**)&container) && container)
        {
            QObject* object = container->containerObject();
            if(object)
                return disconnect(object, SIGNAL(OnTimeout()), this, SLOT(on_timeout()));
        }

        ITimerNodeControlData* timerCtrl = 0;
        if(ctrlData->queryInterface("ITimerNodeControlData", (void**)&timerCtrl) && timerCtrl)
        {
            ITimerNode* node = timerCtrl->timerNode();
            return disconnect(node->containerObject(), SIGNAL(OnTimeout()), this, SLOT(on_timeout()));
        }
    }

    return false;
}

bool CVtkObject::executeCommand(QString cmdName, QString& result)
{
    QString slotName = QString("command_%1").arg(cmdName);
    bool success = QMetaObject::invokeMethod(this, slotName.toAscii().data(), Qt::DirectConnection);
    if(!success)
        result = "Command execution was not successful";
    return success;
}

QRectF CVtkObject::nodeRect()
{
    return d->nodeRect;
}

QRectF CVtkObject::pathRect(IVisSystemNodeConnectionPath* path)
{
    if(d->pathRectMap.contains(path))
        return d->pathRectMap[path];
    return QRectF();
}

void CVtkObject::paintNode(QPainter* p, QRectF rect, const QStyleOption& opt)
{
    Q_UNUSED(p);
    Q_UNUSED(rect);
    Q_UNUSED(opt);
}

void CVtkObject::paintConnectionPath(IVisSystemNodeConnectionPath* path, QPainter* p, QRectF rect, const QStyleOption& opt)
{
    Q_UNUSED(path);
    Q_UNUSED(p);
    Q_UNUSED(rect);
    Q_UNUSED(opt);
}

void CVtkObject::initializeNode()
{
    // Do nothing.
}

void CVtkObject::finalizeNode()
{
    if(d->vtkObjectPtr)
        d->vtkObjectPtr->Delete();
    d->vtkObjectPtr = 0;
    delete this;
}

void CVtkObject::handleTimerControlSignal()
{
    // Do nothing.
}

void CVtkObject::on_timeout()
{
    this->handleTimerControlSignal();
}

void CVtkObject::stopHandlingVtkEvents()
{
    if(d->command)
    {
        d->vtkObjectPtr->RemoveObserver(d->command);
        d->command = 0;
    }
}

void CVtkObject::handleVtkEvent(unsigned long event, void* callData)
{
    switch(event)
    {
    case vtkCommand::ModifiedEvent:
        emit OnModified();
        break;
    case vtkCommand::DeleteEvent:
        emit OnDelete();
        break;
    case vtkCommand::StartEvent:
        emit OnStart();
        break;
    case vtkCommand::EndEvent:
        emit OnEnd();
        break;
    default:
        break;
    }
    Q_UNUSED(callData);
}

void CVtkObjectData::calculateRects()
{
    QFontMetrics fm = QApplication::fontMetrics();

    // This is the least width and height required to draw the node label.
    double textWidth = (double)fm.width(QString("_%1__").arg(nodeName));
    double textHeight = (double)fm.height();

    // This is the least width and height required to draw the icon
    QPixmap normalIcon = object->nodeDesc()->nodeIcon().pixmap(30, 30);
    double iconWidth = (double)( normalIcon.width() );
    double iconHeight = (double)( normalIcon.height() );

    // This is the least width/height we will need to drawing the node class name
    double ncNameWidth = (double)fm.width(QString("_%1__").arg(object->nodeDesc()->nodeClassName()));
    double ncNameHeight = (double)fm.height();

    // Lets measure the number of input, output and control paths.
    IVisSystemNodeDesc* desc = object->nodeDesc();
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

