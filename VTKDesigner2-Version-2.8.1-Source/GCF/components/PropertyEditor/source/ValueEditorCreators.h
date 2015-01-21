/****************************************************************************
**
** Copyright (C) VCreate Logic Private Limited, Bangalore
**
** Use of this file is limited according to the terms specified by
** VCreate Logic Private Limited, Bangalore.  Details of those terms
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

#ifndef VALUE_EDITOR_CREATORS_H
#define VALUE_EDITOR_CREATORS_H

#include "IValueEditorCreator.h"
#include "IEnumValueEditorCreator.h"
#include <QWidget>
#include <QToolButton>

namespace GCF
{
namespace Components
{
class EnumEditorCreator : virtual public IEnumValueEditorCreator
{
public:
    EnumEditorCreator();
    ~EnumEditorCreator();

    // IQueryable implementation
    bool queryInterface(QString iface, void** ifacePtr);
    bool destroySelf();

    // IValueEditorCreator implementation
    const char* typeName();
    QWidget* createEditor(QWidget* parent);
    void connectEditor(QWidget* editor, QObject* receiver, const char* slot);
    bool setEditorValue(QWidget* editor, QVariant value);
    QVariant editorValue(QWidget* editor);
    void closeEditor(QWidget* editor);
    QString toString(QVariant value);
    QVariant fromString(QString value);
    bool paintValue(QPainter* paint, QRect rect, QVariant value);

    // IEnumValueEditorCreator
    void setCurrentEnum(QMetaEnum enumStruct) { mEnumStruct = enumStruct; }
    void initialize(QWidget* editor, QMetaEnum enumStruct);

private:
    int indexFromValue(int value);

private:
    QMetaEnum mEnumStruct;
};

class NumberEditorCreator : virtual public IValueEditorCreator
{
public:
    NumberEditorCreator(int type, const QByteArray& typeName);
    ~NumberEditorCreator();

    // IQueryable implementation
    bool queryInterface(QString iface, void** ifacePtr);
    bool destroySelf();

    // IValueEditorCreator implementation
    const char* typeName();
    QWidget* createEditor(QWidget* parent);
    void connectEditor(QWidget* editor, QObject* receiver, const char* slot);
    bool setEditorValue(QWidget* editor, QVariant value);
    QVariant editorValue(QWidget* editor);
    void closeEditor(QWidget* editor);
    QString toString(QVariant value);
    QVariant fromString(QString value);
    bool paintValue(QPainter* paint, QRect rect, QVariant value);

private:
    bool isIntegerType() const;

private:
    int mType;
    QByteArray mTypeName;
};

class BoolEditorCreator : virtual public IValueEditorCreator
{
public:
    // IQueryable implementation
    bool queryInterface(QString iface, void** ifacePtr);
    bool destroySelf();

    // IValueEditorCreator implementation
    const char* typeName();
    QWidget* createEditor(QWidget* parent);
    void connectEditor(QWidget* editor, QObject* receiver, const char* slot);
    bool setEditorValue(QWidget* editor, QVariant value);
    QVariant editorValue(QWidget* editor);
    void closeEditor(QWidget* editor);
    QString toString(QVariant value);
    QVariant fromString(QString value);
    bool paintValue(QPainter* paint, QRect rect, QVariant value);
};

class StringEditorCreator : virtual public IValueEditorCreator
{
public:
    // IQueryable implementation
    bool queryInterface(QString iface, void** ifacePtr);
    bool destroySelf();

    // IValueEditorCreator implementation
    const char* typeName();
    QWidget* createEditor(QWidget* parent);
    void connectEditor(QWidget* editor, QObject* receiver, const char* slot);
    bool setEditorValue(QWidget* editor, QVariant value);
    QVariant editorValue(QWidget* editor);
    void closeEditor(QWidget* editor);
    QString toString(QVariant value);
    QVariant fromString(QString value);
    bool paintValue(QPainter* paint, QRect rect, QVariant value);
};

class PointEditorCreator : virtual public IValueEditorCreator
{
public:
    // IQueryable implementation
    bool queryInterface(QString iface, void** ifacePtr);
    bool destroySelf();

    // IValueEditorCreator implementation
    const char* typeName();
    QWidget* createEditor(QWidget* parent);
    void connectEditor(QWidget* editor, QObject* receiver, const char* slot);
    bool setEditorValue(QWidget* editor, QVariant value);
    QVariant editorValue(QWidget* editor);
    void closeEditor(QWidget* editor);
    QString toString(QVariant value);
    QVariant fromString(QString value);
    bool paintValue(QPainter* paint, QRect rect, QVariant value);
};

class VectorEditorCreator : virtual public IValueEditorCreator
{
public:
    // IQueryable implementation
    bool queryInterface(QString iface, void** ifacePtr);
    bool destroySelf();

    // IValueEditorCreator implementation
    const char* typeName();
    QWidget* createEditor(QWidget* parent);
    void connectEditor(QWidget* editor, QObject* receiver, const char* slot);
    bool setEditorValue(QWidget* editor, QVariant value);
    QVariant editorValue(QWidget* editor);
    void closeEditor(QWidget* editor);
    QString toString(QVariant value);
    QVariant fromString(QString value);
    bool paintValue(QPainter* paint, QRect rect, QVariant value);
};

class TriangleEditorCreator : virtual public IValueEditorCreator
{
public:
    // IQueryable implementation
    bool queryInterface(QString iface, void** ifacePtr);
    bool destroySelf();

    // IValueEditorCreator implementation
    const char* typeName();
    QWidget* createEditor(QWidget* parent);
    void connectEditor(QWidget* editor, QObject* receiver, const char* slot);
    bool setEditorValue(QWidget* editor, QVariant value);
    QVariant editorValue(QWidget* editor);
    void closeEditor(QWidget* editor);
    QString toString(QVariant value);
    QVariant fromString(QString value);
    bool paintValue(QPainter* paint, QRect rect, QVariant value);
};

class BoundingBoxEditorCreator : virtual public IValueEditorCreator
{
public:
    // IQueryable implementation
    bool queryInterface(QString iface, void** ifacePtr);
    bool destroySelf();

    // IValueEditorCreator implementation
    const char* typeName();
    QWidget* createEditor(QWidget* parent);
    void connectEditor(QWidget* editor, QObject* receiver, const char* slot);
    bool setEditorValue(QWidget* editor, QVariant value);
    QVariant editorValue(QWidget* editor);
    void closeEditor(QWidget* editor);
    QString toString(QVariant value);
    QVariant fromString(QString value);
    bool paintValue(QPainter* paint, QRect rect, QVariant value);
};

class ValueRangeEditorCreator : virtual public IValueEditorCreator
{
public:
    // IQueryable implementation
    bool queryInterface(QString iface, void** ifacePtr);
    bool destroySelf();

    // IValueEditorCreator implementation
    const char* typeName();
    QWidget* createEditor(QWidget* parent);
    void connectEditor(QWidget* editor, QObject* receiver, const char* slot);
    bool setEditorValue(QWidget* editor, QVariant value);
    QVariant editorValue(QWidget* editor);
    void closeEditor(QWidget* editor);
    QString toString(QVariant value);
    QVariant fromString(QString value);
    bool paintValue(QPainter* paint, QRect rect, QVariant value);
};

class ListEditorCreator : virtual public IValueEditorCreator
{
public:
    ListEditorCreator(bool doubleList=false);

    // IQueryable implementation
    bool queryInterface(QString iface, void** ifacePtr);
    bool destroySelf();

    // IValueEditorCreator implementation
    const char* typeName();
    QWidget* createEditor(QWidget* parent);
    void connectEditor(QWidget* editor, QObject* receiver, const char* slot);
    bool setEditorValue(QWidget* editor, QVariant value);
    QVariant editorValue(QWidget* editor);
    void closeEditor(QWidget* editor);
    QString toString(QVariant value);
    QVariant fromString(QString value);
    bool paintValue(QPainter* paint, QRect rect, QVariant value);

private:
    bool IsDoubleList;
};

class ColorEditorCreator : virtual public IValueEditorCreator
{
public:
    // IQueryable implementation
    bool queryInterface(QString iface, void** ifacePtr);
    bool destroySelf();

    // IValueEditorCreator implementation
    const char* typeName();
    QWidget* createEditor(QWidget* parent);
    void connectEditor(QWidget* editor, QObject* receiver, const char* slot);
    bool setEditorValue(QWidget* editor, QVariant value);
    QVariant editorValue(QWidget* editor);
    void closeEditor(QWidget* editor);
    QString toString(QVariant value);
    QVariant fromString(QString value);
    bool paintValue(QPainter* paint, QRect rect, QVariant value);
};

class FileNameEditorCreator : virtual public IValueEditorCreator
{
public:
    // IQueryable implementation
    bool queryInterface(QString iface, void** ifacePtr);
    bool destroySelf();

    // IValueEditorCreator implementation
    const char* typeName();
    QWidget* createEditor(QWidget* parent);
    void connectEditor(QWidget* editor, QObject* receiver, const char* slot);
    bool setEditorValue(QWidget* editor, QVariant value);
    QVariant editorValue(QWidget* editor);
    void closeEditor(QWidget* editor);
    QString toString(QVariant value);
    QVariant fromString(QString value);
    bool paintValue(QPainter* paint, QRect rect, QVariant value);
};

class FontEditorCreator : virtual public IValueEditorCreator
{
public:
    // IQueryable implementation
    bool queryInterface(QString iface, void** ifacePtr);
    bool destroySelf();

    // IValueEditorCreator implementation
    const char* typeName();
    QWidget* createEditor(QWidget* parent);
    void connectEditor(QWidget* editor, QObject* receiver, const char* slot);
    bool setEditorValue(QWidget* editor, QVariant value);
    QVariant editorValue(QWidget* editor);
    void closeEditor(QWidget* editor);
    QString toString(QVariant value);
    QVariant fromString(QString value);
    bool paintValue(QPainter* paint, QRect rect, QVariant value);
};

class SizeEditorCreator : virtual public IValueEditorCreator
{
public:
    // IQueryable implementation
    bool queryInterface(QString iface, void** ifacePtr);
    bool destroySelf();

    // IValueEditorCreator implementation
    const char* typeName();
    QWidget* createEditor(QWidget* parent);
    void connectEditor(QWidget* editor, QObject* receiver, const char* slot);
    bool setEditorValue(QWidget* editor, QVariant value);
    QVariant editorValue(QWidget* editor);
    void closeEditor(QWidget* editor);
    QString toString(QVariant value);
    QVariant fromString(QString value);
    bool paintValue(QPainter* paint, QRect rect, QVariant value);
};

class SizeFEditorCreator : virtual public IValueEditorCreator
{
public:
    // IQueryable implementation
    bool queryInterface(QString iface, void** ifacePtr);
    bool destroySelf();

    // IValueEditorCreator implementation
    const char* typeName();
    QWidget* createEditor(QWidget* parent);
    void connectEditor(QWidget* editor, QObject* receiver, const char* slot);
    bool setEditorValue(QWidget* editor, QVariant value);
    QVariant editorValue(QWidget* editor);
    void closeEditor(QWidget* editor);
    QString toString(QVariant value);
    QVariant fromString(QString value);
    bool paintValue(QPainter* paint, QRect rect, QVariant value);
};

class Point2DEditorCreator : virtual public IValueEditorCreator
{
public:
    // IQueryable implementation
    bool queryInterface(QString iface, void** ifacePtr);
    bool destroySelf();

    // IValueEditorCreator implementation
    const char* typeName();
    QWidget* createEditor(QWidget* parent);
    void connectEditor(QWidget* editor, QObject* receiver, const char* slot);
    bool setEditorValue(QWidget* editor, QVariant value);
    QVariant editorValue(QWidget* editor);
    void closeEditor(QWidget* editor);
    QString toString(QVariant value);
    QVariant fromString(QString value);
    bool paintValue(QPainter* paint, QRect rect, QVariant value);
};

class PointF2DEditorCreator : virtual public IValueEditorCreator
{
public:
    // IQueryable implementation
    bool queryInterface(QString iface, void** ifacePtr);
    bool destroySelf();

    // IValueEditorCreator implementation
    const char* typeName();
    QWidget* createEditor(QWidget* parent);
    void connectEditor(QWidget* editor, QObject* receiver, const char* slot);
    bool setEditorValue(QWidget* editor, QVariant value);
    QVariant editorValue(QWidget* editor);
    void closeEditor(QWidget* editor);
    QString toString(QVariant value);
    QVariant fromString(QString value);
    bool paintValue(QPainter* paint, QRect rect, QVariant value);
};


struct UrlEditData;
class UrlEdit : public QWidget
{
    Q_OBJECT

public:
    UrlEdit(QWidget* parent=0);
    ~UrlEdit();

    void setUrl(QUrl url);
    QUrl url() const;

protected slots:
    void on_urlButton_clicked();
    void on_clearButton_clicked();

signals:
    void urlChanged(const QUrl&);

protected:
    void paintEvent(QPaintEvent* pe);
    void resizeEvent(QResizeEvent* re);

private:
    UrlEditData* d;
};

struct ColorEditData;
class ColorEdit : public QToolButton
{
    Q_OBJECT

public:
    ColorEdit(QWidget* parent=0);
    ~ColorEdit();

    void setColor(QColor color);
    QColor color() const;

protected slots:
    void on_currentColorChanged(QColor color);

signals:
    void colorChanged(QColor color);

private:
    ColorEditData* d;
};

struct FontEditData;
class FontEdit : public QToolButton
{
    Q_OBJECT

public:
    FontEdit(QWidget* parent=0);
    ~FontEdit();

    void setFont(QFont font);
    QFont font() const;

protected slots:
    void on_currentFontChanged(QFont font);
    void on_clicked();

signals:
    void fontChanged(QFont font);

private:
    FontEditData* d;
};

struct SizeEditData;
class SizeEdit : public QWidget
{
    Q_OBJECT

public:
    SizeEdit(QWidget* parent=0);
    ~SizeEdit();

    void setSize(QSizeF size);
    QSizeF size() const;

protected slots:
    void on_currentSizeChanged(QSizeF size);
    void on_editorTextChanged();

signals:
    void sizeChanged(QSizeF size);
    void sizeChanged(QSize size);

private:
    SizeEditData* d;
};

struct Point2DEditData;
class Point2DEdit : public QWidget
{
    Q_OBJECT

public:
    Point2DEdit(QWidget* parent=0);
    ~Point2DEdit();

    void setPoint(QPointF size);
    QPointF point() const;

protected slots:
    void on_currentPoint2DChanged(QPointF size);
    void on_editorTextChanged();

signals:
    void point2DChanged(QPointF size);
    void point2DChanged(QPoint size);

private:
    Point2DEditData* d;
};

}
}

#endif
