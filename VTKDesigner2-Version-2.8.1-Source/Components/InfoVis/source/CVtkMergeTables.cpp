/****************************************************************************
**
** Copyright (C) UNO and VCreate Logic
**
** Use of this file is limited according to the terms specified by
** UNO and VCreate Logic.
**
** Details of those terms are listed in licence.txt included as
** part of the distribution package of this file. This file may not
** be distributed without including the licence.txt file.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include "CVtkMergeTables.h"
#include "InfoVisComponent.h"
#include "vtkMergeTables.h"

DEFINE_VIS_NODE(CVtkMergeTables, CVtkTableAlgorithm)
{
    pDesc->setNodeClassCategory("InfoVis");
    pDesc->setNodeClassName("vtkMergeTables");
    pDesc->setNodeClassDescription("vtkMergeTables");
    pDesc->setNodeIcon( InfoVisComponent::instance().nodeIcon() );

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

struct CVtkMergeTablesData
{
    vtkMergeTables *mergeTables;
};

CVtkMergeTables::CVtkMergeTables()
{
    CVtkMergeTables::InitializeNodeDesc();
    d = new CVtkMergeTablesData;

    d->mergeTables = 0;
    setVtkObject( vtkMergeTables::New() );
}

CVtkMergeTables::~CVtkMergeTables()
{
    // This would be a good time to delete the backend object. If the backend object is a vtkObject
    // subclass, then you have to delete it now.
    // d->VtkObjectPointer->Delete();

    d->mergeTables->Delete();
    delete d;
}

bool CVtkMergeTables::hasInput(IVisSystemNodeConnectionPath* path)
{
    if(!path)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CVtkTableAlgorithm::hasInput(path);
}

bool CVtkMergeTables::setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CVtkTableAlgorithm::setInput(path, inputData);
}

bool CVtkMergeTables::removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CVtkTableAlgorithm::removeInput(path, inputData);
}

bool CVtkMergeTables::fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData)
{
    if(!path || !outputData)
        return false;

    /*
    If you have added output paths in the description block at the header of this file,
    then you will have to handle outputs here
    */

    return CVtkTableAlgorithm::fetchOutput(path, outputData);
}

bool CVtkMergeTables::outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData)
{
    if(!path || !outputData)
        return false;

    /*
    If you have added output paths in the description block at the header of this file,
    then you will have to handle outputs here
    */

    return CVtkTableAlgorithm::outputDerefed(path, outputData);
}

#ifdef ENABLE_ADVANCED_PROPERTIES

int CVtkMergeTables::propertyCount()
{
    return 0;
}

QString CVtkMergeTables::propertyName(int index)
{
    Q_UNUSED(index);
    return QString();
}

QString CVtkMergeTables::propertyGroup(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant CVtkMergeTables::propertyValue(int index)
{
    Q_UNUSED(index);
    return QVariant();
}

void CVtkMergeTables::setPropertyValue(int index, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(value);
}

bool CVtkMergeTables::hasEditor(int index)
{
    Q_UNUSED(index);
    return false;
}

QWidget* CVtkMergeTables::createEditor(int index)
{
    Q_UNUSED(index);
    return 0;
}

void CVtkMergeTables::setEditorValue(int index, QWidget* widget, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(value);
}

QVariant CVtkMergeTables::editorValue(int index, QWidget* widget)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
}

void CVtkMergeTables::connectEditor(int index, QWidget* widget, QObject* receiver, const char* member)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(receiver);
    Q_UNUSED(member);
}

QString CVtkMergeTables::propertyValueString(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant CVtkMergeTables::propertyValueFromString(int index, QString valueStr)
{
    Q_UNUSED(index);
    Q_UNUSED(valueStr);

    return QVariant();
}

bool CVtkMergeTables::canLoadSaveProperty(int index)
{
    Q_UNUSED(index);

    return false;
}

#endif

void CVtkMergeTables::setVtkObject(vtkObject* ptr)
{
    d->mergeTables = dynamic_cast<vtkMergeTables*>(ptr);
    nodeData()["vtkObject"] = qVariantFromValue<vtkObject*>(d->mergeTables);
    CVtkTableAlgorithm::setVtkObject(ptr);
}





