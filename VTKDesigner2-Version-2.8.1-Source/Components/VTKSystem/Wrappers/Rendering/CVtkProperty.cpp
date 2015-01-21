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

#include "CVtkProperty.h"
#include "DefineShaderDialog.h"
#include <GCF/AbstractComponent>
#include <QDebug>
#include <QByteArray>
#include <QString>
#include <QDomDocument>

DEFINE_VTK_OBJECT(CVtkProperty, CVtkObject, vtkProperty)
{
	pDesc->setNodeClassName("vtkProperty");
	pDesc->setNodeClassCategory("Rendering");
	pDesc->setNodeClassDescription("Rendering attributes for an actor");

	pDesc->setPropertyInfo("Interpolation", "SetInterpolation", "GetInterpolation", QStringList() << "int", QStringList(), "void", "int");
	pDesc->setPropertyInfo("Representation", "SetRepresentation", "GetRepresentation", QStringList() << "int", QStringList(), "void", "int");
	pDesc->setPropertyInfo("Color", "SetColor", "GetColor", QStringList() << "color_double" << "color_double" << "color_double", QStringList(), "void", "double*");
	pDesc->setPropertyInfo("Ambient", "SetAmbient", "GetAmbient", QStringList() << "double", QStringList(), "void", "double");
	pDesc->setPropertyInfo("Diffuse", "SetDiffuse", "GetDiffuse", QStringList() << "double", QStringList(), "void", "double");
	pDesc->setPropertyInfo("Specular", "SetSpecular", "GetSpecular", QStringList() << "double", QStringList(), "void", "double");
	pDesc->setPropertyInfo("SpecularPower", "SetSpecularPower", "GetSpecularPower", QStringList() << "double", QStringList(), "void", "double");
	pDesc->setPropertyInfo("Opacity", "SetOpacity", "GetOpacity", QStringList() << "double", QStringList(), "void", "double");
	pDesc->setPropertyInfo("AmbientColor", "SetAmbientColor", "GetAmbientColor", QStringList() << "color_double" << "color_double" << "color_double", QStringList(), "void", "double*");
	pDesc->setPropertyInfo("DiffuseColor", "SetDiffuseColor", "GetDiffuseColor", QStringList() << "color_double" << "color_double" << "color_double", QStringList(), "void", "double*");
	pDesc->setPropertyInfo("SpecularColor", "SetSpecularColor", "GetSpecularColor", QStringList() << "color_double" << "color_double" << "color_double", QStringList(), "void", "double*");
	pDesc->setPropertyInfo("EdgeVisibility", "SetEdgeVisibility", "GetEdgeVisibility", QStringList() << "int" , QStringList(), "void", "int");
	pDesc->setPropertyInfo("EdgeColor", "SetEdgeColor", "GetEdgeColor", QStringList() << "color_double" << "color_double" << "color_double", QStringList(), "void", "double*");
	pDesc->setPropertyInfo("LineWidth", "SetLineWidth", "GetLineWidth", QStringList() << "double", QStringList(), "void", "double");
	pDesc->setPropertyInfo("LineStipplePattern", "SetLineStipplePattern", "GetLineStipplePattern", QStringList() << "int", QStringList(), "void", "int");
	pDesc->setPropertyInfo("LineStippleRepeatFactor", "SetLineStippleRepeatFactor", "GetLineStippleRepeatFactor", QStringList() << "int", QStringList(), "void", "int");
	pDesc->setPropertyInfo("PointSize", "SetPointSize", "GetPointSize", QStringList() << "double", QStringList(), "void", "double");
	pDesc->setPropertyInfo("BackfaceCulling", "SetBackfaceCulling", "GetBackfaceCulling", QStringList() << "int" , QStringList(), "void", "int");
	pDesc->setPropertyInfo("FrontfaceCulling", "SetFrontfaceCulling", "GetFrontfaceCulling", QStringList() << "int" , QStringList(), "void", "int");

	// Property Output
	pDesc->addConnectionPath(
		new CVtkObjectConnectionPath("Property", IVisSystemNodeConnectionPath::OutputPath, "vtkProperty", 0)
		);
}

CVtkProperty::CVtkProperty() : m_vtkProperty(0), m_predefineShaderIndex(0)
{
	CVtkProperty::InitializeObjectDesc();
	setVtkObject(vtkProperty2::New());

	m_variableList << CVtkProperty::Variable("intVariable", VT_Int);
	m_variableList << CVtkProperty::Variable("floatVariable", VT_Float);
	m_variableList << CVtkProperty::Variable("doubleVariable", VT_Double);
}

CVtkProperty::~CVtkProperty()
{

}

CVtkProperty::Interpolation CVtkProperty::interpolation() const
{
	switch(m_vtkProperty->GetInterpolation())
	{
	case VTK_FLAT:return (Flat);
	case VTK_GOURAUD:return (Gouraud);
	case VTK_PHONG:return (Phong);
	default :return (Flat);
	}
}

void CVtkProperty::setInterpolation(Interpolation val)
{
	switch(val)
	{
	case Flat:
		m_vtkProperty->SetInterpolation(VTK_FLAT);
		break;
	case Gouraud:
		m_vtkProperty->SetInterpolation(VTK_GOURAUD);
		break;
	case Phong:
		m_vtkProperty->SetInterpolation(VTK_PHONG);
		break;
	default :
		m_vtkProperty->SetInterpolation(VTK_FLAT);
		break;
	}
}


CVtkProperty::Representation CVtkProperty::representation() const
{
	switch(m_vtkProperty->GetRepresentation())
	{
	case VTK_POINTS:return (Points);
	case VTK_WIREFRAME:return (Wireframe);
	case VTK_SURFACE:return (Surface);
	default :return (Points);
	}
}

void CVtkProperty::setRepresentation(Representation val)
{
	switch(val)
	{
	case Points:
		m_vtkProperty->SetRepresentation(VTK_POINTS);
		break;
	case Wireframe:
		m_vtkProperty->SetRepresentation(VTK_WIREFRAME);
		break;
	case Surface:
		m_vtkProperty->SetRepresentation(VTK_SURFACE);
		break;
	default :
		m_vtkProperty->SetRepresentation(VTK_POINTS);
		break;
	}
}


QColor CVtkProperty::color() const
{
	QColor retColor;
	double* color = m_vtkProperty->GetColor();
	retColor.setRgbF(color[0], color[1], color[2]);
	return retColor;
}


void CVtkProperty::setColor(QColor val)
{
	m_vtkProperty->SetColor(val.redF(), val.greenF(), val.blueF());
}


double CVtkProperty::ambient() const
{
	return m_vtkProperty->GetAmbient();
}


void CVtkProperty::setAmbient(double val)
{
	m_vtkProperty->SetAmbient(val);
}


double CVtkProperty::diffuse() const
{
	return m_vtkProperty->GetDiffuse();
}


void CVtkProperty::setDiffuse(double val)
{
	m_vtkProperty->SetDiffuse(val);
}


double CVtkProperty::specular() const
{
	return m_vtkProperty->GetSpecular();
}


void CVtkProperty::setSpecular(double val)
{
	m_vtkProperty->SetSpecular(val);
}


double CVtkProperty::specularPower() const
{
	return m_vtkProperty->GetSpecularPower();
}


void CVtkProperty::setSpecularPower(double val)
{
	((vtkProperty2*)(m_vtkProperty))->SetSpecularPower2(val);
}


double CVtkProperty::opacity() const
{
	return m_vtkProperty->GetOpacity();
}


void CVtkProperty::setOpacity(double val)
{
	m_vtkProperty->SetOpacity(val);
}


QColor CVtkProperty::ambientColor() const
{
	QColor retColor;
	double* color = m_vtkProperty->GetAmbientColor();
	retColor.setRgbF(color[0], color[1], color[2]);
	return retColor;
}


void CVtkProperty::setAmbientColor(QColor val)
{
	m_vtkProperty->SetAmbientColor(val.redF(), val.greenF(), val.blueF());
}


QColor CVtkProperty::diffuseColor() const
{
	QColor retColor;
	double* color = m_vtkProperty->GetDiffuseColor();
	retColor.setRgbF(color[0], color[1], color[2]);
	return retColor;
}


void CVtkProperty::setDiffuseColor(QColor val)
{
	m_vtkProperty->SetDiffuseColor(val.redF(), val.greenF(), val.blueF());
}


QColor CVtkProperty::specularColor() const
{
	QColor retColor;
	double* color = m_vtkProperty->GetSpecularColor();
	retColor.setRgbF(color[0], color[1], color[2]);
	return retColor;
}


void CVtkProperty::setSpecularColor(QColor val)
{
	m_vtkProperty->SetSpecularColor(val.redF(), val.greenF(), val.blueF());
}


bool CVtkProperty::isEdgeVisibility() const
{
	return m_vtkProperty->GetEdgeVisibility();
}


void CVtkProperty::setEdgeVisibility(bool val)
{
	m_vtkProperty->SetEdgeVisibility(val);
}


QColor CVtkProperty::edgeColor() const
{
	QColor retColor;
	double* color = m_vtkProperty->GetEdgeColor();
	retColor.setRgbF(color[0], color[1], color[2]);
	return retColor;
}


void CVtkProperty::setEdgeColor(QColor val)
{
	m_vtkProperty->SetEdgeColor(val.redF(), val.greenF(), val.blueF());
}


double CVtkProperty::lineWidth() const
{
	return m_vtkProperty->GetLineWidth();
}


void CVtkProperty::setLineWidth(double val)
{
	m_vtkProperty->SetLineWidth(val);
}


int CVtkProperty::lineStipplePattern() const
{
	return m_vtkProperty->GetLineStipplePattern();
}


void CVtkProperty::setLineStipplePattern(int val)
{
	m_vtkProperty->SetLineStipplePattern(val);
}


int CVtkProperty::lineStippleRepeatFactor() const
{
	return m_vtkProperty->GetLineStippleRepeatFactor();
}


void CVtkProperty::setLineStippleRepeatFactor(int val)
{
	m_vtkProperty->SetLineStippleRepeatFactor(val);
}


double CVtkProperty::pointSize() const
{
	return m_vtkProperty->GetPointSize();
}


void CVtkProperty::setPointSize(double val)
{
	m_vtkProperty->SetPointSize(val);
}


bool CVtkProperty::isBackfaceCulling() const
{
	return m_vtkProperty->GetBackfaceCulling();
}


void CVtkProperty::setBackfaceCulling(bool val)
{
	m_vtkProperty->SetBackfaceCulling(val);
}


bool CVtkProperty::isFrontfaceCulling() const
{
	return m_vtkProperty->GetFrontfaceCulling();
}


void CVtkProperty::setFrontfaceCulling(bool val)
{
	m_vtkProperty->SetFrontfaceCulling(val);
}

void CVtkProperty::setShaderEnabled(bool val)
{
	m_vtkProperty->SetShading(val);
    if(val)
        this->applyShader();
}

bool CVtkProperty::isShaderEnabled() const
{
	return m_vtkProperty->GetShading();
}

void CVtkProperty::setVertexShaderCode(const QString& code)
{
	m_vertexShaderCode = code;
}

QString CVtkProperty::vertexShaderCode() const
{
	return m_vertexShaderCode;
}

void CVtkProperty::setFragmentShaderCode(const QString& code)
{
	m_fragmentShaderCode = code;
}

QString CVtkProperty::fragmentShaderCode() const
{
	return m_fragmentShaderCode;
}

void CVtkProperty::setShaderVariables(const QString& names)
{
	m_variableList.clear();

	QStringList vars = names.split("|", QString::SkipEmptyParts);

	for(int i=0; i<vars.count(); i+=3)
	{
		QString type = vars.at(i);
		QString name = vars.at(i+1);
		QString value = vars.at(i+2);

		Variable var;
		if(type == "int")
			var.type = VT_Int;
		else if(type == "float")
			var.type = VT_Float;
		else
			var.type = VT_Double;

		var.name = name;
		var.value = value;
		if(var.value == "(null)")
			var.value.clear();

		m_variableList.append(var);
	}

    for(int i=0; i<m_variableList.count(); i++)
        this->setPropertyValue( i, m_variableList.at(i).value );
}

QString CVtkProperty::shaderVariables() const
{
	QStringList ret;
	Q_FOREACH(Variable var, m_variableList)
	{
		if( var.type == VT_Int )
			ret << "int";
		else if( var.type == VT_Float )
			ret << "float";
		else
			ret << "double";

		ret << var.name;
		if(var.value.isEmpty())
			ret << "(null)";
		else
			ret << var.value;
	}

	QString retStr = ret.join("|");
	return retStr;
}

bool CVtkProperty::fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData)
{
	if(!path || !outputData)
		return false;

	if(path->pathName() == "Property")
	{
		m_vtkPropertyData.setProperty(m_vtkProperty);
		*outputData = &m_vtkPropertyData;
		return true;
	}
	return CVtkObject::fetchOutput(path, outputData);
}

bool CVtkProperty::outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData)
{
	Q_UNUSED(path);
	Q_UNUSED(outputData);
	return CVtkObject::outputDerefed(path, outputData);
}

QStringList CVtkProperty::configurationOptions()
{
	return QStringList() << "Define Shader";
}

void CVtkProperty::showConfigurationDialog(QString option)
{
	if(option == "Define Shader" || option.isEmpty())
	{
		DefineShaderDialog dialog( this, GCF::AbstractComponent::componentHostWidget() );
		dialog.setWindowTitle( QString("%1 - Define Shader").arg( this->nodeName() ) );
		dialog.resize(640, 480);

		dialog.setVertexShaderCode(m_vertexShaderCode);
		dialog.setFragmentShaderCode(m_fragmentShaderCode);
		dialog.setPredefinedShader(m_predefineShaderIndex);

		if( dialog.exec() == QDialog::Accepted )
		{
			m_vertexShaderCode = dialog.vertexShaderCode();
			m_fragmentShaderCode = dialog.fragmentShaderCode();
			m_predefineShaderIndex = dialog.predefinedShader();
            this->applyShader();

            emit propertyChanged("VertexShaderCode");
            emit propertyChanged("FragmentShaderCode");
            emit propertyChanged("ShaderVariables");
		}
	}
}

void CVtkProperty::applyShader()
{
	QString xmlTag = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n" ;
	QString xmlMaterialTag = "<Material name=\"SimpleProceduralTexture\" number_of_properties=\"0\" number_of_vertex_shaders=\"1\" number_of_fragment_shaders=\"1\">\n" ;
	QString xmlVertexShaderTag = "\n<Shader scope=\"Vertex\" name=\"vertexShader\" location=\"Inline\" language=\"GLSL\" entry=\"main\">\n";
	QString xmlFragmentShaderTag = "\n<Shader scope=\"Fragment\" name=\"fragmentShader\" location=\"Inline\" language=\"GLSL\" entry=\"main\">\n";

	QString uniformVarTag;
	QString xmlApplicationUniformTag;

	for(int index=0; index<m_variableList.count(); index++)
	{
		QByteArray variableName = m_variableList.at(index).name.toLatin1();
		QStringList elementList = m_variableList.at(index).value.split(",", QString::SkipEmptyParts);

		if(m_variableList.at(index).type == VT_Int)
		{ 
			xmlApplicationUniformTag = QString("<ApplicationUniform type = \"%1\"  name = \"%2\" value = \"%3\" />\n")
				.arg("int")
				.arg(variableName.constData())
				.arg(variableName.constData());
		}

		if(m_variableList.at(index).type == VT_Float)
		{ 
			xmlApplicationUniformTag = QString("<ApplicationUniform type = \"%1\"  name = \"%2\" value = \"%3\" />\n")
				.arg("float")
				.arg(variableName.constData())
				.arg(variableName.constData());
		}

		if(m_variableList.at(index).type == VT_Double)
		{ 
			xmlApplicationUniformTag = QString("<ApplicationUniform type = \"%1\"  name = \"%2\" value = \"%3\" />\n")
				.arg("double")
				.arg(variableName.constData())
				.arg(variableName.constData());
		}
		uniformVarTag.append(xmlApplicationUniformTag);
	}
	QByteArray xmlMaterial;
	xmlMaterial.append(xmlTag);
	xmlMaterial.append(xmlMaterialTag);
	xmlMaterial.append(xmlVertexShaderTag);
	xmlMaterial.append(uniformVarTag);
	xmlMaterial.append(m_vertexShaderCode);
	xmlMaterial.append("\n</Shader>\n");
	xmlMaterial.append(xmlFragmentShaderTag);
	xmlMaterial.append(m_fragmentShaderCode);
	xmlMaterial.append("\n</Shader>\n");
	xmlMaterial.append("</Material>");
	m_vtkProperty->LoadMaterialFromString(xmlMaterial.constData());
	
    for(int i=0; i<m_variableList.count(); i++)
        this->setPropertyValue( i, m_variableList.at(i).value );
}

void CVtkProperty::setIntShaderVariable(const QString& name, const QList<QVariant>& value)
{
    int varIndex = this->indexOfVariable(name);
    if(varIndex < 0 || this->m_variableList[varIndex].type != VT_Int)
        return;

   	QStringList valueStrList;
    Q_FOREACH(QVariant v, value)
        valueStrList << v.toString();

    QString valueStr = valueStrList.join(", ");
    this->setPropertyValue(varIndex, valueStr);
    emit propertyChanged(name);
}

void CVtkProperty::setFloatShaderVariable(const QString& name, const QList<QVariant>& value)
{
    int varIndex = this->indexOfVariable(name);
    if(varIndex < 0 || this->m_variableList[varIndex].type != VT_Float)
        return;

   	QStringList valueStrList;
    Q_FOREACH(QVariant v, value)
        valueStrList << v.toString();

    QString valueStr = valueStrList.join(", ");
    this->setPropertyValue(varIndex, valueStr);
    emit propertyChanged(name);
}

void CVtkProperty::setDoubleShaderVariable(const QString& name, const QList<QVariant>& value)
{
    int varIndex = this->indexOfVariable(name);
    if(varIndex < 0 || this->m_variableList[varIndex].type != VT_Double)
        return;

   	QStringList valueStrList;
    Q_FOREACH(QVariant v, value)
        valueStrList << v.toString();

    QString valueStr = valueStrList.join(", ");
    this->setPropertyValue(varIndex, valueStr);
    emit propertyChanged(name);
}

QList<QVariant> CVtkProperty::intShaderVariable(const QString& name) const
{
    QList<QVariant> retList;

    int varIndex = this->indexOfVariable(name);
    if(varIndex < 0 || this->m_variableList[varIndex].type != VT_Int)
        return retList;

    QStringList valueStrList = m_variableList[varIndex].value.split(",", QString::SkipEmptyParts);
    Q_FOREACH(QString valueStr, valueStrList)
        retList << valueStr.trimmed().toInt();

    return retList;
}

QList<QVariant> CVtkProperty::floatShaderVariable(const QString& name) const
{
    QList<QVariant> retList;

    int varIndex = this->indexOfVariable(name);
    if(varIndex < 0 || this->m_variableList[varIndex].type != VT_Float)
        return retList;

    QStringList valueStrList = m_variableList[varIndex].value.split(",", QString::SkipEmptyParts);
    Q_FOREACH(QString valueStr, valueStrList)
        retList << valueStr.trimmed().toFloat();

    return retList;
}

QList<QVariant> CVtkProperty::doubleShaderVariable(const QString& name) const
{
    QList<QVariant> retList;

    int varIndex = this->indexOfVariable(name);
    if(varIndex < 0 || this->m_variableList[varIndex].type != VT_Double)
        return retList;

    QStringList valueStrList = m_variableList[varIndex].value.split(",", QString::SkipEmptyParts);
    Q_FOREACH(QString valueStr, valueStrList)
        retList << valueStr.trimmed().toDouble();

    return retList;
}

bool CVtkProperty::showDefaultConfigurationDialog()
{
	showConfigurationDialog(QString());
	return true;
}

int CVtkProperty::propertyCount()
{
	return m_variableList.count();
}

QString CVtkProperty::propertyName(int index)
{
	if(index < 0 || index >= m_variableList.count())
		return QString();

	QString typeStr = "int";
	if(m_variableList[index].type == CVtkProperty::VT_Float)
		typeStr = "float";
	else if(m_variableList[index].type == CVtkProperty::VT_Double)
		typeStr = "double";

	QString propName = QString("%1 (%2)")
		.arg(m_variableList[index].name)
		.arg(typeStr);
	return propName;
}

QString CVtkProperty::propertyGroup(int index)
{
	Q_UNUSED(index);
	return "Shader Variables";
}

QVariant CVtkProperty::propertyValue(int index)
{
	if(index < 0 || index >= m_variableList.count())
		return QVariant();

	return m_variableList.at(index).value;
}

void CVtkProperty::setPropertyValue(int index, QVariant value)
{
	if(index < 0 || index >= m_variableList.count())
		return;

	m_variableList[index].value = value.toString();
	QByteArray varNameBA = m_variableList.at(index).name.toLatin1();

	if(m_variableList.at(index).type == VT_Int)
	{                    
		QString varValue = m_variableList.at(index).value;     
		QStringList list = varValue.split(",", QString::SkipEmptyParts);

		QVector<int> intVec( list.count() );
		for(int j=0; j<list.size(); j++)
		{
			int value = list.at(j).simplified().toInt();
			intVec[j] = value;
		}

		m_vtkProperty->AddShaderVariable(
			varNameBA.constData(),
			list.count(),
			intVec.data());
	}

	if(m_variableList.at(index).type == VT_Float)
	{                     
		QString varValue = m_variableList.at(index).value;     
		QStringList list = varValue.split(",", QString::SkipEmptyParts);

		QVector<float> floatVec( list.count() );
		for(int j=0; j<list.size(); j++)
		{
			float value = list.at(j).simplified().toFloat();
			floatVec[j] = value;
		}

		m_vtkProperty->AddShaderVariable(
			varNameBA.constData(),
			list.count(),
			floatVec.data());
	}   

	if(m_variableList.at(index).type == VT_Double)
	{ 
		QString varValue = m_variableList.at(index).value;   
		QStringList list = varValue.split(",", QString::SkipEmptyParts);

		QVector<double> doubleVec( list.count() );
		for(int j=0; j<list.size(); j++)
		{
			double value = list.at(j).simplified().toDouble();
			doubleVec[j] = value;
		}

		m_vtkProperty->AddShaderVariable(
			varNameBA.constData(),
			list.count(),
			doubleVec.data());
	}
}

bool CVtkProperty::hasEditor(int index)
{
	Q_UNUSED(index);
	return false;
}

QWidget* CVtkProperty::createEditor(int index)
{
	Q_UNUSED(index);
	return 0;
}

void CVtkProperty::setEditorValue(int index, QWidget* widget, QVariant value)
{
	Q_UNUSED(index);
	Q_UNUSED(widget);
	Q_UNUSED(value);
}

QVariant CVtkProperty::editorValue(int index, QWidget* widget)
{
	Q_UNUSED(index);
	Q_UNUSED(widget);
	return QVariant();
}

void CVtkProperty::connectEditor(int index, QWidget* widget, QObject* receiver, const char* member)
{
	Q_UNUSED(index);
	Q_UNUSED(widget);
	Q_UNUSED(receiver);
	Q_UNUSED(member);
}

QString CVtkProperty::propertyValueString(int index)
{
	if(index < 0 || index >= m_variableList.count())
		return QString();

	return m_variableList.at(index).value;
}

QVariant CVtkProperty::propertyValueFromString(int index, QString strValue)
{
	Q_UNUSED(index);
	return strValue;
}

// IObjectPropertyVisibility
bool CVtkProperty::isQPropertyVisible(int qPropertyIndex) const
{
	static QList<int> hiddenPropIndexList;
	if( !hiddenPropIndexList.count() )
	{
		hiddenPropIndexList << metaObject()->indexOfProperty("VertexShaderCode");
		hiddenPropIndexList << metaObject()->indexOfProperty("FragmentShaderCode");
		hiddenPropIndexList << metaObject()->indexOfProperty("ShaderVariables");
	}

	if( hiddenPropIndexList.contains(qPropertyIndex) )
		return false;

	return true;
}

bool CVtkProperty::isEditablePropertyVisible(int ePropertyIndex) const
{
	Q_UNUSED(ePropertyIndex);
	return true;
}

