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

#include "CVtkTableAlgorithm.h"
#include "InfoVisComponent.h"
#include "vtkTableAlgorithm.h"
#include "CVtkTableData.h"

DEFINE_VIS_NODE(CVtkTableAlgorithm, CGenericVisNodeBase)
{
    pDesc->setNodeClassCategory("InfoVis");
    pDesc->setNodeClassName("vtkTableAlgorithm");
    pDesc->setNodeClassDescription("vtkTableAlgorithm");
    pDesc->setNodeIcon( InfoVisComponent::instance().nodeIcon() );

    // Uncomment and use the following code template to add input/output paths
    pDesc->addConnectionPath(
        new CGenericVisNodeConnectionPath(
                "TableOutput",                              // Name of the path
                IVisSystemNodeConnectionPath::OutputPath,   // Path type can be OutputPath or InputPath
                "vtkTable",                                 // Data type of the path
                0,                                          // Path index (don't change)
                false                                       // Allow Multiple Inputs Flag
            )
        );

    // Make sure that instances of this class cannot be created.
    pDesc->setCreateFunction(0);
}

struct CVtkTableAlgorithmData
{
    vtkTableAlgorithm *tableAlgorithm;
    CVtkTableData tableData;
};

CVtkTableAlgorithm::CVtkTableAlgorithm()
{
    CVtkTableAlgorithm::InitializeNodeDesc();
    d = new CVtkTableAlgorithmData;

    d->tableAlgorithm = 0;
    // setVtkObject( vtkTableAlgorithm::New() );
}

CVtkTableAlgorithm::~CVtkTableAlgorithm()
{
    // This would be a good time to delete the backend object. If the backend object is a vtkObject
    // subclass, then you have to delete it now.
    // d->VtkObjectPointer->Delete();

    // d->tableAlgorithm->Delete();
    delete d;
}

bool CVtkTableAlgorithm::hasInput(IVisSystemNodeConnectionPath* path)
{
    if(!path)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CGenericVisNodeBase::hasInput(path);
}

bool CVtkTableAlgorithm::setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CGenericVisNodeBase::setInput(path, inputData);
}

bool CVtkTableAlgorithm::removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CGenericVisNodeBase::removeInput(path, inputData);
}

bool CVtkTableAlgorithm::fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData)
{
    if(!path || !outputData)
        return false;

    if(path->pathName() == "TableOutput")
    {
        d->tableData.setTable( d->tableAlgorithm->GetOutput() );
        *outputData = &d->tableData;
        return true;
    }

    return CGenericVisNodeBase::fetchOutput(path, outputData);
}

bool CVtkTableAlgorithm::outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData)
{
    if(!path || !outputData)
        return false;

    if(path->pathName() == "TableOutput")
        return true;

    return CGenericVisNodeBase::outputDerefed(path, outputData);
}

#ifdef ENABLE_ADVANCED_PROPERTIES

int CVtkTableAlgorithm::propertyCount()
{
    return 0;
}

QString CVtkTableAlgorithm::propertyName(int index)
{
    Q_UNUSED(index);
    return QString();
}

QString CVtkTableAlgorithm::propertyGroup(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant CVtkTableAlgorithm::propertyValue(int index)
{
    Q_UNUSED(index);
    return QVariant();
}

void CVtkTableAlgorithm::setPropertyValue(int index, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(value);
}

bool CVtkTableAlgorithm::hasEditor(int index)
{
    Q_UNUSED(index);
    return false;
}

QWidget* CVtkTableAlgorithm::createEditor(int index)
{
    Q_UNUSED(index);
    return 0;
}

void CVtkTableAlgorithm::setEditorValue(int index, QWidget* widget, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(value);
}

QVariant CVtkTableAlgorithm::editorValue(int index, QWidget* widget)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
}

void CVtkTableAlgorithm::connectEditor(int index, QWidget* widget, QObject* receiver, const char* member)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(receiver);
    Q_UNUSED(member);
}

QString CVtkTableAlgorithm::propertyValueString(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant CVtkTableAlgorithm::propertyValueFromString(int index, QString valueStr)
{
    Q_UNUSED(index);
    Q_UNUSED(valueStr);

    return QVariant();
}

bool CVtkTableAlgorithm::canLoadSaveProperty(int index)
{
    Q_UNUSED(index);

    return false;
}

#endif

void CVtkTableAlgorithm::setVtkObject(vtkObject* ptr)
{
    d->tableAlgorithm = dynamic_cast<vtkTableAlgorithm*>(ptr);
    nodeData()["vtkObject"] = qVariantFromValue<vtkObject*>(d->tableAlgorithm);
}





