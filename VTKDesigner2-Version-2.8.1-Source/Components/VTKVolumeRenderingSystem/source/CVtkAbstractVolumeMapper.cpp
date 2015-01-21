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

#include "CVtkAbstractVolumeMapper.h"
#include "VTKVolumeRenderingSystemComponent.h"
#include "vtkAbstractVolumeMapper.h"

#include "IVtkDataSetIOData.h"
#include "CVtkObjectIOData.h"

#include "vtkDataSet.h"
#include "vtkPointData.h"
#include "vtkDataArray.h"

DEFINE_VIS_NODE(CVtkAbstractVolumeMapper, CGenericVisNodeBase)
{
    pDesc->setNodeClassCategory("VTKVolumeRenderingSystem");
    pDesc->setNodeClassName("vtkAbstractVolumeMapper");
    pDesc->setNodeClassDescription("vtkAbstractVolumeMapper");
    pDesc->setNodeIcon( VTKVolumeRenderingSystemComponent::instance().nodeIcon() );

    // DataSet input
    pDesc->addConnectionPath(
        new CGenericVisNodeConnectionPath(
                "DataSet",                                 // Name of the path
                IVisSystemNodeConnectionPath::InputPath,   // Path type can be OutputPath or InputPath
                "vtkDataSet",                              // Data type of the path
                0,                                         // Path index (don't change)
                false                                      // Allow Multiple Inputs Flag
            )
        );

    // DataSet input
    pDesc->addConnectionPath(
        new CGenericVisNodeConnectionPath(
                "Volume Mapper",                             // Name of the path
                IVisSystemNodeConnectionPath::OutputPath,   // Path type can be OutputPath or InputPath
                "vtkAbstractVolumeMapper",                  // Data type of the path
                0,                                          // Path index (don't change)
                false                                       // Allow Multiple Inputs Flag
            )
        );

    pDesc->setCreateFunction(0);
}

struct CVtkAbstractVolumeMapperData
{
    vtkAbstractVolumeMapper *abstractVolumeMapper;
    CVtkObjectIOData mapperData;
    bool hasDataSetInput;
};

CVtkAbstractVolumeMapper::CVtkAbstractVolumeMapper()
{
    CVtkAbstractVolumeMapper::InitializeNodeDesc();
    d = new CVtkAbstractVolumeMapperData;
    d->hasDataSetInput = false;

    d->abstractVolumeMapper = 0;
}

CVtkAbstractVolumeMapper::~CVtkAbstractVolumeMapper()
{
    // This would be a good time to delete the backend object. If the backend object is a vtkObject
    // subclass, then you have to delete it now.
    // d->VtkObjectPointer->Delete();

    delete d;
}

QList<double> CVtkAbstractVolumeMapper::bounds() const
{
    QList<double> ret;

    double* bounds = d->abstractVolumeMapper->GetBounds();
    for(int i=0; i<6; i++)
        ret << bounds[i];

    return ret;
}

Point3D CVtkAbstractVolumeMapper::center() const
{
    double* c = d->abstractVolumeMapper->GetCenter();
    return Point3D(c);
}

bool CVtkAbstractVolumeMapper::hasInput(IVisSystemNodeConnectionPath* path)
{
    if(!path)
        return false;

    if(path->pathName() == "DataSet")
        return d->hasDataSetInput;

    return CGenericVisNodeBase::hasInput(path);
}

bool CVtkAbstractVolumeMapper::setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    if(path->pathName() == "DataSet")
    {
        IVtkDataSetIOData* data = 0;
        bool success = inputData->queryInterface("IVtkDataSetIOData", (void**)&data);
        if(success && data)
        {
            vtkDataSet* dataSet = data->getVtkDataSet();

            // Most volume rendering algorithms make use of scalarType 
            // to figure out what to do next. Lets make sure that the 
            // scalarType is reachable.
            vtkPointData* pointData = dataSet ? dataSet->GetPointData() : 0;
            vtkDataArray* scalars   = pointData ? pointData->GetScalars() : 0;
            int           scalarType = scalars ? scalars->GetDataType() : 0;
            if(!pointData || !scalars || !scalarType)
            {
                qDebug("ScalarType for input could not be found. Invalid input");
                return false;
            }

            d->abstractVolumeMapper->SetInput( dataSet );
            d->hasDataSetInput = true;
            return true;
        }

        return false;
    }

    return CGenericVisNodeBase::setInput(path, inputData);
}

bool CVtkAbstractVolumeMapper::removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    if(path->pathName() == "DataSet")
    {
        IVtkDataSetIOData* data = 0;
        bool success = inputData->queryInterface("IVtkDataSetIOData", (void**)&data);
        if(success && data)
        {
            d->abstractVolumeMapper->SetInput( 0 );
            d->hasDataSetInput = false;
            return true;
        }

        return false;
    }

    return CGenericVisNodeBase::removeInput(path, inputData);
}

bool CVtkAbstractVolumeMapper::fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData)
{
    if(!path || !outputData)
        return false;

    if(path->pathName() == "Volume Mapper")
    {
        *outputData = &d->mapperData;
        return true;
    }

    return CGenericVisNodeBase::fetchOutput(path, outputData);
}

bool CVtkAbstractVolumeMapper::outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData)
{
    if(!path || !outputData)
        return false;

    /*
    If you have added output paths in the description block at the header of this file,
    then you will have to handle outputs here
    */

    return CGenericVisNodeBase::outputDerefed(path, outputData);
}

#ifdef ENABLE_ADVANCED_PROPERTIES

int CVtkAbstractVolumeMapper::propertyCount()
{
    return 0;
}

QString CVtkAbstractVolumeMapper::propertyName(int index)
{
    Q_UNUSED(index);
    return QString();
}

QString CVtkAbstractVolumeMapper::propertyGroup(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant CVtkAbstractVolumeMapper::propertyValue(int index)
{
    Q_UNUSED(index);
    return QVariant();
}

void CVtkAbstractVolumeMapper::setPropertyValue(int index, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(value);
}

bool CVtkAbstractVolumeMapper::hasEditor(int index)
{
    Q_UNUSED(index);
    return false;
}

QWidget* CVtkAbstractVolumeMapper::createEditor(int index)
{
    Q_UNUSED(index);
    return 0;
}

void CVtkAbstractVolumeMapper::setEditorValue(int index, QWidget* widget, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(value);
}

QVariant CVtkAbstractVolumeMapper::editorValue(int index, QWidget* widget)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
}

void CVtkAbstractVolumeMapper::connectEditor(int index, QWidget* widget, QObject* receiver, const char* member)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(receiver);
    Q_UNUSED(member);
}

QString CVtkAbstractVolumeMapper::propertyValueString(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant CVtkAbstractVolumeMapper::propertyValueFromString(int index, QString valueStr)
{
    Q_UNUSED(index);
    Q_UNUSED(valueStr);

    return QVariant();
}

bool CVtkAbstractVolumeMapper::canLoadSaveProperty(int index)
{
    Q_UNUSED(index);

    return false;
}

#endif

void CVtkAbstractVolumeMapper::setVtkObject(vtkObject* ptr)
{
    d->abstractVolumeMapper = dynamic_cast<vtkAbstractVolumeMapper*>(ptr);
    d->mapperData.setObject(ptr);
}





