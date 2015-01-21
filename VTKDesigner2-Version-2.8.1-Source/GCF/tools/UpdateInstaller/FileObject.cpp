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

#include "FileObject.h"
#include "UpdateInstaller.h"
#include "FileSystemObject.h"

FileObject::FileObject(const QString& name)
     :QObject( & FileSystemObject::instance() ),
      m_file( UpdateInstaller::instance().resolveFileName(name) )
{

}

FileObject::~FileObject()
{

}

QString FileObject::GetFileName() const
{
    return m_file.fileName();
}

bool FileObject::OpenForRead()
{
    if( m_file.isOpen() )
        return false;

    return m_file.open(QFile::ReadOnly);
}

bool FileObject::OpenForWrite()
{
    if( m_file.isOpen() )
        return false;

    return m_file.open(QFile::WriteOnly);
}

bool FileObject::OpenForReadWrite()
{
    if( m_file.isOpen() )
        return false;

    return m_file.open(QFile::ReadWrite);
}

bool FileObject::Close()
{
    if( !m_file.isOpen() )
        return false;

    m_file.close();
    return true;
}

bool FileObject::IsOpen()
{
    return m_file.isOpen();
}

QString FileObject::ReadAll()
{
    return m_file.readAll();
}

QString FileObject::ReadLine()
{
    return m_file.readLine();
}

QString FileObject::ReadField(const QString& delim)
{
    // Not yet implemented
    Q_UNUSED(delim);
    return QString();
}

bool FileObject::Write(const QString& str)
{
    m_file.write( str.toAscii() );
    return true;
}

bool FileObject::IsAtEnd() const
{
    return m_file.atEnd();
}

bool FileObject::IsAtBeginning() const
{
    if( m_file.pos() == 0 )
        return true;

    return false;
}

void FileObject::setPosition(int pos)
{
    if(pos >= 0)
    {
        qint64 pos2 = (qint64)pos;
        m_file.seek(pos2);
    }
    else
        m_file.seek( m_file.size() );
}

int FileObject::position() const
{
    int pos2 = (int)m_file.pos();
    return pos2;
}

const QFile* FileObject::qfile() const
{
    return &m_file;
}


