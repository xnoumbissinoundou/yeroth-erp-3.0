/*
 * yeroth-erp-command-line-parser.cpp
 *
 *      Author: PR. XAVIER NOUMBISSI NOUNDOU
 */

#include "yeroth-erp-command-line-parser.hpp"


#include <QtCore/QString>
#include <QtCore/QDebug>


#include <getopt.h>


enum YerothCommandLineArgumentType
YerothCommandLineParser::parseProgramArguments(int argc, char *argv[])
{
    int getOptLongparseResult;

    static struct option long_options[] =
    {
        {"help",    no_argument, 0,  'h' },
        {"gui", 	no_argument, 0,  'g' },
        {"version", no_argument, 0,  'v' },
        {0,         0,           0,   0  }
    };

    do
    {
        getOptLongparseResult = getopt_long(argc, argv, "hgv", long_options, 0);

        if (-1 == getOptLongparseResult)
        {
            return UNDEFINED;
        }

        switch (getOptLongparseResult)
        {
        case 'h':
            return HELP;

        case 'g':
            return GUI;

        case 'v':
            return VERSION;

        default:
            //qDebug() << QString("?? getopt returned character code (%1) ??\n")
            //		.arg(QString::number(getOptLongparseResult));
            return UNDEFINED;
        }
    }
    while(getOptLongparseResult != -1);
}
