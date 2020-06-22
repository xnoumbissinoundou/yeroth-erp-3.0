/*
 * yeroth-erp-user-magasinier.hpp
 *
 *      Author: Dipl.-Inf. XAVIER NOUMBISSI NOUNDOU
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
		_role = YerothUtils::ROLE_MAGASINIER;
	}

	inline virtual ~YerothPOSUserMagasinier(){}
};

#endif /* SRC_YEROTH_ERP_USER_MAGASINIER_HPP_ */
