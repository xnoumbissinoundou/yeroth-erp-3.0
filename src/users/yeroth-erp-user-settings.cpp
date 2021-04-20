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


bool YerothERPUserSettings::lire_les_parametres_locaux(const QString &user_local_personal_setting_full_file_path)
{
	QFile file(user_local_personal_setting_full_file_path);

	if (!file.open(QFile::ReadOnly))
	{
		return false;
	}

	QTextStream init_cfg(&file);
	QString aValue;
	QString line;
	QStringList list;

	do
	{
		line = init_cfg.readLine();

		list = line.split(YEROTH_ERP_3_0_CONFIGURATION_FILE_SEPARATION_OPERATOR);

		QDEBUG_STRING_OUTPUT_2("line", line);

		if (YerothUtils::isEqualCaseInsensitive("local_parameter_table_row_count", list.at(0)))
		{
			_print_table_row_count = list.at(1).trimmed().toUInt();
		}
		else if (YerothUtils::isEqualCaseInsensitive("local_parameter_printing_position", list.at(0)))
		{
			_a4paper_printing_position = list.at(1).trimmed();
		}
		else if (YerothUtils::isEqualCaseInsensitive("local_parameter_page_from", list.at(0)))
		{
			_page_from = list.at(1).trimmed().toInt();
		}
		else if (YerothUtils::isEqualCaseInsensitive("local_parameter_page_to", list.at(0)))
		{
			_page_to = list.at(1).trimmed().toInt();
		}
	}
	while(!line.isNull());


	return true;
}


//	local_parameter_YerothStocksWindow_table_row_count=20
//	local_parameter_YerothStocksWindow_printing_position=horizontal
//	local_parameter_YerothStocksWindow_page_from_int=-1
//	local_parameter_YerothStocksWindow_page_to_int=-1

bool YerothERPUserSettings::enregistrer_les_parametres_locaux(const QString &user_local_personal_setting_full_file_path)
{
	QFile file(user_local_personal_setting_full_file_path);

	if (!file.open(QFile::WriteOnly))
	{
		return false;
	}

	YerothERPWindows *allWindows = YerothUtils::getAllWindows();

	if (0 != allWindows)
	{
		YerothIMPRESSION_DE_DOCUMENT_Dialog *impressionDeDocumentDialog =
				allWindows->_impressionDeDocumentDialog;

		if (0 != impressionDeDocumentDialog)
		{
			YerothWindowsCommons *a_current_window_to_print_table =
					impressionDeDocumentDialog->_current_window_to_print;

			QTextStream textStream(&file);

			if (0 != a_current_window_to_print_table)
			{
				textStream
				<< QString("local_parameter_table_row_count=%2\n")
					.arg(QString::number(a_current_window_to_print_table->get_PRINTING_PARAMETER_print_table_row_count()))

				<< QString("local_parameter_printing_position=%2\n")
					.arg(a_current_window_to_print_table->get_PRINTING_PARAMETER_printing_position())

				<< QString("local_parameter_page_from=%2\n")
					.arg(QString::number(a_current_window_to_print_table->get_PRINTING_PARAMETER_pageFROM()))

				<< QString("local_parameter_page_to=%2\n")
					.arg(QString::number(a_current_window_to_print_table->get_PRINTING_PARAMETER_pageTO()));
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
	}

	file.close();

	return false;
}
