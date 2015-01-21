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

#include "CVtkDataObjectToTable.h"
#include "InfoVisComponent.h"
#include "vtkDataObjectToTable.h"

DEFINE_VIS_NODE(CVtkDataObjectToTable, CVtkTableAlgorithm)
{
    pDesc->setNodeClassCategory("InfoVis");
    pDesc->setNodeClassName("vtkDataObjectToTable");
    pDesc->setNodeClassDescription("vtkDataObjectToTable");
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

struct CVtkDataObjectToTableData
{
    vtkDataObjectToTable *dataObjectToTable;
};

CVtkDataObjectToTable::CVtkDataObjectToTable()
{
    CVtkDataObjectToTable::InitializeNodeDesc();
    d = new CVtkDataObjectToTableData;

    d->dataObjectToTable = 0;
    setVtkObject( vtkDataObjectToTable::New() );
}

CVtkDataObjectToTable::~CVtkDataObjectToTable()
{
    // This would be a good time to delete the backend object. If the backend object is a vtkObject
    // subclass, then you have to delete it now.
    // d->VtkObjectPointer->Delete();

    d->dataObjectToTable->Delete();
    delete d;
}

bool CVtkDataObjectToTable::hasInput(IVisSystemNodeConnectionPath* path)
{
    if(!path)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CVtkTableAlgorithm::hasInput(path);
}

bool CVtkDataObjectToTable::setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CVtkTableAlgorithm::setInput(path, inputData);
}

bool CVtkDataObjectToTable::removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CVtkTableAlgorithm::removeInput(path, inputData);
}

bool CVtkDataObjectToTable::fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData)
{
    if(!path || !outputData)
        return false;

    /*
    If you have added output paths in the description block at the header of this file,
    then you will have to handle outputs here
    */

    return CVtkTableAlgorithm::fetchOutput(path, outputData);
}

bool CVtkDataObjectToTable::outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData)
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

int CVtkDataObjectToTable::propertyCount()
{
    return 0;
}

QString CVtkDataObjectToTable::propertyName(int index)
{
    Q_UNUSED(index);
    return QString();
}

QString CVtkDataObjectToTable::propertyGroup(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant CVtkDataObjectToTable::propertyValue(int index)
{
    Q_UNUSED(index);
    return QVariant();
}

void CVtkDataObjectToTable::setPropertyValue(int index, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(value);
}

bool CVtkDataObjectToTable::hasEditor(int index)
{
    Q_UNUSED(index);
    return false;
}

QWidget* CVtkDataObjectToTable::createEditor(int index)
{
    Q_UNUSED(index);
    return 0;
}

void CVtkDataObjectToTable::setEditorValue(int index, QWidget* widget, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(value);
}

QVariant CVtkDataObjectToTable::editorValue(int index, QWidget* widget)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
}

void CVtkDataObjectToTable::connectEditor(int index, QWidget* widget, QObject* receiver, const char* member)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(receiver);
    Q_UNUSED(member);
}

QString CVtkDataObjectToTable::propertyValueString(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant CVtkDataObjectToTable::propertyValueFromString(int index, QString valueStr)
{
    Q_UNUSED(index);
    Q_UNUSED(valueStr);

    return QVariant();
}

bool CVtkDataObjectToTable::canLoadSaveProperty(int index)
{
    Q_UNUSED(index);

    return false;
}

#endif

void CVtkDataObjectToTable::setVtkObject(vtkObject* ptr)
{
    d->dataObjectToTable = dynamic_cast<vtkDataObjectToTable*>(ptr);
    nodeData()["vtkObject"] = qVariantFromValue<vtkObject*>(d->dataObjectToTable);
    CVtkTableAlgorithm::setVtkObject(ptr);
}





