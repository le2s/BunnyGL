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

#ifndef C_VTK_CUSTOM_POLY_DATA_SOURCE_H
#define C_VTK_CUSTOM_POLY_DATA_SOURCE_H

#include "CVtkPolyDataAlgorithm.h"
#include "vtkPolyDataSource.h"
#include "IProjectPart.h"

class CScriptableVtkPolyData;
class vtkCustomPolyDataSource;
class CVtkCustomPolyDataSource : public CVtkPolyDataAlgorithm,
                                 virtual public IProjectPart
{
    Q_OBJECT
    Q_INTERFACES(IProjectPart)
    Q_PROPERTY(bool StoreData READ isStoreData WRITE setStoreData)
    DECLARE_VTK_OBJECT

public:
    CVtkCustomPolyDataSource();
    ~CVtkCustomPolyDataSource();

    // Q_INVOKABLE CScriptableVtkPolyData* polyDataObject();
    Q_INVOKABLE QObject* polyDataObject();

    // Shortcut methods to deal with custom polydata.
    Q_INVOKABLE QList<double> points() const;
    Q_INVOKABLE void setPoints(QList<double> pArray);

    Q_INVOKABLE void setVerts(QList<int> v);
    Q_INVOKABLE QList<int> verts() const;

    Q_INVOKABLE void setLines(QList<int> lines);
    Q_INVOKABLE QList<int> lines() const;

    Q_INVOKABLE void setStrips(QList<int> strips);
    Q_INVOKABLE QList<int> strips() const;

    Q_INVOKABLE void setPolys(QList<int> polys);
    Q_INVOKABLE QList<int> polys() const;

    Q_INVOKABLE QList<double> cellNormals() const;
    Q_INVOKABLE void setCellNormals(QList<double> normalsArray);

    Q_INVOKABLE QList<double> cellScalars() const;
    Q_INVOKABLE void setCellScalars(QList<double> normalsArray);

    Q_INVOKABLE QList<double> pointNormals() const;
    Q_INVOKABLE void setPointNormals(QList<double> normalsArray);

    Q_INVOKABLE QList<double> pointScalars() const;
    Q_INVOKABLE void setPointScalars(QList<double> normalsArray);

    Q_INVOKABLE int numberOfPoints() const;
    Q_INVOKABLE int numberOfVerts() const;
    Q_INVOKABLE int numberOfLines() const;
    Q_INVOKABLE int numberOfStrips() const;
    Q_INVOKABLE int numberOfPolys() const;

signals:
    void OnRequestData();

public:
    void setStoreData(bool val);
    bool isStoreData() const;

    // IContainer implementation, inherited from CVtkObject

    // IProjectPart implementation
    QString projectPartID();
    bool newProjectPart();
    bool loadProjectPart(QDomDocument doc, QDomElement partE);
    bool initializeProjectPart();
    bool saveProjectPart(QDomDocument doc, QDomElement partParentE);
    bool closeProjectPart();

signals:
    void projectPartModified();

private:
    friend class vtkCustomPolyDataSource;
    vtkCustomPolyDataSource* m_vtkCustomPolyDataSource;
    CScriptableVtkPolyData* m_polyDataObject;
    bool m_storeData;
};

class vtkCustomPolyDataSource : public vtkPolyDataAlgorithm
{
public:
    static vtkCustomPolyDataSource *New();
    vtkTypeRevisionMacro(vtkCustomPolyDataSource, vtkPolyDataAlgorithm);
    void PrintSelf(ostream& os, vtkIndent indent);

    void SetCustomPolyDataSource(CVtkCustomPolyDataSource* source);
    CVtkCustomPolyDataSource* GetCustomPolyDataSource();

    void SetCustomPolyDataOutput(vtkPolyData* polyData);

protected:
    vtkCustomPolyDataSource();
    ~vtkCustomPolyDataSource();
    int RequestData(vtkInformation *, vtkInformationVector **, vtkInformationVector *);

private:
    vtkCustomPolyDataSource(const vtkCustomPolyDataSource&);  // Not implemented.
    void operator=(const vtkCustomPolyDataSource&);  // Not implemented.
    CVtkCustomPolyDataSource* m_CVtkCustomPolyDataSource;
    int RequestingData;
    vtkPolyData* CustomPolyDataOutput;
};

#endif

