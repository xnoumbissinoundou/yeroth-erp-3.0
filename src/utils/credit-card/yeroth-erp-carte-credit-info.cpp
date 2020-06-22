/*
 * yeroth-erp-credit-card-info.cpp
 *
 *   Author: Dipl.-Inf. XAVIER NOUMBISSI NOUNDOU
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


