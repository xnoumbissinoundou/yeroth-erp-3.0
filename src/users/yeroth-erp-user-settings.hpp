/*
 * yeroth-erp-user-settings.hpp
 *
 * @author: PR. XAVIER NOUMBISSI NOUNDOU
 */

#ifndef SRC_USER_YEROTH_ERP_3_0_USER_SETTINGS_HPP_
#define SRC_USER_YEROTH_ERP_3_0_USER_SETTINGS_HPP_


#include "src/include/yeroth-erp-3-0-definition-oo-class-operators.hpp"

#include "src/utils/print_latex_pdf/yeroth-erp-print_yeroth_latex_util_pdf.hpp"

#include "src/utils/yeroth-erp-utils.hpp"


#include <QtCore/QString>


enum RESULT_PRINTING_PARAMETER
{
	PRINTING_PARAMETER_FILE_DOESNT_EXIT = 0,
	READ_PRINTING_PARAMETER_SUCCESSFUL,
	READ_PRINTING_PARAMETER_FAILED,
	PRINTING_PRAMATER_WINDOW_NOT_YET_DEFINED
};


class YerothERPUserSettings
{

public:

	YEROTH_CLASS_OPERATORS

	inline YerothERPUserSettings()
	:_USERSQL_TABLE_ROW_COUNT(59),
	 _page_from(-1),
	 _page_to(-1),
	 _print_table_row_count(YerothTableViewPRINT_UTILITIES_TEX_TABLE::_STANDARD_INITIAL_MAX_TABLE_ROW_COUNT_A4LANDSCAPE),
	 _a4paper_printing_position(YerothTableViewPRINT_UTILITIES_TEX_TABLE::_A4PAGE_LANDSCAPE_SPECIFICATION)
	{
	}

	inline virtual ~YerothERPUserSettings()
	{
		cleanup_user_settings();
	}

	inline void cleanup_user_settings()
	{
		_window_printing_parameter_key__TO__its_value.clear();
	}

	inline QString GET_WINDOWS_PAGE_FROM_KEY_TO_STORE(const QString &a_window_object_name)
	{
		return QString("%1_page_from")
				.arg(QString("local_parameter_%1")
						.arg(a_window_object_name));
	}

	inline QString GET_WINDOWS_PAGE_TO_KEY_TO_STORE(const QString &a_window_object_name)
	{
		return QString("%1_page_to")
				.arg(QString("local_parameter_%1")
						.arg(a_window_object_name));
	}

	inline QString GET_WINDOWS_A4_PRINTING_POSITION_KEY_TO_STORE(const QString &a_window_object_name)
	{
		return QString("%1_printing_position")
				.arg(QString("local_parameter_%1")
						.arg(a_window_object_name));
	}

	inline QString GET_WINDOWS_USERSQL_TABLE_ROW_COUNT_KEY_TO_STORE(const QString &a_window_object_name)
	{
		return QString("%1_usersql_table_row_count")
				.arg(QString("local_parameter_%1")
						.arg(a_window_object_name));
	}

	inline QString GET_WINDOWS_PRINT_TABLE_ROW_COUNT_KEY_TO_STORE(const QString &a_window_object_name)
	{
		return QString("%1_print_table_row_count")
				.arg(QString("local_parameter_%1")
						.arg(a_window_object_name));
	}

	inline QString GET_PRINTING_PARAMETER_VALUE_FROM_KEY(const QString &a_key)
	{
		return _window_printing_parameter_key__TO__its_value.value(a_key);
	}

	enum RESULT_PRINTING_PARAMETER lire_les_parametres_locaux(const QString &user_local_personal_setting_full_file_path,
															  YerothWindowsCommons *a_current_window_to_table_print_as_parameter = 0);

	bool enregistrer_les_parametres_locaux(const QString &user_local_personal_setting_full_file_path,
										   enum RESULT_PRINTING_PARAMETER a_result_printing_parameter = READ_PRINTING_PARAMETER_SUCCESSFUL);

	inline void PRINT_PARAMETERS_TO_STRING()
	{
		QDEBUG_STRING_OUTPUT_2("YerothERPUserSettings::PRINT_PARAMETERS_TO_STRING()",
				QString("page_from: %1, page_to: %2 "
						"print_table_row_count: %3 "
						"a4paper_printing_position: %4")
					.arg(QString::number(_page_from),
						 QString::number(_page_to),
						 QString::number(_print_table_row_count),
						 _a4paper_printing_position));
	}

protected:

	// local user settings
	// PDF PRINTING PARAMETERS
	uint					_USERSQL_TABLE_ROW_COUNT;

    int						_page_from;
    int 					_page_to;
    uint					_print_table_row_count;
    QString					_a4paper_printing_position;

    QMap<QString, QString> 	_window_printing_parameter_key__TO__its_value;
};


#endif /* SRC_USER_YEROTH_ERP_3_0_USER_SETTINGS_HPP_ */
