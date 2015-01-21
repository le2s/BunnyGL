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

#include "CVtkProjectedTetrahedraMapper.h"
#include "VTKVolumeRenderingSystemComponent.h"
#include "vtkProjectedTetrahedraMapper.h"

#include "IVtkObjectIOData.h"
#include "vtkVisibilitySort.h"
#include "vtkCellCenterDepthSort.h"

DEFINE_VIS_NODE(CVtkProjectedTetrahedraMapper, CVtkUnstructuredGridVolumeMapper)
{
    pDesc->setNodeClassCategory("VTKVolumeRenderingSystem");
    pDesc->setNodeClassName("vtkProjectedTetrahedraMapper");
    pDesc->setNodeClassDescription("vtkProjectedTetrahedraMapper");
    pDesc->setNodeIcon( VTKVolumeRenderingSystemComponent::instance().nodeIcon() );

    // Uncomment and use the following code template to add input/output paths
    pDesc->addConnectionPath(
        new CGenericVisNodeConnectionPath(
                "VisibilitySort",                           // Name of the path
                IVisSystemNodeConnectionPath::InputPath,    // Path type can be OutputPath or InputPath
                "vtkVisibilitySort",                        // Data type of the path
                0,                                          // Path index (don't change)
                false                                       // Allow Multiple Inputs Flag
            )
        );
}

struct CVtkProjectedTetrahedraMapperData
{
    vtkProjectedTetrahedraMapper *projectedTetrahedraMapper;
    bool hasVisibilitySort;
};

CVtkProjectedTetrahedraMapper::CVtkProjectedTetrahedraMapper()
{
    CVtkProjectedTetrahedraMapper::InitializeNodeDesc();
    d = new CVtkProjectedTetrahedraMapperData;

    d->projectedTetrahedraMapper = 0;
    d->hasVisibilitySort = false;
    setVtkObject( vtkProjectedTetrahedraMapper::New() );
}

CVtkProjectedTetrahedraMapper::~CVtkProjectedTetrahedraMapper()
{
    // This would be a good time to delete the backend object. If the backend object is a vtkObject
    // subclass, then you have to delete it now.
    d->projectedTetrahedraMapper->Delete();
    delete d;
}

void CVtkProjectedTetrahedraMapper::setUseScalarMode(CVtkProjectedTetrahedraMapper::ScalarMode mode)
{
    d->projectedTetrahedraMapper->SetScalarMode( int(mode) );
}

CVtkProjectedTetrahedraMapper::ScalarMode CVtkProjectedTetrahedraMapper::useScalarMode() const
{
    return CVtkProjectedTetrahedraMapper::ScalarMode(
                d->projectedTetrahedraMapper->GetScalarMode()
            );
}

bool CVtkProjectedTetrahedraMapper::hasInput(IVisSystemNodeConnectionPath* path)
{
    if(!path)
        return false;

    if(path->pathName() == "VisibilitySort")
        return d->hasVisibilitySort;

    return CVtkUnstructuredGridVolumeMapper::hasInput(path);
}

bool CVtkProjectedTetrahedraMapper::setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    if(path->pathName() == "VisibilitySort")
    {
        IVtkObjectIOData* data = 0;
        bool success = inputData->queryInterface("IVtkObjectIOData", (void**)&data);
        if(success && data)
        {
            vtkObject* obj = data->getVtkObject();
            vtkVisibilitySort* visSortObj = dynamic_cast<vtkVisibilitySort*>(obj);
            if(!visSortObj)
                return false;

            d->projectedTetrahedraMapper->SetVisibilitySort(visSortObj);
            d->hasVisibilitySort = true;
            return true;
        }

        return false;
    }

    return CVtkUnstructuredGridVolumeMapper::setInput(path, inputData);
}

bool CVtkProjectedTetrahedraMapper::removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    if(path->pathName() == "VisibilitySort")
    {
        IVtkObjectIOData* data = 0;
        bool success = inputData->queryInterface("IVtkObjectIOData", (void**)&data);
        if(success && data)
        {
            vtkObject* obj = data->getVtkObject();
            vtkVisibilitySort* visSortObj = dynamic_cast<vtkVisibilitySort*>(obj);
            if(!visSortObj)
                return false;

            if(visSortObj != d->projectedTetrahedraMapper->GetVisibilitySort())
                return false;

            vtkCellCenterDepthSort* defVisSort = vtkCellCenterDepthSort::New();
            d->projectedTetrahedraMapper->SetVisibilitySort(defVisSort);
            defVisSort->Delete();
            d->hasVisibilitySort = false;

            return true;
        }

        return false;
    }

    return CVtkUnstructuredGridVolumeMapper::removeInput(path, inputData);
}

bool CVtkProjectedTetrahedraMapper::fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData)
{
    if(!path || !outputData)
        return false;

    /*
    If you have added output paths in the description block at the header of this file,
    then you will have to handle outputs here
    */

    return CVtkUnstructuredGridVolumeMapper::fetchOutput(path, outputData);
}

bool CVtkProjectedTetrahedraMapper::outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData)
{
    if(!path || !outputData)
        return false;

    /*
    If you have added output paths in the description block at the header of this file,
    then you will have to handle outputs here
    */

    return CVtkUnstructuredGridVolumeMapper::outputDerefed(path, outputData);
}

#ifdef ENABLE_ADVANCED_PROPERTIES

int CVtkProjectedTetrahedraMapper::propertyCount()
{
    return 0;
}

QString CVtkProjectedTetrahedraMapper::propertyName(int index)
{
    Q_UNUSED(index);
    return QString();
}

QString CVtkProjectedTetrahedraMapper::propertyGroup(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant CVtkProjectedTetrahedraMapper::propertyValue(int index)
{
    Q_UNUSED(index);
    return QVariant();
}

void CVtkProjectedTetrahedraMapper::setPropertyValue(int index, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(value);
}

bool CVtkProjectedTetrahedraMapper::hasEditor(int index)
{
    Q_UNUSED(index);
    return false;
}

QWidget* CVtkProjectedTetrahedraMapper::createEditor(int index)
{
    Q_UNUSED(index);
    return 0;
}

void CVtkProjectedTetrahedraMapper::setEditorValue(int index, QWidget* widget, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(value);
}

QVariant CVtkProjectedTetrahedraMapper::editorValue(int index, QWidget* widget)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
}

void CVtkProjectedTetrahedraMapper::connectEditor(int index, QWidget* widget, QObject* receiver, const char* member)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(receiver);
    Q_UNUSED(member);
}

QString CVtkProjectedTetrahedraMapper::propertyValueString(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant CVtkProjectedTetrahedraMapper::propertyValueFromString(int index, QString valueStr)
{
    Q_UNUSED(index);
    Q_UNUSED(valueStr);

    return QVariant();
}

bool CVtkProjectedTetrahedraMapper::canLoadSaveProperty(int index)
{
    Q_UNUSED(index);

    return false;
}

#endif

void CVtkProjectedTetrahedraMapper::setVtkObject(vtkObject* ptr)
{
    d->projectedTetrahedraMapper = dynamic_cast<vtkProjectedTetrahedraMapper*>(ptr);
    CVtkUnstructuredGridVolumeMapper::setVtkObject(ptr);
}





