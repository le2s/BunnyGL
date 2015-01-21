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

#ifndef CVTKVOLUMERAYCASTMAPPER_VIS_NODE_H
#define CVTKVOLUMERAYCASTMAPPER_VIS_NODE_H

#include "UserDefinedDataTypes.h"
#include "CVtkVolumeMapper.h"

#ifdef ENABLE_ADVANCED_PROPERTIES
#include "IEditableProperties.h"
#endif

struct CVtkVolumeRayCastMapperData;
class CVtkVolumeRayCastMapper : public CVtkVolumeMapper
#ifdef ENABLE_ADVANCED_PROPERTIES
                       , virtual public IEditableProperties
                       , virtual public IEditablePropertiesExt
#endif
{
    Q_OBJECT
#ifdef ENABLE_ADVANCED_PROPERTIES
    Q_INTERFACES(IEditableProperties IEditablePropertiesExt)
#endif

	Q_PROPERTY(double SampleDistance READ sampleDistance WRITE setSampleDistance)
	Q_PROPERTY(double ImageSampleDistance READ imageSampleDistance WRITE setImageSampleDistance)
	Q_PROPERTY(double MinimumImageSampleDistance READ minimumImageSampleDistance WRITE setMinimumImageSampleDistance)
	Q_PROPERTY(double MaximumImageSampleDistance READ maximumImageSampleDistance WRITE setMaximumImageSampleDistance)
	Q_PROPERTY(bool AutoAdjustSampleDistances READ isAutoAdjustSampleDistances WRITE setAutoAdjustSampleDistances)
	Q_PROPERTY(int NumberOfThreads READ numberOfThreads WRITE setNumberOfThreads)
	Q_PROPERTY(bool IntermixIntersectingGeometry READ isIntermixIntersectingGeometry WRITE setIntermixIntersectingGeometry)

    DECLARE_VIS_NODE

public:
    CVtkVolumeRayCastMapper();
    ~CVtkVolumeRayCastMapper();

    double sampleDistance() const;
    void setSampleDistance(double val);

    double imageSampleDistance() const;
    void setImageSampleDistance(double val);

    double minimumImageSampleDistance() const;
    void setMinimumImageSampleDistance(double val);

    double maximumImageSampleDistance() const;
    void setMaximumImageSampleDistance(double val);

    bool isAutoAdjustSampleDistances() const;
    void setAutoAdjustSampleDistances(bool val);

    int numberOfThreads() const;
    void setNumberOfThreads(int val);

    bool isIntermixIntersectingGeometry() const;
    void setIntermixIntersectingGeometry(bool val);

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
    CVtkVolumeRayCastMapperData* d;
};

#endif

