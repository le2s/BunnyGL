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

#ifndef I_VTK_SCENE_EXPORTER_H
#define I_VTK_SCENE_EXPORTER_H

#include <GCF/IContainer>

class vtkRenderWindow;
class IVtkSceneExporter : virtual public IContainer
{
public:
    // Methods to export a scene
    virtual bool exportScene(vtkRenderWindow* renderWindow, const QString& fileName) = 0;

    // Must be implemented as signals
    virtual void exportProgress(int percent, const QString& msg) = 0;
};
Q_DECLARE_INTERFACE(IVtkSceneExporter, "com.vcreatelogic.IVtkSceneExporter/1.0");

#endif

