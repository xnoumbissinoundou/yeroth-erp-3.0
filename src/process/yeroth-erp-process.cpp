/*
 * yeroth-erp-process.cpp
 *
 *      Author: PR. XAVIER NOUMBISSI NOUNDOU
 */


#include "yeroth-erp-process.hpp"

#include "src/yeroth-erp-windows.hpp"

#include "src/widgets/yeroth-erp-qmessage-box.hpp"

#include "src/process/yeroth-erp-process-info.hpp"


#include <QtCore/QThread>


bool YerothERPProcess::_yerothPOSAlertProcessFilesSet(false);

QProcess *YerothERPProcess::_alertDeamonProcess(new QProcess);


bool YerothERPProcess::stopYerothERPAlertProcess()
{
	if (!_yerothPOSAlertProcessFilesSet)
	{
		_alertDeamonProcess->setStandardErrorFile("/dev/null");

		_alertDeamonProcess->setStandardOutputFile("/dev/null");

		_yerothPOSAlertProcessFilesSet = true;
	}

	QString msg;

	QStringList progArguments;

	progArguments << "/etc/init.d/yeroth-erp-3-0-system-daemon-initd.sh";

	progArguments << "stop";

    //We don't start this process with 'startDetached' so
    //the following call to function 'check_alert_daemon_process()'
    //works fine.

    YerothERPProcess::startAndWaitForFinished(*_alertDeamonProcess,
    										 "lxqt-sudo",
											 progArguments,
											 60000);

    return _alertDeamonProcess->waitForFinished(5000);
}


bool YerothERPProcess::startYerothERPAlertProcess()
{
	if (!_yerothPOSAlertProcessFilesSet)
	{
    	_alertDeamonProcess->setStandardErrorFile("/dev/null");

    	_alertDeamonProcess->setStandardOutputFile("/dev/null");

    	_yerothPOSAlertProcessFilesSet = true;
	}

    bool started = YerothProcessInfo::checkYerothERPAlertDaemonRunning();

    QString msg;

    if (!started)
    {
    	QStringList progArguments;

    	progArguments << "/etc/init.d/yeroth-erp-3-0-system-daemon-initd.sh";

    	progArguments << "start";

        YerothERPProcess::startDetached(*_alertDeamonProcess,
        								"lxqt-sudo",
    									progArguments);
    }
    else
    {
    	msg.clear();

        msg.append(QObject::trUtf8("Le système d'alerte \"yeroth-erp-3-0-system-daemon\" est déjà en marche !"));

        YerothQMessageBox::information(YerothUtils::getAllWindows()->_adminWindow,
        							  QObject::trUtf8("démarrage du système d'alerte - yeroth-erp-3-0-system-daemon"),
                                      msg);
    }

    return started;
}


int YerothERPProcess::
	start_PROCESS_AND_READ_PROCESS_output_INTO_FILE(const QString &program_executable_location_full_path,
													const QString &output_file_full_path,
												    const QStringList &program_executable_args)
{
    QProcess A_YEROTH_PROCESS;

    A_YEROTH_PROCESS.start(program_executable_location_full_path,
    					   program_executable_args);

    if ( ! A_YEROTH_PROCESS.waitForFinished() )
    {
		return -1;
    }

    QFile tmpFile(output_file_full_path);

    if (tmpFile.open(QFile::WriteOnly))
    {
        tmpFile.write(A_YEROTH_PROCESS.readAllStandardOutput().trimmed());
    }
    else
    {
    	return -1;
    }

    int output_file_size = tmpFile.size();

    tmpFile.close();

    return output_file_size;
}


QString YerothERPProcess::compileLatex(QString prefixFileName)
{
    QStringList progArguments;

    progArguments << "-interaction";

    progArguments << "nonstopmode";

    progArguments << QString("%1tex")
    					.arg(prefixFileName);

    YerothERPProcess::startAndWaitForFinished(YerothERPConfig::pathToPdfLatex(),
    										 progArguments,
											 -1);

    QThread::sleep(0.3);

    YerothERPProcess::startAndWaitForFinished(YerothERPConfig::pathToPdfLatex(),
        										 progArguments,
    											 -1);

    QThread::sleep(0.3);

    progArguments.clear();

    QString pdfResultFile = QString("%1pdf")
    							.arg(prefixFileName);

    return pdfResultFile;
}


QString YerothERPProcess::compileWITH_LUATEX_Latex(QString prefixFileName)
{
    QStringList progArguments;

    progArguments << "-interaction";

    progArguments << "nonstopmode";

    progArguments << QString("%1tex")
    					.arg(prefixFileName);

    YerothERPProcess::startAndWaitForFinished(YerothERPConfig::pathToLualatex(),
    										 progArguments,
											 -1);
    progArguments.clear();

    QString pdfResultFile = QString("%1pdf")
    							.arg(prefixFileName);

    return pdfResultFile;
}


QString YerothERPProcess::startPdfViewerProcess(QString aPDFFileName)
{
    QStringList progArguments(aPDFFileName);

    QProcess::startDetached(YerothERPConfig::pathToPdfReader,
    						progArguments,
							YerothERPConfig::temporaryFilesDir);

    return aPDFFileName;
}


bool YerothERPProcess::startDetached(QProcess &aProcess,
									 QString programFileAbsolutePath,
									 QStringList programArguments)
{
	aProcess.setWorkingDirectory(YerothERPConfig::temporaryFilesDir);

	return aProcess.startDetached(programFileAbsolutePath,
				   	   	   	   	  programArguments);
}


bool YerothERPProcess::startAndWaitForFinished(QProcess &aProcess,
											  QString programFileAbsolutePath,
										  	  QStringList programArguments,
											  int waitForFinished)
{
	aProcess.setWorkingDirectory(YerothERPConfig::temporaryFilesDir);

	aProcess.start(programFileAbsolutePath,
				   programArguments);

	return aProcess.waitForFinished(waitForFinished);
}


bool YerothERPProcess::startAndWaitForFinished(QString programFileAbsolutePath,
										  	  QStringList programArguments,
											  int waitForFinished)
{
	QProcess aProcess;

	aProcess.setWorkingDirectory(YerothERPConfig::temporaryFilesDir);

	aProcess.start(programFileAbsolutePath,
				   programArguments);

	return aProcess.waitForFinished(waitForFinished);
}


