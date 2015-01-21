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

#include "CVtkActor2D.h"
#include "IVtkMapper2DIOData.h"

DEFINE_VTK_OBJECT(CVtkActor2D, CVtkObject, vtkActor2D)
{
    pDesc->setNodeClassCategory("Rendering");
    pDesc->setNodeClassName("vtkActor2D");
    
    pDesc->setPropertyInfo("Visible", "SetVisibility", "GetVisibility", QStringList() << "bool", QStringList(), "void", "bool");
    pDesc->setPropertyInfo("Pickable", "SetPickable", "GetPickable", QStringList() << "bool", QStringList(), "void", "bool");
    pDesc->setPropertyInfo("Draggable", "SetDragable", "GetDragable", QStringList() << "bool", QStringList(), "void", "bool");
    pDesc->setPropertyInfo("Color", "GetProperty()->SetColor", "GetProperty()->GetColor", QStringList() << "color_double" << "color_double" << "color_double", QStringList(),"void", "double*");
    pDesc->setPropertyInfo("Opacity", "GetProperty()->SetOpacity", "GetProperty()->GetOpacity", QStringList() << "double", QStringList(), "void", "double");
    pDesc->setPropertyInfo("PointSize", "GetProperty()->SetPointSize", "GetProperty()->GetPointSize", QStringList() << "double", QStringList(), "void", "double");
    pDesc->setPropertyInfo("LineWidth", "GetProperty()->SetLineWidth", "GetProperty()->GetLineWidth", QStringList() << "double", QStringList(), "void", "double");
    pDesc->setPropertyInfo("Width", "SetWidth", "GetWidth", QStringList() << "double", QStringList(), "void", "double");
    pDesc->setPropertyInfo("Height", "SetHeight", "GetHeight", QStringList() << "double", QStringList(), "void", "double");
    pDesc->setPropertyInfo("DisplayLocation", "GetProperty()->SetDisplayLocation", "GetProperty()->GetDisplayLocation", QStringList() << "int", QStringList(), "void", "int");
    pDesc->setPropertyInfo("Position", "SetPosition", "GetPosition", QStringList() << "double" << "double" , QStringList(), "void", "double*");
    
    // Mapper2D input
    pDesc->addConnectionPath(
        new CVtkObjectConnectionPath("Mapper2D", IVisSystemNodeConnectionPath::InputPath, "vtkMapper2D", 0)
        );

    // Actor2D output
    pDesc->addConnectionPath(
        new CVtkObjectConnectionPath("Actor2D", IVisSystemNodeConnectionPath::OutputPath, "vtkActor2D", 0)
        );
}

CVtkActor2D::CVtkActor2D(bool newVtkActor2D) : m_vtkActor2D(0)
{
    CVtkActor2D::InitializeObjectDesc();
    if(newVtkActor2D)
        setVtkObject(vtkActor2D::New());
}

CVtkActor2D::~CVtkActor2D()
{

}

void CVtkActor2D::setVisible(bool val)
{
    m_vtkActor2D->SetVisibility(val);
}

bool CVtkActor2D::isVisible() const
{
    return m_vtkActor2D->GetVisibility();
}

void CVtkActor2D::setPickable(bool val)
{
    m_vtkActor2D->SetPickable(val);
}

bool CVtkActor2D::isPickable() const
{
    return m_vtkActor2D->GetPickable();
}

void CVtkActor2D::setDraggable(bool val)
{
    m_vtkActor2D->SetDragable(val);
}

bool CVtkActor2D::isDraggable() const
{
    return m_vtkActor2D->GetDragable();
}

void CVtkActor2D::setColor(QColor color)
{
    m_vtkActor2D->GetProperty()->SetColor( color.redF(), color.greenF(), color.blueF() );
}

QColor CVtkActor2D::color() const
{
    QColor ret;
    double* color = m_vtkActor2D->GetProperty()->GetColor();
    ret.setRgbF( color[0], color[1], color[2] );
    return ret;
}

void CVtkActor2D::setOpacity(double val)
{
    m_vtkActor2D->GetProperty()->SetOpacity( val );
}

double CVtkActor2D::opacity() const
{
    return m_vtkActor2D->GetProperty()->GetOpacity();
}

void CVtkActor2D::setPointSize(double ps)
{
    m_vtkActor2D->GetProperty()->SetPointSize(ps);
}

double CVtkActor2D::pointSize() const
{
    return m_vtkActor2D->GetProperty()->GetPointSize();
}

void CVtkActor2D::setLineWidth(double lw)
{
    m_vtkActor2D->GetProperty()->SetLineWidth(lw);
}

double CVtkActor2D::lineWidth() const
{
    return m_vtkActor2D->GetProperty()->GetLineWidth();
}

void CVtkActor2D::setDisplayLocation(CVtkActor2D::DisplayLocation loc)
{
    m_vtkActor2D->GetProperty()->SetDisplayLocation( int(loc) );
}

CVtkActor2D::DisplayLocation CVtkActor2D::displayLocation() const
{
    return CVtkActor2D::DisplayLocation( m_vtkActor2D->GetProperty()->GetDisplayLocation() );
}

void CVtkActor2D::setLayerNumber(int val)
{
    m_vtkActor2D->SetLayerNumber(val);
}

int CVtkActor2D::layerNumber() const
{
    return m_vtkActor2D->GetLayerNumber();
}

void CVtkActor2D::setPosition(QList<QVariant> pos)
{
    m_vtkActor2D->SetPosition( pos[0].toDouble(), pos[1].toDouble() );
}

QList<QVariant> CVtkActor2D::position() const
{
    double* pos = m_vtkActor2D->GetPosition();
    QList<QVariant> ret = QList<QVariant>() << pos[0] << pos[1];
    return ret;
}

void CVtkActor2D::setPosition(double x, double y)
{
    m_vtkActor2D->SetPosition( x, y );
}

void CVtkActor2D::setX(double val)
{
    m_vtkActor2D->SetPosition( val, m_vtkActor2D->GetPosition()[1] );
}

double CVtkActor2D::x() const
{
    return m_vtkActor2D->GetPosition()[0];
}

void CVtkActor2D::setY(double val)
{
    m_vtkActor2D->SetPosition( m_vtkActor2D->GetPosition()[0], val );
}

double CVtkActor2D::y() const
{
    return m_vtkActor2D->GetPosition()[1];
}

void CVtkActor2D::setWidth(double width)
{
    m_vtkActor2D->SetWidth(width);
}

double CVtkActor2D::width() const
{
    return m_vtkActor2D->GetWidth();
}

void CVtkActor2D::setHeight(double height)
{
    m_vtkActor2D->SetHeight(height);
}

double CVtkActor2D::height() const
{
    return m_vtkActor2D->GetHeight();
}

void CVtkActor2D::setRect(double x, double y, double width, double height)
{
    m_vtkActor2D->SetPosition(x, y);
    m_vtkActor2D->SetWidth(width);
    m_vtkActor2D->SetHeight(height);
}

bool CVtkActor2D::hasInput(IVisSystemNodeConnectionPath* path)
{
    if(!path)
        return false;

    if(path->pathName() == "Mapper2D")
        return m_vtkActor2D->GetMapper();

    return CVtkObject::hasInput(path);
}

bool CVtkActor2D::setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    if(path->pathName() == "Mapper2D")
    {
        IVtkMapper2DIOData* data = 0;
        bool success = inputData->queryInterface("IVtkMapper2DIOData", (void**)&data);
        if(success && data->getVtkMapper2D())
        {
            m_vtkActor2D->SetMapper( data->getVtkMapper2D() );
            return true;
        }
        return false;
    }

    return CVtkObject::setInput(path, inputData);
}

bool CVtkActor2D::removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    if(path->pathName() == "Mapper2D")
    {
        IVtkMapper2DIOData* data = 0;
        bool success = inputData->queryInterface("IVtkMapper2DIOData", (void**)&data);
        if(success && data->getVtkMapper2D() == m_vtkActor2D->GetMapper() )
        {
            m_vtkActor2D->SetMapper( 0 );
            return true;
        }
        return false;
    }

    return CVtkObject::removeInput(path, inputData);
}

bool CVtkActor2D::fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData)
{
    if(!path || !outputData)
        return false;

    if(path->pathName() == "Actor2D")
    {
        m_actor2DData.setActor2D( m_vtkActor2D );
        *outputData = &m_actor2DData;
        return true;
    }

    return CVtkObject::fetchOutput(path, outputData);
}

bool CVtkActor2D::outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData)
{
    if(!path || !outputData)
        return false;

    if(path->pathName() == "Actor2D")
    {
        return true;
    }

    return CVtkObject::outputDerefed(path, outputData);
}


