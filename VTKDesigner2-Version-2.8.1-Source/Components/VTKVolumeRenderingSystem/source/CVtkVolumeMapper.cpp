/****************************************************************************
**
** Copyright (C) VCreate Logic Private Limited, Bangalore
**
** Use of this file is limited according to the terms specified by
** VCreate Logic Private Limited, Bangalore. Details of those terms
** are listed in licence.txt included as part of the distribution package
** of this file. This file may not be distributed without including the
** licence.txt file.
**
** Contact info@vcreatelogic.com if any conditions of this licensing are
** not clear to you.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include "CVtkVolumeMapper.h"
#include "VTKVolumeRenderingSystemComponent.h"

DEFINE_VIS_NODE(CVtkVolumeMapper, CVtkAbstractVolumeMapper)
{
    pDesc->setNodeClassCategory("VTKVolumeRenderingSystem");
    pDesc->setNodeClassName("vtkVolumeMapper");
    pDesc->setNodeClassDescription("vtkVolumeMapper");
    pDesc->setNodeIcon( VTKVolumeRenderingSystemComponent::instance().nodeIcon() );

    pDesc->setPropertyInfo("BlendMode", "SetBlendMode", "GetBlendMode", QStringList() << "int", QStringList(), "void", "int");
    pDesc->setPropertyInfo("Cropping", "SetCropping", "GetCropping", QStringList() << "bool", QStringList(), "void", "bool");
    
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

    pDesc->setCreateFunction(0);
}

struct CVtkVolumeMapperData
{
    vtkVolumeMapper *volumeMapper;
};

CVtkVolumeMapper::CVtkVolumeMapper()
{
    CVtkVolumeMapper::InitializeNodeDesc();
    d = new CVtkVolumeMapperData;

    d->volumeMapper = 0;
}

CVtkVolumeMapper::~CVtkVolumeMapper()
{
    // This would be a good time to delete the backend object. If the backend object is a vtkObject
    // subclass, then you have to delete it now.
    // d->VtkObjectPointer->Delete();
    delete d;
}

CVtkVolumeMapper::BlendMode CVtkVolumeMapper::blendMode() const
{
    switch(d->volumeMapper->GetBlendMode())
    {
        case vtkVolumeMapper::COMPOSITE_BLEND:return (Composite);
        case vtkVolumeMapper::MAXIMUM_INTENSITY_BLEND:return (MaximumIntensity);
        default :break;
    }
	return (Composite);
}

void CVtkVolumeMapper::setBlendMode(BlendMode val)
{
    switch(val)
    {
        case Composite:
            d->volumeMapper->SetBlendMode(vtkVolumeMapper::COMPOSITE_BLEND);
            break;
        case MaximumIntensity:
            d->volumeMapper->SetBlendMode(vtkVolumeMapper::MAXIMUM_INTENSITY_BLEND);
            break;
        default:
            d->volumeMapper->SetBlendMode(vtkVolumeMapper::COMPOSITE_BLEND);
            break;
     };
}

bool CVtkVolumeMapper::isCropping() const
{
    return d->volumeMapper->GetCropping();
}


void CVtkVolumeMapper::setCropping(bool val)
{
	d->volumeMapper->SetCropping(val);
}

void CVtkVolumeMapper::setCroppingRegionPlanes(QList<QVariant> planes)
{
	d->volumeMapper->SetCroppingRegionPlanes(planes[0].toDouble(),
											 planes[1].toDouble(),
											 planes[2].toDouble(),
											 planes[3].toDouble(),
											 planes[4].toDouble(),
											 planes[5].toDouble());

}

QList<QVariant> CVtkVolumeMapper::croppingRegionPlanes() const
{
	double* r = d->volumeMapper->GetCroppingRegionPlanes();

	QList<QVariant> ret = QList<QVariant>() << r[0] << r[1] << r[2] << r[3] << r[4] << r[5];
    return ret;
}

QList<QVariant> CVtkVolumeMapper::voxelCroppingRegionPlanes() const
{
	double* r = d->volumeMapper->GetVoxelCroppingRegionPlanes();

	QList<QVariant> ret = QList<QVariant>() << r[0] << r[1] << r[2] << r[3] << r[4] << r[5];
    return ret;
}

CVtkVolumeMapper::CroppingRegion CVtkVolumeMapper::croppingRegion() const
{
	switch(d->volumeMapper->GetCroppingRegionFlags())
    {
        case VTK_CROP_SUBVOLUME:return (SubVolume);
        case VTK_CROP_FENCE:return (Fence);
		case VTK_CROP_INVERTED_FENCE:return (InvertedFence);
		case VTK_CROP_CROSS:return (Cross);
		case VTK_CROP_INVERTED_CROSS:return (InvertedCross);
        default: break;
    }

	return (SubVolume);
}

void CVtkVolumeMapper::setCroppingRegion(CroppingRegion val)
{
    switch(val)
    {
        case SubVolume:
            d->volumeMapper->SetCroppingRegionFlags(VTK_CROP_SUBVOLUME);
            break;
        case Fence:
            d->volumeMapper->SetCroppingRegionFlags(VTK_CROP_FENCE);
            break;
		case InvertedFence:
            d->volumeMapper->SetCroppingRegionFlags(VTK_CROP_INVERTED_FENCE);
            break;
		case Cross:
            d->volumeMapper->SetCroppingRegionFlags(VTK_CROP_CROSS);
            break;
		case InvertedCross:
            d->volumeMapper->SetCroppingRegionFlags(VTK_CROP_INVERTED_CROSS);
            break;
        default:
            d->volumeMapper->SetCroppingRegionFlags(VTK_CROP_SUBVOLUME);
            break;
     };
}

bool CVtkVolumeMapper::hasInput(IVisSystemNodeConnectionPath* path)
{
    if(!path)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CVtkAbstractVolumeMapper::hasInput(path);
}

bool CVtkVolumeMapper::setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CVtkAbstractVolumeMapper::setInput(path, inputData);
}

bool CVtkVolumeMapper::removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CVtkAbstractVolumeMapper::removeInput(path, inputData);
}

bool CVtkVolumeMapper::fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData)
{
    if(!path || !outputData)
        return false;

    /*
    If you have added output paths in the description block at the header of this file,
    then you will have to handle outputs here
    */

    return CVtkAbstractVolumeMapper::fetchOutput(path, outputData);
}

bool CVtkVolumeMapper::outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData)
{
    if(!path || !outputData)
        return false;

    /*
    If you have added output paths in the description block at the header of this file,
    then you will have to handle outputs here
    */

    return CVtkAbstractVolumeMapper::outputDerefed(path, outputData);
}

#ifdef ENABLE_ADVANCED_PROPERTIES

int CVtkVolumeMapper::propertyCount()
{
    return 0;
}

QString CVtkVolumeMapper::propertyName(int index)
{
    Q_UNUSED(index);
    return QString();
}

QString CVtkVolumeMapper::propertyGroup(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant CVtkVolumeMapper::propertyValue(int index)
{
    Q_UNUSED(index);
    return QVariant();
}

void CVtkVolumeMapper::setPropertyValue(int index, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(value);
}

bool CVtkVolumeMapper::hasEditor(int index)
{
    Q_UNUSED(index);
    return false;
}

QWidget* CVtkVolumeMapper::createEditor(int index)
{
    Q_UNUSED(index);
    return 0;
}

void CVtkVolumeMapper::setEditorValue(int index, QWidget* widget, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(value);
}

QVariant CVtkVolumeMapper::editorValue(int index, QWidget* widget)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
}

void CVtkVolumeMapper::connectEditor(int index, QWidget* widget, QObject* receiver, const char* member)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(receiver);
    Q_UNUSED(member);
}

QString CVtkVolumeMapper::propertyValueString(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant CVtkVolumeMapper::propertyValueFromString(int index, QString valueStr)
{
    Q_UNUSED(index);
    Q_UNUSED(valueStr);

    return QVariant();
}

bool CVtkVolumeMapper::canLoadSaveProperty(int index)
{
    Q_UNUSED(index);

    return false;
}

#endif

void CVtkVolumeMapper::setVtkObject(vtkObject* ptr)
{
    d->volumeMapper = dynamic_cast<vtkVolumeMapper*>(ptr);
    CVtkAbstractVolumeMapper::setVtkObject(ptr);
}





