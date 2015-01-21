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

#ifndef CALCULATOR_WIDGET_H
#define CALCULATOR_WIDGET_H

#include <QWidget>
class QSyntaxHighlighter;

struct CalculatorWidgetData;
class CalculatorWidget : public QWidget
{
    Q_OBJECT

public:
    CalculatorWidget(QWidget* parent=0);
    ~CalculatorWidget();

    void setHistory(const QString& history);
    QString history() const;

    void setHighlighter(QSyntaxHighlighter* h);
    QSyntaxHighlighter* highlighter() const;

public slots:
    void clearHistory();
    void clearExpression();

protected slots:
    void on_cmdCompute_clicked();

private:
    CalculatorWidgetData* d;
};

#endif
