/*
 * yeroth-erp-user-magasinier.cpp
 *
 *      Author: PR. XAVIER NOUMBISSI NOUNDOU
 */

#include "yeroth-erp-user-magasinier.hpp"


YerothPOSUserMagasinier::YerothPOSUserMagasinier(YerothERPWindows *allWindows)
:YerothPOSUser(allWindows)
{
	create_user_personal_settings_file();

	_role = YerothUtils::ROLE_MAGASINIER;
}
