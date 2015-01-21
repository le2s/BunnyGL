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

#ifndef DEFINE_SHADER_DIALOG_H
#define DEFINE_SHADER_DIALOG_H


#include "CVtkProperty.h"
#include <QComboBox>
#include <QDialog>
#include <QMap>

class CVtkProperty;

struct DefineShaderDialogData;
class DefineShaderDialog : public QDialog
{
	Q_OBJECT

public:
	DefineShaderDialog(CVtkProperty* property, QWidget* parent);
	~DefineShaderDialog();

	QString vertexShaderCode() const;
	QString fragmentShaderCode() const;
	void setVertexShaderCode(QString vertexShader);
	void setFragmentShaderCode(QString fragmentShader);
	void setPredefinedShader(int index);
	int predefinedShader() const;

	protected slots:
		void on_cmdAddShaderVariable_clicked();
		void on_cmdRemoveShaderVariable_clicked();
		int  on_cmbShaderList_activated(int index);
		void accepted();

private:
	DefineShaderDialogData* d;
};


class VariableTypeComboBox : public QComboBox
{
	Q_OBJECT

public:
	VariableTypeComboBox(QWidget* parent=0) : QComboBox(parent) {
		this->addItem("int");
		this->addItem("float");
		this->addItem("double");
	}

	Q_PROPERTY(QString VariableType READ variableType WRITE setVariableType)
		void setVariableType(const QString& vt) {
			int index = this->findText(vt);
			if(index < 0)
				return;

			this->setCurrentIndex(index);
	}

	QString variableType() const {
		return this->currentText();
	}
};

#endif

