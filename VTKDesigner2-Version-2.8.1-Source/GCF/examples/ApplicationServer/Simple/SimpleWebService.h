#ifndef SIMPLEWEBSERVICE_H
#define SIMPLEWEBSERVICE_H

#include <GCF/AbstractComponent>
#include <GCF/WSMacros>

class SimpleWebService : public GCF::AbstractComponent
{
    Q_OBJECT
    GCF_DECLARE_COMPONENT(SimpleWebService)

    public:
        static SimpleWebService& instance();
    ~SimpleWebService();

    // Our web-service consists of only one service method
    // and that is toUpper(). It accepts a string as parameter
    // and returns the same string in upper case.
    Q_INVOKABLE GCF_ATOMIC_WS QString toUpper(const QString& string) const;

protected:
    SimpleWebService();
    void initializeComponent();
    void finalizeComponent();
    QObject* fetchObject(const QString &completeName) const;
};

#endif // SIMPLEWEBSERVICE_H
