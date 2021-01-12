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

    setup_select_configure_dbcolumn(_allWindows->CLIENTS);

    _curClientsTableModel = &_allWindows->getSqlTableModel_clients();

    setupLineEdits();

    setupLineEditsQCompleters();

    pushButton_clients->disable(this);
	pushButton_groupes_de_clients->disable(this);
	pushButton_menu_principal->disable(this);
	pushButton_detail_client->disable(this);
	pushButton_supprimer->disable(this);


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
	menu.exec(event->globalPos());
}


void YerothGroupesDunClientWindow::setupShortcuts()
{
    setupShortcutActionQuiSuisJe			(*actionQui_suis_je);
    setupShortcutActionExporterAuFormatCsv	(*actionExporter_au_format_csv);
    setupShortcutActionAfficherPDF			(*actionAfficherPDF);
}


void YerothGroupesDunClientWindow::supprimer_appartenance()
{

}


void YerothGroupesDunClientWindow::afficher_au_detail(const QModelIndex &modelIndex)
{
    if (_curClientsTableModel->rowCount() > 0)
    {
    	const QString &curClient_db_ID =
    			tableWidget_groupes_dun_client
					->get_mapListIdxToElement_db_ID().value(modelIndex.row());

        _allWindows->_detailsGroupeDeClientsWindow
							->rendreVisible(_curClientsTableModel,
											_curStocksTableModel,
											curClient_db_ID);

        rendreInvisible();
    }
    else
    {
        YerothQMessageBox::warning(this, QObject::trUtf8("détails d'un groupe de clients"),
                                  QObject::trUtf8("Sélectionnez un groupe de clients à afficher les détails !"));
    }
}


void YerothGroupesDunClientWindow::afficher_tous_les_groupes_du_client()
{
	tableWidget_groupes_dun_client->myClear();

	QSqlRecord record;

	_allWindows->_clientsWindow->
		SQL_QUERY_YEROTH_TABLE_VIEW_LAST_SELECTED_ROW(record);

    _curClientDBID = GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::ID).toInt();

    QString groupes_du_client(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::GROUPES_DU_CLIENT));


    QStringList allClientGroups(groupes_du_client.split("*"));

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
													 _allWindows->GROUPES_DE_CLIENTS,
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


void YerothGroupesDunClientWindow::setupLineEdits()
{
	lineEdit_groupes_dun_client_recherche->
		enableForSearch(QObject::trUtf8("sélectioner un groupe dans lequel le client sera membre"));

	lineEdit_groupes_dun_client_nombre_de_groupes->setYerothEnabled(false);
}


void YerothGroupesDunClientWindow::setupLineEditsQCompleters()
{
	lineEdit_groupes_dun_client_recherche->
		setupMyStaticQCompleter(_allWindows->GROUPES_DE_CLIENTS,
								YerothDatabaseTableColumn::DESIGNATION,
								false,
								true);
}


void YerothGroupesDunClientWindow::rendreInvisible()
{
	tableWidget_groupes_dun_client->myClear();

	YerothWindowsCommons::rendreInvisible();
}


void YerothGroupesDunClientWindow::rendreVisible(YerothSqlTableModel *clientTableModel,
		  	  	  	  	  	  	  	  	  	  	 YerothSqlTableModel *stocksTableModel)
{
    _curStocksTableModel = stocksTableModel;

    tableWidget_groupes_dun_client->resizeColumnsToContents();

	afficher_tous_les_groupes_du_client();

	int tableRowCount = tableWidget_groupes_dun_client->rowCount();

	setVisible(true);

	lineEdit_groupes_dun_client_nombre_de_groupes->
		setText(GET_NUM_STRING(tableRowCount));

    if (tableRowCount > 0)
    {
    	enableExporterAuFormatCsv();
    	enableImprimer();
    }
    else
    {
    	disableExporterAuFormatCsv();
    	disableImprimer();
    }
}


void YerothGroupesDunClientWindow::definirCaissier()
{
    pushButton_clients->disable(this);
	pushButton_groupes_de_clients->disable(this);
	pushButton_menu_principal->disable(this);
	pushButton_detail_client->disable(this);
	pushButton_supprimer->disable(this);
}


void YerothGroupesDunClientWindow::definirManager()
{
    pushButton_clients->enable(this, SLOT(clients()));
	pushButton_groupes_de_clients->enable(this, SLOT(groupes_de_clients()));
	pushButton_menu_principal->enable(this, SLOT(menu()));
	pushButton_detail_client->enable(this, SLOT(afficher_au_detail()));
	pushButton_supprimer->enable(this, SLOT(supprimer_appartenance()));
}


void YerothGroupesDunClientWindow::definirVendeur()
{
    pushButton_clients->enable(this, SLOT(clients()));
	pushButton_groupes_de_clients->enable(this, SLOT(groupes_de_clients()));
	pushButton_menu_principal->enable(this, SLOT(menu()));
	pushButton_detail_client->enable(this, SLOT(afficher_au_detail()));
	pushButton_supprimer->enable(this, SLOT(supprimer_appartenance()));
}


void YerothGroupesDunClientWindow::definirGestionaireDesStocks()
{
    pushButton_clients->disable(this);
	pushButton_groupes_de_clients->disable(this);
	pushButton_menu_principal->disable(this);
	pushButton_detail_client->disable(this);
	pushButton_supprimer->disable(this);
}


void YerothGroupesDunClientWindow::definirMagasinier()
{
}


void YerothGroupesDunClientWindow::definirPasDeRole()
{
    pushButton_clients->disable(this);
	pushButton_groupes_de_clients->disable(this);
	pushButton_menu_principal->disable(this);
	pushButton_detail_client->disable(this);
	pushButton_supprimer->disable(this);
}


bool YerothGroupesDunClientWindow::export_csv_file()
{
	return false;
}


bool YerothGroupesDunClientWindow::imprimer_pdf_document()
{
	return YerothWindowsCommons::imprimer_pdf_document();
}

