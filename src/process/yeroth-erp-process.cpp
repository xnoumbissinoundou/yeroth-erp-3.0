/*
 * yeroth-erp-process.cpp
 *
 *      Author: Dipl.-Inf. Xavier NOUMBISSI NOUNDOU, Ph.D. (ABD)
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

	progArguments << "/etc/init.d/yeroth-erp-alert-3-0-initd.sh";

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

    	progArguments << "/etc/init.d/yeroth-erp-alert-3-0-initd.sh";

    	progArguments << "start";

        YerothERPProcess::startDetached(*_alertDeamonProcess,
        								"lxqt-sudo",
    									progArguments);
    }
    else
    {
    	msg.clear();

        msg.append(QObject::trUtf8("Le système d'alerte \"yeroth-erp-alert-3-0\" est déjà en marche !"));

        YerothQMessageBox::information(YerothUtils::getAllWindows()->_adminWindow,
        							  QObject::trUtf8("démarrage du système d'alerte - yeroth-erp-alert-3-0"),
                                      msg);
    }

    return started;
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


