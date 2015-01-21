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

#include "CVtk3DWidget.h"
#include "IVtkProp3DIOData.h"
#include "IVtkDataSetIOData.h"

DEFINE_VTK_OBJECT(CVtk3DWidget, CVtkInteractorObserver, vtk3DWidget)
{
    pDesc->setNodeClassName("vtk3DWidget");
    pDesc->setNodeClassCategory("Interactor Observers");
    pDesc->setNodeClassDescription("Base class for all 3D Widgets");

    pDesc->setCreateFunction(0);
    
    pDesc->setPropertyInfo("PlaceFactor", "SetPlaceFactor", "GetPlaceFactor", QStringList() << "double", QStringList(), "void", "double");
    pDesc->setPropertyInfo("HandleSize", "SetHandleSize", "GetHandleSize", QStringList() << "double", QStringList(), "void", "double");

    // Prop3D input
    pDesc->addConnectionPath(
        new CVtkObjectConnectionPath("Prop3D", IVisSystemNodeConnectionPath::InputPath, "vtkProp3D", 0)
        );

    // DataSet input
    pDesc->addConnectionPath(
        new CVtkObjectConnectionPath("DataSet", IVisSystemNodeConnectionPath::InputPath, "vtkDataSet", 0)
        );
};

CVtk3DWidget::CVtk3DWidget()
{
    m_vtk3DWidget = 0;
}

CVtk3DWidget::~CVtk3DWidget()
{

}

void CVtk3DWidget::placeWidget(double xmin, double xmax, double ymin, double ymax, double zmin, double zmax)
{
    m_vtk3DWidget->PlaceWidget(xmin, xmax, ymin, ymax, zmin, zmax);
}

void CVtk3DWidget::setPlaceFactor(double val)
{
    m_vtk3DWidget->SetPlaceFactor(val);
}

double CVtk3DWidget::placeFactor() const
{
    return m_vtk3DWidget->GetPlaceFactor();
}

void CVtk3DWidget::setHandleSize(double val)
{
    m_vtk3DWidget->SetHandleSize(val);
}

double CVtk3DWidget::handleSize() const
{
    return m_vtk3DWidget->GetHandleSize();
}

bool CVtk3DWidget::hasInput(IVisSystemNodeConnectionPath* path)
{
    if( !path )
        return false;

    if( path->pathName() == "Prop3D" || path->pathName() == "DataSet" )
        return m_vtk3DWidget->GetProp3D() || m_vtk3DWidget->GetInput();

    return CVtkInteractorObserver::hasInput(path);
}

bool CVtk3DWidget::setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if( !path || !inputData )
        return false;

    if( path->pathName() == "Prop3D" )
    {
        IVtkProp3DIOData* data = 0;
        bool success = inputData->queryInterface("IVtkProp3DIOData", (void**)&data);
        if( success && data )
        {
            vtkProp3D* prop3d = data->getVtkProp3D();
            m_vtk3DWidget->SetProp3D(prop3d);
            return true;
        }

        return false;
    }

    if( path->pathName() == "DataSet" )
    {
        IVtkDataSetIOData* data = 0;
        bool success = inputData->queryInterface("IVtkDataSetIOData", (void**)&data);
        if( success && data )
        {
            vtkDataSet* dataSet = data->getVtkDataSet();
            m_vtk3DWidget->SetInput(dataSet);
            return true;
        }

        return false;
    }

    return CVtkInteractorObserver::setInput(path, inputData);
}

bool CVtk3DWidget::removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if( !path || !inputData )
        return false;

    if( path->pathName() == "Prop3D" )
    {
        IVtkProp3DIOData* data = 0;
        bool success = inputData->queryInterface("IVtkProp3DIOData", (void**)&data);
        if( success && data && data->getVtkProp3D() == m_vtk3DWidget->GetProp3D())
        {
            m_vtk3DWidget->SetProp3D(0);
            return true;
        }

        return false;
    }

    if( path->pathName() == "DataSet" )
    {
        IVtkDataSetIOData* data = 0;
        bool success = inputData->queryInterface("IVtkDataSetIOData", (void**)&data);
        if( success && data && data->getVtkDataSet() == m_vtk3DWidget->GetInput())
        {
            m_vtk3DWidget->SetInput(0);
            return true;
        }

        return false;
    }

    return CVtkInteractorObserver::removeInput(path, inputData);
}


