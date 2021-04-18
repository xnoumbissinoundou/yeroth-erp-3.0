/*
 * yeroth-erp-user-magasinier.hpp
 *
 *      Author: PR. XAVIER NOUMBISSI NOUNDOU
 */

#ifndef SRC_YEROTH_ERP_USER_MAGASINIER_HPP_
#define SRC_YEROTH_ERP_USER_MAGASINIER_HPP_

#include "yeroth-erp-users.hpp"

class YerothPOSUserMagasinier : public YerothPOSUser
{
	Q_OBJECT

public:

	YEROTH_CLASS_OPERATORS

	YerothPOSUserMagasinier(YerothERPWindows *allWindows);

	inline virtual ~YerothPOSUserMagasinier(){}
};

#endif /* SRC_YEROTH_ERP_USER_MAGASINIER_HPP_ */
