#include <GCF/VariantComparisonHelper.h>
#include <QMap>
#include <QtDebug>

bool defaultComparisionFunction(const QVariant& v1, const QVariant& v2)
{
    return v1 == v2;
}

static QMap<QByteArray,ComparisionFunction>& functionMap()
{
    static QMap<QByteArray,ComparisionFunction> theMap;
    return theMap;
}

bool GCF::registerVariantComparisionFunction(const QByteArray& typeName, ComparisionFunction fn)
{
    if(::functionMap().contains(typeName))
        return false;

    ::functionMap()[typeName] = fn;
    return true;
}

bool GCF::unregisterVariantComparisionFunction(const QByteArray& typeName, ComparisionFunction fn)
{
    if(!::functionMap().contains(typeName))
        return false;

    if(::functionMap()[typeName] != fn)
        return false;

    ::functionMap().remove(typeName);
    return true;
}

bool GCF::compareVariants(const QVariant& v1, const QVariant& v2)
{
    if(v1.type() >= QVariant::UserType && v2.type() >= QVariant::UserType)
    {
        if( qstrcmp(v1.typeName(), v2.typeName()) )
            return v1 == v2;

        QByteArray type(v1.typeName());
        ComparisionFunction fn = ::functionMap().value(type, defaultComparisionFunction);
        return fn(v1, v2);
    }

    return v1 == v2;
}
