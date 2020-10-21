/*
 * yeroth-erp-alertes-window.cpp
 *
 *      Author: DIPL.-INF. XAVIER NOUMBISSI NOUNDOU
 */

#include "yeroth-erp-alertes-window.hpp"


#include "src/utils/yeroth-erp-database-table-column.hpp"

#include "src/yeroth-erp-windows.hpp"

#include "src/utils/yeroth-erp-sqltable-model.hpp"

#include "src/users/yeroth-erp-users.hpp"

#include "src/utils/yeroth-erp-logger.hpp"


#include <QtSql/QSqlRecord>


YerothAlertesWindow::YerothAlertesWindow()
:YerothWindowsCommons("yeroth-erp-alertes"),
 _logger(new YerothLogger("YerothAlertesWindow")),
 _currentTabView(0)
{
    _windowName = QString("%1 - %2")
    				.arg(YEROTH_ERP_WINDOW_TITLE,
    					 QObject::trUtf8("alertes"));

    setupUi(this);

    mySetupUi(this);

    setYerothTableView_FROM_WINDOWS_COMMONS(tableView_lister_alertes);

    QMESSAGE_BOX_STYLE_SHEET = QString("QMessageBox {background-color: rgb(%1);}"
                                       "QMessageBox QLabel {color: rgb(%2);}")
                                   .arg(COLOUR_RGB_STRING_YEROTH_FIREBRICK_RED_255_48_48,
                                		COLOUR_RGB_STRING_YEROTH_WHITE_255_255_255);

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMarquerResolue, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimer, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVendre, false);

    setupLineEdits();

    textEdit_alerte_message->setYerothEnabled(false);

    dateEdit_alertes_date_debut->setYerothEnabled(false);
    dateEdit_alertes_date_fin->setYerothEnabled(false);

    radioButton_alertes_periode_temps->setEnabled(false);
    radioButton_alertes_quantite->setEnabled(false);

    tabWidget_alertes->setTabEnabled(AfficherAlerteAuDetail, false);

    pushButton_retour_alertes->enable(this, SLOT(retourAlertes()));

    connect(tabWidget_alertes, SIGNAL(currentChanged(int)), this, SLOT(handleCurrentChanged(int)));
    connect(tableView_lister_alertes, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(afficher()));

    /** Menu actions */
    connect(actionChanger_utilisateur, SIGNAL(triggered()), this, SLOT(changer_utilisateur()));
    connect(actionAppeler_aide, SIGNAL(triggered()), this, SLOT(help()));
    connect(actionDeconnecter_utilisateur, SIGNAL(triggered()), this, SLOT(deconnecter_utilisateur()));
    connect(actionMenu, SIGNAL(triggered()), this, SLOT(menu()));
    connect(actionStocks, SIGNAL(triggered()), this, SLOT(lister_alertes()));
    connect(actionSupprimer, SIGNAL(triggered()), this, SLOT(supprimer()));
    connect(actionFermeture, SIGNAL(triggered()), this, SLOT(fermeture()));
    connect(actionA_propos, SIGNAL(triggered()), this, SLOT(apropos()));
    connect(actionVendre, SIGNAL(triggered()), this, SLOT(vendre()));
    connect(actionAfficher_au_detail, SIGNAL(triggered()), this, SLOT(afficher()));
    connect(actionInformationEntreprise, SIGNAL(triggered()), this, SLOT(infosEntreprise()));
    connect(actionQui_suis_je, SIGNAL(triggered()), this, SLOT(qui_suis_je()));
    connect(actionAdministration, SIGNAL(triggered()), this, SLOT(administration()));

#ifdef YEROTH_SERVER
    connect(actionAdministration, SIGNAL(triggered()), this, SLOT(administration()));
#endif

#ifdef YEROTH_CLIENT
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    setupShortcuts();
}

YerothAlertesWindow::~YerothAlertesWindow()
{
    delete _logger;
}

void YerothAlertesWindow::setupLineEdits()
{
    lineEdit_alertes_nom->setYerothEnabled(false);
    lineEdit_alertes_quantite->setYerothEnabled(false);
    lineEdit_alertes_resolue->setYerothEnabled(false);
    lineEdit_alertes_designation->setYerothEnabled(false);
    lineEdit_alertes_destinataire->setYerothEnabled(false);
    lineEdit_alertes_nom_destinataire->setYerothEnabled(false);
}

void YerothAlertesWindow::deconnecter_utilisateur()
{
    clear_all_fields();
    YerothWindowsCommons::deconnecter_utilisateur();
}

void YerothAlertesWindow::definirCaissier()
{
    _logger->log("definirCaissier");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMarquerResolue, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVendre, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimer, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    pushButton_afficher_detail->enable(this, SLOT(afficher()));
    pushButton_marquer_resolue->enable(this, SLOT(marquer_resolue()));
    pushButton_menu->disable(this);
    pushButton_lister->enable(this, SLOT(lister_les_alertes()));
    pushButton_supprimer->disable(this);
}

void YerothAlertesWindow::definirManager()
{
    _logger->log("definirManager");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMarquerResolue, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVendre, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimer, true);

#ifdef YEROTH_SERVER
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);
#endif

#ifdef YEROTH_CLIENT
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    pushButton_afficher_detail->enable(this, SLOT(afficher()));
    pushButton_marquer_resolue->enable(this, SLOT(marquer_resolue()));
    pushButton_menu->enable(this, SLOT(menu()));
    pushButton_lister->enable(this, SLOT(lister_les_alertes()));
    pushButton_supprimer->enable(this, SLOT(supprimer()));
}


void YerothAlertesWindow::definirVendeur()
{
    _logger->log("definirVendeur");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMarquerResolue, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVendre, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimer, false);

#ifdef YEROTH_SERVER
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

#ifdef YEROTH_CLIENT
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    pushButton_afficher_detail->enable(this, SLOT(afficher()));
    pushButton_marquer_resolue->enable(this, SLOT(marquer_resolue()));
    pushButton_menu->enable(this, SLOT(menu()));
    pushButton_lister->enable(this, SLOT(lister_les_alertes()));
    pushButton_supprimer->disable(this);
}


void YerothAlertesWindow::definirGestionaireDesStocks()
{
    _logger->log("definirGestionaireDesStocks");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMarquerResolue, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVendre, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimer, true);

#ifdef YEROTH_SERVER
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

#ifdef YEROTH_CLIENT
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    pushButton_afficher_detail->enable(this, SLOT(afficher()));
    pushButton_marquer_resolue->enable(this, SLOT(marquer_resolue()));
    pushButton_menu->enable(this, SLOT(menu()));
    pushButton_lister->enable(this, SLOT(lister_les_alertes()));
    pushButton_supprimer->disable(this);
}

void YerothAlertesWindow::definirMagasinier()
{
    _logger->log("definirMagasinier");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMarquerResolue, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVendre, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimer, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    pushButton_afficher_detail->enable(this, SLOT(afficher()));
    pushButton_marquer_resolue->enable(this, SLOT(marquer_resolue()));
    pushButton_menu->enable(this, SLOT(menu()));
    pushButton_lister->enable(this, SLOT(lister_les_alertes()));
    pushButton_supprimer->disable(this);
}

void YerothAlertesWindow::definirPasDeRole()
{
    _logger->log("definirPasDeRole");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVendre, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimer, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);

    pushButton_afficher_detail->disable(this);
    pushButton_marquer_resolue->disable(this);
    pushButton_menu->disable(this);
    pushButton_supprimer->disable(this);
}

void YerothAlertesWindow::handleCurrentChanged(int index)
{
    _currentTabView = index;

    switch (index)
    {
    case TableauDesAlertes:
    	lister_alertes();
        clear_all_fields();
        break;

    case AfficherAlerteAuDetail:
        afficher_au_detail();
        break;

    default:
        break;
    }
}

void YerothAlertesWindow::retourAlertes()
{
    _currentTabView = TableauDesAlertes;

    tabWidget_alertes->setCurrentIndex(TableauDesAlertes);
}


void YerothAlertesWindow::afficher()
{
	if (0 >= tableView_lister_alertes->rowCount())
	{
		return ;
	}
	else
	{
		tabWidget_alertes->setCurrentIndex(1);
	}
}


void YerothAlertesWindow::lister_alertes()
{
    YerothSqlTableModel & courrierAlertesSqlTableModel = _allWindows->getSqlTableModel_courriers_alertes();

    /*
     * Les utilisateurs 'Manager' et 'Administrateur' ont accès
     * à toutes les alertes. 'YerothUtils::ROLE_MAGASINIER' et 'YerothUtils::ROLE_CAISSIER' eux ont
     * juste accès aux alertes qui leurs sont destinées.
     */
    if (_allWindows->getUser()->isMagasinier() || _allWindows->getUser()->isGestionaireDesStocks()
            || _allWindows->getUser()->isCaissier())
    {
        QString userFilter(GENERATE_SQL_IS_STMT(YerothDatabaseTableColumn::DESTINATAIRE, _allWindows->getUser()->nom_utilisateur()));
        //qDebug() << "++ userFilter: " << userFilter;
        courrierAlertesSqlTableModel.yerothSetFilter_WITH_where_clause(userFilter);
    }
    tableView_lister_alertes->lister(courrierAlertesSqlTableModel);
    tableView_lister_alertes->hideColumn(0);
    tableView_lister_alertes->hideColumn(1);
    tableView_lister_alertes->hideColumn(2);
    tableView_lister_alertes->hideColumn(3);
    tableView_lister_alertes->hideColumn(10);

    tabWidget_alertes->setCurrentIndex(TableauDesAlertes);

    tableView_lister_alertes->selectRow(get_INT_last_selected_row_number());

    if (courrierAlertesSqlTableModel.rowCount() <= 0)
    {
        tabWidget_alertes->setTabEnabled(AfficherAlerteAuDetail, false);

    }
    else
    {
        tabWidget_alertes->setTabEnabled(AfficherAlerteAuDetail, true);
    }
}

void YerothAlertesWindow::afficher_au_detail()
{
    YerothSqlTableModel & courrierAlertesSqlTableModel = _allWindows->getSqlTableModel_courriers_alertes();

    if (_allWindows->getUser()->isMagasinier() || _allWindows->getUser()->isCaissier())
    {
        QString userFilter(GENERATE_SQL_IS_STMT(YerothDatabaseTableColumn::DESTINATAIRE, _allWindows->getUser()->nom_utilisateur()));
        //qDebug() << "++ userFilter: " << userFilter;
        courrierAlertesSqlTableModel.yerothSetFilter_WITH_where_clause(userFilter);
    }

    if (courrierAlertesSqlTableModel.select()
            && courrierAlertesSqlTableModel.rowCount() > getLastListerSelectedRow__ID())
    {
        QSqlRecord record;

        _allWindows->_listerAlertesWindow->
			SQL_QUERY_YEROTH_TABLE_VIEW_LAST_SELECTED_ROW(record);

        lineEdit_alertes_nom->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::DESIGNATION_ALERTE));

        lineEdit_alertes_designation->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::DESIGNATION));

        comboBox_alertes_condition->addItem(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::CONDITION_ALERTE));

        int quantite = GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::QUANTITE).toInt();

        if (quantite > -1)
        {
            lineEdit_alertes_quantite->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::QUANTITE));
            radioButton_alertes_quantite->setChecked(true);
            radioButton_alertes_periode_temps->setChecked(false);

            groupBox_alertes_quantite->setVisible(true);

            groupBox_periode_temps->setVisible(false);
        }
        else
        {
            QString date_debut(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::DATE_DEBUT));
            QString date_fin(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::DATE_FIN));

            //qDebug() << "++ date_debut " << date_debut;
            dateEdit_alertes_date_debut->setDate(GET_DATE_FROM_STRING(date_debut));
            dateEdit_alertes_date_fin->setDate(GET_DATE_FROM_STRING(date_fin));
            radioButton_alertes_quantite->setChecked(false);
            radioButton_alertes_periode_temps->setChecked(true);

            groupBox_alertes_quantite->setVisible(false);

            groupBox_periode_temps->setVisible(true);
        }

        lineEdit_alertes_destinataire->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::DESTINATAIRE));
        lineEdit_alertes_nom_destinataire->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::NOM_COMPLET_DESTINATAIRE));
        textEdit_alerte_message->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::MESSAGE_ALERTE));

        int alerte_resolue = GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::ALERTE_RESOLUE).toInt();

        lineEdit_alertes_resolue->setText(INT_TO_STRING(alerte_resolue));
    }
}

void YerothAlertesWindow::marquer_resolue()
{
    //qDebug() << "++ marquer_resolu()";
    YerothSqlTableModel & courrierAlertesSqlTableModel = _allWindows->getSqlTableModel_courriers_alertes();

    if (_allWindows->getUser()->isMagasinier() || _allWindows->getUser()->isCaissier())
    {
        QString userFilter(GENERATE_SQL_IS_STMT(YerothDatabaseTableColumn::DESTINATAIRE,
        										_allWindows->getUser()->nom_utilisateur()));

        //qDebug() << "++ userFilter: " << userFilter;
        courrierAlertesSqlTableModel.yerothSetFilter_WITH_where_clause(userFilter);
    }

    QString userFilter(GENERATE_SQL_IS_STMT(YerothDatabaseTableColumn::DESTINATAIRE,
    										_allWindows->getUser()->nom_utilisateur()));

    QSqlRecord courriersAlertesRecord;

    _allWindows->_listerAlertesWindow->
		SQL_QUERY_YEROTH_TABLE_VIEW_LAST_SELECTED_ROW(courriersAlertesRecord);

    QString id_alerte = GET_SQL_RECORD_DATA(courriersAlertesRecord, YerothDatabaseTableColumn::ID_ALERTE);

    YerothSqlTableModel & alertesSqlTableModel = _allWindows->getSqlTableModel_alertes();

    QString alerteFilter(GENERATE_SQL_IS_STMT(YerothDatabaseTableColumn::ID, id_alerte));
    alertesSqlTableModel.yerothSetFilter_WITH_where_clause(alerteFilter);

    //qDebug() << "++ marquer_resolu(), filter: " << alerteFilter;
    int alertesSqlTableModelRowCount = alertesSqlTableModel.easySelect();

    if (alertesSqlTableModelRowCount > 0)
    {
        //qDebug() << "++ marquer_resolu(), if";
        //Set in table 'alertes' that this alert has been resolved
        QSqlRecord alertesRecord = alertesSqlTableModel.record(0);

        if (1 == GET_SQL_RECORD_DATA(alertesRecord, YerothDatabaseTableColumn::ALERTE_RESOLUE).toInt())
        {
            QString msg(QString(QObject::trUtf8("L'alerte '%1' est déjà marquée résolue !"))
            				.arg(GET_SQL_RECORD_DATA(courriersAlertesRecord,
            					 YerothDatabaseTableColumn::DESIGNATION_ALERTE)));

            YerothQMessageBox::information(this,
            							   QObject::trUtf8("marquer résolue"),
                                           msg);
            return;
        }

        alertesRecord.setValue(YerothDatabaseTableColumn::ALERTE_RESOLUE, 1);

        bool alertes = alertesSqlTableModel.updateRecord(0, alertesRecord);

        //qDebug() << "++ alertes: " << alertes;
        //Set in table 'courriers_alertes' that this alert has been resolved
        courriersAlertesRecord.setValue(YerothDatabaseTableColumn::ALERTE_RESOLUE, 1);

        bool successCourriersAlertes = _allWindows->_listerAlertesWindow->
        		SQL_UPDATE_YEROTH_TABLE_VIEW_LAST_SELECTED_ROW(courriersAlertesRecord);

        if (alertes && successCourriersAlertes)
        {
        	lister_les_alertes();

            QString msg(QString(QObject::trUtf8("L'alerte '%1' a été marquée résolue !"))
            				.arg(GET_SQL_RECORD_DATA(courriersAlertesRecord,
            					 YerothDatabaseTableColumn::DESIGNATION_ALERTE)));

            YerothQMessageBox::information(this,
            							   QObject::trUtf8("marquer résolue - succès"),
                                           msg);
        }
        else
        {
            QString msg(QString(QObject::trUtf8("L'alerte '%1' n'a pas pu être marquée résolue !\n Contacter l'administrateur de '%2' !"))
            				.arg(GET_SQL_RECORD_DATA(courriersAlertesRecord,
            					 YerothDatabaseTableColumn::DESIGNATION_ALERTE),
            						YerothUtils::APPLICATION_NAME));

            YerothQMessageBox::warning(this,
            						   QObject::trUtf8("marquer résolue - échec"),
                                       msg);
        }
        //qDebug() << "++ successCourriersAlertes: " << successCourriersAlertes;
    }
}

void YerothAlertesWindow::lister_les_alertes()
{
    tabWidget_alertes->setCurrentIndex(TableauDesAlertes);
    lister_alertes();
}

void YerothAlertesWindow::supprimer()
{
    //qDebug() << "YerothAlertesWindow::supprimer()";
    if (!_allWindows->getUser()->isManager())
    {
        return;
    }

    QSqlRecord record;

    _allWindows->_listerAlertesWindow->
		SQL_QUERY_YEROTH_TABLE_VIEW_LAST_SELECTED_ROW(record);

    QString designation_alerte(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::DESIGNATION_ALERTE));

    if (!designation_alerte.isEmpty() && !designation_alerte.isNull())
    {
        QString msgSupprimer(QString(QObject::trUtf8("Poursuivre avec la suppression de l'alerte '%1' ?"))
        						.arg(designation_alerte));

        if (QMessageBox::Ok ==
                YerothQMessageBox::question(this,
                							QObject::tr("suppression d'une alerte"),
                							msgSupprimer,
											QMessageBox::Cancel,
											QMessageBox::Ok))
        {
            bool resRemoved = _allWindows->_listerAlertesWindow->
            		SQL_DELETE_YEROTH_TABLE_VIEW_LAST_SELECTED_ROW();
            //qDebug() << "YerothStocksWindow::supprimer_du_stock() " << resRemoved;

            QString supprimerAlerteStr(QString("DELETE FROM %1 WHERE %2 = '%3'")
            								.arg(_allWindows->ALERTES,
            									 YerothDatabaseTableColumn::DESIGNATION_ALERTE,
            									 designation_alerte));

            YerothUtils::execQuery(supprimerAlerteStr);

            lister_alertes();

            if (resRemoved)
            {
                msgSupprimer.clear();
                msgSupprimer.append(QString(QObject::trUtf8("L'alerte '%1' a été supprimée !"))
                						.arg(designation_alerte));

                YerothQMessageBox::information(this,
                							   QObject::tr("suppression d'une alerte"),
											   msgSupprimer);
            }
            else
            {
                msgSupprimer.clear();
                msgSupprimer.append(QString(QObject::trUtf8("L'alerte '%1' ne pouvait pas être supprimée !"))
                						.arg(designation_alerte));

                YerothQMessageBox::information(this,
                							   QObject::trUtf8("suppression d'une alerte"),
											   msgSupprimer);
            }
        }
        else
        {
        }
    }
}

void YerothAlertesWindow::clear_all_fields()
{
    lineEdit_alertes_nom->clear();
    lineEdit_alertes_designation->clear();
    comboBox_alertes_condition->clear();
    lineEdit_alertes_quantite->clear();
    lineEdit_alertes_destinataire->clear();
    lineEdit_alertes_nom_destinataire->clear();
    textEdit_alerte_message->clear();
}

void YerothAlertesWindow::setupShortcuts()
{
    setupShortcutActionMessageDaide 	(*actionAppeler_aide);
    setupShortcutActionQuiSuisJe		(*actionQui_suis_je);
}

void YerothAlertesWindow::rendreVisible(YerothSqlTableModel * stocksTableModel)
{
    _curStocksTableModel = stocksTableModel;

    tabWidget_alertes->resetYerothQTabWidget();

    setVisible(true);

    lister_alertes();
}

void YerothAlertesWindow::rendreInvisible()
{
	tabWidget_alertes->setTabEnabled(AfficherAlerteAuDetail, false);

	clear_all_fields();

    YerothWindowsCommons::rendreInvisible();
}
