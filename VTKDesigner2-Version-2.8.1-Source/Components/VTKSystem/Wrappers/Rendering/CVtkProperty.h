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

#ifndef C_VTK_PROPERTY_H
#define C_VTK_PROPERTY_H

#include "CVtkObject.h"
#include "vtkOpenGLProperty.h"
#include "CVtkPropertyData.h"
#include "IConfigurable.h"
#include "IEditableProperties.h"
#include "IObjectPropertyVisibility.h"

class CVtkProperty : public CVtkObject, virtual public IConfigurable, 
	virtual public IEditableProperties,
	virtual public IObjectPropertyVisibility
{
	Q_OBJECT
		Q_INTERFACES(IConfigurable IEditableProperties IObjectPropertyVisibility)
		Q_ENUMS(Interpolation)
		Q_ENUMS(Representation)
		Q_PROPERTY(Interpolation Interpolation READ interpolation WRITE setInterpolation)
		Q_PROPERTY(Representation Representation READ representation WRITE setRepresentation)
		Q_PROPERTY(QColor Color READ color WRITE setColor)
		Q_PROPERTY(double Ambient READ ambient WRITE setAmbient)
		Q_PROPERTY(double Diffuse READ diffuse WRITE setDiffuse)
		Q_PROPERTY(double Specular READ specular WRITE setSpecular)
		Q_PROPERTY(double SpecularPower READ specularPower WRITE setSpecularPower)
		Q_PROPERTY(double Opacity READ opacity WRITE setOpacity)
		Q_PROPERTY(QColor AmbientColor READ ambientColor WRITE setAmbientColor)
		Q_PROPERTY(QColor DiffuseColor READ diffuseColor WRITE setDiffuseColor)
		Q_PROPERTY(QColor SpecularColor READ specularColor WRITE setSpecularColor)
		Q_PROPERTY(bool EdgeVisibility READ isEdgeVisibility WRITE setEdgeVisibility)
		Q_PROPERTY(QColor EdgeColor READ edgeColor WRITE setEdgeColor)
		Q_PROPERTY(double LineWidth READ lineWidth WRITE setLineWidth)
		Q_PROPERTY(int LineStipplePattern READ lineStipplePattern WRITE setLineStipplePattern)
		Q_PROPERTY(int LineStippleRepeatFactor READ lineStippleRepeatFactor WRITE setLineStippleRepeatFactor)
		Q_PROPERTY(double PointSize READ pointSize WRITE setPointSize)
		Q_PROPERTY(bool BackfaceCulling READ isBackfaceCulling WRITE setBackfaceCulling)
		Q_PROPERTY(bool FrontfaceCulling READ isFrontfaceCulling WRITE setFrontfaceCulling)
		Q_PROPERTY(QString VertexShaderCode READ vertexShaderCode WRITE setVertexShaderCode SCRIPTABLE false)
		Q_PROPERTY(QString FragmentShaderCode READ fragmentShaderCode WRITE setFragmentShaderCode SCRIPTABLE false)
		Q_PROPERTY(QString ShaderVariables READ shaderVariables WRITE setShaderVariables SCRIPTABLE false)
		Q_PROPERTY(bool ShaderEnabled READ isShaderEnabled WRITE setShaderEnabled)
		DECLARE_VTK_OBJECT

public:
	enum Interpolation
	{
		Flat=VTK_FLAT,
		Gouraud=VTK_GOURAUD,
		Phong=VTK_PHONG
	};

	enum Representation
	{
		Points=VTK_POINTS,
		Wireframe=VTK_WIREFRAME,
		Surface=VTK_SURFACE
	};

	CVtkProperty();
	~CVtkProperty();

	Interpolation interpolation() const;
	void setInterpolation(Interpolation val);

	Representation representation() const;
	void setRepresentation(Representation val);

	QColor color() const;
	void setColor(QColor val);

	double ambient() const;
	void setAmbient(double val);

	double diffuse() const;
	void setDiffuse(double val);

	double specular() const;
	void setSpecular(double val);

	double specularPower() const;
	void setSpecularPower(double val);

	double opacity() const;
	void setOpacity(double val);

	QColor ambientColor() const;
	void setAmbientColor(QColor val);

	QColor diffuseColor() const;
	void setDiffuseColor(QColor val);

	QColor specularColor() const;
	void setSpecularColor(QColor val);

	bool isEdgeVisibility() const;
	void setEdgeVisibility(bool val);

	QColor edgeColor() const;
	void setEdgeColor(QColor val);

	double lineWidth() const;
	void setLineWidth(double val);

	int lineStipplePattern() const;
	void setLineStipplePattern(int val);

	int lineStippleRepeatFactor() const;
	void setLineStippleRepeatFactor(int val);

	double pointSize() const;
	void setPointSize(double val);

	bool isBackfaceCulling() const;
	void setBackfaceCulling(bool val);

	bool isFrontfaceCulling() const;
	void setFrontfaceCulling(bool val);

	void setVertexShaderCode(const QString& code);
	QString vertexShaderCode() const;

	void setFragmentShaderCode(const QString& code);
	QString fragmentShaderCode() const;

	void setShaderVariables(const QString& names);
	QString shaderVariables() const;

	void setShaderEnabled(bool val);
	bool isShaderEnabled() const;

protected:
	bool fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData);
	bool outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData);

public:
	// IConfigurable implementation
	QStringList configurationOptions();
	void showConfigurationDialog(QString option);
	bool showDefaultConfigurationDialog();

private:
    void applyShader();

public:
    Q_INVOKABLE void setIntShaderVariable(const QString& name, const QList<QVariant>& value);
    Q_INVOKABLE void setFloatShaderVariable(const QString& name, const QList<QVariant>& value);
    Q_INVOKABLE void setDoubleShaderVariable(const QString& name, const QList<QVariant>& value);

    Q_INVOKABLE QList<QVariant> intShaderVariable(const QString& name) const;
    Q_INVOKABLE QList<QVariant> floatShaderVariable(const QString& name) const;
    Q_INVOKABLE QList<QVariant> doubleShaderVariable(const QString& name) const;

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

public:
	// IObjectPropertyVisibility
	bool isQPropertyVisible(int qPropertyIndex) const;
	bool isEditablePropertyVisible(int ePropertyIndex) const;

public:
	// Custom functions used by DefineShaderDialog
	enum VariableType
	{
		VT_Int,
		VT_Float,
		VT_Double
	};


	int addVariable(const QString& name, VariableType type, const QString& value=QString()) {
		int index = this->indexOfVariable(name);
		if(index >= 0)
			m_variableList[index].type = type;
		else
		{
			index = m_variableList.count();
			m_variableList.append( Variable(name, type, QString()) );
            this->setPropertyValue(index, value);
		}

		emit propertyConfigurationChanged();

		return index;
	}

	void removeVariable(int index) {
		if(index < 0 || index >= m_variableList.count())
			return;

		m_variableList.removeAt(index);
		emit propertyConfigurationChanged();
	}

	void removeAllVariables() {
		m_variableList.clear();
		emit propertyConfigurationChanged();
	}

	void changeVariableName(int index, const QString& newName) {
		if(index < 0 || index >= m_variableList.count())
			return;

		m_variableList[index].name = newName;
		emit propertyConfigurationChanged();
	}

	void changeVariableType(int index, VariableType type) {
		if(index < 0 || index >= m_variableList.count())
			return;

		m_variableList[index].type = type;
		emit propertyConfigurationChanged();
	}

	int indexOfVariable(const QString& name) const {
		for(int i=0; i<m_variableList.count(); i++)
			if( m_variableList.at(i).name == name )
				return i;
		return -1;
	}

	int variableCount() const {
		return m_variableList.count();
	}

	QString variableNameAt(int index) const {
		if(index < 0 || index >= m_variableList.count())
			return QString();

		return m_variableList.at(index).name;
	}

	VariableType variableTypeAt(int index) const {
		if(index < 0 || index >= m_variableList.count())
			return VT_Int;

		return m_variableList.at(index).type;
	}

	QString variableValueAt(int index) const {
		if(index < 0 || index >= m_variableList.count())
			return QString();

		return m_variableList.at(index).value;
	}

private:
	vtkProperty* m_vtkProperty;
	CVtkPropertyData m_vtkPropertyData;

	struct Variable
	{
		Variable() : type(CVtkProperty::VT_Int) { }
		Variable(const QString& n, CVtkProperty::VariableType vt, 
            const QString& val=QString())
			: name(n), type(vt), value(val) { }

		CVtkProperty::VariableType type;
		QString name;
		QString value;
	};
	QList<Variable> m_variableList;
	QString m_vertexShaderCode;
	QString m_fragmentShaderCode;
	int m_predefineShaderIndex;
};

class vtkProperty2 : public vtkOpenGLProperty
{
public:
	static vtkProperty2* New() {
		return new vtkProperty2();
	}

	void SetSpecularPower2(double val) {
		if(val > 128)
			this->SpecularPower = 128;
		else if(val < 0)
			this->SpecularPower = 0;
		else
			this->SpecularPower = val;
		this->Modified();
	}

protected:
	vtkProperty2() { }
	~vtkProperty2() { }
};

#endif
