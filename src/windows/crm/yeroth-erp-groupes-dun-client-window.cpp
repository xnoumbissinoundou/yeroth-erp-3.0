/*
 * yeroth-erp-groupes-dun-client-window.cpp
 *
 *      Author: DR. XAVIER NOUMBISSI NOUNDOU
 */

#include "yeroth-erp-groupes-dun-client-window.hpp"

#include "src/yeroth-erp-windows.hpp"

#include "src/process/yeroth-erp-process.hpp"

#include "src/users/yeroth-erp-users.hpp"

#include "src/utils/yeroth-erp-database-table-column.hpp"

#include "src/utils/yeroth-erp-config.hpp"

#include "src/widgets/yeroth-erp-qmessage-box.hpp"


#include <QtSql/QSqlRelationalTableModel>

#include <QtSql/QSqlQuery>

#include <QtCore/qmath.h>

#include <QtWidgets/QCompleter>


YerothGroupesDunClientWindow::YerothGroupesDunClientWindow()
:YerothWindowsCommons("yeroth-erp-groupes-dun-client"),
 _logger(new YerothLogger("YerothGroupesDunClientWindow")),
 _curClientsTableModel(0)
{
    _windowName = QString("%1 - %2")
    				.arg(YEROTH_ERP_WINDOW_TITLE,
    					 QObject::trUtf8("groupes d'1 client"));

    setupUi(this);

    mySetupUi(this);

    QMESSAGE_BOX_STYLE_SHEET =
        QString("QMessageBox {background-color: rgb(%1);}")
			.arg(COLOUR_RGB_STRING_YEROTH_YELLOW_254_254_0);

    _curClientsTableModel = &_allWindows->getSqlTableModel_clients();

    setupLineEdits();

    setupLineEditsQCompleters();


    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionInformationEntreprise, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionListerDesGroupesDeClients, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionClients, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionA_propos, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficher_ce_groupe_au_detail, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);


	pushButton_groupes_de_clients->disable(this);
	pushButton_menu_clients->disable(this);
	pushButton_RETOUR->disable(this);
	pushButton_retirer->disable(this);


	connect(actionAdministration, SIGNAL(triggered()), this, SLOT(administration()));
    connect(actionChanger_utilisateur, SIGNAL(triggered()), this, SLOT(changer_utilisateur()));
    connect(actionListerDesGroupesDeClients, SIGNAL(triggered()), this, SLOT(groupes_de_clients()));
    connect(actionClients, SIGNAL(triggered()), this, SLOT(clients()));
    connect(actionAppeler_aide, SIGNAL(triggered()), this, SLOT(help()));
    connect(actionDeconnecter_utilisateur, SIGNAL(triggered()), this, SLOT(deconnecter_utilisateur()));
    connect(actionFermeture, SIGNAL(triggered()), this, SLOT(fermeture()));
    connect(actionA_propos, SIGNAL(triggered()), this, SLOT(apropos()));
    connect(actionInformationEntreprise, SIGNAL(triggered()), this, SLOT(infosEntreprise()));
    connect(actionQui_suis_je, SIGNAL(triggered()), this, SLOT(qui_suis_je()));
	connect(actionAfficher_ce_groupe_au_detail, SIGNAL(triggered()), this, SLOT(afficher_au_detail()));
	connect(actionQui_suis_je, SIGNAL(triggered()), this, SLOT(qui_suis_je()));


	connect(tableWidget_groupes_dun_client,
			SIGNAL(doubleClicked(const QModelIndex &)),
			this,
			SLOT(afficher_au_detail(const QModelIndex &)));


    setupShortcuts();


    lineEdit_groupes_dun_client_recherche->setFocus();
}


void YerothGroupesDunClientWindow::contextMenuEvent(QContextMenuEvent * event)
{
	QMenu menu(this);
	menu.setPalette(toolBar_menuGroupesDunClientWindow->palette());
	menu.addAction(actionAfficher_ce_groupe_au_detail);
	menu.addAction(actionRetirer_ce_client_du_groupe_selectione);
	menu.exec(event->globalPos());
}


void YerothGroupesDunClientWindow::executer_ajouter_appartenance(const QString *un_groupe_de_clients)
{
	if (0 == un_groupe_de_clients)
	{
		return ;
	}

	QString SELECT_GROUP_FROM_DB_TABLE(QString("select %1, %2, %3, %4 from %5 where %6='%7'")
											.arg(YerothDatabaseTableColumn::ID,
												 YerothDatabaseTableColumn::REFERENCE_GROUPE,
												 YerothDatabaseTableColumn::MEMBRES_DU_GROUPE_db_ID,
												 YerothDatabaseTableColumn::MAXIMUM_DE_MEMBRES,
												 YerothDatabase::GROUPES_DE_CLIENTS,
												 YerothDatabaseTableColumn::DESIGNATION,
												 *un_groupe_de_clients));

	QSqlQuery aQSqlQuery;

	int query_size = YerothUtils::execQuery(aQSqlQuery, SELECT_GROUP_FROM_DB_TABLE);

	if (query_size <= 0)
	{
		return ;
	}

	QSqlRecord aClientGroupRecordInfo;

	if (!aQSqlQuery.next())
	{
		return ;
	}

	aClientGroupRecordInfo = aQSqlQuery.record();

	int maximum_de_membres =
			GET_SQL_RECORD_DATA(aClientGroupRecordInfo, YerothDatabaseTableColumn::MAXIMUM_DE_MEMBRES).toInt();

	QString membres_du_groupe_db_ID =
			GET_SQL_RECORD_DATA(aClientGroupRecordInfo, YerothDatabaseTableColumn::MEMBRES_DU_GROUPE_db_ID);

	QStringList membres_du_groupe_db_ID_LIST;

	YerothUtils::SPLIT_STAR_SEPARATED_DB_STRING(membres_du_groupe_db_ID_LIST,
											    membres_du_groupe_db_ID);

	if (membres_du_groupe_db_ID_LIST.size() >= maximum_de_membres)
	{
		 YerothQMessageBox::information(this, QObject::tr("ajouter"),
		           QObject::trUtf8("Le groupe de clients '%1' a déjà atteint son nombre maximum de membres !")
		 	 	 	 	 	 	 .arg(*un_groupe_de_clients));

		return ;
	}


	if (YerothUtils::APPEND_NEW_ELEMENT_TO_STAR_SEPARATED_DB_STRING(_curClientDBID,
																	membres_du_groupe_db_ID))
	{

		QString INSERT_UPDATE_CLIENT_WITHIN_GROUP(QString("UPDATE %1 SET %2='%3' WHERE %4='%5'")
													.arg(YerothDatabase::GROUPES_DE_CLIENTS,
														 YerothDatabaseTableColumn::MEMBRES_DU_GROUPE_db_ID,
														 membres_du_groupe_db_ID,
														 YerothDatabaseTableColumn::DESIGNATION,
														 *un_groupe_de_clients));

		bool insert_update_success = YerothUtils::execQuery(INSERT_UPDATE_CLIENT_WITHIN_GROUP);

//		QDEBUG_STRING_OUTPUT_2("insert_update_success - 1", BOOL_TO_STRING(insert_update_success));

		if (!insert_update_success)
		{
			YerothQMessageBox::information(this, QObject::tr("ajouter"),
					QObject::trUtf8("Le client '%1' n'a pas pu être ajouté "
							"dans le groupe de clients '%2' !")
			.arg(_curClient_NOM_ENTREPRISE,
				 *un_groupe_de_clients));

			return ;
		}
	}

	QSqlRecord record;

	_allWindows->_clientsWindow->
		SQL_QUERY_YEROTH_TABLE_VIEW_LAST_SELECTED_ROW(record);

	QString groupes_du_client = GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::GROUPES_DU_CLIENT);

	bool update_DB_GROUPES_DU_CLIENT_TABLE =
			YerothUtils::APPEND_NEW_ELEMENT_TO_STAR_SEPARATED_DB_STRING(*un_groupe_de_clients,
																		groupes_du_client);

	QString groupes_du_client_id =
			GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::GROUPES_DU_CLIENT_ID);

	QString un_groupe_de_clients_db_ID =
			GET_SQL_RECORD_DATA(aClientGroupRecordInfo, YerothDatabaseTableColumn::ID);


	bool update_groupes_du_client_id =
			YerothUtils::APPEND_NEW_ELEMENT_TO_STAR_SEPARATED_DB_STRING(un_groupe_de_clients_db_ID,
																		groupes_du_client_id);

	update_DB_GROUPES_DU_CLIENT_TABLE =
			update_DB_GROUPES_DU_CLIENT_TABLE || update_groupes_du_client_id;


	if (! update_DB_GROUPES_DU_CLIENT_TABLE)
	{
		 YerothQMessageBox::information(this, QObject::tr("ajouter"),
		           QObject::trUtf8("Le client '%1' est déjà membre du groupe de clients '%2' !")
		 	 	 	 	 	 .arg(_curClient_NOM_ENTREPRISE,
		 	 	 	 	 		  *un_groupe_de_clients));
		 return ;
	}


	QString UPDATE_CLIENT_GROUP_INFORMATION(QString("UPDATE %1 SET %2='%3', %4='%5' WHERE %6='%7'")
												.arg(YerothDatabase::CLIENTS,
													 YerothDatabaseTableColumn::GROUPES_DU_CLIENT,
													 groupes_du_client,
													 YerothDatabaseTableColumn::GROUPES_DU_CLIENT_ID,
													 groupes_du_client_id,
													 YerothDatabaseTableColumn::ID,
													 _curClientDBID));

	bool update_client_group_success = YerothUtils::execQuery(UPDATE_CLIENT_GROUP_INFORMATION);

	if (!update_client_group_success)
	{
		YerothQMessageBox::information(this, QObject::tr("ajouter"),
				QObject::trUtf8("Le client '%1' n'a pas pu être ajouté "
								"dans le groupe de clients '%2' !")
						.arg(_curClient_NOM_ENTREPRISE,
							 *un_groupe_de_clients));

		YEROTH_ERP_3_0_ROLLBACK_DATABASE_TRANSACTION;

		return ;
	}

	tableWidget_groupes_dun_client->
		insert_group(GET_SQL_RECORD_DATA(aClientGroupRecordInfo, YerothDatabaseTableColumn::ID),
										 *un_groupe_de_clients,
										 GET_SQL_RECORD_DATA(aClientGroupRecordInfo, YerothDatabaseTableColumn::REFERENCE_GROUPE),
										 QString::number(maximum_de_membres));


	lineEdit_groupes_dun_client_nombre_de_groupes->setText(QString::number(tableWidget_groupes_dun_client->rowCount()));


	YerothQMessageBox::information(this, QObject::tr("ajouter"),
	           QObject::trUtf8("Le client '%1' a été ajouté dans le groupe de clients '%2' !")
	 	 	 	 	 	 .arg(_curClient_NOM_ENTREPRISE,
	 	 	 	 	 		  *un_groupe_de_clients));
}


void YerothGroupesDunClientWindow::retirer_ce_client_du_groupe_selectione()
{

}


void YerothGroupesDunClientWindow::afficher_au_detail(const QModelIndex &modelIndex)
{
    if (_curClientsTableModel->rowCount() > 0)
    {
        _allWindows->_detailsGroupeDeClientsWindow->
			rendreVisible(_curClientsTableModel,
						   _curStocksTableModel,
						   tableWidget_groupes_dun_client->get_DB_ELEMENT_db_ID(modelIndex.row()));

        rendreInvisible();
    }
    else
    {
        YerothQMessageBox::information(this, QObject::trUtf8("détails"),
                                  	  QObject::trUtf8("Sélectionnez 1 compte client à afficher les détails."));
    }
}


void YerothGroupesDunClientWindow::afficher_tous_les_groupes_du_client()
{
	tableWidget_groupes_dun_client->yerothClearTableWidgetContent();

	QSqlRecord record;

	_allWindows->_clientsWindow->
		SQL_QUERY_YEROTH_TABLE_VIEW_LAST_SELECTED_ROW(record);

	_curClient_NOM_ENTREPRISE = GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::NOM_ENTREPRISE);

    _curClientDBID = GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::ID);


    groupBox_groupes_dun_client->setTitle(QObject::tr("Groupes d'appartenance du client (%1)")
    										.arg(_curClient_NOM_ENTREPRISE));


    QString groupes_du_client(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::GROUPES_DU_CLIENT));


    QStringList allClientGroups(groupes_du_client.split(YerothUtils::STAR_CHAR));

    QString current_group_db_ID;

    QString maximum_de_membres;

    QString current_client_group_reference;

    int curQuerySize = -1;

    QSqlQuery aSqlGroupTableModelQUERY;

    QString clientGroupTableModelQUERY_STR;

    for (int k = 0; k < allClientGroups.size(); ++k)
    {
        clientGroupTableModelQUERY_STR = QString("select %1, %2, %3 from %4 where %5='%6'")
        										.arg(YerothDatabaseTableColumn::ID,
        											 YerothDatabaseTableColumn::REFERENCE_GROUPE,
        											 YerothDatabaseTableColumn::MAXIMUM_DE_MEMBRES,
													 YerothDatabase::GROUPES_DE_CLIENTS,
													 YerothDatabaseTableColumn::DESIGNATION,
													 allClientGroups.at(k).trimmed());

        aSqlGroupTableModelQUERY.clear();

        curQuerySize = YerothUtils::execQuery(aSqlGroupTableModelQUERY, clientGroupTableModelQUERY_STR, _logger);

        if (curQuerySize > 0)
        {
        	if (aSqlGroupTableModelQUERY.next())
        	{
        		current_group_db_ID = QString::number(aSqlGroupTableModelQUERY.value(0).toInt());

        		current_client_group_reference = aSqlGroupTableModelQUERY.value(1).toString();

        		maximum_de_membres = aSqlGroupTableModelQUERY.value(2).toString();

        		tableWidget_groupes_dun_client->insert_group(current_group_db_ID,
        													 allClientGroups.at(k).trimmed(),
															 current_client_group_reference,
															 maximum_de_membres);
        	}

//        	qDebug() << tableWidget_groupes_dun_client->get_mapListIdxToElement_db_ID();
        }
    }

    if (tableWidget_groupes_dun_client->rowCount() > 0)
    {
    	tableWidget_groupes_dun_client->selectRow(0);
    }
}


void YerothGroupesDunClientWindow::enable_yeroth_widgets_ON_POSITIVE_QTABLE_WIDGET_ROW_COUNT()
{
	actionAfficher_ce_groupe_au_detail->setVisible(true);
	pushButton_retirer->setVisible(true);
}


void YerothGroupesDunClientWindow::disable_yeroth_widgets()
{
	actionAfficher_ce_groupe_au_detail->setVisible(false);
	pushButton_retirer->setVisible(false);
}


void YerothGroupesDunClientWindow::setupLineEdits()
{
	lineEdit_groupes_dun_client_recherche->
		enableForSearch(QObject::trUtf8("sélectioner un groupe dans lequel le client sera membre"));

	lineEdit_groupes_dun_client_nombre_de_groupes->setYerothEnabled(false);
}


void YerothGroupesDunClientWindow::setupLineEditsQCompleters()
{
	lineEdit_groupes_dun_client_recherche->
		setupMyStaticQCompleter(YerothDatabase::GROUPES_DE_CLIENTS,
								YerothDatabaseTableColumn::DESIGNATION);

    connect(lineEdit_groupes_dun_client_recherche->getMyQCompleter(),
    		SIGNAL(activated(const QString &)),
			this,
            SLOT(ajouter_appartenance(const QString &)));
}


void YerothGroupesDunClientWindow::rendreInvisible()
{
	tableWidget_groupes_dun_client->yerothClearTableWidgetContent();

	YerothWindowsCommons::rendreInvisible();
}


void YerothGroupesDunClientWindow::rendreVisible(YerothSqlTableModel *clientTableModel,
		  	  	  	  	  	  	  	  	  	  	 YerothSqlTableModel *stocksTableModel)
{
    _curStocksTableModel = stocksTableModel;

    setupLineEditsQCompleters();

    tableWidget_groupes_dun_client->resizeColumnsToContents();

	afficher_tous_les_groupes_du_client();

	int tableRowCount = tableWidget_groupes_dun_client->rowCount();

	if (tableRowCount > 0)
	{
		enable_yeroth_widgets_ON_POSITIVE_QTABLE_WIDGET_ROW_COUNT();
	}
	else
	{
		disable_yeroth_widgets();
	}

	setVisible(true);

	lineEdit_groupes_dun_client_nombre_de_groupes->
		setText(GET_NUM_STRING(tableRowCount));
}


void YerothGroupesDunClientWindow::definirManager()
{
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionInformationEntreprise, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionListerDesGroupesDeClients, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionClients, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionA_propos, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficher_ce_groupe_au_detail, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

	pushButton_groupes_de_clients->enable(this, SLOT(groupes_de_clients()));
	pushButton_menu_clients->enable(this, SLOT(clients()));
	pushButton_RETOUR->enable(this, SLOT(afficher_au_detail()));
	pushButton_retirer->enable(this, SLOT(retirer_ce_client_du_groupe_selectione()));
}


void YerothGroupesDunClientWindow::definirVendeur()
{
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionInformationEntreprise, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionListerDesGroupesDeClients, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionClients, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionA_propos, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficher_ce_groupe_au_detail, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

	pushButton_groupes_de_clients->disable(this);
	pushButton_menu_clients->enable(this, SLOT(clients()));
	pushButton_RETOUR->enable(this, SLOT(afficher_au_detail()));
	pushButton_retirer->enable(this, SLOT(retirer_ce_client_du_groupe_selectione()));
}


void YerothGroupesDunClientWindow::definirPasDeRole()
{
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionInformationEntreprise, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionListerDesGroupesDeClients, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionClients, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionA_propos, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficher_ce_groupe_au_detail, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);

	pushButton_groupes_de_clients->disable(this);
	pushButton_menu_clients->disable(this);
	pushButton_RETOUR->disable(this);
	pushButton_retirer->disable(this);
}

