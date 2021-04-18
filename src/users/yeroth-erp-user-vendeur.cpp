/*
 * yeroth-erp-user-vendeur.cpp
 *
 *      Author: PR. XAVIER NOUMBISSI NOUNDOU
 */

#include "yeroth-erp-user-vendeur.hpp"


YerothERPUserVendeur::YerothERPUserVendeur(YerothERPWindows *allWindows)
:YerothPOSUser(allWindows)
{
	create_user_personal_settings_file();

	_role = YerothUtils::ROLE_VENDEUR;
}
