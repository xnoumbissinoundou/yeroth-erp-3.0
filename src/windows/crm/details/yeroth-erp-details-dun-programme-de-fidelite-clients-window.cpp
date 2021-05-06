/*
 * yeroth-erp-details-dun-programme-de-fidelite-clients-window.cpp
 *
 *      Author: PR. XAVIER NOUMBISSI NOUNDOU
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

    doubleSpinBox_details_dun_programme_de_fidelite_clients_pourcentage_du_rabais->setEnabled(false);

    setupLineEdits();

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


void YerothDetailsDunProgrammeDeFideliteClientsWindow::setupLineEdits()
{
	lineEdit_details_dun_programme_de_fidelite_clients_reference->setYerothEnabled(false);
	lineEdit_details_dun_programme_de_fidelite_clients_designation->setYerothEnabled(false);
}


void YerothDetailsDunProgrammeDeFideliteClientsWindow::definirPasDeRole()
{
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionRETOUR, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);

	pushButton_groupes_de_clients->disable(this);
	pushButton_menu_clients->disable(this);
    pushButton_RETOUR->disable(this);
}


void YerothDetailsDunProgrammeDeFideliteClientsWindow::definirManager()
{
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionRETOUR, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

	pushButton_groupes_de_clients->enable(this, SLOT(groupes_de_clients()));
	pushButton_menu_clients->enable(this, SLOT(clients()));
    pushButton_RETOUR->enable(this, SLOT(programmes_de_fidelite_clients()));
}


void YerothDetailsDunProgrammeDeFideliteClientsWindow::rendreInvisible()
{
	comboBox_details_un_programme_de_fidelite_clients_localisation->clear();
    YerothWindowsCommons::rendreInvisible();
}


void YerothDetailsDunProgrammeDeFideliteClientsWindow::rendreVisible(YerothSqlTableModel * curClient_LOYALTY_PROGRAM_TableModel,
											  	  	  	  	  	  	 YerothSqlTableModel * stocksTableModel)
{
	_curStocksTableModel = stocksTableModel;

	_curClient_LOYALTY_PROGRAM_TableModel = curClient_LOYALTY_PROGRAM_TableModel;

    //qDebug() << "++ last selected row: " << YerothERPWindows::get_last_lister_selected_row_ID();

	setVisible(true);

    showClientLOYALTY_PROGRAM_DETAIL();
}


void YerothDetailsDunProgrammeDeFideliteClientsWindow::showClientLOYALTY_PROGRAM_DETAIL()
{
	QSqlRecord record;

	QString designation_programme_de_fidelite_clients;

	_allWindows->_programmesDeFideliteClientsWindow->
		SQL_QUERY_YEROTH_TABLE_VIEW_LAST_SELECTED_ROW(record);

	lineEdit_details_dun_programme_de_fidelite_clients_reference
		->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::REFERENCE_PROGRAMME_DE_FIDELITE_CLIENTS));

	designation_programme_de_fidelite_clients =
			GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::DESIGNATION);

	lineEdit_details_dun_programme_de_fidelite_clients_designation
		->setText(designation_programme_de_fidelite_clients);

	comboBox_details_un_programme_de_fidelite_clients_localisation
		->addItem(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::LOCALISATION));


	doubleSpinBox_details_dun_programme_de_fidelite_clients_pourcentage_du_rabais
		->setValue(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::POURCENTAGE_DU_RABAIS).toDouble());


	textEdit_details_dun_programme_de_fidelite_clients_description
		->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::DESCRIPTION_PROGRAMME_DE_FIDELITE_CLIENTS));

	tableWidget_details_dun_programe_de_fidelite_clients_groupes_de_clients_participants
		->yerothClearTableWidgetContent();

	QString SELECT_ALL_CLIENT_GROUPS_PARTICIPATING(
			QString("select %1, %2, %3, %4 from %5 where %6='%7'")
				.arg(YerothDatabaseTableColumn::ID,
					 YerothDatabaseTableColumn::DESIGNATION,
					 YerothDatabaseTableColumn::REFERENCE_GROUPE,
					 YerothDatabaseTableColumn::MEMBRES_DU_GROUPE_db_ID,
					 YerothDatabase::GROUPES_DE_CLIENTS,
					 YerothDatabaseTableColumn::PROGRAMME_DE_FIDELITE_CLIENTS,
					 designation_programme_de_fidelite_clients));


	QSqlQuery aQSqlQuery;

	int query_size = YerothUtils::execQuery(aQSqlQuery, SELECT_ALL_CLIENT_GROUPS_PARTICIPATING);

	if (query_size <= 0)
	{
		return ;
	}

	QString client_group_db_ID;
	QString designation;
	QString reference_groupe;
	YerothQStringList client_group_member_db_ID;

	while (aQSqlQuery.next())
	{
		client_group_db_ID = aQSqlQuery.value(YerothDatabaseTableColumn::ID).toString();

		designation = aQSqlQuery.value(YerothDatabaseTableColumn::DESIGNATION).toString();

		reference_groupe = aQSqlQuery.value(YerothDatabaseTableColumn::REFERENCE_GROUPE).toString();

		client_group_member_db_ID.clear();

		YerothUtils::SPLIT_STAR_SEPARATED_DB_STRING(
								client_group_member_db_ID,
								aQSqlQuery.value(YerothDatabaseTableColumn::MEMBRES_DU_GROUPE_db_ID).toString());

		tableWidget_details_dun_programe_de_fidelite_clients_groupes_de_clients_participants
			->insert_group(client_group_db_ID,
						   designation,
						   reference_groupe,
						   QString::number(client_group_member_db_ID.size()));
	}
}


void YerothDetailsDunProgrammeDeFideliteClientsWindow::setupShortcuts()
{
    setupShortcutActionMessageDaide 	(*actionAppeler_aide);
    setupShortcutActionQuiSuisJe		(*actionQui_suis_je);
}
