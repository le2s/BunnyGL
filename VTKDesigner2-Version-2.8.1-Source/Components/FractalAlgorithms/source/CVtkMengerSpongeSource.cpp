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

#include "CVtkMengerSpongeSource.h"
#include "FractalAlgorithmsComponent.h"
#include "vtkMengerSpongeSource.h"
#include "CVtkPolyData.h"

DEFINE_VIS_NODE(CVtkMengerSpongeSource, CGenericVisNodeBase)
{
    pDesc->setNodeClassCategory("FractalAlgorithms");
    pDesc->setNodeClassName("vtkMengerSpongeSource");
    pDesc->setNodeClassDescription("vtkMengerSpongeSource");
    pDesc->setNodeIcon( FractalAlgorithmsComponent::instance().nodeIcon() );
    pDesc->setPropertyInfo("NumberOfIterations", "setNumberOfIterations", "numberOfIterations", QStringList() << "int", QStringList(), "void", "int");

    // Uncomment and use the following code template to add input/output paths
    pDesc->addConnectionPath(
        new CGenericVisNodeConnectionPath(
                "MengerSponge",                             // Name of the path
                IVisSystemNodeConnectionPath::OutputPath,   // Path type can be OutputPath or InputPath
                "vtkPolyData",                              // Data type of the path
                0,                                          // Path index (don't change)
                false                                       // Allow Multiple Inputs Flag
            )
        );
}

struct CVtkMengerSpongeSourceData
{
    vtkMengerSpongeSource *mengerSpongeSource;
    CVtkPolyData output;
};

CVtkMengerSpongeSource::CVtkMengerSpongeSource()
{
    CVtkMengerSpongeSource::InitializeNodeDesc();
    d = new CVtkMengerSpongeSourceData;

    d->mengerSpongeSource = 0;
    setVtkObject( vtkMengerSpongeSource::New() );
}

CVtkMengerSpongeSource::~CVtkMengerSpongeSource()
{
    // This would be a good time to delete the backend object. If the backend object is a vtkObject
    // subclass, then you have to delete it now.
    // d->VtkObjectPointer->Delete();

    d->mengerSpongeSource->Delete();
    delete d;
}

void CVtkMengerSpongeSource::setNumberOfIterations(int count)
{
    d->mengerSpongeSource->SetNumberOfIterations(count);
}

int CVtkMengerSpongeSource::numberOfIterations() const
{
    return d->mengerSpongeSource->GetNumberOfIterations();
}

bool CVtkMengerSpongeSource::hasInput(IVisSystemNodeConnectionPath* path)
{
    if(!path)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CGenericVisNodeBase::hasInput(path);
}

bool CVtkMengerSpongeSource::setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CGenericVisNodeBase::setInput(path, inputData);
}

bool CVtkMengerSpongeSource::removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CGenericVisNodeBase::removeInput(path, inputData);
}

bool CVtkMengerSpongeSource::fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData)
{
    if(!path || !outputData)
        return false;

    if(path->pathName() == "MengerSponge")
    {
        d->output.setPolyData(d->mengerSpongeSource->GetOutput());
        *outputData = &d->output;
        return true;
    }

    return CGenericVisNodeBase::fetchOutput(path, outputData);
}

bool CVtkMengerSpongeSource::outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData)
{
    if(!path || !outputData)
        return false;

    if(path->pathName() == "MengerSponge")
        return true;

    return CGenericVisNodeBase::outputDerefed(path, outputData);
}

#ifdef ENABLE_ADVANCED_PROPERTIES

int CVtkMengerSpongeSource::propertyCount()
{
    return 0;
}

QString CVtkMengerSpongeSource::propertyName(int index)
{
    Q_UNUSED(index);
    return QString();
}

QString CVtkMengerSpongeSource::propertyGroup(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant CVtkMengerSpongeSource::propertyValue(int index)
{
    Q_UNUSED(index);
    return QVariant();
}

void CVtkMengerSpongeSource::setPropertyValue(int index, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(value);
}

bool CVtkMengerSpongeSource::hasEditor(int index)
{
    Q_UNUSED(index);
    return false;
}

QWidget* CVtkMengerSpongeSource::createEditor(int index)
{
    Q_UNUSED(index);
    return 0;
}

void CVtkMengerSpongeSource::setEditorValue(int index, QWidget* widget, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(value);
}

QVariant CVtkMengerSpongeSource::editorValue(int index, QWidget* widget)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
}

void CVtkMengerSpongeSource::connectEditor(int index, QWidget* widget, QObject* receiver, const char* member)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(receiver);
    Q_UNUSED(member);
}

QString CVtkMengerSpongeSource::propertyValueString(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant CVtkMengerSpongeSource::propertyValueFromString(int index, QString valueStr)
{
    Q_UNUSED(index);
    Q_UNUSED(valueStr);

    return QVariant();
}

bool CVtkMengerSpongeSource::canLoadSaveProperty(int index)
{
    Q_UNUSED(index);

    return false;
}

#endif

void CVtkMengerSpongeSource::setVtkObject(vtkObject* ptr)
{
    d->mengerSpongeSource = dynamic_cast<vtkMengerSpongeSource*>(ptr);
    nodeData()["vtkObject"] = qVariantFromValue<vtkObject*>(d->mengerSpongeSource);
}





