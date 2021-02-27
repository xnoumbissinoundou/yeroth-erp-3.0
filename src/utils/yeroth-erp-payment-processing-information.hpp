/*
 * yeroth-erp-payment-processing-information.hpp
 *
 *      Author: DR. XAVIER NOUMBISSI NOUNDOU
 */

#ifndef YEROTH_ERP_PAYMENT_PROCESSING_HPP_
#define YEROTH_ERP_PAYMENT_PROCESSING_HPP_

#include "src/utils/yeroth-erp-utils.hpp"


#include <QtCore/QString>


class YerothERPPaymentProcessingInformation
{
public:

	YEROTH_CLASS_OPERATORS

	inline YerothERPPaymentProcessingInformation()
	:_type_de_paiement(YerothUtils::DECAISSEMENT_INDEFINI),
	 _nouveau_compte_fournisseur(0.0),
	 _nouveau_compteClient_PROGRAMME_DE_FIDELITE_CLIENTS(0.0),
	 _nouveau_compte_client(0.0),
	 _montant_paye(0.0)
	{
	}

	inline ~YerothERPPaymentProcessingInformation()
	{
	}


public:

	double montant_paye(int type_de_paiement);

	bool save_payment_info_record(bool is_supplier_payment = false);


protected:

	bool update_supplier_account(int type_de_paiment,
								 YerothERPWindows &allWindows);

public:

	int _type_de_paiement;

	double _nouveau_compteClient_PROGRAMME_DE_FIDELITE_CLIENTS;

	double _nouveau_compte_client;

	double _montant_paye;

	QString _paiement_intitule_compte_bancaire;

	QString _nom_entreprise;

	QString _NOTES;

	QString _reference;

protected:

	double _nouveau_compte_fournisseur;
};


#endif //YEROTH_ERP_PAYMENT_PROCESSING_HPP_
