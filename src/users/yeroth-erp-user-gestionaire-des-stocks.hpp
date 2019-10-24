/*
 * yeroth-erp-user-gestionaire-des-stocks.hpp
 *
 *  Created on: Oct 23, 2015
 *      Author: Dipl.-Inf. Xavier NOUMBISSI NOUNDOU, Ph.D. (ABD)
 *      Email:  xnoundou7@gmail.com
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
		_role = RoleGestionaireDesStocks;
	}

	inline virtual ~YerothPOSUserGestionaireDesStocks(){}
};


#endif /* SRC_YEROTH_ERP_USER_MANAGER_HPP_ */
