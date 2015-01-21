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

#ifndef CVTKVOLUMEMAPPER_VIS_NODE_H
#define CVTKVOLUMEMAPPER_VIS_NODE_H

#include "UserDefinedDataTypes.h"
#include "CVtkAbstractVolumeMapper.h"
#include "vtkVolumeMapper.h"

#ifdef ENABLE_ADVANCED_PROPERTIES
#include "IEditableProperties.h"
#endif

struct CVtkVolumeMapperData;
class CVtkVolumeMapper : public CVtkAbstractVolumeMapper
#ifdef ENABLE_ADVANCED_PROPERTIES
                       , virtual public IEditableProperties
                       , virtual public IEditablePropertiesExt
#endif
{
    Q_OBJECT
#ifdef ENABLE_ADVANCED_PROPERTIES
    Q_INTERFACES(IEditableProperties IEditablePropertiesExt)
#endif

	Q_ENUMS(BlendMode)
	Q_PROPERTY(BlendMode BlendMode READ blendMode WRITE setBlendMode)
    Q_PROPERTY(bool Cropping READ isCropping WRITE setCropping)
    Q_PROPERTY(QList<QVariant> CroppingRegionPlanes READ croppingRegionPlanes WRITE setCroppingRegionPlanes)

    Q_ENUMS(CroppingRegion)
    Q_PROPERTY(CroppingRegion CroppingRegion READ croppingRegion WRITE setCroppingRegion)

    DECLARE_VIS_NODE

public:

	enum BlendMode
	{
		Composite = vtkVolumeMapper::COMPOSITE_BLEND,
		MaximumIntensity = vtkVolumeMapper::MAXIMUM_INTENSITY_BLEND
	};

	enum CroppingRegion
	{
		SubVolume = VTK_CROP_SUBVOLUME,
		Fence = VTK_CROP_FENCE,
		InvertedFence = VTK_CROP_INVERTED_FENCE,
		Cross = VTK_CROP_CROSS,
		InvertedCross = VTK_CROP_INVERTED_CROSS
	};

    CVtkVolumeMapper();
    ~CVtkVolumeMapper();

    BlendMode blendMode() const;
    void setBlendMode(BlendMode val);

    bool isCropping() const;
    void setCropping(bool val);

    void setCroppingRegionPlanes(QList<QVariant> planes);
    QList<QVariant> croppingRegionPlanes() const;

    Q_INVOKABLE QList<QVariant> voxelCroppingRegionPlanes() const;

    CroppingRegion croppingRegion() const;
    void setCroppingRegion(CroppingRegion val);

protected:
    bool hasInput(IVisSystemNodeConnectionPath* path);
    bool setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData);
    bool removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData);
    bool fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData);
    bool outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData);

#ifdef ENABLE_ADVANCED_PROPERTIES
public:
    // IEditableProperties implementation
    int propertyCount();
    QString propertyName(int index);
    QString propertyGroup(int index);
    QVariant propertyValue(int index);
    void setPropertyValue(int index, QVariant value);
    bool hasEditor(int index);
    QWidget* createEditor(int index);
    void setEditorValue(int index, QWidget* widget, QVariant value);
    QVariant editorValue(int index, QWidget* widget);
    void connectEditor(int index, QWidget* widget, QObject* receiver, const char* member);
    QString propertyValueString(int index);
    QVariant propertyValueFromString(int index, QString valueStr);

signals:
    void propertyConfigurationChanged();

public:
    // IEditablePropertiesExt implementation
    bool canLoadSaveProperty(int index);
#endif

protected:
    void setVtkObject(vtkObject* obj);

private:
    CVtkVolumeMapperData* d;
};

#endif

