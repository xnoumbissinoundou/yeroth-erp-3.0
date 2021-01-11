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

    textEdit_details_dun_programme_de_fidelite_clients_description->setYerothEnabled(false);

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionClients, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);



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


void YerothDetailsDunProgrammeDeFideliteClientsWindow::setupLineEdits()
{

}


void YerothDetailsDunProgrammeDeFideliteClientsWindow::definirPasDeRole()
{
    _logger->log("definirPasDeRole");
}

void YerothDetailsDunProgrammeDeFideliteClientsWindow::definirCaissier()
{
    _logger->log("definirCaissier - definirPasDeRole()");

    definirPasDeRole();
}

void YerothDetailsDunProgrammeDeFideliteClientsWindow::definirManager()
{
    _logger->log("definirManager");
}


void YerothDetailsDunProgrammeDeFideliteClientsWindow::definirVendeur()
{
    _logger->log("definirVendeur");
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


void YerothDetailsDunProgrammeDeFideliteClientsWindow::rendreVisible(YerothSqlTableModel * clientTableModel,
											  YerothSqlTableModel * stocksTableModel)
{
	_curStocksTableModel = stocksTableModel;

	_curClientTableModel = clientTableModel;

    //qDebug() << "++ last selected row: " << YerothERPWindows::get_last_lister_selected_row_ID();

	setVisible(true);

    showClientROYALTY_PROGRAM_DETAIL();
}


void YerothDetailsDunProgrammeDeFideliteClientsWindow::showClientROYALTY_PROGRAM_DETAIL()
{

}


void YerothDetailsDunProgrammeDeFideliteClientsWindow::setupShortcuts()
{
    setupShortcutActionMessageDaide 	(*actionAppeler_aide);
    setupShortcutActionQuiSuisJe		(*actionQui_suis_je);
}

