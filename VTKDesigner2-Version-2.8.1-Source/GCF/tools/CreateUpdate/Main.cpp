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

#include "UpdateFileCompressor.h"

int main(int argc, char** argv)
{
    // There is no need for an event loop in this program, so we wont
    // create an instance of QCoreApplication

    /* Options are
       --output | -o       Name of the output file (should have gvz as extension)
       --input  | -i       Path (relative or absolute) to the directory containing
                            the files that are supposed to go into the update file
                            This program will look for a file called Install.js in the
                            directory.
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
               "     %s -i <path-to-update-file-directory> -o <output-file-name>\n"
               "or\n"
               "     %s --input <path-to-update-file-directory> --output <output-file-name>\n",
               argv[0], argv[0]);

        return -1;
    }

    // Check to see if input actually contains Install.js
    QFileInfo fi(output);
    if(fi.suffix() != "gvz")
    {
        output = QString("%1/%2.gvz").arg( fi.absolutePath() ).arg( fi.baseName() );
        qDebug("Setting output file extension to gvz. Output file is now %s",
                qPrintable(output));
    }

    if( !QFile::exists( QString("%1/Install.js").arg(input) ) )
    {
        qDebug("Could not find Install.js in the input directory. Aborting %s", argv[0]);
        return -1;
    }

    UpdateFileCompressor compressor;
    compressor.setFileName( output );
    compressor.setSource( input );
    compressor.compress();

    return 0;
}


