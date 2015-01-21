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

#include "CVtkHyperStreamline.h"


DEFINE_VTK_OBJECT(CVtkHyperStreamline, CVtkPolyDataFilter, vtkHyperStreamline)
{
    pDesc->setNodeClassName("vtkHyperStreamline");
    pDesc->setNodeClassDescription("Generate hyperstreamline in arbitrary dataset");

    pDesc->setPropertyInfo("StartPosition","SetStartPosition","GetStartPosition",QStringList() <<"double"<<"double"<<"double",QStringList(),"void","double*");
    pDesc->setPropertyInfo("IntegrationEigenvector","SetIntegrationEigenvector","GetIntegrationEigenvector",QStringList()<<"int", QStringList(),"void", "int");
    pDesc->setPropertyInfo("IntegrationStepLength","SetIntegrationStepLength","GetIntegrationStepLength",QStringList()<<"double", QStringList(),"void","double");
    pDesc->setPropertyInfo("StepLength","SetStepLength","GetStepLength",QStringList()<<"double", QStringList(),"void","double");
    pDesc->setPropertyInfo("IntegrationDirection","SetIntegrationDirection","GetIntegrationDirection",QStringList()<<"int", QStringList(),"void","int");
    pDesc->setPropertyInfo("TerminalEigenvalue","SetTerminalEigenvalue","GetTerminalEigenvalue",QStringList()<<"double", QStringList(),"void","double");
    pDesc->setPropertyInfo("NumberOfSides","SetNumberOfSides","GetNumberOfSides",QStringList()<<"int", QStringList(),"void", "int");    
    pDesc->setPropertyInfo("Radius","SetRadius","GetRadius",QStringList()<<"double", QStringList(),"void","double");
    pDesc->setPropertyInfo("LogScaling","isLogScaling","setLogScaling",QStringList()<<"int",QStringList(),"void","int");
}


CVtkHyperStreamline::CVtkHyperStreamline() : m_vtkHyperStreamline(0)
{
    CVtkHyperStreamline::InitializeObjectDesc();
    setVtkObject(vtkHyperStreamline::New());
}

CVtkHyperStreamline::~CVtkHyperStreamline()
{

}


Point3D CVtkHyperStreamline::startPosition() const
{
    return Point3D(m_vtkHyperStreamline->GetStartPosition());
}

void CVtkHyperStreamline::setStartPosition(Point3D p)
{
    m_vtkHyperStreamline->SetStartPosition(p.x,p.y,p.z);
}


CVtkHyperStreamline::IntegrationEigenvector CVtkHyperStreamline::integrationEigenvector() const
{
    switch(m_vtkHyperStreamline->GetIntegrationEigenvector())
    {
        case VTK_INTEGRATE_MAJOR_EIGENVECTOR: return Major;
        case VTK_INTEGRATE_MEDIUM_EIGENVECTOR: return Medium;
        case VTK_INTEGRATE_MINOR_EIGENVECTOR: return Minor ;
        default : return Major;
    }
}

void CVtkHyperStreamline::setIntegrationEigenvector(IntegrationEigenvector val)
{
    switch(val)
    {
        case Major:    m_vtkHyperStreamline->SetIntegrationEigenvector(VTK_INTEGRATE_MAJOR_EIGENVECTOR);break;
        case Medium:    m_vtkHyperStreamline->SetIntegrationEigenvector(VTK_INTEGRATE_MEDIUM_EIGENVECTOR);break;
        case Minor:    m_vtkHyperStreamline->SetIntegrationEigenvector(VTK_INTEGRATE_MINOR_EIGENVECTOR);break;
        default:m_vtkHyperStreamline->SetIntegrationEigenvector(VTK_INTEGRATE_MAJOR_EIGENVECTOR);break;
    }
}


double CVtkHyperStreamline::integrationStepLength() const
{
    return m_vtkHyperStreamline->GetIntegrationStepLength();
}

void CVtkHyperStreamline::setIntegrationStepLength(double val)
{
    m_vtkHyperStreamline->SetIntegrationStepLength(val);
}

double CVtkHyperStreamline::stepLength() const
{
    return m_vtkHyperStreamline->GetStepLength();
}

void CVtkHyperStreamline::setStepLength(double val)
{
    m_vtkHyperStreamline->SetStepLength(val);
}


CVtkHyperStreamline::IntegrationDirection CVtkHyperStreamline::integrationDirection() const
{
    switch(m_vtkHyperStreamline->GetIntegrationDirection())
    {
        case VTK_INTEGRATE_FORWARD:
            return Forward;
        case VTK_INTEGRATE_BACKWARD:
            return Backward;
        case VTK_INTEGRATE_BOTH_DIRECTIONS:
            return BothDirections;
        default:
            return Forward;
    }
}

void CVtkHyperStreamline::setIntegrationDirection(IntegrationDirection val)
{
    switch(val)
    {
        case Forward:m_vtkHyperStreamline->SetIntegrationDirection(VTK_INTEGRATE_FORWARD);
                break;
        case Backward:m_vtkHyperStreamline->SetIntegrationDirection(VTK_INTEGRATE_BACKWARD);
            break;
        case BothDirections:m_vtkHyperStreamline->SetIntegrationDirection(VTK_INTEGRATE_BOTH_DIRECTIONS);
            break;
        default :m_vtkHyperStreamline->SetIntegrationDirection(VTK_INTEGRATE_FORWARD);
            break;
    }
}


double CVtkHyperStreamline::terminalEigenvalue() const
{
    return m_vtkHyperStreamline->GetTerminalEigenvalue();
}

void CVtkHyperStreamline::setTerminalEigenvalue(double val)
{
    m_vtkHyperStreamline->SetTerminalEigenvalue(val);
}

int CVtkHyperStreamline::numberOfSides() const
{
    return m_vtkHyperStreamline->GetNumberOfSides();
}

void CVtkHyperStreamline::setNumberOfSides(int val)
{
    m_vtkHyperStreamline->SetNumberOfSides(val);
}

double CVtkHyperStreamline::radius() const
{
    return m_vtkHyperStreamline->GetRadius();
}

void CVtkHyperStreamline::setRadius(double val)
{
    m_vtkHyperStreamline->SetRadius(val);
}

bool CVtkHyperStreamline::isLogScaling() const
{
    return m_vtkHyperStreamline->GetLogScaling();
}

void CVtkHyperStreamline::setLogScaling(bool val)
{
    m_vtkHyperStreamline->SetLogScaling(val);
}

