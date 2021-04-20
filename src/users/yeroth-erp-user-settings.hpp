/*
 * yeroth-erp-user-settings.hpp
 *
 * @author: PR. XAVIER NOUMBISSI NOUNDOU
 */

#ifndef SRC_USER_YEROTH_ERP_3_0_USER_SETTINGS_HPP_
#define SRC_USER_YEROTH_ERP_3_0_USER_SETTINGS_HPP_


#include "src/include/yeroth-erp-3-0-definition-oo-class-operators.hpp"


#include <QtCore/QString>


#include "src/utils/print_latex_pdf/yeroth-erp-print_yeroth_latex_util_pdf.hpp"


class YerothERPUserSettings
{

public:

	YEROTH_CLASS_OPERATORS

	inline YerothERPUserSettings()
	:_page_from(-1),
	 _page_to(-1),
	 _print_table_row_count(YerothTableViewPRINT_UTILITIES_TEX_TABLE::_STANDARD_INITIAL_MAX_TABLE_ROW_COUNT_A4LANDSCAPE),
	 _a4paper_printing_position(YerothTableViewPRINT_UTILITIES_TEX_TABLE::_A4PAGE_LANDSCAPE_SPECIFICATION)
	{
	}

	bool lire_les_parametres_locaux(const QString &user_local_personal_setting_full_file_path);

	bool enregistrer_les_parametres_locaux(const QString &user_local_personal_setting_full_file_path);

protected:
	// local user settings
	// PDF PRINTING PARAMETERS
    int		_page_from;
    int 	_page_to;
    uint	_print_table_row_count;

    QString	_a4paper_printing_position;
};


#endif /* SRC_USER_YEROTH_ERP_3_0_USER_SETTINGS_HPP_ */
