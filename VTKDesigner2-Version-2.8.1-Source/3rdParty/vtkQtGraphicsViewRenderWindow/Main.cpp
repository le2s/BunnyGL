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

#include <QtGui>

#include "vtkFractalTerrainSource.h"
#include "vtkTransformPolyDataFilter.h"
#include "vtkTransform.h"
#include "vtkElevationFilter.h"
#include "vtkPolyDataNormals.h"
#include "vtkDataSetMapper.h"
#include "vtkActor.h"
#include "vtkProperty.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkLightKit.h"
#include "vtkCamera.h"
#include "vtkMatrix4x4.h"
#include "QVTKWidget.h"

#include "vtkQtGraphicsViewRenderWindow.h"

vtkRenderer* CreateVTKPipeline()
{
    vtkFractalTerrainSource* terrainSource = vtkFractalTerrainSource::New();
    vtkTransformPolyDataFilter* txFilter = vtkTransformPolyDataFilter::New();
    vtkTransform* tx = vtkTransform::New();
    vtkElevationFilter* elevFilter = vtkElevationFilter::New();
    vtkPolyDataNormals* pdNormals = vtkPolyDataNormals::New();
    vtkDataSetMapper* mapper = vtkDataSetMapper::New();
    vtkActor* actor = vtkActor::New();
    vtkRenderer* renderer = vtkRenderer::New();
    vtkLightKit* lightKit = vtkLightKit::New();

    terrainSource->SetNumberOfIterations(6);

    tx->Identity();
    tx->Scale(1, 1, 6);

    txFilter->SetTransform(tx);

    pdNormals->SetFeatureAngle(120);

    elevFilter->SetLowPoint(0, 0, -0.6);
    elevFilter->SetHighPoint(0, 0, 0.6);

    renderer->SetBackground(0.2, 0.2, 0.2);
    renderer->SetBackground2(0.8, 0.8, 0.8);
    renderer->GradientBackgroundOn();

    lightKit->AddLightsToRenderer(renderer);

    txFilter->SetInput( terrainSource->GetOutput() );
    pdNormals->SetInput( txFilter->GetOutput() );
    elevFilter->SetInput( pdNormals->GetOutput() );
    mapper->SetInput( elevFilter->GetOutput() );
    actor->SetMapper( mapper );
    renderer->AddActor( actor );

    vtkCamera* camera = renderer->GetActiveCamera();
    camera->SetPosition(-1.45377, 1.11981, -1.31595);
    camera->SetFocalPoint(-0.102167, 0.0264094, -0.0974712);
    camera->SetViewUp(0.499143, -0.29211, -0.815799);

    return renderer;
}

int main(int argc, char** argv)
{
    QApplication a(argc, argv);

    vtkRenderer* renderer = CreateVTKPipeline();

    vtkQtGraphicsViewRenderWindow gView;
	gView.AddRenderer( renderer );
    gView.resize(800, 600);
    gView.show();

    QCalendarWidget* calendar = new QCalendarWidget;
    calendar->setWindowOpacity(0.7);

    vtkQtGraphicsProxyWidget* widgetItem = new vtkQtGraphicsProxyWidget(&gView, Qt::Tool);
    widgetItem->setWidget(calendar);
    widgetItem->setPos( -widgetItem->boundingRect().topLeft() );
    widgetItem->setFlag(QGraphicsItem::ItemIsMovable);
    widgetItem->setCacheMode(QGraphicsItem::DeviceCoordinateCache);
    widgetItem->setWindowTitle("Calendar Widget");

    QGraphicsScene* scene = gView.scene();
    scene->addItem(widgetItem);

    return a.exec();
}

