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

#include "CVtkMengerSquareSource.h"
#include "FractalAlgorithmsComponent.h"
#include "vtkMengerSquareSource.h"
#include "CVtkPolyData.h"

DEFINE_VIS_NODE(CVtkMengerSquareSource, CGenericVisNodeBase)
{
    pDesc->setNodeClassCategory("FractalAlgorithms");
    pDesc->setNodeClassName("vtkMengerSquareSource");
    pDesc->setNodeClassDescription("vtkMengerSquareSource");
    pDesc->setNodeIcon( FractalAlgorithmsComponent::instance().nodeIcon() );
    pDesc->setPropertyInfo("NumberOfIterations", "setNumberOfIterations", "numberOfIterations", QStringList() << "int", QStringList(), "void", "int");


    // Uncomment and use the following code template to add input/output paths
    pDesc->addConnectionPath(
        new CGenericVisNodeConnectionPath(
                "MengerSquares",                            // Name of the path
                IVisSystemNodeConnectionPath::OutputPath,   // Path type can be OutputPath or InputPath
                "vtkPolyData",                              // Data type of the path
                0,                                          // Path index (don't change)
                false                                       // Allow Multiple Inputs Flag
            )
        );
}

struct CVtkMengerSquareSourceData
{
    vtkMengerSquareSource *mengerSquareSource;
    CVtkPolyData output;
};

CVtkMengerSquareSource::CVtkMengerSquareSource()
{
    CVtkMengerSquareSource::InitializeNodeDesc();
    d = new CVtkMengerSquareSourceData;

    d->mengerSquareSource = 0;
    setVtkObject( vtkMengerSquareSource::New() );
}

CVtkMengerSquareSource::~CVtkMengerSquareSource()
{
    // This would be a good time to delete the backend object. If the backend object is a vtkObject
    // subclass, then you have to delete it now.
    // d->VtkObjectPointer->Delete();

    d->mengerSquareSource->Delete();
    delete d;
}

void CVtkMengerSquareSource::setNumberOfIterations(int count)
{
    d->mengerSquareSource->SetNumberOfIterations(count);
}

int CVtkMengerSquareSource::numberOfIterations() const
{
    return d->mengerSquareSource->GetNumberOfIterations();
}

bool CVtkMengerSquareSource::hasInput(IVisSystemNodeConnectionPath* path)
{
    if(!path)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CGenericVisNodeBase::hasInput(path);
}

bool CVtkMengerSquareSource::setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CGenericVisNodeBase::setInput(path, inputData);
}

bool CVtkMengerSquareSource::removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CGenericVisNodeBase::removeInput(path, inputData);
}

bool CVtkMengerSquareSource::fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData)
{
    if(!path || !outputData)
        return false;

    if(path->pathName() == "MengerSquares")
    {
        d->output.setPolyData( d->mengerSquareSource->GetOutput() );
        *outputData = &d->output;
        return true;
    }

    return CGenericVisNodeBase::fetchOutput(path, outputData);
}

bool CVtkMengerSquareSource::outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData)
{
    if(!path || !outputData)
        return false;

    if(path->pathName() == "MengerSquares")
        return true;

    return CGenericVisNodeBase::outputDerefed(path, outputData);
}

#ifdef ENABLE_ADVANCED_PROPERTIES

int CVtkMengerSquareSource::propertyCount()
{
    return 0;
}

QString CVtkMengerSquareSource::propertyName(int index)
{
    Q_UNUSED(index);
    return QString();
}

QString CVtkMengerSquareSource::propertyGroup(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant CVtkMengerSquareSource::propertyValue(int index)
{
    Q_UNUSED(index);
    return QVariant();
}

void CVtkMengerSquareSource::setPropertyValue(int index, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(value);
}

bool CVtkMengerSquareSource::hasEditor(int index)
{
    Q_UNUSED(index);
    return false;
}

QWidget* CVtkMengerSquareSource::createEditor(int index)
{
    Q_UNUSED(index);
    return 0;
}

void CVtkMengerSquareSource::setEditorValue(int index, QWidget* widget, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(value);
}

QVariant CVtkMengerSquareSource::editorValue(int index, QWidget* widget)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
}

void CVtkMengerSquareSource::connectEditor(int index, QWidget* widget, QObject* receiver, const char* member)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(receiver);
    Q_UNUSED(member);
}

QString CVtkMengerSquareSource::propertyValueString(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant CVtkMengerSquareSource::propertyValueFromString(int index, QString valueStr)
{
    Q_UNUSED(index);
    Q_UNUSED(valueStr);

    return QVariant();
}

bool CVtkMengerSquareSource::canLoadSaveProperty(int index)
{
    Q_UNUSED(index);

    return false;
}

#endif

void CVtkMengerSquareSource::setVtkObject(vtkObject* ptr)
{
    d->mengerSquareSource = dynamic_cast<vtkMengerSquareSource*>(ptr);
    nodeData()["vtkObject"] = qVariantFromValue<vtkObject*>(d->mengerSquareSource);
}





