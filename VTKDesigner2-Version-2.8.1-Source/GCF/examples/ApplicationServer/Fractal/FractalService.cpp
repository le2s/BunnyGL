#include "FractalService.h"
#include "MengerSquareFractal.h"
#include "SierpinskiTriangleFractal.h"

GCF_DEFINE_COMPONENT(FractalService)

FractalService& FractalService::instance()
{
    return *GCF_CREATE_COMPONENT(FractalService);
}

FractalService::FractalService() { }
FractalService::~FractalService() { }
void FractalService::finalizeComponent() { }

void FractalService::initializeComponent()
{
    this->setGuiXmlFileName(":/FractalService/FractalService.xml");
}

QObject* FractalService::fetchObject(const QString& completeName) const
{
    static MengerSquareFractal mengerSquareFractal;
    static SierpinskiTriangleFractal sierpinskiTriangleFractal;

    if(completeName == "FractalService.MengerSquareFractal")
        return &mengerSquareFractal;

    if(completeName == "FractalService.SierpinskiTriangleFractal")
        return &sierpinskiTriangleFractal;

    return 0;
}

GCF_EXPORT_COMPONENT_PLUGIN(FractalService)
