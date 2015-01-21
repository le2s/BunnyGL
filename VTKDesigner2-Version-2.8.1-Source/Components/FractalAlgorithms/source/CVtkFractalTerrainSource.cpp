/****************************************************************************
**
** Copyright (C) VCreate Logic Pvt Ltd
**
** Use of this file is limited according to the terms specified by
** VCreate Logic Pvt Ltd.
**
** Details of those terms are listed in licence.txt included as
** part of the distribution package of this file. This file may not
** be distributed without including the licence.txt file.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include "CVtkFractalTerrainSource.h"
#include "FractalAlgorithmsComponent.h"
#include "vtkFractalTerrainSource.h"
#include "CVtkPolyData.h"

DEFINE_VIS_NODE(CVtkFractalTerrainSource, CGenericVisNodeBase)
{
    pDesc->setNodeClassCategory("FractalAlgorithms");
    pDesc->setNodeClassName("vtkFractalTerrainSource");
    pDesc->setNodeClassDescription("vtkFractalTerrainSource");
    pDesc->setNodeIcon( FractalAlgorithmsComponent::instance().nodeIcon() );

    pDesc->setPropertyInfo("NumberOfIterations", "SetNumberOfIterations", "GetNumberOfIterations", QStringList() << "int", QStringList(), "void", "int");
    // Uncomment and use the following code template to add input/output paths
    pDesc->addConnectionPath(
        new CGenericVisNodeConnectionPath(
                "FractalTerrain",                           // Name of the path
                IVisSystemNodeConnectionPath::OutputPath,   // Path type can be OutputPath or InputPath
                "vtkPolyData",                              // Data type of the path
                0,                                          // Path index (don't change)
                false                                       // Allow Multiple Inputs Flag
            )
        );
}

struct CVtkFractalTerrainSourceData
{
    vtkFractalTerrainSource *fractalTerrainSource;
    CVtkPolyData output;
};

CVtkFractalTerrainSource::CVtkFractalTerrainSource()
{
    CVtkFractalTerrainSource::InitializeNodeDesc();
    d = new CVtkFractalTerrainSourceData;

    d->fractalTerrainSource = 0;
    setVtkObject( vtkFractalTerrainSource::New() );
}

CVtkFractalTerrainSource::~CVtkFractalTerrainSource()
{
    // This would be a good time to delete the backend object. If the backend object is a vtkObject
    // subclass, then you have to delete it now.
    // d->VtkObjectPointer->Delete();

    d->fractalTerrainSource->Delete();
    delete d;
}

void CVtkFractalTerrainSource::setNumberOfIterations(int count)
{
    d->fractalTerrainSource->SetNumberOfIterations(count);
}

int CVtkFractalTerrainSource::numberOfIterations() const
{
    return d->fractalTerrainSource->GetNumberOfIterations();
}

void CVtkFractalTerrainSource::regenerate()
{
    d->fractalTerrainSource->Modified();
}

void CVtkFractalTerrainSource::command_Regenerate()
{
    this->regenerate();
}

bool CVtkFractalTerrainSource::hasInput(IVisSystemNodeConnectionPath* path)
{
    if(!path)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CGenericVisNodeBase::hasInput(path);
}

bool CVtkFractalTerrainSource::setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CGenericVisNodeBase::setInput(path, inputData);
}

bool CVtkFractalTerrainSource::removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CGenericVisNodeBase::removeInput(path, inputData);
}

bool CVtkFractalTerrainSource::fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData)
{
    if(!path || !outputData)
        return false;

    if(path->pathName() == "FractalTerrain")
    {
        d->output.setPolyData(d->fractalTerrainSource->GetOutput());
        *outputData = &d->output;
        return true;
    }

    return CGenericVisNodeBase::fetchOutput(path, outputData);
}

bool CVtkFractalTerrainSource::outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData)
{
    if(!path || !outputData)
        return false;

    if(path->pathName() == "FractalTerrain")
        return true;

    return CGenericVisNodeBase::outputDerefed(path, outputData);
}

#ifdef ENABLE_ADVANCED_PROPERTIES

int CVtkFractalTerrainSource::propertyCount()
{
    return 0;
}

QString CVtkFractalTerrainSource::propertyName(int index)
{
    Q_UNUSED(index);
    return QString();
}

QString CVtkFractalTerrainSource::propertyGroup(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant CVtkFractalTerrainSource::propertyValue(int index)
{
    Q_UNUSED(index);
    return QVariant();
}

void CVtkFractalTerrainSource::setPropertyValue(int index, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(value);
}

bool CVtkFractalTerrainSource::hasEditor(int index)
{
    Q_UNUSED(index);
    return false;
}

QWidget* CVtkFractalTerrainSource::createEditor(int index)
{
    Q_UNUSED(index);
    return 0;
}

void CVtkFractalTerrainSource::setEditorValue(int index, QWidget* widget, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(value);
}

QVariant CVtkFractalTerrainSource::editorValue(int index, QWidget* widget)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
}

void CVtkFractalTerrainSource::connectEditor(int index, QWidget* widget, QObject* receiver, const char* member)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(receiver);
    Q_UNUSED(member);
}

QString CVtkFractalTerrainSource::propertyValueString(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant CVtkFractalTerrainSource::propertyValueFromString(int index, QString valueStr)
{
    Q_UNUSED(index);
    Q_UNUSED(valueStr);

    return QVariant();
}

bool CVtkFractalTerrainSource::canLoadSaveProperty(int index)
{
    Q_UNUSED(index);

    return false;
}

#endif

void CVtkFractalTerrainSource::setVtkObject(vtkObject* ptr)
{
    d->fractalTerrainSource = dynamic_cast<vtkFractalTerrainSource*>(ptr);
    nodeData()["vtkObject"] = qVariantFromValue<vtkObject*>(d->fractalTerrainSource);
}





