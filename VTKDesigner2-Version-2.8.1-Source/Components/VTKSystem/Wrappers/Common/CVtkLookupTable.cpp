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

#include "CVtkLookupTable.h"

#include <QDialog>
#include <QVBoxLayout>
#include <QLabel>
#include <QMainWindow>
#include <QRadioButton>
#include <QDialogButtonBox>

#include <IProperty>
#include <GCF/AbstractComponent>

DEFINE_VTK_OBJECT(CVtkLookupTable, CVtkScalarsToColors, vtkLookupTable)
{
    pDesc->setNodeClassName("vtkLookupTable");
    pDesc->setNodeClassCategory("Algorithm");
    
    pDesc->setPropertyInfo("Ramp", "SetRamp" , "GetRamp", QStringList () << "int" , QStringList() , "void" ,"int" );
    pDesc->setPropertyInfo("Scale", "SetScale" , "GetScale", QStringList () << "int" , QStringList() , "void" ,"int" );
    pDesc->setPropertyInfo("NumberOfColors", "SetNumberOfColors" , "GetNumberOfColors", QStringList () << "int" , QStringList() , "void" ,"int" );
    pDesc->setPropertyInfo("ScalarRange", "SetScalarRange" , "scalarRange", QStringList () << "double" << "double" , QStringList() , "void" ,"ValueRange" );
    pDesc->setPropertyInfo("SaturationRange", "SetSaturationRange" , "saturationRange", QStringList () << "double" << "double" , QStringList() , "void" ,"ValueRange" );
    pDesc->setPropertyInfo("ValueRange", "SetValueRange" , "valueRange", QStringList () << "double" << "double" , QStringList() , "void" ,"ValueRange" );
    pDesc->setPropertyInfo("AlphaRange", "SetAlphaRange" , "alphaRange", QStringList () << "double" << "double" , QStringList() , "void" ,"ValueRange" );
    pDesc->setPropertyInfo("HueRange", "SetHueRange" , "hueRange", QStringList () << "double" << "double" , QStringList() , "void" ,"ValueRange" );
}

CVtkLookupTable::CVtkLookupTable() : m_vtkLookupTable(0)
{
    CVtkLookupTable::InitializeObjectDesc();
    setVtkObject(vtkLookupTable::New());
    m_vtkLookupTable->Build();
    m_NumColorsEditor = 0;

    loadColorTable(TYPE_BRY);
}

CVtkLookupTable::~CVtkLookupTable()
{

}

void CVtkLookupTable::setRamp(CVtkLookupTable::Ramp ramp)
{
    m_vtkLookupTable->SetRamp( int(ramp) );
}

CVtkLookupTable::Ramp CVtkLookupTable::ramp() const
{
    return CVtkLookupTable::Ramp( m_vtkLookupTable->GetRamp() );
}

void CVtkLookupTable::setScale(CVtkLookupTable::Scale scale)
{
    m_vtkLookupTable->SetScale( int(scale) );
}

CVtkLookupTable::Scale CVtkLookupTable::scale() const
{
    return CVtkLookupTable::Scale(m_vtkLookupTable->GetScale());
}

void CVtkLookupTable::setNumberOfColors(int count)
{
    m_vtkLookupTable->SetNumberOfColors(count);
    m_vtkLookupTable->Build();

    if(!m_NumColorsEditor)
        QMetaObject::invokeMethod(this, "propertyConfigurationChanged", Qt::QueuedConnection);
}

int CVtkLookupTable::numberOfColors() const
{
    return m_vtkLookupTable->GetNumberOfColors();
}

void CVtkLookupTable::setScalarRange(ValueRange range)
{
    m_vtkLookupTable->SetTableRange(range.min, range.max);
    m_vtkLookupTable->Build();
}

ValueRange CVtkLookupTable::scalarRange() const
{
    return ValueRange( m_vtkLookupTable->GetTableRange() );
}

void CVtkLookupTable::setHueRange(ValueRange range)
{
    m_vtkLookupTable->SetHueRange( range.min, range.max );
    m_vtkLookupTable->Build();
}

ValueRange CVtkLookupTable::hueRange() const
{
    return ValueRange( m_vtkLookupTable->GetHueRange() );
}

void CVtkLookupTable::setSaturationRange(ValueRange range)
{
    m_vtkLookupTable->SetSaturationRange( range.min, range.max );
    m_vtkLookupTable->Build();
}

ValueRange CVtkLookupTable::saturationRange() const
{
    return ValueRange( m_vtkLookupTable->GetSaturationRange() );
}

void CVtkLookupTable::setValueRange(ValueRange range)
{
    m_vtkLookupTable->SetValueRange( range.min, range.max );
    m_vtkLookupTable->Build();
}

ValueRange CVtkLookupTable::valueRange() const
{
    return ValueRange( m_vtkLookupTable->GetValueRange() );
}

void CVtkLookupTable::setAlphaRange(ValueRange range)
{
    m_vtkLookupTable->SetAlphaRange( range.min, range.max );
    m_vtkLookupTable->Build();
}

ValueRange CVtkLookupTable::alphaRange() const
{
    return ValueRange( m_vtkLookupTable->GetAlphaRange() );
}

void CVtkLookupTable::forceBuildLookupTable()
{
    m_vtkLookupTable->ForceBuild();
}

void CVtkLookupTable::buildLookupTable()
{
    m_vtkLookupTable->Build();
}

void CVtkLookupTable::setTableValue(int index, QColor color)
{
    if(index >= numberOfColors())
        return;
    m_vtkLookupTable->SetTableValue(index, color.redF(), color.greenF(), color.blueF(), color.alphaF());
}

QColor CVtkLookupTable::getTableValue(int index)
{
    QColor ret;

    if(index >= numberOfColors())
        return ret;

    double* c = m_vtkLookupTable->GetTableValue(index);
    if(!c)
        return ret;

    ret.setRgbF(c[0], c[1], c[2], c[3]);
    return ret;
}

double CVtkLookupTable::getOpacity(double val)
{
    return m_vtkLookupTable->GetOpacity(val);
}

int CVtkLookupTable::propertyCount()
{
    int nrColors = m_vtkLookupTable->GetNumberOfColors();
    return nrColors;
}

QString CVtkLookupTable::propertyName(int index)
{
    int nrColors = m_vtkLookupTable->GetNumberOfColors();
    if(index < 0 || index >= nrColors)
        return QString();

    return QString("Color %1").arg(index+1);
}

QString CVtkLookupTable::propertyGroup(int index)
{
    return "Color Table";
}

QVariant CVtkLookupTable::propertyValue(int index)
{
    int nrColors = m_vtkLookupTable->GetNumberOfColors();
    if(index < 0 || index >= nrColors)
        return QVariant();

    // Get color vector
    double* cv = m_vtkLookupTable->GetTableValue(index);

    // Convert to QColor
    QColor ret;
    ret.setRgbF(cv[0], cv[1], cv[2], cv[3]);
    return qVariantFromValue<QColor>(ret);
}

void CVtkLookupTable::setPropertyValue(int index, QVariant value)
{
    int nrColors = m_vtkLookupTable->GetNumberOfColors();
    if(index < 0 || index >= nrColors)
        return;

    QColor color = value.value<QColor>();
    m_vtkLookupTable->SetTableValue(index, color.redF(), color.greenF(), color.blueF());
}

bool CVtkLookupTable::hasEditor(int index)
{
    // use the default editor
    Q_UNUSED(index);
    return false;
}

QWidget* CVtkLookupTable::createEditor(int index)
{
    // use the default editor
    Q_UNUSED(index);
    return 0;
}

void CVtkLookupTable::setEditorValue(int index, QWidget* widget, QVariant value)
{
    // use the default editor
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(value);
}

QVariant CVtkLookupTable::editorValue(int index, QWidget* widget)
{
    // use the default editor
    Q_UNUSED(index);
    Q_UNUSED(widget);
    return QVariant();
}

void CVtkLookupTable::connectEditor(int index, QWidget* widget, QObject* receiver, const char* member)
{
    // use the default editor
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(receiver);
    Q_UNUSED(member);
}

QString CVtkLookupTable::propertyValueString(int index)
{
    int nrColors = m_vtkLookupTable->GetNumberOfColors();
    if(index < 0 || index >= nrColors)
        return QString();

    QVariant ret = propertyValue(index);
    QColor color = ret.value<QColor>();
    return color.name();
}

QVariant CVtkLookupTable::propertyValueFromString(int index, QString strValue)
{
    int nrColors = m_vtkLookupTable->GetNumberOfColors();
    if(index < 0 || index >= nrColors)
        return QVariant();

    QColor color(strValue);
    return qVariantFromValue<QColor>(color);
}

void CVtkLookupTable::editorCreatedNotification(IProperty* property, QWidget* editor)
{
    if( !qstrcmp(property->propertyName(), "NumberOfColors") )
        m_NumColorsEditor = editor;
}

void CVtkLookupTable::editorEditedNotification(IProperty* property, QWidget* editor)
{
    Q_UNUSED(property);
    Q_UNUSED(editor);
}

void CVtkLookupTable::editorAboutToDestroyNotification(IProperty* property, QWidget* editor)
{
    if( !qstrcmp(property->propertyName(), "NumberOfColors") )
    {
        QMetaObject::invokeMethod(this, "propertyConfigurationChanged", Qt::QueuedConnection);
        m_NumColorsEditor = 0;
    }

    Q_UNUSED(editor);
}

void CVtkLookupTable::loadBandedColorTable()
{
    loadColorTable(TYPE_BANDED);
}

void CVtkLookupTable::loadBlueGrayRedYellowColorTable()
{
    loadColorTable(TYPE_BGRY);
}

void CVtkLookupTable::loadBlueRedYellowColorTable()
{
    loadColorTable(TYPE_BRY);
}

void CVtkLookupTable::loadGammaColorTable()
{
    loadColorTable(TYPE_GAMMA);
}

void CVtkLookupTable::loadHot1ColorTable()
{
    loadColorTable(TYPE_HOT1);
}

void CVtkLookupTable::loadHot2ColorTable()
{
    loadColorTable(TYPE_HOT2);
}

void CVtkLookupTable::loadIceColorTable()
{
    loadColorTable(TYPE_ICE);
}

void CVtkLookupTable::loadLightHuesColorTable()
{
    loadColorTable(TYPE_LIGHTHUES);
}

void CVtkLookupTable::loadRichColorTable()
{
    loadColorTable(TYPE_RICH);
}

void CVtkLookupTable::loadSmoothRichColorTable()
{
    loadColorTable(TYPE_SMOOTHRICH);
}

void CVtkLookupTable::loadLut16ColorTable()
{
    loadColorTable(TYPE_LUT16);
}

void CVtkLookupTable::command_ColorTablePresets()
{
    QDialog dlg( GCF::AbstractComponent::mainWindow() );
    dlg.setWindowTitle("Select Preset");

    static QStringList options = QStringList() <<
                                 "Banded" <<
                                 "Blue-Green-Red-Yellow" <<
                                 "Blue-Red-Yellow" <<
                                 "Gamma" <<
                                 "Hot1" <<
                                 "Hot2" <<
                                 "Ice" <<
                                 "Light Hues" <<
                                 "Rich" <<
                                 "Smooth Rich" <<
                                 "LUT 16" <<
                                 "Leave Unchanged";

    QVBoxLayout* layout = new QVBoxLayout(&dlg);

    QLabel* label = new QLabel(&dlg);
    label->setText("Select one of the presets below");
    layout->addWidget(label);

    QFrame* hline1 = new QFrame(&dlg);
    hline1->setFrameShape(QFrame::HLine);
    layout->addWidget(hline1);

    QList<QRadioButton*> radioButtonList;
    for(int i=0; i<options.count(); i++)
    {
        QRadioButton* btn = new QRadioButton(&dlg);
        btn->setText( options[i] );
        btn->setChecked( i==options.count()-1 );
        layout->addWidget(btn);

        radioButtonList.append(btn);
    }

    QFrame* hline2 = new QFrame(&dlg);
    hline2->setFrameShape(QFrame::HLine);
    layout->addWidget(hline2);

    QDialogButtonBox* btnBox = new QDialogButtonBox(&dlg);
    btnBox->setStandardButtons(QDialogButtonBox::Ok|QDialogButtonBox::Cancel);
    connect(btnBox, SIGNAL(accepted()), &dlg, SLOT(accept()));
    connect(btnBox, SIGNAL(rejected()), &dlg, SLOT(reject()));
    layout->addWidget(btnBox);

    dlg.setFixedWidth(300);

    if( dlg.exec() == QDialog::Rejected )
        return;

    for(int i=0; i<radioButtonList.count()-1; i++)
    {
        if(radioButtonList[i]->isChecked())
        {
            loadColorTable(i);
            break;
        }
    }
}

void CVtkLookupTable::loadColorTable(int type)
{
    static Palette pal;

    // Get a predefined color palette
    unsigned char colorBuf[256*4];
    pal.GetPalette(type, colorBuf);

    // Load it into the lookup table
    m_vtkLookupTable->SetNumberOfColors(256);
    for(int i=0; i<256; i++)
    {
        double r = double(colorBuf[4*i+0])/255.0;
        double g = double(colorBuf[4*i+1])/255.0;
        double b = double(colorBuf[4*i+2])/255.0;

        m_vtkLookupTable->SetTableValue(i, r, g, b);
    }

    if(!m_NumColorsEditor)
        QMetaObject::invokeMethod(this, "propertyConfigurationChanged", Qt::QueuedConnection);
}

/*
Code contributed by Prof. Vijay Natarajan, the Indian Institute of Science, Bangalore

Vijay Natarajan [vijayn@csa.iisc.ernet.in]
*/

Palette::Palette()
{
    m_curPalette = TYPE_RICH;
}

Palette::~Palette()
{

}

int Palette::GetCurPalette()
{
    return m_curPalette;
}

void Palette::GetPalette(int type, unsigned char *dest)
{
    int buf[256];

    m_curPalette = type;
    switch(type) {
    case TYPE_BANDED:
        buf[0] = 0xab006c; buf[1] = 0xab0064; buf[2] = 0xac0065; buf[3] = 0xc1006c;
        buf[4] = 0xc3006e; buf[5] = 0xc5006f; buf[6] = 0xc7006d; buf[7] = 0xc9006a;
        buf[8] = 0xcc0066; buf[9] = 0xce0060; buf[10] = 0xbb0054; buf[11] = 0xbd0050;
        buf[12] = 0xbf004b; buf[13] = 0xd60050; buf[14] = 0xd8004c; buf[15] = 0xda0047;
        buf[16] = 0xdd0042; buf[17] = 0xdf003e; buf[18] = 0xe1003b; buf[19] = 0xe30037;
        buf[20] = 0xce002c; buf[21] = 0xd00029; buf[22] = 0xd20026; buf[23] = 0xeb0026;
        buf[24] = 0xee0021; buf[25] = 0xf0001d; buf[26] = 0xf20019; buf[27] = 0xf40015;
        buf[28] = 0xf60010; buf[29] = 0xf8000c; buf[30] = 0xe10007; buf[31] = 0xe30003;
        buf[32] = 0xe50000; buf[33] = 0xff1000; buf[34] = 0xff2100; buf[35] = 0xff3300;
        buf[36] = 0xff4300; buf[37] = 0xff5500; buf[38] = 0xff6600; buf[39] = 0xff7700;
        buf[40] = 0xe57a00; buf[41] = 0xe58900; buf[42] = 0xe59900; buf[43] = 0xffbb00;
        buf[44] = 0xffcb00; buf[45] = 0xffdc00; buf[46] = 0xffee00; buf[47] = 0xffff00;
        buf[48] = 0xeeff00; buf[49] = 0xddff00; buf[50] = 0xb7e500; buf[51] = 0xa8e500;
        buf[52] = 0x97e500; buf[53] = 0x99ff00; buf[54] = 0x86ff00; buf[55] = 0x76ff00;
        buf[56] = 0x64ff00; buf[57] = 0x54ff00; buf[58] = 0x42ff00; buf[59] = 0x32ff00;
        buf[60] = 0x1ee500; buf[61] = 0xee500; buf[62] = 0xe500; buf[63] = 0xff07;
        buf[64] = 0xff10; buf[65] = 0xff19; buf[66] = 0xff21; buf[67] = 0xff2a;
        buf[68] = 0xff33; buf[69] = 0xff3b; buf[70] = 0xe53d; buf[71] = 0xe544;
        buf[72] = 0xe54b; buf[73] = 0xff5d; buf[74] = 0xff65; buf[75] = 0xff6d;
        buf[76] = 0xff77; buf[77] = 0xff7f; buf[78] = 0xff87; buf[79] = 0xff90;
        buf[80] = 0xe589; buf[81] = 0xe591; buf[82] = 0xe599; buf[83] = 0xffb2;
        buf[84] = 0xffbb; buf[85] = 0xffc3; buf[86] = 0xffcc; buf[87] = 0xffd4;
        buf[88] = 0xffdd; buf[89] = 0xffe4; buf[90] = 0xe5d6; buf[91] = 0xe5dc;
        buf[92] = 0xe5e5; buf[93] = 0xfaff; buf[94] = 0xf6ff; buf[95] = 0xf2ff;
        buf[96] = 0xeeff; buf[97] = 0xe8ff; buf[98] = 0xe4ff; buf[99] = 0xe1ff;
        buf[100] = 0xc6e5; buf[101] = 0xc3e5; buf[102] = 0xbfe5; buf[103] = 0xd0ff;
        buf[104] = 0xccff; buf[105] = 0xc7ff; buf[106] = 0xc3ff; buf[107] = 0xbfff;
        buf[108] = 0xbbff; buf[109] = 0xb6ff; buf[110] = 0xa0e5; buf[111] = 0x9ce5;
        buf[112] = 0x98e5; buf[113] = 0xa5ff; buf[114] = 0xa1ff; buf[115] = 0x9dff;
        buf[116] = 0x99ff; buf[117] = 0x94ff; buf[118] = 0x90ff; buf[119] = 0x8cff;
        buf[120] = 0x7ae5; buf[121] = 0x76e5; buf[122] = 0x72e5; buf[123] = 0x7dff;
        buf[124] = 0x7bff; buf[125] = 0x79ff; buf[126] = 0x77ff; buf[127] = 0x73ff;
        buf[128] = 0x71ff; buf[129] = 0x70ff; buf[130] = 0x63e5; buf[131] = 0x61e5;
        buf[132] = 0x5fe5; buf[133] = 0x68ff; buf[134] = 0x66ff; buf[135] = 0x63ff;
        buf[136] = 0x61ff; buf[137] = 0x5fff; buf[138] = 0x5dff; buf[139] = 0x5bff;
        buf[140] = 0x4fe5; buf[141] = 0x4ee5; buf[142] = 0x4ce5; buf[143] = 0x51ff;
        buf[144] = 0x50ff; buf[145] = 0x4eff; buf[146] = 0x4cff; buf[147] = 0x4aff;
        buf[148] = 0x48ff; buf[149] = 0x46ff; buf[150] = 0x3de5; buf[151] = 0x3be5;
        buf[152] = 0x39e5; buf[153] = 0x3dff; buf[154] = 0x3bff; buf[155] = 0x38ff;
        buf[156] = 0x37ff; buf[157] = 0x35ff; buf[158] = 0x31ff; buf[159] = 0x30ff;
        buf[160] = 0x2ae5; buf[161] = 0x27e5; buf[162] = 0x25e5; buf[163] = 0x28ff;
        buf[164] = 0x26ff; buf[165] = 0x23ff; buf[166] = 0x20ff; buf[167] = 0x1fff;
        buf[168] = 0x1dff; buf[169] = 0x1aff; buf[170] = 0x16e5; buf[171] = 0x15e5;
        buf[172] = 0x12e5; buf[173] = 0x12ff; buf[174] = 0x10ff; buf[175] = 0xeff;
        buf[176] = 0xcff; buf[177] = 0x9ff; buf[178] = 0x8ff; buf[179] = 0x6ff;
        buf[180] = 0x3e5; buf[181] = 0x1e5; buf[182] = 0xe5; buf[183] = 0xfd;
        buf[184] = 0xfb; buf[185] = 0xf9; buf[186] = 0xf8; buf[187] = 0xf6;
        buf[188] = 0xf4; buf[189] = 0xf2; buf[190] = 0xd9; buf[191] = 0xd7;
        buf[192] = 0xd5; buf[193] = 0xec; buf[194] = 0xea; buf[195] = 0xe8;
        buf[196] = 0xe6; buf[197] = 0xe5; buf[198] = 0xe3; buf[199] = 0xe1;
        buf[200] = 0xc9; buf[201] = 0xc8; buf[202] = 0xc6; buf[203] = 0xda;
        buf[204] = 0xd9; buf[205] = 0xd7; buf[206] = 0xd5; buf[207] = 0xd3;
        buf[208] = 0xd2; buf[209] = 0xd0; buf[210] = 0xba; buf[211] = 0xb8;
        buf[212] = 0xb6; buf[213] = 0xc9; buf[214] = 0xc7; buf[215] = 0xc6;
        buf[216] = 0xc4; buf[217] = 0xc2; buf[218] = 0xc0; buf[219] = 0xbf;
        buf[220] = 0xaa; buf[221] = 0xa9; buf[222] = 0xa7; buf[223] = 0xb8;
        buf[224] = 0xb6; buf[225] = 0xb4; buf[226] = 0xb3; buf[227] = 0xb1;
        buf[228] = 0xaf; buf[229] = 0xae; buf[230] = 0x9b; buf[231] = 0x99;
        buf[232] = 0x97; buf[233] = 0xa7; buf[234] = 0xa5; buf[235] = 0xa3;
        buf[236] = 0xa1; buf[237] = 0xa0; buf[238] = 0x9e; buf[239] = 0x9c;
        buf[240] = 0x8b; buf[241] = 0x8a; buf[242] = 0x88; buf[243] = 0x95;
        buf[244] = 0x94; buf[245] = 0x92; buf[246] = 0x90; buf[247] = 0x8f;
        buf[248] = 0x8d; buf[249] = 0x8b; buf[250] = 0x7c; buf[251] = 0x7a;
        buf[252] = 0x78; buf[253] = 0x82; buf[254] = 0x84; buf[255] = 0x7a;
        break;

    case TYPE_BGRY :/* Blue-green-red-yellow.pal */
        buf[0] = 0; buf[1] = 0x20000; buf[2] = 0x40000; buf[3] = 0x60000;
        buf[4] = 0x80000; buf[5] = 0xa0000; buf[6] = 0xc0000; buf[7] = 0xe0000;
        buf[8] = 0x100000; buf[9] = 0x120000; buf[10] = 0x140000; buf[11] = 0x160000;
        buf[12] = 0x190000; buf[13] = 0x1b0000; buf[14] = 0x1d0000; buf[15] = 0x1f0000;
        buf[16] = 0x210000; buf[17] = 0x230000; buf[18] = 0x250000; buf[19] = 0x270000;
        buf[20] = 0x290000; buf[21] = 0x2b0000; buf[22] = 0x2d0000; buf[23] = 0x2f0000;
        buf[24] = 0x320000; buf[25] = 0x340000; buf[26] = 0x360000; buf[27] = 0x380000;
        buf[28] = 0x3a0000; buf[29] = 0x3c0000; buf[30] = 0x3e0000; buf[31] = 0x400000;
        buf[32] = 0x420000; buf[33] = 0x440300; buf[34] = 0x460600; buf[35] = 0x480900;
        buf[36] = 0x4b0c00; buf[37] = 0x4d0f00; buf[38] = 0x4f1200; buf[39] = 0x511500;
        buf[40] = 0x531900; buf[41] = 0x551c00; buf[42] = 0x571f00; buf[43] = 0x592200;
        buf[44] = 0x5b2500; buf[45] = 0x5d2800; buf[46] = 0x5f2b00; buf[47] = 0x612e00;
        buf[48] = 0x643200; buf[49] = 0x643500; buf[50] = 0x643800; buf[51] = 0x643b00;
        buf[52] = 0x643e00; buf[53] = 0x644100; buf[54] = 0x644400; buf[55] = 0x644700;
        buf[56] = 0x644b00; buf[57] = 0x644e00; buf[58] = 0x645100; buf[59] = 0x645400;
        buf[60] = 0x645700; buf[61] = 0x645a00; buf[62] = 0x645d00; buf[63] = 0x646000;
        buf[64] = 0x646400; buf[65] = 0x646700; buf[66] = 0x646a00; buf[67] = 0x646d00;
        buf[68] = 0x647000; buf[69] = 0x647300; buf[70] = 0x647600; buf[71] = 0x647900;
        buf[72] = 0x647d00; buf[73] = 0x648000; buf[74] = 0x648300; buf[75] = 0x648600;
        buf[76] = 0x648900; buf[77] = 0x648c00; buf[78] = 0x648f00; buf[79] = 0x649200;
        buf[80] = 0x649600; buf[81] = 0x609600; buf[82] = 0x5d9600; buf[83] = 0x5a9600;
        buf[84] = 0x579600; buf[85] = 0x549600; buf[86] = 0x519600; buf[87] = 0x4e9600;
        buf[88] = 0x4b9600; buf[89] = 0x479600; buf[90] = 0x449600; buf[91] = 0x419600;
        buf[92] = 0x3e9600; buf[93] = 0x3b9600; buf[94] = 0x389600; buf[95] = 0x359600;
        buf[96] = 0x329600; buf[97] = 0x2e9500; buf[98] = 0x2b9400; buf[99] = 0x289400;
        buf[100] = 0x259300; buf[101] = 0x229200; buf[102] = 0x1f9200; buf[103] = 0x1c9100;
        buf[104] = 0x199100; buf[105] = 0x159000; buf[106] = 0x128f00; buf[107] = 0xf8f00;
        buf[108] = 0xc8e00; buf[109] = 0x98d00; buf[110] = 0x68d00; buf[111] = 0x38c00;
        buf[112] = 0x8c00; buf[113] = 0x8907; buf[114] = 0x870f; buf[115] = 0x8416;
        buf[116] = 0x821e; buf[117] = 0x7f25; buf[118] = 0x7d2d; buf[119] = 0x7a34;
        buf[120] = 0x783c; buf[121] = 0x7543; buf[122] = 0x734b; buf[123] = 0x7052;
        buf[124] = 0x6e5a; buf[125] = 0x6b61; buf[126] = 0x6969; buf[127] = 0x6670;
        buf[128] = 0x6478; buf[129] = 0x5d7d; buf[130] = 0x5782; buf[131] = 0x5187;
        buf[132] = 0x4b8c; buf[133] = 0x4491; buf[134] = 0x3e96; buf[135] = 0x389b;
        buf[136] = 0x32a0; buf[137] = 0x2ba5; buf[138] = 0x25aa; buf[139] = 0x1faf;
        buf[140] = 0x19b4; buf[141] = 0x12b9; buf[142] = 0xcbe; buf[143] = 0x6c3;
        buf[144] = 0xc8; buf[145] = 0x2c8; buf[146] = 0x4c9; buf[147] = 0x6c9;
        buf[148] = 0x9ca; buf[149] = 0xbca; buf[150] = 0xdcb; buf[151] = 0x10cb;
        buf[152] = 0x12cc; buf[153] = 0x14cc; buf[154] = 0x17cd; buf[155] = 0x19cd;
        buf[156] = 0x1bce; buf[157] = 0x1dce; buf[158] = 0x20cf; buf[159] = 0x22cf;
        buf[160] = 0x24d0; buf[161] = 0x27d0; buf[162] = 0x29d1; buf[163] = 0x2bd1;
        buf[164] = 0x2ed2; buf[165] = 0x30d2; buf[166] = 0x32d3; buf[167] = 0x35d3;
        buf[168] = 0x37d4; buf[169] = 0x39d4; buf[170] = 0x3bd5; buf[171] = 0x3ed5;
        buf[172] = 0x40d6; buf[173] = 0x42d6; buf[174] = 0x45d7; buf[175] = 0x47d7;
        buf[176] = 0x49d8; buf[177] = 0x4cd8; buf[178] = 0x4ed9; buf[179] = 0x50d9;
        buf[180] = 0x53da; buf[181] = 0x55da; buf[182] = 0x57db; buf[183] = 0x59db;
        buf[184] = 0x5cdc; buf[185] = 0x5edc; buf[186] = 0x60dd; buf[187] = 0x63dd;
        buf[188] = 0x65de; buf[189] = 0x67de; buf[190] = 0x6adf; buf[191] = 0x6cdf;
        buf[192] = 0x6ee0; buf[193] = 0x71e0; buf[194] = 0x73e1; buf[195] = 0x75e1;
        buf[196] = 0x77e2; buf[197] = 0x7ae2; buf[198] = 0x7ce3; buf[199] = 0x7ee3;
        buf[200] = 0x81e4; buf[201] = 0x83e4; buf[202] = 0x85e5; buf[203] = 0x88e5;
        buf[204] = 0x8ae6; buf[205] = 0x8ce6; buf[206] = 0x8ee7; buf[207] = 0x91e7;
        buf[208] = 0x93e8; buf[209] = 0x95e8; buf[210] = 0x98e9; buf[211] = 0x9ae9;
        buf[212] = 0x9cea; buf[213] = 0x9fea; buf[214] = 0xa1eb; buf[215] = 0xa3eb;
        buf[216] = 0xa6ec; buf[217] = 0xa8ec; buf[218] = 0xaaed; buf[219] = 0xaced;
        buf[220] = 0xafee; buf[221] = 0xb1ee; buf[222] = 0xb3ef; buf[223] = 0xb6ef;
        buf[224] = 0xb8f0; buf[225] = 0xbaf0; buf[226] = 0xbdf1; buf[227] = 0xbff1;
        buf[228] = 0xc1f2; buf[229] = 0xc4f2; buf[230] = 0xc6f3; buf[231] = 0xc8f3;
        buf[232] = 0xcaf4; buf[233] = 0xcdf4; buf[234] = 0xcff5; buf[235] = 0xd1f5;
        buf[236] = 0xd4f6; buf[237] = 0xd6f6; buf[238] = 0xd8f7; buf[239] = 0xdbf7;
        buf[240] = 0xddf8; buf[241] = 0xdff8; buf[242] = 0xe2f9; buf[243] = 0xe4f9;
        buf[244] = 0xe6fa; buf[245] = 0xe8fa; buf[246] = 0xebfb; buf[247] = 0xedfb;
        buf[248] = 0xeffc; buf[249] = 0xf2fc; buf[250] = 0xf4fd; buf[251] = 0xf6fd;
        buf[252] = 0xf9fe; buf[253] = 0xfbfe; buf[254] = 0xfdff; buf[255] = 0xffff;
        break;

    case TYPE_BRY :/* Blue-red-yellow.pal */
        buf[0] = 0xff0500; buf[1] = 0xff0500; buf[2] = 0xfa0500; buf[3] = 0xf50500;
        buf[4] = 0xf00004; buf[5] = 0xeb0005; buf[6] = 0xe50008; buf[7] = 0xe00009;
        buf[8] = 0xdb000c; buf[9] = 0xd6000c; buf[10] = 0xd0000f; buf[11] = 0xcb0011;
        buf[12] = 0xc60013; buf[13] = 0xc10014; buf[14] = 0xbc0016; buf[15] = 0xb60018;
        buf[16] = 0xb1001a; buf[17] = 0xac001a; buf[18] = 0xa7001c; buf[19] = 0xa2001f;
        buf[20] = 0x9d001f; buf[21] = 0x980022; buf[22] = 0x920024; buf[23] = 0x8d0025;
        buf[24] = 0x880027; buf[25] = 0x830028; buf[26] = 0x7e002a; buf[27] = 0x7a002c;
        buf[28] = 0x75002e; buf[29] = 0x700030; buf[30] = 0x6b0032; buf[31] = 0x660033;
        buf[32] = 0x620034; buf[33] = 0x5d0037; buf[34] = 0x590038; buf[35] = 0x54003a;
        buf[36] = 0x50003c; buf[37] = 0x4c003e; buf[38] = 0x47003f; buf[39] = 0x430040;
        buf[40] = 0x3e0043; buf[41] = 0x3c0045; buf[42] = 0x370046; buf[43] = 0x340048;
        buf[44] = 0x31004a; buf[45] = 0x2c004b; buf[46] = 0x2a004d; buf[47] = 0x26004f;
        buf[48] = 0x230050; buf[49] = 0x200052; buf[50] = 0x1e0054; buf[51] = 0x1c0055;
        buf[52] = 0x190057; buf[53] = 0x160059; buf[54] = 0x15005a; buf[55] = 0x13005c;
        buf[56] = 0x10005d; buf[57] = 0xf005f; buf[58] = 0xe0061; buf[59] = 0xd0062;
        buf[60] = 0xa0064; buf[61] = 0xa0065; buf[62] = 0x80067; buf[63] = 0x70068;
        buf[64] = 0x60069; buf[65] = 0x5006b; buf[66] = 0x5006c; buf[67] = 0x4006e;
        buf[68] = 0x3006f; buf[69] = 0x30070; buf[70] = 0x20072; buf[71] = 0x10073;
        buf[72] = 0x20074; buf[73] = 0x75; buf[74] = 0x77; buf[75] = 0x78;
        buf[76] = 0x79; buf[77] = 0x7a; buf[78] = 0x7b; buf[79] = 0x7c;
        buf[80] = 0x7e; buf[81] = 0x7f; buf[82] = 0x80; buf[83] = 0x81;
        buf[84] = 0x82; buf[85] = 0x83; buf[86] = 0x84; buf[87] = 0x85;
        buf[88] = 0x85; buf[89] = 0x86; buf[90] = 0x87; buf[91] = 0x88;
        buf[92] = 0x89; buf[93] = 0x8a; buf[94] = 0x8b; buf[95] = 0x8c;
        buf[96] = 0x8c; buf[97] = 0x8d; buf[98] = 0x8e; buf[99] = 0x8f;
        buf[100] = 0x90; buf[101] = 0x90; buf[102] = 0x91; buf[103] = 0x92;
        buf[104] = 0x93; buf[105] = 0x93; buf[106] = 0x94; buf[107] = 0x95;
        buf[108] = 0x96; buf[109] = 0x96; buf[110] = 0x97; buf[111] = 0x98;
        buf[112] = 0x99; buf[113] = 0x99; buf[114] = 0x9a; buf[115] = 0x19b;
        buf[116] = 0x9c; buf[117] = 0x19c; buf[118] = 0x19d; buf[119] = 0x19e;
        buf[120] = 0x29e; buf[121] = 0x29f; buf[122] = 0x4a0; buf[123] = 0x4a1;
        buf[124] = 0x4a1; buf[125] = 0x5a2; buf[126] = 0x6a3; buf[127] = 0x6a3;
        buf[128] = 0x7a4; buf[129] = 0x7a5; buf[130] = 0x9a6; buf[131] = 0x9a6;
        buf[132] = 0xaa7; buf[133] = 0xaa8; buf[134] = 0xca9; buf[135] = 0xca9;
        buf[136] = 0xeaa; buf[137] = 0xfab; buf[138] = 0x11ab; buf[139] = 0x11ac;
        buf[140] = 0x13ad; buf[141] = 0x15ae; buf[142] = 0x15ae; buf[143] = 0x17af;
        buf[144] = 0x19b0; buf[145] = 0x1bb0; buf[146] = 0x1bb1; buf[147] = 0x1db2;
        buf[148] = 0x1eb3; buf[149] = 0x21b3; buf[150] = 0x22b4; buf[151] = 0x25b5;
        buf[152] = 0x27b6; buf[153] = 0x28b6; buf[154] = 0x2ab7; buf[155] = 0x2bb8;
        buf[156] = 0x2eb8; buf[157] = 0x30b9; buf[158] = 0x31ba; buf[159] = 0x34bb;
        buf[160] = 0x36bb; buf[161] = 0x38bc; buf[162] = 0x3abd; buf[163] = 0x3cbe;
        buf[164] = 0x3ebe; buf[165] = 0x40bf; buf[166] = 0x41c0; buf[167] = 0x44c0;
        buf[168] = 0x46c1; buf[169] = 0x49c2; buf[170] = 0x4bc3; buf[171] = 0x4dc3;
        buf[172] = 0x4fc4; buf[173] = 0x51c5; buf[174] = 0x52c5; buf[175] = 0x55c6;
        buf[176] = 0x57c7; buf[177] = 0x59c8; buf[178] = 0x5cc8; buf[179] = 0x5ec9;
        buf[180] = 0x5fca; buf[181] = 0x62cb; buf[182] = 0x63cb; buf[183] = 0x67cc;
        buf[184] = 0x69cd; buf[185] = 0x6bcd; buf[186] = 0x6dce; buf[187] = 0x6fcf;
        buf[188] = 0x71d0; buf[189] = 0x74d0; buf[190] = 0x76d1; buf[191] = 0x77d2;
        buf[192] = 0x7ad2; buf[193] = 0x7cd3; buf[194] = 0x7ed4; buf[195] = 0x80d5;
        buf[196] = 0x83d5; buf[197] = 0x85d6; buf[198] = 0x87d7; buf[199] = 0x89d8;
        buf[200] = 0x8bd8; buf[201] = 0x8dd9; buf[202] = 0x8fda; buf[203] = 0x92da;
        buf[204] = 0x94db; buf[205] = 0x96dc; buf[206] = 0x98dd; buf[207] = 0x9add;
        buf[208] = 0x9cde; buf[209] = 0x9edf; buf[210] = 0xa1df; buf[211] = 0xa3e0;
        buf[212] = 0xa5e1; buf[213] = 0xa7e2; buf[214] = 0xa9e2; buf[215] = 0xabe3;
        buf[216] = 0xaee4; buf[217] = 0xb0e5; buf[218] = 0xb2e5; buf[219] = 0xb3e6;
        buf[220] = 0xb6e7; buf[221] = 0xb8e7; buf[222] = 0xbae8; buf[223] = 0xbde9;
        buf[224] = 0xbfea; buf[225] = 0xc1ea; buf[226] = 0xc3eb; buf[227] = 0xc5ec;
        buf[228] = 0xc7ec; buf[229] = 0xc9ed; buf[230] = 0xccee; buf[231] = 0xceef;
        buf[232] = 0xd0ef; buf[233] = 0xd2f0; buf[234] = 0xd3f1; buf[235] = 0xd6f2;
        buf[236] = 0xd8f2; buf[237] = 0xdbf3; buf[238] = 0xddf4; buf[239] = 0xdff4;
        buf[240] = 0xe1f5; buf[241] = 0xe3f6; buf[242] = 0xe5f7; buf[243] = 0xe8f7;
        buf[244] = 0xeaf8; buf[245] = 0xecf9; buf[246] = 0xeef9; buf[247] = 0xeffa;
        buf[248] = 0xf2fb; buf[249] = 0xf3fc; buf[250] = 0xf6fc; buf[251] = 0xf9fd;
        buf[252] = 0xfbfe; buf[253] = 0xf6ff; buf[254] = 0xf6ff; buf[255] = 0xfff0;
        break;

    case TYPE_GAMMA :/* Gamma.pal */
        buf[0] = 0; buf[1] = 0x50000; buf[2] = 0xa0000; buf[3] = 0xf0000;
        buf[4] = 0x140000; buf[5] = 0x1a0000; buf[6] = 0x1f0000; buf[7] = 0x240000;
        buf[8] = 0x290000; buf[9] = 0x2e0000; buf[10] = 0x340000; buf[11] = 0x390000;
        buf[12] = 0x3e0000; buf[13] = 0x430000; buf[14] = 0x480000; buf[15] = 0x4e0000;
        buf[16] = 0x530000; buf[17] = 0x580000; buf[18] = 0x5d0000; buf[19] = 0x620000;
        buf[20] = 0x680000; buf[21] = 0x6d0000; buf[22] = 0x720000; buf[23] = 0x770000;
        buf[24] = 0x7c0000; buf[25] = 0x820000; buf[26] = 0x870000; buf[27] = 0x8c0000;
        buf[28] = 0x910000; buf[29] = 0x960000; buf[30] = 0x9c0000; buf[31] = 0xa10000;
        buf[32] = 0xa60000; buf[33] = 0xab0000; buf[34] = 0xb00000; buf[35] = 0xb60000;
        buf[36] = 0xbb0000; buf[37] = 0xc00000; buf[38] = 0xc50000; buf[39] = 0xca0000;
        buf[40] = 0xd00000; buf[41] = 0xd50000; buf[42] = 0xda0000; buf[43] = 0xdf0000;
        buf[44] = 0xe40000; buf[45] = 0xea0000; buf[46] = 0xef0000; buf[47] = 0xf40000;
        buf[48] = 0xf90004; buf[49] = 0xff0009; buf[50] = 0xfa000e; buf[51] = 0xf50013;
        buf[52] = 0xef0017; buf[53] = 0xea001c; buf[54] = 0xe40021; buf[55] = 0xdf0026;
        buf[56] = 0xda002a; buf[57] = 0xd4002f; buf[58] = 0xcf0034; buf[59] = 0xc90039;
        buf[60] = 0xc4003d; buf[61] = 0xbe0042; buf[62] = 0xb90047; buf[63] = 0xb4004c;
        buf[64] = 0xae0051; buf[65] = 0xa90051; buf[66] = 0xa30051; buf[67] = 0x9e0051;
        buf[68] = 0x980051; buf[69] = 0x930051; buf[70] = 0x8e0051; buf[71] = 0x880051;
        buf[72] = 0x830050; buf[73] = 0x7d0050; buf[74] = 0x780050; buf[75] = 0x720050;
        buf[76] = 0x6d0050; buf[77] = 0x680050; buf[78] = 0x620050; buf[79] = 0x5d004f;
        buf[80] = 0x570054; buf[81] = 0x520059; buf[82] = 0x4c005e; buf[83] = 0x470063;
        buf[84] = 0x420068; buf[85] = 0x3c006d; buf[86] = 0x370072; buf[87] = 0x310077;
        buf[88] = 0x2c007c; buf[89] = 0x260081; buf[90] = 0x210086; buf[91] = 0x1c008b;
        buf[92] = 0x160090; buf[93] = 0x110095; buf[94] = 0xb009a; buf[95] = 0x6009f;
        buf[96] = 0xa4; buf[97] = 0xa9; buf[98] = 0xae; buf[99] = 0xb4;
        buf[100] = 0xb9; buf[101] = 0xbe; buf[102] = 0xc4; buf[103] = 0xc9;
        buf[104] = 0xce; buf[105] = 0xd4; buf[106] = 0xd9; buf[107] = 0xde;
        buf[108] = 0xe4; buf[109] = 0xe9; buf[110] = 0xff; buf[111] = 0xff;
        buf[112] = 0xff; buf[113] = 0xff; buf[114] = 0x5ff; buf[115] = 0xaff;
        buf[116] = 0x10ff; buf[117] = 0x15ff; buf[118] = 0x1bff; buf[119] = 0x20ff;
        buf[120] = 0x25ff; buf[121] = 0x2bff; buf[122] = 0x30ff; buf[123] = 0x36ff;
        buf[124] = 0x3bff; buf[125] = 0x40ff; buf[126] = 0x46ff; buf[127] = 0x4bff;
        buf[128] = 0x51ff; buf[129] = 0x455ff; buf[130] = 0x95aff; buf[131] = 0xe5fff;
        buf[132] = 0x1364ff; buf[133] = 0x1869ff; buf[134] = 0x1c6dff; buf[135] = 0x2172ff;
        buf[136] = 0x2677ff; buf[137] = 0x2b7cff; buf[138] = 0x3081ff; buf[139] = 0x3586ff;
        buf[140] = 0x398aff; buf[141] = 0x3e8fff; buf[142] = 0x4394ff; buf[143] = 0x4899ff;
        buf[144] = 0x4d9eff; buf[145] = 0x52a3ff; buf[146] = 0x4da3ff; buf[147] = 0x47a3ff;
        buf[148] = 0x41a3ff; buf[149] = 0x3ba3ff; buf[150] = 0x35a3ff; buf[151] = 0x2fa3ff;
        buf[152] = 0x29a3ff; buf[153] = 0x24a3ff; buf[154] = 0x1ea3ff; buf[155] = 0x18a3ff;
        buf[156] = 0x12a3ff; buf[157] = 0xca3ff; buf[158] = 0x6a3ff; buf[159] = 0xa3ff;
        buf[160] = 0xa3ff; buf[161] = 0xa3ff; buf[162] = 0xa3ff; buf[163] = 0xa3f8;
        buf[164] = 0xa3f0; buf[165] = 0xa3e8; buf[166] = 0xa3e1; buf[167] = 0xa3d9;
        buf[168] = 0xa3d1; buf[169] = 0xa3ca; buf[170] = 0xa3c2; buf[171] = 0xa3ba;
        buf[172] = 0xa3b3; buf[173] = 0xa3ab; buf[174] = 0xa3a3; buf[175] = 0xa3a8;
        buf[176] = 0xa3ad; buf[177] = 0x3a9b2; buf[178] = 0x6afb7; buf[179] = 0x9b5bc;
        buf[180] = 0xcbbc1; buf[181] = 0x10c1c6; buf[182] = 0x13c7cb; buf[183] = 0x16cdd1;
        buf[184] = 0x19d4d6; buf[185] = 0x1ddadb; buf[186] = 0x20e0e0; buf[187] = 0x23e6e5;
        buf[188] = 0x26ecea; buf[189] = 0x29f2ef; buf[190] = 0x2df8f4; buf[191] = 0x30fff9;
        buf[192] = 0x33ffff; buf[193] = 0x36ffff; buf[194] = 0x3affff; buf[195] = 0x3dffff;
        buf[196] = 0x40ffff; buf[197] = 0x43ffff; buf[198] = 0x47ffff; buf[199] = 0x4affff;
        buf[200] = 0x4dffff; buf[201] = 0x50ffff; buf[202] = 0x53ffff; buf[203] = 0x57ffff;
        buf[204] = 0x5affff; buf[205] = 0x5dffff; buf[206] = 0x60ffff; buf[207] = 0x64ffff;
        buf[208] = 0x67ffff; buf[209] = 0x6affff; buf[210] = 0x6dffff; buf[211] = 0x70ffff;
        buf[212] = 0x74ffff; buf[213] = 0x77ffff; buf[214] = 0x7affff; buf[215] = 0x7dffff;
        buf[216] = 0x81ffff; buf[217] = 0x84ffff; buf[218] = 0x87ffff; buf[219] = 0x8affff;
        buf[220] = 0x8effff; buf[221] = 0x91ffff; buf[222] = 0x94ffff; buf[223] = 0x97ffff;
        buf[224] = 0x9affff; buf[225] = 0x9effff; buf[226] = 0xa1ffff; buf[227] = 0xa4ffff;
        buf[228] = 0xa7ffff; buf[229] = 0xabffff; buf[230] = 0xaeffff; buf[231] = 0xb1ffff;
        buf[232] = 0xb4ffff; buf[233] = 0xb7ffff; buf[234] = 0xbbffff; buf[235] = 0xbeffff;
        buf[236] = 0xc1ffff; buf[237] = 0xc4ffff; buf[238] = 0xc8ffff; buf[239] = 0xcbffff;
        buf[240] = 0xceffff; buf[241] = 0xd1ffff; buf[242] = 0xd5ffff; buf[243] = 0xd8ffff;
        buf[244] = 0xdbffff; buf[245] = 0xdeffff; buf[246] = 0xe1ffff; buf[247] = 0xe5ffff;
        buf[248] = 0xe8ffff; buf[249] = 0xebffff; buf[250] = 0xeeffff; buf[251] = 0xf2ffff;
        buf[252] = 0xf5ffff; buf[253] = 0xf8ffff; buf[254] = 0xfbffff; buf[255] = 0xffffff;
        break;


    case TYPE_HOT1 :  /* Hot.pal */
        buf[0] = 0; buf[1] = 0x1; buf[2] = 0x2; buf[3] = 0x4;
        buf[4] = 0x5; buf[5] = 0x7; buf[6] = 0x8; buf[7] = 0xa;
        buf[8] = 0xb; buf[9] = 0xd; buf[10] = 0xe; buf[11] = 0xf;
        buf[12] = 0x11; buf[13] = 0x12; buf[14] = 0x14; buf[15] = 0x15;
        buf[16] = 0x17; buf[17] = 0x18; buf[18] = 0x1a; buf[19] = 0x1b;
        buf[20] = 0x1c; buf[21] = 0x1e; buf[22] = 0x1f; buf[23] = 0x21;
        buf[24] = 0x22; buf[25] = 0x24; buf[26] = 0x25; buf[27] = 0x27;
        buf[28] = 0x28; buf[29] = 0x2a; buf[30] = 0x2b; buf[31] = 0x2c;
        buf[32] = 0x2e; buf[33] = 0x2f; buf[34] = 0x31; buf[35] = 0x32;
        buf[36] = 0x34; buf[37] = 0x35; buf[38] = 0x37; buf[39] = 0x38;
        buf[40] = 0x39; buf[41] = 0x3b; buf[42] = 0x3c; buf[43] = 0x3e;
        buf[44] = 0x3f; buf[45] = 0x41; buf[46] = 0x42; buf[47] = 0x44;
        buf[48] = 0x45; buf[49] = 0x46; buf[50] = 0x48; buf[51] = 0x49;
        buf[52] = 0x4b; buf[53] = 0x4c; buf[54] = 0x4e; buf[55] = 0x4f;
        buf[56] = 0x51; buf[57] = 0x52; buf[58] = 0x54; buf[59] = 0x55;
        buf[60] = 0x56; buf[61] = 0x58; buf[62] = 0x59; buf[63] = 0x5b;
        buf[64] = 0x5c; buf[65] = 0x5e; buf[66] = 0x5f; buf[67] = 0x61;
        buf[68] = 0x62; buf[69] = 0x63; buf[70] = 0x65; buf[71] = 0x66;
        buf[72] = 0x68; buf[73] = 0x69; buf[74] = 0x6b; buf[75] = 0x6c;
        buf[76] = 0x6e; buf[77] = 0x6f; buf[78] = 0x71; buf[79] = 0x72;
        buf[80] = 0x73; buf[81] = 0x75; buf[82] = 0x76; buf[83] = 0x78;
        buf[84] = 0x79; buf[85] = 0x7b; buf[86] = 0x7c; buf[87] = 0x7e;
        buf[88] = 0x7f; buf[89] = 0x80; buf[90] = 0x82; buf[91] = 0x83;
        buf[92] = 0x85; buf[93] = 0x86; buf[94] = 0x88; buf[95] = 0x89;
        buf[96] = 0x8b; buf[97] = 0x8c; buf[98] = 0x8d; buf[99] = 0x8f;
        buf[100] = 0x90; buf[101] = 0x92; buf[102] = 0x93; buf[103] = 0x95;
        buf[104] = 0x96; buf[105] = 0x98; buf[106] = 0x99; buf[107] = 0x9b;
        buf[108] = 0x9c; buf[109] = 0x9d; buf[110] = 0x9f; buf[111] = 0xa0;
        buf[112] = 0xa2; buf[113] = 0xa3; buf[114] = 0xa5; buf[115] = 0xa6;
        buf[116] = 0xa8; buf[117] = 0xa9; buf[118] = 0xaa; buf[119] = 0xac;
        buf[120] = 0xad; buf[121] = 0x1af; buf[122] = 0x3b0; buf[123] = 0x5b2;
        buf[124] = 0x7b3; buf[125] = 0x9b5; buf[126] = 0xbb6; buf[127] = 0xdb8;
        buf[128] = 0xfb9; buf[129] = 0x11ba; buf[130] = 0x12bc; buf[131] = 0x14bd;
        buf[132] = 0x16bf; buf[133] = 0x18c0; buf[134] = 0x1ac2; buf[135] = 0x1cc3;
        buf[136] = 0x1ec5; buf[137] = 0x20c6; buf[138] = 0x22c7; buf[139] = 0x23c9;
        buf[140] = 0x25ca; buf[141] = 0x27cc; buf[142] = 0x29cd; buf[143] = 0x2bcf;
        buf[144] = 0x2dd0; buf[145] = 0x2fd2; buf[146] = 0x31d3; buf[147] = 0x33d4;
        buf[148] = 0x34d6; buf[149] = 0x36d7; buf[150] = 0x38d9; buf[151] = 0x3ada;
        buf[152] = 0x3cdc; buf[153] = 0x3edd; buf[154] = 0x40df; buf[155] = 0x42e0;
        buf[156] = 0x44e2; buf[157] = 0x45e3; buf[158] = 0x47e4; buf[159] = 0x49e6;
        buf[160] = 0x4be7; buf[161] = 0x4de9; buf[162] = 0x4fea; buf[163] = 0x51ec;
        buf[164] = 0x53ed; buf[165] = 0x55ef; buf[166] = 0x56f0; buf[167] = 0x58f1;
        buf[168] = 0x5af3; buf[169] = 0x5cf4; buf[170] = 0x5ef6; buf[171] = 0x60f7;
        buf[172] = 0x62f9; buf[173] = 0x64fa; buf[174] = 0x66fc; buf[175] = 0x67fd;
        buf[176] = 0x69ff; buf[177] = 0x6bff; buf[178] = 0x6dff; buf[179] = 0x6fff;
        buf[180] = 0x71ff; buf[181] = 0x73ff; buf[182] = 0x75ff; buf[183] = 0x77ff;
        buf[184] = 0x78ff; buf[185] = 0x7aff; buf[186] = 0x7cff; buf[187] = 0x7eff;
        buf[188] = 0x80ff; buf[189] = 0x82ff; buf[190] = 0x84ff; buf[191] = 0x386ff;
        buf[192] = 0x788ff; buf[193] = 0xb89ff; buf[194] = 0xf8bff; buf[195] = 0x138dff;
        buf[196] = 0x178fff; buf[197] = 0x1b91ff; buf[198] = 0x1f93ff; buf[199] = 0x2395ff;
        buf[200] = 0x2797ff; buf[201] = 0x2b99ff; buf[202] = 0x2f9aff; buf[203] = 0x339cff;
        buf[204] = 0x369eff; buf[205] = 0x3aa0ff; buf[206] = 0x3ea2ff; buf[207] = 0x42a4ff;
        buf[208] = 0x46a6ff; buf[209] = 0x4aa8ff; buf[210] = 0x4eaaff; buf[211] = 0x52abff;
        buf[212] = 0x56adff; buf[213] = 0x5aafff; buf[214] = 0x5eb1ff; buf[215] = 0x62b3ff;
        buf[216] = 0x66b5ff; buf[217] = 0x69b7ff; buf[218] = 0x6db9ff; buf[219] = 0x71bbff;
        buf[220] = 0x75bcff; buf[221] = 0x79beff; buf[222] = 0x7dc0ff; buf[223] = 0x81c2ff;
        buf[224] = 0x85c4ff; buf[225] = 0x89c6ff; buf[226] = 0x8dc8ff; buf[227] = 0x91caff;
        buf[228] = 0x95ccff; buf[229] = 0x99cdff; buf[230] = 0x9ccfff; buf[231] = 0xa0d1ff;
        buf[232] = 0xa4d3ff; buf[233] = 0xa8d5ff; buf[234] = 0xacd7ff; buf[235] = 0xb0d9ff;
        buf[236] = 0xb4dbff; buf[237] = 0xb8ddff; buf[238] = 0xbcdeff; buf[239] = 0xc0e0ff;
        buf[240] = 0xc4e2ff; buf[241] = 0xc8e4ff; buf[242] = 0xcce6ff; buf[243] = 0xcfe8ff;
        buf[244] = 0xd3eaff; buf[245] = 0xd7ecff; buf[246] = 0xdbeeff; buf[247] = 0xdfefff;
        buf[248] = 0xe3f1ff; buf[249] = 0xe7f3ff; buf[250] = 0xebf5ff; buf[251] = 0xeff7ff;
        buf[252] = 0xf3f9ff; buf[253] = 0xf7fbff; buf[254] = 0xfbfdff; buf[255] = 0xffffff;
        break;

    case TYPE_HOT2 :/* HotII.pal */
        buf[0] = 0; buf[1] = 0x1; buf[2] = 0x1; buf[3] = 0x3;
        buf[4] = 0x5; buf[5] = 0x6; buf[6] = 0x8; buf[7] = 0xa;
        buf[8] = 0xc; buf[9] = 0xd; buf[10] = 0xf; buf[11] = 0x11;
        buf[12] = 0x13; buf[13] = 0x14; buf[14] = 0x16; buf[15] = 0x18;
        buf[16] = 0x1a; buf[17] = 0x1b; buf[18] = 0x1d; buf[19] = 0x1f;
        buf[20] = 0x20; buf[21] = 0x22; buf[22] = 0x24; buf[23] = 0x26;
        buf[24] = 0x27; buf[25] = 0x29; buf[26] = 0x2b; buf[27] = 0x2d;
        buf[28] = 0x2e; buf[29] = 0x30; buf[30] = 0x32; buf[31] = 0x33;
        buf[32] = 0x35; buf[33] = 0x37; buf[34] = 0x39; buf[35] = 0x3a;
        buf[36] = 0x3c; buf[37] = 0x3e; buf[38] = 0x40; buf[39] = 0x41;
        buf[40] = 0x43; buf[41] = 0x45; buf[42] = 0x46; buf[43] = 0x48;
        buf[44] = 0x4a; buf[45] = 0x4b; buf[46] = 0x4d; buf[47] = 0x4f;
        buf[48] = 0x50; buf[49] = 0x52; buf[50] = 0x54; buf[51] = 0x55;
        buf[52] = 0x57; buf[53] = 0x59; buf[54] = 0x5a; buf[55] = 0x5c;
        buf[56] = 0x5d; buf[57] = 0x5f; buf[58] = 0x61; buf[59] = 0x62;
        buf[60] = 0x64; buf[61] = 0x65; buf[62] = 0x67; buf[63] = 0x68;
        buf[64] = 0x69; buf[65] = 0x6b; buf[66] = 0x6c; buf[67] = 0x6e;
        buf[68] = 0x6f; buf[69] = 0x70; buf[70] = 0x72; buf[71] = 0x73;
        buf[72] = 0x74; buf[73] = 0x75; buf[74] = 0x77; buf[75] = 0x78;
        buf[76] = 0x79; buf[77] = 0x7a; buf[78] = 0x7b; buf[79] = 0x7c;
        buf[80] = 0x7e; buf[81] = 0x7f; buf[82] = 0x80; buf[83] = 0x81;
        buf[84] = 0x82; buf[85] = 0x83; buf[86] = 0x84; buf[87] = 0x85;
        buf[88] = 0x85; buf[89] = 0x86; buf[90] = 0x87; buf[91] = 0x88;
        buf[92] = 0x89; buf[93] = 0x8a; buf[94] = 0x8b; buf[95] = 0x8c;
        buf[96] = 0x8c; buf[97] = 0x8d; buf[98] = 0x8e; buf[99] = 0x8f;
        buf[100] = 0x90; buf[101] = 0x90; buf[102] = 0x91; buf[103] = 0x92;
        buf[104] = 0x93; buf[105] = 0x93; buf[106] = 0x94; buf[107] = 0x95;
        buf[108] = 0x96; buf[109] = 0x96; buf[110] = 0x97; buf[111] = 0x98;
        buf[112] = 0x99; buf[113] = 0x99; buf[114] = 0x9a; buf[115] = 0x19b;
        buf[116] = 0x9c; buf[117] = 0x19c; buf[118] = 0x19d; buf[119] = 0x19e;
        buf[120] = 0x29e; buf[121] = 0x29f; buf[122] = 0x4a0; buf[123] = 0x4a1;
        buf[124] = 0x4a1; buf[125] = 0x5a2; buf[126] = 0x6a3; buf[127] = 0x6a3;
        buf[128] = 0x7a4; buf[129] = 0x7a5; buf[130] = 0x9a6; buf[131] = 0x9a6;
        buf[132] = 0xaa7; buf[133] = 0xaa8; buf[134] = 0xca9; buf[135] = 0xca9;
        buf[136] = 0xeaa; buf[137] = 0xfab; buf[138] = 0x11ab; buf[139] = 0x11ac;
        buf[140] = 0x13ad; buf[141] = 0x15ae; buf[142] = 0x15ae; buf[143] = 0x17af;
        buf[144] = 0x19b0; buf[145] = 0x1bb0; buf[146] = 0x1bb1; buf[147] = 0x1db2;
        buf[148] = 0x1eb3; buf[149] = 0x21b3; buf[150] = 0x22b4; buf[151] = 0x25b5;
        buf[152] = 0x27b6; buf[153] = 0x28b6; buf[154] = 0x2ab7; buf[155] = 0x2bb8;
        buf[156] = 0x2eb8; buf[157] = 0x30b9; buf[158] = 0x31ba; buf[159] = 0x34bb;
        buf[160] = 0x36bb; buf[161] = 0x38bc; buf[162] = 0x3abd; buf[163] = 0x3cbe;
        buf[164] = 0x3ebe; buf[165] = 0x40bf; buf[166] = 0x41c0; buf[167] = 0x44c0;
        buf[168] = 0x46c1; buf[169] = 0x49c2; buf[170] = 0x4bc3; buf[171] = 0x4dc3;
        buf[172] = 0x4fc4; buf[173] = 0x51c5; buf[174] = 0x52c5; buf[175] = 0x55c6;
        buf[176] = 0x57c7; buf[177] = 0x59c8; buf[178] = 0x5cc8; buf[179] = 0x5ec9;
        buf[180] = 0x15fca; buf[181] = 0x62cb; buf[182] = 0x63cb; buf[183] = 0x167cc;
        buf[184] = 0x269cd; buf[185] = 0x26acd; buf[186] = 0x26dce; buf[187] = 0x36ecf;
        buf[188] = 0x371d0; buf[189] = 0x574d0; buf[190] = 0x676d1; buf[191] = 0x677d2;
        buf[192] = 0x77ad2; buf[193] = 0x87cd3; buf[194] = 0x97ed4; buf[195] = 0xa80d5;
        buf[196] = 0xc83d5; buf[197] = 0xe84d6; buf[198] = 0x1087d7; buf[199] = 0x1189d8;
        buf[200] = 0x128ad8; buf[201] = 0x158dd9; buf[202] = 0x178fda; buf[203] = 0x1992da;
        buf[204] = 0x1b94db; buf[205] = 0x1e96dc; buf[206] = 0x2198dd; buf[207] = 0x249add;
        buf[208] = 0x269cde; buf[209] = 0x2a9edf; buf[210] = 0x2da1df; buf[211] = 0x30a3e0;
        buf[212] = 0x33a5e1; buf[213] = 0x38a7e2; buf[214] = 0x3ca9e2; buf[215] = 0x3fabe3;
        buf[216] = 0x43aee4; buf[217] = 0x46b0e5; buf[218] = 0x4bb1e5; buf[219] = 0x50b4e6;
        buf[220] = 0x54b6e7; buf[221] = 0x58b8e7; buf[222] = 0x5cb9e8; buf[223] = 0x62bde9;
        buf[224] = 0x65bfea; buf[225] = 0x6bc1ea; buf[226] = 0x70c3eb; buf[227] = 0x74c5ec;
        buf[228] = 0x79c7ec; buf[229] = 0x7dc9ed; buf[230] = 0x83ccee; buf[231] = 0x87ceef;
        buf[232] = 0x8dd0ef; buf[233] = 0x92d2f0; buf[234] = 0x98d4f1; buf[235] = 0x9dd6f2;
        buf[236] = 0xa2d8f2; buf[237] = 0xa7dbf3; buf[238] = 0xacddf4; buf[239] = 0xb0dff4;
        buf[240] = 0xb6e1f5; buf[241] = 0xbce3f6; buf[242] = 0xc1e5f7; buf[243] = 0xc6e8f7;
        buf[244] = 0xcbeaf8; buf[245] = 0xcfecf9; buf[246] = 0xd6eef9; buf[247] = 0xdbf0fa;
        buf[248] = 0xe0f2fb; buf[249] = 0xe4f3fb; buf[250] = 0xeaf5fb; buf[251] = 0xeff7fc;
        buf[252] = 0xf3f9fc; buf[253] = 0xf8fbfd; buf[254] = 0xfefefe; buf[255] = 0xffffff;
        break;

    case TYPE_ICE : /* Ice.pal */
        buf[0] = 0; buf[1] = 0x10000; buf[2] = 0x20000; buf[3] = 0x40000;
        buf[4] = 0x50000; buf[5] = 0x60000; buf[6] = 0x80000; buf[7] = 0x90000;
        buf[8] = 0xa0000; buf[9] = 0xc0000; buf[10] = 0xd0000; buf[11] = 0xe0000;
        buf[12] = 0x100000; buf[13] = 0x110000; buf[14] = 0x120000; buf[15] = 0x140000;
        buf[16] = 0x150000; buf[17] = 0x170000; buf[18] = 0x180000; buf[19] = 0x190000;
        buf[20] = 0x1b0000; buf[21] = 0x1c0000; buf[22] = 0x1d0000; buf[23] = 0x1f0000;
        buf[24] = 0x200000; buf[25] = 0x210000; buf[26] = 0x230000; buf[27] = 0x240000;
        buf[28] = 0x250000; buf[29] = 0x270000; buf[30] = 0x280000; buf[31] = 0x2a0000;
        buf[32] = 0x2b0000; buf[33] = 0x2c0000; buf[34] = 0x2e0000; buf[35] = 0x2f0000;
        buf[36] = 0x300000; buf[37] = 0x320000; buf[38] = 0x330000; buf[39] = 0x340000;
        buf[40] = 0x360000; buf[41] = 0x370000; buf[42] = 0x380000; buf[43] = 0x3a0000;
        buf[44] = 0x3b0000; buf[45] = 0x3d0000; buf[46] = 0x3e0000; buf[47] = 0x3f0000;
        buf[48] = 0x410000; buf[49] = 0x420000; buf[50] = 0x430000; buf[51] = 0x450000;
        buf[52] = 0x460000; buf[53] = 0x470000; buf[54] = 0x490000; buf[55] = 0x4a0000;
        buf[56] = 0x4b0000; buf[57] = 0x4d0000; buf[58] = 0x4e0000; buf[59] = 0x500000;
        buf[60] = 0x510000; buf[61] = 0x520000; buf[62] = 0x540000; buf[63] = 0x550000;
        buf[64] = 0x560000; buf[65] = 0x580000; buf[66] = 0x590000; buf[67] = 0x5a0000;
        buf[68] = 0x5c0000; buf[69] = 0x5d0000; buf[70] = 0x5e0000; buf[71] = 0x600000;
        buf[72] = 0x610000; buf[73] = 0x630000; buf[74] = 0x640000; buf[75] = 0x650000;
        buf[76] = 0x670000; buf[77] = 0x680000; buf[78] = 0x690000; buf[79] = 0x6b0000;
        buf[80] = 0x6c0000; buf[81] = 0x6d0000; buf[82] = 0x6f0000; buf[83] = 0x700000;
        buf[84] = 0x710000; buf[85] = 0x730000; buf[86] = 0x740000; buf[87] = 0x760000;
        buf[88] = 0x770000; buf[89] = 0x780000; buf[90] = 0x7a0000; buf[91] = 0x7b0000;
        buf[92] = 0x7c0000; buf[93] = 0x7e0000; buf[94] = 0x7f0000; buf[95] = 0x800000;
        buf[96] = 0x820000; buf[97] = 0x830200; buf[98] = 0x840400; buf[99] = 0x860500;
        buf[100] = 0x870700; buf[101] = 0x880900; buf[102] = 0x8a0a00; buf[103] = 0x8b0c00;
        buf[104] = 0x8d0d00; buf[105] = 0x8e0f00; buf[106] = 0x8f1100; buf[107] = 0x911200;
        buf[108] = 0x921400; buf[109] = 0x931500; buf[110] = 0x951700; buf[111] = 0x961900;
        buf[112] = 0x971a00; buf[113] = 0x991c00; buf[114] = 0x9a1d00; buf[115] = 0x9b1f00;
        buf[116] = 0x9d2100; buf[117] = 0x9e2200; buf[118] = 0xa02400; buf[119] = 0xa12500;
        buf[120] = 0xa22700; buf[121] = 0xa42900; buf[122] = 0xa52a00; buf[123] = 0xa62c00;
        buf[124] = 0xa82d00; buf[125] = 0xa92f00; buf[126] = 0xaa3100; buf[127] = 0xac3200;
        buf[128] = 0xad3400; buf[129] = 0xae3500; buf[130] = 0xb03700; buf[131] = 0xb13900;
        buf[132] = 0xb33a00; buf[133] = 0xb43c00; buf[134] = 0xb53d00; buf[135] = 0xb73f00;
        buf[136] = 0xb84100; buf[137] = 0xb94200; buf[138] = 0xbb4400; buf[139] = 0xbc4500;
        buf[140] = 0xbd4700; buf[141] = 0xbf4900; buf[142] = 0xc04a00; buf[143] = 0xc14c00;
        buf[144] = 0xc34d00; buf[145] = 0xc44f00; buf[146] = 0xc65100; buf[147] = 0xc75200;
        buf[148] = 0xc85400; buf[149] = 0xca5500; buf[150] = 0xcb5700; buf[151] = 0xcc5900;
        buf[152] = 0xce5a00; buf[153] = 0xcf5c00; buf[154] = 0xd05e00; buf[155] = 0xd25f00;
        buf[156] = 0xd36100; buf[157] = 0xd46200; buf[158] = 0xd66400; buf[159] = 0xd76600;
        buf[160] = 0xd96700; buf[161] = 0xda6900; buf[162] = 0xdb6a00; buf[163] = 0xdd6c00;
        buf[164] = 0xde6e00; buf[165] = 0xdf6f00; buf[166] = 0xe17100; buf[167] = 0xe27200;
        buf[168] = 0xe37400; buf[169] = 0xe57600; buf[170] = 0xe67700; buf[171] = 0xe77900;
        buf[172] = 0xe97a00; buf[173] = 0xea7c00; buf[174] = 0xec7e00; buf[175] = 0xed7f00;
        buf[176] = 0xee8100; buf[177] = 0xf08200; buf[178] = 0xf18400; buf[179] = 0xf28600;
        buf[180] = 0xf48700; buf[181] = 0xf58900; buf[182] = 0xf68a00; buf[183] = 0xf88c00;
        buf[184] = 0xf98e00; buf[185] = 0xfa8f00; buf[186] = 0xfc9100; buf[187] = 0xfd9200;
        buf[188] = 0xff9400; buf[189] = 0xff9600; buf[190] = 0xff9700; buf[191] = 0xff9900;
        buf[192] = 0xff9a00; buf[193] = 0xff9c05; buf[194] = 0xff9e09; buf[195] = 0xff9f0d;
        buf[196] = 0xffa111; buf[197] = 0xffa215; buf[198] = 0xffa419; buf[199] = 0xffa61d;
        buf[200] = 0xffa721; buf[201] = 0xffa925; buf[202] = 0xffaa29; buf[203] = 0xffac2d;
        buf[204] = 0xffae31; buf[205] = 0xffaf35; buf[206] = 0xffb139; buf[207] = 0xffb33d;
        buf[208] = 0xffb441; buf[209] = 0xffb645; buf[210] = 0xffb749; buf[211] = 0xffb94d;
        buf[212] = 0xffbb51; buf[213] = 0xffbc55; buf[214] = 0xffbe5a; buf[215] = 0xffbf5e;
        buf[216] = 0xffc162; buf[217] = 0xffc366; buf[218] = 0xffc46a; buf[219] = 0xffc66e;
        buf[220] = 0xffc772; buf[221] = 0xffc976; buf[222] = 0xffcb7a; buf[223] = 0xffcc7e;
        buf[224] = 0xffce82; buf[225] = 0xffcf86; buf[226] = 0xffd18a; buf[227] = 0xffd38e;
        buf[228] = 0xffd492; buf[229] = 0xffd696; buf[230] = 0xffd79a; buf[231] = 0xffd99e;
        buf[232] = 0xffdba2; buf[233] = 0xffdca6; buf[234] = 0xffdeaa; buf[235] = 0xffdfaf;
        buf[236] = 0xffe1b3; buf[237] = 0xffe3b7; buf[238] = 0xffe4bb; buf[239] = 0xffe6bf;
        buf[240] = 0xffe7c3; buf[241] = 0xffe9c7; buf[242] = 0xffebcb; buf[243] = 0xffeccf;
        buf[244] = 0xffeed3; buf[245] = 0xffefd7; buf[246] = 0xfff1db; buf[247] = 0xfff3df;
        buf[248] = 0xfff4e3; buf[249] = 0xfff6e7; buf[250] = 0xfff7eb; buf[251] = 0xfff9ef;
        buf[252] = 0xfffbf3; buf[253] = 0xfffcf7; buf[254] = 0xfffefb; buf[255] = 0xffffff;
        break;

    case TYPE_LIGHTHUES :/* Lighthues.pal */
        buf[0] = 0xff; buf[1] = 0x6ff; buf[2] = 0x10dff; buf[3] = 0x214ff;
        buf[4] = 0x31bff; buf[5] = 0x422ff; buf[6] = 0x529ff; buf[7] = 0x62fff;
        buf[8] = 0x736ff; buf[9] = 0x83dff; buf[10] = 0x943ff; buf[11] = 0xa4aff;
        buf[12] = 0xb50ff; buf[13] = 0xc57ff; buf[14] = 0xd5dff; buf[15] = 0xe63ff;
        buf[16] = 0xf69ff; buf[17] = 0x1070ff; buf[18] = 0x1176ff; buf[19] = 0x127cff;
        buf[20] = 0x1382ff; buf[21] = 0x1488ff; buf[22] = 0x158eff; buf[23] = 0x1694ff;
        buf[24] = 0x179aff; buf[25] = 0x18a0ff; buf[26] = 0x19a6ff; buf[27] = 0x1aabff;
        buf[28] = 0x1bb1ff; buf[29] = 0x1cb7ff; buf[30] = 0x1dbcff; buf[31] = 0x1ec2ff;
        buf[32] = 0x1fc7ff; buf[33] = 0x20cdff; buf[34] = 0x21d2ff; buf[35] = 0x22d8ff;
        buf[36] = 0x23ddff; buf[37] = 0x24e2ff; buf[38] = 0x25e8ff; buf[39] = 0x26edff;
        buf[40] = 0x27f2ff; buf[41] = 0x28f7ff; buf[42] = 0x29fcff; buf[43] = 0x2afffc;
        buf[44] = 0x2bfff7; buf[45] = 0x2cfff2; buf[46] = 0x2dffed; buf[47] = 0x2effe8;
        buf[48] = 0x2fffe4; buf[49] = 0x30ffdf; buf[50] = 0x31ffda; buf[51] = 0x32ffd6;
        buf[52] = 0x33ffd1; buf[53] = 0x34ffcd; buf[54] = 0x35ffc8; buf[55] = 0x36ffc4;
        buf[56] = 0x37ffbf; buf[57] = 0x38ffbb; buf[58] = 0x39ffb7; buf[59] = 0x3affb2;
        buf[60] = 0x3bffae; buf[61] = 0x3cffaa; buf[62] = 0x3dffa6; buf[63] = 0x3effa2;
        buf[64] = 0x3fff9e; buf[65] = 0x40ff9a; buf[66] = 0x41ff96; buf[67] = 0x42ff92;
        buf[68] = 0x43ff8e; buf[69] = 0x44ff8b; buf[70] = 0x45ff87; buf[71] = 0x46ff83;
        buf[72] = 0x47ff7f; buf[73] = 0x48ff7c; buf[74] = 0x49ff78; buf[75] = 0x4aff75;
        buf[76] = 0x4bff71; buf[77] = 0x4cff6e; buf[78] = 0x4dff6b; buf[79] = 0x4eff67;
        buf[80] = 0x4fff64; buf[81] = 0x50ff61; buf[82] = 0x51ff5e; buf[83] = 0x52ff5b;
        buf[84] = 0x53ff58; buf[85] = 0x54ff55; buf[86] = 0x59ff55; buf[87] = 0x5eff56;
        buf[88] = 0x63ff57; buf[89] = 0x68ff58; buf[90] = 0x6dff59; buf[91] = 0x72ff5a;
        buf[92] = 0x76ff5b; buf[93] = 0x7bff5c; buf[94] = 0x80ff5d; buf[95] = 0x84ff5e;
        buf[96] = 0x89ff5f; buf[97] = 0x8dff60; buf[98] = 0x92ff61; buf[99] = 0x96ff62;
        buf[100] = 0x9aff63; buf[101] = 0x9eff64; buf[102] = 0xa3ff65; buf[103] = 0xa7ff66;
        buf[104] = 0xabff67; buf[105] = 0xafff68; buf[106] = 0xb3ff69; buf[107] = 0xb7ff6a;
        buf[108] = 0xbbff6b; buf[109] = 0xbfff6c; buf[110] = 0xc3ff6d; buf[111] = 0xc7ff6e;
        buf[112] = 0xcaff6f; buf[113] = 0xceff70; buf[114] = 0xd2ff71; buf[115] = 0xd5ff72;
        buf[116] = 0xd9ff73; buf[117] = 0xdcff74; buf[118] = 0xe0ff75; buf[119] = 0xe3ff76;
        buf[120] = 0xe7ff77; buf[121] = 0xeaff78; buf[122] = 0xedff79; buf[123] = 0xf1ff7a;
        buf[124] = 0xf4ff7b; buf[125] = 0xf7ff7c; buf[126] = 0xfaff7d; buf[127] = 0xfdff7e;
        buf[128] = 0xfffd7f; buf[129] = 0xfffa80; buf[130] = 0xfff781; buf[131] = 0xfff482;
        buf[132] = 0xfff183; buf[133] = 0xffef84; buf[134] = 0xffec85; buf[135] = 0xffe986;
        buf[136] = 0xffe787; buf[137] = 0xffe488; buf[138] = 0xffe289; buf[139] = 0xffdf8a;
        buf[140] = 0xffdd8b; buf[141] = 0xffda8c; buf[142] = 0xffd88d; buf[143] = 0xffd68e;
        buf[144] = 0xffd38f; buf[145] = 0xffd190; buf[146] = 0xffcf91; buf[147] = 0xffcd92;
        buf[148] = 0xffcb93; buf[149] = 0xffc994; buf[150] = 0xffc795; buf[151] = 0xffc596;
        buf[152] = 0xffc397; buf[153] = 0xffc198; buf[154] = 0xffc099; buf[155] = 0xffbe9a;
        buf[156] = 0xffbc9b; buf[157] = 0xffba9c; buf[158] = 0xffb99d; buf[159] = 0xffb79e;
        buf[160] = 0xffb69f; buf[161] = 0xffb4a0; buf[162] = 0xffb3a1; buf[163] = 0xffb2a2;
        buf[164] = 0xffb0a3; buf[165] = 0xffafa4; buf[166] = 0xffaea5; buf[167] = 0xffada6;
        buf[168] = 0xffaca7; buf[169] = 0xffaba8; buf[170] = 0xffaaa9; buf[171] = 0xffaaac;
        buf[172] = 0xffabaf; buf[173] = 0xffacb2; buf[174] = 0xffadb5; buf[175] = 0xffaeb8;
        buf[176] = 0xffafbb; buf[177] = 0xffb0bd; buf[178] = 0xffb1c0; buf[179] = 0xffb2c3;
        buf[180] = 0xffb3c5; buf[181] = 0xffb4c8; buf[182] = 0xffb5ca; buf[183] = 0xffb6cd;
        buf[184] = 0xffb7cf; buf[185] = 0xffb8d1; buf[186] = 0xffb9d3; buf[187] = 0xffbad6;
        buf[188] = 0xffbbd8; buf[189] = 0xffbcda; buf[190] = 0xffbddc; buf[191] = 0xffbede;
        buf[192] = 0xffbfe0; buf[193] = 0xffc0e2; buf[194] = 0xffc1e4; buf[195] = 0xffc2e6;
        buf[196] = 0xffc3e8; buf[197] = 0xffc4e9; buf[198] = 0xffc5eb; buf[199] = 0xffc6ed;
        buf[200] = 0xffc7ee; buf[201] = 0xffc8f0; buf[202] = 0xffc9f1; buf[203] = 0xffcaf3;
        buf[204] = 0xffcbf4; buf[205] = 0xffccf6; buf[206] = 0xffcdf7; buf[207] = 0xffcef8;
        buf[208] = 0xffcffa; buf[209] = 0xffd0fb; buf[210] = 0xffd1fc; buf[211] = 0xffd2fd;
        buf[212] = 0xffd3fe; buf[213] = 0xfed4ff; buf[214] = 0xfdd5ff; buf[215] = 0xfcd6ff;
        buf[216] = 0xfbd7ff; buf[217] = 0xfad8ff; buf[218] = 0xfad9ff; buf[219] = 0xf9daff;
        buf[220] = 0xf8dbff; buf[221] = 0xf8dcff; buf[222] = 0xf7ddff; buf[223] = 0xf7deff;
        buf[224] = 0xf6dfff; buf[225] = 0xf6e0ff; buf[226] = 0xf5e1ff; buf[227] = 0xf5e2ff;
        buf[228] = 0xf5e3ff; buf[229] = 0xf4e4ff; buf[230] = 0xf4e5ff; buf[231] = 0xf4e6ff;
        buf[232] = 0xf4e7ff; buf[233] = 0xf4e8ff; buf[234] = 0xf4e9ff; buf[235] = 0xf4eaff;
        buf[236] = 0xf4ebff; buf[237] = 0xf4ecff; buf[238] = 0xf4edff; buf[239] = 0xf5eeff;
        buf[240] = 0xf5efff; buf[241] = 0xf5f0ff; buf[242] = 0xf5f1ff; buf[243] = 0xf6f2ff;
        buf[244] = 0xf6f3ff; buf[245] = 0xf7f4ff; buf[246] = 0xf7f5ff; buf[247] = 0xf8f6ff;
        buf[248] = 0xf8f7fe; buf[249] = 0xf9f8ff; buf[250] = 0xfaf9ff; buf[251] = 0xfbfaff;
        buf[252] = 0xfcfbff; buf[253] = 0xfdfcff; buf[254] = 0xfefdff; buf[255] = 0xfefeff;
        break;

    case TYPE_RICH : /* Rich.pal */
        buf[0] = 0x7e0000; buf[1] = 0x830000; buf[2] = 0x870000; buf[3] = 0x8b0000;
        buf[4] = 0x8f0000; buf[5] = 0x930000; buf[6] = 0x970000; buf[7] = 0x9b0000;
        buf[8] = 0x9f0000; buf[9] = 0xa30000; buf[10] = 0xa70000; buf[11] = 0xab0000;
        buf[12] = 0xaf0000; buf[13] = 0xb30000; buf[14] = 0xb70000; buf[15] = 0xbb0000;
        buf[16] = 0xbf0000; buf[17] = 0xc30000; buf[18] = 0xc70000; buf[19] = 0xcb0000;
        buf[20] = 0xcf0000; buf[21] = 0xd30000; buf[22] = 0xd70000; buf[23] = 0xdb0000;
        buf[24] = 0xdf0000; buf[25] = 0xe30000; buf[26] = 0xe70000; buf[27] = 0xeb0000;
        buf[28] = 0xef0000; buf[29] = 0xf30000; buf[30] = 0xf70100; buf[31] = 0xfb0200;
        buf[32] = 0xff0300; buf[33] = 0xff0500; buf[34] = 0xff0700; buf[35] = 0xff0a00;
        buf[36] = 0xff0d00; buf[37] = 0xff1000; buf[38] = 0xff1300; buf[39] = 0xff1700;
        buf[40] = 0xff1b00; buf[41] = 0xff1f00; buf[42] = 0xff2300; buf[43] = 0xff2700;
        buf[44] = 0xff2b00; buf[45] = 0xff2f00; buf[46] = 0xff3300; buf[47] = 0xff3700;
        buf[48] = 0xff3b00; buf[49] = 0xff3f00; buf[50] = 0xff4300; buf[51] = 0xff4700;
        buf[52] = 0xff4b00; buf[53] = 0xff4f00; buf[54] = 0xff5300; buf[55] = 0xff5700;
        buf[56] = 0xff5b00; buf[57] = 0xff5f00; buf[58] = 0xff6300; buf[59] = 0xff6700;
        buf[60] = 0xff6b00; buf[61] = 0xff6f00; buf[62] = 0xff7300; buf[63] = 0xff7700;
        buf[64] = 0xff7b00; buf[65] = 0xff7f00; buf[66] = 0xff8300; buf[67] = 0xff8600;
        buf[68] = 0xff8a00; buf[69] = 0xff8e00; buf[70] = 0xff9200; buf[71] = 0xff9600;
        buf[72] = 0xff9a00; buf[73] = 0xff9e00; buf[74] = 0xffa200; buf[75] = 0xffa600;
        buf[76] = 0xffaa00; buf[77] = 0xffae00; buf[78] = 0xffb200; buf[79] = 0xffb600;
        buf[80] = 0xffba00; buf[81] = 0xffbe00; buf[82] = 0xffc200; buf[83] = 0xffc600;
        buf[84] = 0xffca00; buf[85] = 0xffce00; buf[86] = 0xffd200; buf[87] = 0xffd600;
        buf[88] = 0xffda00; buf[89] = 0xffde00; buf[90] = 0xffe200; buf[91] = 0xffe600;
        buf[92] = 0xffea00; buf[93] = 0xffee00; buf[94] = 0xfff200; buf[95] = 0xfff600;
        buf[96] = 0xfff900; buf[97] = 0xfffd00; buf[98] = 0xfcff00; buf[99] = 0xf8ff02;
        buf[100] = 0xf5ff07; buf[101] = 0xf1ff0a; buf[102] = 0xedff0f; buf[103] = 0xe9ff12;
        buf[104] = 0xe5ff17; buf[105] = 0xe1ff1a; buf[106] = 0xddff1f; buf[107] = 0xd9ff22;
        buf[108] = 0xd5ff27; buf[109] = 0xd1ff2b; buf[110] = 0xcdff2f; buf[111] = 0xc9ff32;
        buf[112] = 0xc5ff37; buf[113] = 0xc1ff3b; buf[114] = 0xbdff3f; buf[115] = 0xb9ff42;
        buf[116] = 0xb5ff47; buf[117] = 0xb1ff4b; buf[118] = 0xadff4f; buf[119] = 0xa9ff52;
        buf[120] = 0xa4ff57; buf[121] = 0xa0ff5b; buf[122] = 0x9cff5f; buf[123] = 0x98ff62;
        buf[124] = 0x94ff67; buf[125] = 0x90ff6b; buf[126] = 0x8cff6f; buf[127] = 0x88ff72;
        buf[128] = 0x85ff77; buf[129] = 0x81ff7b; buf[130] = 0x7fff7f; buf[131] = 0x7bff81;
        buf[132] = 0x77ff85; buf[133] = 0x72ff88; buf[134] = 0x6fff8c; buf[135] = 0x6bff90;
        buf[136] = 0x67ff94; buf[137] = 0x62ff98; buf[138] = 0x5fff9c; buf[139] = 0x5bffa0;
        buf[140] = 0x57ffa4; buf[141] = 0x52ffa9; buf[142] = 0x4fffad; buf[143] = 0x4bffb1;
        buf[144] = 0x47ffb5; buf[145] = 0x42ffb9; buf[146] = 0x3fffbd; buf[147] = 0x3bffc1;
        buf[148] = 0x37ffc5; buf[149] = 0x32ffc9; buf[150] = 0x2fffcd; buf[151] = 0x2bffd1;
        buf[152] = 0x27ffd5; buf[153] = 0x22ffd9; buf[154] = 0x1fffdd; buf[155] = 0x1affe1;
        buf[156] = 0x17ffe5; buf[157] = 0x12ffe9; buf[158] = 0xfffee; buf[159] = 0xafff2;
        buf[160] = 0x7fff6; buf[161] = 0x2fffa; buf[162] = 0xfffe; buf[163] = 0xfbff;
        buf[164] = 0xf7ff; buf[165] = 0xf3ff; buf[166] = 0xefff; buf[167] = 0xebff;
        buf[168] = 0xe7ff; buf[169] = 0xe3ff; buf[170] = 0xdfff; buf[171] = 0xdbff;
        buf[172] = 0xd7ff; buf[173] = 0xd3ff; buf[174] = 0xcfff; buf[175] = 0xcbff;
        buf[176] = 0xc7ff; buf[177] = 0xc3ff; buf[178] = 0xbfff; buf[179] = 0xbbff;
        buf[180] = 0xb7ff; buf[181] = 0xb3ff; buf[182] = 0xafff; buf[183] = 0xabff;
        buf[184] = 0xa7ff; buf[185] = 0xa3ff; buf[186] = 0x9fff; buf[187] = 0x9bff;
        buf[188] = 0x96ff; buf[189] = 0x93ff; buf[190] = 0x8fff; buf[191] = 0x8bff;
        buf[192] = 0x87ff; buf[193] = 0x83ff; buf[194] = 0x7eff; buf[195] = 0x7aff;
        buf[196] = 0x77ff; buf[197] = 0x72ff; buf[198] = 0x6fff; buf[199] = 0x6bff;
        buf[200] = 0x67ff; buf[201] = 0x62ff; buf[202] = 0x5fff; buf[203] = 0x5bff;
        buf[204] = 0x57ff; buf[205] = 0x53ff; buf[206] = 0x4fff; buf[207] = 0x4bff;
        buf[208] = 0x47ff; buf[209] = 0x43ff; buf[210] = 0x3fff; buf[211] = 0x3bff;
        buf[212] = 0x37ff; buf[213] = 0x33ff; buf[214] = 0x2fff; buf[215] = 0x2bff;
        buf[216] = 0x27ff; buf[217] = 0x23ff; buf[218] = 0x1fff; buf[219] = 0x1bff;
        buf[220] = 0x17ff; buf[221] = 0x13ff; buf[222] = 0x10ff; buf[223] = 0xdff;
        buf[224] = 0xaff; buf[225] = 0x7ff; buf[226] = 0x5ff; buf[227] = 0x3fa;
        buf[228] = 0x2f6; buf[229] = 0x1f1; buf[230] = 0xed; buf[231] = 0xe9;
        buf[232] = 0xe4; buf[233] = 0xe0; buf[234] = 0xdb; buf[235] = 0xd7;
        buf[236] = 0xd3; buf[237] = 0xce; buf[238] = 0xca; buf[239] = 0xc5;
        buf[240] = 0xc1; buf[241] = 0xbd; buf[242] = 0xb8; buf[243] = 0xb4;
        buf[244] = 0xaf; buf[245] = 0xab; buf[246] = 0xa7; buf[247] = 0xa2;
        buf[248] = 0x9e; buf[249] = 0x99; buf[250] = 0x95; buf[251] = 0x91;
        buf[252] = 0x8c; buf[253] = 0x88; buf[254] = 0x83; buf[255] = 0x7d;
        break;

    case TYPE_SMOOTHRICH :/* Smoothrich.pal */
        buf[0] = 0x7d0000; buf[1] = 0x830000; buf[2] = 0x870000; buf[3] = 0x8b0000;
        buf[4] = 0x8f0000; buf[5] = 0x930000; buf[6] = 0x970000; buf[7] = 0x9a0000;
        buf[8] = 0x9e0000; buf[9] = 0xa20000; buf[10] = 0xa60000; buf[11] = 0xaa0000;
        buf[12] = 0xae0000; buf[13] = 0xb20000; buf[14] = 0xb60000; buf[15] = 0xba0000;
        buf[16] = 0xbd0000; buf[17] = 0xc10000; buf[18] = 0xc50100; buf[19] = 0xc80000;
        buf[20] = 0xcc0000; buf[21] = 0xcf0200; buf[22] = 0xd30300; buf[23] = 0xd60100;
        buf[24] = 0xd90200; buf[25] = 0xdc0500; buf[26] = 0xdf0600; buf[27] = 0xe20500;
        buf[28] = 0xe40800; buf[29] = 0xe70800; buf[30] = 0xe90a00; buf[31] = 0xeb0c00;
        buf[32] = 0xed0d00; buf[33] = 0xef1000; buf[34] = 0xf11000; buf[35] = 0xf31400;
        buf[36] = 0xf41700; buf[37] = 0xf61800; buf[38] = 0xf71b00; buf[39] = 0xf81e00;
        buf[40] = 0xf91f00; buf[41] = 0xfa2400; buf[42] = 0xfb2500; buf[43] = 0xfb2900;
        buf[44] = 0xfc2c00; buf[45] = 0xfc3100; buf[46] = 0xfd3500; buf[47] = 0xfd3800;
        buf[48] = 0xfd3c00; buf[49] = 0xfe4000; buf[50] = 0xfe4300; buf[51] = 0xfe4600;
        buf[52] = 0xfe4900; buf[53] = 0xfe4f00; buf[54] = 0xfe5200; buf[55] = 0xfe5500;
        buf[56] = 0xfe5b00; buf[57] = 0xfe5f00; buf[58] = 0xfe6300; buf[59] = 0xfe6700;
        buf[60] = 0xfe6b00; buf[61] = 0xfe6f00; buf[62] = 0xff7300; buf[63] = 0xff7700;
        buf[64] = 0xff7b00; buf[65] = 0xff7f00; buf[66] = 0xff8300; buf[67] = 0xff8500;
        buf[68] = 0xff8800; buf[69] = 0xfe8c00; buf[70] = 0xfe9000; buf[71] = 0xfe9400;
        buf[72] = 0xfe9800; buf[73] = 0xfe9c00; buf[74] = 0xfea000; buf[75] = 0xfea600;
        buf[76] = 0xfea800; buf[77] = 0xfeae00; buf[78] = 0xfeb200; buf[79] = 0xfeb500;
        buf[80] = 0xfeba00; buf[81] = 0xfebd00; buf[82] = 0xfdc100; buf[83] = 0xfdc400;
        buf[84] = 0xfdc600; buf[85] = 0xfccb00; buf[86] = 0xfcce00; buf[87] = 0xfbd201;
        buf[88] = 0xfbd401; buf[89] = 0xfad904; buf[90] = 0xf9dc05; buf[91] = 0xf8dd05;
        buf[92] = 0xf7e207; buf[93] = 0xf6e306; buf[94] = 0xf4e508; buf[95] = 0xf3e809;
        buf[96] = 0xf1ea0c; buf[97] = 0xefeb0e; buf[98] = 0xedef0f; buf[99] = 0xeaf110;
        buf[100] = 0xe8f312; buf[101] = 0xe7f417; buf[102] = 0xe3f617; buf[103] = 0xe1f71a;
        buf[104] = 0xdff81e; buf[105] = 0xdcf920; buf[106] = 0xd9fa24; buf[107] = 0xd5fb26;
        buf[108] = 0xd1fb29; buf[109] = 0xcffc2e; buf[110] = 0xccfc30; buf[111] = 0xc7fd35;
        buf[112] = 0xc3fd36; buf[113] = 0xbffd3c; buf[114] = 0xbdfe40; buf[115] = 0xb9fe41;
        buf[116] = 0xb6fe47; buf[117] = 0xb0fe4b; buf[118] = 0xadfe4f; buf[119] = 0xaafe51;
        buf[120] = 0xa4fe57; buf[121] = 0xa2fe5b; buf[122] = 0x9efe5f; buf[123] = 0x99fe61;
        buf[124] = 0x96fe67; buf[125] = 0x92fe6b; buf[126] = 0x8dfe6f; buf[127] = 0x88ff71;
        buf[128] = 0x87ff77; buf[129] = 0x83ff7b; buf[130] = 0x7fff7f; buf[131] = 0x7bff83;
        buf[132] = 0x77ff87; buf[133] = 0x71fe8a; buf[134] = 0x6ffe8e; buf[135] = 0x6bfe92;
        buf[136] = 0x67fe96; buf[137] = 0x61fe98; buf[138] = 0x5ffe9e; buf[139] = 0x5bfea2;
        buf[140] = 0x57fea4; buf[141] = 0x51feaa; buf[142] = 0x4ffead; buf[143] = 0x4bfeb2;
        buf[144] = 0x47feb6; buf[145] = 0x41feb9; buf[146] = 0x40fdbd; buf[147] = 0x3cfdc0;
        buf[148] = 0x36fdc4; buf[149] = 0x35fcc8; buf[150] = 0x30fccc; buf[151] = 0x2efbcf;
        buf[152] = 0x29fbd1; buf[153] = 0x27fad5; buf[154] = 0x24f9d9; buf[155] = 0x20f8db;
        buf[156] = 0x1ef7df; buf[157] = 0x1af6e1; buf[158] = 0x18f4e3; buf[159] = 0x17f3e7;
        buf[160] = 0x12f1e8; buf[161] = 0x11efeb; buf[162] = 0xfeded; buf[163] = 0xee9ef;
        buf[164] = 0xce9f1; buf[165] = 0x9e7f3; buf[166] = 0x8e3f4; buf[167] = 0x6e2f6;
        buf[168] = 0x7dff7; buf[169] = 0x5dcf8; buf[170] = 0x5d9f9; buf[171] = 0x4d6fa;
        buf[172] = 0x1d3fb; buf[173] = 0x1cffb; buf[174] = 0xccfc; buf[175] = 0xc8fc;
        buf[176] = 0xc4fd; buf[177] = 0xc1fd; buf[178] = 0xbdfd; buf[179] = 0xbafe;
        buf[180] = 0xb6fe; buf[181] = 0xb2fe; buf[182] = 0xaefe; buf[183] = 0xaafe;
        buf[184] = 0xa6fe; buf[185] = 0xa2fe; buf[186] = 0x9efe; buf[187] = 0x9afe;
        buf[188] = 0x96fe; buf[189] = 0x92fe; buf[190] = 0x8efe; buf[191] = 0x8afe;
        buf[192] = 0x87ff; buf[193] = 0x83ff; buf[194] = 0x7fff; buf[195] = 0x7bff;
        buf[196] = 0x77ff; buf[197] = 0x73fe; buf[198] = 0x6ffe; buf[199] = 0x6bfe;
        buf[200] = 0x66fe; buf[201] = 0x63fe; buf[202] = 0x5ffe; buf[203] = 0x5bfe;
        buf[204] = 0x57fe; buf[205] = 0x53fe; buf[206] = 0x4ffe; buf[207] = 0x4bfe;
        buf[208] = 0x45fe; buf[209] = 0x41fe; buf[210] = 0x40fd; buf[211] = 0x3afd;
        buf[212] = 0x38fd; buf[213] = 0x33fc; buf[214] = 0x30fc; buf[215] = 0x2efb;
        buf[216] = 0x2bfa; buf[217] = 0x26f9; buf[218] = 0x22f8; buf[219] = 0x21f7;
        buf[220] = 0x1ef6; buf[221] = 0x1cf5; buf[222] = 0x17f3; buf[223] = 0x16f1;
        buf[224] = 0x14f0; buf[225] = 0x10ee; buf[226] = 0x10ec; buf[227] = 0xde9;
        buf[228] = 0xce7; buf[229] = 0xbe4; buf[230] = 0x9e1; buf[231] = 0x6df;
        buf[232] = 0x7db; buf[233] = 0x6d8; buf[234] = 0x5d5; buf[235] = 0x4d2;
        buf[236] = 0x1ce; buf[237] = 0x3ca; buf[238] = 0x1c6; buf[239] = 0xc3;
        buf[240] = 0xbf; buf[241] = 0x1bb; buf[242] = 0xb7; buf[243] = 0xb2;
        buf[244] = 0xae; buf[245] = 0xaa; buf[246] = 0xa6; buf[247] = 0xa2;
        buf[248] = 0x9d; buf[249] = 0x99; buf[250] = 0x94; buf[251] = 0x90;
        buf[252] = 0x8c; buf[253] = 0x87; buf[254] = 0x83; buf[255] = 0x7f;
        break;

    case TYPE_LUT16 : /* lut16.pal */
        buf[0] = 0xffffff; buf[1] = 0xffffff; buf[2] = 0xffffff; buf[3] = 0xffffff;
        buf[4] = 0xffffff; buf[5] = 0xffffff; buf[6] = 0xffffff; buf[7] = 0xffffff;
        buf[8] = 0xffffff; buf[9] = 0xffffff; buf[10] = 0xffffff; buf[11] = 0xffffff;
        buf[12] = 0xffffff; buf[13] = 0xffffff; buf[14] = 0xffffff; buf[15] = 0xffffff;
        buf[16] = 0xff; buf[17] = 0xff; buf[18] = 0xff; buf[19] = 0xff;
        buf[20] = 0xff; buf[21] = 0xff; buf[22] = 0xff; buf[23] = 0xff;
        buf[24] = 0xff; buf[25] = 0xff; buf[26] = 0xff; buf[27] = 0xff;
        buf[28] = 0xff; buf[29] = 0xff; buf[30] = 0xff; buf[31] = 0xff;
        buf[32] = 0xc7; buf[33] = 0xc7; buf[34] = 0xc7; buf[35] = 0xc7;
        buf[36] = 0xc7; buf[37] = 0xc7; buf[38] = 0xc7; buf[39] = 0xc7;
        buf[40] = 0xc7; buf[41] = 0xc7; buf[42] = 0xc7; buf[43] = 0xc7;
        buf[44] = 0xc7; buf[45] = 0xc7; buf[46] = 0xc7; buf[47] = 0xc7;
        buf[48] = 0x95; buf[49] = 0x95; buf[50] = 0x95; buf[51] = 0x95;
        buf[52] = 0x95; buf[53] = 0x95; buf[54] = 0x95; buf[55] = 0x95;
        buf[56] = 0x95; buf[57] = 0x95; buf[58] = 0x95; buf[59] = 0x95;
        buf[60] = 0x95; buf[61] = 0x95; buf[62] = 0x95; buf[63] = 0x95;
        buf[64] = 0x95ff; buf[65] = 0x95ff; buf[66] = 0x95ff; buf[67] = 0x95ff;
        buf[68] = 0x95ff; buf[69] = 0x95ff; buf[70] = 0x95ff; buf[71] = 0x95ff;
        buf[72] = 0x95ff; buf[73] = 0x95ff; buf[74] = 0x95ff; buf[75] = 0x95ff;
        buf[76] = 0x95ff; buf[77] = 0x95ff; buf[78] = 0x95ff; buf[79] = 0x95ff;
        buf[80] = 0xffff; buf[81] = 0xffff; buf[82] = 0xffff; buf[83] = 0xffff;
        buf[84] = 0xffff; buf[85] = 0xffff; buf[86] = 0xffff; buf[87] = 0xffff;
        buf[88] = 0xffff; buf[89] = 0xffff; buf[90] = 0xffff; buf[91] = 0xffff;
        buf[92] = 0xffff; buf[93] = 0xffff; buf[94] = 0xffff; buf[95] = 0xffff;
        buf[96] = 0xff00; buf[97] = 0xff00; buf[98] = 0xff00; buf[99] = 0xff00;
        buf[100] = 0xff00; buf[101] = 0xff00; buf[102] = 0xff00; buf[103] = 0xff00;
        buf[104] = 0xff00; buf[105] = 0xff00; buf[106] = 0xff00; buf[107] = 0xff00;
        buf[108] = 0xff00; buf[109] = 0xff00; buf[110] = 0xff00; buf[111] = 0xff00;
        buf[112] = 0xae00; buf[113] = 0xae00; buf[114] = 0xae00; buf[115] = 0xae00;
        buf[116] = 0xae00; buf[117] = 0xae00; buf[118] = 0xae00; buf[119] = 0xae00;
        buf[120] = 0xae00; buf[121] = 0xae00; buf[122] = 0xae00; buf[123] = 0xae00;
        buf[124] = 0xae00; buf[125] = 0xae00; buf[126] = 0xae00; buf[127] = 0xae00;
        buf[128] = 0x97c09; buf[129] = 0x97c09; buf[130] = 0x97c09; buf[131] = 0x97c09;
        buf[132] = 0x97c09; buf[133] = 0x97c09; buf[134] = 0x97c09; buf[135] = 0x97c09;
        buf[136] = 0x97c09; buf[137] = 0x97c09; buf[138] = 0x97c09; buf[139] = 0x97c09;
        buf[140] = 0x97c09; buf[141] = 0x97c09; buf[142] = 0x97c09; buf[143] = 0x97c09;
        buf[144] = 0xffae00; buf[145] = 0xffae00; buf[146] = 0xffae00; buf[147] = 0xffae00;
        buf[148] = 0xffae00; buf[149] = 0xffae00; buf[150] = 0xffae00; buf[151] = 0xffae00;
        buf[152] = 0xffae00; buf[153] = 0xffae00; buf[154] = 0xffae00; buf[155] = 0xffae00;
        buf[156] = 0xffae00; buf[157] = 0xffae00; buf[158] = 0xffae00; buf[159] = 0xffae00;
        buf[160] = 0xff7c00; buf[161] = 0xff7c00; buf[162] = 0xff7c00; buf[163] = 0xff7c00;
        buf[164] = 0xff7c00; buf[165] = 0xff7c00; buf[166] = 0xff7c00; buf[167] = 0xff7c00;
        buf[168] = 0xff7c00; buf[169] = 0xff7c00; buf[170] = 0xff7c00; buf[171] = 0xff7c00;
        buf[172] = 0xff7c00; buf[173] = 0xff7c00; buf[174] = 0xff7c00; buf[175] = 0xff7c00;
        buf[176] = 0xff0000; buf[177] = 0xff0000; buf[178] = 0xff0000; buf[179] = 0xff0000;
        buf[180] = 0xff0000; buf[181] = 0xff0000; buf[182] = 0xff0000; buf[183] = 0xff0000;
        buf[184] = 0xff0000; buf[185] = 0xff0000; buf[186] = 0xff0000; buf[187] = 0xff0000;
        buf[188] = 0xff0000; buf[189] = 0xff0000; buf[190] = 0xff0000; buf[191] = 0xff0000;
        buf[192] = 0xff00c7; buf[193] = 0xff00c7; buf[194] = 0xff00c7; buf[195] = 0xff00c7;
        buf[196] = 0xff00c7; buf[197] = 0xff00c7; buf[198] = 0xff00c7; buf[199] = 0xff00c7;
        buf[200] = 0xff00c7; buf[201] = 0xff00c7; buf[202] = 0xff00c7; buf[203] = 0xff00c7;
        buf[204] = 0xff00c7; buf[205] = 0xff00c7; buf[206] = 0xff00c7; buf[207] = 0xff00c7;
        buf[208] = 0xbd0095; buf[209] = 0xbd0095; buf[210] = 0xbd0095; buf[211] = 0xbd0095;
        buf[212] = 0xbd0095; buf[213] = 0xbd0095; buf[214] = 0xbd0095; buf[215] = 0xbd0095;
        buf[216] = 0xbd0095; buf[217] = 0xbd0095; buf[218] = 0xbd0095; buf[219] = 0xbd0095;
        buf[220] = 0xbd0095; buf[221] = 0xbd0095; buf[222] = 0xbd0095; buf[223] = 0xbd0095;
        buf[224] = 0x7c007c; buf[225] = 0x7c007c; buf[226] = 0x7c007c; buf[227] = 0x7c007c;
        buf[228] = 0x7c007c; buf[229] = 0x7c007c; buf[230] = 0x7c007c; buf[231] = 0x7c007c;
        buf[232] = 0x7c007c; buf[233] = 0x7c007c; buf[234] = 0x7c007c; buf[235] = 0x7c007c;
        buf[236] = 0x7c007c; buf[237] = 0x7c007c; buf[238] = 0x7c007c; buf[239] = 0x7c007c;
        buf[240] = 0; buf[241] = 0; buf[242] = 0; buf[243] = 0;
        buf[244] = 0; buf[245] = 0; buf[246] = 0; buf[247] = 0;
        buf[248] = 0; buf[249] = 0; buf[250] = 0; buf[251] = 0;
        buf[252] = 0; buf[253] = 0; buf[254] = 0; buf[255] = 0;
        break;
    }

    for (int i=0;i<256;i++)
    {
        dest[4*i+0]= (unsigned char) ((buf[i]    ) & 0xff);
        dest[4*i+1]= (unsigned char) ((buf[i]>>8 ) & 0xff);
        dest[4*i+2]= (unsigned char) ((buf[i]>>16) & 0xff);
        dest[4*i+3]= (unsigned char) (0xff);
    }
}

