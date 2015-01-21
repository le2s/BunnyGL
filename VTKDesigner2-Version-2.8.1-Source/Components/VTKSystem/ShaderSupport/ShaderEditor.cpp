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

#include "ShaderEditor.h"

#include <QPlainTextEdit>
#include <QPainter>
#include <QAbstractItemView>
#include <QScrollBar>
#include <QCompleter>
#include <QFile>
#include <QStringListModel>
#include <QApplication>

struct ShaderEditorData
{
    QWidget *lineNumberArea;
};

class LineNumberArea : public QWidget
{
public:
    LineNumberArea(ShaderEditor *editor) : QWidget(editor) {
        shaderEditor = editor;
    }

    LineNumberArea() { }

    QSize sizeHint() const {
        return QSize(shaderEditor->lineNumberAreaWidth(), 0);
    }

protected:
    void paintEvent(QPaintEvent *event) {
        shaderEditor->lineNumberAreaPaintEvent(event);
    }

private:
    ShaderEditor *shaderEditor;
};

ShaderEditor::ShaderEditor(QWidget *parent) 
: QPlainTextEdit(parent)
{
    d = new ShaderEditorData;

#ifdef Q_WS_WIN
    QFont font("Courier New", 12);
#else
    QFont font("Monospace", 12);
#endif

    this->setFont2(font);

    d->lineNumberArea = new LineNumberArea(this);
    new SyntaxHighlight(this->document());

    c = new QCompleter(this);
	c->setModel(modelFromFile(QLatin1String(":/VTKSystem/ShaderKeywords.txt")));
	c->setModelSorting(QCompleter::UnsortedModel);  
    c->setCaseSensitivity(Qt::CaseInsensitive);
    c->setWrapAround(false); 
    this->setCompleter(c);

    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
    connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateLineNumberArea(QRect,int)));
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));

    updateLineNumberAreaWidth(0);
    highlightCurrentLine();
}

ShaderEditor::~ShaderEditor()
{
    delete d;
}

int ShaderEditor::lineNumberAreaWidth()
{
    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10) {
        max /= 10;
        ++digits;
    }
    int space = 3 + fontMetrics().width(QLatin1Char('9')) * digits;
    return space;    
}

void ShaderEditor::updateLineNumberAreaWidth(int /* newBlockCount */)
{ 
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

void ShaderEditor::updateLineNumberArea(const QRect &rect, int dy)
{
    if (dy)
        d->lineNumberArea->scroll(0, dy);
    else
        d->lineNumberArea->update(0, rect.y(), d->lineNumberArea->width(), rect.height());

    if (rect.contains(viewport()->rect()))
        updateLineNumberAreaWidth(0);
}

void ShaderEditor::resizeEvent(QResizeEvent *e)
{
    QPlainTextEdit::resizeEvent(e);

    QRect cr = contentsRect();
    d->lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

void ShaderEditor::wheelEvent(QWheelEvent *event)
{
    if (event->modifiers() == Qt::ControlModifier)
    {
        QFont font = this->font();
        int pointSize = font.pointSize();
        
        if(event->delta() > 0)
            ++pointSize;
        else
            --pointSize;

        font.setPointSize(pointSize);

        this->setFont2(font);
    }

    QPlainTextEdit::wheelEvent(event);
}

void ShaderEditor::highlightCurrentLine()
{
    QList<QTextEdit::ExtraSelection> extraSelections;

    if (!isReadOnly()) {
        QTextEdit::ExtraSelection selection;

        QColor lineColor = QColor(Qt::yellow).lighter(160);

        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }
    setExtraSelections(extraSelections);
}

void ShaderEditor::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(d->lineNumberArea);
    painter.fillRect(event->rect(), Qt::lightGray);

    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = top + (int) blockBoundingRect(block).height();

    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(Qt::black);
            painter.drawText(0, top, d->lineNumberArea->width(), fontMetrics().height(),
                Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + (int) blockBoundingRect(block).height();
        ++blockNumber;
    }
}


void ShaderEditor::setCompleter(QCompleter *completer)
{
    if (c)
        QObject::disconnect(c, 0, this, 0);
    c = completer;

    if (!c)
        return;

    c->setWidget(this);
    c->setCompletionMode(QCompleter::PopupCompletion);
    c->setCaseSensitivity(Qt::CaseInsensitive);
    connect(c, SIGNAL(activated(QString)),
        this, SLOT(insertCompletion(QString)));
}


QCompleter *ShaderEditor::completer() const
{
    return c;
}

void ShaderEditor::setFont2(const QFont& font)
{
    QPlainTextEdit::setFont(font);

    QFontMetrics fm(font);
    int width = fm.width("    ");
    this->setTabStopWidth(width);
}

void ShaderEditor::insertCompletion(const QString& completion)
{
    if (c->widget() != this)
        return;
    QTextCursor tc = textCursor();
    int extra = completion.length() - c->completionPrefix().length();
    tc.movePosition(QTextCursor::Left);
    tc.movePosition(QTextCursor::EndOfWord);
    tc.insertText(completion.right(extra));
    setTextCursor(tc);
}

QString ShaderEditor::textUnderCursor() const
{
    QTextCursor tc = QPlainTextEdit::textCursor();
    tc.select(QTextCursor::WordUnderCursor);
    return tc.selectedText();
}

QAbstractItemModel *ShaderEditor::modelFromFile(const QString& fileName)
{
	QFile file(fileName);
     if (!file.open(QFile::ReadOnly)) {
         return new QStringListModel(c);
     }
 #ifndef QT_NO_CURSOR
     QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
 #endif
     QStringList words;

     while (!file.atEnd()) {
         QByteArray line = file.readLine();
         if (!line.isEmpty())
             words << line.trimmed();
     }

 #ifndef QT_NO_CURSOR
     QApplication::restoreOverrideCursor();
 #endif
     return new QStringListModel(words, c);
}

void ShaderEditor::focusInEvent(QFocusEvent *e)
{
    if (c)
        c->setWidget(this);
    QPlainTextEdit::focusInEvent(e);
}

void ShaderEditor::keyPressEvent(QKeyEvent *e)
{
    if (c && c->popup()->isVisible()) {
        // The following keys are forwarded by the completer to the widget
        switch (e->key()) {
case Qt::Key_Enter:
case Qt::Key_Return:
case Qt::Key_Escape:
case Qt::Key_Tab:
case Qt::Key_Backtab:
    e->ignore();
    return; // let the completer do default behavior
default:
    break;
        }
    }

    bool isShortcut = ((e->modifiers() & Qt::ControlModifier) && e->key() == Qt::Key_E); // CTRL+E
    if (!c || !isShortcut) // do not process the shortcut when we have a completer
        QPlainTextEdit::keyPressEvent(e);

    const bool ctrlOrShift = e->modifiers() & (Qt::ControlModifier | Qt::ShiftModifier);
    if (!c || (ctrlOrShift && e->text().isEmpty()))
        return;

    static QString eow("~!@#$%^&*()_+{}|:\"<>?,./;'[]\\-="); // end of word
    bool hasModifier = (e->modifiers() != Qt::NoModifier) && !ctrlOrShift;
    QString completionPrefix = textUnderCursor();

    if (!isShortcut && (hasModifier || e->text().isEmpty()|| completionPrefix.length() < 3
        || eow.contains(e->text().right(1)))) {
            c->popup()->hide();
            return;
    }

    if (completionPrefix != c->completionPrefix()) {
        c->setCompletionPrefix(completionPrefix);
        c->popup()->setCurrentIndex(c->completionModel()->index(0, 0));
    }
    QRect cr = cursorRect();
    cr.setWidth(c->popup()->sizeHintForColumn(0)
        + c->popup()->verticalScrollBar()->sizeHint().width());
    c->complete(cr); // popup it up!
}

SyntaxHighlight::SyntaxHighlight(QTextDocument *parent)
:QSyntaxHighlighter(parent)
{
    HighlightingRule rule1;
    keywordFormat.setForeground(Qt::blue);
    keywordFormat.setFontWeight(QFont::Courier);
    QStringList variableTypes;
    variableTypes <<"#[A-Za-z0-9]*" << "\\bvoid\\b" << "\\bbool\\b"
        << "\\bint\\b" << "\\bunit\\b"

        << "\\bfloat\\b" << "\\bdouble\\b"
        << "\\bvec2\\b" << "\\bvec3\\b" << "\\bvec4\\b"
        << "\\bdvec2\\b" << "\\bdvec3\\b" << "\\bdvec4\\b"
        << "\\bbvec2\\b" << "\\bbvec3\\b" << "\\bbvec4\\b"
        << "\\bivec2\\b" << "\\bivec3\\b" << "\\bivec4\\b"
        << "\\buvec2\\b" << "\\buvec3\\b" << "\\buvec4\\b"

        << "\\bmat2x2\\b" << "\\bmat2x3\\b" << "\\bmat2x4\\b"
        << "\\bmat3x2\\b" << "\\bmat3x3\\b" << "\\bmat3x4\\b"
        << "\\bmat4x2\\b" << "\\bmat4x3\\b" << "\\bmat4x4\\b"

        << "\\bdmat2\\b" << "\\bdmat3\\b" << "\\bdmat4\\b"
        << "\\bdmat2x2\\b" << "\\bdmat2x3\\b" << "\\bdmat2x4\\b"
        << "\\bdmat3x2\\b" << "\\bdmat3x3\\b" << "\\bdmat3x4\\b"
        << "\\bdmat4x2\\b" << "\\bdmat4x3\\b" << "\\bdmat4x4\\b"

        << "\\bsampler1D\\b" << "\\bsampler2D\\b" << "\\bsampler3D\\b"
        << "\\bsamplerCube\\b" << "\\bsampler2DRect\\b" << "\\bsampler2DRectShadow\\b"
        << "\\bsampler1DShadow\\b"  << "\\bsampler2DShadow\\b"
        << "\\bsampler2DRectShadow\\b"
        << "\\bsampler1DArray\\b" << "\\bsampler2DArray\\b"
        << "\\bsampler1DArrayShadow\\b" << "\\bsampler2DArrayShadow\\b"
        << "\\bsamplerBuffer\\b" << "\\bsampler2DMS\\b"
        << "\\bsampler2DMSArray\\b" << "\\bsampler2DMSArray\\b" << "\\bsamplerCubeArray\\b"
        << "\\bsamplerCubeArrayShadow\\b"

        << "\\bisampler1D\\b" << "\\bisampler2D\\b" << "\\ibsampler3D\\b"
        << "\\bisamplerCube\\b" << "\\bisampler2DRect\\b"
        << "\\bisampler1DArray\\b" << "\\bisampler2DArray\\b"
        << "\\ibsamplerBuffer\\b" << "\\bisampler2DArray\\b"
        << "\\bisamplerBuffer\\b" << "\\bisampler2DMS\\b" << "\\bisampler2DMSArray\\b"
        << "\\bisamplerCubeArray\\b"

        << "\\busampler1D\\b" << "\\busampler2D\\b" << "\\busampler3D\\b"
        << "\\busamplerCube\\b" << "\\busampler2DRect\\b"
        << "\\busampler1DArray\\b" << "\\busampler2DArray\\b"
        << "\\busamplerBuffer\\b" << "\\busampler2DArray\\b"
        << "\\busamplerBuffer\\b" << "\\busampler2DMS\\b" << "\\busampler2DMSArray\\b"
        << "\\busamplerCubeArray\\b" ;

    foreach (const QString &pattern, variableTypes)
    {
        rule1.pattern = QRegExp(pattern);
        rule1.format = keywordFormat;
        highlightingRules.append(rule1);
    }

	HighlightingRule rule2;
    keywordFormat.setForeground(Qt::darkYellow);
    keywordFormat.setFontWeight(QFont::Courier);
    QStringList builtInFunctions;

    // Angle and Trigonometry Functions
    builtInFunctions << "\\bradian\\b" << "\\bdegrees\\b"
        << "\\bsin\\b" << "\\bcos\\b" << "\\btan\\b"
        << "\\basin\\b" << "\\bacos\\b" << "\\batan\\b"
        << "\\bsinh\\b" << "\\bcosh\\b" << "\\btanh\\b"
        << "\\basinh\\b" << "\\bacosh\\b" << "\\batanh\\b"

        //Exponantial functions
        << "\\bpow\\b" << "\\bexp\\b" << "\\bexp2\\b"
        << "\\blog\\b" << "\\blog2\\b"
        << "\\bsqrt\\b" << "\\binversesqrt\\b"

        //Common Functions
        << "\\babs\\b" << "\\bsign\\b"<< "\\bfloor\\b" << "\\btrunc\\b"
        << "\\bround\\b" << "\\broundEven\\b"<< "\\bceil\\b" << "\\bfract\\b"
        << "\\bmod\\b" << "\\bmodf\\b"<< "\\bmin\\b" << "\\bmax\\b"
        << "\\bclamp\\b" << "\\bmix\\b"<< "\\bstep\\b" << "\\bsmoothStep\\b"
        << "\\bisnan\\b" << "\\bisinf\\b"<< "\\bfloatBitsToInt\\b" << "\\bfloatBitsToUint\\b"
        << "\\bintBitsToFloat\\b" << "\\buintBitsToFloat\\b"
        << "\\bfma\\b" << "\\bfrexp\\b" << "\\bIdexp\\b"

        //Floating-Point Pack and Unpack Functions
        << "\\bpackUnorm2x16\\b" << "\\bpackUnorm4x8\\b" << "\\bpackSnorm4x8\\b"
        << "\\bunpackUnorm2x16\\b" << "\\bunpackUnorm4x8\\b" << "\\bunpackSnorm4x8\\b"
        << "\\bpackDouble2x32\\b" << "\\bunpackDouble2x32\\b"

        // Geometric Functions
        << "\\blength\\b" << "\\bdistance\\b"<< "\\bdot\\b" << "\\bcross\\b"
        << "\\bnormalize\\b" << "\\bftransform\\b"<< "\\bfaceforward\\b" << "\\breflect\\b"<< "\\brefract\\b"

        // Matrix Functions
        << "\\bmatrixCompMult\\b"<< "\\bouterProduct\\b" << "\\btranspose\\b"
        << "\\bdeterminant\\b" << "\\binverse\\b"

        // Vector Relational Functions
        << "\\blessThan\\b" << "\\blessThanEqual\\b"<< "\\bgreaterThan\\b" << "\\bgreaterThanEqual\\b"
        << "\\bequal\\b" << "\\bnotEqual\\b" << "\\bany\\b" << "\\ball\\b" << "\\bnot\\b"

        // Integer Functions
        << "\\buaddCarry\\b" << "\\busubBorrow\\b"<< "\\bumulExtended\\b" << "\\bimulExtended\\b"
        << "\\bbitfieldExtract\\b" << "\\bbitfieldInsert\\b" << "\\bbitfieldReverse\\b"
        << "\\bbitCount\\b" << "\\bfindLSB\\b"<< "\\bfindMSB\\b"

        // Texture Functions
        << "\\btextureSize\\b" << "\\btextureQueryLod\\b"<< "\\btexture\\b"
        << "\\btextureProj\\b" << "\\btextureLod\\b"<< "\\btextureOffset\\b"
        << "\\btexelFetch\\b" << "\\btexelFetchOffset\\b"<< "\\btextureProjOffset\\b"
        << "\\btextureLodOffset\\b" << "\\btextureProjLod\\b"<< "\\btextureProjLodOffset\\b"
        << "\\btextureGrad\\b" << "\\btextureGradOffset\\b"<< "\\btextureProjGrad\\b"
        << "\\btextureProjGradOffset\\b" << "\\btextureGather\\b" << "\\btextureGatherOffset\\b"
        << "\\btexture1D\\b" << "\\btexture1DProj\\b" << "\\btexture1DLod\\b"<< "\\btexture1DProjLod\\b"
        << "\\btexture2D\\b" << "\\btexture2DProj\\b" << "\\btexture2DLod\\b"<< "\\btexture2DProjLod\\b"
        << "\\btexture3D\\b" << "\\btexture3DProj\\b" << "\\btexture3DLod\\b"<< "\\btexture3DProjLod\\b"
        << "\\btextureCube\\b" << "\\btextureCubeLod\\b"
        << "\\bshadow1D\\b" << "\\btshadow1DProj\\b" << "\\bshadow1DLod\\b"<< "\\bshadow1DProjLod\\b"
        << "\\bshadow2D\\b" << "\\btshadow2DProj\\b" << "\\bshadow2DLod\\b"<< "\\bshadow2DProjLod\\b"

        // Fragment Processing Functions
        << "\\bdFdx\\b" << "\\bdFdy\\b" << "\\bfwidth\\b"
        // Interpolation function
        << "\\binterpolateAtCentroid\\b" << "\\binterpolateAtSample\\b" << "\\binterpolateAtOffset\\b"
        //Noise Function
        << "\\bnoise1\\b" << "\\bnoise2\\b" << "\\bnoise3\\b"<< "\\bnoise4\\b"
        << "\\bEmitStreamVertex\\b" << "\\bEndStreamPrimitive\\b" << "\\bEmitVertex\\b"<< "\\bEndPrimitive\\b"
        << "\\bbarrier\\b" ;

    foreach (const QString &pattern, builtInFunctions)
    {
        rule2.pattern = QRegExp(pattern);
        rule2.format = keywordFormat;
        highlightingRules.append(rule2);
    }

    HighlightingRule rule3;
    keywordFormat.setForeground(Qt::darkMagenta);
    keywordFormat.setFontWeight(QFont::Courier);
    QStringList glKeywordList;
    glKeywordList << "\\bgl_[A-Za-z0-9]+\\b" << "\\bGL_[A-Za-z]+\\b";

    foreach (const QString &pattern, glKeywordList)
    {
        rule3.pattern = QRegExp(pattern);
        rule3.format = keywordFormat;
        highlightingRules.append(rule3);
    }

    HighlightingRule rule4;
    keywordFormat.setForeground(Qt::darkCyan);
    keywordFormat.setFontWeight(QFont::Courier);
    QStringList specialVariableList;
    specialVariableList << "\\buniform\\b" << "\\battribute\\b" << "\\bvarying\\b";

    foreach (const QString &pattern, specialVariableList)
    {
        rule4.pattern = QRegExp(pattern);
        rule4.format = keywordFormat;
        highlightingRules.append(rule4);
    }

    HighlightingRule rule5;
    keywordFormat.setForeground(Qt::darkGreen);
    keywordFormat.setFontWeight(QFont::Courier);
    QStringList commentList;
    commentList << "\".*\"" << "//[^\n]*" ;

    foreach (const QString &pattern, commentList)
    {
        rule5.pattern = QRegExp(pattern);
        rule5.format = keywordFormat;
        highlightingRules.append(rule5);
    }

	commentStartExpression = QRegExp("/\\*");
    commentEndExpression = QRegExp("\\*/");
    multiLineCommentFormat.setForeground(Qt::darkGreen);
}


SyntaxHighlight::~SyntaxHighlight()
{
    //Do Nothing
}

void SyntaxHighlight::highlightBlock(const QString &text)
{
    foreach (const HighlightingRule &rule, highlightingRules) {
        QRegExp expression(rule.pattern);
        int index = expression.indexIn(text);
        while (index >= 0) {
            int length = expression.matchedLength();
            setFormat(index, length, rule.format);
            index = expression.indexIn(text, index + length);
        }
    }
	setCurrentBlockState(0);
    int startIndex = 0;
    if (previousBlockState() != 1)
        startIndex = commentStartExpression.indexIn(text);

    while (startIndex >= 0) {
        int endIndex = commentEndExpression.indexIn(text, startIndex);
        int commentLength;
        if (endIndex == -1) {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        } else {
            commentLength = endIndex - startIndex
                            + commentEndExpression.matchedLength();
        }
        setFormat(startIndex, commentLength, multiLineCommentFormat);
        startIndex = commentStartExpression.indexIn(text, startIndex + commentLength);
    }
}


