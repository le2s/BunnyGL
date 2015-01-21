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

#ifndef C_VTK_RENDER_WINDOW_H
#define C_VTK_RENDER_WINDOW_H

#include "CVtkObject.h"
#include "vtkRenderWindow.h"
#include "CVtkRenderWindowData.h"
#include "CVtkRenderWindowInteractorData.h"
#include "UserDefinedDataTypes.h"
#include <GCF/ComponentGui>

class QWidget;
class vtkQtGraphicsViewRenderWindow;
class vtkRenderer;

class CVtkRenderWindow : public CVtkObject
{
    Q_OBJECT
    DECLARE_VTK_OBJECT

public:
    static QList<CVtkRenderWindow*> RenderWindowList();

    CVtkRenderWindow();
    ~CVtkRenderWindow();

    QWidget* vtkWidget() const;
    void setNodeName(QString name);

    Q_INVOKABLE void render();

signals:
    void OnMousePress(int x, int y);
    void OnMouseMove(int x, int y);
    void OnMouseRelease(int x, int y);
    void OnMouseDoubleClick(int x, int y);
    void OnKeyPress(QString text);
    void OnKeyRelease(QString text);

protected slots:
    void command_Render();

protected:
    void handleTimerControlSignal();
    bool hasInput(IVisSystemNodeConnectionPath* path);
    bool setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData);
    bool removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData);
    bool fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData);
    bool outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData);
    void finalizeNode();

protected:
    bool eventFilter(QObject* object, QEvent* event);

public:
    Q_INVOKABLE void tileRenderers();

private:
    vtkQtGraphicsViewRenderWindow* m_vtkWidget;
    vtkRenderWindow* m_vtkRenderWindow;
    CVtkRenderWindowData m_vtkRenderWindowOutput;
    vtkRenderer* m_lastPokedRenderer;
    bool m_mousePressed;
    CVtkRenderWindowInteractorData m_renWinIntData;
    GCF::ComponentGuiNode m_guiNode;
};

#endif
