/*
 * yeroth-erp-info-entreprise.cpp
 *
 *      Author: DR. XAVIER NOUMBISSI NOUNDOU
 */

#include "yeroth-erp-info-entreprise.hpp"

#include "src/yeroth-erp-windows.hpp"

#include "src/include/yeroth-erp-3-0-software.text-configuration.hpp"

#include <QtCore/QFile>
#include <QtCore/QDebug>


bool YerothInfoEntreprise::load_COMPANY_INFO_LOGO_FROM_DB_TABLE_ENTREPRISE_INFO(QLabel *a_temporary_label_for_company_logo /* = 0 */)
{
	QString yerothEntrepriseLogoImageFile_FULL_PATH(QString("%1/%2")
			.arg(YerothERPConfig::temporaryFilesDir,
					"YEROTH-ENTREPRISE-LOGO.JPG"));

	YerothERPConfig::pathToPdfCOMPANY_LOGO = yerothEntrepriseLogoImageFile_FULL_PATH;

	YerothUtils::CREATE_AN_EMPTY_FILE(YerothERPConfig::pathToPdfCOMPANY_LOGO);


	YerothERPWindows *allWindows = YerothUtils::getAllWindows();

	if (0 == allWindows)
	{
		return false;
	}


	YerothSqlTableModel &entreprise_info_TableModel =
			allWindows->getSqlTableModel_entreprise_info();

	//row 0 of table corresponds to initialization company information.
	//row 1 of table corresponds to actual company information.
	unsigned int ROW_ONE_ACTUAL_COMPANY_INFORMATION = 1;


	QSqlRecord entrepriseInfoRecord =
			entreprise_info_TableModel.record(ROW_ONE_ACTUAL_COMPANY_INFORMATION);


	QVariant img(entrepriseInfoRecord.value(YerothDatabaseTableColumn::LOGO_ENTREPRISE));


	if (img.isNull())
	{
		return false;
	}

	bool _apointer__CREATED_HERE = false;

	if (0 == a_temporary_label_for_company_logo)
	{
		_apointer__CREATED_HERE = true;

		a_temporary_label_for_company_logo = new QLabel;
	}

	YerothUtils::loadPixmapFromDB(*a_temporary_label_for_company_logo, img, "JPG");

	if (0 != a_temporary_label_for_company_logo->pixmap())
	{
		YerothUtils::savePixmapToFile(yerothEntrepriseLogoImageFile_FULL_PATH,
									  *(a_temporary_label_for_company_logo->pixmap()),
									  "JPG");
	}

	if (_apointer__CREATED_HERE)
	{
		YEROTH_DELETE_FREE_POINTER_NOW(a_temporary_label_for_company_logo)
	}

	return true;
}


void YerothInfoEntreprise::updateInfoEntrepriseFromDB()
{
	YerothERPWindows *allWindows = YerothUtils::getAllWindows();

	if (0 == allWindows)
	{
		return ;
	}

	YerothSqlTableModel &entreprise_info_TableModel =
			allWindows->getSqlTableModel_entreprise_info();

	//row 0 of table corresponds to initialization company information.
	//row 1 of table corresponds to actual company information.
	unsigned int ROW_ONE_ACTUAL_COMPANY_INFORMATION = 1;

	QSqlRecord entrepriseInfoRecord = entreprise_info_TableModel.record(ROW_ONE_ACTUAL_COMPANY_INFORMATION);

	_nom_commercial 			= QString( GET_SQL_RECORD_DATA(entrepriseInfoRecord, 1) );
	_siege_social 				= QString( GET_SQL_RECORD_DATA(entrepriseInfoRecord, 2) );
	_localisation 				= QString( GET_SQL_RECORD_DATA(entrepriseInfoRecord, 3) );
	_boite_postale 				= QString( GET_SQL_RECORD_DATA(entrepriseInfoRecord, 4) );
	_adresse 					= QString( GET_SQL_RECORD_DATA(entrepriseInfoRecord, 5) );
	_ville 						= QString( GET_SQL_RECORD_DATA(entrepriseInfoRecord, 6) );
	_pays 						= QString( GET_SQL_RECORD_DATA(entrepriseInfoRecord, 7) );
	_email 						= QString( GET_SQL_RECORD_DATA(entrepriseInfoRecord, 8) );
	_telephone 					= QString( GET_SQL_RECORD_DATA(entrepriseInfoRecord, 9) );
	_numero_contribuable 		= QString( GET_SQL_RECORD_DATA(entrepriseInfoRecord, 10) );
	_secteurs_activites 		= QString( GET_SQL_RECORD_DATA(entrepriseInfoRecord, 11) );
	_agence_comptebancaire 		= QString( GET_SQL_RECORD_DATA(entrepriseInfoRecord, 12) );
	_reference_comptebancaire	= QString( GET_SQL_RECORD_DATA(entrepriseInfoRecord, 13) );

	_reference_registre_chambre_du_commerce
		= QString( GET_SQL_RECORD_DATA(entrepriseInfoRecord, 14) );


	_CARTE_DE_FIDELITE_nom_entreprise =
			QString( GET_SQL_RECORD_DATA(entrepriseInfoRecord,
					YerothDatabaseTableColumn::CARTE_DE_FIDELITE_CLIENT_nom_entreprise ));

	_CARTE_DE_FIDELITE_siege =
			QString( GET_SQL_RECORD_DATA(entrepriseInfoRecord,
					YerothDatabaseTableColumn::CARTE_DE_FIDELITE_CLIENT_siege ));

	_CARTE_DE_FIDELITE_service_de_gestion_clientele =
			QString( GET_SQL_RECORD_DATA(entrepriseInfoRecord,
					YerothDatabaseTableColumn::CARTE_DE_FIDELITE_CLIENT_service_de_gestion_clientele ));

	_CARTE_DE_FIDELITE_service_du_programme_de_fidelite_clients =
			QString( GET_SQL_RECORD_DATA(entrepriseInfoRecord,
					YerothDatabaseTableColumn::CARTE_DE_FIDELITE_CLIENT_service_du_programme_de_fidelite_client ));

	_CARTE_DE_FIDELITE_email =
			QString( GET_SQL_RECORD_DATA(entrepriseInfoRecord,
					YerothDatabaseTableColumn::CARTE_DE_FIDELITE_CLIENT_email ));

	_CARTE_DE_FIDELITE_telephone =
			QString( GET_SQL_RECORD_DATA(entrepriseInfoRecord,
					YerothDatabaseTableColumn::CARTE_DE_FIDELITE_CLIENT_telephone ));


	YerothInfoEntreprise::load_COMPANY_INFO_LOGO_FROM_DB_TABLE_ENTREPRISE_INFO();
}

QString YerothInfoEntreprise::toString()
{
	updateInfoEntrepriseFromDB();

    QString result;

#ifdef YEROTH_FRANCAIS_LANGUAGE
    result.append("1) Dénomination de l'entreprise: ").append(getNomCommercial()).append("\n")
    .append("2) Siège social: ").append(getSiegeSocial()).append("\n")
    .append("3) Localisation: ").append(getLocalisation()).append("\n")
    .append("4) Boîte postale: ").append(getBoitePostal()).append("\n")
    .append("5) Adresse: ").append(getAdresse()).append("\n")
    .append("6) Ville: ").append(getVille()).append("\n")
    .append("7) Pays: ").append(getPays()).append("\n")
    .append("8) Émail: ").append(getEmail()).append("\n")
    .append("9) Téléphone: ").append(getTelephone()).append("\n")
	.append("10) RCCM N.: ").append(getReferenceRegistreDuCommerce()).append("\n")
	.append("11) Numéro de contribuable: ").append(getNumeroDeContribuable()).append("\n")
    .append("12) Secteurs d'activités: ").append(getSecteursActivites()).append("\n");
#endif

#ifdef YEROTH_ENGLISH_LANGUAGE
    result.append("1) Name of the company: ").append(getNomCommercial()).append("\n")
    .append("2) Headquarters: ").append(getSiegeSocial()).append("\n")
    .append("3) Site: ").append(getLocalisation()).append("\n")
    .append("4) P.O Box: ").append(getBoitePostal()).append("\n")
    .append("5) Address: ").append(getAdresse()).append("\n")
    .append("6) City: ").append(getVille()).append("\n")
    .append("7) Country: ").append(getPays()).append("\n")
    .append("8) Email: ").append(getEmail()).append("\n")
    .append("9) Phone: ").append(getTelephone()).append("\n")
	.append("10) RCCM N.: ").append(getReferenceRegistreDuCommerce()).append("\n")
	.append("11) Company tax registration number: ").append(getNumeroDeContribuable()).append("\n")
    .append("12) Commercial activities: ").append(getSecteursActivites()).append("\n");
#endif

    return result;
}


QString YerothInfoEntreprise::getNomCommercial_truncated_FOR_SMALL_RECEIPT()
{
    QString NomCommercial_truncated(getNomCommercial().trimmed());

    if (NomCommercial_truncated.length() > 31)
    {
        NomCommercial_truncated.truncate(31);
        NomCommercial_truncated.append(".");
    }

	return NomCommercial_truncated;
}

