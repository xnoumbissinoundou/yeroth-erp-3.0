/*
 * yeroth-erp-admin-create-window.cpp
 *
 *      Author: Xavier NOUMBISSI NOUNDOU, Dipl.-Inf., Ph.D. (ABD)
 */


#include "yeroth-erp-admin-create-window.hpp"

#include "src/yeroth-erp-windows.hpp"


#include <QtCore/QDebug>

#include <QtWidgets/QDesktopWidget>

#include <QtSql/QSqlError>

#include <QtSql/QSqlRecord>

#include <QtSql/QSqlQuery>


YerothAdminCreateWindow::YerothAdminCreateWindow()
:YerothPOSAdminWindowsCommons(QObject::trUtf8("administration ~ créer")),
 _logger(new YerothLogger("YerothAdminCreateWindow"))
{
    setupUi(this);

    mySetupUi(this);

    QMESSAGE_BOX_STYLE_SHEET = QString("QMessageBox {background-color: rgb(%1);}"
                                       "QMessageBox QLabel {color: rgb(%2);}")
                                    .arg(COLOUR_RGB_STRING_YEROTH_DARK_GREEN_47_67_67,
                                    	 COLOUR_RGB_STRING_YEROTH_WHITE_255_255_255);

    setupLineEdits();
    setupDateTimeEdits();

    YEROTH_ERP_ADMIN_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    YEROTH_ERP_ADMIN_WRAPPER_QACTION_SET_ENABLED(actionRetournerMenuPrincipal, false);

    pushButton_menu->enable(this, SLOT(menu()));

    pushButton_creer_utilisateur_annuler->enable(this, SLOT(annuler()));
    pushButton_creer_localisation_annuler->enable(this, SLOT(annuler()));
    pushButton_creer_categorie_annuler->enable(this, SLOT(annuler()));
    pushButton_creer_alerte_annuler->enable(this, SLOT(annuler()));
    pushButton_creer_fournisseur_annuler->enable(this, SLOT(annuler()));
    pushButton_creer_remise_annuler->enable(this, SLOT(annuler()));


    pushButton_creer_utilisateur_valider->enable(this, SLOT(creer_utilisateur()));
    pushButton_creer_localisation_valider->enable(this, SLOT(creer_localisation()));
    pushButton_creer_categorie_valider->enable(this, SLOT(creer_categorie()));
    pushButton_creer_alerte_valider->enable(this, SLOT(creer_alerte()));
    pushButton_creer_fournisseur_valider->enable(this, SLOT(creer_fournisseur()));
    pushButton_creer_remise_valider->enable(this, SLOT(creer_remise()));


    pushButton_lister->enable(this, SLOT(lister()));
    pushButton_modifier->enable(this, SLOT(lister()));
    pushButton_supprimer->enable(this, SLOT(lister()));


    connect(actionStocks, SIGNAL(triggered()), this, SLOT(lister()));
    connect(actionMenu, SIGNAL(triggered()), this, SLOT(menu()));
    connect(actionModifier, SIGNAL(triggered()), this, SLOT(lister()));
    connect(actionSupprimer, SIGNAL(triggered()), this, SLOT(lister()));

    /** Menu actions */
    connect(actionChanger_utilisateur, SIGNAL(triggered()), this, SLOT(changer_utilisateur()));
    connect(actionDeconnecter_utilisateur, SIGNAL(triggered()), this, SLOT(deconnecter_utilisateur()));
    connect(actionAppeler_aide, SIGNAL(triggered()), this, SLOT(help()));
    connect(actionFermeture, SIGNAL(triggered()), this, SLOT(fermeture()));
    connect(actionA_propos, SIGNAL(triggered()), this, SLOT(apropos()));
    connect(actionRetournerMenuPrincipal, SIGNAL(triggered()), this, SLOT(retour_menu_principal()));
    connect(actionQui_suis_je, SIGNAL(triggered()), this, SLOT(qui_suis_je()));

    connect(radioButton_creer_alerte_quantite, SIGNAL(clicked(bool)), this, SLOT(radioButtons_quantite()));

    connect(radioButton_creer_alerte_periode_temps, SIGNAL(clicked(bool)), this,
            SLOT(radioButtons_periode_temps()));

    connect(tabWidget_creer, SIGNAL(currentChanged(int)), this, SLOT(handleCurrentChanged()));

    connect(tabWidget_creer, SIGNAL(currentChanged(int)), this, SLOT(handleCurrentChanged()));
}

void YerothAdminCreateWindow::definirPasDeRole()
{
    _logger->log("definirPasDeRole");
    YEROTH_ERP_ADMIN_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_ADMIN_WRAPPER_QACTION_SET_ENABLED(actionRetournerMenuPrincipal, false);
}

void YerothAdminCreateWindow::definirAdministrateur()
{
    _logger->log("definirAdministrateur");
    YEROTH_ERP_ADMIN_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_ADMIN_WRAPPER_QACTION_SET_ENABLED(actionRetournerMenuPrincipal, false);
}

void YerothAdminCreateWindow::definirManager()
{
    _logger->log("definirManager");
    YEROTH_ERP_ADMIN_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_ADMIN_WRAPPER_QACTION_SET_ENABLED(actionRetournerMenuPrincipal, true);
}

void YerothAdminCreateWindow::setupLineEdits()
{
    lineEdit_creer_alerte_quantite->setValidator(&YerothUtils::IntValidator);
    lineEdit_creer_utilisateur_mot_passe_1->setEchoMode(QLineEdit::Password);
    lineEdit_creer_utilisateur_verification->setEchoMode(QLineEdit::Password);
    lineEdit_creer_alerte_designation->setupMyStaticQCompleter(_allWindows->STOCKS, YerothDatabaseTableColumn::DESIGNATION);
    lineEdit_creer_alerte_destinataire->setupMyStaticQCompleter(_allWindows->USERS, "nom_utilisateur");

    connect(lineEdit_creer_alerte_destinataire, SIGNAL(textChanged(const QString &)), this,
            SLOT(showDestinataireNomComplet(const QString &)));
    connect(lineEdit_creer_alerte_designation, SIGNAL(textChanged(const QString &)), this,
            SLOT(showProduitInfo(const QString &)));
}

void YerothAdminCreateWindow::setupDateTimeEdits()
{
    dateEdit_creer_utilisateur_date_naissance->setStartDate(GET_CURRENT_DATE);
    dateEdit_creer_localisation_date_ouverture->setStartDate(GET_CURRENT_DATE);

    dateEdit_creer_alerte_date_debut->setStartDate(GET_CURRENT_DATE);
    dateEdit_creer_alerte_date_fin->setStartDate(GET_CURRENT_DATE);
}

void YerothAdminCreateWindow::rendreVisible(unsigned selectedSujetAction)
{
	tabWidget_creer->setCurrentIndex(selectedSujetAction);

    clear_utilisateur_all_fields();

    clear_categorie_all_fields();

    clear_localisation_all_fields();

    clear_fournisseur_all_fields();

    clear_alerte_all_fields();

    clear_remise_all_fields();

    lineEdit_creer_alerte_designation->setupMyStaticQCompleter(_allWindows->STOCKS, YerothDatabaseTableColumn::DESIGNATION);
    lineEdit_creer_alerte_destinataire->setupMyStaticQCompleter(_allWindows->USERS, "nom_utilisateur");
    lineEdit_creer_utilisateur_localisation->setEnabled(false);
    lineEdit_creer_utilisateur_localisation->setText(_allWindows->getInfoEntreprise().getLocalisation());

    populateUtilisateurComboBoxes();

    populateAlerteComboBoxes();

    populateLocalisationComboBoxes();

    creer_utilisateur_check_fields_entry();

    creer_categorie_check_fields();

    creer_localisation_check_fields();

    creer_fournisseur_check_fields();

    creer_categorie_check_fields();

    creer_alerte_check_fields_entry();

    this->setVisible(true);
}

void YerothAdminCreateWindow::rendreInvisible()
{
    dateEdit_creer_utilisateur_date_naissance->reset();
    dateEdit_creer_localisation_date_ouverture->reset();
    dateEdit_creer_alerte_date_debut->reset();
    dateEdit_creer_alerte_date_fin->reset();
    YerothPOSAdminWindowsCommons::rendreInvisible();
}

void YerothAdminCreateWindow::showDestinataireNomComplet(const QString & destinataireId)
{
    _logger->log("showDestinataireNomComplet(const QString &)",
                 QString("destinataireId: %1").arg(destinataireId));
    QString strQuery("SELECT nom_complet FROM ");
    strQuery.append(_allWindows->USERS).append(" WHERE nom_utilisateur = '").append(destinataireId).append("'");
    QSqlQuery query(strQuery);
    QSqlRecord rec = query.record();
    if (query.last())
    {
        QString destinataireId_nom_complet(query.value(0).toString());
        lineEdit_creer_alerte_nom_destinataire->setText(destinataireId_nom_complet);
    }
}

void YerothAdminCreateWindow::showProduitInfo(const QString & nomProduit)
{
    _logger->log("showProduitInfo(const QString &)", QString("designation: %1").arg(nomProduit));
    QString sqlNomProduit(YerothUtils::prepareSqlStr(nomProduit));
    QString strQuery("SELECT quantite_total FROM ");

    strQuery.append(_allWindows->STOCKS)
    .append(" WHERE designation = '")
    .append(sqlNomProduit).append("'");

    QSqlQuery query(strQuery);
    QSqlRecord rec = query.record();
    double quantite_en_stock = 0.0;
    while (query.next())
    {
        quantite_en_stock += query.value(0).toDouble();
    }
    lineEdit_creer_alerte_quantite_en_stock->setText(QString::number(quantite_en_stock, 'f', 0));
    query.clear();
    strQuery = "SELECT stock_minimum FROM ";
    strQuery.append(_allWindows->STOCKS)
    .append(" WHERE designation = '")
    .append(sqlNomProduit).append("'");
    if (query.exec(strQuery))
    {
        rec = query.record();
        double stock_minimum = 0.0;
        if (query.next())
        {
            stock_minimum = query.value(0).toDouble();
        }
        lineEdit_creer_alerte_stock_minimum->setText(QString::number(stock_minimum, 'f', 0));
    }
    showDatePeremption(nomProduit);
}

void YerothAdminCreateWindow::showDatePeremption(const QString & nomProduit)
{
    _logger->log("showDatePeremption", QString("designation: %1").arg(nomProduit));
    QString sqlNomProduit(YerothUtils::prepareSqlStr(nomProduit));
    QString strQuery("SELECT date_peremption FROM ");
    strQuery.append(_allWindows->STOCKS).append(" WHERE designation = '").append(sqlNomProduit).append("'");
    QSqlQuery query(strQuery);
    QSqlRecord rec = query.record();
    QDate date_peremption;
    QDate date_peremption_tmp;
    unsigned c = 0;
    while (query.next())
    {
        date_peremption_tmp = query.value(0).toDate();
        if (0 == c)
        {
            date_peremption = date_peremption_tmp;
            ++c;
        }
        else
        {
            if (date_peremption_tmp < date_peremption)
            {
                date_peremption = date_peremption_tmp;
            }
        }
    }
    lineEdit_creer_alerte_date_peremption->setText(date_peremption.toString(YerothUtils::DATE_FORMAT));
}

void YerothAdminCreateWindow::lister()
{
    _allWindows->_adminListerWindow->rendreVisible(tabWidget_creer->currentIndex());
    this->rendreInvisible();
}

void YerothAdminCreateWindow::annuler()
{
    clear_utilisateur_all_fields();

    clear_categorie_all_fields();

    clear_localisation_all_fields();

    clear_fournisseur_all_fields();

    clear_alerte_all_fields();

    clear_remise_all_fields();

    _allWindows->_adminWindow->rendreVisible();

    this->rendreInvisible();
}

void YerothAdminCreateWindow::radioButtons_quantite()
{
	dateEdit_creer_alerte_date_fin->setYerothEnabled(false);
	dateEdit_creer_alerte_date_debut->setYerothEnabled(false);

	comboBox_creer_alerte_condition->setEnabled(true);
	lineEdit_creer_alerte_quantite->setEnabled(true);

    if (radioButton_creer_alerte_periode_temps->isChecked())
    {
        radioButton_creer_alerte_periode_temps->setChecked(false);
    }
}

void YerothAdminCreateWindow::radioButtons_periode_temps()
{
	dateEdit_creer_alerte_date_fin->setYerothEnabled(true);
	dateEdit_creer_alerte_date_debut->setYerothEnabled(true);

	comboBox_creer_alerte_condition->setEnabled(false);
	lineEdit_creer_alerte_quantite->setEnabled(false);

    if (radioButton_creer_alerte_quantite->isChecked())
    {
        radioButton_creer_alerte_quantite->setChecked(false);
    }
}

void YerothAdminCreateWindow::handleCurrentChanged()
{
    clear_utilisateur_all_fields();

    clear_localisation_all_fields();

    clear_categorie_all_fields();

    clear_fournisseur_all_fields();

    clear_alerte_all_fields();

    clear_remise_all_fields();

    populateUtilisateurComboBoxes();

    populateAlerteComboBoxes();

    populateLocalisationComboBoxes();

    creer_utilisateur_check_fields_entry();

    creer_categorie_check_fields();

    creer_localisation_check_fields();

    creer_fournisseur_check_fields();

    creer_categorie_check_fields();

    creer_alerte_check_fields_entry();
}

void YerothAdminCreateWindow::hideEvent(QHideEvent * hideEvent)
{
    dateEdit_creer_utilisateur_date_naissance->reset();
    dateEdit_creer_localisation_date_ouverture->reset();
    dateEdit_creer_alerte_date_debut->reset();
    dateEdit_creer_alerte_date_fin->reset();
}

#include "creer-utilisateur.cpp"

#include "creer-localisation.cpp"

#include "creer-categorie.cpp"

#include "creer-fournisseur.cpp"

#include "creer-alerte.cpp"

#include "creer-remise.cpp"

