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

#ifndef C_VIS_SYSTEM_CANVAS_H
#define C_VIS_SYSTEM_CANVAS_H

#include <QGraphicsView>
#include "IVisNetworkCanvas.h"
#include "IVisNetwork.h"
#include "IProjectPart.h"
#include "IVisNetworkCanvasWidgetExt.h"
#include "IVisNetworkCanvasNotesExt.h"
#include <QVariant>

class IPropertyEditor;
class IVisSystemNodeFactoryRegistry;
class CVisSystemCanvasNodeItem;
class CVisSystemCanvasConnectionItem;
class IXmlUtils;
class IQObjectLoadSave;
class CUndoRedoStack;

struct CVisSystemCanvasData;
class CVisSystemCanvas : public QGraphicsView,
                         virtual public IVisNetworkCanvas,
                         virtual public IVisNetwork,
                         virtual public IProjectPart,
                         virtual public IVisNetworkCanvasWidgetExt,
                         virtual public IVisNetworkCanvasNotesExt
{
    Q_OBJECT
    Q_INTERFACES(IVisNetworkCanvas IVisNetwork IProjectPart IVisNetworkCanvasWidgetExt IVisNetworkCanvasNotesExt)

public:
    CVisSystemCanvas(QWidget* parent=0);
    ~CVisSystemCanvas();

    // Custom methods
    void setNodeFactoryRegistry(IVisSystemNodeFactoryRegistry* reg);
    IVisSystemNodeFactoryRegistry* nodeFactoryRegistry() const;

    void setPropertyEditor(IPropertyEditor* propEditor);
    IPropertyEditor* propertyEditor() const;

    void setXmlUtils(IXmlUtils* xmlUtils);
    IXmlUtils* xmlUtils() const;

    void setQObjectLoadSave(IQObjectLoadSave* ls);
    IQObjectLoadSave* qobjectLoadSave() const;

    CVisSystemCanvasNodeItem* nodeItem(IVisSystemNode* node) const;
    CVisSystemCanvasConnectionItem* connectionItem(IVisNetworkConnection* con) const;
    QList<CVisSystemCanvasNodeItem*> nodeItemList() const;
    QList<CVisSystemCanvasConnectionItem*> connectionItemList() const;

    void selectNext(CVisSystemCanvasNodeItem* item);
    void selectPrevious(CVisSystemCanvasNodeItem* item);

    bool hasConnections(IVisSystemNode* node) const;

    CUndoRedoStack* undoRedoStack() const;

public slots:
    void newCanvas();
    void saveCanvas();
    void saveAsCanvas();
    void loadCanvas();
    void loadCanvasFrom(QString fileName);
    void saveCanvasInto(QString fileName);

public:
    // IContainer implementation
    QObject* containerObject();

    // IVisNetworkCanvas implementation
    QGraphicsView* graphicsView();
    QGraphicsScene* graphicsScene();
    void setVisNetwork(IVisNetwork* network);
    IVisNetwork* visNetwork();
    bool supportsMultipleSelection();
    void setMultipleSelection(bool val);
    bool isMultipleSelection();
    void select(IVisSystemNode* node, bool select=true);
    int selectedNodeCount();
    IVisSystemNode* selectedNode(int index);
    void centerOn(IVisSystemNode* node);
    bool isUndoAvailable();
    bool isRedoAvailable();
    int zoomLevel();
    bool isReadOnly();
    void queryZoomLevelRange(int& min, int& max);

public slots:
    void clear();
    void cut();
    void copy();
    void paste();
    void undo();
    void redo();
    void zoomIn();
    void zoomOut();
    void zoomFit();
    void zoomOne();
    void setZoomLevel(int zl);
    void bringToCenter();
    void layoutNodes();
    void setReadOnly(bool val);

signals:
    void zoomLevelChanged(int zl);
    void zoomScaleChanged(double zs);
    void nodeClickedEvent(IVisSystemNode* node, const QPoint pos, Qt::MouseButton button, Qt::KeyboardModifiers modifiers);
    void nodeDoubleClickedEvent(IVisSystemNode* node, const QPoint pos, Qt::MouseButton button, Qt::KeyboardModifiers modifiers);
    void nodeContextMenuEvent(IVisSystemNode* node, QMenu* menu, Qt::KeyboardModifiers modifiers);
    void connectionClickedEvent(IVisNetworkConnection* con, const QPoint pos, Qt::MouseButton button, Qt::KeyboardModifiers modifiers);
    void connectionDoubleClickedEvent(IVisNetworkConnection* con, const QPoint pos, Qt::MouseButton button, Qt::KeyboardModifiers modifiers);
    void connectionContextMenuEvent(IVisNetworkConnection* node, QMenu* menu, Qt::KeyboardModifiers modifiers);
    void canvasClickedEvent(IVisNetworkCanvas* canvas, const QPoint pos, Qt::MouseButton button, Qt::KeyboardModifiers modifiers);
    void canvasDoubleClickedEvent(IVisNetworkCanvas* canvas, const QPoint pos, Qt::MouseButton button, Qt::KeyboardModifiers modifiers);
    void canvasContextMenuEvent(IVisNetworkCanvas* canvas, QMenu* menu, Qt::KeyboardModifiers modifiers);
    void nodeKeyEvent(IVisSystemNode* node, int key, QString text, Qt::KeyboardModifiers modifiers);
    void connectionKeyEvent(IVisNetworkConnection* con, int key, QString text, Qt::KeyboardModifiers modifiers);
    void canvasKeyEvent(IVisNetworkCanvas* canvas, int key, QString text, Qt::KeyboardModifiers modifiers);
    void panEvent(double dx, double dy);
    void zoomEvent(double dScale);
    void nodeSelected(IVisSystemNode* node);
    void nodeUnselected(IVisSystemNode* node);
    void connectionSelected(IVisNetworkConnection* con);
    void connectionUnselected(IVisNetworkConnection* con);

public:
    // IVisNetwork implementation
    void addNode(IVisSystemNode* node);
    void removeNode(IVisSystemNode* node);
    void removeAllNodes();
    int nodeCount();
    IVisSystemNode* node(int index);
    bool hasNode(IVisSystemNode* node);
    bool connectNode(IVisSystemNode* sender, IVisSystemNode* receiver);
    bool connectNode(IVisSystemNode* sender, int outLine, IVisSystemNode* receiver, int inLine, IVisNetworkConnection** connection);
    bool disconnectNode(IVisSystemNode* sender, int outLine, IVisSystemNode* receiver, int inLine);
    bool disconnectNode(IVisNetworkConnection* connection);
    bool disconnectNode(IVisSystemNode* sender, IVisSystemNode* receiver);
    bool disconnectNode(IVisSystemNode* node);
    int connectionCount();
    IVisNetworkConnection* connection(int index);
    bool hasConnection(IVisNetworkConnection* con);

signals:
    void nodeAdded(IVisSystemNode* node);
    void nodeRemoved(IVisSystemNode* node);
    void connectionCreated(IVisNetworkConnection* connection);
    void connectionDestroyed(IVisNetworkConnection* connection);

protected:
    // Custom Methods
    void drawBackground(QPainter * paint, const QRectF & rect);
    void paintEvent(QPaintEvent* pe);
    void mousePressEvent(QMouseEvent* me);
    void mouseMoveEvent(QMouseEvent* me);
    void mouseReleaseEvent(QMouseEvent* me);
    void mouseDoubleClickEvent(QMouseEvent* me);
    void contextMenuEvent(QContextMenuEvent* me);
    void wheelEvent(QWheelEvent* e);
    void keyPressEvent(QKeyEvent* ke);
    void showEvent(QShowEvent* se);
    void hideEvent(QHideEvent* he);
    void dragEnterEvent(QDragEnterEvent* de);
    void dragMoveEvent(QDragMoveEvent* de);
    void dropEvent(QDropEvent* de);
    void itemSelected(QGraphicsItem* item);
    void bringToFront(QGraphicsItem* item);
    void sendToBack(QGraphicsItem* item);
    bool eventFilter(QObject* obj, QEvent* event);

protected slots:
    void on_scene_changed();
    void on_nodeItem_itemSelected(bool val);
    void on_connectionItem_itemSelected(bool val);
    void on_connectionCreated(IVisNetworkConnection* connection);
    void on_connectionDestroyed(IVisNetworkConnection* connection);
    void on_propertyEditor_propertyValueChanged(QObject* obj, QString propName, QVariant oldVal, QVariant newVal);

public: // invokable methods in this object
    Q_INVOKABLE QObject* addNode(QString nodeClassName);
    Q_INVOKABLE bool deleteNode(QObject* nodeObject);
    Q_INVOKABLE bool deleteAllNodes();
    Q_INVOKABLE int  getNodeCount();
    Q_INVOKABLE QObject* getNode(int index);
    Q_INVOKABLE QList<QObject*> getNodesOfClass(QString nodeClassName);
    Q_INVOKABLE QList<QObject*> getNodesOfName(QString nodeName, bool startsWith=true, bool contains=false);
    Q_INVOKABLE bool connectNode(QObject* senderNodeObj, QObject* receiverNodeObj);
    Q_INVOKABLE bool connectNode(QObject* senderNodeObj, int outLine, QObject* receiverNodeObj, int inLine);
    Q_INVOKABLE bool disconnectNode(QObject* senderNodeObj, int outLine, QObject* receiverNodeObj, int inLine);
    Q_INVOKABLE bool disconnectNode(QObject* senderNodeObj, QObject* receiverNodeObj);
    Q_INVOKABLE bool disconnectNode(QObject* nodeObject);

    // For calling from remote systems
    Q_INVOKABLE bool addNode(QString nodeClassName, QString nodeName);
    Q_INVOKABLE bool removeNode(QString nodeName);
    Q_INVOKABLE bool connectNode(QString senderName, QString receiverName);
    Q_INVOKABLE bool connectNode(QString senderName, int outLine, QString receiverName, int inLine);
    Q_INVOKABLE bool disconnectNode(QString senderName, int outLine, QString receiverName, int inLine);
    Q_INVOKABLE bool disconnectNode(QString senderName, QString receiverName);
    Q_INVOKABLE bool disconnectNode(QString nodeName);
    Q_INVOKABLE QString getNodeName(int index);
    Q_INVOKABLE QStringList getNodesNames();
    Q_INVOKABLE QStringList getNodeNamesOfClass(QString nodeClassName);
    Q_INVOKABLE QStringList getNodeNamesOfName(QString nodeName, bool startsWith, bool contains);


public: // IProjectPart implementation
    QString projectPartID();
    bool newProjectPart();
    bool loadProjectPart(QDomDocument doc, QDomElement partE);
    bool initializeProjectPart();
    bool saveProjectPart(QDomDocument doc, QDomElement partParentE);
    bool closeProjectPart();

signals:
    void projectPartModified();

public: // IVisNetworkCanvasWidgetExt implementation
    void addWidget(QWidget* widget, const QRectF& rect=QRectF(-40,-40,80,80), const QPointF& pos=QPointF(0,0));
    void removeWidget(QWidget* widget);
    int widgetCount() const;
    QWidget* widget(int index) const;

signals:
    void widgetAdded(QWidget* widget);
    void widgetRemoved(QWidget* widget);

public:
    // IVisNetworkCanvasNotesExt stuff
    Q_INVOKABLE void setNoteSvgFile(const QString& noteSvg, const QString& elementId=QString(), bool qcompressed=true);
    QString noteSvgFile() const;
    QString noteSvgElement() const;
    void addNote(IVisNetworkCanvasNote* note);
    void removeNote(IVisNetworkCanvasNote* note);
    int noteCount() const;
    IVisNetworkCanvasNote* note(int index) const;

public slots:
    void showNotes(); // from IVisNetworkCanvasNotesExt 
    void hideNotes(); // from IVisNetworkCanvasNotesExt
    void setNotesVisible(bool val); // for UI actions only

signals: // IVisNetworkCanvasNotesExt signals
    void noteAdded(IVisNetworkCanvasNote* note);
    void noteRemoved(IVisNetworkCanvasNote* note);

public slots: // for notes extension
    void addNote(); // for UI/actions
    void addNote(const QString& text, double x, double y, double w, double h); // for scripting only

    // For capturing snapshots
    void captureSnapshotToClipboard();

protected:
    // For showing property editor based on double click
    void connectNotify(const char* signal);
    void disconnectNotify(const char* signal);
    bool isDoubleClickSignalConnected() const;
    IPropertyEditor* getNewPropertyEditorInstance() const;

private:
    friend class CVisSystemCanvasNote;
	friend class CVisSystemCanvasWidget;
    friend class CVisSystemCanvasNodeItem;
    friend class CVisSystemCanvasConnectionItem;
    CVisSystemCanvasData* d;
};

#endif

