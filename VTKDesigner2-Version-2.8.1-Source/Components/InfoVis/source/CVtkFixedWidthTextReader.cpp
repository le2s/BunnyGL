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

#include "CVtkFixedWidthTextReader.h"
#include "InfoVisComponent.h"
#include "vtkFixedWidthTextReader.h"

DEFINE_VIS_NODE(CVtkFixedWidthTextReader, CVtkTableAlgorithm)
{
    pDesc->setNodeClassCategory("InfoVis");
    pDesc->setNodeClassName("vtkFixedWidthTextReader");
    pDesc->setNodeClassDescription("vtkFixedWidthTextReader");
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

struct CVtkFixedWidthTextReaderData
{
    vtkFixedWidthTextReader *fixedWidthTextReader;
};

CVtkFixedWidthTextReader::CVtkFixedWidthTextReader()
{
    CVtkFixedWidthTextReader::InitializeNodeDesc();
    d = new CVtkFixedWidthTextReaderData;

    d->fixedWidthTextReader = 0;
    setVtkObject( vtkFixedWidthTextReader::New() );
}

CVtkFixedWidthTextReader::~CVtkFixedWidthTextReader()
{
    // This would be a good time to delete the backend object. If the backend object is a vtkObject
    // subclass, then you have to delete it now.
    // d->VtkObjectPointer->Delete();

    d->fixedWidthTextReader->Delete();
    delete d;
}

bool CVtkFixedWidthTextReader::hasInput(IVisSystemNodeConnectionPath* path)
{
    if(!path)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CVtkTableAlgorithm::hasInput(path);
}

bool CVtkFixedWidthTextReader::setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CVtkTableAlgorithm::setInput(path, inputData);
}

bool CVtkFixedWidthTextReader::removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CVtkTableAlgorithm::removeInput(path, inputData);
}

bool CVtkFixedWidthTextReader::fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData)
{
    if(!path || !outputData)
        return false;

    /*
    If you have added output paths in the description block at the header of this file,
    then you will have to handle outputs here
    */

    return CVtkTableAlgorithm::fetchOutput(path, outputData);
}

bool CVtkFixedWidthTextReader::outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData)
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

int CVtkFixedWidthTextReader::propertyCount()
{
    return 0;
}

QString CVtkFixedWidthTextReader::propertyName(int index)
{
    Q_UNUSED(index);
    return QString();
}

QString CVtkFixedWidthTextReader::propertyGroup(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant CVtkFixedWidthTextReader::propertyValue(int index)
{
    Q_UNUSED(index);
    return QVariant();
}

void CVtkFixedWidthTextReader::setPropertyValue(int index, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(value);
}

bool CVtkFixedWidthTextReader::hasEditor(int index)
{
    Q_UNUSED(index);
    return false;
}

QWidget* CVtkFixedWidthTextReader::createEditor(int index)
{
    Q_UNUSED(index);
    return 0;
}

void CVtkFixedWidthTextReader::setEditorValue(int index, QWidget* widget, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(value);
}

QVariant CVtkFixedWidthTextReader::editorValue(int index, QWidget* widget)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
}

void CVtkFixedWidthTextReader::connectEditor(int index, QWidget* widget, QObject* receiver, const char* member)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(receiver);
    Q_UNUSED(member);
}

QString CVtkFixedWidthTextReader::propertyValueString(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant CVtkFixedWidthTextReader::propertyValueFromString(int index, QString valueStr)
{
    Q_UNUSED(index);
    Q_UNUSED(valueStr);

    return QVariant();
}

bool CVtkFixedWidthTextReader::canLoadSaveProperty(int index)
{
    Q_UNUSED(index);

    return false;
}

#endif

void CVtkFixedWidthTextReader::setVtkObject(vtkObject* ptr)
{
    d->fixedWidthTextReader = dynamic_cast<vtkFixedWidthTextReader*>(ptr);
    nodeData()["vtkObject"] = qVariantFromValue<vtkObject*>(d->fixedWidthTextReader);
    CVtkTableAlgorithm::setVtkObject(ptr);
}





