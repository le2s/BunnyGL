#include "PatientList.h"
#include "DICOMImageSeries.h"

#include <GCF/Application>
#include <QtDebug>

struct PatientListData
{
    QVariantList patientInfoList;
};

PatientList::PatientList(QObject* parent)
: QObject(parent)
{
    d = new PatientListData;

    QString baseDir = gApp->parameterValue("DICOMService_BaseDirectory");
    if(!baseDir.isEmpty())
    {
        QStringList seriesList = gApp->parameterValue("DICOMService_Series").split(",");
        Q_FOREACH(QString series, seriesList)
        {
            series = series.remove("\n");
            series = series.trimmed();
            if(series.isEmpty())
                continue;

            QString seriesDir = baseDir + "/" + series;
            this->addPatient(seriesDir);
        }
    }
}

PatientList::~PatientList()
{
    delete d;
}

void PatientList::addPatient(const QString& seriesDir)
{
#ifdef VTK5_FOUND
    QString patientName;
    QImage sampleImage;
    int sliceCount;

    bool success = DICOMImageSeries::LoadInformation(seriesDir, patientName,
                                                     sampleImage, sliceCount);
    if(!success)
        return;

    QVariantMap patientInfo;
    patientInfo["PatientName"] = patientName;
    patientInfo["SampleImage"] = sampleImage;
    patientInfo["SliceCount"] = sliceCount;
    patientInfo["SeriesPath"] = seriesDir;

    d->patientInfoList.append(patientInfo);
#else
    qDebug() << "Cannot add " << seriesDir << " because VTK5 was not found during compilation!";
#endif
}

void PatientList::removePatient(const QString& seriesDir)
{
    for(int i=0; i<d->patientInfoList.count(); i++)
    {
        QVariantMap patientInfo = d->patientInfoList.at(i).toMap();
        if(patientInfo["SeriesPath"].toString() == seriesDir)
        {
            d->patientInfoList.removeAt(i);
            return;
        }
    }
}

int PatientList::patientCount() const
{
    return d->patientInfoList.count();
}

QVariantList PatientList::patientList() const
{
    return d->patientInfoList;
}

QString PatientList::nameAt(int index) const
{
    if(index < 0 || index >= d->patientInfoList.count())
        return QString();

    QVariantMap info = d->patientInfoList.at(index).toMap();
    return info["PatientName"].toString();
}

QImage PatientList::sampleImageAt(int index) const
{
    if(index < 0 || index >= d->patientInfoList.count())
        return QImage();

    QVariantMap info = d->patientInfoList.at(index).toMap();
    return info["SampleImage"].value<QImage>();
}

int PatientList::sliceCountAt(int index) const
{
    if(index < 0 || index >= d->patientInfoList.count())
        return 0;

    QVariantMap info = d->patientInfoList.at(index).toMap();
    return info["SliceCount"].toInt();
}

QString PatientList::pathAt(int index) const
{
    if(index < 0 || index >= d->patientInfoList.count())
        return QString();

    QVariantMap info = d->patientInfoList.at(index).toMap();
    return info["SeriesPath"].toString();
}

