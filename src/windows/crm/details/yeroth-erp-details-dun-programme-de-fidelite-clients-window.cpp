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

	radioButton_montant_du_rabais->setEnabled(false);
	radioButton_pourcentage_du_rabais->setEnabled(false);

	checkBox_refereur_details_dun_programme_de_fidelite_clients_pourcentage_rabais->setEnabled(false);
	checkBox_details_dun_programme_de_fidelite_clients_rabais_en_cascade_par_le_haut_tous_refereurs->setEnabled(false);

	doubleSpinBox_details_dun_programme_de_fidelite_clients_pourcentage_du_rabais->setEnabled(false);
	doubleSpinBox_details_dun_programme_de_fidelite_clients_pourcentage_du_rabais_du_refereur->setEnabled(false);

	comboBox_details_dun_programme_de_fidelite_clients_pourcentage_rabais_refereur_condition->setYerothEnabled(false);

    textEdit_details_dun_programme_de_fidelite_clients_description->setYerothEnabled(false);

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionClients, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);


    pushButton_achats_aux_fournisseurs->disable(this);
	pushButton_annuler->disable(this);
	pushButton_menu_principal->disable(this);
	pushButton_clients->disable(this);
	pushButton_modifier->disable(this);


    /** Menu actions */
    connect(actionChanger_utilisateur, SIGNAL(triggered()), this, SLOT(changer_utilisateur()));
    connect(actionAppeler_aide, SIGNAL(triggered()), this, SLOT(help()));
    connect(actionDeconnecter_utilisateur, SIGNAL(triggered()), this, SLOT(deconnecter_utilisateur()));
    connect(actionMenu_principal, SIGNAL(triggered()), this, SLOT(menu()));
    connect(actionClients, SIGNAL(triggered()), this, SLOT(clients()));
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
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionClients, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);

    pushButton_achats_aux_fournisseurs->disable(this);
	pushButton_annuler->disable(this);
	pushButton_menu_principal->disable(this);
	pushButton_clients->disable(this);
	pushButton_modifier->disable(this);
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
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionClients, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    pushButton_achats_aux_fournisseurs->enable(this, SLOT(achats_aux_fournisseurs()));
	pushButton_annuler->enable(this, SLOT(groupes_de_clients()));
	pushButton_menu_principal->enable(this, SLOT(menu()));
	pushButton_clients->enable(this, SLOT(clients()));
	pushButton_modifier->enable(this, SLOT(modifier_un_programme_de_fidelite_clients()));
}


void YerothDetailsDunProgrammeDeFideliteClientsWindow::definirVendeur()
{
    _logger->log("definirVendeur");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionClients, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    pushButton_achats_aux_fournisseurs->enable(this, SLOT(achats_aux_fournisseurs()));
	pushButton_annuler->enable(this, SLOT(groupes_de_clients()));
	pushButton_menu_principal->enable(this, SLOT(menu()));
	pushButton_clients->enable(this, SLOT(clients()));
	pushButton_modifier->enable(this, SLOT(modifier_un_programme_de_fidelite_clients()));
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

	_allWindows->_groupesDeClientsWindow->
		SQL_QUERY_YEROTH_TABLE_VIEW_LAST_SELECTED_ROW(record);

	lineEdit_details_dun_programme_de_fidelite_clients_reference
		->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::REFERENCE_GROUPE));

	QDEBUG_STRINGS_OUTPUT_2("designation",
			GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::DESIGNATION));

	lineEdit_details_dun_programme_de_fidelite_clients_designation
		->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::DESIGNATION));


	double pourcentage_du_rabais =
			GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::POURCENTAGE_DU_RABAIS).toDouble();


	if (pourcentage_du_rabais > 0)
	{
		radioButton_montant_du_rabais->setChecked(false);

		radioButton_montant_du_rabais->setEnabled(false);

		radioButton_pourcentage_du_rabais->setChecked(true);

		radioButton_pourcentage_du_rabais->setEnabled(true);

		doubleSpinBox_details_dun_programme_de_fidelite_clients_pourcentage_du_rabais
			->setValue(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::POURCENTAGE_DU_RABAIS).toDouble());
	}
	else
	{
		radioButton_montant_du_rabais->setChecked(true);

		radioButton_montant_du_rabais->setEnabled(true);

		radioButton_pourcentage_du_rabais->setChecked(false);

		radioButton_pourcentage_du_rabais->setEnabled(false);

		lineEdit_details_dun_programme_de_fidelite_clients_montant_du_rabais
			->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::MONTANT_DU_RABAIS));
	}


	doubleSpinBox_details_dun_programme_de_fidelite_clients_pourcentage_du_rabais_du_refereur
		->setValue(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::POURCENTAGE_DU_RABAIS_REFEREUR).toDouble());

	comboBox_details_dun_programme_de_fidelite_clients_pourcentage_rabais_refereur_condition
			->setItemText(0, GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::CONDITION_DU_BENEFICE));


	bool rabais_en_cascade_par_le_haut_tous_refereurs =
			GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::RABAIS_EN_CASCADE_TOUS_REFEREUR_PAR_LE_HAUT).toInt();

	checkBox_refereur_details_dun_programme_de_fidelite_clients_pourcentage_rabais
		->setChecked(rabais_en_cascade_par_le_haut_tous_refereurs);

	checkBox_details_dun_programme_de_fidelite_clients_rabais_en_cascade_par_le_haut_tous_refereurs
		->setChecked(rabais_en_cascade_par_le_haut_tous_refereurs);
}


void YerothDetailsDunProgrammeDeFideliteClientsWindow::setupShortcuts()
{
    setupShortcutActionMessageDaide 	(*actionAppeler_aide);
    setupShortcutActionQuiSuisJe		(*actionQui_suis_je);
}

