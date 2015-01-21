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

#ifndef C_VTK_LOOKUP_TABLE_H
#define C_VTK_LOOKUP_TABLE_H

#include "CVtkScalarsToColors.h"
#include "vtkLookupTable.h"
#include "UserDefinedDataTypes.h"

#include "IEditableProperties.h"
#include "IPropertyEditorNotifications.h"

class CVtkLookupTable : public CVtkScalarsToColors, virtual public IEditableProperties,
                        virtual public IPropertyEditorNotifications
{
    Q_OBJECT
    Q_ENUMS(Ramp Scale)
    Q_INTERFACES(IEditableProperties IPropertyEditorNotifications)
    Q_PROPERTY(Ramp Ramp READ ramp WRITE setRamp)
    Q_PROPERTY(Scale Scale READ scale WRITE setScale)
    Q_PROPERTY(int NumberOfColors READ numberOfColors WRITE setNumberOfColors)
    Q_PROPERTY(ValueRange ScalarRange READ scalarRange WRITE setScalarRange)
    Q_PROPERTY(ValueRange HueRange READ hueRange WRITE setHueRange)
    Q_PROPERTY(ValueRange SaturationRange READ saturationRange WRITE setSaturationRange)
    Q_PROPERTY(ValueRange ValueRange READ valueRange WRITE setValueRange)
    Q_PROPERTY(ValueRange AlphaRange READ alphaRange WRITE setAlphaRange)
    DECLARE_VTK_OBJECT

public:
    enum Ramp
    {
        LinearRamp = VTK_RAMP_LINEAR,
        SCurveRamp = VTK_RAMP_SCURVE,
        SqrtRamp = VTK_RAMP_SQRT
    };

    enum Scale
    {
        LinearScale = VTK_SCALE_LINEAR,
        Log10Scale = VTK_SCALE_LOG10
    };

    CVtkLookupTable();
    ~CVtkLookupTable();

    void setRamp(Ramp ramp);
    Ramp ramp() const;

    void setScale(Scale scale);
    Scale scale() const;

    void setNumberOfColors(int count);
    int numberOfColors() const;

    void setScalarRange(ValueRange range);
    ValueRange scalarRange() const;

    void setHueRange(ValueRange range);
    ValueRange hueRange() const;

    void setSaturationRange(ValueRange range);
    ValueRange saturationRange() const;

    void setValueRange(ValueRange range);
    ValueRange valueRange() const;

    void setAlphaRange(ValueRange range);
    ValueRange alphaRange() const;

    Q_INVOKABLE void forceBuildLookupTable();
    Q_INVOKABLE void buildLookupTable();
    Q_INVOKABLE void setTableValue(int index, QColor color);
    Q_INVOKABLE QColor getTableValue(int index);
    Q_INVOKABLE double getOpacity(double val);

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
    QVariant propertyValueFromString(int index, QString strValue);

signals:
    void propertyConfigurationChanged();

public: // IPropertyEditorNotifications implementation
    void editorCreatedNotification(IProperty* property, QWidget* editor);
    void editorEditedNotification(IProperty* property, QWidget* editor);
    void editorAboutToDestroyNotification(IProperty* property, QWidget* editor);

    Q_INVOKABLE void loadBandedColorTable();
    Q_INVOKABLE void loadBlueGrayRedYellowColorTable();
    Q_INVOKABLE void loadBlueRedYellowColorTable();
    Q_INVOKABLE void loadGammaColorTable();
    Q_INVOKABLE void loadHot1ColorTable();
    Q_INVOKABLE void loadHot2ColorTable();
    Q_INVOKABLE void loadIceColorTable();
    Q_INVOKABLE void loadLightHuesColorTable();
    Q_INVOKABLE void loadRichColorTable();
    Q_INVOKABLE void loadSmoothRichColorTable();
    Q_INVOKABLE void loadLut16ColorTable();

protected slots:
    void command_ColorTablePresets();

protected:
    void loadColorTable(int type); // type = the #define enumeration below

private:
    vtkLookupTable* m_vtkLookupTable;
    QWidget* m_NumColorsEditor;
};

/*
Code contrubuted by Prof. Vijay Natarajan of the Indian Institute of Science, Bangalore

Vijay Natarajan [vijayn@csa.iisc.ernet.in]
*/

//constants for palette type
#define TYPE_BANDED         0
#define TYPE_BGRY           1
#define TYPE_BRY            2
#define TYPE_GAMMA          3
#define TYPE_HOT1           4
#define TYPE_HOT2           5
#define TYPE_ICE            6
#define TYPE_LIGHTHUES      7
#define TYPE_RICH           8
#define TYPE_SMOOTHRICH     9
#define TYPE_LUT16          10

class Palette
{
public:
    Palette();
    virtual ~Palette();

    int GetCurPalette();
    void GetPalette(int type, unsigned char *dest);

public:
    int m_curPalette;
};


#endif
