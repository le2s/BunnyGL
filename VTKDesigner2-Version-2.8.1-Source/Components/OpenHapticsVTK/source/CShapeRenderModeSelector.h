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

#ifndef C_SHAPE_RENDER_MODE_SELECTOR_H
#define C_SHAPE_RENDER_MODE_SELECTOR_H

#include <QWidget>
#include "ui_CShapeRenderModeSelector.h"

class CShapeRenderModeSelector : public QWidget
{
    Q_OBJECT

public:
    CShapeRenderModeSelector(QWidget* parent=0);
    ~CShapeRenderModeSelector();

protected slots:
    void on_rbDepthBuffer_toggled(bool val);
    void on_rbFeedbackBuffer_toggled(bool val);

protected:
    void showEvent(QShowEvent* se);

private:
    Ui::CShapeRenderModeSelector ui;
};

#endif
