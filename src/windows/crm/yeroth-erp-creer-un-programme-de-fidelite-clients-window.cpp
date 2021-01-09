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


//    _NOT_VISIBLE_FOR_USER_DB_TABLE_COLUMN_NAME
//		<< YerothDatabaseTableColumn::MEMBRES_DU_GROUPE_db_ID;


    setup_select_configure_dbcolumn(_allWindows->PROGRAMMES_DE_FIDELITE_CLIENTS);

    setupLineEdits();


    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionInformationEntreprise, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionA_propos, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_principal, false);
//    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionListerLesGroupesDeClients, false);
//    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionClients, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAnnuler, false);
//    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerUnMembreDuGroupeDeClients, false);
//    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEnregistrer, false);



    /** Menu actions */
    connect(actionChanger_utilisateur, SIGNAL(triggered()), this, SLOT(changer_utilisateur()));
    connect(actionAppeler_aide, SIGNAL(triggered()), this, SLOT(help()));
    connect(actionDeconnecter_utilisateur, SIGNAL(triggered()), this, SLOT(deconnecter_utilisateur()));
    connect(actionMenu_principal, SIGNAL(triggered()), this, SLOT(menu()));
//    connect(actionListerLesGroupesDeClients, SIGNAL(triggered()), this, SLOT(groupes_de_clients()));
//    connect(actionClients, SIGNAL(triggered()), this, SLOT(clients()));
//    connect(actionAnnuler, SIGNAL(triggered()), this, SLOT(menu()));
//    connect(actionSupprimerUnMembreDuGroupeDeClients, SIGNAL(triggered()), this, SLOT(supprimerUnMembreDunGroupeDeClients()));
//    connect(actionEnregistrer, SIGNAL(triggered()), this, SLOT(creerEnregistrerUnGroupeDeClients()));
    connect(actionFermeture, SIGNAL(triggered()), this, SLOT(fermeture()));
    connect(actionA_propos, SIGNAL(triggered()), this, SLOT(apropos()));
    connect(actionInformationEntreprise, SIGNAL(triggered()), this, SLOT(infosEntreprise()));
    connect(actionQui_suis_je, SIGNAL(triggered()), this, SLOT(qui_suis_je()));
    connect(actionAdministration, SIGNAL(triggered()), this, SLOT(administration()));


#ifdef YEROTH_CLIENT
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    setupShortcuts();


    lineEdit_creer_un_programme_de_fidelite_clients_designation->setFocus();
}

void YerothCreerUnProgrammeDeFideliteClientsWindow::deconnecter_utilisateur()
{
    clear_all_fields();
    YerothWindowsCommons::deconnecter_utilisateur();
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
//    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionListerLesGroupesDeClients, true);
//    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionClients, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAnnuler, true);
//    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerUnMembreDuGroupeDeClients, true);
//    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEnregistrer, true);
}


void YerothCreerUnProgrammeDeFideliteClientsWindow::definirVendeur()
{
    _logger->log("definirVendeur");

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

//	YerothSqlTableModel &groupes_de_clients_TableModel = _allWindows->getSqlTableModel_groupes_de_clients();
//
//	// ** check if customer account with same name exist
//	result =
//		YerothUtils::checkIfCustomerAccountAlreadyExist_NOMENTREPRISE(*this,
//																	  groupes_de_clients_TableModel,
//																	  *lineEdit_creer_groupe_clients_recherche_client_initiaux);

	return result;
}


bool YerothCreerUnProgrammeDeFideliteClientsWindow::check_fields()
{
    bool result = false;
//
//    bool nom_du_groupe_de_clients_creer = lineEdit_creer_groupe_clients_designation->checkField();
//
//    result = nom_du_groupe_de_clients_creer;
//
    return result;
}


void YerothCreerUnProgrammeDeFideliteClientsWindow::clear_all_fields()
{
//	lineEdit_creer_groupe_clients_recherche_client_initiaux->clear();
//
//	lineEdit_creer_groupe_clients_reference_groupe->clear();
//	lineEdit_creer_groupe_clients_designation->clearField();
//	lineEdit_creer_groupe_clients_maximum_de_membres->clear();
//	lineEdit_creer_groupe_clients_programme_de_fidelite_de_clients->clearField();
//
//	textEdit_creer_groupe_clients_description_groupe->clear();
//
//	tableWidget_creer_groupe_clients_membres_initiaux_du_groupe->myClear();
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


bool YerothCreerUnProgrammeDeFideliteClientsWindow::creerEnregistrerUnProgrammeDeFideliteClients()
{
	if (!check_fields())
	{
		if (QMessageBox::Ok ==
				YerothQMessageBox::warning(this, "programme de fidélité clients",
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

	return false;
//	QString retMsg(QObject::tr("Le groupe de clients '"));
//
//	YerothSqlTableModel &clientGroupTableModel = _allWindows->getSqlTableModel_groupes_de_clients();
//
//
//	QSqlRecord record = clientGroupTableModel.record();
//
//
//	QString new_client_group_designation = lineEdit_creer_groupe_clients_designation->text();
//
//	int new_client_group_db_ID = YerothERPWindows::getNextIdSqlTableModel_groupes_de_clients();
//
//
//	record.setValue(YerothDatabaseTableColumn::ID, new_client_group_db_ID);
//
//	record.setValue(YerothDatabaseTableColumn::DATE_CREATION,
//			DATE_TO_DB_FORMAT_STRING(GET_CURRENT_DATE));
//
//	record.setValue(YerothDatabaseTableColumn::REFERENCE_GROUPE,
//			lineEdit_creer_groupe_clients_reference_groupe->text());
//
//	record.setValue(YerothDatabaseTableColumn::DESIGNATION, new_client_group_designation);
//
//	record.setValue(YerothDatabaseTableColumn::DESCRIPTION_GROUPE,
//			textEdit_creer_groupe_clients_description_groupe->toPlainText());
//
//	record.setValue(YerothDatabaseTableColumn::MAXIMUM_DE_MEMBRES,
//			lineEdit_creer_groupe_clients_maximum_de_membres->text().toInt());
//
//
//	QMapIterator<int, QString> 	it_MapListIdxToElement_db_ID(
//			tableWidget_creer_groupe_clients_membres_initiaux_du_groupe->get_mapListIdxToElement_db_ID());
//
//
//	QString a_cur_client_db_ID;
//
//	QString membres_du_groupe_db_ID;
//
//	while(it_MapListIdxToElement_db_ID.hasNext())
//	{
//		it_MapListIdxToElement_db_ID.next();
//
//		a_cur_client_db_ID = it_MapListIdxToElement_db_ID.value();
//
//		update_client_group_member_data(new_client_group_designation,
//										QString::number(new_client_group_db_ID),
//										a_cur_client_db_ID);
//
//	    YerothUtils::APPEND_NEW_ELEMENT_TO_STAR_SEPARATED_DB_STRING(a_cur_client_db_ID,
//	    															membres_du_groupe_db_ID);
//	}
//
//	record.setValue(YerothDatabaseTableColumn::MEMBRES_DU_GROUPE_db_ID, membres_du_groupe_db_ID);
//
//
//	retMsg.append(lineEdit_creer_groupe_clients_designation->text());
//
//	bool success = clientGroupTableModel.insertNewRecord(record);
//
//	if (!success)
//	{
//		retMsg.append(QObject::trUtf8("' n'a pas pu être créer !"));
//
//		YerothQMessageBox::warning(this,
//				QObject::trUtf8("créer 1 groupe de clients"),
//				retMsg);
//		return false;
//	}
//
//	clientGroupTableModel.select();
//
//	retMsg.append(QObject::trUtf8("' a été créer avec succès !"));
//
//	YerothQMessageBox::information(this,
//			QObject::trUtf8("créer 1 groupe de clients"),
//			retMsg);
//
//	clear_all_fields();
//
//	groupes_de_clients();
//
//    return true;
}


