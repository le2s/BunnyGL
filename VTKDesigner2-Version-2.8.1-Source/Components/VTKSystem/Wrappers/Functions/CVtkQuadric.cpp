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
#include "CVtkQuadric.h"
#include <QList>

DEFINE_VTK_OBJECT(CVtkQuadric,CVtkImplicitFunction,vtkQuadric)
{
    pDesc->setNodeClassName("vtkQuadric");
    pDesc->setNodeClassDescription("Evaluate implicit quadric function");
    pDesc->setNodeClassCategory("Functions");

    pDesc->setPropertyInfo("Coefficients", "SetCoefficients", "GetCoefficients", QStringList() << "QList<double>", QStringList(), "void", "QList<double>");
}

CVtkQuadric::CVtkQuadric() : m_vtkQuadric(0)
{
    CVtkQuadric::InitializeObjectDesc();
    setVtkObject(vtkQuadric::New());
}

CVtkQuadric::~CVtkQuadric()
{

}

QList<QVariant> CVtkQuadric::coefficients() const
{
    double *val = 0;
    QList<QVariant> list;
    val = m_vtkQuadric->GetCoefficients();
    if(val)
    {
        for(int i = 0; i < 10; i++)
            list.append(val[i]);
    }
    return list;
}


void CVtkQuadric::setCoefficients(QList<QVariant> val)
{
    double vec[10];
    for(int i=0; i<10; i++)
    {
        if(i >= val.count())
            vec[i] = 0;
        else
            vec[i] = val[i].toDouble();
    }
    m_vtkQuadric->SetCoefficients(vec);
}

