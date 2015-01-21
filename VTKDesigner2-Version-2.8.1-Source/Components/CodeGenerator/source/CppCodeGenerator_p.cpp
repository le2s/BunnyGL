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

#include "CppCodeGenerator_p.h"
#include "UserDefinedDataTypes.h"

///////////////////////////////////////////////////////////////////////////////
// StringParamGenerator
///////////////////////////////////////////////////////////////////////////////

QObject* StringParamGenerator::containerObject()
{
    return this;
}

bool StringParamGenerator::canGenerateParams(
                 const QStringList& params,
                 const QVariant& value)
{
    return  (params == QStringList() << "string" ||
             params == QStringList() << "const char*" ||
             params == QStringList() << "QString") &&
            value.canConvert(QVariant::String);
}

bool StringParamGenerator::generateParams(
                 const QStringList& params,
                 const QVariant& value,
                 QTextStream& ts)
{
    Q_UNUSED(params);

    ts << "\"" << value.toString() <<"\"";
    return true;
}

///////////////////////////////////////////////////////////////////////////////
// BoolParamGenerator
///////////////////////////////////////////////////////////////////////////////

QObject* BoolParamGenerator::containerObject()
{
    return this;
}

bool BoolParamGenerator::canGenerateParams(
                 const QStringList& params,
                 const QVariant& value)
{
    return  params == QStringList() << "bool" &&
            value.canConvert(QVariant::Bool);
}

bool BoolParamGenerator::generateParams(
                 const QStringList& params,
                 const QVariant& value,
                 QTextStream& ts)
{
    Q_UNUSED(params);

    if(value.toBool())
        ts << "1";
    else
        ts << "0";

    return true;
}

///////////////////////////////////////////////////////////////////////////////
// IntParamGenerator
///////////////////////////////////////////////////////////////////////////////

QObject* IntParamGenerator::containerObject()
{
    return this;
}

bool IntParamGenerator::canGenerateParams(
                 const QStringList& params,
                 const QVariant& value)
{
    return  params == QStringList() << "int" &&
            value.canConvert(QVariant::Int);
}

bool IntParamGenerator::generateParams(
                 const QStringList& params,
                 const QVariant& value,
                 QTextStream& ts)
{
    Q_UNUSED(params);

    ts << value.toInt();

    return true;
}

///////////////////////////////////////////////////////////////////////////////
// IntListParamGenerator
///////////////////////////////////////////////////////////////////////////////

QObject* IntListParamGenerator::containerObject()
{
    return this;
}

bool IntListParamGenerator::canGenerateParams(
                 const QStringList& params,
                 const QVariant& value)
{
    Q_FOREACH(QString param, params)
        if( param != "int" )
            return false;

    if( !value.canConvert(QVariant::List) )
    {
        if( qstrcmp(value.typeName(), "QList<int>") )
            return false;
    }

    return true;
}

bool IntListParamGenerator::generateParams(
                 const QStringList& params,
                 const QVariant& value,
                 QTextStream& ts)
{
    Q_UNUSED(params);

    if(value.type() == QVariant::List)
    {
        QVariantList list = value.toList();

        for(int i=0; i<list.count(); i++)
        {
            QVariant item = list.at(i);
            ts << item.toInt();
            if(i+1 != list.count()-1)
                ts << ", ";
        }
    }
    else if( !qstrcmp(value.typeName(), "QList<int>") )
    {
        QList<int> vals = value.value< QList<int> >();
        for(int i=0; i<vals.count(); i++)
        {
            int item = vals.at(i);
            ts << item;
            if(i+1 != vals.count()-1)
                ts << ", ";
        }
    }
    else
        return false;

    return true;
}

///////////////////////////////////////////////////////////////////////////////
// DoubleParamGenerator
///////////////////////////////////////////////////////////////////////////////

QObject* DoubleParamGenerator::containerObject()
{
    return this;
}

bool DoubleParamGenerator::canGenerateParams(
                 const QStringList& params,
                 const QVariant& value)
{
    return  params == QStringList() << "double" &&
            value.canConvert(QVariant::Double);
}

bool DoubleParamGenerator::generateParams(
                 const QStringList& params,
                 const QVariant& value,
                 QTextStream& ts)
{
    Q_UNUSED(params);

    ts << value.toDouble();

    return true;
}

///////////////////////////////////////////////////////////////////////////////
// DoubleParamGenerator
///////////////////////////////////////////////////////////////////////////////

QObject* DoubleListParamGenerator::containerObject()
{
    return this;
}

bool DoubleListParamGenerator::canGenerateParams(
                 const QStringList& params,
                 const QVariant& value)
{
    Q_FOREACH(QString param, params)
        if(param != "double")
            return false;

    if( !value.canConvert(QVariant::List) )
    {
        // Value can be Point3D, Vector3D, BoundingBox3D
        // ValueRange, QList<double>
        if( qstrcmp(value.typeName(), "Point3D") &&
            qstrcmp(value.typeName(), "Vector3D") &&
            qstrcmp(value.typeName(), "BoundingBox3D") &&
            qstrcmp(value.typeName(), "ValueRange") &&
            qstrcmp(value.typeName(), "QList<double>") )
        return false;
    }

    return true;
}

bool DoubleListParamGenerator::generateParams(
                 const QStringList& params,
                 const QVariant& value,
                 QTextStream& ts)
{
    Q_UNUSED(params);

    if(value.type() == QVariant::List)
    {
        QVariantList list = value.toList();

        for(int i=0; i<list.count(); i++)
        {
            QVariant item = list.at(i);
            ts << item.toDouble();
            if(i+1 <= list.count()-1)
                ts << ", ";
        }
    }
    else if( !qstrcmp(value.typeName(), "Point3D") )
    {
        Point3D pt = value.value<Point3D>();
        ts << pt.x << ", " << pt.y << ", " << pt.z;
    }
    else if( !qstrcmp(value.typeName(), "Vector3D") )
    {
        Vector3D vec = value.value<Vector3D>();
        ts << vec.x << ", " << vec.y << ", " << vec.z;
    }
    else if( !qstrcmp(value.typeName(), "BoundingBox3D") )
    {
        BoundingBox3D box = value.value<BoundingBox3D>();
        ts << box.xMin << ", " << box.xMax
           << box.yMin << ", " << box.yMax
           << box.zMin << ", " << box.zMax;
    }
    else if( !qstrcmp(value.typeName(), "ValueRange") )
    {
        ValueRange vr = value.value<ValueRange>();
        ts << vr.min << ", " << vr.max;
    }
    else if( !qstrcmp(value.typeName(), "QList<double>") )
    {
        QList<double> vals = value.value< QList<double> >();
        for(int i=0; i<vals.count(); i++)
        {
            double item = vals.at(i);
            ts << item;
            if(i+1 != vals.count()-1)
                ts << ", ";
        }
    }
    else
        return false;

    return true;
}

///////////////////////////////////////////////////////////////////////////////
// ColorParamGenerator
///////////////////////////////////////////////////////////////////////////////

QObject* ColorParamGenerator::containerObject()
{
    return this;
}

bool ColorParamGenerator::canGenerateParams(
                 const QStringList& params,
                 const QVariant& value)
{
    return ( (params == QStringList() << "QColor") || (params == QStringList() << "color_double" << "color_double" << "color_double")) &&
             value.canConvert(QVariant::Color);
}

bool ColorParamGenerator::generateParams(
                 const QStringList& params,
                 const QVariant& value,
                 QTextStream& ts)
{
    QColor color = value.value<QColor>();
    if( params == QStringList() << "QColor")
        ts << "QColor(" << color.red() << ", "
           << color.green() << ", " << color.blue()
           << ");";
    else
        ts << color.redF() << ", "
           << color.greenF() << ", " << color.blueF();

    return true;
}

///////////////////////////////////////////////////////////////////////////////
// VtkAlgorithmGetOutputCodeGenerator implementation
///////////////////////////////////////////////////////////////////////////////

#include "IVtkObjectWrapper.h"
#include "vtkObject.h"
#include "vtkAlgorithm.h"
#include "vtkAbstractMapper.h"
#include "IVtkDataObjectIOData.h"
#include "vtkActor.h"
#include "vtkMapper.h"

#include <GCF/DataStore>

Q_DECLARE_METATYPE(vtkObject*)

vtkObject* GetVtkObjectFromNode(IVisSystemNode* node)
{
    // We need to somehow figure out the vtkObject stored within
    // the node.
    vtkObject* vtkObjectPtr = 0;

    // Check if the node implements the IVtkObjectWrapper interface
    IVtkObjectWrapper* vtkObjectWrapper
        = qobject_cast<IVtkObjectWrapper*>(node->containerObject());
    if(vtkObjectWrapper)
        vtkObjectPtr = vtkObjectWrapper->getVtkObject();

    if(vtkObjectPtr)
        return vtkObjectPtr;

    if( node->nodeData().hasKey("vtkObject") )
        vtkObjectPtr = node->nodeData()["vtkObject"].value<vtkObject*>();

    return vtkObjectPtr;
}

QObject* VtkAlgorithmGetOutputCodeGenerator::containerObject()
{
    return this;
}

bool VtkAlgorithmGetOutputCodeGenerator::canGenerateGetOutputCode(
                      IVisSystemNode* node,
                      IVisSystemNodeConnectionPath* path,
                      IVisNetworkConnection* con)
{
    vtkObject* vtkObjectPtr = GetVtkObjectFromNode(node);
    if(!vtkObjectPtr)
        return false;

    // Now check if vtkObjectPtr inherits from vtkAlgorithm.
    vtkAlgorithm* alg = vtkAlgorithm::SafeDownCast(vtkObjectPtr);
    if(!alg)
        return false;

    // Fetch the output from the node at the given path
    IVisSystemNodeIOData* data = con->connectionData();
    IVtkDataObjectIOData* data2 = 0;
    bool success = data->queryInterface("IVtkDataObjectIOData", (void**)&data2);

    Q_UNUSED(path);

    return success;
}

bool VtkAlgorithmGetOutputCodeGenerator::generateGetOutputCode(
                      IVisSystemNode* node,
                      IVisSystemNodeConnectionPath* path,
                      IVisNetworkConnection* con,
                      QTextStream& ts, int tab,
                      QString& outputVariable)
{
    ts << node->nodeName() << "->GetOutput()";

    Q_UNUSED(outputVariable);
    Q_UNUSED(path);
    Q_UNUSED(con);
    Q_UNUSED(tab);

    return true;
}

///////////////////////////////////////////////////////////////////////////////
// VtkAlgorithmSetInputCodeGenerator implementation
///////////////////////////////////////////////////////////////////////////////

QObject* VtkAlgorithmSetInputCodeGenerator::containerObject()
{
    return this;
}

bool VtkAlgorithmSetInputCodeGenerator::canGenerateSetInputCode(IVisSystemNode* node,
                          IVisSystemNodeConnectionPath* path,
                          IVisNetworkConnection* con)
{
    vtkObject* vtkObjectPtr = GetVtkObjectFromNode(node);
    if(!vtkObjectPtr)
        return false;

    // Now check if vtkObjectPtr inherits from vtkAlgorithm.
    vtkAlgorithm* alg = vtkAlgorithm::SafeDownCast(vtkObjectPtr);
    if(!alg)
        return false;

    // Fetch the output from the node at the given path
    IVisSystemNodeIOData* data = con->connectionData();
    IVtkDataObjectIOData* data2 = 0;
    bool success = data->queryInterface("IVtkDataObjectIOData", (void**)&data2);

    Q_UNUSED(path);

    return success;
}

bool VtkAlgorithmSetInputCodeGenerator::generateSetInputCode(IVisSystemNode* node,
                          IVisSystemNodeConnectionPath* path,
                          IVisNetworkConnection* con,
                          const QString& inputCode,
                          QTextStream& ts, int tab)
{
    ts << QByteArray(tab, ' ') << node->nodeName() << "->SetInput(" << inputCode << ");\n";

    Q_UNUSED(path);
    Q_UNUSED(con);
    Q_UNUSED(path);

    return true;
}

///////////////////////////////////////////////////////////////////////////////
// VtkObjectGetOutputCodeGenerator implementation
///////////////////////////////////////////////////////////////////////////////

#include "IVtkObjectIOData.h"

QObject* VtkObjectGetOutputCodeGenerator::containerObject()
{
    return this;
}

bool VtkObjectGetOutputCodeGenerator::canGenerateGetOutputCode(IVisSystemNode* node,
                          IVisSystemNodeConnectionPath* path,
                          IVisNetworkConnection* con)
{
    vtkObject* vtkObjectPtr = GetVtkObjectFromNode(node);
    if(!vtkObjectPtr)
        return false;

    // Now check if vtkObjectPtr inherits from vtkObject.
    vtkObject* object = vtkObject::SafeDownCast(vtkObjectPtr);
    if(!object)
        return false;

    // Fetch the output from the node at the given path
    IVisSystemNodeIOData* data = con->connectionData();
    IVtkObjectIOData* data2 = 0;
    bool success = data->queryInterface("IVtkObjectIOData", (void**)&data2);

    Q_UNUSED(path);

    return success;
}

bool VtkObjectGetOutputCodeGenerator::generateGetOutputCode(IVisSystemNode* node,
                          IVisSystemNodeConnectionPath* path,
                          IVisNetworkConnection* con,
                          QTextStream& ts, int tab,
                          QString& outputVariable)
{
    ts << node->nodeName();

    Q_UNUSED(outputVariable);
    Q_UNUSED(path);
    Q_UNUSED(con);
    Q_UNUSED(tab);

    return true;
}

///////////////////////////////////////////////////////////////////////////////
// VtkActorSetInputCodeGenerator implementation
///////////////////////////////////////////////////////////////////////////////

#include "IVtkAbstractMapperIOData.h"
#include "IVtkPropertyIOData.h"

QObject* VtkActorSetInputCodeGenerator::containerObject()
{
    return this;
}

bool VtkActorSetInputCodeGenerator::canGenerateSetInputCode(IVisSystemNode* node,
                          IVisSystemNodeConnectionPath* path,
                          IVisNetworkConnection* con)
{
    vtkObject* vtkObjectPtr = GetVtkObjectFromNode(node);
    if(!vtkObjectPtr)
        return false;

    // Now check if vtkObjectPtr inherits from vtkActor.
    vtkActor* actor = vtkActor::SafeDownCast(vtkObjectPtr);
    if(!actor)
        return false;

    // Fetch the output from the node at the given path
    IVisSystemNodeIOData* data = con->connectionData();
    IVtkAbstractMapperIOData* mapperData = 0;
    IVtkPropertyIOData* propertyData = 0;
    bool success = data->queryInterface("IVtkAbstractMapperIOData", (void**)&mapperData);
    if(!success)
        success = data->queryInterface("IVtkPropertyIOData", (void**)&propertyData);

    Q_UNUSED(path);

    return success;
}

bool VtkActorSetInputCodeGenerator::generateSetInputCode(IVisSystemNode* node,
                          IVisSystemNodeConnectionPath* path,
                          IVisNetworkConnection* con,
                          const QString& inputCode,
                          QTextStream& ts, int tab)
{
    // Fetch the output from the node at the given path
    IVisSystemNodeIOData* data = con->connectionData();
    IVtkAbstractMapperIOData* mapperData = 0;
    IVtkPropertyIOData* propertyData = 0;
    bool success = data->queryInterface("IVtkAbstractMapperIOData", (void**)&mapperData);
    if(!success)
        success = data->queryInterface("IVtkPropertyIOData", (void**)&propertyData);

    if(propertyData)
        ts << QByteArray(tab, ' ') << node->nodeName() << "->SetProperty(" << inputCode << ");\n";
    else
        ts << QByteArray(tab, ' ') << node->nodeName() << "->SetMapper(" << inputCode << ");\n";

    Q_UNUSED(path);
    Q_UNUSED(path);

    return true;
}

///////////////////////////////////////////////////////////////////////////////
// VtkRendererSetInputCodeGenerator implementation
///////////////////////////////////////////////////////////////////////////////

#include "IVtkRendererIOData.h"
#include "IVtkCameraIOData.h"
#include "vtkRenderer.h"

QObject* VtkRendererSetInputCodeGenerator::containerObject()
{
    return this;
}

bool VtkRendererSetInputCodeGenerator::canGenerateSetInputCode(IVisSystemNode* node,
                          IVisSystemNodeConnectionPath* path,
                          IVisNetworkConnection* con)
{
    vtkObject* vtkObjectPtr = GetVtkObjectFromNode(node);
    if(!vtkObjectPtr)
        return false;

    // Now check if vtkObjectPtr inherits from vtkRenderer.
    vtkRenderer* renderer = vtkRenderer::SafeDownCast(vtkObjectPtr);
    if(!renderer)
        return false;

    // Fetch the output from the node at the given path
    IVisSystemNodeIOData* data = con->connectionData();
    IVtkRendererIOData* actorData = 0;
    IVtkCameraIOData* cameraData = 0;
    bool success = data->queryInterface("IVtkActorIOData", (void**)&actorData);
    if (!success)
        success = data->queryInterface("IVtkCameraIOData",(void**)&cameraData);

    Q_UNUSED(path);

    return success;
}

bool VtkRendererSetInputCodeGenerator::generateSetInputCode(IVisSystemNode* node,
                          IVisSystemNodeConnectionPath* path,
                          IVisNetworkConnection* con,
                          const QString& inputCode,
                          QTextStream& ts, int tab)
{
    // Fetch the output from the node at the given path
    IVisSystemNodeIOData* data = con->connectionData();
    IVtkRendererIOData* actorData = 0;
    IVtkCameraIOData* cameraData = 0;
    bool success = data->queryInterface("IVtkActorIOData", (void**)&actorData);
    if (!success)
        success = data->queryInterface("IVtkCameraIOData",(void**)&cameraData);
    if(actorData)
        ts << QByteArray(tab, ' ') << node->nodeName() << "->AddActor(" << inputCode << ");\n";
    else if (cameraData)
        ts << QByteArray(tab, ' ') << node->nodeName() << "->SetActiveCamera(" << inputCode << ");\n";

    Q_UNUSED(path);

    return true;
}

///////////////////////////////////////////////////////////////////////////////
// VtkRenderWindowSetInputCodeGenerator implementation
///////////////////////////////////////////////////////////////////////////////

#include "IVtkRenderWindowIOData.h"
#include "vtkRenderWindow.h"

QObject* VtkRenderWindowSetInputCodeGenerator::containerObject()
{
    return this;
}

bool VtkRenderWindowSetInputCodeGenerator::canGenerateSetInputCode(IVisSystemNode* node,
                          IVisSystemNodeConnectionPath* path,
                          IVisNetworkConnection* con)
{
    vtkObject* vtkObjectPtr = GetVtkObjectFromNode(node);
    if(!vtkObjectPtr)
        return false;

    // Now check if vtkObjectPtr inherits from vtkRenderWindow.
    vtkRenderWindow* renderWindow = vtkRenderWindow::SafeDownCast(vtkObjectPtr);
    if(!renderWindow)
        return false;

    // Fetch the output from the node at the given path
    IVisSystemNodeIOData* data = con->connectionData();
    IVtkRenderWindowIOData* data2 = 0;
    bool success = data->queryInterface("IVtkRendererIOData", (void**)&data2);

    Q_UNUSED(path);

    return success;
}

bool VtkRenderWindowSetInputCodeGenerator::generateSetInputCode(IVisSystemNode* node,
                          IVisSystemNodeConnectionPath* path,
                          IVisNetworkConnection* con,
                          const QString& inputCode,
                          QTextStream& ts, int tab)
{
    // Fetch the output from the node at the given path
    IVisSystemNodeIOData* data = con->connectionData();
    IVtkRenderWindowIOData* data2 = 0;
    data->queryInterface("IVtkRendererIOData", (void**)&data2);

    ts << QByteArray(tab, ' ') << node->nodeName() << "->AddRenderer(" << inputCode << ");\n";

    Q_UNUSED(path);

    return true;
}

///////////////////////////////////////////////////////////////////////////////
// VtkImplicitBooleanSetInputCodeGenerator implementation
///////////////////////////////////////////////////////////////////////////////

#include "IVtkImplicitFunctionIOData.h"
#include "vtkImplicitBoolean.h"

QObject* VtkImplicitBooleanSetInputCodeGenerator::containerObject()
{
    return this;
}

bool VtkImplicitBooleanSetInputCodeGenerator::canGenerateSetInputCode(IVisSystemNode* node,
                          IVisSystemNodeConnectionPath* path,
                          IVisNetworkConnection* con)
{
    vtkObject* vtkObjectPtr = GetVtkObjectFromNode(node);
    if(!vtkObjectPtr)
        return false;

    // Now check if vtkObjectPtr inherits from vtkBooleanImplicit.
    vtkImplicitBoolean* implicitBoolean = vtkImplicitBoolean::SafeDownCast(vtkObjectPtr);
    if(!implicitBoolean)
        return false;

    // Fetch the output from the node at the given path
    IVisSystemNodeIOData* data = con->connectionData();
    IVtkImplicitFunctionIOData* data2 = 0;
    bool success = data->queryInterface("IVtkImplicitFunctionIOData", (void**)&data2);

    Q_UNUSED(path);

    return success;
}

bool VtkImplicitBooleanSetInputCodeGenerator::generateSetInputCode(IVisSystemNode* node,
                          IVisSystemNodeConnectionPath* path,
                          IVisNetworkConnection* con,
                          const QString& inputCode,
                          QTextStream& ts, int tab)
{
    // Fetch the output from the node at the given path
    IVisSystemNodeIOData* data = con->connectionData();
    IVtkImplicitFunctionIOData* data2 = 0;
    data->queryInterface("IVtkImplicitFunctionIOData", (void**)&data2);

    ts << QByteArray(tab, ' ') << node->nodeName() << "->AddFunction(" << inputCode << ");\n";

    Q_UNUSED(path);

    return true;
}

///////////////////////////////////////////////////////////////////////////////
// VtkSampleFunctionSetInputCodeGenerator implementation
///////////////////////////////////////////////////////////////////////////////

#include "IVtkImplicitFunctionIOData.h"
#include "vtkSampleFunction.h"

QObject* VtkSampleFunctionSetInputCodeGenerator::containerObject()
{
    return this;
}

bool VtkSampleFunctionSetInputCodeGenerator::canGenerateSetInputCode(IVisSystemNode* node,
                          IVisSystemNodeConnectionPath* path,
                          IVisNetworkConnection* con)
{
    vtkObject* vtkObjectPtr = GetVtkObjectFromNode(node);
    if(!vtkObjectPtr)
        return false;

    // Now check if vtkObjectPtr inherits from vtkBooleanImplicit.
    vtkSampleFunction* sampleFunction = vtkSampleFunction::SafeDownCast(vtkObjectPtr);
    if(!sampleFunction)
        return false;

    // Fetch the output from the node at the given path
    IVisSystemNodeIOData* data = con->connectionData();
    IVtkImplicitFunctionIOData* data2 = 0;
    bool success = data->queryInterface("IVtkImplicitFunctionIOData", (void**)&data2);

    Q_UNUSED(path);

    return success;
}

bool VtkSampleFunctionSetInputCodeGenerator::generateSetInputCode(IVisSystemNode* node,
                          IVisSystemNodeConnectionPath* path,
                          IVisNetworkConnection* con,
                          const QString& inputCode,
                          QTextStream& ts, int tab)
{
    // Fetch the output from the node at the given path
    IVisSystemNodeIOData* data = con->connectionData();
    IVtkImplicitFunctionIOData* data2 = 0;
    data->queryInterface("IVtkImplicitFunctionIOData", (void**)&data2);

    ts << QByteArray(tab, ' ') << node->nodeName() << "->SetImplicitFunction(" << inputCode << ");\n";

    Q_UNUSED(path);

    return true;
}
