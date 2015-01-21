/****************************************************************************
**
** Copyright (C) UNO and VCreate Logic
**
** Use of this file is limited according to the terms specified by
** UNO and VCreate Logic.
**
** Details of those terms are listed in licence.txt included as
** part of the distribution package of this file. This file may not
** be distributed without including the licence.txt file.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include "CVtkDelimitedTextReader.h"
#include "InfoVisComponent.h"
#include "vtkDelimitedTextReader.h"

#include <QFileInfo>
#include <QtDebug>

DEFINE_VIS_NODE(CVtkDelimitedTextReader, CVtkTableAlgorithm)
{
    pDesc->setNodeClassCategory("InfoVis");
    pDesc->setNodeClassName("vtkDelimitedTextReader");
    pDesc->setNodeClassDescription("vtkDelimitedTextReader");
    pDesc->setNodeIcon( InfoVisComponent::instance().nodeIcon() );

    pDesc->setPropertyInfo("Length", "SetLength", "GetLength", QStringList() << "double", QStringList(), "void", "double");
    
    // Uncomment and use the following code template to add input/output paths
    /*
    pDesc->addConnectionPath(
        new CGenericVisNodeConnectionPath(
                "PathName",                                 // Name of the path
                IVisSystemNodeConnectionPath::OutputPath,   // Path type can be OutputPath or InputPath
                "PathType",                                 // Data type of the path
                0,                                          // Path index (don't change)
                false                                       // Allow Multiple Inputs Flag
            )
        );
    */
}

struct CVtkDelimitedTextReaderData
{
    vtkDelimitedTextReader *delimitedTextReader;
    QString fileName;
};

CVtkDelimitedTextReader::CVtkDelimitedTextReader()
{
    CVtkDelimitedTextReader::InitializeNodeDesc();
    d = new CVtkDelimitedTextReaderData;

    d->delimitedTextReader = 0;
    setVtkObject( vtkDelimitedTextReader::New() );
}

CVtkDelimitedTextReader::~CVtkDelimitedTextReader()
{
    // This would be a good time to delete the backend object. If the backend object is a vtkObject
    // subclass, then you have to delete it now.
    // d->VtkObjectPointer->Delete();

    d->delimitedTextReader->Delete();
    delete d;
}

void CVtkDelimitedTextReader::setFileName(const QString& fileName)
{
    d->fileName = fileName;

    if( QFileInfo(d->fileName).exists() )
    {
        QByteArray ba = fileName.toLatin1();
        d->delimitedTextReader->SetFileName(ba.constData());
        d->delimitedTextReader->Update();

        qDebug() << "Updated content from file";
    }
    else
        d->delimitedTextReader->SetFileName(0);
}

QString CVtkDelimitedTextReader::fileName() const
{
    return d->fileName;
}

void CVtkDelimitedTextReader::setFieldDelimiterCharacters(const QString& chars)
{
    QByteArray ba = chars.toLatin1();
    d->delimitedTextReader->SetFieldDelimiterCharacters(ba.constData());
}

QString CVtkDelimitedTextReader::fieldDelimiterCharacters() const
{
    return QString( d->delimitedTextReader->GetFieldDelimiterCharacters() );
}

void CVtkDelimitedTextReader::setStringDelimiter(const QString& str)
{
    QChar ch = str.length() ? str.at(0) : QChar(0);
    d->delimitedTextReader->SetStringDelimiter(ch.toAscii());
}

QString CVtkDelimitedTextReader::stringDelimiter() const
{
    return QString( QChar(d->delimitedTextReader->GetStringDelimiter()) );
}

void CVtkDelimitedTextReader::setUseStringDelimiter(bool val)
{
    d->delimitedTextReader->SetUseStringDelimiter(val);
}

bool CVtkDelimitedTextReader::isUseStringDelimiter() const
{
    return d->delimitedTextReader->GetUseStringDelimiter();
}

void CVtkDelimitedTextReader::setHaveHeaders(bool val)
{
    d->delimitedTextReader->SetHaveHeaders(val);
}

bool CVtkDelimitedTextReader::isHaveHeaders() const
{
    return d->delimitedTextReader->GetHaveHeaders();
}

void CVtkDelimitedTextReader::setMergeConsecutiveDelimiters(bool val)
{
    d->delimitedTextReader->SetMergeConsecutiveDelimiters(val);
}

bool CVtkDelimitedTextReader::isMergeConsecutiveDelimiters() const
{
    return d->delimitedTextReader->GetMergeConsecutiveDelimiters();
}

void CVtkDelimitedTextReader::setMaxRecords(int val)
{
    d->delimitedTextReader->SetMaxRecords(val);
}

int CVtkDelimitedTextReader::maxRecords() const
{
    return d->delimitedTextReader->GetMaxRecords();
}

void CVtkDelimitedTextReader::setDetectNumericColumns(bool val)
{
    d->delimitedTextReader->SetDetectNumericColumns(val);
}

bool CVtkDelimitedTextReader::isDetectNumericColumns() const
{
    return d->delimitedTextReader->GetDetectNumericColumns();
}

bool CVtkDelimitedTextReader::hasInput(IVisSystemNodeConnectionPath* path)
{
    if(!path)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CVtkTableAlgorithm::hasInput(path);
}

bool CVtkDelimitedTextReader::setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CVtkTableAlgorithm::setInput(path, inputData);
}

bool CVtkDelimitedTextReader::removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CVtkTableAlgorithm::removeInput(path, inputData);
}

bool CVtkDelimitedTextReader::fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData)
{
    if(!path || !outputData)
        return false;

    /*
    If you have added output paths in the description block at the header of this file,
    then you will have to handle outputs here
    */

    return CVtkTableAlgorithm::fetchOutput(path, outputData);
}

bool CVtkDelimitedTextReader::outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData)
{
    if(!path || !outputData)
        return false;

    /*
    If you have added output paths in the description block at the header of this file,
    then you will have to handle outputs here
    */

    return CVtkTableAlgorithm::outputDerefed(path, outputData);
}

#ifdef ENABLE_ADVANCED_PROPERTIES

int CVtkDelimitedTextReader::propertyCount()
{
    return 0;
}

QString CVtkDelimitedTextReader::propertyName(int index)
{
    Q_UNUSED(index);
    return QString();
}

QString CVtkDelimitedTextReader::propertyGroup(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant CVtkDelimitedTextReader::propertyValue(int index)
{
    Q_UNUSED(index);
    return QVariant();
}

void CVtkDelimitedTextReader::setPropertyValue(int index, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(value);
}

bool CVtkDelimitedTextReader::hasEditor(int index)
{
    Q_UNUSED(index);
    return false;
}

QWidget* CVtkDelimitedTextReader::createEditor(int index)
{
    Q_UNUSED(index);
    return 0;
}

void CVtkDelimitedTextReader::setEditorValue(int index, QWidget* widget, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(value);
}

QVariant CVtkDelimitedTextReader::editorValue(int index, QWidget* widget)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
}

void CVtkDelimitedTextReader::connectEditor(int index, QWidget* widget, QObject* receiver, const char* member)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(receiver);
    Q_UNUSED(member);
}

QString CVtkDelimitedTextReader::propertyValueString(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant CVtkDelimitedTextReader::propertyValueFromString(int index, QString valueStr)
{
    Q_UNUSED(index);
    Q_UNUSED(valueStr);

    return QVariant();
}

bool CVtkDelimitedTextReader::canLoadSaveProperty(int index)
{
    Q_UNUSED(index);

    return false;
}

#endif

void CVtkDelimitedTextReader::setVtkObject(vtkObject* ptr)
{
    d->delimitedTextReader = dynamic_cast<vtkDelimitedTextReader*>(ptr);
    nodeData()["vtkObject"] = qVariantFromValue<vtkObject*>(d->delimitedTextReader);
    CVtkTableAlgorithm::setVtkObject(ptr);
}





