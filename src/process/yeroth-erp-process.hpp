/*
 * yeroth-erp-process.hpp
 *
 *      Author: DIPL.-INF. XAVIER NOUMBISSI NOUNDOU
 */

#ifndef YEROTH_ERP_PROCESS_HPP_
#define YEROTH_ERP_PROCESS_HPP_


#include <QtCore/QProcess>


#include "src/utils/yeroth-erp-config.hpp"


#include "src/utils/yeroth-erp-utils.hpp"


class YerothERPProcess : public QProcess
{

public:

	inline YerothERPProcess()
	{
	    setWorkingDirectory(YerothERPConfig::temporaryFilesDir);
	}


	inline ~YerothERPProcess()
	{
	}

	static bool stopYerothERPAlertProcess();

	static bool startYerothERPAlertProcess();

	/**
	 * Returns the size of the output file created
	 * by execution of the program 'program' !
	 */
	static int start_PROCESS_AND_READ_PROCESS_output_INTO_FILE(const QString &program_executable_location_full_path,
													   	   	   const QString &program_working_directory_full_path,
															   const QString &output_file_name,
															   const QStringList &program_executable_args);

	/**
	 * @return: the full file path name of the
	 *          opened PDF file with prefix: prefixFileName
	 *          ("prefixFileName.pdf").
	 */
	static QString compileLatex(QString prefixFileName);

	static QString startPdfViewerProcess(QString aPDFFileName);

	static bool startDetached(QProcess &aProcess,
					   	   	  QString programFileAbsolutePath,
							  QStringList programArguments);

	static bool startAndWaitForFinished(QProcess &aProcess,
										QString programFileAbsolutePath,
										QStringList programArguments,
										int waitForFinished);

	static bool startAndWaitForFinished(QString programFileAbsolutePath,
									    QStringList programArguments,
									    int waitForFinished);


private:

	static bool 		_yerothPOSAlertProcessFilesSet;

    static QProcess		*_alertDeamonProcess;
};


#endif /* YEROTH_ERP_PROCESS_HPP_ */
