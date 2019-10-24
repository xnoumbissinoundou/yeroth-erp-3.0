/*
 * yeroth-erp-user-magasinier.hpp
 *
 *  Created on: Oct 23, 2015
 *      Author: Dipl.-Inf. Xavier NOUMBISSI NOUNDOU, Ph.D. (ABD)
 *      Email:  xnoundou7@gmail.com
 */

#ifndef SRC_YEROTH_ERP_USER_MAGASINIER_HPP_
#define SRC_YEROTH_ERP_USER_MAGASINIER_HPP_

#include "yeroth-erp-users.hpp"

class YerothPOSUserMagasinier : public YerothPOSUser
{
	Q_OBJECT

public:

	YEROTH_CLASS_OPERATORS

	inline YerothPOSUserMagasinier(YerothERPWindows *allWindows)
	:YerothPOSUser(allWindows)
	{
		_role = RoleMagasinier;
	}

	inline virtual ~YerothPOSUserMagasinier(){}
};

#endif /* SRC_YEROTH_ERP_USER_MAGASINIER_HPP_ */
