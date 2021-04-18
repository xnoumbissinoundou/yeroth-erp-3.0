/*
 * yeroth-erp-user-settings.hpp
 *
 * @author: PR. XAVIER NOUMBISSI NOUNDOU
 */

#ifndef SRC_USER_YEROTH_ERP_3_0_USER_SETTINGS_HPP_
#define SRC_USER_YEROTH_ERP_3_0_USER_SETTINGS_HPP_


#include "src/include/yeroth-erp-3-0-definition-oo-class-operators.hpp"


#include <QtCore/QString>


class YerothERPUserSettings
{

public:

	YEROTH_CLASS_OPERATORS

	bool enregistrer_les_parametres_serveurs();

	bool enregistrer_les_parametres_locaux();

protected:
	// local user settings
	// PDF PRINTING PARAMETERS
    int		_page_from;
    int 	_page_to;
    uint	_print_table_row_count;

    QString	_a4paper_printing_position;
};


#endif /* SRC_USER_YEROTH_ERP_3_0_USER_SETTINGS_HPP_ */
