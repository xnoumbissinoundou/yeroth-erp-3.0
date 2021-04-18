/*
 * yeroth-erp-user-patron.cpp
 *
 *      Author: PR. XAVIER NOUMBISSI NOUNDOU
 */

#include "yeroth-erp-user-manager.hpp"


YerothPOSUserManager::YerothPOSUserManager(YerothERPWindows *allWindows)
:YerothPOSUser(allWindows)
{
	create_user_personal_settings_file();

	_role = YerothUtils::ROLE_MANAGER;
}
