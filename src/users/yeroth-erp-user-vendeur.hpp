/*
 * yeroth-erp-user-vendeur.hpp
 *
 *      Author: PR. XAVIER NOUMBISSI NOUNDOU
 */

#ifndef SRC_YEROTH_ERP_USER_VENDEUR_HPP_
#define SRC_YEROTH_ERP_USER_VENDEUR_HPP_

#include "yeroth-erp-users.hpp"

class YerothERPUserVendeur: public YerothPOSUser
{
	Q_OBJECT

public:

	YEROTH_CLASS_OPERATORS

	YerothERPUserVendeur(YerothERPWindows *allWindows);

	inline virtual ~YerothERPUserVendeur(){}
};


#endif /* SRC_YEROTH_ERP_USER_MANAGER_HPP_ */
