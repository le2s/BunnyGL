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

#ifndef I_VIS_NETWORK_CANVAS_NOTES_EXT_H
#define I_VIS_NETWORK_CANVAS_NOTES_EXT_H

#include <GCF/IContainer>
#include <QMetaType>

#include "IVisNetworkCanvasNote.h"

class IVisNetworkCanvasNotesExt : virtual public IContainer
{
public:
    // Change the background note SVG
    virtual void setNoteSvgFile(const QString& noteSvg, const QString& elemId=QString(), bool qcompressed=true) = 0;
    virtual QString noteSvgFile() const = 0;
    virtual QString noteSvgElement() const = 0;

    // Add/remove notes
    virtual void addNote(IVisNetworkCanvasNote* note) = 0;
    virtual void removeNote(IVisNetworkCanvasNote* note) = 0;
    virtual int noteCount() const = 0;
    virtual IVisNetworkCanvasNote* note(int index) const = 0;

    // Show/hide notes
    virtual void showNotes() = 0;
    virtual void hideNotes() = 0;

    // Events
    virtual void noteAdded(IVisNetworkCanvasNote* note) = 0;
    virtual void noteRemoved(IVisNetworkCanvasNote* note) = 0;
};

Q_DECLARE_INTERFACE(IVisNetworkCanvasNotesExt, "com.vcreatelogic.IVisNetworkCanvasNotesExt/1.0")

#endif
