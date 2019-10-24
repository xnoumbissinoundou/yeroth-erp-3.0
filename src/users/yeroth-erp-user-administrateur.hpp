/*
 * yeroth-erp-user-administrateur.hpp
 *
 *  Created on: Oct 23, 2015
 *      Author: Dipl.-Inf. Xavier NOUMBISSI NOUNDOU, Ph.D. (ABD)
 *      Email:  xnoundou7@gmail.com
 */

#ifndef SRC_YEROTH_ERP_USER_ADMINISTRATEUR_HPP_
#define SRC_YEROTH_ERP_USER_ADMINISTRATEUR_HPP_

#include "yeroth-erp-users.hpp"

class YerothPOSUserAdministrateur : public YerothPOSUser
{
	Q_OBJECT

public:

	YEROTH_CLASS_OPERATORS

	inline YerothPOSUserAdministrateur(YerothERPWindows *allWindows)
	:YerothPOSUser(allWindows)
	{
		_role = RoleAdministrateur;
	}

	inline virtual ~YerothPOSUserAdministrateur(){}
};

#endif /* SRC_YEROTH_ERP_USER_ADMINISTRATEUR_HPP_ */
