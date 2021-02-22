/*
 * yeroth-erp-payment-processing.cpp
 *
 *      Author: DR. XAVIER NOUMBISSI NOUNDOU
 */

#include "src/utils/yeroth-erp-payment-processing.hpp"


#include "src/yeroth-erp-windows.hpp"

#include "src/users/yeroth-erp-users.hpp"


bool YerothERPPaymentProcessing::save_payment_info_record()
{
	YerothERPWindows *_allWindows = YerothUtils::getAllWindows();

	if (0 == _allWindows)
	{
		return false;
	}

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

	//This amount is debited from company account to client account;
	//that is why we negate its value into the payments table.
	paiementsRecord.setValue(YerothDatabaseTableColumn::MONTANT_PAYE, (-1 * _montant_paye));

	paiementsRecord.setValue(YerothDatabaseTableColumn::TYPE_DE_PAIEMENT, _type_de_paiement);


	paiementsRecord.setValue(YerothDatabaseTableColumn::NOTES, _NOTES);

	int IDforReceipt = YerothERPWindows::getNextIdSqlTableModel_paiements();

	QString referenceRecuPaiementClient(YerothUtils::GET_REFERENCE_RECU_PAIEMENT_CLIENT(QString::number(IDforReceipt)));

	paiementsRecord.setValue(YerothDatabaseTableColumn::REFERENCE_RECU_PAIEMENT_CLIENT, referenceRecuPaiementClient);

	paiementsRecord.setValue(YerothDatabaseTableColumn::REFERENCE, _reference);

	paiementsRecord.setValue(YerothDatabaseTableColumn::COMPTE_CLIENT, _nouveau_compte_client);

	paiementsRecord.setValue(YerothDatabaseTableColumn::ID, IDforReceipt);

	bool successPaiementsInsert = paiementsSqlTableModel.insertNewRecord(paiementsRecord);

	return successPaiementsInsert;
}
