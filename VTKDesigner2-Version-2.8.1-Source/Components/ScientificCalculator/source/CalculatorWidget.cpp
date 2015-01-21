/****************************************************************************
**
** Copyright (C) My Organization
**
** Use of this file is limited according to the terms specified by
** My Organization.
**
** Details of those terms are listed in licence.txt included as
** part of the distribution package of this file. This file may not
** be distributed without including the licence.txt file.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include "CalculatorWidget.h"
#include "ui_CalculatorWidget.h"
#include <QSyntaxHighlighter>
#include <QtScript>

struct CalculatorWidgetData
{
    CalculatorWidgetData() : highlighter(0) { }

    Ui::CalculatorWidget ui;  

    QSyntaxHighlighter* highlighter;
};

CalculatorWidget::CalculatorWidget(QWidget* parent)
: QWidget(parent)
{
    d = new CalculatorWidgetData;
    d->ui.setupUi(this);
}

CalculatorWidget::~CalculatorWidget()
{
    delete d;
}

void CalculatorWidget::setHistory(const QString& history)
{
    d->ui.txtHistory->setHtml(history);
}

QString CalculatorWidget::history() const
{
    return d->ui.txtHistory->toHtml();
}

void CalculatorWidget::setHighlighter(QSyntaxHighlighter* h)
{
    if(!h || h == d->highlighter)
        return;

    if(d->highlighter)
        delete d->highlighter;
    d->highlighter = h;
    d->highlighter->setParent(d->ui.txtExpression);
    d->highlighter->setDocument(d->ui.txtExpression->document());
}

QSyntaxHighlighter* CalculatorWidget::highlighter() const
{
    return d->highlighter;
}

void CalculatorWidget::clearHistory()
{
    d->ui.txtHistory->clear();
}

void CalculatorWidget::clearExpression()
{
    d->ui.txtExpression->clear();
}

void CalculatorWidget::on_cmdCompute_clicked()
{
    QString text = d->ui.txtExpression->toPlainText();
    
    QScriptEngine engine;
    if( !engine.canEvaluate(text) )
    {
        d->ui.txtHistory->append(
            "<i><font color=\"red\">Cannot evaluate expression</font></i>"
            );
        return;
    }

    QScriptValue result = engine.evaluate(text);
    if(result.isError())
    {
        d->ui.txtHistory->append(
            QString("<i><font color=\"red\">Error: %1</font></i>").arg( result.toString() )
            );
        return;
    }

    QScriptValue ans = engine.globalObject().property("ans");
    if(!ans.isValid())
    {
        d->ui.txtHistory->append(
            "<i><font color=\"red\">Variable ans not found</font></i>"
            );
        return;
    }

    QString htmlText = d->ui.txtExpression->toHtml();
    d->ui.txtHistory->append(
        QString("<h3>Expression</h3>%1").arg(htmlText)
        );
    
    QString resultStr = QString("<p><b><u>Answer</u> = <font color=\"blue\">%1</font></b></p><br>").arg(ans.toString());
    d->ui.txtHistory->append(resultStr);
    d->ui.txtExpression->clear();
}

