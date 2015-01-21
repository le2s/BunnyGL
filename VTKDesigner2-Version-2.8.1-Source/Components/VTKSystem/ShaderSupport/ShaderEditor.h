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

#ifndef SHADER_EDITOR_H
#define SHADER_EDITOR_H

#include <QPlainTextEdit>
#include <QSyntaxHighLighter>
#include <QCompleter>

class SyntaxHighLight;

struct ShaderEditorData;
class ShaderEditor : public QPlainTextEdit
{
	Q_OBJECT

public:
	ShaderEditor(QWidget *parent = 0);
	~ShaderEditor();

	void lineNumberAreaPaintEvent(QPaintEvent *event);
	int lineNumberAreaWidth();
	void setCompleter(QCompleter *c);
	QCompleter *completer() const;

	void setFont2(const QFont& font);

protected:
	void resizeEvent(QResizeEvent *event);
	void wheelEvent(QWheelEvent *event);
	void keyPressEvent(QKeyEvent *event);
	void focusInEvent(QFocusEvent *event);

private slots:
		void updateLineNumberAreaWidth(int newBlockCount);
		void highlightCurrentLine();
		void updateLineNumberArea(const QRect &, int);
		void insertCompletion(const QString &completion);

private:
	ShaderEditorData *d;
	QCompleter *c;
	QString textUnderCursor() const;
	QAbstractItemModel *modelFromFile(const QString& fileName);
};


class SyntaxHighlight : public QSyntaxHighlighter
{

public:
	SyntaxHighlight(QTextDocument *parent = 0);
	~SyntaxHighlight();

protected:
	virtual void highlightBlock(const QString &text);

private:
	struct HighlightingRule
	{
		QRegExp pattern;
		QTextCharFormat format;
	};
	QVector<HighlightingRule> highlightingRules;
	QRegExp commentStartExpression;
    QRegExp commentEndExpression;
	QTextCharFormat keywordFormat;
	QTextCharFormat multiLineCommentFormat;
};

#endif
