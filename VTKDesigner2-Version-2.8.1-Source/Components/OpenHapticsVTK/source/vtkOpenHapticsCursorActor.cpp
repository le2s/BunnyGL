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

#include "vtkOpenHapticsCursorActor.h"
#include "vtkOpenHapticsRenderer.h"
#include "vtkOutputWindow.h"
#include "vtkMath.h"

#include "HL/hl.h"
#include "HD/hd.h"
#include "GL/gl.h"

vtkOpenHapticsCursorActor* vtkOpenHapticsCursorActor::New()
{
	return new vtkOpenHapticsCursorActor;
}

vtkOpenHapticsCursorActor::vtkOpenHapticsCursorActor()
{

}

vtkOpenHapticsCursorActor::~vtkOpenHapticsCursorActor()
{

}

void vtkOpenHapticsCursorActor::Render(vtkRenderer *ren, vtkMapper *mapper)
{
#if 0
    vtkOpenHapticsRenderer* hRen = dynamic_cast<vtkOpenHapticsRenderer*>(ren);
    if(!hRen || hRen->GetHapticsPass() != 2)
        return;

    if(hRen)
    {
#endif
        // Save the current matrix.
        glMatrixMode( GL_MODELVIEW );
        glPushMatrix();

        // Fetch the transformation matrix of the end effector.
        HLdouble proxytransform[16];
        hlGetDoublev(HL_PROXY_TRANSFORM, proxytransform);
        glMultMatrixd(proxytransform);
        
        if(!this->IsIdentity)
        {
            vtkOpenGLActor::SetPosition(0,0,0);
            vtkOpenGLActor::SetOrientation(0,0,0);
            vtkOpenGLActor::SetScale(1,1,1);
        }
#if 0
    }
#endif

	vtkOpenGLActor::Render(ren, mapper);

#if 0
    if(hRen)
    {
#endif
        // Rever to the old matrix.
        glMatrixMode( GL_MODELVIEW );
        glPopMatrix();
#if 0
    }
#endif

}

