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

#ifndef C_VTK_FULLSCREEN_COMPONENT_H
#define C_VTK_FULLSCREEN_COMPONENT_H

#include <GCF/AbstractComponent>

class IVisSystemNode;

struct CVTKFullscreenComponentData;
class CVTKFullscreenComponent : public GCF::AbstractComponent
{
    Q_OBJECT
    GCF_DECLARE_COMPONENT(CVTKFullscreenComponent)

public:
    static CVTKFullscreenComponent & instance();
    ~CVTKFullscreenComponent();

public slots:
    void showCurrentRenderWindowFullScreen();

protected:
    CVTKFullscreenComponent();
    QObject* fetchObject(const QString& completeName) const;
    void initializeComponent();
    void finalizeComponent();
    void creationAnnounced();

protected slots:
    void slotNodeAdded(IVisSystemNode* node);
    void slotNodeRemoved(IVisSystemNode* node);
    void slotWorkspaceWidgetActivated(QWidget* widget);

private:
    CVTKFullscreenComponentData* d;
};

#endif

