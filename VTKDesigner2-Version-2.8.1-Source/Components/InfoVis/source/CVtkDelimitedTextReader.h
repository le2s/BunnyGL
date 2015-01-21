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

#ifndef CVTKDELIMITEDTEXTREADER_VIS_NODE_H
#define CVTKDELIMITEDTEXTREADER_VIS_NODE_H

#include "UserDefinedDataTypes.h"
#include "CVtkTableAlgorithm.h"

#ifdef ENABLE_ADVANCED_PROPERTIES
#include "IEditableProperties.h"
#endif

struct CVtkDelimitedTextReaderData;
class CVtkDelimitedTextReader : public CVtkTableAlgorithm
#ifdef ENABLE_ADVANCED_PROPERTIES
                       , virtual public IEditableProperties
                       , virtual public IEditablePropertiesExt
#endif
{
    Q_OBJECT
    Q_PROPERTY(QString FileName READ fileName WRITE setFileName)
    Q_PROPERTY(QString FieldDelimiter READ fieldDelimiterCharacters WRITE setFieldDelimiterCharacters)
    Q_PROPERTY(QString StringDelimiter READ stringDelimiter WRITE setStringDelimiter)
    Q_PROPERTY(bool UseStringDelimiter READ isUseStringDelimiter WRITE setUseStringDelimiter)
    Q_PROPERTY(bool HaveHeaders READ isHaveHeaders WRITE setHaveHeaders)
    Q_PROPERTY(bool MergeConsecutiveDelimiters READ isMergeConsecutiveDelimiters WRITE setMergeConsecutiveDelimiters)
    Q_PROPERTY(int MaxRecords READ maxRecords WRITE setMaxRecords)
    Q_PROPERTY(bool DetectNumericColumns READ isDetectNumericColumns WRITE setDetectNumericColumns)

#ifdef ENABLE_ADVANCED_PROPERTIES
    Q_INTERFACES(IEditableProperties IEditablePropertiesExt)
#endif
    DECLARE_VIS_NODE

public:
    CVtkDelimitedTextReader();
    ~CVtkDelimitedTextReader();

    void setFileName(const QString& fileName);
    QString fileName() const;

    void setFieldDelimiterCharacters(const QString& chars);
    QString fieldDelimiterCharacters() const;

    void setStringDelimiter(const QString& str);
    QString stringDelimiter() const;

    void setUseStringDelimiter(bool val);
    bool isUseStringDelimiter() const;

    void setHaveHeaders(bool val);
    bool isHaveHeaders() const;

    void setMergeConsecutiveDelimiters(bool val);
    bool isMergeConsecutiveDelimiters() const;

    void setMaxRecords(int val);
    int maxRecords() const;

    void setDetectNumericColumns(bool val);
    bool isDetectNumericColumns() const;

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
    CVtkDelimitedTextReaderData* d;
};

#endif

