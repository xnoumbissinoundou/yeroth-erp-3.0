/*
 * yeroth-erp-user-gestionaire-des-stocks.hpp
 *
 *      Author: DR. XAVIER NOUMBISSI NOUNDOU
 */

#ifndef SRC_YEROTH_ERP_USER_GESTIONAIRE_DES_STOCKS_HPP_
#define SRC_YEROTH_ERP_USER_GESTIONAIRE_DES_STOCKS_HPP_

#include "yeroth-erp-users.hpp"

class YerothPOSUserGestionaireDesStocks: public YerothPOSUser
{
	Q_OBJECT

public:

	YEROTH_CLASS_OPERATORS

	inline YerothPOSUserGestionaireDesStocks(YerothERPWindows *allWindows)
	:YerothPOSUser(allWindows)
	{
		_role = YerothUtils::ROLE_GESTIONNAIREDESTOCK;
	}

	inline virtual ~YerothPOSUserGestionaireDesStocks(){}
};


#endif /* SRC_YEROTH_ERP_USER_MANAGER_HPP_ */
