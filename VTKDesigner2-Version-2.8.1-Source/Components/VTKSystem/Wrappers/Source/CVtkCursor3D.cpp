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

#include "CVtkCursor3D.h"

DEFINE_VTK_OBJECT(CVtkCursor3D, CVtkPolyDataAlgorithm, vtkCursor3D)
{
    pDesc->setNodeClassCategory("PolyDataSource");
    pDesc->setNodeClassName("vtkCursor3D");
    pDesc->setNodeClassDescription("Generate a 3D cursor representation");
    pDesc->setPropertyInfo("FocalPoint", "SetFocalPoint", "GetFocalPoint", QStringList() << "double" << "double" << "double", 
                            QStringList(), "void", "double*");
    pDesc->setPropertyInfo("Outline", "SetOutline", "GetOutline", QStringList() << "int",  QStringList(), "void", "int");
    pDesc->setPropertyInfo("Axes", "SetAxes", "GetAxes", QStringList() << "int",  QStringList(), "void", "int");
    pDesc->setPropertyInfo("XShadows", "SetXShadows", "GetXShadows", QStringList() << "int",  QStringList(), "void", "int");
    pDesc->setPropertyInfo("YShadows", "SetYShadows", "GetYShadows", QStringList() << "int",  QStringList(), "void", "int");
    pDesc->setPropertyInfo("ZShadows", "SetZShadows", "GetZShadows", QStringList() << "int",  QStringList(), "void", "int");
    pDesc->setPropertyInfo("TranslationMode", "SetTranslationMode", "GetTranslationMode", QStringList() << "int", QStringList(), "void", "int");
    pDesc->setPropertyInfo("Wrap", "SetWrap", "GetWrap", QStringList() << "int", QStringList(), "void", "int");    
}


CVtkCursor3D::CVtkCursor3D() : m_vtkCursor3D(0)
{
    CVtkCursor3D::InitializeObjectDesc();
    setVtkObject(vtkCursor3D::New());
}

CVtkCursor3D::~CVtkCursor3D()
{

}

Point3D CVtkCursor3D::focalPoint() const
{
    return Point3D(m_vtkCursor3D->GetFocalPoint());
}

void CVtkCursor3D::setFocalPoint(Point3D fp)
{
    m_vtkCursor3D->SetFocalPoint(fp.x, fp.y, fp.z);
}

bool CVtkCursor3D::isOutline() const
{
    return m_vtkCursor3D->GetOutline();
}

void CVtkCursor3D::setOutline(bool val)
{
    m_vtkCursor3D->SetOutline(val);
}

bool CVtkCursor3D::isAxes() const
{
    return m_vtkCursor3D->GetAxes();
}

void CVtkCursor3D::setAxes(bool val)
{
    m_vtkCursor3D->SetAxes(val);
}

bool CVtkCursor3D::isXShadows() const
{
    return m_vtkCursor3D->GetXShadows();
}

void CVtkCursor3D::setXShadows(bool val)
{
    m_vtkCursor3D->SetXShadows(val);
}

bool CVtkCursor3D::isYShadows() const
{
    return m_vtkCursor3D->GetYShadows();
}

void CVtkCursor3D::setYShadows(bool val)
{
    m_vtkCursor3D->SetYShadows(val);
}

bool CVtkCursor3D::isZShadows() const
{
    return m_vtkCursor3D->GetZShadows();
}

void CVtkCursor3D::setZShadows(bool val)
{
    m_vtkCursor3D->SetZShadows(val);
}

bool CVtkCursor3D::isTranslationMode() const
{
    return m_vtkCursor3D->GetTranslationMode();
}

void CVtkCursor3D::setTranslationMode(bool val)
{
    m_vtkCursor3D->SetTranslationMode(val);
}

bool CVtkCursor3D::isWrap() const
{
    return m_vtkCursor3D->GetWrap();
}

void CVtkCursor3D::setWrap(bool val)
{
    m_vtkCursor3D->SetWrap(val);
}

