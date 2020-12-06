/*
 * yeroth-erp-process-info.cpp
 *
 *      Author: DR. XAVIER NOUMBISSI NOUNDOU
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

	QString yeroth_erp_3_0_restore_backup_sql_file(QString("%1.sql")
					.arg(FILE_NAME_USERID_CURRENT_TIME("yeroth_erp_3_0_BACKUP_RESTORE")));

	QString mysqlProcessProgram("/usr/bin/pgrep");

	QString yerothpsoutput_file(QString("%1/%2")
	    							.arg(YerothERPConfig::temporaryFilesDir,
	    								 "yerothpsoutput"));

	int output_file_size =
			YerothERPProcess::start_PROCESS_AND_READ_PROCESS_output_INTO_FILE(mysqlProcessProgram,
																			  yerothpsoutput_file,
																			  progArguments);
	if (output_file_size > 0)
	{
		return YerothUtils::GREP_YEROTH_FILE_CONTENT(yerothpsoutput_file,
    									  	  	 	 YerothUtils::getYerothAlertDaemonExecutableFullpath());
	}

	return false;
}
