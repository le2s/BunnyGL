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

#include "CVtkCellCenterDepthSort.h"
#include "VTKVolumeRenderingSystemComponent.h"
#include "vtkCellCenterDepthSort.h"

DEFINE_VIS_NODE(CVtkCellCenterDepthSort, CVtkVisibilitySort)
{
    pDesc->setNodeClassCategory("VTKVolumeRenderingSystem");
    pDesc->setNodeClassName("vtkCellCenterDepthSort");
    pDesc->setNodeClassDescription("vtkCellCenterDepthSort");
    pDesc->setNodeIcon( VTKVolumeRenderingSystemComponent::instance().nodeIcon() );

    // Uncomment and use the following code template to add input/output paths
    /*
    pDesc->addConnectionPath(
        new CGenericVisNodeConnectionPath(
                "PathName",                                 // Name of the path
                IVisSystemNodeConnectionPath::OutputPath,   // Path type can be OutputPath or InputPath
                "PathType",                                 // Data type of the path
                0,                                          // Path index (don't change)
                false                                       // Allow Multiple Inputs Flag
            )
        );
    */
}

struct CVtkCellCenterDepthSortData
{
    vtkCellCenterDepthSort *cellCenterDepthSort;
};

CVtkCellCenterDepthSort::CVtkCellCenterDepthSort()
{
    CVtkCellCenterDepthSort::InitializeNodeDesc();
    d = new CVtkCellCenterDepthSortData;

    d->cellCenterDepthSort = 0;
    setVtkObject( vtkCellCenterDepthSort::New() );
}

CVtkCellCenterDepthSort::~CVtkCellCenterDepthSort()
{
    // This would be a good time to delete the backend object. If the backend object is a vtkObject
    // subclass, then you have to delete it now.
    // d->VtkObjectPointer->Delete();
    d->cellCenterDepthSort->Delete();
    delete d;
}

bool CVtkCellCenterDepthSort::hasInput(IVisSystemNodeConnectionPath* path)
{
    if(!path)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CVtkVisibilitySort::hasInput(path);
}

bool CVtkCellCenterDepthSort::setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CVtkVisibilitySort::setInput(path, inputData);
}

bool CVtkCellCenterDepthSort::removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CVtkVisibilitySort::removeInput(path, inputData);
}

bool CVtkCellCenterDepthSort::fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData)
{
    if(!path || !outputData)
        return false;

    /*
    If you have added output paths in the description block at the header of this file,
    then you will have to handle outputs here
    */

    return CVtkVisibilitySort::fetchOutput(path, outputData);
}

bool CVtkCellCenterDepthSort::outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData)
{
    if(!path || !outputData)
        return false;

    /*
    If you have added output paths in the description block at the header of this file,
    then you will have to handle outputs here
    */

    return CVtkVisibilitySort::outputDerefed(path, outputData);
}

#ifdef ENABLE_ADVANCED_PROPERTIES

int CVtkCellCenterDepthSort::propertyCount()
{
    return 0;
}

QString CVtkCellCenterDepthSort::propertyName(int index)
{
    Q_UNUSED(index);
    return QString();
}

QString CVtkCellCenterDepthSort::propertyGroup(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant CVtkCellCenterDepthSort::propertyValue(int index)
{
    Q_UNUSED(index);
    return QVariant();
}

void CVtkCellCenterDepthSort::setPropertyValue(int index, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(value);
}

bool CVtkCellCenterDepthSort::hasEditor(int index)
{
    Q_UNUSED(index);
    return false;
}

QWidget* CVtkCellCenterDepthSort::createEditor(int index)
{
    Q_UNUSED(index);
    return 0;
}

void CVtkCellCenterDepthSort::setEditorValue(int index, QWidget* widget, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(value);
}

QVariant CVtkCellCenterDepthSort::editorValue(int index, QWidget* widget)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
}

void CVtkCellCenterDepthSort::connectEditor(int index, QWidget* widget, QObject* receiver, const char* member)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(receiver);
    Q_UNUSED(member);
}

QString CVtkCellCenterDepthSort::propertyValueString(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant CVtkCellCenterDepthSort::propertyValueFromString(int index, QString valueStr)
{
    Q_UNUSED(index);
    Q_UNUSED(valueStr);

    return QVariant();
}

bool CVtkCellCenterDepthSort::canLoadSaveProperty(int index)
{
    Q_UNUSED(index);

    return false;
}

#endif

void CVtkCellCenterDepthSort::setVtkObject(vtkObject* ptr)
{
    d->cellCenterDepthSort = dynamic_cast<vtkCellCenterDepthSort*>(ptr);
    CVtkVisibilitySort::setVtkObject(ptr);
}





