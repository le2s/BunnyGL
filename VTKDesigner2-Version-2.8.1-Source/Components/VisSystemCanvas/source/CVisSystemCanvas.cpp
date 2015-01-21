/****************************************************************************
**
** Copyright (C) VCreate Logic Private Limited, Bangalore
**
** Use of this file is limited according to the terms specified by
** VCreate Logic Private Limited, Bangalore.  Details of those terms
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

#include "CVisSystemCanvas.h"
#include "CVisNetworkConnection.h"
#include "CVisSystemCanvasNodeLayout.h"
#include "CVisSystemCanvasNodeItem.h"
#include "CVisSystemCanvasConnectionItem.h"
#include "CLocalBoolean.h"
#include "CVisSystemCanvasCmds.h"
#include "CVisSystemCanvasWidget.h"
#include "CVisSystemCanvasNote.h"
#include "CVisSystemCanvasComponent.h"

#include "IXmlUtils.h"
#include "IVisSystemNode.h"
#include "IVisSystemNodeDesc.h"
#include "IPropertyEditor.h"
#include "IQObjectLoadSave.h"
#include "IEditableProperties.h"
#include "IValueEditorFactory.h"
#include "IValueEditorCreator.h"
#include "IEnumValueEditorCreator.h"
#include "IVisNetworkConnection.h"
#include "IVisSystemNodeFactory.h"
#include "IVisSystemNodeConnectionPath.h"
#include "IVisSystemNodeFactoryRegistry.h"
#include "IVisSystemNodeFactoryRegistryExplorer.h"

#include <QGraphicsScene>
#include <QPainter>
#include <QPainterPath>
#include <QMap>
#include <QList>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QWheelEvent>
#include <QMenu>
#include <QPointer>
#include <QScrollBar>
#include <QDropEvent>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QMetaProperty>
#include <QMetaObject>
#include <QToolTip>
#include <QGraphicsSceneHelpEvent>
#include <QClipboard>
#include <QUndoStack>
#include <QGLWidget>
#include <QSvgRenderer>
#include <QFile>
#include <QClipboard>

struct CVisSystemCanvasData
{
    CVisSystemCanvasData() : scene(0), network(0), nodeFactoryReg(0), propertyEditor(0), xmlUtils(0),
        qobjectLoadSave(0), doubleClickConnectCount(0), genericConnect(0), updatePropertyEditor(true), 
        zoomLevel(0), zoomValue(1.0),panMode(false), senderNode(0), receiverNode(0), senderPath(-1), 
        receiverPath(-1), conLine(0), captureNodeDeletion(false), noteSvgRenderer(0) { }

    CVisSystemCanvas* canvas;
    QGraphicsScene* scene;
    IVisNetwork* network;
    IVisSystemNodeFactoryRegistry* nodeFactoryReg;
    IPropertyEditor* propertyEditor;
    IXmlUtils* xmlUtils;
    IQObjectLoadSave* qobjectLoadSave;
    int doubleClickConnectCount;

    // Network structures.
    QMap<IVisSystemNode*, CVisSystemCanvasNodeItem*> nodeMap;
    QMap<IVisNetworkConnection*, CVisSystemCanvasConnectionItem*> connectionMap;
    QList<IVisSystemNode*> nodeList;
    QList<IVisNetworkConnection*> connectionList;
    QPointer<CVisSystemCanvasNodeItem> selectedNodeItem;
    IVisSystemNode* findNode(QString nodeName) {
        for(int i=0; i<nodeList.count(); i++)
        {
            IVisSystemNode* node = nodeList[i];
            if(node->nodeName() == nodeName)
                return node;
        }
        return 0;
    }

    // Structures for connection
    bool genericConnect;
    bool updatePropertyEditor;

    // Zoom manipulation stuff
    int zoomLevel;
    double zoomValue;
    double calculateScaleFor(int zoomLevel) const;
    int calculateZoomLevelFor(double scale) const;

    // Pan mode stuff.
    bool panMode;
    QPoint panMousePos;

    // Data for making connections.
    IVisSystemNode* senderNode;
    IVisSystemNode* receiverNode;
    int senderPath;
    int receiverPath;
    QGraphicsLineItem* conLine;
    CVisSystemCanvasNodeItem* nodeItemAt(QPointF pos, bool scenePos=false);
    CVisSystemCanvasConnectionItem* connectionItemAt(QPointF pos, bool scenePos=false);

    // Data for drawing the canvas background
    QColor bgTopColor, bgMidColor, bgBottomColor;

    // Undo Redo Stuff
    CUndoRedoStack undoStack;
    bool captureNodeDeletion;

    // Widgets
    QList<CVisSystemCanvasWidget*> widgetList;
	QMap<QString, QRectF> widgetGeoMap;

    // Notes
    QSvgRenderer* noteSvgRenderer;
    QString noteSvgFile;
    QString noteSvgElementId;
    bool noteSvgCompressed;
    QList<IVisNetworkCanvasNote*> noteList;
    void updateSvgRenderer();
};

CVisSystemCanvas::CVisSystemCanvas(QWidget* parent)
:QGraphicsView(parent)
{
    d = new CVisSystemCanvasData;
    d->canvas = this;
    d->scene = new QGraphicsScene(this);
    d->network = qobject_cast<IVisNetwork*>(this);

    setScene(d->scene);

    // I am facing serious problems with this one on Windows Vista. 
    // Dont know what the damn problem is 
#ifdef Q_WS_WIN
	// setViewport(new QGLWidget);
#endif
	setAttribute(Qt::WA_Hover);

    d->scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    d->scene->installEventFilter(this);
	d->scene->setSceneRect(-5000, -5000, 10000, 10000);
    setRenderHints(QPainter::Antialiasing|QPainter::TextAntialiasing|QPainter::SmoothPixmapTransform);

    d->bgTopColor = palette().highlight().color();
    d->bgMidColor = QColor( (d->bgTopColor.red()+255)/2, (d->bgTopColor.green()+255)/2, (d->bgTopColor.blue()+255)/2 );
    d->bgBottomColor = QColor( (d->bgTopColor.red()+128)/2, (d->bgTopColor.green()+128)/2, (d->bgTopColor.blue()+128)/2 );
    d->bgTopColor.setAlphaF(0.1);
    d->bgMidColor.setAlphaF(0.2);
    d->bgBottomColor.setAlphaF(0.1);

    CCanvasUndoRedoCmd::VisNetwork = d->network;
    CCanvasUndoRedoCmd::VisNetworkCanvas = this;
    CCanvasUndoRedoCmd::Canvas = this;

    d->noteSvgFile = ":/VisSystemCanvas/notes.svg.qz";
    d->noteSvgElementId = "yellow-notes";
    d->noteSvgCompressed = true;

    connect(&d->undoStack, SIGNAL(countChanged(int)), this, SIGNAL(projectPartModified()));
}

CVisSystemCanvas::~CVisSystemCanvas()
{
	this->newCanvas();
    delete d;
}

void CVisSystemCanvas::setNodeFactoryRegistry(IVisSystemNodeFactoryRegistry* reg)
{
    d->nodeFactoryReg = reg;
    CCanvasUndoRedoCmd::NodeFactoryRegistry = reg;
}

IVisSystemNodeFactoryRegistry* CVisSystemCanvas::nodeFactoryRegistry() const
{
    return d->nodeFactoryReg;
}

void CVisSystemCanvas::setPropertyEditor(IPropertyEditor* propEditor)
{
    if(d->propertyEditor)
        disconnect(d->propertyEditor->containerObject(), 0, this, 0);

    d->propertyEditor = propEditor;
    CCanvasUndoRedoCmd::PropertyEditor = propEditor;

    if(d->propertyEditor)
        connect(d->propertyEditor->containerObject(),
                SIGNAL(propertyValueChanged(QObject*, QString, QVariant, QVariant)),
                this,
                SLOT(on_propertyEditor_propertyValueChanged(QObject*, QString, QVariant, QVariant)));
}

IPropertyEditor* CVisSystemCanvas::propertyEditor() const
{
    return d->propertyEditor;
}

void CVisSystemCanvas::setXmlUtils(IXmlUtils* xmlUtils)
{
    d->xmlUtils = xmlUtils;
}

IXmlUtils* CVisSystemCanvas::xmlUtils() const
{
    return d->xmlUtils;
}

void CVisSystemCanvas::setQObjectLoadSave(IQObjectLoadSave* ls)
{
    d->qobjectLoadSave = ls;
    CCanvasUndoRedoCmd::QObjectLoadSave = ls;
}

IQObjectLoadSave* CVisSystemCanvas::qobjectLoadSave() const
{
    return d->qobjectLoadSave;
}

CVisSystemCanvasNodeItem* CVisSystemCanvas::nodeItem(IVisSystemNode* node) const
{
    if(d->nodeMap.contains(node))
        return d->nodeMap[node];

    return 0;
}

CVisSystemCanvasConnectionItem* CVisSystemCanvas::connectionItem(IVisNetworkConnection* con) const
{
    if(d->connectionMap.contains(con))
        return d->connectionMap[con];

    return 0;
}

QList<CVisSystemCanvasNodeItem*> CVisSystemCanvas::nodeItemList() const
{
    return d->nodeMap.values();
}

QList<CVisSystemCanvasConnectionItem*> CVisSystemCanvas::connectionItemList() const
{
    return d->connectionMap.values();
}

void CVisSystemCanvas::selectNext(CVisSystemCanvasNodeItem* item)
{
    Q_UNUSED(item);
}

void CVisSystemCanvas::selectPrevious(CVisSystemCanvasNodeItem* item)
{
    Q_UNUSED(item);
}

bool CVisSystemCanvas::hasConnections(IVisSystemNode* node) const
{
    IVisNetworkConnection* con = 0;
    Q_FOREACH(con, d->connectionList)
    {
        if(con->senderNode() == node || con->receiverNode() == node)
            return true;
    }
    return false;
}

CUndoRedoStack* CVisSystemCanvas::undoRedoStack() const
{
    return & (d->undoStack);
}

void CVisSystemCanvas::newCanvas()
{
    d->undoStack.clear();
	qDeleteAll(d->widgetList);
	d->widgetList.clear();
    for(int i=0; i<d->noteList.count(); i++)
    {
        IVisNetworkCanvasNote* note = d->noteList[i];
        d->scene->removeItem(note->graphicsItem());
        note->containerObject()->deleteLater();
    }
    d->noteList.clear();
    this->removeAllNodes();
}

void CVisSystemCanvas::saveCanvas()
{

}

void CVisSystemCanvas::saveAsCanvas()
{

}

void CVisSystemCanvas::loadCanvas()
{

}

void CVisSystemCanvas::loadCanvasFrom(QString fileName)
{
    Q_UNUSED(fileName);
}

void CVisSystemCanvas::saveCanvasInto(QString fileName)
{
    Q_UNUSED(fileName);
}

QObject* CVisSystemCanvas::containerObject()
{
    return this;
}

QGraphicsView* CVisSystemCanvas::graphicsView()
{
    return this;
}

QGraphicsScene* CVisSystemCanvas::graphicsScene()
{
    return d->scene;
}

void CVisSystemCanvas::setVisNetwork(IVisNetwork* network)
{
    // Because this object is both the network and the network canvas itself
    Q_UNUSED(network);
}

IVisNetwork* CVisSystemCanvas::visNetwork()
{
    return d->network;
}

bool CVisSystemCanvas::supportsMultipleSelection()
{
    return false;
}

void CVisSystemCanvas::setMultipleSelection(bool val)
{
    Q_UNUSED(val);
    // no support for multiple selection
}

bool CVisSystemCanvas::isMultipleSelection()
{
    return false;
}

void CVisSystemCanvas::select(IVisSystemNode* node, bool select)
{
    CVisSystemCanvasNodeItem* nodeItem = this->nodeItem(node);
    if(!nodeItem)
        return;

    if(select)
        this->ensureVisible(nodeItem);
        
    nodeItem->setSelected(select);
}

int CVisSystemCanvas::selectedNodeCount()
{
    return 1;
}

IVisSystemNode* CVisSystemCanvas::selectedNode(int index)
{
    if(!index)
        return d->selectedNodeItem->visSystemNode();

    return 0;
}

void CVisSystemCanvas::centerOn(IVisSystemNode* node)
{
    CVisSystemCanvasNodeItem* nodeItem = this->nodeItem(node);
    if(!nodeItem)
        return;

    QGraphicsView::centerOn(nodeItem);
}

void CVisSystemCanvas::clear()
{
    newCanvas();
}

void CVisSystemCanvas::cut()
{
    if(!selectedNodeCount())
        return;

    // First copy the selection
    copy();

    // Now cut (aka delete) the selection
    for(int i=0; i<selectedNodeCount(); i++)
    {
        IVisSystemNode* node = selectedNode(i);
        d->captureNodeDeletion = true;
        removeNode(node);
        d->captureNodeDeletion = false;
    }
}

void CVisSystemCanvas::copy()
{
    if(!selectedNodeCount())
        return;

    QDomDocument doc("VisCanvasSnapshot");
    QDomElement rootE = doc.createElement("VisCanvasSnapshot");
    doc.appendChild(rootE);

    for(int i=0; i<selectedNodeCount(); i++)
    {
        IVisSystemNode* node = selectedNode(i);

        // Save node information
        QDomElement nodeE = doc.createElement("Node");
        rootE.appendChild(nodeE);
        nodeE.setAttribute("Name", node->nodeName());
        nodeE.setAttribute("ClassName", node->nodeDesc()->nodeClassName());
        nodeE.setAttribute("ID", QString("%1").arg(i));

        CVisSystemCanvasNodeItem* item = this->nodeItem(node);
        if(item)
        {
            QPointF pos = item->pos();
            nodeE.setAttribute("X", QString("%1").arg(pos.x()));
            nodeE.setAttribute("Y", QString("%1").arg(pos.y()));
        }

        // Save node property information
        d->qobjectLoadSave->saveProperties(node->containerObject(), doc, nodeE);
    }

    QString docStr = doc.toString(2);
    QClipboard* clipboard = qApp->clipboard();
    clipboard->setText(docStr);
}

void CVisSystemCanvas::paste()
{
    if(!d->xmlUtils || !d->nodeFactoryReg || !d->qobjectLoadSave)
        return;

    QClipboard* clipboard = qApp->clipboard();
    QString docStr = clipboard->text();
    if(docStr.isEmpty())
        return;

    QDomDocument doc;
    doc.setContent(docStr);
    if(doc.isNull())
        return;

    QDomElement rootE = doc.documentElement();

    QList<QDomElement> nodeEList = d->xmlUtils->findElements(rootE, "Node");
    for(int i=0; i<nodeEList.count(); i++)
    {
        QDomElement nodeE = nodeEList[i];

        QString className = nodeE.attribute("ClassName");
        IVisSystemNode* node = d->nodeFactoryReg->createNode(className);
        if(!node)
            continue;
        d->network->addNode(node);
        CVisSystemCanvasNodeItem* nodeItem = this->nodeItem(node);
        if(!nodeItem)
            continue;
        QPointF pos = QPointF(nodeE.attribute("X").toDouble(), nodeE.attribute("Y").toDouble()) + QPoint(30, 30);

        // Alter the position for the second paste (if any)
        nodeE.setAttribute("X", QString("%1").arg(pos.x()));
        nodeE.setAttribute("Y", QString("%1").arg(pos.y()));

        nodeItem->setPos(pos);
        node->setNodeName(nodeE.attribute("Name"));

        // Load properties.
        d->qobjectLoadSave->loadProperties(node->containerObject(), doc, nodeE);
    }

    docStr = doc.toString(2);
    clipboard->setText(docStr);
}

void CVisSystemCanvas::undo()
{
    d->undoStack.undo();
}

void CVisSystemCanvas::redo()
{
    d->undoStack.redo();
}

bool CVisSystemCanvas::isUndoAvailable()
{
    return false;
}

bool CVisSystemCanvas::isRedoAvailable()
{
    return false;
}

void CVisSystemCanvas::zoomIn()
{
    setZoomLevel(d->zoomLevel + 1);
}

void CVisSystemCanvas::zoomOut()
{
    setZoomLevel(d->zoomLevel - 1);
}

void CVisSystemCanvas::zoomFit()
{
    QRectF iRect = d->scene->itemsBoundingRect();
    fitInView(iRect, Qt::KeepAspectRatio);

    double scale = matrix().m11();
    d->zoomLevel = d->calculateZoomLevelFor(scale);

    emit zoomLevelChanged(d->zoomLevel);
    emit zoomScaleChanged(scale);
}

void CVisSystemCanvas::zoomOne()
{
    setZoomLevel(0);
}

int CVisSystemCanvas::zoomLevel()
{
    return d->zoomLevel;
}

void CVisSystemCanvas::setZoomLevel(int zl)
{
    if(d->zoomLevel == zl)
        return;

    // Clip zoom level between -10 and +10.
    if(zl < -10)
        zl = -10;
    else if(zl > 10)
        zl = 10;

    if(d->zoomLevel == zl)
        return;

    d->zoomLevel = zl;
    d->zoomValue = d->calculateScaleFor(zl);

    // Set the zoom to the graphics view.
    QApplication::setOverrideCursor(Qt::WaitCursor);
    QMatrix matrix;
    matrix.scale(d->zoomValue, d->zoomValue);
    setMatrix(matrix);
    QApplication::restoreOverrideCursor();

    emit zoomLevelChanged(d->zoomLevel);
    emit zoomScaleChanged(d->zoomValue);
    emit zoomEvent(d->zoomValue);
}

void CVisSystemCanvas::queryZoomLevelRange(int& min, int& max)
{
    min = -10;
    max = 10;
}

void CVisSystemCanvas::bringToCenter()
{
    QRectF bRect = d->scene->itemsBoundingRect();
    QPointF bCenter = bRect.center();
QList<QGraphicsItem*> items = d->scene->items();
    for(int i=0; i<items.count(); i++)
    {
        QGraphicsItem* item = items[i];
        CVisSystemCanvasNodeItem* nodeItem = dynamic_cast<CVisSystemCanvasNodeItem*>(item);
        if(!nodeItem)
            continue;

        QPointF pos = item->pos();
        pos -= bCenter;
        item->setPos(pos);
    }
}

void CVisSystemCanvas::layoutNodes()
{
    CVisSystemCanvasNodeLayout layout;
    layout.setCanvas(this);
    layout.layoutNodes();
}

void CVisSystemCanvas::setReadOnly(bool val)
{
    setInteractive(val);
}

bool CVisSystemCanvas::isReadOnly()
{
    return isInteractive();
}

void CVisSystemCanvas::addNode(IVisSystemNode* node)
{
    if(!node)
        return;

    if(d->nodeMap.contains(node))
        return;

    CVisSystemCanvasNodeItem* nodeItem = new CVisSystemCanvasNodeItem(node, this);
    d->nodeMap[node] = nodeItem;
    d->nodeList.append(node);
    d->scene->addItem(nodeItem);
    nodeItem->show();

    connect(nodeItem, SIGNAL(itemSelected(bool)), this, SLOT(on_nodeItem_itemSelected(bool)));
    nodeItem->setSelected(true);

    connect(node->containerObject(), SIGNAL(propertyChanged(const QString&)), this, SIGNAL(projectPartModified()));

    emit nodeAdded(node);
    emit projectPartModified();
}

void CVisSystemCanvas::removeNode(IVisSystemNode* node)
{
    if(!node)
        return;

    if(!d->nodeMap.contains(node))
        return;

    if(d->propertyEditor && d->propertyEditor->object() == node->containerObject())
        d->propertyEditor->setObject(0);

    CVisSystemCanvasNodeItem* nodeItem = d->nodeMap[node];
    IVisSystemNodeFactory* factory = node->nodeFactory();
    emit nodeRemoved(node);
    d->network->disconnectNode(node);

    if(d->captureNodeDeletion)
        d->undoStack.push( new CNodeDeleteCmd(node) );

    if(d->propertyEditor && d->propertyEditor->object() == node->containerObject())
        d->propertyEditor->setObject(0);

    if(factory)
        factory->deleteNode(node);
    else
        node->finalizeNode();
    delete nodeItem;
    d->nodeMap.remove(node);
    d->nodeList.removeAll(node);
    emit projectPartModified();
}

void CVisSystemCanvas::removeAllNodes()
{
    // it is better to reset the property editor before hand
    if(d->propertyEditor)
        d->propertyEditor->setObject(0);

    // Now delete all nodes.
    while(d->nodeList.count())
        removeNode(d->nodeList.first());
}

int CVisSystemCanvas::nodeCount()
{
    return d->nodeList.count();
}

IVisSystemNode* CVisSystemCanvas::node(int index)
{
    if(index < 0 || index > d->nodeList.count())
        return 0;

    return d->nodeList[index];
}

bool CVisSystemCanvas::hasNode(IVisSystemNode* node)
{
    return d->nodeMap.contains(node);
}

bool CVisSystemCanvas::connectNode(IVisSystemNode* senderNode, IVisSystemNode* receiverNode)
{
    if(!senderNode || !receiverNode)
        return false;

    CLocalBoolean gc(d->genericConnect);
    gc = true;

    // First make a list of all output and input paths between sender and receiver.
    QList<IVisSystemNodeConnectionPath*> outputPaths;
    QList<IVisSystemNodeConnectionPath*> inputPaths;
    IVisSystemNodeDesc* senderDesc = senderNode->nodeDesc();
    IVisSystemNodeDesc* receiverDesc = receiverNode->nodeDesc();
    for(int i=0; i<senderDesc->connectionPathCount(); i++)
    {
        IVisSystemNodeConnectionPath* path = senderDesc->connectionPath(i);
        if(path->pathType() == IVisSystemNodeConnectionPath::OutputPath)
            outputPaths.append(path);
    }
    for(int i=0; i<receiverDesc->connectionPathCount(); i++)
    {
        IVisSystemNodeConnectionPath* path = receiverDesc->connectionPath(i);
        if(path->pathType() == IVisSystemNodeConnectionPath::InputPath)
            inputPaths.append(path);
    }

    // Now try out all permutations and combinations of connections between them.
    for(int i=0; i<outputPaths.count(); i++)
    {
        IVisSystemNodeConnectionPath* outPath = outputPaths[i];
        int outLine = outPath->pathIndex();
        for(int j=0; j<inputPaths.count(); j++)
        {
            IVisSystemNodeConnectionPath* inPath = inputPaths[j];
            int inLine = inPath->pathIndex();
            bool success = connectNode(senderNode, outLine, receiverNode, inLine, 0);
            if(success)
                return true;
        }
    }

    // If we are here, it means that all input/output paths failed. The only thing that can be done 
    // now is to connect control paths.
    outputPaths.clear();
    inputPaths.clear();
    for(int i=0; i<senderDesc->connectionPathCount(); i++)
    {
        IVisSystemNodeConnectionPath* path = senderDesc->connectionPath(i);
        if(path->pathType() == IVisSystemNodeConnectionPath::ControlPath)
            outputPaths.append(path);
    }
    for(int i=0; i<receiverDesc->connectionPathCount(); i++)
    {
        IVisSystemNodeConnectionPath* path = receiverDesc->connectionPath(i);
        if(path->pathType() == IVisSystemNodeConnectionPath::ControlPath)
            inputPaths.append(path);
    }
    for(int i=0; i<outputPaths.count(); i++)
    {
        IVisSystemNodeConnectionPath* outPath = outputPaths[i];
        int outLine = outPath->pathIndex();
        for(int j=0; j<inputPaths.count(); j++)
        {
            IVisSystemNodeConnectionPath* inPath = inputPaths[j];
            int inLine = inPath->pathIndex();
            bool success = connectNode(senderNode, outLine, receiverNode, inLine, 0);
            if(success)
                return true;
        }
    }

    // Announce failurs to receivers.
    QString errMsg = QString("Connection between %1 and %2 not possible").arg(senderNode->nodeName()).arg(receiverNode->nodeName());
    qWarning("%s", qPrintable(errMsg));

    // No way to connect these two nodes.
    return false;
}

bool CVisSystemCanvas::connectNode(IVisSystemNode* sender, int outLine, IVisSystemNode* receiver, int inLine, IVisNetworkConnection** connection)
{
    // Check for input parameters.
    if(!sender || !receiver || outLine < 0 || inLine < 0)
    {
        if(!d->genericConnect)
            qWarning("Invalid sender, receiver or path specified");
        return false;
    }

    // Find out if the sender is a IVisSystemNode subclass.
    QObject* senderObject = sender->containerObject();
    IVisSystemNode* sender2 = qobject_cast<IVisSystemNode*>(senderObject);
    if(!sender2)
    {
        if(!d->genericConnect)
            qWarning("Sender is not a VTK Object");
        return false;
    }

    // Find out if the receiver is a IVisSystemNode subclass.
    QObject* receiverObject = receiver->containerObject();
    IVisSystemNode* receiver2 = qobject_cast<IVisSystemNode*>(receiverObject);
    if(!receiver2)
    {
        if(!d->genericConnect)
            qWarning("Receiver is not a VTK Object");
        return false;
    }

    IVisSystemNodeIOData* data = 0;
    IVisSystemNodeControlData* senderControlData = 0;
    IVisSystemNodeControlData* receiverControlData = 0;
    IVisSystemNodeConnectionPath* outPath = 0;
    IVisSystemNodeConnectionPath* inPath = 0;
    IVisSystemNodeDesc* outDesc = sender2->nodeDesc();
    IVisSystemNodeDesc* inDesc = receiver2->nodeDesc();
    if(!outDesc || !inDesc)
    {
        if(!d->genericConnect)
            qWarning("Invalid Sender/Receiver VTK Object(s)");
        return false;
    }

    outPath = outDesc->connectionPath(outLine);
    inPath  = inDesc->connectionPath(inLine);
    if(!outPath || !inPath)
    {
        if(!d->genericConnect)
            qWarning("Invalid Sender/Receiver VTK Object(s)");
        return false;
    }

    if( (outPath->pathType() == IVisSystemNodeConnectionPath::ControlPath && inPath->pathType() != IVisSystemNodeConnectionPath::ControlPath) &&
        (outPath->pathType() != IVisSystemNodeConnectionPath::ControlPath && inPath->pathType() == IVisSystemNodeConnectionPath::ControlPath) &&
        (outPath->pathType() != IVisSystemNodeConnectionPath::OutputPath  || inPath->pathType() != IVisSystemNodeConnectionPath::InputPath) )
    {
        if(!d->genericConnect)
            qWarning("Input/Output paths incompatible");
        return false;
    }

    bool success = true;

    if(outPath->pathType() == IVisSystemNodeConnectionPath::ControlPath && inPath->pathType() == IVisSystemNodeConnectionPath::ControlPath)
    {
        success = sender->fetchControlData(outPath, &senderControlData);
        if(!success)
        {
            QString errMsg = QString("%1 did not provide any control data at %1/%2")
                            .arg(sender->nodeName()).arg(outLine).arg(outPath->pathName());
            if(!d->genericConnect)
                qWarning("%s", qPrintable(errMsg));
            return false;
        }

        success = receiver->fetchControlData(inPath, &receiverControlData);
        if(!success)
        {
            QString errMsg = QString("%1 did not provide any control data at %1/%2")
                            .arg(receiver->nodeName()).arg(inLine).arg(inPath->pathName());
            if(!d->genericConnect)
                qWarning("%s", qPrintable(errMsg));
            return false;
        }

        success = sender->connectControl(outPath, receiverControlData);
        if(!success)
        {
            QString errMsg = QString("Cannot connect to control path %1 in %2")
                                .arg(outPath->pathName()).arg(sender->nodeName());
            if(!d->genericConnect)
                qWarning("%s", qPrintable(errMsg));
            return false;
        }

        success = receiver->connectControl(inPath, senderControlData);
        if(!success)
        {
            QString errMsg = QString("Cannot connect to control path %1 in %2")
                                .arg(inPath->pathName()).arg(receiver->nodeName());
            if(!d->genericConnect)
                qWarning("%s", qPrintable(errMsg));
            sender->disconnectControl(outPath, receiverControlData);
            return false;
        }
    }
    else
    {
        success = receiver->hasInput(inPath);
        if(success && !inPath->allowMultipleInputs())
        {
            QString errorMessage = QString("%1 did not accept multiple inputs at %2/%3")
                .arg(receiver->nodeName())
                .arg(inLine).arg(inPath->pathName());
            if(!d->genericConnect)
                qWarning("%s", qPrintable(errorMessage));
            return false;
        }

        success = sender->fetchOutput(outPath, &data);
        if(!success || !data)
        {
            QString errorMessage = QString("%1 did not provide any output at %2/%3").arg(sender->nodeName().toAscii().data())
                                        .arg(outLine).arg(outPath->pathName());
            if(!d->genericConnect)
                qWarning("%s", qPrintable(errorMessage));
            return false;
        }

        success = receiver->setInput(inPath, data);
        if(!success)
        {
            QString errorMessage = QString("%1 did not accept input at %2/%3").arg(receiver->nodeName().toAscii().data())
                                        .arg(inLine).arg(inPath->pathName());
            if(!d->genericConnect)
                qWarning("%s", qPrintable(errorMessage));
            sender->outputDerefed(outPath, data);
            return false;
        }
    }

    // Create a new conneciton.
    CVisNetworkConnection* con = new CVisNetworkConnection;
    con->setSenderNode(sender2);
    con->setReceiverNode(receiver2);
    con->setSenderPathIndex(outLine);
    con->setReceiverPathIndex(inLine);
    con->setConnectionData(data);
    con->setSenderControlData(senderControlData);
    con->setReceiverControlData(receiverControlData);

    // Append the connection to the list of connections in the system.
    d->connectionList.append(con);
    on_connectionCreated(con);

    // Return the connection to the caller
    if(connection)
        *connection = con;

    // Announce the connection creation event to event listeners.
    emit connectionCreated(con);
    emit projectPartModified();

    return true;
}

bool CVisSystemCanvas::disconnectNode(IVisSystemNode* sender, int outLine, IVisSystemNode* receiver, int inLine)
{
    for(int i=0; i<d->connectionList.count(); i++)
    {
        IVisNetworkConnection* con = d->connectionList[i];
        if( con->senderNode() == sender &&
            con->receiverNode() == receiver &&
            con->senderPathIndex() == outLine &&
            con->receiverPathIndex() == inLine )
            return disconnectNode(con);
    }

    return false;
}

bool CVisSystemCanvas::disconnectNode(IVisNetworkConnection* con)
{
    IVisSystemNode* sender = con->senderNode();
    IVisSystemNode* receiver = con->receiverNode();
    int outLine = con->senderPathIndex();
    int inLine = con->receiverPathIndex();
    IVisSystemNodeIOData* data = con->connectionData();
    IVisSystemNodeControlData* senderControlData = con->senderControlData();
    IVisSystemNodeControlData* receiverControlData = con->receiverControlData();
    if(!sender || !receiver)
    {
        qWarning("Invalid connection state");
        return false;
    }

    IVisSystemNodeConnectionPath* outPath = 0;
    IVisSystemNodeConnectionPath* inPath = 0;
    IVisSystemNodeDesc* outDesc = sender->nodeDesc();
    IVisSystemNodeDesc* inDesc = receiver->nodeDesc();
    if(!outDesc || !inDesc)
    {
        qWarning("Invalid Sender/Receiver VTK Object(s)");
        return false;
    }

    outPath = outDesc->connectionPath(outLine);
    inPath  = inDesc->connectionPath(inLine);
    if(!outPath || !inPath)
    {
        qWarning("Invalid Sender/Receiver VTK Object(s)");
        return false;
    }

    if( (outPath->pathType() == IVisSystemNodeConnectionPath::ControlPath && inPath->pathType() != IVisSystemNodeConnectionPath::ControlPath) &&
        (outPath->pathType() != IVisSystemNodeConnectionPath::ControlPath && inPath->pathType() == IVisSystemNodeConnectionPath::ControlPath) &&
        (outPath->pathType() != IVisSystemNodeConnectionPath::OutputPath  || inPath->pathType() != IVisSystemNodeConnectionPath::InputPath) )
    {
        qWarning("Input/Output paths incompatible");
        return false;
    }

    bool success = true;

    if(outPath->pathType() == IVisSystemNodeConnectionPath::ControlPath && inPath->pathType() == IVisSystemNodeConnectionPath::ControlPath)
    {
        if(!senderControlData || !receiverControlData)
        {
            qWarning("Invalid connection state");
            return false;
        }

        success = sender->disconnectControl(outPath, receiverControlData);
        if(!success)
        {
            QString errMsg = QString("Could not disconnect control path %1/%2").arg(sender->nodeName()).arg(outPath->pathName());
            qWarning("%s", qPrintable(errMsg));
            return false;
        }

        success = receiver->disconnectControl(inPath, senderControlData);
        if(!success)
        {
            QString errMsg = QString("Could not disconnect control path %1/%2").arg(receiver->nodeName()).arg(inPath->pathName());
            qWarning("%s", qPrintable(errMsg));
            return false;
        }
    }
    else
    {
        if(!data)
        {
            qWarning("Invalid connection state");
            return false;
        }

        success = receiver->removeInput(inPath, data);
        if(!success)
        {
            QString errMsg = QString("Cannot remove input from %1/%2 of %3")
                                .arg(inLine).arg(inPath->pathName())
                                .arg(receiver->nodeName());
            qWarning("%s", qPrintable(errMsg));
            return false;
        }

        success = sender->outputDerefed(outPath, data);
        if(!success)
        {
            QString errMsg = QString("Sender did not dereference output data");
            qWarning("%s", qPrintable(errMsg));
            return false;
        }
    }

    // Announce the connection creation event to event listeners.
    emit connectionDestroyed(con);
    emit projectPartModified();

    // Unregister and delete the connection.
    on_connectionDestroyed(con);
    d->connectionList.removeAll(con);
    con->destroySelf();

    return true;
}

bool CVisSystemCanvas::disconnectNode(IVisSystemNode* sender, IVisSystemNode* receiver)
{
    IVisNetworkConnection* con = 0;
    QList<IVisNetworkConnection*> conList;
    bool success = true;
    Q_FOREACH(con, d->connectionList)
    {
        if(con->senderNode() == sender && con->receiverNode() == receiver)
            conList.append(con);
    }
    Q_FOREACH(con, conList)
        success &= disconnectNode(con);
    return success;
}

bool CVisSystemCanvas::disconnectNode(IVisSystemNode* node)
{
    IVisNetworkConnection* con = 0;
    QList<IVisNetworkConnection*> conList;
    bool success = true;
    Q_FOREACH(con, d->connectionList)
    {
        if(con->senderNode() == node || con->receiverNode() == node)
            conList.append(con);
    }
    Q_FOREACH(con, conList)
    {
        if(d->captureNodeDeletion)
        {
            CNodeDisconnectionCmd* cmd = new CNodeDisconnectionCmd(
                con->senderNode(), con->senderPathIndex(),
                con->receiverNode(), con->receiverPathIndex());
            d->undoStack.push(cmd);
        }
        success &= disconnectNode(con);
    }
    return success;
}

int CVisSystemCanvas::connectionCount()
{
    return d->connectionList.count();
}

IVisNetworkConnection* CVisSystemCanvas::connection(int index)
{
    if(index < 0 || index >= d->connectionList.count())
        return 0;

    return d->connectionList[index];
}

bool CVisSystemCanvas::hasConnection(IVisNetworkConnection* con)
{
    return d->connectionList.contains(con);
}

void CVisSystemCanvas::drawBackground(QPainter * paint, const QRectF & rect)
{
    QLinearGradient grad(QPointF(0,0), QPointF(0,1000));
    grad.setColorAt(0.0, d->bgTopColor);
    grad.setColorAt(0.5, d->bgMidColor);
    grad.setColorAt(1.0, d->bgBottomColor);
    paint->fillRect(rect, grad);
    paint->fillRect(rect, QBrush(d->bgMidColor, Qt::CrossPattern));
}

void CVisSystemCanvas::paintEvent(QPaintEvent* pe)
{
    QGraphicsView::paintEvent(pe);
}

void CVisSystemCanvas::mousePressEvent(QMouseEvent* me)
{
    d->senderNode = 0;
    d->senderPath = -1;
    d->receiverNode = 0;
    d->receiverPath = -1;

    CVisSystemCanvasNodeItem* nodeItem = d->nodeItemAt(me->pos());
    if(nodeItem && nodeItem->visSystemNode())
    {
        QPointF pos = mapToScene(me->pos());
        QPointF itemPos = nodeItem->mapFromScene(pos);
        IVisSystemNodeConnectionPath* path = nodeItem->pathAt(itemPos);
        if(me->modifiers()&Qt::ControlModifier)
        {
            pos = nodeItem->pos();
            d->conLine = new QGraphicsLineItem(0, d->scene);
            d->conLine->setPen(QPen(palette().highlight().color(), 2, Qt::DashLine));
            d->conLine->setZValue(10.0f);
            d->conLine->setLine(QLineF(pos, pos));
            d->conLine->show();
            d->senderNode = nodeItem->visSystemNode();
        }
        else if(path && path->pathType() != IVisSystemNodeConnectionPath::InputPath)
        {
            itemPos = nodeItem->pathRect(path).center();
            pos = nodeItem->mapToScene(itemPos);
            d->conLine = new QGraphicsLineItem(0, d->scene);
            d->conLine->setPen(QPen(palette().highlight().color(), 2, Qt::DashLine));
            d->conLine->setZValue(10.0f);
            d->conLine->setLine(QLineF(pos, pos));
            d->conLine->show();

            d->senderNode = nodeItem->visSystemNode();
            d->senderPath = path->pathIndex();
        }
    }

    if(!d->conLine)
    {
        QGraphicsView::mousePressEvent(me);
        if(!me->isAccepted() && !scene()->selectedItems().count())
        {
            d->panMode = true;
            d->panMousePos = QCursor::pos();
        }
    }
}

void CVisSystemCanvas::mouseMoveEvent(QMouseEvent* me)
{
    if(d->conLine)
    {
        QLineF line = d->conLine->line();
        QPointF pos = mapToScene(me->pos());
        line = QLineF(line.p1(), pos);
        d->conLine->setLine(line);
    }
    else if(d->panMode)
    {
        QPoint pos = QCursor::pos();
        QPoint dp = d->panMousePos - pos;
        d->panMousePos = pos;
        horizontalScrollBar()->setValue(horizontalScrollBar()->value()+dp.x());
        verticalScrollBar()->setValue(verticalScrollBar()->value()+dp.y());

        emit panEvent(dp.x(), dp.y());
    }
    else
        QGraphicsView::mouseMoveEvent(me);
}

void CVisSystemCanvas::mouseReleaseEvent(QMouseEvent* me)
{
    if(d->conLine)
    {
        d->conLine->hide();
        delete d->conLine;

        CVisSystemCanvasNodeItem* nodeItem = d->nodeItemAt(me->pos());
        if(nodeItem && d->senderNode)
        {
            int conCount = d->connectionList.count();

            d->receiverNode = nodeItem->visSystemNode();
            if(d->senderPath >= 0)
            {
                QPointF pos = mapToScene(me->pos());
                QPointF itemPos = nodeItem->mapFromScene(pos);
                IVisSystemNodeConnectionPath* path = nodeItem->pathAt(itemPos);
                if(path && path->pathType() != IVisSystemNodeConnectionPath::OutputPath)
                {
                    d->receiverPath = path->pathIndex();
                    d->network->connectNode(d->senderNode, d->senderPath, d->receiverNode, d->receiverPath, 0);
                }
            }
            else
                d->network->connectNode(d->senderNode, d->receiverNode);

            if(d->connectionList.count() > conCount)
            {
                for(int i=conCount; i<d->connectionList.count(); i++)
                {
                    IVisNetworkConnection* con = d->connectionList[i];
                    CNodeConnectionCmd* cmd = new CNodeConnectionCmd(
                            con->senderNode(), con->senderPathIndex(), 
                            con->receiverNode(), con->receiverPathIndex());
                    d->undoStack.push(cmd);
                }
            }
        }
    }
    else if(d->panMode)
    {
        d->panMode = false;
    }
    else
    {
        QGraphicsView::mouseReleaseEvent(me);
        
        if(!itemAt(me->pos()))
        {
            QPoint pos = mapToScene(me->pos()).toPoint();
            emit canvasClickedEvent(this, pos, me->button(), me->modifiers());
        }
    }

    d->conLine = 0;
    d->senderNode = 0;
    d->receiverNode = 0;
    d->senderPath = -1;
    d->receiverPath = -1;
}

void CVisSystemCanvas::mouseDoubleClickEvent(QMouseEvent* me)
{
    QGraphicsView::mouseDoubleClickEvent(me);

    if(!itemAt(me->pos()))
    {
        QPoint pos = mapToScene(me->pos()).toPoint();
        emit canvasDoubleClickedEvent(this, pos, me->button(), me->modifiers());
    }
}

void CVisSystemCanvas::contextMenuEvent(QContextMenuEvent* me)
{
    QGraphicsView::contextMenuEvent(me);

    if(!itemAt(me->pos()))
    {
        QMenu menu;

        QAction* addNote = menu.addAction("Add Note");

        emit canvasContextMenuEvent(this, &menu, me->modifiers());
        if(menu.actions().count())
        {
            QAction* result = menu.exec(QCursor::pos());
            if(result == addNote)
            {
                QPointF pos = mapToScene(me->pos());
                this->addNote("Double click and type your note here..", pos.x(), pos.y(), 200, 200);
            }
        }
    }
}

void CVisSystemCanvas::wheelEvent(QWheelEvent* e)
{
    int delta = int(e->delta()) / 120;
    setZoomLevel(delta + d->zoomLevel);
    e->accept();
}

void CVisSystemCanvas::keyPressEvent(QKeyEvent* ke)
{
    switch(ke->key())
    {
    case Qt::Key_Delete:
        if(d->scene->selectedItems().count())
        {
            QGraphicsItem* item = d->scene->selectedItems().first();

            CVisSystemCanvasNodeItem* nodeItem = dynamic_cast<CVisSystemCanvasNodeItem*>(item);
            if(nodeItem)
            {
                d->captureNodeDeletion = true;
                d->network->removeNode(nodeItem->visSystemNode());
                d->captureNodeDeletion = false;
                return;
            }

            CVisSystemCanvasConnectionItem* conItem = dynamic_cast<CVisSystemCanvasConnectionItem*>(item);
            if(conItem)
            {
                IVisNetworkConnection* con = conItem->connection();
                CNodeDisconnectionCmd* cmd = new CNodeDisconnectionCmd(
                    con->senderNode(), con->senderPathIndex(),
                    con->receiverNode(), con->receiverPathIndex());
                d->undoStack.push(cmd);
                d->network->disconnectNode(conItem->connection());
                return;
            }
        }
        break;
#ifdef VW_ENABLE_EASTER_EGGS
    case Qt::Key_Right:
    case Qt::Key_Left: {
        bool control = ke->modifiers()&Qt::ControlModifier;
        if(!control)
            break;

        QTransform transform;
        if(ke->key() == Qt::Key_Right)
            d->rotationAngle += 5.0f;
        else
            d->rotationAngle -= 5.0f;
        transform = transform.rotate(d->rotationAngle, Qt::YAxis);
        setTransform(transform);
        } break;
#endif
    default:
        break;
    }
    QGraphicsView::keyPressEvent(ke);

    if(!d->scene->focusItem())
        emit canvasKeyEvent(this, ke->key(), ke->text(), ke->modifiers());
}

void CVisSystemCanvas::showEvent(QShowEvent* se)
{
    zoomOne();
    QGraphicsView::showEvent(se);
}

void CVisSystemCanvas::hideEvent(QHideEvent* he)
{
    QGraphicsView::hideEvent(he);
}

void CVisSystemCanvas::dragEnterEvent(QDragEnterEvent* de)
{
    if(!d->network)
        return;

    QWidget* sender = de->source();
    IVisSystemNodeFactoryRegistryExplorer* ncExp = qobject_cast<IVisSystemNodeFactoryRegistryExplorer*>(sender);
    if(!ncExp)
        return;
    de->acceptProposedAction();
}

void CVisSystemCanvas::dragMoveEvent(QDragMoveEvent* de)
{
    if(!d->network)
        return;

    QWidget* sender = de->source();
    IVisSystemNodeFactoryRegistryExplorer* ncExp = qobject_cast<IVisSystemNodeFactoryRegistryExplorer*>(sender);
    if(!ncExp)
        return;
    de->acceptProposedAction();
}

void CVisSystemCanvas::dropEvent(QDropEvent* de)
{
    if(!d->network)
        return;

    QWidget* sender = de->source();
    IVisSystemNodeFactoryRegistryExplorer* ncExp = qobject_cast<IVisSystemNodeFactoryRegistryExplorer*>(sender);
    if(!ncExp)
        return;
    de->acceptProposedAction();
    QString className = de->mimeData()->text();
    IVisSystemNode* node = d->nodeFactoryReg->createNode(className);
    if(node)
    {
        d->network->addNode(node);
        CVisSystemCanvasNodeItem* nodeItem = this->nodeItem(node);
        if(!nodeItem)
            return;
        QPointF pos = mapToScene(de->pos());
        nodeItem->setPos(pos);

        d->undoStack.push( new CNodeCreateCmd(node) );
    }
}

void CVisSystemCanvas::itemSelected(QGraphicsItem* item)
{
    Q_UNUSED(item);
}

void CVisSystemCanvas::bringToFront(QGraphicsItem* item)
{
    Q_UNUSED(item);
}

void CVisSystemCanvas::sendToBack(QGraphicsItem* item)
{
    Q_UNUSED(item);
}

bool CVisSystemCanvas::eventFilter(QObject* obj, QEvent* event)
{
    if(obj != this->scene())
        return false;

    if(event->type() != QEvent::GraphicsSceneHelp)
        return false;

    QGraphicsSceneHelpEvent* he = (QGraphicsSceneHelpEvent*)(event);
    QGraphicsItem* item = scene()->itemAt( he->scenePos() );
    if(!item || item->type() != CVisSystemCanvasNodeItem_Type)
        return false;

    CVisSystemCanvasNodeItem* nodeItem = static_cast<CVisSystemCanvasNodeItem*>(item);
    QRectF rect;
    QPointF pos = item->mapFromScene( he->scenePos() );
    QString toolTip = nodeItem->toolTipAt(pos, rect);
    if(toolTip.isEmpty())
        return false;

    QRectF sceneRect = item->mapToScene( sceneRect ).boundingRect();
    QRect screenRect = mapFromScene( sceneRect ).boundingRect();
    QToolTip::showText(he->screenPos(), toolTip, viewport(), screenRect);
    return true;
}

void CVisSystemCanvas::on_scene_changed()
{
    QRectF rect = scene()->sceneRect();

    // ensure symetry around the center of the canvas.
    qreal left = rect.left();
    qreal right = rect.right();
    qreal top = rect.top();
    qreal bottom = rect.bottom();

    if(qAbs(left) > qAbs(right))
        right = -left;
    else
        left = -right;
    if(qAbs(top) > qAbs(bottom))
        bottom = -top;
    else
        top = -bottom;

    setSceneRect(left, top, (right-left), (bottom-top));
    update();
}

void CVisSystemCanvas::on_nodeItem_itemSelected(bool val)
{
    CVisSystemCanvasNodeItem* nodeItem = qobject_cast<CVisSystemCanvasNodeItem*>(sender());
    if(!nodeItem || nodeItem == d->selectedNodeItem)
        return;
    if(!d->selectedNodeItem.isNull())
        d->selectedNodeItem->setSelected(false);
    d->selectedNodeItem = nodeItem;

    if(!d->updatePropertyEditor)
        return;

    IVisSystemNode* node = nodeItem->visSystemNode();
    QObject* nodeObject = node->containerObject();

    if(val)
        d->propertyEditor->setObject(nodeObject);
    else
    {
        if(d->propertyEditor->object() == nodeObject)
            d->propertyEditor->setObject(0);
    }
}

void CVisSystemCanvas::on_connectionItem_itemSelected(bool val)
{
    Q_UNUSED(val);
}

void CVisSystemCanvas::on_connectionCreated(IVisNetworkConnection* connection)
{
    if(d->connectionMap.contains(connection))
        return;

    CVisSystemCanvasConnectionItem* conItem = new CVisSystemCanvasConnectionItem(connection, this);
    d->connectionMap[connection] = conItem;
    d->scene->addItem(conItem);
    conItem->show();
}

void CVisSystemCanvas::on_connectionDestroyed(IVisNetworkConnection* connection)
{
    if(!d->connectionMap.contains(connection))
        return;

    CVisSystemCanvasConnectionItem* conItem = d->connectionMap[connection];
    delete conItem;
    d->connectionMap.remove(connection);
}

void CVisSystemCanvas::on_propertyEditor_propertyValueChanged(QObject* obj, QString propName, QVariant oldVal, QVariant newVal)
{
    if(!obj || propName.isEmpty())
        return;

    IVisSystemNode* node = qobject_cast<IVisSystemNode*>(obj);
    if(!node || !d->nodeList.contains(node))
        return;

    if(oldVal == newVal)
        return;

    CNodePropertyChangeCmd* cmd = new CNodePropertyChangeCmd(obj, propName, oldVal, newVal);
    d->undoStack.push(cmd);
}

QObject* CVisSystemCanvas::addNode(QString className)
{
    if(!d->nodeFactoryReg)
        return 0;

    if(!d->nodeFactoryReg->canCreate(className))
        return 0;

    IVisSystemNode* node = d->nodeFactoryReg->createNode(className);
    if(!node)
        return 0;

    this->addNode(node);
    return node->containerObject();
}

bool CVisSystemCanvas::deleteNode(QObject* nodeObject)
{
    IVisSystemNode* node = qobject_cast<IVisSystemNode*>(nodeObject);
    if(node)
    {
        this->removeNode(node);
        return true;
    }
    return false;
}

bool CVisSystemCanvas::deleteAllNodes()
{
    this->removeAllNodes();
    return this->nodeCount() == 0;
}

int CVisSystemCanvas::getNodeCount()
{
    return this->nodeCount();
}

QObject* CVisSystemCanvas::getNode(int index)
{
    IVisSystemNode* node = this->node(index);
    if(node)
        return node->containerObject();
    return 0;
}

QList<QObject*> CVisSystemCanvas::getNodesOfClass(QString nodeClassName)
{
    QList<QObject*> ret;
    for(int i=0; i<d->nodeList.count(); i++)
    {
        IVisSystemNode* node = d->nodeList[i];
        if(nodeClassName.isEmpty() || node->nodeDesc()->nodeClassName() == nodeClassName)
            ret.append(node->containerObject());
    }
    return ret;
}

QList<QObject*> CVisSystemCanvas::getNodesOfName(QString nodeName, bool startsWith, bool contains)
{
    QList<QObject*> ret;
    for(int i=0; i<d->nodeList.count(); i++)
    {
        IVisSystemNode* node = d->nodeList[i];
        if(nodeName.isEmpty())
            ret.append(node->containerObject());
        else
        {
            if(startsWith && node->nodeName().startsWith(nodeName))
                ret.append(node->containerObject());
            else if(contains && node->nodeName().contains(nodeName))
                ret.append(node->containerObject());
        }
    }
    return ret;
}

bool CVisSystemCanvas::connectNode(QObject* senderNodeObj, QObject* receiverNodeObj)
{
    if(!senderNodeObj || !receiverNodeObj)
        return false;

    IVisSystemNode* senderNode = qobject_cast<IVisSystemNode*>(senderNodeObj);
    IVisSystemNode* receiverNode = qobject_cast<IVisSystemNode*>(receiverNodeObj);
    if(!senderNode || !receiverNode)
        return false;

    return this->connectNode( senderNode, receiverNode );
}

bool CVisSystemCanvas::connectNode(QObject* senderNodeObj, int outLine, QObject* receiverNodeObj, int inLine)
{
    if(!senderNodeObj || !receiverNodeObj)
        return false;

    IVisSystemNode* senderNode = qobject_cast<IVisSystemNode*>(senderNodeObj);
    IVisSystemNode* receiverNode = qobject_cast<IVisSystemNode*>(receiverNodeObj);

    return this->connectNode(senderNode, outLine, receiverNode, inLine, 0);
}

bool CVisSystemCanvas::disconnectNode(QObject* senderNodeObj, int outLine, QObject* receiverNodeObj, int inLine)
{
    if(!senderNodeObj || !receiverNodeObj)
        return false;

    IVisSystemNode* senderNode = qobject_cast<IVisSystemNode*>(senderNodeObj);
    IVisSystemNode* receiverNode = qobject_cast<IVisSystemNode*>(receiverNodeObj);

    for(int i=0; i<d->connectionList.count(); i++)
    {
        IVisNetworkConnection* con = d->connectionList[i];
        if(con->senderNode() == senderNode && con->receiverNode() == receiverNode &&
           con->senderPathIndex() == outLine && con->receiverPathIndex() == inLine)
        {
            this->disconnectNode(con);
            return true;
        }
    }

    return false;
}

bool CVisSystemCanvas::disconnectNode(QObject* senderNodeObj, QObject* receiverNodeObj)
{
    if(!senderNodeObj || !receiverNodeObj)
        return false;

    IVisSystemNode* senderNode = qobject_cast<IVisSystemNode*>(senderNodeObj);
    IVisSystemNode* receiverNode = qobject_cast<IVisSystemNode*>(receiverNodeObj);

    return this->disconnectNode(senderNode, receiverNode);
}

bool CVisSystemCanvas::disconnectNode(QObject* nodeObject)
{
    if(!nodeObject)
        return false;

    IVisSystemNode* node = qobject_cast<IVisSystemNode*>(nodeObject);
    return this->disconnectNode(node);
}

///////////////////////////////////////////////////////////////////////////////////////////////
// The following methods are for calling from remote methods only

bool CVisSystemCanvas::addNode(QString nodeClassName, QString nodeName)
{
    QObject* newNode = this->addNode(nodeClassName);
    IVisSystemNode* node = qobject_cast<IVisSystemNode*>(newNode);
    if(node)
        node->setNodeName(nodeName);

    return node != 0;
}

bool CVisSystemCanvas::removeNode(QString nodeName)
{
    IVisSystemNode* node = d->findNode(nodeName);
    if( !node )
        return false;

    removeNode(node);
    return true;
}

bool CVisSystemCanvas::connectNode(QString senderName, QString receiverName)
{
    IVisSystemNode* senderNode = d->findNode(senderName);
    IVisSystemNode* receiverNode = d->findNode(receiverName);

    return connectNode(senderNode, receiverNode);
}

bool CVisSystemCanvas::connectNode(QString senderName, int outLine, QString receiverName, int inLine)
{
    IVisSystemNode* senderNode = d->findNode(senderName);
    IVisSystemNode* receiverNode = d->findNode(receiverName);

    IVisNetworkConnection* con = 0;
    return connectNode(senderNode, outLine, receiverNode, inLine, &con);
}

bool CVisSystemCanvas::disconnectNode(QString senderName, int outLine, QString receiverName, int inLine)
{
    IVisSystemNode* senderNode = d->findNode(senderName);
    IVisSystemNode* receiverNode = d->findNode(receiverName);

    return disconnectNode(senderNode, outLine, receiverNode, inLine);
}

bool CVisSystemCanvas::disconnectNode(QString senderName, QString receiverName)
{
    IVisSystemNode* senderNode = d->findNode(senderName);
    IVisSystemNode* receiverNode = d->findNode(receiverName);

    return disconnectNode(senderNode, receiverNode);
}

bool CVisSystemCanvas::disconnectNode(QString nodeName)
{
    IVisSystemNode* node = d->findNode(nodeName);

    return disconnectNode(node);
}

QString CVisSystemCanvas::getNodeName(int index)
{
    if( index < 0 || index >= d->nodeList.count() )
        return QString();

    return d->nodeList[index]->nodeName();
}

QStringList CVisSystemCanvas::getNodesNames()
{
    QStringList ret;

    for(int i=0; i<d->nodeList.count(); i++)
        ret << d->nodeList[i]->nodeName();

    return ret;
}

QStringList CVisSystemCanvas::getNodeNamesOfClass(QString nodeClassName)
{
    QStringList ret;

    for(int i=0; i<d->nodeList.count(); i++)
    {
        IVisSystemNode* node = d->nodeList[i];
        if( node->nodeDesc()->nodeClassName() == nodeClassName )
            ret << node->nodeName();
    }

    return ret;
}

QStringList CVisSystemCanvas::getNodeNamesOfName(QString nodeName, bool startsWith, bool contains)
{
    QStringList ret;

    for(int i=0; i<d->nodeList.count(); i++)
    {
        IVisSystemNode* node = d->nodeList[i];
        QString name = node->nodeName();
        if( startsWith && name.startsWith(nodeName) )
            ret << name;
        else if( contains && name.contains(nodeName) )
            ret << name;
    }

    return ret;
}

///////////////////////////////////////////////////////////////////////////////////////////////

QString CVisSystemCanvas::projectPartID()
{
    return "VisNetwork";
}

bool CVisSystemCanvas::newProjectPart()
{
    this->newCanvas();
    return true;
}

bool CVisSystemCanvas::loadProjectPart(QDomDocument doc, QDomElement partE)
{
    this->newCanvas();
    if(!d->xmlUtils || !d->nodeFactoryReg || !d->qobjectLoadSave)
        return false;

    d->updatePropertyEditor = false;
    QMap<QString, IVisSystemNode*> nameNodeMap;

    QList<QDomElement> nodeEList = d->xmlUtils->findElements(partE, "Node");
    for(int i=0; i<nodeEList.count(); i++)
    {
        QDomElement nodeE = nodeEList[i];

        QString className = nodeE.attribute("ClassName");
        IVisSystemNode* node = d->nodeFactoryReg->createNode(className);
        if(!node)
            continue;
        d->network->addNode(node);
        CVisSystemCanvasNodeItem* nodeItem = this->nodeItem(node);
        if(!nodeItem)
            continue;
        QPointF pos = QPointF(nodeE.attribute("X").toDouble(), nodeE.attribute("Y").toDouble());
        nodeItem->setPos(pos);
        node->setNodeName(nodeE.attribute("Name"));

        nameNodeMap[node->nodeName()] = node;

        // Load properties.
        d->qobjectLoadSave->loadProperties(node->containerObject(), doc, nodeE);
    }

    QList<QDomElement> conEList = d->xmlUtils->findElements(partE, "Connection");
    for(int i=0; i<conEList.count(); i++)
    {
        IVisNetworkConnection* con = 0;
        QDomElement conE = conEList[i];
        IVisSystemNode* sender = nameNodeMap[conE.attribute("Sender")];
        IVisSystemNode* receiver = nameNodeMap[conE.attribute("Receiver")];
        int sPathIndex = conE.attribute("SPathIndex").toInt();
        int rPathIndex = conE.attribute("RPathIndex").toInt();
        d->network->connectNode(sender, sPathIndex, receiver, rPathIndex, &con);
    }

	// Lets store the required geometry in a map. We can set the geometry into widgets
	// later
	d->widgetGeoMap.clear();
	QList<QDomElement> wrEList = d->xmlUtils->findElements(partE, "CWidget");
	for(int i=0; i<wrEList.count(); i++)
	{
		QDomElement wrE = wrEList[i];
		QRectF geo( wrE.attribute("X").toDouble(), wrE.attribute("Y").toDouble(),
					wrE.attribute("Width").toDouble(), wrE.attribute("Height").toDouble() );
		QString name = wrE.attribute("Name");

		d->widgetGeoMap[ name ] = geo;
	}

    // Load note information
    QList<QDomElement> noteEList = d->xmlUtils->findElements(partE, "Note");
    for(int i=0; i<noteEList.count(); i++)
    {
        CVisSystemCanvasNote* note = new CVisSystemCanvasNote(this);
		QDomElement noteE = noteEList[i];
		QRectF noteRect( noteE.attribute("X").toDouble(), noteE.attribute("Y").toDouble(),
					noteE.attribute("Width").toDouble(), noteE.attribute("Height").toDouble() );
        QString text = noteE.text();

        note->setNoteRect(noteRect);
        note->setNoteText(text);
        addNote(note);
    }

    d->updatePropertyEditor = true;

    return true;
}

bool CVisSystemCanvas::initializeProjectPart()
{
	for(int i=0; i<d->widgetList.count(); i++)
	{
		CVisSystemCanvasWidget* cWidget = d->widgetList[i];
		QString name = cWidget->widget()->objectName();
		if( !d->widgetGeoMap.contains(name) )
			continue;

		QRectF geo = d->widgetGeoMap[name];
		cWidget->setGeometry(geo);
	}
	d->widgetGeoMap.clear();

    this->zoomFit();
    return true;
}

bool CVisSystemCanvas::saveProjectPart(QDomDocument doc, QDomElement partParentE)
{
    if(!d->qobjectLoadSave)
        return false;

    // First save node information
    for(int i=0; i<d->nodeList.count(); i++)
    {
        IVisSystemNode* node = d->nodeList[i];

        // Save node information
        QDomElement nodeE = doc.createElement("Node");
        partParentE.appendChild(nodeE);
        nodeE.setAttribute("Name", node->nodeName());
        nodeE.setAttribute("ClassName", node->nodeDesc()->nodeClassName());
        nodeE.setAttribute("ID", QString("%1").arg(i));

        CVisSystemCanvasNodeItem* item = this->nodeItem(node);
        if(item)
        {
            QPointF pos = item->pos();
            nodeE.setAttribute("X", QString("%1").arg(pos.x()));
            nodeE.setAttribute("Y", QString("%1").arg(pos.y()));
        }

        // Save node property information
        d->qobjectLoadSave->saveProperties(node->containerObject(), doc, nodeE);
    }

    // Now save connectivity information
    for(int i=0; i<d->connectionList.count(); i++)
    {
        IVisNetworkConnection* con = d->connectionList[i];
        QDomElement conE = doc.createElement("Connection");
        conE.setAttribute("Sender", con->senderNode()->nodeName());
        conE.setAttribute("Receiver", con->receiverNode()->nodeName());
        conE.setAttribute("SPathIndex", QString("%1").arg(con->senderPathIndex()));
        conE.setAttribute("RPathIndex", QString("%1").arg(con->receiverPathIndex()));
        partParentE.appendChild(conE);
    }

	// Now save geometry of each and every form added to the canvas
	for(int i=0; i<d->widgetList.count(); i++)
	{
		CVisSystemCanvasWidget* cWidget = d->widgetList[i];
		QRectF geo = cWidget->geometry();
		QDomElement wrE = doc.createElement("CWidget");
		wrE.setAttribute("Name", cWidget->widget()->objectName());
		wrE.setAttribute("X", geo.x());
		wrE.setAttribute("Y", geo.y());
		wrE.setAttribute("Width", geo.width());
		wrE.setAttribute("Height", geo.height());
		partParentE.appendChild(wrE);
	}

    // Now save note information
    for(int i=0; i<d->noteList.count(); i++)
    {
        IVisNetworkCanvasNote* note = d->noteList[i];
        QRectF noteRect = note->noteRect();
        QDomElement noteE = doc.createElement("Note");
        partParentE.appendChild(noteE);
		noteE.setAttribute("X", noteRect.x());
		noteE.setAttribute("Y", noteRect.y());
		noteE.setAttribute("Width", noteRect.width());
		noteE.setAttribute("Height", noteRect.height());
        noteE.appendChild( doc.createTextNode(note->noteText()) );
    }

    return true;    
}

bool CVisSystemCanvas::closeProjectPart()
{
    this->newCanvas();
    return true;
}

void CVisSystemCanvas::addWidget(QWidget* widget, const QRectF& rect, const QPointF& pos)
{
    if(!widget)
        return;

    for(int i=0; i<d->widgetList.count(); i++)
    {
		if(d->widgetList[i]->widget() == widget)
			return;
    }

	QRectF geo = rect.adjusted(pos.x(), pos.y(), pos.x(), pos.y());

    CVisSystemCanvasWidget* cWidget = new CVisSystemCanvasWidget(this);
	cWidget->setWidget(widget);
	cWidget->setGeometry(geo);
	cWidget->show();

    d->widgetList << cWidget;
}

void CVisSystemCanvas::removeWidget(QWidget* widget)
{
    if(!widget)
        return;

    for(int i=0; i<d->widgetList.count(); i++)
    {
		CVisSystemCanvasWidget* cWidget = d->widgetList[i];
        if(cWidget->widget() == widget)
        {
            d->scene->removeItem(cWidget);
            d->widgetList.removeAt(i);
			delete cWidget;
            return;
        }
    }
}

int CVisSystemCanvas::widgetCount() const
{
    return d->widgetList.count();
}

QWidget* CVisSystemCanvas::widget(int index) const
{
    if(index < 0 || index >= d->widgetList.count())
        return 0;

    return d->widgetList[index]->widget();
}

void CVisSystemCanvas::setNoteSvgFile(const QString& noteSvg, const QString& elementId, bool qcompressed)
{
    if(d->noteSvgFile == noteSvg && d->noteSvgElementId == elementId)
        return;

    d->noteSvgFile = noteSvg;
    d->noteSvgElementId = elementId;
    d->noteSvgCompressed = qcompressed;

    if(d->noteSvgRenderer)
    {
        d->updateSvgRenderer();
        for(int i=0; i<d->nodeList.count(); i++)
            d->noteList[i]->setNoteSvgElementId(d->noteSvgElementId);
        update();
    }
}

QString CVisSystemCanvas::noteSvgFile() const
{
    return d->noteSvgFile;
}

QString CVisSystemCanvas::noteSvgElement() const
{
    return d->noteSvgElementId;
}

void CVisSystemCanvas::addNote(IVisNetworkCanvasNote* note)
{
    if(!note || d->noteList.contains(note))
        return;

    d->updateSvgRenderer();

    // Register the note
    d->noteList.append(note);
    note->setNoteSvgRenderer(d->noteSvgRenderer);
    note->setNoteSvgElementId(d->noteSvgElementId);
    this->scene()->addItem( note->graphicsItem() );
    showNotes();
    note->graphicsItem()->show();

    emit noteAdded(note);
}

void CVisSystemCanvas::removeNote(IVisNetworkCanvasNote* note)
{
    if(!note || !d->noteList.contains(note))
        return;

    d->noteList.removeAll(note);
    this->scene()->removeItem( note->graphicsItem() );

    emit noteRemoved(note);
}

int CVisSystemCanvas::noteCount() const
{
    return d->noteList.count();
}

IVisNetworkCanvasNote* CVisSystemCanvas::note(int index) const
{
    if(index < 0 || index >= d->noteList.count())
        return 0;

    return d->noteList[index];
}

void CVisSystemCanvas::showNotes()
{
    for(int i=0; i<d->noteList.count(); i++)
    {
        IVisNetworkCanvasNote* note = d->noteList[i];
        if(note->graphicsItem()->isVisible())
            return;

        note->graphicsItem()->show();
    }
}

void CVisSystemCanvas::hideNotes()
{
    for(int i=0; i<d->noteList.count(); i++)
    {
        IVisNetworkCanvasNote* note = d->noteList[i];
        if(!note->graphicsItem()->isVisible())
            return;

        note->graphicsItem()->hide();
    }
}

void CVisSystemCanvas::setNotesVisible(bool val)
{
    if(val)
        hideNotes();
    else
        showNotes();
}

void CVisSystemCanvas::addNote()
{
    CVisSystemCanvasNote* note = new CVisSystemCanvasNote(this);
    note->setNoteText("Double click and type your note here..");
    addNote(note);
}

void CVisSystemCanvas::addNote(const QString& text, double x, double y, double w, double h)
{
    CVisSystemCanvasNote* note = new CVisSystemCanvasNote(this);
    addNote(note);
    note->setNoteRect( QRectF(x, y, w, h) );
    note->setNoteText( text );
}

void CVisSystemCanvas::captureSnapshotToClipboard()
{
    QApplication::setOverrideCursor(Qt::WaitCursor);
    QRectF sceneRect = d->scene->itemsBoundingRect();
    QPixmap pixmap( sceneRect.size().toSize() );
    pixmap.fill(Qt::white);

    QPainter paint(&pixmap);
    paint.setRenderHints(QPainter::Antialiasing|QPainter::TextAntialiasing|QPainter::SmoothPixmapTransform);
    d->scene->render(&paint, QRectF(), sceneRect);
    paint.end();

    QClipboard* clipboard = QApplication::clipboard();
    clipboard->setPixmap(pixmap);
    QApplication::restoreOverrideCursor();
}

void CVisSystemCanvas::connectNotify(const char* signal)
{
    const char* compareSignal 
        = SIGNAL(nodeDoubleClickedEvent(IVisSystemNode,QPoint,Qt::MouseButton,Qt::KeyboardModifiers));
    if( QLatin1String(signal) == compareSignal )
        ++d->doubleClickConnectCount;
}

void CVisSystemCanvas::disconnectNotify(const char* signal)
{
    const char* compareSignal 
        = SIGNAL(nodeDoubleClickedEvent(IVisSystemNode,QPoint,Qt::MouseButton,Qt::KeyboardModifiers));
    if( QLatin1String(signal) == compareSignal )
        --d->doubleClickConnectCount;
}

bool CVisSystemCanvas::isDoubleClickSignalConnected() const
{
    return d->doubleClickConnectCount > 0;
}

IPropertyEditor* CVisSystemCanvas::getNewPropertyEditorInstance() const
{
    return CVisSystemCanvasComponent::instance().getNewPropertyEditorInstance();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// CVisSystemCanvasData implementation
/////////////////////////////////////////////////////////////////////////////////////////////////////////

double CVisSystemCanvasData::calculateScaleFor(int zoomLevel) const
{
    double boardWidth = scene->sceneRect().width();
    double boardHeight = scene->sceneRect().height();
    double boardSize = boardWidth > boardHeight ? boardWidth : boardHeight;
    double viewSize = boardWidth > boardHeight ? 
                        double(canvas->viewport()->width()) : 
                        double(canvas->viewport()->height());

    // At zoom level -10 boardSize must become viewSize/3;
    double zoomOutFactor = (viewSize*0.95f*0.33 / boardSize);

    // At zoom level +10 boardSize must be 3 times its size.
    double zoomInFactor = 3.0f / 10.0f;

    // Calculate the zoom
    double zoom = 1.0f;
    if(zoomLevel < 0)
        zoom = 1.0f + double(zoomLevel) * (1.0f - zoomOutFactor) / 10.0f;
    else
        zoom = 1.0f + zoomInFactor * double(zoomLevel);

    return zoom;
}

int CVisSystemCanvasData::calculateZoomLevelFor(double scale) const
{
    double boardWidth = scene->sceneRect().width();
    double boardHeight = scene->sceneRect().height();
    double boardSize = boardWidth > boardHeight ? boardWidth : boardHeight;
    double viewSize = boardWidth > boardHeight ? 
                        double(canvas->viewport()->width()) : 
                        double(canvas->viewport()->height());

    // At zoom level -10 boardSize must become viewSize;
    double zoomOutFactor = (viewSize*0.95f / boardSize);

    // At zoom level +10 boardSize must be 3 times its size.
    double zoomInFactor = 3.0f / 10.0f;

    double zoomLevel = 1.0f;

    if(scale < 1)
        zoomLevel = (scale - 1.0f) * 10.0f / (1.0f - zoomOutFactor);
    else
        zoomLevel = (scale - 1.0f) / zoomInFactor;

    return int(zoomLevel);
}

CVisSystemCanvasNodeItem* CVisSystemCanvasData::nodeItemAt(QPointF pos, bool scenePos)
{
    if(!this->network)
        return 0;

    QPointF sPos = pos;
    if(!scenePos)
        sPos = canvas->mapToScene(pos.toPoint());
    QList<QGraphicsItem*> items = scene->items(sPos);
    for(int i=0; i<items.count(); i++)
    {
        CVisSystemCanvasNodeItem* nodeItem = dynamic_cast<CVisSystemCanvasNodeItem*>(items[i]);
        if(nodeItem)
            return nodeItem;
    }
    return 0;
}


CVisSystemCanvasConnectionItem* CVisSystemCanvasData::connectionItemAt(QPointF pos, bool scenePos)
{
    if(!this->network)
        return 0;

    QPointF sPos = pos;
    if(!scenePos)
        sPos = canvas->mapToScene(pos.toPoint());
    QList<QGraphicsItem*> items = scene->items(sPos);
    for(int i=0; i<items.count(); i++)
    {
        CVisSystemCanvasConnectionItem* conItem = dynamic_cast<CVisSystemCanvasConnectionItem*>(items[i]);
        if(conItem)
            return conItem;
    }
    return 0;
}

void CVisSystemCanvasData::updateSvgRenderer()
{
    // Create the renderer if not already done so
    if(!this->noteSvgRenderer)
        this->noteSvgRenderer = new QSvgRenderer(this->canvas);

    QFile file(this->noteSvgFile);
    file.open(QFile::ReadOnly);

    QByteArray svgData = file.readAll();
    file.close();

    if(this->noteSvgCompressed)
        svgData = qUncompress(svgData);

    this->noteSvgRenderer->load(svgData);
    if(!this->noteSvgElementId.isEmpty())
    {
        QRectF r = this->noteSvgRenderer->boundsOnElement(this->noteSvgElementId);
        this->noteSvgRenderer->setViewBox(r);
    }
}
