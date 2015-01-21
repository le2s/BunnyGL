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

#ifndef I_VTK_XML_UTILS_H
#define I_VTK_XML_UTILS_H

#include <GCF/IContainer>
#include <QDomElement>
#include <QDomDocument>

class vtkPoints;
class vtkObject;
class vtkCellArray;
class vtkDataArray;
class vtkPolyData;
class vtkPointSet;
class vtkImageData;

class IVtkXmlUtils : virtual public IContainer
{
public:
    virtual void savePoints(vtkPoints* points, QDomDocument doc, QDomElement ele) = 0;
    virtual void loadPoints(vtkPoints* points, QDomDocument doc, QDomElement ele) = 0;

    virtual void saveCells(vtkCellArray* cells, QDomDocument doc, QDomElement ele) = 0;
    virtual void loadCells(vtkCellArray* cells, QDomDocument doc, QDomElement ele) = 0;

    virtual void saveData(vtkDataArray* dataArray, QDomDocument doc, QDomElement ele) = 0;
    virtual void loadData(vtkDataArray* dataArray, QDomDocument doc, QDomElement ele) = 0;

    virtual void savePolyData(vtkPolyData* polyData, QDomDocument doc, QDomElement ele) = 0;
    virtual void loadPolyData(vtkPolyData* polyData, QDomDocument doc, QDomElement ele) = 0;

    virtual void saveImageData(vtkImageData* imageData, QDomDocument doc, QDomElement ele) = 0;
    virtual void loadImageData(vtkImageData* imageData, QDomDocument doc, QDomElement ele) = 0;
};

Q_DECLARE_INTERFACE(IVtkXmlUtils, "com.vcreatelogic.IVtkXmlUtils/1.0");

#endif

