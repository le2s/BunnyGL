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

#ifndef I_VIS_NETWORK_CANVAS_NOTE_H
#define I_VIS_NETWORK_CANVAS_NOTE_H

#include <GCF/IContainer>
#include <QColor>
#include <QFont>

class QSvgRenderer;
class IVisNetworkCanvasNote : virtual public IContainer
{
public:
    // Graphics item associated with the note
    virtual QGraphicsItem* graphicsItem() = 0;

    // Configure the position and size of the note item in one shot
    virtual void setNoteRect(const QRectF& rect) = 0;
    virtual QRectF noteRect() const = 0;

    // Configure the note's appearance
    virtual void setNoteSvgRenderer(QSvgRenderer* ren) = 0;
    virtual QSvgRenderer* noteSvgRenderer() const = 0;

    virtual void setNoteSvgElementId(const QString& id) = 0;
    virtual QString noteSvgElementId() const = 0;

    // Configure the note's text
    virtual void setNoteText(const QString& text) = 0;
    virtual QString noteText() const = 0;

    virtual void setNoteTextColor(const QColor& noteColor) = 0;
    virtual QColor noteTextColor() const = 0;

    virtual void setNoteFont(const QFont& font) = 0;
    virtual QFont noteFont() const = 0;

    // Events
    virtual void noteRectChanged() = 0;
    virtual void noteTextChanged() = 0;
    virtual void noteTextEdited() = 0;
};
Q_DECLARE_INTERFACE(IVisNetworkCanvasNote, "com.vcreatelogic.IVisNetworkCanvasNote/1.0")

Q_DECLARE_METATYPE(IVisNetworkCanvasNote*);

#endif
