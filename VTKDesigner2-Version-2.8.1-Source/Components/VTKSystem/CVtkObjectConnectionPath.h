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

#ifndef C_VTK_OBJECT_CONNECTION_PATH_H
#define C_VTK_OBJECT_CONNECTION_PATH_H

#include "IVisSystemNodeConnectionPath.h"

class CVtkObjectConnectionPath : virtual public IVisSystemNodeConnectionPath
{
public:
    CVtkObjectConnectionPath();
    CVtkObjectConnectionPath(QString pName, IVisSystemNodeConnectionPath::PathType pType, QString pdType, int pIndex, bool ami=false);
    ~CVtkObjectConnectionPath();

    void setPathName(QString pName);
    void setPathType(IVisSystemNodeConnectionPath::PathType pType);
    void setPathDataType(QString pdType);
    void setPathIndex(int pIndex);
    void setAllowMultipleInputs(bool val);

    // IContainer implementation
    QObject* containerObject();

    // IVisSystemNodeConnectionPath implementation
    QString pathName();
    IVisSystemNodeConnectionPath::PathType pathType();
    QString pathDataType();
    int pathIndex();
    bool allowMultipleInputs();

private:
    QString mPathName;
    IVisSystemNodeConnectionPath::PathType mPathType;
    QString mPathDataType;
    int mPathIndex;
    bool mMultipleInputs;
};

inline CVtkObjectConnectionPath::CVtkObjectConnectionPath()
{
    mPathIndex = -1;
    mPathType = ControlPath;
    mMultipleInputs = false;
}

inline CVtkObjectConnectionPath::CVtkObjectConnectionPath(QString pName, IVisSystemNodeConnectionPath::PathType pType, QString pdType, int pIndex, bool ami)
{
    mPathName = pName;
    mPathType = pType;
    mPathDataType = pdType;
    mPathIndex = pIndex;
    mMultipleInputs = ami;
}

inline CVtkObjectConnectionPath::~CVtkObjectConnectionPath()
{

}

inline void CVtkObjectConnectionPath::setPathName(QString pName)
{
    mPathName = pName;
}

inline void CVtkObjectConnectionPath::setPathType(IVisSystemNodeConnectionPath::PathType pType)
{
    mPathType = pType;
}

inline void CVtkObjectConnectionPath::setPathDataType(QString pdType)
{
    mPathDataType = pdType;
}

inline void CVtkObjectConnectionPath::setPathIndex(int pIndex)
{
    mPathIndex = pIndex;
}

inline QObject* CVtkObjectConnectionPath::containerObject()
{
    return 0;
}

inline QString CVtkObjectConnectionPath::pathName()
{
    return mPathName;
}

inline IVisSystemNodeConnectionPath::PathType CVtkObjectConnectionPath::pathType()
{
    return mPathType;
}

inline QString CVtkObjectConnectionPath::pathDataType()
{
    return mPathDataType;
}

inline int CVtkObjectConnectionPath::pathIndex()
{
    return mPathIndex;
}

inline bool CVtkObjectConnectionPath::allowMultipleInputs()
{
    return mMultipleInputs;
}

#endif

