/*
 * yeroth-erp-details-dun-groupe-de-clients-window.cpp
 *
 *      Author: DR. XAVIER NOUMBISSI NOUNDOU
 */

#include "yeroth-erp-details-dun-groupe-de-clients-window.hpp"

#include "src/yeroth-erp-windows.hpp"

#include "src/process/yeroth-erp-process.hpp"

#include "src/users/yeroth-erp-users.hpp"

#include "src/utils/yeroth-erp-sqltable-model.hpp"


#include <QtSql/QSqlQuery>

#include <QtSql/QSqlRecord>


YerothDetailsDunGroupeDeClientsWindow::YerothDetailsDunGroupeDeClientsWindow()
:YerothWindowsCommons(),
 _logger(new YerothLogger("YerothDetailsDunGroupeDeClientsWindow"))
{
    _windowName = QString("%1 - %2")
    				.arg(YEROTH_ERP_WINDOW_TITLE,
    					 QObject::trUtf8("détails d'un groupe de clients"));

    setupUi(this);

    mySetupUi(this);

    QMESSAGE_BOX_STYLE_SHEET = QString("QMessageBox {background-color: rgb(%1);}"
                                       "QMessageBox QLabel {color: rgb(%2);}")
                            		.arg(COLOUR_RGB_STRING_YEROTH_YELLOW_254_254_0,
                            		     COLOUR_RGB_STRING_YEROTH_BLACK_0_0_0);

    setupLineEdits();

    textEdit_details_dun_groupe_de_clients_description_groupe->setYerothEnabled(false);

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionRETOUR, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);


	pushButton_programmes_de_fidelite_clients->disable(this);
	pushButton_menu_clients->disable(this);
    pushButton_RETOUR->disable(this);

    /** Menu actions */
    connect(actionChanger_utilisateur, SIGNAL(triggered()), this, SLOT(changer_utilisateur()));
    connect(actionAppeler_aide, SIGNAL(triggered()), this, SLOT(help()));
    connect(actionDeconnecter_utilisateur, SIGNAL(triggered()), this, SLOT(deconnecter_utilisateur()));
    connect(actionMenuClients, SIGNAL(triggered()), this, SLOT(clients()));
    connect(actionRETOUR, SIGNAL(triggered()), this, SLOT(groupes_de_clients()));
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


void YerothDetailsDunGroupeDeClientsWindow::afficher_groupes_dun_client()
{
	if (0 == _curClientTableModel)
	{
		groupes_de_clients();
	}

	rendreInvisible();

	_allWindows->_groupesDunClientWindow->rendreVisible(_curClientTableModel,
														_curStocksTableModel);
}


void YerothDetailsDunGroupeDeClientsWindow::modifier_un_groupe_de_clients()
{

}


void YerothDetailsDunGroupeDeClientsWindow::setupLineEdits()
{
	lineEdit_details_dun_groupe_de_clients_reference_groupe->setYerothEnabled(false);
	lineEdit_details_dun_groupe_de_clients_designation->setYerothEnabled(false);
	lineEdit_details_dun_groupe_de_clients_maximum_de_membres->setYerothEnabled(false);
	lineEdit_details_dun_groupe_de_clients_programme_de_fidelite_de_clients->setYerothEnabled(false);
}


void YerothDetailsDunGroupeDeClientsWindow::definirPasDeRole()
{
    _logger->log("definirPasDeRole");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionRETOUR, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);

	pushButton_programmes_de_fidelite_clients->disable(this);
	pushButton_menu_clients->disable(this);
    pushButton_RETOUR->disable(this);
}

void YerothDetailsDunGroupeDeClientsWindow::definirCaissier()
{
    _logger->log("definirCaissier - definirPasDeRole()");

    definirPasDeRole();
}

void YerothDetailsDunGroupeDeClientsWindow::definirManager()
{
    _logger->log("definirManager");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionRETOUR, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

	pushButton_programmes_de_fidelite_clients->enable(this, SLOT(programmes_de_fidelite_clients()));
	pushButton_menu_clients->enable(this, SLOT(clients()));
    pushButton_RETOUR->enable(this, SLOT(groupes_de_clients()));
}


void YerothDetailsDunGroupeDeClientsWindow::definirVendeur()
{
    _logger->log("definirVendeur");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionRETOUR, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

	pushButton_programmes_de_fidelite_clients->disable(this);
	pushButton_menu_clients->enable(this, SLOT(clients()));
    pushButton_RETOUR->enable(this, SLOT(groupes_de_clients()));
}


void YerothDetailsDunGroupeDeClientsWindow::definirGestionaireDesStocks()
{
    _logger->log("definirGestionaireDesStocks - definirPasDeRole()");

    definirPasDeRole();
}

void YerothDetailsDunGroupeDeClientsWindow::definirMagasinier()
{
    _logger->log("definirMagasinier - definirPasDeRole");

    definirPasDeRole();
}


void YerothDetailsDunGroupeDeClientsWindow::rendreInvisible()
{

    YerothWindowsCommons::rendreInvisible();
}


void YerothDetailsDunGroupeDeClientsWindow::rendreVisible(YerothSqlTableModel *aClientTableModel,
														  YerothSqlTableModel *aClientGroupTableModel,
											  	  	  	  YerothSqlTableModel *stocksTableModel,
														  const QString &clientGroup_db_ID /* = YerothUtils::EMPTY_STRING */)
{
	disconnect(actionRETOUR, 0, 0, 0);

	pushButton_RETOUR->disable(this);

	connect(actionRETOUR, SIGNAL(triggered()), this, SLOT(groupes_de_clients()));

	pushButton_RETOUR->enable(this, SLOT(groupes_de_clients()));


	_curClientTableModel = 0;

	_curStocksTableModel = stocksTableModel;

	_curClientGroupTableModel = aClientGroupTableModel;

	setVisible(true);

    showClientGroup_DETAIL(clientGroup_db_ID);
}


void YerothDetailsDunGroupeDeClientsWindow::rendreVisible(YerothSqlTableModel *clientTableModel,
														  YerothSqlTableModel *stocksTableModel,
							   	   	  	  	  	  	  	  const QString &clientGroup_db_ID /* = YerothUtils::EMPTY_STRING */)
 {
	disconnect(actionRETOUR, 0, 0, 0);

	pushButton_RETOUR->disable(this);

	connect(actionRETOUR, SIGNAL(triggered()), this, SLOT(afficher_groupes_dun_client()));

	pushButton_RETOUR->enable(this, SLOT(afficher_groupes_dun_client()));


	_curClientTableModel = clientTableModel;

	_curStocksTableModel = stocksTableModel;

	_curClientGroupTableModel = &_allWindows->getSqlTableModel_groupes_de_clients();

	setVisible(true);

    showClientGroup_DETAIL(clientGroup_db_ID);
 }


void YerothDetailsDunGroupeDeClientsWindow::showClientGroup_DETAIL(const QString &clientGroup_db_ID /* = YerothUtils::EMPTY_STRING */)
{
	tableWidget_details_dun_groupe_de_clients_membres_initiaux_du_groupe->yerothClearTableWidgetContent();

	QSqlRecord record;

	if (clientGroup_db_ID.isEmpty())
	{
		_allWindows->_groupesDeClientsWindow
			->SQL_QUERY_YEROTH_TABLE_VIEW_LAST_SELECTED_ROW(record);
	}
	else
	{
		_curClientGroupTableModel->setFilter(QString("%1='%2'")
												.arg(YerothDatabaseTableColumn::ID,
													 clientGroup_db_ID));

		record = _curClientGroupTableModel->record(0);

		_curClientGroupTableModel->resetFilter();
	}


	QString membres_du_groupe_db_ID =
			GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::MEMBRES_DU_GROUPE_db_ID);

	QStringList membres_du_groupe_db_ID_LIST;

	YerothUtils::SPLIT_STAR_SEPARATED_DB_STRING(membres_du_groupe_db_ID_LIST,
											    membres_du_groupe_db_ID);


    int curQuerySize = -1;

    QSqlQuery aSqlClientTableModelQUERY;

    QString clientTableModelQUERY_STR;


    QString aClientMember_nom_entreprise;

    QString aClientMember_reference;

	QString aClientMember_db_ID;

	for (unsigned int i = 0; i < membres_du_groupe_db_ID_LIST.size(); ++i)
	{
		aClientMember_db_ID = membres_du_groupe_db_ID_LIST.at(i).trimmed();

		if (aClientMember_db_ID.isEmpty())
		{
			continue;
		}

        clientTableModelQUERY_STR = QString("select %1, %2 from %3 where %4='%5'")
        										.arg(YerothDatabaseTableColumn::REFERENCE_CLIENT,
        											 YerothDatabaseTableColumn::NOM_ENTREPRISE,
													 YerothDatabase::CLIENTS,
													 YerothDatabaseTableColumn::ID,
													 aClientMember_db_ID);

        aSqlClientTableModelQUERY.clear();

        curQuerySize = YerothUtils::execQuery(aSqlClientTableModelQUERY, clientTableModelQUERY_STR, _logger);

        if (curQuerySize > 0)
        {
        	if (aSqlClientTableModelQUERY.next())
        	{
        		aClientMember_reference = aSqlClientTableModelQUERY.value(0).toString();

        		aClientMember_nom_entreprise = aSqlClientTableModelQUERY.value(1).toString();

        		tableWidget_details_dun_groupe_de_clients_membres_initiaux_du_groupe
					->addAClientGroupMember(aClientMember_db_ID,
											aClientMember_reference,
											aClientMember_nom_entreprise);
        	}
        }
	}

//	QDEBUG_STRING_OUTPUT_2("designation",
//			GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::DESIGNATION));

	lineEdit_details_dun_groupe_de_clients_reference_groupe
		->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::REFERENCE_GROUPE));

	lineEdit_details_dun_groupe_de_clients_designation
		->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::DESIGNATION));

	lineEdit_details_dun_groupe_de_clients_maximum_de_membres
		->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::MAXIMUM_DE_MEMBRES));

	lineEdit_details_dun_groupe_de_clients_programme_de_fidelite_de_clients
		->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::PROGRAMME_DE_FIDELITE_CLIENTS));

	textEdit_details_dun_groupe_de_clients_description_groupe
		->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::DESCRIPTION_GROUPE));
}


void YerothDetailsDunGroupeDeClientsWindow::setupShortcuts()
{
    setupShortcutActionMessageDaide 	(*actionAppeler_aide);
    setupShortcutActionQuiSuisJe		(*actionQui_suis_je);
}

