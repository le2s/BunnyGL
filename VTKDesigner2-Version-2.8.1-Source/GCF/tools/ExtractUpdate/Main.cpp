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

#include <QtCore>

#include "UpdateFileUncompressor.h"

int main(int argc, char** argv)
{
    // There is no need for an event loop in this program, so we wont
    // create an instance of QCoreApplication

    /* Options are
       --output | -o       Path to the directory where the update file should be uncompressedss
       --input  | -i       Name of the compressed update file.
    */
    QString input, output;

    for(int i=1; i<argc; i++)
    {
        QString arg( argv[i] );
        QString nextArg = i+1 < argc ? QString(argv[i+1]) : QString();

        if( arg == "-i" || arg == "--input" )
        {
            input = nextArg;
            ++i;
            continue;
        }

        if( arg == "-o" || arg == "--output" )
        {
            output = nextArg;
            ++i;
            continue;
        }

        if( !input.isEmpty() && !output.isEmpty() )
            break;

        qDebug("Ignoring argument %s", argv[i]);
    }

    if( input.isEmpty() || output.isEmpty() )
    {
        qDebug("\nUsage\n"
               "     %s -i <input-file-name> -o <path-where-files-should-be-extracted>\n"
               "or\n"
               "     %s --input <input-file-name> --output <path-where-files-should-be-extracted>\n",
               argv[0], argv[0]);

        return -1;
    }

    // Check to see if input actually contains Install.js
    QFileInfo fi(input);
    if(fi.suffix() != "gvz")
    {
        qDebug("Input file doesnt have gvz extension. Aborting extract process");
        return -1;
    }

    UpdateFileUncompressor uncompressor;
    uncompressor.setFileName( input );
    uncompressor.setDestination( output );
    QDir dir = uncompressor.uncompress();

    if(dir.exists())
    {
        QString dirPath = dir.absolutePath();
        qDebug("Update extracted into %s", qPrintable(dirPath));
    }

    return 0;
}


