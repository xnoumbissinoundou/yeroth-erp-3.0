/*
 * yeroth-erp-user-gestionaire-des-stocks.cpp
 *
 *      Author: PR. XAVIER NOUMBISSI NOUNDOU
 */

#include "yeroth-erp-user-gestionaire-des-stocks.hpp"


YerothPOSUserGestionaireDesStocks::YerothPOSUserGestionaireDesStocks(YerothERPWindows *allWindows)
:YerothPOSUser(allWindows)
{
	create_user_personal_settings_file();

	_role = YerothUtils::ROLE_GESTIONNAIREDESTOCK;
}
