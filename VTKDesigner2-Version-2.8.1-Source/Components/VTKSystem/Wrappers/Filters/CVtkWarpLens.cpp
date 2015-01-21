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
#include "CVtkWarpLens.h"

DEFINE_VTK_OBJECT(CVtkWarpLens,CVtkPointSetFilter,vtkWarpLens)
{
    pDesc->setNodeClassName("vtkWarpLens");
    pDesc->setNodeClassDescription("Deform geometry by applying lens distortion");
    pDesc->setNodeClassCategory("PolyData Filters");

    pDesc->setPropertyInfo("Kappa", "SetKappa", "GetKappa", QStringList() << "double", QStringList(), "void", "double");
    pDesc->setPropertyInfo("Center", "SetCenter", "GetCenter", QStringList() << "double" << "double", QStringList(), "void", "double*");
    pDesc->setPropertyInfo("PrincipalPoint", "SetPrincipalPoint", "GetPrincipalPoint", QStringList() << "double" << "double", QStringList(), "void", "double*");
    pDesc->setPropertyInfo("K1", "SetK1", "GetK1", QStringList() << "double", QStringList(), "void", "double");
    pDesc->setPropertyInfo("K2", "SetK2", "GetK2", QStringList() << "double", QStringList(), "void", "double");
    pDesc->setPropertyInfo("P1", "SetP1", "GetP1", QStringList() << "double", QStringList(), "void", "double");
    pDesc->setPropertyInfo("P2", "SetP2", "GetP2", QStringList() << "double", QStringList(), "void", "double");
    pDesc->setPropertyInfo("FormatWidth", "SetFormatWidth", "GetFormatWidth", QStringList() << "double", QStringList(), "void", "double");
    pDesc->setPropertyInfo("FormatHeight", "SetFormatHeight", "GetFormatHeight", QStringList() << "double", QStringList(), "void", "double");
    pDesc->setPropertyInfo("ImageWidth", "SetImageWidth", "GetImageWidth", QStringList() << "int", QStringList(), "void", "int");
    pDesc->setPropertyInfo("ImageHeight", "SetImageHeight", "GetImageHeight", QStringList() << "int", QStringList(), "void", "int");
}

CVtkWarpLens::CVtkWarpLens() : m_vtkWarpLens(0)
{
    CVtkWarpLens::InitializeObjectDesc();
    setVtkObject(vtkWarpLens::New());
}

CVtkWarpLens::~CVtkWarpLens()
{

}

double CVtkWarpLens::kappa() const
{
    return m_vtkWarpLens->GetKappa();
}


void CVtkWarpLens::setKappa(double val)
{
    m_vtkWarpLens->SetKappa(val);
}


Point3D CVtkWarpLens::center() const
{
    double *dpt = m_vtkWarpLens->GetCenter();
    return Point3D(dpt,true);
}


void CVtkWarpLens::setCenter(Point3D val)
{
    m_vtkWarpLens->SetCenter(val.x, val.y);
}


Point3D CVtkWarpLens::principalPoint() const
{
    double *dpt = m_vtkWarpLens->GetPrincipalPoint();
    return Point3D(dpt,true);
}


void CVtkWarpLens::setPrincipalPoint(Point3D val)
{
    m_vtkWarpLens->SetPrincipalPoint(val.x, val.y);
}


double CVtkWarpLens::k1() const
{
    return m_vtkWarpLens->GetK1();
}


void CVtkWarpLens::setK1(double val)
{
    m_vtkWarpLens->SetK1(val);
}


double CVtkWarpLens::k2() const
{
    return m_vtkWarpLens->GetK2();
}


void CVtkWarpLens::setK2(double val)
{
    m_vtkWarpLens->SetK2(val);
}


double CVtkWarpLens::p1() const
{
    return m_vtkWarpLens->GetP1();
}


void CVtkWarpLens::setP1(double val)
{
    m_vtkWarpLens->SetP1(val);
}


double CVtkWarpLens::p2() const
{
    return m_vtkWarpLens->GetP2();
}


void CVtkWarpLens::setP2(double val)
{
    m_vtkWarpLens->SetP2(val);
}


double CVtkWarpLens::formatWidth() const
{
    return m_vtkWarpLens->GetFormatWidth();
}


void CVtkWarpLens::setFormatWidth(double val)
{
    m_vtkWarpLens->SetFormatWidth(val);
}


double CVtkWarpLens::formatHeight() const
{
    return m_vtkWarpLens->GetFormatHeight();
}


void CVtkWarpLens::setFormatHeight(double val)
{
    m_vtkWarpLens->SetFormatHeight(val);
}


int CVtkWarpLens::imageWidth() const
{
    return m_vtkWarpLens->GetImageWidth();
}


void CVtkWarpLens::setImageWidth(int val)
{
    m_vtkWarpLens->SetImageWidth(val);
}


int CVtkWarpLens::imageHeight() const
{
    return m_vtkWarpLens->GetImageHeight();
}


void CVtkWarpLens::setImageHeight(int val)
{
    m_vtkWarpLens->SetImageHeight(val);
}

