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

#include "CShapeRenderModeSelector.h"
#include "CVtkOpenHapticsRenderer.h"

CShapeRenderModeSelector::CShapeRenderModeSelector(QWidget* parent)
: QWidget(parent)
{
    ui.setupUi(this);
}

CShapeRenderModeSelector::~CShapeRenderModeSelector()
{

}

void CShapeRenderModeSelector::on_rbDepthBuffer_toggled(bool val)
{
    if(!val)
        return;

    CVtkOpenHapticsRenderer* ren = CVtkOpenHapticsRenderer::GetInstancePointer();
    if(ren)
        ren->setShapeRendering(CVtkOpenHapticsRenderer::DepthBuffer);
}

void CShapeRenderModeSelector::on_rbFeedbackBuffer_toggled(bool val)
{
    if(!val)
        return;

    CVtkOpenHapticsRenderer* ren = CVtkOpenHapticsRenderer::GetInstancePointer();
    if(ren)
        ren->setShapeRendering(CVtkOpenHapticsRenderer::FeedbackBuffer);
}

void CShapeRenderModeSelector::showEvent(QShowEvent* se)
{
    CVtkOpenHapticsRenderer* ren = CVtkOpenHapticsRenderer::GetInstancePointer();

    if(ren)
        setEnabled(true);
    else
        setEnabled(false);

    Q_UNUSED(se);
}




