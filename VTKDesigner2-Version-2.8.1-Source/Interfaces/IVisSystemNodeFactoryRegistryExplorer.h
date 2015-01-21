/****************************************************************************
**
** Copyright (C) VCreate Logic Private Limited, Bangalore
**
** Use of this file is limited according to the terms specified by
** VCreate Logic Private Limited, Bangalore.  Details of those terms
** are listed in licence.txt included as part of the distribution package
** of this file. This file may not be distributed without including the
** licence.txt file.
**
** Contact info@vcreatelogic.com if any conditions of this licensing are
** not clear to you.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#ifndef I_VIS_SYSTEM_NODE_FACTORY_REGISTRY_EXPLORER_H
#define I_VIS_SYSTEM_NODE_FACTORY_REGISTRY_EXPLORER_H

#include <GCF/IContainer>

class IVisSystem;
class IVisSystemNodeFactoryRegistry;

class IVisSystemNodeFactoryRegistryExplorer : public virtual IContainer
{
public:
    enum ExploreMode
    {
        CategoryMode,
        ClassHierarchyMode,
        ContextMode
    };

    // Basic stuff.
    virtual void            updateClassList() = 0;
    virtual void            selectClassName(QString className) = 0;
    virtual QString         selectedClassName() = 0;
    virtual void            setClassNameFilter(QString filter) = 0;

    virtual void            setExploreMode(ExploreMode mode) = 0;
    virtual ExploreMode     exploreMode() = 0;

    virtual void            setNodeFactoryRegistry(IVisSystemNodeFactoryRegistry* reg) = 0;
    virtual IVisSystemNodeFactoryRegistry*     nodeFactoryRegistry() = 0;
};

Q_DECLARE_INTERFACE(IVisSystemNodeFactoryRegistryExplorer, "com.vcreatelogic.IVisSystemNodeFactoryRegistryExplorer/1.0")

#endif
