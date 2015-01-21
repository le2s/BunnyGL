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

#ifndef I_VIS_NETWORK_CANVAS_H
#define I_VIS_NETWORK_CANVAS_H

#include <GCF/IContainer>
#include <QMetaType>

class IVisNetwork;
class IVisSystemNode;
class IVisNetworkConnection;
class QMenu;
class QGraphicsView;
class QGraphicsScene;

class IVisNetworkCanvas : virtual public IContainer
{
public:
    // Fetch basic objects.
    virtual QGraphicsView*      graphicsView() = 0;
    virtual QGraphicsScene*     graphicsScene() = 0;

    // Associate with the visualization system.
    virtual void                setVisNetwork(IVisNetwork* network) = 0;
    virtual IVisNetwork*        visNetwork() = 0;

    // Methods to select/unselect items in the canvas
    virtual bool                supportsMultipleSelection() = 0;
    virtual void                setMultipleSelection(bool val) = 0;
    virtual bool                isMultipleSelection() = 0;
    virtual void                select(IVisSystemNode* node, bool select=true) = 0;
    virtual int                 selectedNodeCount() = 0;
    virtual IVisSystemNode*     selectedNode(int index) = 0;
    virtual void                centerOn(IVisSystemNode* node) = 0;

    // Canvases have to provide the following editing capabilities
    virtual void                clear() = 0;
    virtual void                cut() = 0;
    virtual void                copy() = 0;
    virtual void                paste() = 0;
    virtual void                undo() = 0;
    virtual void                redo() = 0;
    virtual bool                isUndoAvailable() = 0;
    virtual bool                isRedoAvailable() = 0;
    virtual void                zoomIn() = 0;
    virtual void                zoomOut() = 0;
    virtual void                zoomFit() = 0;
    virtual void                zoomOne() = 0;
    virtual int                 zoomLevel() = 0;
    virtual void                setZoomLevel(int zl) = 0;
    virtual void                queryZoomLevelRange(int& min, int& max) = 0;
    virtual void                bringToCenter() = 0;
    virtual void                layoutNodes() = 0;

    // Read Only mode flags.
    virtual void                setReadOnly(bool val) = 0;
    virtual bool                isReadOnly() = 0;

    // The following must be implemented as signals only in the implementation of this interface.
    // Node mouse events.
    virtual void                zoomLevelChanged(int zl) = 0;
    virtual void                zoomScaleChanged(double zs) = 0;

    virtual void                nodeClickedEvent(IVisSystemNode* node, const QPoint pos, Qt::MouseButton button, Qt::KeyboardModifiers modifiers) = 0;
    virtual void                nodeDoubleClickedEvent(IVisSystemNode* node, const QPoint pos, Qt::MouseButton button, Qt::KeyboardModifiers modifiers) = 0;
    virtual void                nodeContextMenuEvent(IVisSystemNode* node, QMenu* menu, Qt::KeyboardModifiers modifiers) = 0;

    // Connection mouse events.
    virtual void                connectionClickedEvent(IVisNetworkConnection* con, const QPoint pos, Qt::MouseButton button, Qt::KeyboardModifiers modifiers) = 0;
    virtual void                connectionDoubleClickedEvent(IVisNetworkConnection* con, const QPoint pos, Qt::MouseButton button, Qt::KeyboardModifiers modifiers) = 0;
    virtual void                connectionContextMenuEvent(IVisNetworkConnection* node, QMenu* menu, Qt::KeyboardModifiers modifiers) = 0;

    // Canvas mouse events
    virtual void                canvasClickedEvent(IVisNetworkCanvas* canvas, const QPoint pos, Qt::MouseButton button, Qt::KeyboardModifiers modifiers) = 0;
    virtual void                canvasDoubleClickedEvent(IVisNetworkCanvas* canvas, const QPoint pos, Qt::MouseButton button, Qt::KeyboardModifiers modifiers) = 0;
    virtual void                canvasContextMenuEvent(IVisNetworkCanvas* canvas, QMenu* menu, Qt::KeyboardModifiers modifiers) = 0;

    // Keyboard events
    virtual void                nodeKeyEvent(IVisSystemNode* node, int key, QString text, Qt::KeyboardModifiers modifiers) = 0;
    virtual void                connectionKeyEvent(IVisNetworkConnection* con, int key, QString text, Qt::KeyboardModifiers modifiers) = 0;
    virtual void                canvasKeyEvent(IVisNetworkCanvas* canvas, int key, QString text, Qt::KeyboardModifiers modifiers) = 0;

    // Other events
    virtual void                panEvent(double dx, double dy) = 0;
    virtual void                zoomEvent(double dScale) = 0;

    // Selection events
    virtual void                nodeSelected(IVisSystemNode* node) = 0;
    virtual void                nodeUnselected(IVisSystemNode* node) = 0;
    virtual void                connectionSelected(IVisNetworkConnection* con) = 0;
    virtual void                connectionUnselected(IVisNetworkConnection* con) = 0;
};

Q_DECLARE_METATYPE(IVisSystemNode*)
Q_DECLARE_METATYPE(IVisNetworkConnection*)
Q_DECLARE_METATYPE(IVisNetworkCanvas*)

Q_DECLARE_INTERFACE(IVisNetworkCanvas, "com.vcreatelogic.IVisNetworkCanvas/1.0")

#endif
