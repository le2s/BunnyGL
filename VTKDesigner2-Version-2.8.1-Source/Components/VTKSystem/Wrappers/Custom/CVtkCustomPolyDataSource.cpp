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

#include "CVtkCustomPolyDataSource.h"
#include "CScriptableVtkPolyData.h"
#include "vtkObjectFactory.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "CVtkVisSystemComponent.h"

#define NCTHIS (const_cast<CVtkCustomPolyDataSource*>(this))

DEFINE_VTK_OBJECT(CVtkCustomPolyDataSource, CVtkPolyDataAlgorithm, vtkCustomPolyDataSource)
{
    pDesc->setNodeClassName("vtkCustomPolyDataSource");
    pDesc->setNodeClassCategory("Scriptable Nodes");
    pDesc->setNodeClassDescription("Scriptable polydata source");
	pDesc->setNodeIcon(qApp->windowIcon()); // vtkCustomFunction is not a standard VTK class
}

CVtkCustomPolyDataSource::CVtkCustomPolyDataSource() : m_vtkCustomPolyDataSource(0),
m_polyDataObject(0)
{
    CVtkCustomPolyDataSource::InitializeObjectDesc();
    setVtkObject(vtkCustomPolyDataSource::New());
    m_vtkCustomPolyDataSource->SetCustomPolyDataSource(this);
    m_vtkCustomPolyDataSource->Update();
    m_storeData = false;
}

CVtkCustomPolyDataSource::~CVtkCustomPolyDataSource()
{
    if(m_polyDataObject)
        delete m_polyDataObject;
}

QObject* CVtkCustomPolyDataSource::polyDataObject()
{
    if(!m_polyDataObject)
        m_polyDataObject = new CScriptableVtkPolyData(m_vtkCustomPolyDataSource->GetOutput(), this);
    return m_polyDataObject;
}

QList<double> CVtkCustomPolyDataSource::points() const
{
    NCTHIS->polyDataObject();
    return m_polyDataObject->points();
}

void CVtkCustomPolyDataSource::setPoints(QList<double> pArray)
{
    NCTHIS->polyDataObject();
    return m_polyDataObject->setPoints(pArray);
}

void CVtkCustomPolyDataSource::setVerts(QList<int> v)
{
    NCTHIS->polyDataObject();
    m_polyDataObject->setVerts(v);
}

QList<int> CVtkCustomPolyDataSource::verts() const
{
    NCTHIS->polyDataObject();
    return m_polyDataObject->verts();
}

void CVtkCustomPolyDataSource::setLines(QList<int> lines)
{
    NCTHIS->polyDataObject();
    m_polyDataObject->setLines(lines);
}

QList<int> CVtkCustomPolyDataSource::lines() const
{
    NCTHIS->polyDataObject();
    return m_polyDataObject->lines();
}


void CVtkCustomPolyDataSource::setStrips(QList<int> strips)
{
    NCTHIS->polyDataObject();
    m_polyDataObject->setStrips(strips);
}

QList<int> CVtkCustomPolyDataSource::strips() const
{
    NCTHIS->polyDataObject();
    return m_polyDataObject->strips();
}


void CVtkCustomPolyDataSource::setPolys(QList<int> polys)
{
    NCTHIS->polyDataObject();
    return m_polyDataObject->setPolys(polys);
}

QList<int> CVtkCustomPolyDataSource::polys() const
{
    NCTHIS->polyDataObject();
    return m_polyDataObject->polys();
}


QList<double> CVtkCustomPolyDataSource::cellNormals() const
{
    NCTHIS->polyDataObject();
    return m_polyDataObject->cellNormals();
}

void CVtkCustomPolyDataSource::setCellNormals(QList<double> normalsArray)
{
    NCTHIS->polyDataObject();
    m_polyDataObject->setCellNormals(normalsArray);
}


QList<double> CVtkCustomPolyDataSource::cellScalars() const
{
    NCTHIS->polyDataObject();
    return m_polyDataObject->cellScalars();
}

void CVtkCustomPolyDataSource::setCellScalars(QList<double> scalars)
{
    NCTHIS->polyDataObject();
    m_polyDataObject->setCellScalars(scalars);
}


QList<double> CVtkCustomPolyDataSource::pointNormals() const
{
    NCTHIS->polyDataObject();
    return m_polyDataObject->pointNormals();
}

void CVtkCustomPolyDataSource::setPointNormals(QList<double> normalsArray)
{
    NCTHIS->polyDataObject();
    m_polyDataObject->setPointNormals(normalsArray);
}


QList<double> CVtkCustomPolyDataSource::pointScalars() const
{
    NCTHIS->polyDataObject();
    return m_polyDataObject->pointScalars();
}

void CVtkCustomPolyDataSource::setPointScalars(QList<double> scalars)
{
    NCTHIS->polyDataObject();
    m_polyDataObject->setPointScalars(scalars);
}

int CVtkCustomPolyDataSource::numberOfPoints() const
{
    NCTHIS->polyDataObject();
    return m_polyDataObject->numberOfPoints();
}

int CVtkCustomPolyDataSource::numberOfVerts() const
{
    NCTHIS->polyDataObject();
    return m_polyDataObject->numberOfVerts();
}

int CVtkCustomPolyDataSource::numberOfLines() const
{
    NCTHIS->polyDataObject();
    return m_polyDataObject->numberOfLines();
}

int CVtkCustomPolyDataSource::numberOfStrips() const
{
    NCTHIS->polyDataObject();
    return m_polyDataObject->numberOfStrips();
}

int CVtkCustomPolyDataSource::numberOfPolys() const
{
    NCTHIS->polyDataObject();
    return m_polyDataObject->numberOfPolys();
}

void CVtkCustomPolyDataSource::setStoreData(bool val)
{
    m_storeData = val;
}

bool CVtkCustomPolyDataSource::isStoreData() const
{
    return m_storeData;
}

QString CVtkCustomPolyDataSource::projectPartID()
{
    return this->nodeName();
}

bool CVtkCustomPolyDataSource::newProjectPart()
{
    vtkPolyData* polyData = m_vtkCustomPolyDataSource->GetOutput();
    polyData->ReleaseData();

    return true;
}

bool CVtkCustomPolyDataSource::loadProjectPart(QDomDocument doc, QDomElement partE)
{
    // Catch hold of the output polydata
    vtkPolyData* polyData = vtkPolyData::New();

    // Load it from XML
    CVtkVisSystemComponent::instance().loadPolyData(polyData, doc, partE);
    m_vtkCustomPolyDataSource->SetCustomPolyDataOutput(polyData);
    polyData->Delete();

    return true;
}

bool CVtkCustomPolyDataSource::initializeProjectPart()
{
    return true; // Do nothing
}

bool CVtkCustomPolyDataSource::saveProjectPart(QDomDocument doc, QDomElement partParentE)
{
    if( !m_storeData )
        return true;

    // Catch hold of the output polydata
    vtkPolyData* polyData = m_vtkCustomPolyDataSource->GetOutput();

    // Save it into XML
    CVtkVisSystemComponent::instance().savePolyData(polyData, doc, partParentE);

    // All done
    return true;
}

bool CVtkCustomPolyDataSource::closeProjectPart()
{
    return newProjectPart();
}

////////////////////////////////////////////////////////////////////////////////////////////////
// vtkCustomPolyDataSource implementation
////////////////////////////////////////////////////////////////////////////////////////////////

vtkCxxRevisionMacro(vtkCustomPolyDataSource, "$Revision: 1.53 $");
vtkStandardNewMacro(vtkCustomPolyDataSource);

void vtkCustomPolyDataSource::PrintSelf(ostream& os, vtkIndent indent)
{
    vtkPolyDataAlgorithm::PrintSelf(os, indent);
}

void vtkCustomPolyDataSource::SetCustomPolyDataSource(CVtkCustomPolyDataSource* source)
{
    m_CVtkCustomPolyDataSource = source;
}

CVtkCustomPolyDataSource* vtkCustomPolyDataSource::GetCustomPolyDataSource()
{
    return m_CVtkCustomPolyDataSource;
}

vtkCustomPolyDataSource::vtkCustomPolyDataSource() : m_CVtkCustomPolyDataSource(0)
{
    this->SetNumberOfInputPorts(0);
    this->RequestingData = 0;
    this->CustomPolyDataOutput = 0;
}

vtkCustomPolyDataSource::~vtkCustomPolyDataSource()
{

}

void vtkCustomPolyDataSource::SetCustomPolyDataOutput(vtkPolyData* polyData)
{
    if( this->CustomPolyDataOutput == polyData )
        return;

    if(this->CustomPolyDataOutput)
        this->CustomPolyDataOutput->UnRegister(this);

    this->CustomPolyDataOutput = polyData;

    if(this->CustomPolyDataOutput)
        this->CustomPolyDataOutput->Register(this);

    this->Modified();
}

int vtkCustomPolyDataSource::RequestData(
            vtkInformation *vtkNotUsed(request),
            vtkInformationVector **vtkNotUsed(inputVector),
            vtkInformationVector *outputVector
        )
{
    if(this->RequestingData)
        return 1;

    this->RequestingData = 1;

    // get the info object
    vtkInformation *outInfo = outputVector->GetInformationObject(0);

    // get the ouptut
    vtkPolyData *output = vtkPolyData::SafeDownCast(outInfo->Get(vtkDataObject::DATA_OBJECT()));
    Q_UNUSED(output);

    if( this->CustomPolyDataOutput )
    {
        output->ShallowCopy(this->CustomPolyDataOutput);
        this->CustomPolyDataOutput->UnRegister(this);
        this->CustomPolyDataOutput = 0;
    }
    else if(m_CVtkCustomPolyDataSource)
        m_CVtkCustomPolyDataSource->OnRequestData();

    this->RequestingData = 0;

    return 1;
}

