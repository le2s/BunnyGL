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

#include "CVtkImageMapper.h"
#include "IVtkImageDataIOData.h"
#include "vtkImageData.h"

DEFINE_VTK_OBJECT(CVtkImageMapper, CVtkMapper2D, vtkImageMapper)
{
    pDesc->setNodeClassCategory("Rendering");
    pDesc->setNodeClassName("vtkImageMapper");

    pDesc->setPropertyInfo("ColorWindow", "SetColorWindow", "GetColorWindow", QStringList()<<"double",QStringList(),"void","double");
    pDesc->setPropertyInfo("ColorLevel", "SetColorLevel", "GetColorLevel", QStringList()<<"double",QStringList(),"void","double");
    pDesc->setPropertyInfo("ZSlice", "SetZSlice", "GetZSlice", QStringList()<<"double",QStringList(),"void","double");
    
    // Image input
    pDesc->addConnectionPath(
        new CVtkObjectConnectionPath("ImageData", IVisSystemNodeConnectionPath::InputPath, "vtkImageData", 0)
        );
}

CVtkImageMapper::CVtkImageMapper() : m_vtkImageMapper(0)
{
    CVtkImageMapper::InitializeObjectDesc();
    setVtkObject( vtkImageMapper::New() );
}

CVtkImageMapper::~CVtkImageMapper()
{

}

void CVtkImageMapper::setColorWindow(double val)
{
    m_vtkImageMapper->SetColorWindow(val);
}

double CVtkImageMapper::colorWindow() const
{
    return m_vtkImageMapper->GetColorWindow();
}

void CVtkImageMapper::setColorLevel(double val)
{
    m_vtkImageMapper->SetColorLevel( val );
}

double CVtkImageMapper::colorLevel() const
{
    return m_vtkImageMapper->GetColorLevel();
}

void CVtkImageMapper::setZSlice(double val)
{
    m_vtkImageMapper->SetZSlice( val );
}

double CVtkImageMapper::zSlice() const
{
    return m_vtkImageMapper->GetZSlice();
}

double CVtkImageMapper::getColorShift() const
{
    return m_vtkImageMapper->GetColorShift();
}

double CVtkImageMapper::getColorScale() const
{
    return m_vtkImageMapper->GetColorScale();
}

bool CVtkImageMapper::hasInput(IVisSystemNodeConnectionPath* path)
{
    if(!path)
        return false;

    if(path->pathName() == "ImageData")
        return m_vtkImageMapper->GetInput();

    return CVtkMapper2D::hasInput(path);
}

bool CVtkImageMapper::setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if( !path || !inputData )
        return false;

    if(path->pathName() == "ImageData")
    {
        IVtkImageDataIOData* data = 0;
        bool success = inputData->queryInterface("IVtkImageDataIOData", (void**)&data);
        if( success && data->getVtkImageData() )
        {
            m_vtkImageMapper->SetInput( data->getVtkImageData() );
            return true;
        }
        return false;
    }

    return CVtkMapper2D::setInput(path, inputData);
}

bool CVtkImageMapper::removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if( !path || !inputData )
        return false;

    if(path->pathName() == "ImageData")
    {
        IVtkImageDataIOData* data = 0;
        bool success = inputData->queryInterface("IVtkImageDataIOData", (void**)&data);
        if( success && data->getVtkImageData() == m_vtkImageMapper->GetInput() )
        {
            m_vtkImageMapper->SetInput( 0 );
            return true;
        }
        return false;
    }

    return CVtkMapper2D::removeInput(path, inputData);
}

