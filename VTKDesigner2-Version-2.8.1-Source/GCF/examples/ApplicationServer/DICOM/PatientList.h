#ifndef PATIENT_LIST_H
#define PATIENT_LIST_H

#include <QObject>
#include <QImage>
#include <QVariantList>

#include <GCF/WSMacros>

struct PatientListData;
class PatientList : public QObject
{
    Q_OBJECT

public:
    PatientList(QObject* parent=0);
    ~PatientList();

    void addPatient(const QString& seriesDir);
    void removePatient(const QString& seriesDir);
    int patientCount() const;

    Q_INVOKABLE GCF_ATOMIC_WS QVariantList patientList() const;

    Q_INVOKABLE GCF_CLIENT_INDEPENDENT_WS QString nameAt(int index) const;
    Q_INVOKABLE GCF_CLIENT_INDEPENDENT_WS QImage sampleImageAt(int index) const;
    Q_INVOKABLE GCF_CLIENT_INDEPENDENT_WS int sliceCountAt(int index) const;
    Q_INVOKABLE GCF_CLIENT_INDEPENDENT_WS QString pathAt(int index) const;

private:
    PatientListData* d;
};

#endif
