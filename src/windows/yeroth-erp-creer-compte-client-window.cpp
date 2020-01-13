/*
 * yeroth-erp-creer-compte-client-window.cpp
 *
 *      Author: Xavier NOUMBISSI NOUNDOU, Dipl.-Inf., Ph.D. (ABD)
 */

#include "yeroth-erp-creer-compte-client-window.hpp"

#include "src/yeroth-erp-windows.hpp"

#include "src/utils/yeroth-erp-database-table-column.hpp"

#include "src/users/yeroth-erp-users.hpp"

#include "src/utils/yeroth-erp-config.hpp"

#include <QtSql/QSqlQuery>

#include <QtSql/QSqlIndex>

#include "src/utils/yeroth-erp-database-table-column.hpp"

#include "src/utils/yeroth-erp-historique-stock.hpp"


const QString YerothCreerCompteClientWindow::_WINDOW_TITLE(QString(QObject::trUtf8("%1 - %2")).
        arg(YEROTH_ERP_WINDOW_TITLE, QObject::trUtf8("créer un compte client")));


YerothCreerCompteClientWindow::YerothCreerCompteClientWindow()
:YerothWindowsCommons(YerothCreerCompteClientWindow::_WINDOW_TITLE),
 _logger(new YerothLogger("YerothCreerCompteClientWindow"))
{
    setupUi(this);

    this->mySetupUi(this);

    QMESSAGE_BOX_STYLE_SHEET = QString("QMessageBox {background-color: rgb(%1);}"
                                       "QMessageBox QLabel {color: rgb(%2);}")
                                    .arg(COLOUR_RGB_STRING_YEROTH_YELLOW_254_254_0,
                                    	 COLOUR_RGB_STRING_YEROTH_BLACK_0_0_0);

    setupLineEdits();


    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionInformationEntreprise, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionA_propos, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_principal, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAchats, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionClients, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAnnuler, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEnregistrer, false);

    pushButton_achats->disable(this);
    pushButton_clients->disable(this);
    pushButton_menu_principal->disable(this);
    pushButton_annuler->disable(this);
    pushButton_enregistrer->disable(this);

    /** Menu actions */
    connect(actionChanger_utilisateur, SIGNAL(triggered()), this, SLOT(changer_utilisateur()));
    connect(actionAppeler_aide, SIGNAL(triggered()), this, SLOT(help()));
    connect(actionDeconnecter_utilisateur, SIGNAL(triggered()), this, SLOT(deconnecter_utilisateur()));
    connect(actionMenu_principal, SIGNAL(triggered()), this, SLOT(menu()));
    connect(actionAchats, SIGNAL(triggered()), this, SLOT(achats()));
    connect(actionClients, SIGNAL(triggered()), this, SLOT(clients()));
    connect(actionAnnuler, SIGNAL(triggered()), this, SLOT(menu()));
    connect(actionEnregistrer, SIGNAL(triggered()), this, SLOT(creerEnregistrerCompteClient()));
    connect(actionFermeture, SIGNAL(triggered()), this, SLOT(fermeture()));
    connect(actionA_propos, SIGNAL(triggered()), this, SLOT(apropos()));
    connect(actionAlertes, SIGNAL(triggered()), this, SLOT(alertes()));
    connect(actionInformationEntreprise, SIGNAL(triggered()), this, SLOT(infosEntreprise()));
    connect(actionQui_suis_je, SIGNAL(triggered()), this, SLOT(qui_suis_je()));
    connect(actionAdministration, SIGNAL(triggered()), this, SLOT(administration()));

#ifdef YEROTH_CLIENT
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    setupShortcuts();
}

void YerothCreerCompteClientWindow::deconnecter_utilisateur()
{
    this->clear_all_fields();
    _allWindows->definirPasDeRole();
    _allWindows->_mainWindow->show();
    this->rendreInvisible();
}

void YerothCreerCompteClientWindow::setupLineEdits()
{
    lineEdit_compte_client_dette_maximale->setValidator(&YerothUtils::DoubleValidator);
}


void YerothCreerCompteClientWindow::setupShortcuts()
{
    this->setupShortcutActionMessageDaide 	(*actionAppeler_aide);
    this->setupShortcutActionQuiSuisJe		(*actionQui_suis_je);
}


void YerothCreerCompteClientWindow::definirPasDeRole()
{
    _logger->log("definirPasDeRole");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionInformationEntreprise, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionA_propos, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_principal, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAchats, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionClients, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAnnuler, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEnregistrer, false);

    pushButton_achats->disable(this);
    pushButton_clients->disable(this);
    pushButton_menu_principal->disable(this);
    pushButton_annuler->disable(this);
    pushButton_enregistrer->disable(this);
}

void YerothCreerCompteClientWindow::definirCaissier()
{
    _logger->log("definirCaissier");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionInformationEntreprise, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionA_propos, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_principal, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAchats, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionClients, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAnnuler, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEnregistrer, false);

#ifdef YEROTH_CLIENT

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);

#elif YEROTH_SERVER

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);

#endif

    pushButton_achats->disable(this);
    pushButton_clients->disable(this);
    pushButton_menu_principal->disable(this);
    pushButton_annuler->disable(this);
    pushButton_enregistrer->disable(this);
}

void YerothCreerCompteClientWindow::definirManager()
{
    _logger->log("definirManager");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionInformationEntreprise, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionA_propos, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_principal, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAchats, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionClients, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAnnuler, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEnregistrer, true);

#ifdef YEROTH_CLIENT

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);

#elif YEROTH_SERVER

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);

#endif

    pushButton_achats->enable(this, SLOT(achats()));
    pushButton_clients->enable(this, SLOT(clients()));
    pushButton_menu_principal->enable(this, SLOT(menu()));
    pushButton_annuler->enable(this, SLOT(clients()));
    pushButton_enregistrer->enable(this, SLOT(creerEnregistrerCompteClient()));
}


void YerothCreerCompteClientWindow::definirVendeur()
{
    _logger->log("definirVendeur");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionInformationEntreprise, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionA_propos, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_principal, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAchats, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionClients, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAnnuler, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEnregistrer, false);

#ifdef YEROTH_CLIENT

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);

#elif YEROTH_SERVER

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);

#endif

    pushButton_achats->enable(this, SLOT(achats()));
    pushButton_clients->enable(this, SLOT(clients()));
    pushButton_menu_principal->enable(this, SLOT(menu()));
    pushButton_annuler->enable(this, SLOT(clients()));
    pushButton_enregistrer->enable(this, SLOT(creerEnregistrerCompteClient()));
}


void YerothCreerCompteClientWindow::definirGestionaireDesStocks()
{
    _logger->log("definirGestionaireDesStocks");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionInformationEntreprise, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionA_propos, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_principal, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAchats, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionClients, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAnnuler, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEnregistrer, false);

#ifdef YEROTH_CLIENT

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);

#elif YEROTH_SERVER

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);

#endif

    pushButton_achats->disable(this);
    pushButton_clients->disable(this);
    pushButton_menu_principal->disable(this);
    pushButton_annuler->disable(this);
    pushButton_enregistrer->disable(this);
}


void YerothCreerCompteClientWindow::definirMagasinier()
{
    _logger->log("definirMagasinier");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionInformationEntreprise, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionA_propos, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_principal, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAchats, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionClients, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAnnuler, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEnregistrer, false);

#ifdef YEROTH_CLIENT

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);

#elif YEROTH_SERVER

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);

#endif

    pushButton_achats->disable(this);
    pushButton_clients->disable(this);
    pushButton_menu_principal->disable(this);
    pushButton_annuler->disable(this);
    pushButton_enregistrer->disable(this);
}


bool YerothCreerCompteClientWindow::customerAccountExist()
{
	YerothSqlTableModel &clientsTableModel = _allWindows->getSqlTableModel_clients();

	{ // ** check if customer account with same name exist
		QString nom_entreprise_filter(QString("LOWER(%1) = LOWER('%2')")
				.arg(YerothDatabaseTableColumn::NOM_ENTREPRISE,
						lineEdit_compte_client_nom_de_lentreprise->text()));

//		qDebug() << QString("++ nom_entreprise_filter: %1")
//						.arg(nom_entreprise_filter);

		clientsTableModel.yerothSetFilter(nom_entreprise_filter);

		int clientsTableModelRowCount = clientsTableModel.rowCount();

		if (clientsTableModelRowCount > 0)
		{
			clientsTableModel.resetFilter();

			QString retMsg(QString(QObject::trUtf8("Une entreprise nommée '%1' existe déjà "
												   "dans la base de données !"))
					.arg(lineEdit_compte_client_nom_de_lentreprise->text()));

			YerothQMessageBox::warning(this,
					QObject::trUtf8("compte client déjà existant"),
					retMsg);
			return true;
		}

		clientsTableModel.resetFilter();
	}

	// ** check if customer account with same reference exist
	if (! lineEdit_compte_client_reference_client->isEmpty())
	{
		QString reference_client_filter(QString("LOWER(%1) = LOWER('%2')")
											.arg(YerothDatabaseTableColumn::REFERENCE_CLIENT,
												 lineEdit_compte_client_reference_client->text()));

		clientsTableModel.yerothSetFilter(reference_client_filter);

		int clientsTableModelRowCount = clientsTableModel.rowCount();

		if (clientsTableModelRowCount > 0)
		{
			clientsTableModel.resetFilter();

			QString retMsg(QString(QObject::trUtf8("Une entreprise avec la référence '%1' existe déjà "
												   "dans la base de données !"))
								.arg(lineEdit_compte_client_reference_client->text()));

			YerothQMessageBox::warning(this,
					QObject::trUtf8("compte client déjà existant"),
					retMsg);

			return true;
		}
		clientsTableModel.resetFilter();
	}

	return false;
}


bool YerothCreerCompteClientWindow::check_fields()
{
    bool result;

    bool nom_entreprise = lineEdit_compte_client_nom_de_lentreprise->checkField();

    bool nom_representant = lineEdit_compte_client_nom_du_representant->checkField();

    result = nom_entreprise && nom_representant;

    return result;
}


void YerothCreerCompteClientWindow::clear_all_fields()
{
	lineEdit_compte_client_reference_client->clear();
	lineEdit_compte_client_nom_de_lentreprise->clearField();
	lineEdit_compte_client_nom_du_representant->clearField();
    lineEdit_compte_client_quartier->clear();
    lineEdit_compte_client_ville->clear();
    lineEdit_compte_client_province_etat->clear();
    lineEdit_compte_client_pays->clear();
    lineEdit_compte_client_boite_postale->clear();
    lineEdit_compte_client_siege_social->clear();
    lineEdit_compte_client_email->clear();
    lineEdit_compte_client_numero_telephone_1->clear();
    lineEdit_compte_client_numero_telephone_2->clear();
    lineEdit_compte_client_numero_de_contribuable->clear();
    lineEdit_compte_client_dette_maximale->clear();
    textEdit_creer_compte_client_description_client->clear();
}

void YerothCreerCompteClientWindow::rendreInvisible()
{
    clear_all_fields();
    setVisible(false);
}

void YerothCreerCompteClientWindow::rendreVisible(YerothSqlTableModel * stocksTableModel)
{
    _curStocksTableModel = stocksTableModel;

    YerothPOSUser *aUser = YerothUtils::getAllWindows()->getUser();

    if (0 != YerothUtils::getAllWindows())
    {
    	if (0 != aUser && aUser->isManager())
    	{
    		label_compte_client_dette_maximale->setVisible(true);
    		lineEdit_compte_client_dette_maximale->setVisible(true);
    		lineEdit_compte_client_dette_maximale->setEnabled(true);
    	}
    	else
    	{
    		label_compte_client_dette_maximale->setVisible(false);
    		lineEdit_compte_client_dette_maximale->setVisible(false);
    		lineEdit_compte_client_dette_maximale->setEnabled(false);
    	}
    }

    check_fields();

    this->setVisible(true);
}


bool YerothCreerCompteClientWindow::creerEnregistrerCompteClient()
{
    if (check_fields())
    {
        if (customerAccountExist())
        {
        	return false;
        }

        QString retMsg(QObject::tr("Le client '"));

        YerothSqlTableModel &clientsTableModel = _allWindows->getSqlTableModel_clients();

        QSqlRecord record = clientsTableModel.record();

        record.setValue(YerothDatabaseTableColumn::ID, _allWindows->getNextIdSqlTableModel_clients());
        record.setValue(YerothDatabaseTableColumn::REFERENCE_CLIENT, lineEdit_compte_client_reference_client->text());
        record.setValue(YerothDatabaseTableColumn::NOM_ENTREPRISE, lineEdit_compte_client_nom_de_lentreprise->text());
        record.setValue(YerothDatabaseTableColumn::NOM_REPRESENTANT, lineEdit_compte_client_nom_du_representant->text());
        record.setValue(YerothDatabaseTableColumn::QUARTIER, lineEdit_compte_client_quartier->text());
        record.setValue(YerothDatabaseTableColumn::VILLE, lineEdit_compte_client_ville->text());
        record.setValue(YerothDatabaseTableColumn::PROVINCE_ETAT, lineEdit_compte_client_province_etat->text());
        record.setValue(YerothDatabaseTableColumn::PAYS, lineEdit_compte_client_pays->text());
        record.setValue(YerothDatabaseTableColumn::BOITE_POSTALE, lineEdit_compte_client_boite_postale->text());
        record.setValue(YerothDatabaseTableColumn::SIEGE_SOCIAL, lineEdit_compte_client_siege_social->text());
        record.setValue(YerothDatabaseTableColumn::EMAIL, lineEdit_compte_client_email->text());
        record.setValue(YerothDatabaseTableColumn::NUMERO_TELEPHONE_1, lineEdit_compte_client_numero_telephone_1->text());
        record.setValue(YerothDatabaseTableColumn::NUMERO_TELEPHONE_2, lineEdit_compte_client_numero_telephone_2->text());
        record.setValue(YerothDatabaseTableColumn::NUMERO_CONTRIBUABLE, lineEdit_compte_client_numero_de_contribuable->text());
        record.setValue(YerothDatabaseTableColumn::DETTE_MAXIMALE_COMPTE_CLIENT, lineEdit_compte_client_dette_maximale->text());

        YerothPOSUser *aUser = YerothUtils::getAllWindows()->getUser();

        if (0 != YerothUtils::getAllWindows())
        {
        	if (0 != aUser && aUser->isManager())
        	{
        		record.setValue(YerothDatabaseTableColumn::DETTE_MAXIMALE_COMPTE_CLIENT,
        						lineEdit_compte_client_dette_maximale->text().toDouble());
        	}
        	else
        	{
        		record.setValue(YerothDatabaseTableColumn::DETTE_MAXIMALE_COMPTE_CLIENT, 0.0);
        	}
        }
        else
        {
        	record.setValue(YerothDatabaseTableColumn::DETTE_MAXIMALE_COMPTE_CLIENT, 0.0);
        }

        record.setValue(YerothDatabaseTableColumn::DESCRIPTION_CLIENT, textEdit_creer_compte_client_description_client->toPlainText());

        retMsg.append(lineEdit_compte_client_nom_de_lentreprise->text());

        bool success = clientsTableModel.insertNewRecord(record);

        if (!success)
        {
            retMsg.append(QObject::trUtf8("' n'a pas pu être créer !"));

            YerothQMessageBox::warning(this,
                                 QObject::trUtf8("Yeroth-erp-3.0 ~ administration ~ créer ~ client"),
                                 retMsg);
            return false;
        }

        retMsg.append(QObject::trUtf8("' a été créer avec succès !"));

        YerothQMessageBox::information(this,
                                 QObject::trUtf8("Yeroth-erp-3.0 ~ administration ~ créer ~ client"),
                                 retMsg);

        clear_all_fields();

        clients();
    }

    return true;
}


