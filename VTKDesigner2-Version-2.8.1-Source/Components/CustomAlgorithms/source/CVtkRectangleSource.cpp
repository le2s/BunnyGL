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

#include "CVtkRectangleSource.h"
#include "CustomAlgorithmsComponent.h"
#include "vtkRectangleSource.h"

#include "CVtkPolyData.h"

DEFINE_VIS_NODE(CVtkRectangleSource, CGenericVisNodeBase)
{
    pDesc->setNodeClassCategory("CustomAlgorithms");
    pDesc->setNodeClassName("vtkRectangleSource");
    pDesc->setNodeClassDescription("Provides a rectangle polydata");
    pDesc->setNodeIcon( CustomAlgorithmsComponent::instance().nodeIcon() );

    pDesc->setPropertyInfo("Length", "SetLength", "GetLength", QStringList() << "double", QStringList(), "void", "double");
    pDesc->setPropertyInfo("Breadth", "SetBreadth", "GetBreadth", QStringList() << "double", QStringList(), "void", "double");
    
    // Uncomment and use the following code template to add input/output paths
    pDesc->addConnectionPath(
        new CGenericVisNodeConnectionPath(
                "Rectangle",                                // Name of the path
                IVisSystemNodeConnectionPath::OutputPath,   // Path type can be OutputPath or InputPath
                "vtkPolyData",                              // Data type of the path
                0,                                          // Path index (don't change)
                false                                       // Allow Multiple Inputs Flag
            )
        );
}

struct CVtkRectangleSourceData
{
    vtkRectangleSource *rectangleSource;
    CVtkPolyData outputData;
};

CVtkRectangleSource::CVtkRectangleSource()
{
    CVtkRectangleSource::InitializeNodeDesc();
    d = new CVtkRectangleSourceData;

    d->rectangleSource = 0;
    setVtkObject( vtkRectangleSource::New() );
}

CVtkRectangleSource::~CVtkRectangleSource()
{
    // This would be a good time to delete the backend object. If the backend object is a vtkObject
    // subclass, then you have to delete it now.
    // d->VtkObjectPointer->Delete();

    d->rectangleSource->Delete();
    delete d;
}

void CVtkRectangleSource::setLength(double l)
{
    d->rectangleSource->SetLength(l);
}

double CVtkRectangleSource::length() const
{
    return d->rectangleSource->GetLength();
}

void CVtkRectangleSource::setBreadth(double b)
{
    d->rectangleSource->SetBreadth(b);
}

double CVtkRectangleSource::breadth() const
{
    return d->rectangleSource->GetBreadth();
}

bool CVtkRectangleSource::hasInput(IVisSystemNodeConnectionPath* path)
{
    if(!path)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CGenericVisNodeBase::hasInput(path);
}

bool CVtkRectangleSource::setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CGenericVisNodeBase::setInput(path, inputData);
}

bool CVtkRectangleSource::removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CGenericVisNodeBase::removeInput(path, inputData);
}

bool CVtkRectangleSource::fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData)
{
    if(!path || !outputData)
        return false;

    if(path->pathName() == "Rectangle")
    {
        vtkPolyData* output = d->rectangleSource->GetOutput();
        if(d->outputData.getVtkPolyData() == 0)
            d->outputData.setPolyData(output);

        *outputData = &(d->outputData);
        return true;
    }

    return CGenericVisNodeBase::fetchOutput(path, outputData);
}

bool CVtkRectangleSource::outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData)
{
    if(!path || !outputData)
        return false;

    if(path->pathName() == "Rectangle")
        return true;

    return CGenericVisNodeBase::outputDerefed(path, outputData);
}

#ifdef ENABLE_ADVANCED_PROPERTIES

int CVtkRectangleSource::propertyCount()
{
    return 0;
}

QString CVtkRectangleSource::propertyName(int index)
{
    Q_UNUSED(index);
    return QString();
}

QString CVtkRectangleSource::propertyGroup(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant CVtkRectangleSource::propertyValue(int index)
{
    Q_UNUSED(index);
    return QVariant();
}

void CVtkRectangleSource::setPropertyValue(int index, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(value);
}

bool CVtkRectangleSource::hasEditor(int index)
{
    Q_UNUSED(index);
    return false;
}

QWidget* CVtkRectangleSource::createEditor(int index)
{
    Q_UNUSED(index);
    return 0;
}

void CVtkRectangleSource::setEditorValue(int index, QWidget* widget, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(value);
}

QVariant CVtkRectangleSource::editorValue(int index, QWidget* widget)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
}

void CVtkRectangleSource::connectEditor(int index, QWidget* widget, QObject* receiver, const char* member)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(receiver);
    Q_UNUSED(member);
}

QString CVtkRectangleSource::propertyValueString(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant CVtkRectangleSource::propertyValueFromString(int index, QString valueStr)
{
    Q_UNUSED(index);
    Q_UNUSED(valueStr);

    return QVariant();
}

bool CVtkRectangleSource::canLoadSaveProperty(int index)
{
    Q_UNUSED(index);

    return false;
}

#endif

void CVtkRectangleSource::setVtkObject(vtkObject* ptr)
{
    d->rectangleSource = dynamic_cast<vtkRectangleSource*>(ptr);
}





