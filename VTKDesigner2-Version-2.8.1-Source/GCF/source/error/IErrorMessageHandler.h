#ifndef IERRORMESSAGEHANDLER_H
#define IERRORMESSAGEHANDLER_H

#include <GCF/IContainer>

class QTextStream;
namespace GCF { class ErrorMessage; }

class IErrorMessageHandler : virtual public IContainer
{
public:
    virtual void handleErrorMessage(GCF::ErrorMessage* msg) = 0;
    virtual void print(GCF::ErrorMessage* msg, QTextStream& ts) = 0;
    virtual void submit(const QString& msg) = 0;

    // Use this method to force immediate handling of error messages reported so far
    virtual void flush() {
    }
};

Q_DECLARE_INTERFACE(IErrorMessageHandler, "com.vcreatelogic.IErrorMessageHandler/1.0")

#endif // IERRORMESSAGEHANDLER_H
