#ifndef VARIANTCOMPARISONHELPER_H
#define VARIANTCOMPARISONHELPER_H

#include <GCF/Common>
#include <QByteArray>
#include <QVariant>

typedef bool (*ComparisionFunction)(const QVariant& v1, const QVariant&v2);

namespace GCF
{

bool GCF_EXPORT_METHOD registerVariantComparisionFunction(const QByteArray& typeName, ComparisionFunction fn);
bool GCF_EXPORT_METHOD unregisterVariantComparisionFunction(const QByteArray& typeName, ComparisionFunction fn);
bool GCF_EXPORT_METHOD compareVariants(const QVariant& v1, const QVariant& v2);

}

#endif // VARIANTCOMPARISONHELPER_H
