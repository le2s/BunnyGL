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

#include "CVtkSierpinskiPyramidSource.h"
#include "FractalAlgorithmsComponent.h"
#include "vtkSierpinskiPyramidSource.h"
#include "CVtkPolyData.h"

DEFINE_VIS_NODE(CVtkSierpinskiPyramidSource, CGenericVisNodeBase)
{
    pDesc->setNodeClassCategory("FractalAlgorithms");
    pDesc->setNodeClassName("vtkSierpinskiPyramidSource");
    pDesc->setNodeClassDescription("vtkSierpinskiPyramidSource");
    pDesc->setNodeIcon( FractalAlgorithmsComponent::instance().nodeIcon() );
    pDesc->setPropertyInfo("NumberOfIterations", "setNumberOfIterations", "numberOfIterations", QStringList() << "int", QStringList(), "void", "int");

    // Uncomment and use the following code template to add input/output paths
    pDesc->addConnectionPath(
        new CGenericVisNodeConnectionPath(
                "SierpinskiPyramid",                        // Name of the path
                IVisSystemNodeConnectionPath::OutputPath,   // Path type can be OutputPath or InputPath
                "vtkPolyData",                              // Data type of the path
                0,                                          // Path index (don't change)
                false                                       // Allow Multiple Inputs Flag
            )
        );
}

struct CVtkSierpinskiPyramidSourceData
{
    vtkSierpinskiPyramidSource *sierpinskiPyramidSource;
    CVtkPolyData output;
};

CVtkSierpinskiPyramidSource::CVtkSierpinskiPyramidSource()
{
    CVtkSierpinskiPyramidSource::InitializeNodeDesc();
    d = new CVtkSierpinskiPyramidSourceData;

    d->sierpinskiPyramidSource = 0;
    setVtkObject( vtkSierpinskiPyramidSource::New() );
}

CVtkSierpinskiPyramidSource::~CVtkSierpinskiPyramidSource()
{
    // This would be a good time to delete the backend object. If the backend object is a vtkObject
    // subclass, then you have to delete it now.
    // d->VtkObjectPointer->Delete();

    d->sierpinskiPyramidSource->Delete();
    delete d;
}

void CVtkSierpinskiPyramidSource::setNumberOfIterations(int count)
{
    d->sierpinskiPyramidSource->SetNumberOfIterations(count);
}

int CVtkSierpinskiPyramidSource::numberOfIterations() const
{
    return d->sierpinskiPyramidSource->GetNumberOfIterations();
}

bool CVtkSierpinskiPyramidSource::hasInput(IVisSystemNodeConnectionPath* path)
{
    if(!path)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CGenericVisNodeBase::hasInput(path);
}

bool CVtkSierpinskiPyramidSource::setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CGenericVisNodeBase::setInput(path, inputData);
}

bool CVtkSierpinskiPyramidSource::removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CGenericVisNodeBase::removeInput(path, inputData);
}

bool CVtkSierpinskiPyramidSource::fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData)
{
    if(!path || !outputData)
        return false;

    if(path->pathName() == "SierpinskiPyramid")
    {
        d->output.setPolyData( d->sierpinskiPyramidSource->GetOutput() );
        *outputData = &d->output;
        return true;
    }

    return CGenericVisNodeBase::fetchOutput(path, outputData);
}

bool CVtkSierpinskiPyramidSource::outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData)
{
    if(!path || !outputData)
        return false;

    if(path->pathName() == "SierpinskiPyramid")
        return true;

    return CGenericVisNodeBase::outputDerefed(path, outputData);
}

#ifdef ENABLE_ADVANCED_PROPERTIES

int CVtkSierpinskiPyramidSource::propertyCount()
{
    return 0;
}

QString CVtkSierpinskiPyramidSource::propertyName(int index)
{
    Q_UNUSED(index);
    return QString();
}

QString CVtkSierpinskiPyramidSource::propertyGroup(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant CVtkSierpinskiPyramidSource::propertyValue(int index)
{
    Q_UNUSED(index);
    return QVariant();
}

void CVtkSierpinskiPyramidSource::setPropertyValue(int index, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(value);
}

bool CVtkSierpinskiPyramidSource::hasEditor(int index)
{
    Q_UNUSED(index);
    return false;
}

QWidget* CVtkSierpinskiPyramidSource::createEditor(int index)
{
    Q_UNUSED(index);
    return 0;
}

void CVtkSierpinskiPyramidSource::setEditorValue(int index, QWidget* widget, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(value);
}

QVariant CVtkSierpinskiPyramidSource::editorValue(int index, QWidget* widget)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
}

void CVtkSierpinskiPyramidSource::connectEditor(int index, QWidget* widget, QObject* receiver, const char* member)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(receiver);
    Q_UNUSED(member);
}

QString CVtkSierpinskiPyramidSource::propertyValueString(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant CVtkSierpinskiPyramidSource::propertyValueFromString(int index, QString valueStr)
{
    Q_UNUSED(index);
    Q_UNUSED(valueStr);

    return QVariant();
}

bool CVtkSierpinskiPyramidSource::canLoadSaveProperty(int index)
{
    Q_UNUSED(index);

    return false;
}

#endif

void CVtkSierpinskiPyramidSource::setVtkObject(vtkObject* ptr)
{
    d->sierpinskiPyramidSource = dynamic_cast<vtkSierpinskiPyramidSource*>(ptr);
    nodeData()["vtkObject"] = qVariantFromValue<vtkObject*>(d->sierpinskiPyramidSource);
}





