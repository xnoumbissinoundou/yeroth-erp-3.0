/*
 * yeroth-erp-payment-processing.hpp
 *
 *      Author: DR. XAVIER NOUMBISSI NOUNDOU
 */

#ifndef YEROTH_ERP_PAYMENT_PROCESSING_HPP_
#define YEROTH_ERP_PAYMENT_PROCESSING_HPP_

#include "src/utils/yeroth-erp-utils.hpp"


#include <QtCore/QString>


class YerothERPPaymentProcessing
{
public:

	YEROTH_CLASS_OPERATORS

	inline YerothERPPaymentProcessing()
	:_type_de_paiement(YerothUtils::DECAISSEMENT_INDEFINI),
	 _nouveau_compte_client(0.0),
	 _montant_paye(0.0)
	{
	}

	inline ~YerothERPPaymentProcessing()
	{
	}


	bool save_payment_info_record();


	int _type_de_paiement;

	double _nouveau_compte_client;

	double _montant_paye;

	QString _nom_entreprise;

	QString _NOTES;

	QString _reference;
};


#endif //YEROTH_ERP_PAYMENT_PROCESSING_HPP_
