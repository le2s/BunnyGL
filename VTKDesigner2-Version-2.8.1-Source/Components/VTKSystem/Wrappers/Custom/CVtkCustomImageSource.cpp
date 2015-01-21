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

#include "CVtkCustomImageSource.h"
#include "CScriptableVtkImageData.h"
#include "vtkObjectFactory.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"


DEFINE_VTK_OBJECT(CVtkCustomImageSource, CVtkAlgorithm, vtkImageAlgorithm)
{
    pDesc->setNodeClassName("vtkCustomImageSource");
    pDesc->setNodeClassDescription("Custom Image Source");
    pDesc->setNodeClassCategory("Scriptable Nodes");
	pDesc->setNodeIcon(qApp->windowIcon()); // vtkCustomFunction is not a standard VTK class

    // Statistics Output
    pDesc->addConnectionPath(
        new CVtkObjectConnectionPath("ImageSource", IVisSystemNodeConnectionPath::OutputPath, "vtkCustomImageSource", 0)
        );
}

CVtkCustomImageSource::CVtkCustomImageSource() : m_vtkImageAlgorithm(0), m_imageDataAccess(0), isRequestingData(false)
{
    CVtkCustomImageSource::InitializeObjectDesc();
    vtkCustomImageSource* cis = vtkCustomImageSource::New();
    setVtkObject(cis);
    m_imageDataAccess = new CScriptableVtkImageData(cis->GetOutput(), this);
    cis->SetCustomImageSource(this);
}

CVtkCustomImageSource::~CVtkCustomImageSource()
{
    delete m_imageDataAccess;
}

QList<int> CVtkCustomImageSource::dimensions() const
{
    return m_imageDataAccess->dimensions();
}

void CVtkCustomImageSource::setDimensions(QList<int> dim)
{
    if(isRequestingData)
        m_imageDataAccess->setDimensions(dim);
}

QList<double> CVtkCustomImageSource::spacing() const
{
    return m_imageDataAccess->spacing();
}

void CVtkCustomImageSource::setSpacing(QList<double> dim)
{
    if(isRequestingData)
        m_imageDataAccess->setSpacing(dim);
}

Point3D CVtkCustomImageSource::origin() const
{
    return m_imageDataAccess->origin();
}

void CVtkCustomImageSource::setOrigin(Point3D point)
{
    if(isRequestingData)
        m_imageDataAccess->setOrigin(point);
}

QString CVtkCustomImageSource::scalarType() const
{
    return m_imageDataAccess->scalarType();
}

void CVtkCustomImageSource::setScalarTypeToDouble()
{
    if(isRequestingData)
        m_imageDataAccess->setScalarTypeToDouble();
}

void CVtkCustomImageSource::setScalarTypeToChar()
{
    if(isRequestingData)
        m_imageDataAccess->setScalarTypeToChar();
}

void CVtkCustomImageSource::setScalarTypeToInt()
{
    if(isRequestingData)
        m_imageDataAccess->setScalarTypeToInt();
}

void CVtkCustomImageSource::setScalarTypeToUnsignedChar()
{
    if(isRequestingData)
        m_imageDataAccess->setScalarTypeToUnsignedChar();
}

void CVtkCustomImageSource::setScalarTypeToUnsignedInt()
{
    if(isRequestingData)
        m_imageDataAccess->setScalarTypeToUnsignedInt();
}

int CVtkCustomImageSource::numberOfScalarComponents() const
{
    return m_imageDataAccess->numberOfScalarComponents();
}

void CVtkCustomImageSource::setNumberOfScalarComponents(int val)
{
    if(isRequestingData)
        m_imageDataAccess->setNumberOfScalarComponents(val);
}

void CVtkCustomImageSource::reset()
{
    if(isRequestingData)
        m_imageDataAccess->reset();
}

void CVtkCustomImageSource::allocateScalars()
{
    if(isRequestingData)
        m_imageDataAccess->allocateScalars();
}

int CVtkCustomImageSource::numberOfPoints() const
{
    return m_imageDataAccess->numberOfPoints();
}

Point3D CVtkCustomImageSource::point(int index) const
{
    return m_imageDataAccess->point(index);
}

Point3D CVtkCustomImageSource::point(int x, int y, int z) const
{
    return m_imageDataAccess->point(x, y, z);
}

QList<double> CVtkCustomImageSource::scalarComponent(int x, int y, int z) const
{
    return m_imageDataAccess->scalarComponent(x, y, z);
}

void CVtkCustomImageSource::setScalarComponent(int x, int y, int z, QList<double> scalar)
{
    if(isRequestingData)
        m_imageDataAccess->setScalarComponent(x, y, z, scalar);
}


bool CVtkCustomImageSource::fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData)
{
    if(!path || !outputData)
        return false;

    if(path->pathName() == "ImageSource")
    {
        m_imageData.setImageData( m_vtkImageAlgorithm->GetOutput() );
        *outputData = &m_imageData;
        return true;
    }

    return CVtkAlgorithm::fetchOutput(path, outputData);
}

bool CVtkCustomImageSource::outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData)
{
    if(!path || !outputData)
        return false;

    if(path->pathName() == "ImageSource")
    {
        return true;
    }

    return CVtkAlgorithm::outputDerefed(path, outputData);
}

    
////////////////////////////////////////////////////////////////////////////////////////////////
/// vtkCustomImageSource
////////////////////////////////////////////////////////////////////////////////////////////////

vtkCustomImageSource* vtkCustomImageSource::New()
{
    return new vtkCustomImageSource;
}

vtkCustomImageSource::vtkCustomImageSource()
{
    this->SetNumberOfInputPorts(0);
    this->SetNumberOfOutputPorts(1);
    this->CustomImageSource = 0;
}

vtkCustomImageSource::~vtkCustomImageSource()
{

}

int vtkCustomImageSource::RequestData(
            vtkInformation *vtkNotUsed(request),
            vtkInformationVector **vtkNotUsed(inputVector),
            vtkInformationVector *outputVector
        )
{
    if(CustomImageSource->isRequestingData)
        return 1;

    // get the info object
    vtkInformation *outInfo = outputVector->GetInformationObject(0);
    
    // get the ouptut
    vtkImageData *output = vtkImageData::SafeDownCast(outInfo->Get(vtkDataObject::DATA_OBJECT()));
    output->SetDimensions(10, 10, 1);
    output->SetNumberOfScalarComponents(1);
    output->AllocateScalars();

    vtkIdType index = 0;
    for(int i=0; i<10; i++)
    {
        for(int j=0; j<10; j++)
        {
            for(int k=0; k<1; k++)
            {
                double* point = output->GetPoint(index++);
                double dist = sqrt( point[0]*point[0] + point[1]*point[1] + point[2]*point[2] );
                output->SetScalarComponentFromDouble(i, j, k, 0, dist);
            }
        }
    }

    if(CustomImageSource)
    {
        CustomImageSource->isRequestingData = true;
        CustomImageSource->OnRequestData();
        CustomImageSource->isRequestingData = false;
    }

    return 1;
}


