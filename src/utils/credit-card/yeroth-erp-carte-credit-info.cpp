/*
 * yeroth-erp-credit-card-info.cpp
 *
 *   Author: Dipl.-Inf. Xavier NOUMBISSI NOUNDOU, Ph.D. (ABD)
 *   Email:  xnoundou7@gmail.com
 */



#include "yeroth-erp-carte-credit-info.hpp"



void YerothPOSCreditCardInfo::clearAllFields()
{
	_issuingCompany.clear();

	_nameOnCard.clear();

	_postalCode.clear();

	_cardNumber.clear();

	_expiryDate_YY.clear();

	_expiryDate_MM.clear();

	_cvCode.clear();
}


