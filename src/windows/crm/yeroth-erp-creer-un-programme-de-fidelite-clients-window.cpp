/*
 * yeroth-erp-creer-un-programme-de-fidelite-clients-window.cpp
 *
 *      Author: DR. XAVIER NOUMBISSI NOUNDOU
 */

#include "yeroth-erp-creer-un-programme-de-fidelite-clients-window.hpp"

#include "src/yeroth-erp-windows.hpp"

#include "src/utils/yeroth-erp-database-table-column.hpp"

#include "src/users/yeroth-erp-users.hpp"

#include "src/utils/yeroth-erp-config.hpp"

#include <QtSql/QSqlQuery>

#include <QtSql/QSqlIndex>

#include "src/utils/yeroth-erp-database-table-column.hpp"

#include "src/utils/yeroth-erp-historique-stock.hpp"


YerothCreerUnProgrammeDeFideliteClientsWindow::YerothCreerUnProgrammeDeFideliteClientsWindow()
:YerothWindowsCommons(),
 _logger(new YerothLogger("YerothCreerUnProgrammeDeFideliteClientsWindow"))
{
    _windowName = QString("%1 - %2")
    				.arg(YEROTH_ERP_WINDOW_TITLE,
    					 QObject::trUtf8("créer un programme de fidélité clients"));

    setupUi(this);

    mySetupUi(this);

    QMESSAGE_BOX_STYLE_SHEET = QString("QMessageBox {background-color: rgb(%1);}"
                                       "QMessageBox QLabel {color: rgb(%2);}")
                                    .arg(COLOUR_RGB_STRING_YEROTH_YELLOW_254_254_0,
                                    	 COLOUR_RGB_STRING_YEROTH_BLACK_0_0_0);


    _NOT_VISIBLE_FOR_USER_DB_TABLE_COLUMN_NAME
		<< YerothDatabaseTableColumn::CONDITION_DU_BENEFICE
		<< YerothDatabaseTableColumn::RABAIS_EN_CASCADE_TOUS_REFEREUR_PAR_LE_HAUT;


    setup_select_configure_dbcolumn(_allWindows->PROGRAMMES_DE_FIDELITE_CLIENTS);

    populateComboBoxes();

    setupLineEdits();


    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionInformationEntreprise, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionA_propos, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_principal, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionClients, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAppeler_aide, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAnnuler, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEnregistrer, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionFermeture, false);

    pushButton_achats_aux_fournisseurs->disable(this);
	pushButton_annuler->disable(this);
	pushButton_menu_principal->disable(this);
	pushButton_clients->disable(this);
	pushButton_enregistrer->disable(this);


    /** Menu actions */
    connect(actionChanger_utilisateur, SIGNAL(triggered()), this, SLOT(changer_utilisateur()));
    connect(actionAppeler_aide, SIGNAL(triggered()), this, SLOT(help()));
    connect(actionDeconnecter_utilisateur, SIGNAL(triggered()), this, SLOT(deconnecter_utilisateur()));
    connect(actionMenu_principal, SIGNAL(triggered()), this, SLOT(menu()));
    connect(actionClients, SIGNAL(triggered()), this, SLOT(clients()));
    connect(actionAnnuler, SIGNAL(triggered()), this, SLOT(annuler_la_creation_dun_programme_de_fidelite_de_clients()));
    connect(actionEnregistrer, SIGNAL(triggered()), this, SLOT(creerEnregistrerUnProgrammeDeFideliteClients()));
    connect(actionFermeture, SIGNAL(triggered()), this, SLOT(fermeture()));
    connect(actionA_propos, SIGNAL(triggered()), this, SLOT(apropos()));
    connect(actionInformationEntreprise, SIGNAL(triggered()), this, SLOT(infosEntreprise()));
    connect(actionQui_suis_je, SIGNAL(triggered()), this, SLOT(qui_suis_je()));
    connect(actionAdministration, SIGNAL(triggered()), this, SLOT(administration()));


    connect(radioButton_montant_du_rabais,
    		SIGNAL(toggled(bool)),
			this,
            SLOT(activateLineEdit_MONTANT_DU_RABAIS(bool)));


    connect(radioButton_pourcentage_du_rabais,
    		SIGNAL(toggled(bool)),
			this,
            SLOT(activateLineEdit_POURCENTAGE_DU_RABAIS(bool)));



#ifdef YEROTH_CLIENT
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    setupShortcuts();

    radioButton_pourcentage_du_rabais->setChecked(true);

    lineEdit_creer_un_programme_de_fidelite_clients_designation->setFocus();
}

void YerothCreerUnProgrammeDeFideliteClientsWindow::deconnecter_utilisateur()
{
    clear_all_fields();
    YerothWindowsCommons::deconnecter_utilisateur();
}


void YerothCreerUnProgrammeDeFideliteClientsWindow::populateComboBoxes()
{
	QStringList aQStringList;

	aQStringList.append(">=");

	aQStringList.append("<=");

	aQStringList.append(">");

	aQStringList.append("<");

	aQStringList.append("=");

	comboBox_creer_un_programme_de_fidelite_clients_pourcentage_rabais_refereur_condition
		->addItems(aQStringList);
}


void YerothCreerUnProgrammeDeFideliteClientsWindow::setupLineEdits()
{
}


void YerothCreerUnProgrammeDeFideliteClientsWindow::setupShortcuts()
{
    setupShortcutActionMessageDaide 	(*actionAppeler_aide);
    setupShortcutActionQuiSuisJe		(*actionQui_suis_je);
}


void YerothCreerUnProgrammeDeFideliteClientsWindow::definirPasDeRole()
{
    _logger->log("definirPasDeRole");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionInformationEntreprise, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionA_propos, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_principal, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionClients, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAppeler_aide, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAnnuler, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEnregistrer, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionFermeture, false);

    pushButton_achats_aux_fournisseurs->disable(this);
	pushButton_annuler->disable(this);
	pushButton_menu_principal->disable(this);
	pushButton_clients->disable(this);
	pushButton_enregistrer->disable(this);

}


void YerothCreerUnProgrammeDeFideliteClientsWindow::definirCaissier()
{
    _logger->log("definirCaissier - definirPasDeRole()");

   definirPasDeRole();
}

void YerothCreerUnProgrammeDeFideliteClientsWindow::definirManager()
{
    _logger->log("definirManager");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionInformationEntreprise, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionA_propos, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_principal, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionClients, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAppeler_aide, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAnnuler, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEnregistrer, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionFermeture, true);


    pushButton_achats_aux_fournisseurs->enable(this, SLOT(achats_aux_fournisseurs()));
	pushButton_annuler->enable(this, SLOT(annuler_la_creation_dun_programme_de_fidelite_de_clients()));
	pushButton_menu_principal->enable(this, SLOT(menu()));
	pushButton_clients->enable(this, SLOT(clients()));
	pushButton_enregistrer->enable(this, SLOT(creerEnregistrerUnProgrammeDeFideliteClients()));
}


void YerothCreerUnProgrammeDeFideliteClientsWindow::definirVendeur()
{
    _logger->log("definirVendeur");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionInformationEntreprise, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionA_propos, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_principal, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionClients, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAppeler_aide, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAnnuler, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEnregistrer, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionFermeture, true);


    pushButton_achats_aux_fournisseurs->enable(this, SLOT(achats_aux_fournisseurs()));
	pushButton_annuler->enable(this, SLOT(annuler_la_creation_dun_programme_de_fidelite_de_clients()));
	pushButton_menu_principal->enable(this, SLOT(menu()));
	pushButton_clients->enable(this, SLOT(clients()));
	pushButton_enregistrer->enable(this, SLOT(creerEnregistrerUnProgrammeDeFideliteClients()));
}


void YerothCreerUnProgrammeDeFideliteClientsWindow::definirGestionaireDesStocks()
{
    _logger->log("definirGestionaireDesStocks - definirPasDeRole()");

    definirPasDeRole();
}


void YerothCreerUnProgrammeDeFideliteClientsWindow::definirMagasinier()
{
    _logger->log("definirMagasinier - definirPasDeRole()");

    definirPasDeRole();
}


bool YerothCreerUnProgrammeDeFideliteClientsWindow::royaltyClientProgramAlreadyExists()
{
	bool result = false;

	YerothSqlTableModel &programmes_de_fidelite_clients_TableModel =
			_allWindows->getSqlTableModel_programmes_de_fidelite_clients();

	// ** check if customer account with same name exist
	result =
		YerothUtils::checkIf_KEYWORD_ALREADY_EXISTS(*this,
													programmes_de_fidelite_clients_TableModel,
													*lineEdit_creer_un_programme_de_fidelite_clients_designation,
													QObject::trUtf8("programme de fidélité clients"),
													YerothDatabaseTableColumn::DESIGNATION);

	return result;
}


bool YerothCreerUnProgrammeDeFideliteClientsWindow::check_fields()
{
    bool result = false;

    bool programme_de_fidelite_clients_designation =
    		lineEdit_creer_un_programme_de_fidelite_clients_designation->checkField();

    result = programme_de_fidelite_clients_designation;

    return result;
}


void YerothCreerUnProgrammeDeFideliteClientsWindow::clear_all_fields()
{
	lineEdit_creer_un_programme_de_fidelite_clients_reference->clear();

	lineEdit_creer_un_programme_de_fidelite_clients_designation->clearField();

	lineEdit_creer_un_programme_de_fidelite_clients_valeur_condition_beneficiaire_du_refereur->clear();

	lineEdit_creer_un_programme_de_fidelite_clients_montant_du_rabais->clear();

	doubleSpinBox_creer_un_programme_de_fidelite_clients_pourcentage_du_rabais->setValue(0.0);

	doubleSpinBox_creer_un_programme_de_fidelite_clients_pourcentage_du_rabais_du_refereur->setValue(0.0);

	comboBox_creer_un_programme_de_fidelite_clients_pourcentage_rabais_refereur_condition
		->resetYerothComboBox();

	checkBox_refereur_creer_un_programme_de_fidelite_clients_pourcentage_rabais->setChecked(false);

	checkBox_creer_un_programme_de_fidelite_clients_rabais_en_cascade_par_le_haut_tous_refereurs
		->setChecked(false);
}


void YerothCreerUnProgrammeDeFideliteClientsWindow::rendreInvisible()
{
    setVisible(false);
}


void YerothCreerUnProgrammeDeFideliteClientsWindow::rendreVisible(YerothSqlTableModel * stocksTableModel)
{
    _curStocksTableModel = stocksTableModel;

    check_fields();

    setVisible(true);
}


void YerothCreerUnProgrammeDeFideliteClientsWindow::annuler_la_creation_dun_programme_de_fidelite_de_clients()
{
	clear_all_fields();

	clients();
}


void YerothCreerUnProgrammeDeFideliteClientsWindow::activateLineEdit_MONTANT_DU_RABAIS(bool toggled)
{
    if (toggled)
    {
    	lineEdit_creer_un_programme_de_fidelite_clients_montant_du_rabais->setEnabled(true);
    }
    else
    {
    	lineEdit_creer_un_programme_de_fidelite_clients_montant_du_rabais->setEnabled(false);
    }
}


void YerothCreerUnProgrammeDeFideliteClientsWindow::activateLineEdit_POURCENTAGE_DU_RABAIS(bool toggled)
{
    if (toggled)
    {
    	doubleSpinBox_creer_un_programme_de_fidelite_clients_pourcentage_du_rabais->setEnabled(true);
    }
    else
    {
    	doubleSpinBox_creer_un_programme_de_fidelite_clients_pourcentage_du_rabais->setEnabled(false);
    }
}


bool YerothCreerUnProgrammeDeFideliteClientsWindow::creerEnregistrerUnProgrammeDeFideliteClients()
{
	if (!check_fields())
	{
		if (QMessageBox::Ok ==
				YerothQMessageBox::warning(this, "",
										   QObject::tr("Remplisser tous les champs obligatoires !")))
		{
		}
		else
		{
		}

		return false;
	}

	if (royaltyClientProgramAlreadyExists())
	{
		return false;
	}

	QString retMsg(QObject::trUtf8("Le programme de fidélité de clients '"));

	YerothSqlTableModel &clientRoyaltyProgramTableModel = _allWindows->getSqlTableModel_programmes_de_fidelite_clients();


	QSqlRecord record = clientRoyaltyProgramTableModel.record();


	QString new_royalty_program_designation = lineEdit_creer_un_programme_de_fidelite_clients_designation->text();

	int new_client_ROYALTY_PROGRAM_db_ID = YerothERPWindows::getNextIdSqlTableModel_programme_de_fidelite_clients();


	record.setValue(YerothDatabaseTableColumn::ID, new_client_ROYALTY_PROGRAM_db_ID);

	record.setValue(YerothDatabaseTableColumn::DATE_CREATION,
			DATE_TO_DB_FORMAT_STRING(GET_CURRENT_DATE));

	record.setValue(YerothDatabaseTableColumn::REFERENCE_PROGRAMME_DE_FIDELITE_CLIENTS,
			lineEdit_creer_un_programme_de_fidelite_clients_reference->text());

	record.setValue(YerothDatabaseTableColumn::DESIGNATION, new_royalty_program_designation);

	record.setValue(YerothDatabaseTableColumn::DESCRIPTION_PROGRAMME_DE_FIDELITE_CLIENTS,
			textEdit_creer_un_programme_de_fidelite_clients_description->toPlainText());

	if (radioButton_montant_du_rabais->isChecked())
	{
		record.setValue(YerothDatabaseTableColumn::MONTANT_DU_RABAIS,
				lineEdit_creer_un_programme_de_fidelite_clients_montant_du_rabais->text().toDouble());
	}
	else
	{
		record.setValue(YerothDatabaseTableColumn::POURCENTAGE_DU_RABAIS,
				doubleSpinBox_creer_un_programme_de_fidelite_clients_pourcentage_du_rabais->value());
	}


	if (checkBox_refereur_creer_un_programme_de_fidelite_clients_pourcentage_rabais->isChecked())
	{
		record.setValue(YerothDatabaseTableColumn::CONDITION_DU_BENEFICE,
				comboBox_creer_un_programme_de_fidelite_clients_pourcentage_rabais_refereur_condition->currentText());

		record.setValue(YerothDatabaseTableColumn::VALEUR_DE_LA_CONDITION_BENEFICIARE,
				lineEdit_creer_un_programme_de_fidelite_clients_valeur_condition_beneficiaire_du_refereur->text());

		record.setValue(YerothDatabaseTableColumn::POURCENTAGE_DU_RABAIS_REFEREUR,
				doubleSpinBox_creer_un_programme_de_fidelite_clients_pourcentage_du_rabais_du_refereur->value());

		if (checkBox_creer_un_programme_de_fidelite_clients_rabais_en_cascade_par_le_haut_tous_refereurs->isChecked())
		{
			record.setValue(YerothDatabaseTableColumn::RABAIS_EN_CASCADE_TOUS_REFEREUR_PAR_LE_HAUT, true);
		}
		else
		{
			record.setValue(YerothDatabaseTableColumn::RABAIS_EN_CASCADE_TOUS_REFEREUR_PAR_LE_HAUT, false);
		}
	}


	retMsg.append(lineEdit_creer_un_programme_de_fidelite_clients_designation->text());

	bool success = clientRoyaltyProgramTableModel.insertNewRecord(record);

	if (!success)
	{
		retMsg.append(QObject::trUtf8("' n'a pas pu être créer !"));

		YerothQMessageBox::warning(this,
				QObject::trUtf8("créer 1 programme de fidélité clients"),
				retMsg);
		return false;
	}

	clientRoyaltyProgramTableModel.select();

	retMsg.append(QObject::trUtf8("' a été créer avec succès !"));

	YerothQMessageBox::information(this,
			QObject::trUtf8("créer 1 programme de fidélité clients"),
			retMsg);

	clear_all_fields();

	clients();

    return true;
}


