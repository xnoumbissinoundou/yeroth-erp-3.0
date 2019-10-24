/*
 * yeroth-erp-credit-card-info.hpp
 *
 *   Author: Dipl.-Inf. Xavier NOUMBISSI NOUNDOU, Ph.D. (ABD)
 *   Email:  xnoundou7@gmail.com
 */

#ifndef YEROTH_ERP_CREDIT_CARD_INFO_HPP_
#define YEROTH_ERP_CREDIT_CARD_INFO_HPP_



#include <QtCore/QString>



class YerothPOSCreditCardInfo
{

public:

	inline YerothPOSCreditCardInfo()
	{
	}


	inline ~YerothPOSCreditCardInfo()
	{
	}

	void clearAllFields();

	QString _issuingCompany;

	QString _nameOnCard;

	QString _postalCode;

	QString _cardNumber;

	QString _expiryDate_YY;

	QString _expiryDate_MM;

	QString _cvCode;
};





#endif // YEROTH_ERP_CREDIT_CARD_INFO_HPP_
