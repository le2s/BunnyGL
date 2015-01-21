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

#include "CVtkInteractorObserverData.h"

CVtkInteractorObserverData::CVtkInteractorObserverData(vtkInteractorObserver* interactorObserver)
{
    m_vtkInteractorObserver = interactorObserver;
}

CVtkInteractorObserverData::~CVtkInteractorObserverData()
{

}

void CVtkInteractorObserverData::setInteractorObserver(vtkInteractorObserver* interactorObserver)
{
    m_vtkInteractorObserver = interactorObserver;
}

GCF_DEFINE_QUERY_TABLE(CVtkInteractorObserverData)
    GCF_IMPLEMENTS(IVtkInteractorObserverIOData)
    GCF_IMPLEMENTS(IVtkObjectIOData)
GCF_END_QUERY_TABLE(CVtkInteractorObserverData)

void* CVtkInteractorObserverData::dataPointer()
{
    return m_vtkInteractorObserver;
}

QString CVtkInteractorObserverData::dataTypeName()
{
    return "vtkInteractorObserver";
}

vtkInteractorObserver* CVtkInteractorObserverData::getVtkInteractorObserver()
{
    return m_vtkInteractorObserver;
}

vtkObject* CVtkInteractorObserverData::getVtkObject()
{
    return vtkObject::SafeDownCast(m_vtkInteractorObserver);
}


