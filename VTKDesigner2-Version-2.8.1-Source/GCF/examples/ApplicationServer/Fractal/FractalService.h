#ifndef FRACTALSERVICE_H
#define FRACTALSERVICE_H

#include <GCF/AbstractComponent>

struct FractalServiceData;
class FractalService : public GCF::AbstractComponent
{
    Q_OBJECT
    GCF_DECLARE_COMPONENT(FractalService)

    public:
        static FractalService& instance();
    ~FractalService();

protected:
    FractalService();
    void initializeComponent();
    void finalizeComponent();
    QObject* fetchObject(const QString& completeName) const;
};

#endif // FRACTALSERVICE_H
