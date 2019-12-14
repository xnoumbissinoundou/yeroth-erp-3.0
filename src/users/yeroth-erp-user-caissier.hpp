/*
 * yeroth-erp-user-caissier.hpp
 *
 *      Author: Xavier NOUMBISSI NOUNDOU, Dipl.-Inf., Ph.D. (ABD)
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
