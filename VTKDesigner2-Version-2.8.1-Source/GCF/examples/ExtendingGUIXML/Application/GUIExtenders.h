#ifndef GUIEXTENDERS_H
#define GUIEXTENDERS_H

#include <GCF/ComponentGui>

class MenuWidgetTabItemExtender;

class MenuWidgetTabExtender : public GCF::AbstractGuiXmlExtender, public GCF::AbstractNodeInitializer
{
public:
    MenuWidgetTabExtender() { }
    ~MenuWidgetTabExtender() { }

    enum { Type = GCF::ComponentGuiNode::Other + 1 };

    // GCF::AbstractGuiXmlExtender implementation
    int type() const { return MenuWidgetTabExtender::Type; }
    QString tagName() const { return "menutab"; }
    void parseElement(QDomElement e, GCF::ComponentGui gui);
    GCF::AbstractNodeInitializer* guiNodeInitializer() const {
        return const_cast<MenuWidgetTabExtender*>(this);
    }

    // GCF::AbstractNodeInitializer implementation
    int handledType() const { return MenuWidgetTabExtender::Type; }
    void initializeNode(GCF::ComponentGuiNode node);
    void cleanupNode(GCF::ComponentGuiNode node);
    void showNode(GCF::ComponentGuiNode node);
    void hideNode(GCF::ComponentGuiNode node);
};

class MenuWidgetTabItemExtender : public GCF::AbstractGuiXmlExtender, public GCF::AbstractNodeInitializer
{
public:
    MenuWidgetTabItemExtender() { }
    ~MenuWidgetTabItemExtender() { }

    enum { Type = GCF::ComponentGuiNode::Other + 2 };

    // GCF::AbstractGuiXmlExtender implementation
    int type() const { return MenuWidgetTabItemExtender::Type; }
    QString tagName() const { return "menutabitem"; }
    void parseElement(QDomElement e, GCF::ComponentGui gui);
    GCF::AbstractNodeInitializer* guiNodeInitializer() const {
        return const_cast<MenuWidgetTabItemExtender*>(this);
    }

    // GCF::AbstractNodeInitializer implementation
    int handledType() const { return MenuWidgetTabItemExtender::Type; }
    void initializeNode(GCF::ComponentGuiNode node);
    void cleanupNode(GCF::ComponentGuiNode node);
    void showNode(GCF::ComponentGuiNode node);
    void hideNode(GCF::ComponentGuiNode node);
};

#include <QMetaType>
Q_DECLARE_METATYPE(QAction*)

#endif // GUIEXTENDERS_H
