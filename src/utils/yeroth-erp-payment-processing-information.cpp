/*
 * yeroth-erp-payment-processing-information.cpp
 *
 *      Author: DR. XAVIER NOUMBISSI NOUNDOU
 */

#include "yeroth-erp-payment-processing-information.hpp"

#include "src/yeroth-erp-windows.hpp"

#include "src/users/yeroth-erp-users.hpp"



bool YerothERPPaymentProcessingInformation::update_supplier_account(int type_de_paiment,
																	YerothERPWindows &allWindows)
{
	YerothERPWindows *_allWindows = YerothUtils::getAllWindows();

	if (0 == _allWindows)
	{
		return false;
	}

	bool success_update_compte_fournisseur = false;

	YerothSqlTableModel &fournisseursTableModel = allWindows.getSqlTableModel_fournisseurs();

	QString fournisseursTableFilter = QString("%1 = '%2'")
        	            				.arg(YerothDatabaseTableColumn::NOM_ENTREPRISE,
        	            					 _nom_entreprise);

	fournisseursTableModel.yerothSetFilter_WITH_where_clause(fournisseursTableFilter);

	int rows = fournisseursTableModel.easySelect();

	if (rows > 0)
	{
		QSqlRecord fournisseursRecord = fournisseursTableModel.record(0);

		double compte_fournisseur =
				GET_SQL_RECORD_DATA(fournisseursRecord, YerothDatabaseTableColumn::COMPTE_FOURNISSEUR).toDouble();

		_nouveau_compte_fournisseur = compte_fournisseur + montant_paye(type_de_paiment);

		fournisseursRecord.setValue(YerothDatabaseTableColumn::COMPTE_FOURNISSEUR, _nouveau_compte_fournisseur);

		success_update_compte_fournisseur = fournisseursTableModel.updateRecord(0, fournisseursRecord);

		fournisseursTableModel.resetFilter();
	}

	return success_update_compte_fournisseur;
}


double YerothERPPaymentProcessingInformation::montant_paye(int type_de_paiement)
{
	if (YerothUtils::ENCAISSEMENT_COMPTANT 			== _type_de_paiement 	||
		YerothUtils::ENCAISSEMENT_CHEQUE 			== _type_de_paiement 	||
		YerothUtils::ENCAISSEMENT_TELEPHONE 		== _type_de_paiement 	||
		YerothUtils::ENCAISSEMENT_BANCAIRE 			== _type_de_paiement 	||
		YerothUtils::ENCAISSEMENT_VIREMENT_BANCAIRE == _type_de_paiement 	||
		YerothUtils::ENCAISSEMENT_ACHAT_DE_SERVICE_ANNULE == _type_de_paiement)
	{
		return _montant_paye;
	}

	return (-1 * _montant_paye);
}


bool YerothERPPaymentProcessingInformation::save_payment_info_record(bool is_supplier_payment /* = false */)
{
	YerothERPWindows *_allWindows = YerothUtils::getAllWindows();

	if (0 == _allWindows)
	{
		return false;
	}

	bool successPaiementsInsert = true;

	YerothSqlTableModel & paiementsSqlTableModel = _allWindows->getSqlTableModel_paiements();

	QSqlRecord paiementsRecord = paiementsSqlTableModel.record();

	paiementsRecord.setValue(YerothDatabaseTableColumn::DATE_PAIEMENT, GET_CURRENT_DATE);
	paiementsRecord.setValue(YerothDatabaseTableColumn::HEURE_PAIEMENT, CURRENT_TIME);
	paiementsRecord.setValue(YerothDatabaseTableColumn::NOM_ENTREPRISE, _nom_entreprise);

	QString utilisateurCourantNomComplet;

	YerothPOSUser *aUser = _allWindows->getUser();

	if (0 != aUser)
	{
		utilisateurCourantNomComplet = aUser->nom_complet();

		paiementsRecord.setValue(YerothDatabaseTableColumn::NOM_ENCAISSEUR, utilisateurCourantNomComplet);
	}
	else
	{
		paiementsRecord.setValue(YerothDatabaseTableColumn::NOM_ENCAISSEUR, QObject::tr("inconnu(e)"));
	}


	paiementsRecord.setValue(YerothDatabaseTableColumn::TYPE_DE_PAIEMENT, _type_de_paiement);

	paiementsRecord.setValue(YerothDatabaseTableColumn::MONTANT_PAYE, montant_paye(_type_de_paiement));

	paiementsRecord.setValue(YerothDatabaseTableColumn::INTITULE_DU_COMPTE_BANCAIRE,
			_paiement_intitule_compte_bancaire);

	paiementsRecord.setValue(YerothDatabaseTableColumn::NOTES, _NOTES);

	int IDforReceipt = YerothERPWindows::getNextIdSqlTableModel_paiements();

	QString referenceRecuPaiementClient(YerothUtils::GET_REFERENCE_RECU_PAIEMENT_CLIENT(QString::number(IDforReceipt)));

	paiementsRecord.setValue(YerothDatabaseTableColumn::REFERENCE_RECU_PAIEMENT_CLIENT, referenceRecuPaiementClient);

	paiementsRecord.setValue(YerothDatabaseTableColumn::REFERENCE, _reference);

	if (is_supplier_payment)
	{
		successPaiementsInsert = successPaiementsInsert &&
				update_supplier_account(_type_de_paiement, *_allWindows);

		paiementsRecord.setValue(YerothDatabaseTableColumn::COMPTE_FOURNISSEUR, _nouveau_compte_fournisseur);
	}
	else
	{
		paiementsRecord.setValue(YerothDatabaseTableColumn::COMPTE_CLIENT_PROGRAMME_DE_FIDELITE_CLIENTS,
				_nouveau_compteClient_PROGRAMME_DE_FIDELITE_CLIENTS);

		paiementsRecord.setValue(YerothDatabaseTableColumn::COMPTE_CLIENT, _nouveau_compte_client);
	}

	paiementsRecord.setValue(YerothDatabaseTableColumn::ID, IDforReceipt);

	successPaiementsInsert = successPaiementsInsert &&
			paiementsSqlTableModel.insertNewRecord(paiementsRecord);


	return successPaiementsInsert;
}

