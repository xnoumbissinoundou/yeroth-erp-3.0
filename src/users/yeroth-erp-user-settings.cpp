/*
 * yeroth-erp-user-settings.cpp
 *
 * @author: PR. XAVIER NOUMBISSI NOUNDOU
 */


#include "yeroth-erp-user-settings.hpp"

#include "src/yeroth-erp-windows.hpp"

#include "src/dialogs/yeroth-erp-IMPRESSION_DE_DOCUMENT-dialog.hpp"

#include "src/utils/yeroth-erp-utils.hpp"


#include <QtCore/QTextStream>

#include <QtCore/QFile>


RESULT_PRINTING_PARAMETER YerothERPUserSettings::lire_les_parametres_locaux(const QString &user_local_personal_setting_full_file_path,
																			YerothWindowsCommons *a_current_window_to_table_print_as_parameter /* = 0 */)
{
	YerothERPWindows *allWindows = YerothUtils::getAllWindows();

	if (0 == allWindows)
	{
//		QDEBUG_STRING_OUTPUT_1("YEROTH-PP-0-failed");
		return READ_PRINTING_PARAMETER_FAILED;
	}

	YerothIMPRESSION_DE_DOCUMENT_Dialog *impressionDeDocumentDialog =
			allWindows->_impressionDeDocumentDialog;

	if (0 == impressionDeDocumentDialog)
	{
//		QDEBUG_STRING_OUTPUT_1("YEROTH-PP-1-failed");
		return READ_PRINTING_PARAMETER_FAILED;
	}

	YerothWindowsCommons *a_current_window_to_print_table =
			impressionDeDocumentDialog->_current_window_to_print;

	if (0 == a_current_window_to_print_table)
	{
		if (0 != a_current_window_to_table_print_as_parameter)
		{
//			QDEBUG_STRING_OUTPUT_1("YEROTH-PP-2-success-initialization");
			a_current_window_to_print_table = a_current_window_to_table_print_as_parameter;
		}
		else
		{
//			QDEBUG_STRING_OUTPUT_1("YEROTH-PP-2-failed");
			return PRINTING_PRAMATER_WINDOW_NOT_YET_DEFINED;
		}
	}

	QString a_current_window_object_name = a_current_window_to_print_table->objectName();

	if (_window_printing_parameter_key__TO__its_value.contains(
			GET_WINDOWS_TABLE_ROW_COUNT_KEY_TO_STORE(a_current_window_object_name)))
	{
		return READ_PRINTING_PARAMETER_SUCCESSFUL;
	}


	QFile file(user_local_personal_setting_full_file_path);

	if (!file.open(QFile::ReadOnly))
	{
		return PRINTING_PARAMETER_FILE_DOESNT_EXIT;
	}


	QTextStream init_cfg(&file);
	QString aValue;
	QString line;
	QStringList list;

	do
	{
		line = init_cfg.readLine();

		if (!line.isEmpty())
		{
			list = line.split(YEROTH_ERP_3_0_CONFIGURATION_FILE_SEPARATION_OPERATOR);

//			QDEBUG_STRING_OUTPUT_2("line", line);

			_window_printing_parameter_key__TO__its_value.insert(
					list.at(0).trimmed(),
					list.at(1).trimmed());
		}
	}
	while(!line.isNull());


	return READ_PRINTING_PARAMETER_SUCCESSFUL;
}


bool YerothERPUserSettings::enregistrer_les_parametres_locaux(const QString &user_local_personal_setting_full_file_path,
															  enum RESULT_PRINTING_PARAMETER a_result_printing_parameter/* = READ_PRINTING_PARAMETER_SUCCESSFUL*/)
{
	QFile file(user_local_personal_setting_full_file_path);

	if (PRINTING_PRAMATER_WINDOW_NOT_YET_DEFINED == a_result_printing_parameter)
	{
		if (file.exists())
		{
			return true;
		}
	}

	if (!file.open(QFile::WriteOnly))
	{
		return false;
	}

	YerothERPWindows *allWindows = YerothUtils::getAllWindows();

	if (0 != allWindows)
	{
		YerothIMPRESSION_DE_DOCUMENT_Dialog *impressionDeDocumentDialog =
				allWindows->_impressionDeDocumentDialog;

		QTextStream textStream(&file);

		if (0 != impressionDeDocumentDialog)
		{
			YerothWindowsCommons *a_current_window_to_print_table =
					impressionDeDocumentDialog->_current_window_to_print;

			if (0 != a_current_window_to_print_table)
			{
				QString a_current_window_object_name =
						a_current_window_to_print_table->objectName();

				QString aPageTO_STRING =
						QString::number(a_current_window_to_print_table->get_PRINTING_PARAMETER_pageTO());

				QString aPageFROM_STRING =
						QString::number(a_current_window_to_print_table->get_PRINTING_PARAMETER_pageFROM());

				QString aPrintingPosition_STRING =
						a_current_window_to_print_table->get_PRINTING_PARAMETER_printing_position();

				QString aTableRowCount_STRING =
						QString::number(a_current_window_to_print_table->get_PRINTING_PARAMETER_print_table_row_count());

				_window_printing_parameter_key__TO__its_value.insert(
						GET_WINDOWS_TABLE_ROW_COUNT_KEY_TO_STORE(a_current_window_object_name),
						aTableRowCount_STRING);

				_window_printing_parameter_key__TO__its_value.insert(
						GET_WINDOWS_A4_PRINTING_POSITION_KEY_TO_STORE(a_current_window_object_name),
						aPrintingPosition_STRING);

				_window_printing_parameter_key__TO__its_value.insert(
						GET_WINDOWS_PAGE_FROM_KEY_TO_STORE(a_current_window_object_name),
						aPageFROM_STRING);

				_window_printing_parameter_key__TO__its_value.insert(
						GET_WINDOWS_PAGE_TO_KEY_TO_STORE(a_current_window_object_name),
						aPageTO_STRING);
			}
			else //in user settings file initialization
			{
				textStream
				<< QString("local_parameter_table_row_count=%2\n")
					.arg(QString::number(_print_table_row_count))

				<< QString("local_parameter_printing_position=%2\n")
					.arg(_a4paper_printing_position)

				<< QString("local_parameter_page_from=%2\n")
					.arg(QString::number(_page_from))

				<< QString("local_parameter_page_to=%2\n")
					.arg(QString::number(_page_to));
			}
		}


//		qDebug() << _window_printing_parameter_key__TO__its_value;


		QMapIterator<QString, QString> it(_window_printing_parameter_key__TO__its_value);

		while(it.hasNext())
		{
			it.next();

			textStream
			<< QString("%1=%2\n")
				.arg(it.key(),
					 it.value());
		}
	}

	file.close();

	return true;
}
