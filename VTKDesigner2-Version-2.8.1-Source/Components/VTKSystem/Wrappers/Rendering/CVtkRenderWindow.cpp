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

#include "CVtkRenderWindow.h"
#include "vtkRendererCollection.h"
#include "IVtkRendererIOData.h"
#include "vtkRenderer.h"

#include "vtkQtGraphicsViewRenderWindow.h"

#include <QMouseEvent>
#include <QKeyEvent>
#include <QSize>
#include <QApplication>

#include "vtkCommand.h"
#include "CVtkVisSystemComponent.h"
#include "vtkInteractorStyleTrackballActor.h"

#include <GCF/AbstractComponent>
#include "IScriptEngineLoadFormContainerDatabase.h"

static IScriptEngineLoadFormContainerDatabase* LoadFormContainerDB()
{
    static IScriptEngineLoadFormContainerDatabase* ptr = 0;
    if(ptr == 0)
        ptr = GCF::findObject<IScriptEngineLoadFormContainerDatabase*>();

    return ptr;
}

class CRenderWindowMonitor : public vtkCommand
{
public:
    void Execute(vtkObject *caller, unsigned long eventId, void *callData);
};

static CRenderWindowMonitor& RenderWindowMonitor()
{
    static CRenderWindowMonitor rwm;
    return rwm;
}

QList<CVtkRenderWindow*>& TheRenderWindowList()
{
    static QList<CVtkRenderWindow*> theList;
    return theList;
}

QList<CVtkRenderWindow*> CVtkRenderWindow::RenderWindowList()
{
    return TheRenderWindowList();
}

DEFINE_VTK_OBJECT(CVtkRenderWindow, CVtkObject, vtkRenderWindow)
{
    pDesc->setNodeClassCategory("Rendering");
    pDesc->setNodeClassName("vtkRenderWindow");

    // Renderer Input
    pDesc->addConnectionPath(
        new CVtkObjectConnectionPath("Renderer", IVisSystemNodeConnectionPath::InputPath, "vtkRenderer", 0, true)
        );

    // RenderWindow Output
    pDesc->addConnectionPath(
        new CVtkObjectConnectionPath("RenderWindow", IVisSystemNodeConnectionPath::OutputPath, "vtkRenderWindow", 0, true)
        );

    // RenderWindowInteractor Output
    pDesc->addConnectionPath(
        new CVtkObjectConnectionPath("RenderWindowInteractor", IVisSystemNodeConnectionPath::OutputPath, "vtkRenderWindowInteractor", 0, true)
        );

}

CVtkRenderWindow::CVtkRenderWindow() : m_vtkRenderWindow(0), m_lastPokedRenderer(0), m_mousePressed(false)
{
    CVtkRenderWindow::InitializeObjectDesc();

    m_vtkWidget = new vtkQtGraphicsViewRenderWindow;
    m_vtkWidget->setAutoRendererTilingEnabled(false);

    setVtkObject( (vtkRenderWindow*)m_vtkWidget );
    m_vtkWidget->installEventFilter(this);
    m_vtkRenderWindow->AddObserver(vtkCommand::StartEvent, &RenderWindowMonitor());
    m_vtkRenderWindow->AddObserver(vtkCommand::EndEvent, &RenderWindowMonitor());

    CVtkVisSystemComponent::instance().vtkOutputDisplay()->addTab(m_vtkWidget, this->nodeName());
    // m_vtkWidget->GetRenderWindow()->GetInteractor()->SetInteractorStyle(vtkInteractorStyleTrackballActor::New());

    // Store the global list of render-windows.
    TheRenderWindowList().append(this);

    // Add widget to the ComponentGUI node.
    m_guiNode.setType(GCF::ComponentGuiNode::Widget);
    m_guiNode.setName(this->nodeName());
    m_guiNode["name"] = this->nodeName();
    m_guiNode["parentWidget"] = QString();
    m_guiNode["hint"] = QString();
    m_guiNode["title"] = this->nodeName();
    m_guiNode["hideWhenInactive"] = false;
    m_guiNode["visible"] = false;
    CVtkVisSystemComponent::instance().componentGui().addNode(m_guiNode);

    // Add widget to the load form-container database.
    LoadFormContainerDB()->addLoadFormContainer(m_vtkWidget, m_guiNode.completeName());
}

CVtkRenderWindow::~CVtkRenderWindow()
{
    // Add widget to the ComponentGUI node.
    CVtkVisSystemComponent::instance().componentGui().removeNode(m_guiNode);

    // Remove from global list of render-windows..
    TheRenderWindowList().removeAll(this);
}

QWidget* CVtkRenderWindow::vtkWidget() const
{
    return m_vtkWidget;
}

void CVtkRenderWindow::setNodeName(QString name)
{
    CVtkObject::setNodeName(name);
    int index = CVtkVisSystemComponent::instance().vtkOutputDisplay()->indexOf(m_vtkWidget);
    if(index < 0)
        return;
    CVtkVisSystemComponent::instance().vtkOutputDisplay()->setTabText(index, name);

    m_guiNode.setName(this->nodeName());
    m_guiNode["name"] = this->nodeName();
    m_guiNode["title"] = this->nodeName();

    LoadFormContainerDB()->changeLoadFormContainer(m_vtkWidget, m_guiNode.completeName());
}

void CVtkRenderWindow::render()
{
    command_Render();
}

void CVtkRenderWindow::command_Render()
{
    m_vtkRenderWindow->Render();
}

void CVtkRenderWindow::handleTimerControlSignal()
{
    m_vtkRenderWindow->Render();
}

bool CVtkRenderWindow::hasInput(IVisSystemNodeConnectionPath* path)
{
    if(!path)
        return false;

    if(path->pathName() == "Renderer")
    {
        if(m_vtkRenderWindow->GetRenderers())
            return m_vtkRenderWindow->GetRenderers()->GetNumberOfItems() != 0;
    }

    return false;
}

bool CVtkRenderWindow::setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    if(path->pathName() == "Renderer")
    {
        IVtkRendererIOData* rendererData = 0;
        bool success = inputData->queryInterface("IVtkRendererIOData", (void**)&rendererData);
        if(success && rendererData)
        {
            m_vtkRenderWindow->AddRenderer(rendererData->getVtkRenderer());
            rendererData->getVtkRenderer()->SetRenderWindow(m_vtkRenderWindow);
            tileRenderers();
            return true;
        }
    }

    return false;
}

bool CVtkRenderWindow::removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    if(path->pathName() == "Renderer")
    {
        IVtkRendererIOData* rendererData = 0;
        bool success = inputData->queryInterface("IVtkRendererIOData", (void**)&rendererData);
        if(success && rendererData && m_vtkRenderWindow->GetRenderers()->IsItemPresent(rendererData->getVtkRenderer()))
        {
            m_vtkRenderWindow->RemoveRenderer(rendererData->getVtkRenderer());
            rendererData->getVtkRenderer()->SetRenderWindow(0);
            if(m_lastPokedRenderer == rendererData->getVtkRenderer())
                m_lastPokedRenderer = 0;
            tileRenderers();
            return true;
        }
        return false;
    }

    return false;
}

bool CVtkRenderWindow::fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData)
{
    if(!path || !outputData)
        return false;

    if(path->pathName() == "RenderWindow")
    {
        m_vtkRenderWindowOutput.setRenderWindow(m_vtkRenderWindow);
        *outputData = &m_vtkRenderWindowOutput;
        return true;
    }

    if(path->pathName() == "RenderWindowInteractor")
    {
        if( !m_vtkRenderWindow->GetInteractor() )
            m_vtkRenderWindow->MakeRenderWindowInteractor();
        m_renWinIntData.setRenderWindowInteractor(m_vtkRenderWindow->GetInteractor());
        *outputData = &m_renWinIntData;
        return true;
    }

    return false;
}

bool CVtkRenderWindow::outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData)
{
    Q_UNUSED(path);
    Q_UNUSED(outputData);
    return true;
}

void CVtkRenderWindow::finalizeNode()
{
    LoadFormContainerDB()->removeLoadFormContainer(m_vtkWidget);
    this->stopHandlingVtkEvents();
    m_vtkWidget->hide();
    // setVtkObject(0);
    delete m_vtkWidget;
    delete this;
}

#define EventID(x) (unsigned long)(int(vtkCommand::UserEvent) + int(QEvent::x))

bool CVtkRenderWindow::eventFilter(QObject* object, QEvent* event)
{
    if(object != m_vtkWidget)
        return false;

    QSize size = m_vtkWidget->size();

    switch(event->type())
    {
    case QEvent::MouseButtonPress: {
        QMouseEvent* me = dynamic_cast<QMouseEvent*>(event);
        vtkRenderer* renderer = m_vtkRenderWindow->GetInteractor()->FindPokedRenderer(me->x(), me->y());
        emit OnMousePress(me->x(), size.height()-me->y());
        if(renderer)
            renderer->InvokeEvent(EventID(MouseButtonPress), (void*)me);
        m_mousePressed = true;
        }
        break;
    case QEvent::MouseMove: {
        if(!m_mousePressed)
            break;
        QMouseEvent* me = dynamic_cast<QMouseEvent*>(event);
        vtkRenderer* renderer = m_vtkRenderWindow->GetInteractor()->FindPokedRenderer(me->x(), me->y());
        emit OnMouseMove(me->x(), size.height()-me->y());
        if(renderer)
            renderer->InvokeEvent(EventID(MouseMove), (void*)me);
        }
        break;
    case QEvent::MouseButtonRelease:  {
        QMouseEvent* me = dynamic_cast<QMouseEvent*>(event);
        vtkRenderer* renderer = m_vtkRenderWindow->GetInteractor()->FindPokedRenderer(me->x(), me->y());
        emit OnMouseRelease(me->x(), size.height()-me->y());
        if(renderer)
            renderer->InvokeEvent(EventID(MouseButtonRelease), (void*)me);
        m_mousePressed = false;
        }
        break;
    case QEvent::MouseButtonDblClick:  {
        QMouseEvent* me = dynamic_cast<QMouseEvent*>(event);
        vtkRenderer* renderer = m_vtkRenderWindow->GetInteractor()->FindPokedRenderer(me->x(), me->y());
        emit OnMouseDoubleClick(me->x(), size.height()-me->y());
        if(renderer)
            renderer->InvokeEvent(EventID(MouseButtonDblClick), (void*)me);
        }
        break;
    case QEvent::KeyPress: {
        QKeyEvent* ke = dynamic_cast<QKeyEvent*>(event);
        vtkRenderer* renderer = m_lastPokedRenderer;
        emit OnKeyPress(ke->text());
        if(renderer)
            renderer->InvokeEvent(EventID(KeyPress), (void*)ke);
        }
        break;
    case QEvent::KeyRelease: {
        QKeyEvent* ke = dynamic_cast<QKeyEvent*>(event);
        vtkRenderer* renderer = m_lastPokedRenderer;
        emit OnKeyRelease(ke->text());
        if(renderer)
            renderer->InvokeEvent(EventID(KeyRelease), (void*)ke);
        }
        break;
    default:
        break;
    }

    return false;
}

void CVtkRenderWindow::tileRenderers()
{
    vtkRendererCollection* col = m_vtkRenderWindow->GetRenderers();
    int nrItems = col->GetNumberOfItems();
    int index = 0;
    bool odd = (nrItems % 2);
    double x=0.0f, y=0.0f;
    int nrRows = 0;
    double dy = 0.0f;
    int nrCols = 0;
    double dx = 0.0f;
    vtkRenderer* ren = 0;
    bool lastRow = false;

    col->InitTraversal();
    ren = col->GetNextItem();

    if( !nrItems )
        return;

    if( nrItems == 1 )
    {
        ren->SetViewport(0.0f, 0.0f, 1.0f, 1.0f);
        return;
    }

    if(odd)
        nrRows = (nrItems-1)/2;
    else
        nrRows = (nrItems)/2;
    nrCols = nrItems/nrRows;

    dx = 1.0f / (double)nrCols;
    dy = 1.0f / (double)nrRows;

    while(ren)
    {
        ren->SetViewport(x, y, x+dx, y+dy);
        ++index;
        x += dx;
        if( !lastRow && !(index%nrCols) )
        {
            y += dy;
            x = 0.0f;
            if( index + nrCols + 1 == nrItems )
            {
                ++nrCols;
                dx = 1.0f / (double)nrCols;
                lastRow = true;
            }
        }
        ren = col->GetNextItem();
    }
}

void CRenderWindowMonitor::Execute(vtkObject *caller, unsigned long eventId, void *callData)
{
    Q_UNUSED(caller);
    Q_UNUSED(callData);
    switch(eventId)
    {
    case vtkCommand::StartEvent:
        QApplication::setOverrideCursor(Qt::WaitCursor);
        break;
    case vtkCommand::EndEvent:
        QApplication::restoreOverrideCursor();
        break;
    }
}

