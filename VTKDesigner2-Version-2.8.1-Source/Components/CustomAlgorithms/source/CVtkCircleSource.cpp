/****************************************************************************
**
** Copyright (C) Adithya
**
** Use of this file is limited according to the terms specified by
** Adithya.
**
** Details of those terms are listed in licence.txt included as
** part of the distribution package of this file. This file may not
** be distributed without including the licence.txt file.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include "CVtkCircleSource.h"
#include "CustomAlgorithmsComponent.h"
#include "vtkCircleSource.h"
#include "CVtkPolyData.h"

DEFINE_VIS_NODE(CVtkCircleSource, CGenericVisNodeBase)
{
    pDesc->setNodeClassCategory("CustomAlgorithms");
    pDesc->setNodeClassName("vtkCircleSource");
    pDesc->setNodeClassDescription("vtkCircleSource");
    pDesc->setNodeIcon( CustomAlgorithmsComponent::instance().nodeIcon() );

    pDesc->setPropertyInfo("Radius", "SetRadius", "GetRadius", QStringList() << "double", QStringList(), "void", "double");
    pDesc->setPropertyInfo("Resolution", "SetResolution", "GetResolution", QStringList() << "int", QStringList(), "void", "int");
    
    // Uncomment and use the following code template to add input/output paths
    pDesc->addConnectionPath(
        new CGenericVisNodeConnectionPath(
                "CircleOutput",                             // Name of the path
                IVisSystemNodeConnectionPath::OutputPath,   // Path type can be OutputPath or InputPath
                "vtkPolyData",                              // Data type of the path
                0,                                          // Path index (don't change)
                false                                       // Allow Multiple Inputs Flag
            )
        );
}

struct CVtkCircleSourceData
{
    vtkCircleSource *circleSource;
    CVtkPolyData output;
};

CVtkCircleSource::CVtkCircleSource()
{
    CVtkCircleSource::InitializeNodeDesc();
    d = new CVtkCircleSourceData;

    d->circleSource = 0;
    setVtkObject( vtkCircleSource::New() );
}

CVtkCircleSource::~CVtkCircleSource()
{
    // This would be a good time to delete the backend object. If the backend object is a vtkObject
    // subclass, then you have to delete it now.
    // d->VtkObjectPointer->Delete();

    d->circleSource->Delete();
    delete d;
}

void CVtkCircleSource::setRadius(double r)
{
    d->circleSource->SetRadius(r);
}

double CVtkCircleSource::radius() const
{
    return d->circleSource->GetRadius();
}

void CVtkCircleSource::setResolution(int r)
{
    d->circleSource->SetResolution(r);
}

int CVtkCircleSource::resolution() const
{
    return d->circleSource->GetResolution();
}

bool CVtkCircleSource::hasInput(IVisSystemNodeConnectionPath* path)
{
    if(!path)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CGenericVisNodeBase::hasInput(path);
}

bool CVtkCircleSource::setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CGenericVisNodeBase::setInput(path, inputData);
}

bool CVtkCircleSource::removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CGenericVisNodeBase::removeInput(path, inputData);
}

bool CVtkCircleSource::fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData)
{
    if(!path || !outputData)
        return false;

    if(path->pathName() == "CircleOutput")
    {
        d->output.setPolyData(d->circleSource->GetOutput());
        *outputData = &d->output;
        return true;
    }

    return CGenericVisNodeBase::fetchOutput(path, outputData);
}

bool CVtkCircleSource::outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData)
{
    if(!path || !outputData)
        return false;

    if(path->pathName() == "CircleOutput")
        return true;

    return CGenericVisNodeBase::outputDerefed(path, outputData);
}

#ifdef ENABLE_ADVANCED_PROPERTIES

int CVtkCircleSource::propertyCount()
{
    return 0;
}

QString CVtkCircleSource::propertyName(int index)
{
    Q_UNUSED(index);
    return QString();
}

QString CVtkCircleSource::propertyGroup(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant CVtkCircleSource::propertyValue(int index)
{
    Q_UNUSED(index);
    return QVariant();
}

void CVtkCircleSource::setPropertyValue(int index, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(value);
}

bool CVtkCircleSource::hasEditor(int index)
{
    Q_UNUSED(index);
    return false;
}

QWidget* CVtkCircleSource::createEditor(int index)
{
    Q_UNUSED(index);
    return 0;
}

void CVtkCircleSource::setEditorValue(int index, QWidget* widget, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(value);
}

QVariant CVtkCircleSource::editorValue(int index, QWidget* widget)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
}

void CVtkCircleSource::connectEditor(int index, QWidget* widget, QObject* receiver, const char* member)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(receiver);
    Q_UNUSED(member);
}

QString CVtkCircleSource::propertyValueString(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant CVtkCircleSource::propertyValueFromString(int index, QString valueStr)
{
    Q_UNUSED(index);
    Q_UNUSED(valueStr);

    return QVariant();
}

bool CVtkCircleSource::canLoadSaveProperty(int index)
{
    Q_UNUSED(index);

    return false;
}

#endif

void CVtkCircleSource::setVtkObject(vtkObject* ptr)
{
    d->circleSource = dynamic_cast<vtkCircleSource*>(ptr);
}





