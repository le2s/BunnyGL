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

#include "CVtkVisibilitySort.h"
#include "VTKVolumeRenderingSystemComponent.h"
#include "vtkVisibilitySort.h"

#include "IVtkDataSetIOData.h"
#include "IVtkCameraIOData.h"

#include "vtkDataSet.h"
#include "CVtkObjectIOData.h"

DEFINE_VIS_NODE(CVtkVisibilitySort, CGenericVisNodeBase)
{
    pDesc->setNodeClassCategory("VTKVolumeRenderingSystem");
    pDesc->setNodeClassName("vtkVisibilitySort");
    pDesc->setNodeClassDescription("vtkVisibilitySort");
    pDesc->setNodeIcon( VTKVolumeRenderingSystemComponent::instance().nodeIcon() );

    // Uncomment and use the following code template to add input/output paths
    pDesc->addConnectionPath(
        new CGenericVisNodeConnectionPath(
            "DataSet",                                 // Name of the path
            IVisSystemNodeConnectionPath::InputPath,   // Path type can be OutputPath or InputPath
            "vtkDataSet",                              // Data type of the path
            0,                                         // Path index (don't change)
            false                                      // Allow Multiple Inputs Flag
        )
    );

    pDesc->addConnectionPath(
        new CGenericVisNodeConnectionPath(
            "Camera",                                  // Name of the path
            IVisSystemNodeConnectionPath::InputPath,   // Path type can be OutputPath or InputPath
            "vtkCamera",                               // Data type of the path
            0,                                         // Path index (don't change)
            false                                      // Allow Multiple Inputs Flag
        )
    );

    pDesc->addConnectionPath(
        new CGenericVisNodeConnectionPath(
            "Visibility Sort",                         // Name of the path
            IVisSystemNodeConnectionPath::OutputPath,  // Path type can be OutputPath or InputPath
            "vtkVisibilitySort",                       // Data type of the path
            0,                                         // Path index (don't change)
            false                                      // Allow Multiple Inputs Flag
        )
    );

    pDesc->setCreateFunction(0);
}

struct CVtkVisibilitySortData
{
    vtkVisibilitySort *visibilitySort;
    CVtkObjectIOData outputData;
};

CVtkVisibilitySort::CVtkVisibilitySort()
{
    CVtkVisibilitySort::InitializeNodeDesc();
    d = new CVtkVisibilitySortData;

    d->visibilitySort = 0;
}

CVtkVisibilitySort::~CVtkVisibilitySort()
{
    // This would be a good time to delete the backend object. If the backend object is a vtkObject
    // subclass, then you have to delete it now.
    // d->VtkObjectPointer->Delete();
    delete d;
}

void CVtkVisibilitySort::setDirection(CVtkVisibilitySort::Direction dir)
{
    d->visibilitySort->SetDirection( int(dir) );
}

CVtkVisibilitySort::Direction CVtkVisibilitySort::direction() const
{
    return CVtkVisibilitySort::Direction(
                d->visibilitySort->GetDirection()
            );
}

bool CVtkVisibilitySort::hasInput(IVisSystemNodeConnectionPath* path)
{
    if(!path)
        return false;

    if(path->pathName() == "DataSet" )
        return d->visibilitySort->GetInput();

    if(path->pathName() == "Camera")
        return d->visibilitySort->GetCamera();
    
    return CGenericVisNodeBase::hasInput(path);
}

bool CVtkVisibilitySort::setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    if(path->pathName() == "DataSet")
    {
        IVtkDataSetIOData* data = 0;
        bool success = inputData->queryInterface("IVtkDataSetIOData", (void**)&data);
        if(success && data)
        {
            vtkDataSet* ds = data->getVtkDataSet();
            if(!ds)
                return false;

            d->visibilitySort->SetInput(ds);
            return true;
        }

        return false;
    }

    if(path->pathName() == "Camera")
    {
        IVtkCameraIOData* data = 0;
        bool success = inputData->queryInterface("IVtkCameraIOData", (void**)&data);
        if(success && data)
        {
            vtkCamera* cam = data->getVtkCamera();
            if(!cam)
                return false;
            
            d->visibilitySort->SetCamera(cam);
            return true;
        }

        return false;
    }

    return CGenericVisNodeBase::setInput(path, inputData);
}

bool CVtkVisibilitySort::removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    if(path->pathName() == "DataSet")
    {
        IVtkDataSetIOData* data = 0;
        bool success = inputData->queryInterface("IVtkDataSetIOData", (void**)&data);

        if(success && data)
        {
            vtkDataSet* ds = data->getVtkDataSet();
            if(!ds)
                return false;
            
            if(ds != d->visibilitySort->GetInput())
                return false;

            d->visibilitySort->SetInput(0);
            return true;
        }

        return false;
    }

    if(path->pathName() == "Camera")
    {
        IVtkCameraIOData* data = 0;
        bool success = inputData->queryInterface("IVtkCameraIOData", (void**)&data);
        
        if(success && data)
        {
            vtkCamera* cam = data->getVtkCamera();
           
            if(!cam)
                return false;

            if(cam != d->visibilitySort->GetCamera())
                return false;

            d->visibilitySort->SetCamera(0);
            return true;
        }

        return false;
    }
    
    return CGenericVisNodeBase::removeInput(path, inputData);
}

bool CVtkVisibilitySort::fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData)
{
    if(!path || !outputData)
        return false;

    if(path->pathName() == "Visibility Sort")
    {
        *outputData = &d->outputData;
        return true;
    }

    return CGenericVisNodeBase::fetchOutput(path, outputData);
}

bool CVtkVisibilitySort::outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData)
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

int CVtkVisibilitySort::propertyCount()
{
    return 0;
}

QString CVtkVisibilitySort::propertyName(int index)
{
    Q_UNUSED(index);
    return QString();
}

QString CVtkVisibilitySort::propertyGroup(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant CVtkVisibilitySort::propertyValue(int index)
{
    Q_UNUSED(index);
    return QVariant();
}

void CVtkVisibilitySort::setPropertyValue(int index, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(value);
}

bool CVtkVisibilitySort::hasEditor(int index)
{
    Q_UNUSED(index);
    return false;
}

QWidget* CVtkVisibilitySort::createEditor(int index)
{
    Q_UNUSED(index);
    return 0;
}

void CVtkVisibilitySort::setEditorValue(int index, QWidget* widget, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(value);
}

QVariant CVtkVisibilitySort::editorValue(int index, QWidget* widget)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
}

void CVtkVisibilitySort::connectEditor(int index, QWidget* widget, QObject* receiver, const char* member)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(receiver);
    Q_UNUSED(member);
}

QString CVtkVisibilitySort::propertyValueString(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant CVtkVisibilitySort::propertyValueFromString(int index, QString valueStr)
{
    Q_UNUSED(index);
    Q_UNUSED(valueStr);

    return QVariant();
}

bool CVtkVisibilitySort::canLoadSaveProperty(int index)
{
    Q_UNUSED(index);

    return false;
}

#endif

void CVtkVisibilitySort::setVtkObject(vtkObject* ptr)
{
    d->visibilitySort = dynamic_cast<vtkVisibilitySort*>(ptr);
    d->outputData.setObject(ptr);
}





