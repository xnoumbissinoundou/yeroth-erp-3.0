/*
 * yeroth-erp-user-manager.hpp
 *
 *  Created on: Oct 23, 2015
 *      Author: Dipl.-Inf. Xavier NOUMBISSI NOUNDOU, Ph.D. (ABD)
 *      Email:  xnoundou7@gmail.com
 */

#ifndef SRC_YEROTH_ERP_USER_MANAGER_HPP_
#define SRC_YEROTH_ERP_USER_MANAGER_HPP_

#include "yeroth-erp-users.hpp"

class YerothPOSUserManager : public YerothPOSUser
{
	Q_OBJECT

public:

	YEROTH_CLASS_OPERATORS

	inline YerothPOSUserManager(YerothERPWindows *allWindows)
	:YerothPOSUser(allWindows)
	{
		_role = RoleManager;
	}

	inline virtual ~YerothPOSUserManager(){}
};


#endif /* SRC_YEROTH_ERP_USER_MANAGER_HPP_ */
