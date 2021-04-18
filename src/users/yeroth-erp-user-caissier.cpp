/*
 * yeroth-erp-user-caissier.cpp
 *
 *      Author: PR. XAVIER NOUMBISSI NOUNDOU
 */

#include "yeroth-erp-user-caissier.hpp"


YerothPOSUserCaissier::YerothPOSUserCaissier(YerothERPWindows *allWindows)
:YerothPOSUser(allWindows)
{
	create_user_personal_settings_file();

	_role = YerothUtils::ROLE_CAISSIER;
}
