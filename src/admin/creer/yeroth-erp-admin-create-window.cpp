/*
 * yeroth-erp-admin-create-window.cpp
 *
 *      Author: PR. XAVIER NOUMBISSI NOUNDOU
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
    pushButton_creer_departements_de_produits_annuler->enable(this, SLOT(annuler()));
    pushButton_creer_categorie_annuler->enable(this, SLOT(annuler()));
    pushButton_creer_compte_bancaire_annuler->enable(this, SLOT(annuler()));
    pushButton_creer_alerte_annuler->enable(this, SLOT(annuler()));
    pushButton_creer_remise_annuler->enable(this, SLOT(annuler()));


    pushButton_creer_utilisateur_valider->enable(this, SLOT(creer_utilisateur()));
    pushButton_creer_localisation_valider->enable(this, SLOT(creer_localisation()));
    pushButton_creer_departements_de_produits_valider->enable(this, SLOT(creer_departements_de_produits()));
    pushButton_creer_categorie_valider->enable(this, SLOT(creer_categorie()));
    pushButton_creer_compte_bancaire_valider->enable(this, SLOT(creer_compte_bancaire()));
    pushButton_creer_alerte_valider->enable(this, SLOT(creer_alerte()));
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


    connect(lineEdit_creer_alerte_designation, SIGNAL(textChanged(const QString &)), this,
            SLOT(showProduitInfo(const QString &)));

    connect(comboBox_creer_alerte_destinataire, SIGNAL(currentTextChanged(const QString &)), this,
            SLOT(showDestinataireNomComplet(const QString &)));


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
	lineEdit_creer_remise_montant->setYerothEnabled(false);

	lineEdit_creer_localisation_adresse_ip->
		setValidator(&YerothUtils::STRING_FOR_YEROTH_ERP_3_0_IP_ADDRESS_VALIDATOR);

	lineEdit_creer_remise_montant->setValidator(&YerothUtils::POSITIVE_DoubleValidator);
    lineEdit_creer_alerte_quantite->setValidator(&YerothUtils::IntValidator);
    lineEdit_creer_utilisateur_mot_passe_1->setEchoMode(QLineEdit::Password);
    lineEdit_creer_utilisateur_verification->setEchoMode(QLineEdit::Password);

    connect(lineEdit_creer_remise_designation_article, SIGNAL(textChanged(const QString &)), this,
            SLOT(showProduitInfo(const QString &)));
}

void YerothAdminCreateWindow::setupDateTimeEdits()
{
    dateEdit_creer_utilisateur_date_naissance->setStartDate(GET_CURRENT_DATE);
    dateEdit_creer_localisation_date_ouverture->setStartDate(GET_CURRENT_DATE);

    dateEdit_creer_alerte_date_debut->setStartDate(GET_CURRENT_DATE);
    dateEdit_creer_alerte_date_fin->setStartDate(GET_CURRENT_DATE);

    dateEdit_creer_remise_date_debut->setStartDate(GET_CURRENT_DATE);
    dateEdit_creer_remise_date_fin->setStartDate(GET_CURRENT_DATE);
}

void YerothAdminCreateWindow::rendreVisible(unsigned selectedSujetAction)
{
	tabWidget_creer->setCurrentIndex(selectedSujetAction);

    clear_utilisateur_all_fields();

    clear_departements_de_produits_all_fields();

    clear_categorie_all_fields();

    clear_compte_bancaire_all_fields();

    clear_localisation_all_fields();

    clear_alerte_all_fields();

    clear_remise_all_fields();

	lineEdit_creer_alerte_designation->setupMyStaticQCompleter(YerothDatabase::STOCKS,
												  	  	  	   YerothDatabaseTableColumn::DESIGNATION,
															   false);

    lineEdit_creer_remise_designation_article->setupMyStaticQCompleter(YerothDatabase::STOCKS,
    																   YerothDatabaseTableColumn::DESIGNATION);

    lineEdit_creer_utilisateur_localisation->setYerothEnabled(false);

    lineEdit_creer_utilisateur_localisation->setText(_allWindows->getInfoEntreprise().getLocalisation());

    populateUtilisateurComboBoxes();

    populateRemiseComboBoxes();

    populateAlerteComboBoxes();

    populateLocalisationComboBoxes();

    creer_utilisateur_check_fields_entry();

    creer_departements_de_produits_check_fields();

    creer_categorie_check_fields();

    creer_compte_bancaire_check_fields();

    creer_localisation_check_fields();

    creer_alerte_check_fields_entry();

    creer_remise_check_fields_entry();

    setVisible(true);
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

    QString strQuery(QString("SELECT %1 FROM %2 WHERE %3 = '%4'")
    					.arg(YerothDatabaseTableColumn::NOM_COMPLET,
    						YerothDatabase::USERS,
    						 YerothDatabaseTableColumn::NOM_UTILISATEUR,
							 destinataireId));

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

    double stock_dalerte = 0.0;

    double quantite_en_stock = 0.0;

    QString sqlNomProduit(YerothUtils::prepareSqlStr(nomProduit));

    QString strQuery(QString("SELECT %1 FROM %2 WHERE %3 = '%4'")
    					.arg(YerothDatabaseTableColumn::QUANTITE_TOTALE,
    						YerothDatabase::STOCKS,
    						 YerothDatabaseTableColumn::DESIGNATION,
							 sqlNomProduit));

    QSqlQuery query(strQuery);
    QSqlRecord rec = query.record();

    while (query.next())
    {
        quantite_en_stock += query.value(0).toDouble();
    }

    query.clear();

    strQuery = QString("SELECT %1 FROM %2 WHERE %3 = '%4'")
    					.arg(YerothDatabaseTableColumn::STOCK_DALERTE,
    						YerothDatabase::STOCKS,
    						 YerothDatabaseTableColumn::DESIGNATION,
							 sqlNomProduit);

    if (query.exec(strQuery))
    {
        rec = query.record();

        if (query.next())
        {
            stock_dalerte = query.value(0).toDouble();
        }
    }

    switch (tabWidget_creer->currentIndex())
    {
    case SUJET_ACTION_ALERTE:
    	lineEdit_creer_alerte_quantite_en_stock->setText(QString::number(quantite_en_stock, 'f', 0));
    	lineEdit_creer_alerte_stock_dalerte->setText(QString::number(stock_dalerte, 'f', 0));
        break;

    case SUJET_ACTION_REMISE:
    	lineEdit_creer_remise_quantite_en_stock->setText(QString::number(quantite_en_stock, 'f', 0));
        break;

    default:
        break;
    }

    showDatePeremption(nomProduit);
}

void YerothAdminCreateWindow::showDatePeremption(const QString & nomProduit)
{
    _logger->log("showDatePeremption", QString("designation: %1").arg(nomProduit));

    QString sqlNomProduit(YerothUtils::prepareSqlStr(nomProduit));

    QString strQuery(QString("SELECT %1 FROM %2 WHERE %3 = '%4'")
    					.arg(YerothDatabaseTableColumn::DATE_PEREMPTION,
    						YerothDatabase::STOCKS,
    						 YerothDatabaseTableColumn::DESIGNATION,
							 sqlNomProduit));

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

    switch (tabWidget_creer->currentIndex())
    {
    case SUJET_ACTION_ALERTE:
    	lineEdit_creer_alerte_date_peremption->setText(date_peremption.toString(YerothUtils::DATE_FORMAT));
        break;

    case SUJET_ACTION_REMISE:
    	lineEdit_creer_remise_date_peremption->setText(date_peremption.toString(YerothUtils::DATE_FORMAT));
        break;

    default:
        break;
    }
}

void YerothAdminCreateWindow::lister()
{
    _allWindows->_adminListerWindow->rendreVisible(tabWidget_creer->currentIndex());
    rendreInvisible();
}

void YerothAdminCreateWindow::annuler()
{
    clear_utilisateur_all_fields();

    clear_departements_de_produits_all_fields();

    clear_categorie_all_fields();

    clear_compte_bancaire_all_fields();

    clear_localisation_all_fields();

    clear_alerte_all_fields();

    clear_remise_all_fields();

    _allWindows->_adminWindow->rendreVisible();

    rendreInvisible();
}

void YerothAdminCreateWindow::radioButtons_quantite()
{
	dateEdit_creer_alerte_date_fin->setYerothEnabled(false);
	dateEdit_creer_alerte_date_debut->setYerothEnabled(false);

	comboBox_creer_alerte_condition->setEnabled(true);
	lineEdit_creer_alerte_quantite->setYerothEnabled(true);

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
	lineEdit_creer_alerte_quantite->setYerothEnabled(false);

    if (radioButton_creer_alerte_quantite->isChecked())
    {
        radioButton_creer_alerte_quantite->setChecked(false);
    }
}

void YerothAdminCreateWindow::handleCurrentChanged()
{
    clear_utilisateur_all_fields();

    clear_localisation_all_fields();

    clear_departements_de_produits_all_fields();

    clear_categorie_all_fields();

    clear_compte_bancaire_all_fields();

    clear_alerte_all_fields();

    clear_remise_all_fields();

    populateUtilisateurComboBoxes();

    populateAlerteComboBoxes();

    populateLocalisationComboBoxes();

    creer_utilisateur_check_fields_entry();

    creer_departements_de_produits_check_fields();

    creer_categorie_check_fields();

    creer_compte_bancaire_check_fields();

    creer_localisation_check_fields();

    creer_alerte_check_fields_entry();

    creer_remise_check_fields_entry();
}

void YerothAdminCreateWindow::hideEvent(QHideEvent * hideEvent)
{
    dateEdit_creer_utilisateur_date_naissance->reset();
    dateEdit_creer_localisation_date_ouverture->reset();
    dateEdit_creer_alerte_date_debut->reset();
    dateEdit_creer_alerte_date_fin->reset();
    dateEdit_creer_remise_date_debut->reset();
    dateEdit_creer_remise_date_fin->reset();
}

#include "creer-utilisateur.cpp"

#include "creer-localisation.cpp"

#include "creer-departements-de-produits.cpp"

#include "creer-categorie.cpp"

#include "creer-alerte.cpp"

#include "creer-remise.cpp"

#include "creer-compte-bancaire.cpp"

