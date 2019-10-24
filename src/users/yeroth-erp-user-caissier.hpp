/*
 * yeroth-erp-user-caissier.hpp
 *
 *  Created on: Oct 23, 2015
 *      Author: Dipl.-Inf. Xavier NOUMBISSI NOUNDOU, Ph.D. (ABD)
 *      Email:  xnoundou7@gmail.com
 */

#ifndef SRC_YEROTH_ERP_USER_CAISSIER_HPP_
#define SRC_YEROTH_ERP_USER_CAISSIER_HPP_

#include "yeroth-erp-users.hpp"

class YerothPOSUserCaissier : public YerothPOSUser
{
	Q_OBJECT

public:

	YEROTH_CLASS_OPERATORS

	inline YerothPOSUserCaissier(YerothERPWindows *allWindows)
	:YerothPOSUser(allWindows)
	{
		_role = RoleCaissier;
	}

	inline virtual ~YerothPOSUserCaissier(){}
};

#endif /* SRC_YEROTH_ERP_USER_CAISSIER_HPP_ */
