/*
 * yeroth-erp-process-info.cpp
 *
 *  Created on: Apr 23, 2016
 *      Author: Dipl.-Inf. Xavier NOUMBISSI NOUNDOU, Ph.D. (ABD)
 *      Email:  xnoundou7@gmail.com
 */

#include "yeroth-erp-process-info.hpp"

#include "src/utils/yeroth-erp-logger.hpp"

#include "src/utils/yeroth-erp-config.hpp"

#include <QtCore/QByteArray>

#include <QtCore/QDebug>

#include <QtCore/QFile>

#include <QtCore/QProcess>


bool YerothProcessInfo::checkYerothERPAlertDaemonRunning(YerothLogger *logger /* = 0 */)
{
    if (logger)
    {
        logger->log("yeroth-erp-3-0 | YerothProcessInfo::checkYerothERPAlertDaemonRunning");
    }

    QStringList progArguments;

    progArguments << "-a"
    		      << YerothERPConfig::YEROTH_ALERT_DAEMON_ID_STR;

    QProcess checkProcess;

    checkProcess.start("/usr/bin/pgrep", progArguments);

    checkProcess.waitForFinished();

    QFile tmpFile( QString("%1/%2")
    					.arg(YerothERPConfig::temporaryFilesDir, "yerothpsoutput") );

    if (tmpFile.open(QFile::WriteOnly))
    {
        tmpFile.write(checkProcess.readAllStandardOutput().trimmed());
    }

    tmpFile.close();

    progArguments.clear();

    progArguments << YerothUtils::getYerothAlertDaemonExecutableFullpath()
    			  << tmpFile.fileName();

    checkProcess.start("/bin/grep", progArguments);
    checkProcess.waitForFinished(1000);

    QString checkAlertDeamonProcessOutput(checkProcess.readAllStandardOutput().trimmed());

    return checkAlertDeamonProcessOutput.contains(YerothUtils::getYerothAlertDaemonExecutableFullpath());
}

