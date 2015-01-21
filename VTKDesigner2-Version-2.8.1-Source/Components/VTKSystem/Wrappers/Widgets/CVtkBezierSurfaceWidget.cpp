/****************************************************************************
**
** Copyright (C)    VCreate Logic Private Limited, Bangalore
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

#include "CVtkBezierSurfaceWidget.h"
#include "IVtkAlgorithmIOData.h"
#include "vtkBezierSurfaceSource.h"
#include <QApplication>

DEFINE_VTK_OBJECT(CVtkBezierSurfaceWidget, CVtk3DWidget, vtkBezierSurfaceWidget)
{
    pDesc->setNodeClassName("vtkBezierSurfaceWidget");
    pDesc->setNodeClassCategory("Interactor Observers");
    pDesc->setNodeIcon(qApp->windowIcon());
    pDesc->setNodeClassDescription("Provides a bezier surface control point manipulation");

    // PolyData output
    pDesc->addConnectionPath(
        new CVtkObjectConnectionPath("vtkBezierSurfaceSource", IVisSystemNodeConnectionPath::InputPath, "vtkAlgorithm", 0)
    );
};

CVtkBezierSurfaceWidget::CVtkBezierSurfaceWidget()
{
    m_vtkBezierSurfaceWidget = 0;
    setVtkObject(vtkBezierSurfaceWidget::New());
}

CVtkBezierSurfaceWidget::~CVtkBezierSurfaceWidget()
{

}

bool CVtkBezierSurfaceWidget::hasInput(IVisSystemNodeConnectionPath* path)
{
    if(!path)
        return false;

    if(path->pathName() == "vtkBezierSurfaceSource")
        return m_vtkBezierSurfaceWidget->GetSource() != 0;

    // Disallow inputs along prop3d and dataset
    if(path->pathName() == "Prop3D" || path->pathName() == "DataSet")
        return false;

    return CVtk3DWidget::hasInput(path);
}

bool CVtkBezierSurfaceWidget::setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    if(path->pathName() == "vtkBezierSurfaceSource")
    {
        IVtkAlgorithmIOData* data = 0;
        bool success = inputData->queryInterface("IVtkAlgorithmIOData", (void**)&data);
        if(!success || !data || !data->getVtkAlgorithm())
            return false;

        vtkBezierSurfaceSource* bsSource = vtkBezierSurfaceSource::SafeDownCast(data->getVtkAlgorithm());
        if(!bsSource)
            return false;

        m_vtkBezierSurfaceWidget->SetSource(bsSource);
        return true;
    }

    // Disallow inputs along prop3d and dataset
    if(path->pathName() == "Prop3D" || path->pathName() == "DataSet")
    {
        qDebug("Input not accepted along %s", qPrintable(path->pathName()));
        return false;
    }

    return CVtk3DWidget::setInput(path, inputData);
}

bool CVtkBezierSurfaceWidget::removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    if(path->pathName() == "vtkBezierSurfaceSource")
    {
        m_vtkBezierSurfaceWidget->SetSource(0);
        return true;
    }

    // Disallow inputs along prop3d and dataset
    if(path->pathName() == "Prop3D" || path->pathName() == "DataSet")
        return true; // input is not accepted anyway, so dont worry

    return CVtk3DWidget::removeInput(path, inputData);
}


