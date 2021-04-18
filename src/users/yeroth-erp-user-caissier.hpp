/*
 * yeroth-erp-user-caissier.hpp
 *
 *      Author: PR. XAVIER NOUMBISSI NOUNDOU
 */

#ifndef SRC_YEROTH_ERP_USER_CAISSIER_HPP_
#define SRC_YEROTH_ERP_USER_CAISSIER_HPP_

#include "yeroth-erp-users.hpp"

class YerothPOSUserCaissier : public YerothPOSUser
{
	Q_OBJECT

public:

	YEROTH_CLASS_OPERATORS

	YerothPOSUserCaissier(YerothERPWindows *allWindows);

	inline virtual ~YerothPOSUserCaissier(){}
};

#endif /* SRC_YEROTH_ERP_USER_CAISSIER_HPP_ */
