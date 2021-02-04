/*
 * yeroth-erp-modifier-groupe-de-clients-window.cpp
 *
 *      Author: DR. XAVIER NOUMBISSI NOUNDOU
 */

#include "yeroth-erp-modifier-groupe-de-clients-window.hpp"

#include "src/yeroth-erp-windows.hpp"

#include "src/utils/yeroth-erp-database-table-column.hpp"

#include "src/users/yeroth-erp-users.hpp"

#include "src/utils/yeroth-erp-config.hpp"

#include <QtSql/QSqlQuery>

#include <QtSql/QSqlIndex>

#include "src/utils/yeroth-erp-database-table-column.hpp"

#include "src/utils/yeroth-erp-historique-stock.hpp"


YerothModifierGroupeDeClientsWindow::YerothModifierGroupeDeClientsWindow()
:YerothWindowsCommons(),
 _logger(new YerothLogger("YerothModifierGroupeDeClientsWindow"))
{
    _windowName = QString("%1 - %2")
    				.arg(YEROTH_ERP_WINDOW_TITLE,
    					 QObject::trUtf8("créer un groupe de clients"));

    setupUi(this);

    mySetupUi(this);

    QMESSAGE_BOX_STYLE_SHEET = QString("QMessageBox {background-color: rgb(%1);}"
                                       "QMessageBox QLabel {color: rgb(%2);}")
                                    .arg(COLOUR_RGB_STRING_YEROTH_YELLOW_254_254_0,
                                    	 COLOUR_RGB_STRING_YEROTH_BLACK_0_0_0);


    _NOT_VISIBLE_FOR_USER_DB_TABLE_COLUMN_NAME
		<< YerothDatabaseTableColumn::MEMBRES_DU_GROUPE_db_ID;


    setup_select_configure_dbcolumn(YerothDatabase::GROUPES_DE_CLIENTS);

    setupLineEdits();

    setupLineEditsQCompleters();


    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionInformationEntreprise, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionA_propos, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_principal, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionListerLesGroupesDeClients, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionClients, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAnnuler, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerUnMembreDuGroupeDeClients, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEnregistrer, false);


    pushButton_annuler->disable(this);
    pushButton_menu_clients->disable(this);
    pushButton_supprimer_membre_du_groupe_de_clients->disable(this);
    pushButton_enregistrer->disable(this);

    /** Menu actions */
    connect(actionChanger_utilisateur, SIGNAL(triggered()), this, SLOT(changer_utilisateur()));
    connect(actionAppeler_aide, SIGNAL(triggered()), this, SLOT(help()));
    connect(actionDeconnecter_utilisateur, SIGNAL(triggered()), this, SLOT(deconnecter_utilisateur()));
    connect(actionMenu_principal, SIGNAL(triggered()), this, SLOT(clients()));
    connect(actionListerLesGroupesDeClients, SIGNAL(triggered()), this, SLOT(groupes_de_clients()));
    connect(actionClients, SIGNAL(triggered()), this, SLOT(clients()));
    connect(actionAnnuler, SIGNAL(triggered()), this, SLOT(clients()));
    connect(actionSupprimerUnMembreDuGroupeDeClients, SIGNAL(triggered()), this, SLOT(supprimerUnMembreDunGroupeDeClients()));
    connect(actionEnregistrer, SIGNAL(triggered()), this, SLOT(modifierEnregistrerUnGroupeDeClients()));
    connect(actionFermeture, SIGNAL(triggered()), this, SLOT(fermeture()));
    connect(actionA_propos, SIGNAL(triggered()), this, SLOT(apropos()));
    connect(actionInformationEntreprise, SIGNAL(triggered()), this, SLOT(infosEntreprise()));
    connect(actionQui_suis_je, SIGNAL(triggered()), this, SLOT(qui_suis_je()));
    connect(actionAdministration, SIGNAL(triggered()), this, SLOT(administration()));


    connect(tableWidget_modifier_groupe_clients_membres_initiaux_du_groupe,
    		SIGNAL(itemClicked(QTableWidgetItem *)),
			this,
            SLOT(handle_select_table_widget_row(QTableWidgetItem *)));

    connect(tableWidget_modifier_groupe_clients_membres_initiaux_du_groupe,
    		SIGNAL(itemDoubleClicked(QTableWidgetItem *)),
			this,
            SLOT(handle_select_table_widget_row(QTableWidgetItem *)));


#ifdef YEROTH_CLIENT
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    setupShortcuts();


    lineEdit_modifier_groupe_clients_designation->setFocus();
}

void YerothModifierGroupeDeClientsWindow::deconnecter_utilisateur()
{
    clear_all_fields();
    YerothWindowsCommons::deconnecter_utilisateur();
}

void YerothModifierGroupeDeClientsWindow::setupLineEdits()
{
	lineEdit_modifier_groupe_clients_maximum_de_membres->setValidator(&YerothUtils::DoubleValidator);
}


void YerothModifierGroupeDeClientsWindow::setupLineEditsQCompleters()
{
	lineEdit_modifier_groupe_clients_programme_de_fidelite_de_clients->
		setupMyStaticQCompleter(YerothDatabase::PROGRAMMES_DE_FIDELITE_CLIENTS,
								YerothDatabaseTableColumn::DESIGNATION);

	lineEdit_modifier_groupe_clients_recherche_client_initiaux->
		setupMyStaticQCompleter(YerothDatabase::CLIENTS,
								YerothDatabaseTableColumn::NOM_ENTREPRISE);

    connect((QObject *)lineEdit_modifier_groupe_clients_recherche_client_initiaux->getMyQCompleter(),
    		SIGNAL(activated(const QString &)),
			this,
            SLOT(ajouter_un_membre_au_groupe_de_clients(const QString &)));
}


void YerothModifierGroupeDeClientsWindow::setupShortcuts()
{
    setupShortcutActionMessageDaide 	(*actionAppeler_aide);
    setupShortcutActionQuiSuisJe		(*actionQui_suis_je);
}


void YerothModifierGroupeDeClientsWindow::definirPasDeRole()
{
    _logger->log("definirPasDeRole");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionInformationEntreprise, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionA_propos, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_principal, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionListerLesGroupesDeClients, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionClients, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAnnuler, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerUnMembreDuGroupeDeClients, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEnregistrer, false);


    pushButton_annuler->disable(this);
    pushButton_menu_clients->disable(this);
    pushButton_supprimer_membre_du_groupe_de_clients->disable(this);
    pushButton_enregistrer->disable(this);
}


void YerothModifierGroupeDeClientsWindow::definirCaissier()
{
    _logger->log("definirCaissier - definirPasDeRole()");

   definirPasDeRole();
}

void YerothModifierGroupeDeClientsWindow::definirManager()
{
    _logger->log("definirManager");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionInformationEntreprise, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionA_propos, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_principal, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionListerLesGroupesDeClients, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionClients, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAnnuler, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerUnMembreDuGroupeDeClients, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEnregistrer, true);

#ifdef YEROTH_CLIENT

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);

#elif YEROTH_SERVER

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);

#endif

    pushButton_annuler->enable(this, SLOT(annuler_la_creation_dun_groupe_de_clients()));
    pushButton_menu_clients->enable(this, SLOT(clients()));
    pushButton_supprimer_membre_du_groupe_de_clients->enable(this, SLOT(supprimerUnMembreDunGroupeDeClients()));
    pushButton_enregistrer->enable(this, SLOT(modifierEnregistrerUnGroupeDeClients()));
}


void YerothModifierGroupeDeClientsWindow::definirVendeur()
{
    _logger->log("definirVendeur");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionInformationEntreprise, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionA_propos, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_principal, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionListerLesGroupesDeClients, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionClients, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAnnuler, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerUnMembreDuGroupeDeClients, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEnregistrer, true);

#ifdef YEROTH_CLIENT

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);

#elif YEROTH_SERVER

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);

#endif

    pushButton_annuler->enable(this, SLOT(annuler_la_creation_dun_groupe_de_clients()));
    pushButton_menu_clients->enable(this, SLOT(clients()));
    pushButton_supprimer_membre_du_groupe_de_clients->enable(this, SLOT(supprimerUnMembreDunGroupeDeClients()));
    pushButton_enregistrer->enable(this, SLOT(modifierEnregistrerUnGroupeDeClients()));
}


void YerothModifierGroupeDeClientsWindow::definirGestionaireDesStocks()
{
    _logger->log("definirGestionaireDesStocks - definirPasDeRole()");

    definirPasDeRole();
}


void YerothModifierGroupeDeClientsWindow::definirMagasinier()
{
    _logger->log("definirMagasinier - definirPasDeRole()");

    definirPasDeRole();
}


void YerothModifierGroupeDeClientsWindow::update_client_group_member_data(const QString &new_client_group_designation,
																	   const QString &new_client_group_db_ID,
																	   const QString &client_db_ID)
{
	QString strQueryClientGroupMemberDATA(QString("select %1, %2 from %3 where %4='%5'")
											.arg(YerothDatabaseTableColumn::GROUPES_DU_CLIENT,
												 YerothDatabaseTableColumn::GROUPES_DU_CLIENT_ID,
												 YerothDatabase::CLIENTS,
												 YerothDatabaseTableColumn::ID,
												 client_db_ID));

    QSqlQuery query;

    int querySize = YerothUtils::execQuery(query, strQueryClientGroupMemberDATA, _logger);

    if (querySize <= 0)
    {
    	return ;
    }

    QString groupes_du_clients;
    QString groupes_du_clients_ID;

    if (query.next())
    {
    	groupes_du_clients = query.value(0).toString();
    	groupes_du_clients_ID = query.value(1).toString();
    }

    query.clear();

    YerothUtils::APPEND_NEW_ELEMENT_TO_STAR_SEPARATED_DB_STRING(new_client_group_designation,
    															groupes_du_clients);

    YerothUtils::APPEND_NEW_ELEMENT_TO_STAR_SEPARATED_DB_STRING(new_client_group_db_ID,
    															groupes_du_clients_ID);

	QString strQuery_UPDATE_ClientGroupMemberDATA(QString("UPDATE %1 SET %2='%3', %4='%5' WHERE %6='%7'")
													.arg(YerothDatabase::CLIENTS,
														 YerothDatabaseTableColumn::GROUPES_DU_CLIENT,
														 groupes_du_clients,
														 YerothDatabaseTableColumn::GROUPES_DU_CLIENT_ID,
														 groupes_du_clients_ID,
														 YerothDatabaseTableColumn::ID,
														 client_db_ID));

//	QDEBUG_STRING_OUTPUT_2("strQuery_UPDATE_ClientGroupMemberDATA", strQuery_UPDATE_ClientGroupMemberDATA);

	YerothUtils::execQuery(query, strQuery_UPDATE_ClientGroupMemberDATA, _logger);
}


bool YerothModifierGroupeDeClientsWindow::clientGroupAlreadyExists()
{
	bool result = false;

	YerothSqlTableModel &groupes_de_clients_TableModel = _allWindows->getSqlTableModel_groupes_de_clients();

	// ** check if customer account with same name exist
	result =
		YerothUtils::checkIf_KEYWORD_ALREADY_EXISTS(*this,
													groupes_de_clients_TableModel,
													*lineEdit_modifier_groupe_clients_recherche_client_initiaux,
													QObject::tr("Un groupe de client"),
													YerothDatabaseTableColumn::NOM_ENTREPRISE);

	return result;
}


bool YerothModifierGroupeDeClientsWindow::check_fields()
{
    bool result;

    bool nom_du_groupe_de_clients_modifier = lineEdit_modifier_groupe_clients_designation->checkField();

    result = nom_du_groupe_de_clients_modifier;

    return result;
}


void YerothModifierGroupeDeClientsWindow::clear_all_fields()
{
	lineEdit_modifier_groupe_clients_recherche_client_initiaux->clear();

	lineEdit_modifier_groupe_clients_reference_groupe->clear();
	lineEdit_modifier_groupe_clients_designation->clearField();
	lineEdit_modifier_groupe_clients_maximum_de_membres->clear();
	lineEdit_modifier_groupe_clients_programme_de_fidelite_de_clients->clearField();

	textEdit_modifier_groupe_clients_description_groupe->clear();

	tableWidget_modifier_groupe_clients_membres_initiaux_du_groupe->myClear();
}


void YerothModifierGroupeDeClientsWindow::rendreInvisible()
{
    setVisible(false);
}


void YerothModifierGroupeDeClientsWindow::rendreVisible(YerothSqlTableModel * stocksTableModel)
{
    _curStocksTableModel = stocksTableModel;

    setupLineEditsQCompleters();

    check_fields();

    setVisible(true);
}


bool YerothModifierGroupeDeClientsWindow::IS_client_present_dans_un_groupe_de_clients(const QString &aClientGroupMemberName)
{
	QTableWidgetItem *anItem = 0;

	for (unsigned int k = 0; k < tableWidget_modifier_groupe_clients_membres_initiaux_du_groupe->rowCount(); ++k)
	{
		anItem = tableWidget_modifier_groupe_clients_membres_initiaux_du_groupe->item(k, 1);

		if (0 != anItem)
		{
			if (YerothUtils::isEqualCaseInsensitive(aClientGroupMemberName, anItem->text()))
			{
				return true;
			}
		}
	}

	return false;
}


void YerothModifierGroupeDeClientsWindow::ajouter_un_membre_au_groupe_de_clients(const QString &aClientGroupMemberName)
{
	if (IS_client_present_dans_un_groupe_de_clients(aClientGroupMemberName))
	{
		return ;
	}

	YerothSqlTableModel &aClientSqlTableModel = _allWindows->getSqlTableModel_clients();

	aClientSqlTableModel.setFilter(QString("%1='%2'")
										.arg(YerothDatabaseTableColumn::NOM_ENTREPRISE,
												aClientGroupMemberName));

	QSqlRecord aClientRecord = aClientSqlTableModel.record(0);

	QString aClient_db_ID = GET_SQL_RECORD_DATA(aClientRecord, YerothDatabaseTableColumn::ID);

	QString aClientReference = GET_SQL_RECORD_DATA(aClientRecord, YerothDatabaseTableColumn::REFERENCE_CLIENT);


	tableWidget_modifier_groupe_clients_membres_initiaux_du_groupe
		->addAClientGroupMember(aClient_db_ID,
								aClientReference,
								aClientGroupMemberName);

	aClientSqlTableModel.resetFilter();

	lineEdit_modifier_groupe_clients_recherche_client_initiaux->myClear();
}


void YerothModifierGroupeDeClientsWindow::annuler_la_creation_dun_groupe_de_clients()
{
	clear_all_fields();

	groupes_de_clients();
}


void YerothModifierGroupeDeClientsWindow::supprimerUnMembreDunGroupeDeClients()
{
	tableWidget_modifier_groupe_clients_membres_initiaux_du_groupe
		->removeArticle(_last_selected_table_widget_row);
}


bool YerothModifierGroupeDeClientsWindow::modifierEnregistrerUnGroupeDeClients()
{
	if (!check_fields())
	{
		if (QMessageBox::Ok ==
				YerothQMessageBox::warning(this, "stock (service)",
										   QObject::tr("Remplisser tous les champs obligatoires !")))
		{
		}
		else
		{
		}

		return false;
	}

	if (clientGroupAlreadyExists())
	{
		return false;
	}


	QString retMsg(QObject::tr("Le groupe de clients '"));

	YerothSqlTableModel &clientGroupTableModel = _allWindows->getSqlTableModel_groupes_de_clients();


	QSqlRecord record = clientGroupTableModel.record();


	QString new_client_group_designation = lineEdit_modifier_groupe_clients_designation->text();

	int new_client_group_db_ID = YerothERPWindows::getNextIdSqlTableModel_groupes_de_clients();


	record.setValue(YerothDatabaseTableColumn::ID, new_client_group_db_ID);

	record.setValue(YerothDatabaseTableColumn::DATE_CREATION,
			DATE_TO_DB_FORMAT_STRING(GET_CURRENT_DATE));

	record.setValue(YerothDatabaseTableColumn::REFERENCE_GROUPE,
			lineEdit_modifier_groupe_clients_reference_groupe->text());

	record.setValue(YerothDatabaseTableColumn::DESIGNATION, new_client_group_designation);

	record.setValue(YerothDatabaseTableColumn::DESCRIPTION_GROUPE,
			textEdit_modifier_groupe_clients_description_groupe->toPlainText());

	record.setValue(YerothDatabaseTableColumn::MAXIMUM_DE_MEMBRES,
			lineEdit_modifier_groupe_clients_maximum_de_membres->text().toInt());

	record.setValue(YerothDatabaseTableColumn::PROGRAMME_DE_FIDELITE_CLIENTS,
			lineEdit_modifier_groupe_clients_programme_de_fidelite_de_clients->text());


	QMapIterator<int, QString> 	it_MapListIdxToElement_db_ID(
			tableWidget_modifier_groupe_clients_membres_initiaux_du_groupe->get_mapListIdxToElement_db_ID());


	QString a_cur_client_db_ID;

	QString membres_du_groupe_db_ID;

	while(it_MapListIdxToElement_db_ID.hasNext())
	{
		it_MapListIdxToElement_db_ID.next();

		a_cur_client_db_ID = it_MapListIdxToElement_db_ID.value();

		update_client_group_member_data(new_client_group_designation,
										QString::number(new_client_group_db_ID),
										a_cur_client_db_ID);

	    YerothUtils::APPEND_NEW_ELEMENT_TO_STAR_SEPARATED_DB_STRING(a_cur_client_db_ID,
	    															membres_du_groupe_db_ID);
	}

	record.setValue(YerothDatabaseTableColumn::MEMBRES_DU_GROUPE_db_ID, membres_du_groupe_db_ID);


	retMsg.append(lineEdit_modifier_groupe_clients_designation->text());

	bool success = clientGroupTableModel.insertNewRecord(record);

	if (!success)
	{
		retMsg.append(QObject::trUtf8("' n'a pas pu être créer !"));

		YerothQMessageBox::warning(this,
				QObject::trUtf8("créer 1 groupe de clients"),
				retMsg);
		return false;
	}

	clientGroupTableModel.select();

	retMsg.append(QObject::trUtf8("' a été créer avec succès !"));

	YerothQMessageBox::information(this,
			QObject::trUtf8("créer 1 groupe de clients"),
			retMsg);

	clear_all_fields();

	groupes_de_clients();

    return true;
}


