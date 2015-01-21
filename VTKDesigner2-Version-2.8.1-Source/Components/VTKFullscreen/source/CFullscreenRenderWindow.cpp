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

#include "CFullscreenRenderWindow.h"

#include <QPushButton>
#include <QMenu>
#include <QAction>
#include <QKeyEvent>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QList>
#include <QApplication>

#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "QVTKWidget.h"
#include "vtkRendererCollection.h"
#include "vtkActorCollection.h"
#include "vtkVolumeCollection.h"
#include "vtkCamera.h"
#include "vtkLightCollection.h"
#include "vtkActor.h"
#include "vtkVolume.h"
#include "vtkLight.h"

struct CFullscreenRenderWindowData
{
    CFullscreenRenderWindowData() : vtkWidget(0), toolbar(0),
        renderWindow(0) { }

	QVTKWidget* vtkWidget;
	CFullscreenRenderWindowToolbar* toolbar;
	vtkRenderWindow* renderWindow;
    QList<vtkRenderer*> rendererList;

	vtkRenderer* cloneRenderer(vtkRenderer* ren);
    void releaseRenderer(vtkRenderer* ren);
};

CFullscreenRenderWindow::CFullscreenRenderWindow()
:QWidget(0)
{
	d = new CFullscreenRenderWindowData;

	QVBoxLayout* layout = new QVBoxLayout(this);
	d->vtkWidget = new QVTKWidget(this);
	layout->addWidget( d->vtkWidget );
    layout->setMargin(0);

	d->toolbar = new CFullscreenRenderWindowToolbar;
	d->toolbar->hide();
	connect(d->toolbar, SIGNAL(closeButtonClicked()), this, SLOT(close()));
	connect(d->toolbar, SIGNAL(activeRenderWindowChanged()),
			this, SLOT(slotActiveRenderWindowChanged()));

	setWindowState(Qt::WindowFullScreen);
	setAttribute(Qt::WA_DeleteOnClose, true);
    setWindowFlags( Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint );

    qApp->installEventFilter( this );
}

CFullscreenRenderWindow::~CFullscreenRenderWindow()
{
	setRenderWindow(0);
    delete d->vtkWidget;
	delete d->toolbar;
    for(int i=0; i<d->rendererList.count(); i++)
    {
        vtkRenderer* ren = d->rendererList[i];
        while(ren->GetReferenceCount() > 1)
            ren->Delete();
        ren->Delete();
    }
	delete d;
}

QVTKWidget* CFullscreenRenderWindow::vtkWidget() const
{
	return d->vtkWidget;
}

CFullscreenRenderWindowToolbar* CFullscreenRenderWindow::toolbar() const
{
	return d->toolbar;
}

void CFullscreenRenderWindow::setRenderWindow(vtkRenderWindow* rWin)
{
	if( d->renderWindow == rWin )
		return;

	d->vtkWidget->setEnabled(false);

	vtkRenderWindow* renWin = d->vtkWidget->GetRenderWindow();

	// Create QList of renderers in the render window.
	vtkRendererCollection* renCol = renWin->GetRenderers();
	renCol->InitTraversal();
	QList<vtkRenderer*> renList;
	while(1)
	{
		vtkRenderer* ren = vtkRenderer::SafeDownCast( renCol->GetNextItemAsObject() );
		if( !ren )
			break;
		renList.append(ren);
	}

	// Remove all the renderers
	for(int i=0; i<renList.count(); i++)
    {
        vtkRenderer* ren = renList[i];
        d->releaseRenderer(ren);
        if( ren->GetReferenceCount() > 1 )
            d->rendererList.append(ren);
		renWin->RemoveRenderer( ren );
        ren->SetRenderWindow(0);
    }
	renList.clear();

	if( !rWin )
		return;

	// Create a QList of new renderers to add
	renCol = rWin->GetRenderers();
    renCol->InitTraversal();
	while(1)
	{
		vtkRenderer* ren = vtkRenderer::SafeDownCast( renCol->GetNextItemAsObject() );
		if( !ren )
			break;
		renList.append(ren);
	}

	// Now create a clone of each renderer and add it to our render window
	for(int i=0; i<renList.count(); i++)
	{
		vtkRenderer* newRen = d->cloneRenderer( renList[i] );
		renWin->AddRenderer( newRen );
	}
	renList.clear();

	// Now update the renderer
	renWin->Render();

	// Update internal variables
	d->renderWindow = rWin;
    d->toolbar->setActiveRenderWindow( rWin );

	d->vtkWidget->setEnabled(true);
}

vtkRenderWindow* CFullscreenRenderWindow::renderWindow() const
{
	return d->renderWindow;
}

bool CFullscreenRenderWindow::eventFilter(QObject* obj, QEvent* event)
{
	Q_UNUSED(obj);

	if( event->type() == QEvent::MouseMove )
	{
		if( QCursor::pos().y() < 100 )
		{
			if( !d->toolbar->isVisible() )
			{
				d->toolbar->setGeometry(0, 0, width(), 60);
				d->toolbar->show();
			}
		}
		else 
		{
			if( d->toolbar->isVisible() && !d->toolbar->isMenuVisible() )
				d->toolbar->hide();
		}
	}
	else if( event->type() == QEvent::KeyPress )
	{
		QKeyEvent* ke = (QKeyEvent*)event;
		if( ke->key() == Qt::Key_Escape )
			close();
	}

	return false;
}

void CFullscreenRenderWindow::slotActiveRenderWindowChanged()
{
	vtkRenderWindow* renWin = d->toolbar->activeRenderWindow();
	setRenderWindow( renWin );
}

vtkRenderer* CFullscreenRenderWindowData::cloneRenderer(vtkRenderer* ren)
{
	// Create a new renderer
	vtkRenderer* newRen = vtkRenderer::New();

	// Copy actors
	vtkActorCollection* actors = ren->GetActors();
	actors->InitTraversal();
	while(1)
	{
		vtkActor* actor = vtkActor::SafeDownCast( actors->GetNextItemAsObject() );
		if( !actor )
			break;
		newRen->AddActor( actor );
	}

	// Copy volumes
	vtkVolumeCollection* volumes = ren->GetVolumes();
	volumes->InitTraversal();
	while(1)
	{
		vtkVolume* volume = vtkVolume::SafeDownCast( volumes->GetNextItemAsObject() );
		if( !volume )
			break;
		newRen->AddVolume( volume );
	}

	// Copy lights
	vtkLightCollection* lights = ren->GetLights();
	lights->InitTraversal();
	while(1)
	{
		vtkLight* light = vtkLight::SafeDownCast( lights->GetNextItemAsObject() );
		if( !light )
			break;
		newRen->AddLight( light );
	}

	// Copy other variables
	newRen->SetTwoSidedLighting( ren->GetTwoSidedLighting() );
	newRen->SetAutomaticLightCreation( ren->GetAutomaticLightCreation() );
	newRen->SetLightFollowCamera( ren->GetLightFollowCamera() );
	newRen->SetInteractive( ren->GetInteractive() );
	newRen->SetBackground( ren->GetBackground() );
	newRen->SetViewport( ren->GetViewport() );
    newRen->SetActiveCamera( ren->GetActiveCamera() );

	// All done.
	return newRen;
}

void CFullscreenRenderWindowData::releaseRenderer(vtkRenderer* ren)
{
    // Remove actors
    QList<vtkActor*> actorList;
    vtkActorCollection* actors = ren->GetActors();
    actors->InitTraversal();
    while(1)
    {
        vtkActor* actor = vtkActor::SafeDownCast( actors->GetNextItemAsObject() );
        if( !actor )
            break;
        actorList.append( actor );
    }
    for(int i=0; i<actorList.count(); i++)
        ren->RemoveActor( actorList[i] );

    // Remove actors
    QList<vtkVolume*> volumeList;
    vtkVolumeCollection* volumes = ren->GetVolumes();
    volumes->InitTraversal();
    while(1)
    {
        vtkVolume* volume = vtkVolume::SafeDownCast( volumes->GetNextItemAsObject() );
        if( !volume )
            break;
        volumeList.append( volume );
    }
    for(int i=0; i<volumeList.count(); i++)
        ren->RemoveVolume( volumeList[i] );

    // Remove lights
    QList<vtkLight*> lightList;
    vtkLightCollection* lights = ren->GetLights();
    lights->InitTraversal();
    while(1)
    {
        vtkLight* light = vtkLight::SafeDownCast( lights->GetNextItemAsObject() );
        if( !light )
            break;
        lightList.append( light );
    }
    for(int i=0; i<lightList.count(); i++)
        ren->RemoveLight( lightList[i] );

    // Remove camera
    ren->SetActiveCamera(0);
}

/////////////////////////////////////////////////////////////////////////////
// CFullscreenRenderWindowToolbar
/////////////////////////////////////////////////////////////////////////////

#include "ui_CFullscreenRenderWindowToolbar.h"

struct CFullscreenRenderWindowToolbarData
{
	Ui::CFullscreenRenderWindowToolbar ui;
	QMenu* renderWindowMenu;

	QMap<vtkRenderWindow*, QAction*> rWinActionMap;
	vtkRenderWindow* activeRenderWindow;
};

CFullscreenRenderWindowToolbar::CFullscreenRenderWindowToolbar(QWidget* parent)
:QWidget(parent)
{
	d = new CFullscreenRenderWindowToolbarData;
	d->ui.setupUi(this);
	d->activeRenderWindow = 0;

	d->renderWindowMenu = new QMenu( d->ui.cmdChangeRenderWindow );
	d->ui.cmdChangeRenderWindow->setMenu( d->renderWindowMenu );

	connect(d->ui.cmdClose, SIGNAL(clicked()), this, SIGNAL(closeButtonClicked()));
	connect(d->renderWindowMenu, SIGNAL(triggered(QAction*)), this, SLOT(slotActionTriggered(QAction*)));

	setFixedHeight(90);
	setWindowFlags( Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint );
}

CFullscreenRenderWindowToolbar::~CFullscreenRenderWindowToolbar()
{
	delete d;
}

void CFullscreenRenderWindowToolbar::addRenderWindow(vtkRenderWindow* renWin, const QString& title)
{
	QAction* action = new QAction( this );
	action->setText( title );
	d->rWinActionMap[renWin] = action;
    d->renderWindowMenu->addAction( action );

	if( !d->activeRenderWindow )
		setActiveRenderWindow( renWin );
}

void CFullscreenRenderWindowToolbar::removeRenderWindow(vtkRenderWindow* renWin)
{
	if( d->rWinActionMap.contains(renWin) )
	{
		delete d->rWinActionMap[renWin];
		d->rWinActionMap.remove( renWin );
	}
}

int CFullscreenRenderWindowToolbar::renderWindowCount() const
{
	return d->renderWindowMenu->actions().count();
}

vtkRenderWindow* CFullscreenRenderWindowToolbar::renderWindow(int index) const
{
	if( index < 0 || index >= renderWindowCount() )
		return 0;

	QAction* action = d->renderWindowMenu->actions()[ index ];
	return d->rWinActionMap.key( action );
}

void CFullscreenRenderWindowToolbar::setActiveRenderWindow(vtkRenderWindow* rWin)
{
	if( d->activeRenderWindow == rWin )
		return;

	if( d->rWinActionMap.contains(rWin) )
	{
		d->activeRenderWindow = rWin;

        QAction* action = d->rWinActionMap[rWin];
        d->ui.label->setText( action->text() );

		emit activeRenderWindowChanged();
	}
}

vtkRenderWindow* CFullscreenRenderWindowToolbar::activeRenderWindow() const
{
	return d->activeRenderWindow;
}

bool CFullscreenRenderWindowToolbar::isMenuVisible() const
{
    return d->renderWindowMenu->isVisible();
}

void CFullscreenRenderWindowToolbar::slotActionTriggered(QAction* action)
{
	if( !action )
		return;

	if( !d->rWinActionMap.values().contains(action) )
		return;

	vtkRenderWindow* rWin = d->rWinActionMap.key(action);
	setActiveRenderWindow( rWin );
}




