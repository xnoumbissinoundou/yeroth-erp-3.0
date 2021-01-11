/*
 * yeroth-erp-details-dun-programme-de-fidelite-clients-window.cpp
 *      Author: DR. XAVIER NOUMBISSI NOUNDOU
 */

#include "yeroth-erp-details-dun-programme-de-fidelite-clients-window.hpp"

#include "src/yeroth-erp-windows.hpp"

#include "src/process/yeroth-erp-process.hpp"

#include "src/users/yeroth-erp-users.hpp"

#include "src/utils/yeroth-erp-sqltable-model.hpp"


#include <QtSql/QSqlRecord>


YerothDetailsDunProgrammeDeFideliteClientsWindow::YerothDetailsDunProgrammeDeFideliteClientsWindow()
:YerothWindowsCommons(),
 _logger(new YerothLogger("YerothDetailsDunProgrammeDeFideliteClientsWindow"))
{
    _windowName = QString("%1 - %2")
    				.arg(YEROTH_ERP_WINDOW_TITLE,
    					 QObject::trUtf8("détails d'un programme de fidélité clients"));

    setupUi(this);

    mySetupUi(this);

    QMESSAGE_BOX_STYLE_SHEET = QString("QMessageBox {background-color: rgb(%1);}"
                                       "QMessageBox QLabel {color: rgb(%2);}")
                            		.arg(COLOUR_RGB_STRING_YEROTH_YELLOW_254_254_0,
                            		     COLOUR_RGB_STRING_YEROTH_BLACK_0_0_0);

    setupLineEdits();

	radioButton_montant_du_rabais->setVisible(false);
	radioButton_pourcentage_du_rabais->setVisible(false);

	checkBox_refereur_details_dun_programme_de_fidelite_clients_pourcentage_rabais->setVisible(false);
	checkBox_details_dun_programme_de_fidelite_clients_rabais_en_cascade_par_le_haut_tous_refereurs->setVisible(false);

	doubleSpinBox_details_dun_programme_de_fidelite_clients_pourcentage_du_rabais->setVisible(false);
	doubleSpinBox_details_dun_programme_de_fidelite_clients_pourcentage_du_rabais_du_refereur->setVisible(false);

	comboBox_details_dun_programme_de_fidelite_clients_pourcentage_rabais_refereur_condition->setVisible(false);

    textEdit_details_dun_programme_de_fidelite_clients_description->setYerothEnabled(false);

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionRETOUR, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);


	pushButton_groupes_de_clients->disable(this);
	pushButton_menu_clients->disable(this);
    pushButton_RETOUR->disable(this);

    /** Menu actions */
    connect(actionChanger_utilisateur, SIGNAL(triggered()), this, SLOT(changer_utilisateur()));
    connect(actionAppeler_aide, SIGNAL(triggered()), this, SLOT(help()));
    connect(actionDeconnecter_utilisateur, SIGNAL(triggered()), this, SLOT(deconnecter_utilisateur()));
    connect(actionMenuClients, SIGNAL(triggered()), this, SLOT(clients()));
    connect(actionRETOUR, SIGNAL(triggered()), this, SLOT(programmes_de_fidelite_clients()));
    connect(actionFermeture, SIGNAL(triggered()), this, SLOT(fermeture()));
    connect(actionA_propos, SIGNAL(triggered()), this, SLOT(apropos()));
    connect(actionInformationEntreprise, SIGNAL(triggered()), this, SLOT(infosEntreprise()));
    connect(actionQui_suis_je, SIGNAL(triggered()), this, SLOT(qui_suis_je()));


#ifdef YEROTH_CLIENT

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);

#else //YEROTH_SERVER, YEROTH_STANDALONE, YEROTH_ACADEMIC_EVALUSATION_VERSION

    connect(actionAdministration, SIGNAL(triggered()), this, SLOT(administration()));

#endif

    setupShortcuts();
}


void YerothDetailsDunProgrammeDeFideliteClientsWindow::modifier_un_programme_de_fidelite_clients()
{

}


void YerothDetailsDunProgrammeDeFideliteClientsWindow::setupLineEdits()
{
	lineEdit_details_dun_programme_de_fidelite_clients_reference->setYerothEnabled(false);
	lineEdit_details_dun_programme_de_fidelite_clients_designation->setYerothEnabled(false);
	lineEdit_details_dun_programme_de_fidelite_clients_montant_du_rabais->setYerothEnabled(false);
	lineEdit_details_dun_programme_de_fidelite_clients_valeur_condition_beneficiaire_du_refereur->setYerothEnabled(false);
}


void YerothDetailsDunProgrammeDeFideliteClientsWindow::definirPasDeRole()
{
    _logger->log("definirPasDeRole");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionRETOUR, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);

	pushButton_groupes_de_clients->disable(this);
	pushButton_menu_clients->disable(this);
    pushButton_RETOUR->disable(this);
}

void YerothDetailsDunProgrammeDeFideliteClientsWindow::definirCaissier()
{
    _logger->log("definirCaissier - definirPasDeRole()");

    definirPasDeRole();
}

void YerothDetailsDunProgrammeDeFideliteClientsWindow::definirManager()
{
    _logger->log("definirManager");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionRETOUR, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

	pushButton_groupes_de_clients->enable(this, SLOT(groupes_de_clients()));
	pushButton_menu_clients->enable(this, SLOT(clients()));
    pushButton_RETOUR->enable(this, SLOT(programmes_de_fidelite_clients()));
}


void YerothDetailsDunProgrammeDeFideliteClientsWindow::definirVendeur()
{
    _logger->log("definirVendeur");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionRETOUR, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

	pushButton_groupes_de_clients->enable(this, SLOT(groupes_de_clients()));
	pushButton_menu_clients->enable(this, SLOT(clients()));
    pushButton_RETOUR->enable(this, SLOT(programmes_de_fidelite_clients()));
}


void YerothDetailsDunProgrammeDeFideliteClientsWindow::definirGestionaireDesStocks()
{
    _logger->log("definirGestionaireDesStocks - definirPasDeRole()");

    definirPasDeRole();
}

void YerothDetailsDunProgrammeDeFideliteClientsWindow::definirMagasinier()
{
    _logger->log("definirMagasinier - definirPasDeRole");

    definirPasDeRole();
}


void YerothDetailsDunProgrammeDeFideliteClientsWindow::rendreInvisible()
{

    YerothWindowsCommons::rendreInvisible();
}


void YerothDetailsDunProgrammeDeFideliteClientsWindow::rendreVisible(YerothSqlTableModel * curClient_ROYALTY_PROGRAM_TableModel,
											  	  	  	  	  	  	 YerothSqlTableModel * stocksTableModel)
{
	_curStocksTableModel = stocksTableModel;

	_curClient_ROYALTY_PROGRAM_TableModel = curClient_ROYALTY_PROGRAM_TableModel;

    //qDebug() << "++ last selected row: " << YerothERPWindows::get_last_lister_selected_row_ID();

	setVisible(true);

    showClientROYALTY_PROGRAM_DETAIL();
}


void YerothDetailsDunProgrammeDeFideliteClientsWindow::showClientROYALTY_PROGRAM_DETAIL()
{
	QSqlRecord record;

	_allWindows->_programmesDeFideliteClientsWindow->
		SQL_QUERY_YEROTH_TABLE_VIEW_LAST_SELECTED_ROW(record);

	lineEdit_details_dun_programme_de_fidelite_clients_reference
		->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::REFERENCE_PROGRAMME_DE_FIDELITE_CLIENTS));

	lineEdit_details_dun_programme_de_fidelite_clients_designation
		->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::DESIGNATION));

	double pourcentage_du_rabais =
			GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::POURCENTAGE_DU_RABAIS).toDouble();


	if (pourcentage_du_rabais > 0)
	{
		radioButton_montant_du_rabais->setChecked(false);

		radioButton_montant_du_rabais->setVisible(false);

		radioButton_pourcentage_du_rabais->setChecked(true);

		radioButton_pourcentage_du_rabais->setVisible(true);

		lineEdit_details_dun_programme_de_fidelite_clients_montant_du_rabais
			->setVisible(false);

		doubleSpinBox_details_dun_programme_de_fidelite_clients_pourcentage_du_rabais
			->setValue(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::POURCENTAGE_DU_RABAIS).toDouble());

		doubleSpinBox_details_dun_programme_de_fidelite_clients_pourcentage_du_rabais
			->setVisible(true);
	}
	else
	{
		radioButton_montant_du_rabais->setChecked(true);

		radioButton_montant_du_rabais->setVisible(true);

		radioButton_pourcentage_du_rabais->setChecked(false);

		radioButton_pourcentage_du_rabais->setVisible(false);

		doubleSpinBox_details_dun_programme_de_fidelite_clients_pourcentage_du_rabais
			->setVisible(false);

		lineEdit_details_dun_programme_de_fidelite_clients_montant_du_rabais
			->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::MONTANT_DU_RABAIS));

		lineEdit_details_dun_programme_de_fidelite_clients_montant_du_rabais->setVisible(true);
	}

	double pourcentage_du_rabais_refereur =
			GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::POURCENTAGE_DU_RABAIS_REFEREUR).toDouble();

	if (pourcentage_du_rabais_refereur <= 0)
	{
		doubleSpinBox_details_dun_programme_de_fidelite_clients_pourcentage_du_rabais_du_refereur
			->setVisible(false);

		comboBox_details_dun_programme_de_fidelite_clients_pourcentage_rabais_refereur_condition
			->setVisible(false);

		lineEdit_details_dun_programme_de_fidelite_clients_valeur_condition_beneficiaire_du_refereur
			->setVisible(false);

		checkBox_refereur_details_dun_programme_de_fidelite_clients_pourcentage_rabais
			->setChecked(false);

		checkBox_refereur_details_dun_programme_de_fidelite_clients_pourcentage_rabais
			->setVisible(false);

		checkBox_details_dun_programme_de_fidelite_clients_rabais_en_cascade_par_le_haut_tous_refereurs
			->setChecked(false);

		checkBox_details_dun_programme_de_fidelite_clients_rabais_en_cascade_par_le_haut_tous_refereurs
			->setVisible(false);
	}
	else
	{

		doubleSpinBox_details_dun_programme_de_fidelite_clients_pourcentage_du_rabais_du_refereur
			->setValue(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::POURCENTAGE_DU_RABAIS_REFEREUR).toDouble());

		doubleSpinBox_details_dun_programme_de_fidelite_clients_pourcentage_du_rabais_du_refereur
			->setVisible(true);


		QDEBUG_STRINGS_OUTPUT_2("YEROTH CONDITION",
				GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::CONDITION_DU_BENEFICE));

		comboBox_details_dun_programme_de_fidelite_clients_pourcentage_rabais_refereur_condition
			->addItem(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::CONDITION_DU_BENEFICE));

		comboBox_details_dun_programme_de_fidelite_clients_pourcentage_rabais_refereur_condition
			->setVisible(true);


		lineEdit_details_dun_programme_de_fidelite_clients_valeur_condition_beneficiaire_du_refereur
			->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::VALEUR_DE_LA_CONDITION_BENEFICIARE));

		lineEdit_details_dun_programme_de_fidelite_clients_valeur_condition_beneficiaire_du_refereur
			->setVisible(true);


		checkBox_refereur_details_dun_programme_de_fidelite_clients_pourcentage_rabais
			->setChecked(true);

		checkBox_refereur_details_dun_programme_de_fidelite_clients_pourcentage_rabais
			->setVisible(true);


		bool rabais_en_cascade_par_le_haut_tous_refereurs =
				GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::RABAIS_EN_CASCADE_TOUS_REFEREUR_PAR_LE_HAUT).toInt();

		if (rabais_en_cascade_par_le_haut_tous_refereurs)
		{
			checkBox_refereur_details_dun_programme_de_fidelite_clients_pourcentage_rabais
				->setChecked(true);

			checkBox_refereur_details_dun_programme_de_fidelite_clients_pourcentage_rabais
				->setVisible(true);

			checkBox_details_dun_programme_de_fidelite_clients_rabais_en_cascade_par_le_haut_tous_refereurs
				->setChecked(true);

			checkBox_details_dun_programme_de_fidelite_clients_rabais_en_cascade_par_le_haut_tous_refereurs
				->setVisible(true);
		}
		else
		{
			checkBox_details_dun_programme_de_fidelite_clients_rabais_en_cascade_par_le_haut_tous_refereurs
				->setChecked(false);

			checkBox_details_dun_programme_de_fidelite_clients_rabais_en_cascade_par_le_haut_tous_refereurs
				->setVisible(false);
		}
	}

	textEdit_details_dun_programme_de_fidelite_clients_description
		->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::DESCRIPTION_PROGRAMME_DE_FIDELITE_CLIENTS));
}


void YerothDetailsDunProgrammeDeFideliteClientsWindow::setupShortcuts()
{
    setupShortcutActionMessageDaide 	(*actionAppeler_aide);
    setupShortcutActionQuiSuisJe		(*actionQui_suis_je);
}

