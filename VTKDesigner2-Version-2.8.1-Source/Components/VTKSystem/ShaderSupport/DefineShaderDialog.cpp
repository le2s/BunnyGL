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

#include "DefineShaderDialog.h"
#include "ui_DefineShaderDialog.h"

#include "CVtkProperty.h"

#include <QComboBox>
#include <QMessageBox>
#include <QItemDelegate>
#include <QItemEditorFactory>
#include <QItemEditorCreatorBase>
#include <QLineEdit>
#include <QFileDialog>
#include <QFile>
#include <QDomDocument>
#include <QDomElement>
#include <QDomAttr>
#include <QDomNode>
#include <QString>
#include <QDebug>
#include <QFileInfo>
#include <QDir>

struct DefineShaderDialogData
{
	DefineShaderDialogData() : selectedShaderIndex(0) {}
	Ui::DefineShaderDialog ui;
	CVtkProperty* property;
	int selectedShaderIndex;
	QString typeToString(CVtkProperty::VariableType type) {
		if(type == CVtkProperty::VT_Int)
			return "int";
		if(type == CVtkProperty::VT_Float)
			return "float";
		return "double";
	}

	CVtkProperty::VariableType stringToType(const QString& type) {
		if(type == "int")
			return CVtkProperty::VT_Int;
		if(type == "float")
			return CVtkProperty::VT_Float;
		return CVtkProperty::VT_Double;
	}

	QStringList defaultValueList;
};

class VariableTypeCreator : public QItemEditorCreatorBase
{
public:
	QWidget* createWidget(QWidget* parent) const {
		return new VariableTypeComboBox(parent);
	}

	QByteArray valuePropertyName() const {
		return QByteArray("VariableType");
	}
};

class VariableNameCreator : public QItemEditorCreatorBase
{
public:
	QWidget* createWidget(QWidget* parent) const {
		QLineEdit* lineEdit = new QLineEdit(parent);
		QRegExpValidator* Validator = new QRegExpValidator( QRegExp("^\\D\\S*"), lineEdit );
		lineEdit->setValidator( Validator );
		return lineEdit ;
	}
	QByteArray valuePropertyName() const {
		return QByteArray("text");
	}
};

DefineShaderDialog::DefineShaderDialog(CVtkProperty* prop, QWidget* parent)
: QDialog(parent)
{
	d = new DefineShaderDialogData;

	d->ui.setupUi(this);

	// Setup the code editors.
	d->ui.txtFragmentShaderCode->setTabStopWidth(d->ui.tabFragmentShader->fontMetrics().width("    "));
	d->ui.txtVertexShaderCode->setTabStopWidth(d->ui.tabVertexShader->fontMetrics().width("    "));

	// Load variables from the property
	d->property = prop;
	for(int i=0; i<d->property->variableCount(); i++)
	{
		QTreeWidgetItem* item = new QTreeWidgetItem( d->ui.lstShaderVariables );
		item->setFlags(item->flags()|Qt::ItemIsEditable);
		item->setText( 0, d->property->variableNameAt(i) );
		item->setText( 1, d->typeToString( d->property->variableTypeAt(i) ) );
		item->setFont( 0, QFont("Monospace", 12) );
		item->setSizeHint( 0, QSize(50, 30) );
		item->setSizeHint( 1, QSize(50, 30) );
	}

	QItemEditorFactory* itemEditorFactory1 = new QItemEditorFactory;
	itemEditorFactory1->registerEditor(QVariant::String, new VariableNameCreator);
	QItemDelegate* firstColumnDelegate = new QItemDelegate(d->ui.lstShaderVariables);
	firstColumnDelegate->setItemEditorFactory(itemEditorFactory1);
	d->ui.lstShaderVariables->setItemDelegateForColumn(0, firstColumnDelegate);

	// Second column in lstShaderVariables, should be a QComboBox with int/float/double as options.
	QItemEditorFactory* itemEditorFactory2 = new QItemEditorFactory;
	itemEditorFactory2->registerEditor(QVariant::String, new VariableTypeCreator);
	QItemDelegate* secondColumnDelegate = new QItemDelegate(d->ui.lstShaderVariables);
	secondColumnDelegate->setItemEditorFactory(itemEditorFactory2);
	d->ui.lstShaderVariables->setItemDelegateForColumn(1, secondColumnDelegate);

	d->ui.txtVertexShaderCode->setLineWrapMode(QPlainTextEdit::NoWrap);
	d->ui.txtFragmentShaderCode->setLineWrapMode(QPlainTextEdit::NoWrap);
	

	d->ui.cmbShaderList->addItem("<--Select Shader-->");
    QDir shadersDir = QString("%1/Shaders").arg(qApp->applicationDirPath());
    QFileInfoList fiList = shadersDir.entryInfoList(QStringList("*.xml"), QDir::Files, QDir::Name);
    Q_FOREACH(QFileInfo fi, fiList)
        d->ui.cmbShaderList->addItem( fi.baseName(), fi.absoluteFilePath() );
	d->ui.cmbShaderList->setCurrentIndex(d->selectedShaderIndex);

	connect(d->ui.buttonBox,SIGNAL(clicked()),this,SLOT(accepted()));
}

DefineShaderDialog::~DefineShaderDialog()
{   
	delete d;
}

QString DefineShaderDialog::vertexShaderCode() const
{
	return d->ui.txtVertexShaderCode->toPlainText();
}

QString DefineShaderDialog::fragmentShaderCode() const
{
	return d->ui.txtFragmentShaderCode->toPlainText();
}

void DefineShaderDialog::setVertexShaderCode(QString vertexShader)
{
	d->ui.txtVertexShaderCode->setPlainText(vertexShader);
}

void DefineShaderDialog::setFragmentShaderCode(QString fragmentShader)
{
	d->ui.txtFragmentShaderCode->setPlainText(fragmentShader);
}

void DefineShaderDialog::setPredefinedShader(int index)
{
	d->selectedShaderIndex = index;
	d->ui.cmbShaderList->setCurrentIndex(d->selectedShaderIndex);
}

int DefineShaderDialog::predefinedShader() const
{
	return d->selectedShaderIndex;
}

void DefineShaderDialog::on_cmdAddShaderVariable_clicked()
{
	QTreeWidgetItem* item = new QTreeWidgetItem(d->ui.lstShaderVariables);
	item->setFlags(item->flags()|Qt::ItemIsEditable);
	item->setText( 0, "Enter Variable Name" );
	item->setText( 1, "int" );
	item->setFont( 0, QFont("Monospace", 12) );
	item->setSizeHint( 0, QSize(50, 30) );
	item->setSizeHint( 1, QSize(50, 30) );

	d->ui.lstShaderVariables->editItem(item);
}

void DefineShaderDialog::on_cmdRemoveShaderVariable_clicked()
{
	QTreeWidgetItem* item = d->ui.lstShaderVariables->currentItem();
	if(!item)
		return;

	int index = d->ui.lstShaderVariables->indexOfTopLevelItem(item);
	d->property->removeVariable(index);
	delete item;
}

void  DefineShaderDialog::accepted()
{   
	int count = d->property->variableCount();
	for(int i=0; i<d->ui.lstShaderVariables->topLevelItemCount(); i++)
	{
		QTreeWidgetItem* item = d->ui.lstShaderVariables->topLevelItem(i);
		if(i < count)
		{
			d->property->changeVariableName(i, item->text(0));
			d->property->changeVariableType(i, d->stringToType(item->text(1)));
		}
		else
		{
			QString value = "";
			if(i < d->defaultValueList.size())
				value = d->defaultValueList.at(i);

			d->property->addVariable( item->text(0), d->stringToType(item->text(1)), value);
		}
	}
}

int DefineShaderDialog::on_cmbShaderList_activated(int index)
{
    QString fileName = d->ui.cmbShaderList->itemData(index).toString();
	if( fileName.isEmpty() )
    {
        d->ui.txtVertexShaderCode->clear();
        d->ui.txtFragmentShaderCode->clear();
        d->ui.lstShaderVariables->clear();
		
    }
	d->selectedShaderIndex = index;
	d->property->removeAllVariables();
    
	QDomDocument doc;
    QFile file( fileName );
    if (!file.open(QIODevice::ReadOnly))
        return 0;
    if (!doc.setContent(&file))
    {
        file.close();
        return 0;
    }
    file.close();

    QDomElement docRootElem = doc.documentElement();
    QDomNode n = docRootElem.firstChild();
		
    while(!n.isNull())
    {
        QDomElement e = n.toElement(); // try to convert the node to an element.

        if(!e.isNull())
        {
            QDomAttr a =e.attributeNode("scope");
			QString str = e.text().trimmed();
			str.replace("<","&lt;");
			str.replace(">","&gt;");
			str.replace("&&","&amp;&amp;");
			str.replace("'","&apos;");
			str.replace("\"","&quot;");
			
			if(a.value()=="Vertex")
			{
				d->ui.txtVertexShaderCode->setPlainText(str);
			    d->ui.txtVertexShaderCode->setTabStopWidth(40);
			}  
			if(a.value()=="Fragment")
			{
				d->ui.txtFragmentShaderCode->setPlainText(str);
			    d->ui.txtFragmentShaderCode->setTabStopWidth(40);
			}
		}
        n = n.nextSibling();
    }
	d->ui.lstShaderVariables->clear();
    QDomElement rootElement = doc.documentElement();
	for(QDomNode n = rootElement.firstChild(); !n.isNull(); n = n.nextSibling())
    {
        if(n.toElement().tagName()!="")
        {
            for(QDomNode p = n.toElement().firstChild();!p.isNull(); p = p.nextSibling())
            {
                if(p.toElement().tagName()!="")
                {
                    QDomElement pElement = p.toElement();
                    QDomAttr name = pElement.attributeNode("name"); 
                    QDomAttr type = pElement.attributeNode("type"); 
    
					d->defaultValueList.append(pElement.attributeNode("defaultValue").value()); 
                    
					QTreeWidgetItem* item = new QTreeWidgetItem(d->ui.lstShaderVariables);
					item->setFlags(item->flags()|Qt::ItemIsEditable);
					item->setText( 0, name.value() );
					item->setText( 1, type.value());
					item->setFont( 0, QFont("Monospace", 12) );
					item->setSizeHint( 0, QSize(50, 30) );
					item->setSizeHint( 1, QSize(50, 30) );
				}   
            }
        }
    }
    return 0;
}
 