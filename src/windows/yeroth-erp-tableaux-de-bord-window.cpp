/*
 * yeroth-erp-tableaux-de-bord-window.cpp
 *
 *      Author: Xavier NOUMBISSI NOUNDOU, Dipl.-Inf., Ph.D. (ABD)
 */
#include "yeroth-erp-tableaux-de-bord-window.hpp"

#include "src/yeroth-erp-windows.hpp"

#include "src/users/yeroth-erp-users.hpp"

#include "src/process/yeroth-erp-process.hpp"

#include "src/utils/yeroth-erp-logger.hpp"
#include "src/utils/yeroth-erp-config.hpp"
#include "src/utils/yeroth-erp-utils.hpp"

#include <unistd.h>

#include <QtCore/QtMath>

#include <QtCore/QPair>

#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>

#ifdef YEROTH_FRANCAIS_LANGUAGE
		const QString YerothTableauxDeBordWindow::OPERATION_GENERER("générer les");
		const QString YerothTableauxDeBordWindow::QUALITE_MEILLEURS("les chiffres d'affaires les plus élevés");
		const QString YerothTableauxDeBordWindow::QUALITE_DERNIERS("les chiffres d'affaires les moins élevés");
		const QString YerothTableauxDeBordWindow::OBJET_ARTICLES("articles");
		const QString YerothTableauxDeBordWindow::OBJET_CATEGORIES("catégories");
		const QString YerothTableauxDeBordWindow::OBJET_CAISSIERS("caissiers");
		const QString YerothTableauxDeBordWindow::OBJET_CLIENTS("clients");
		const QString YerothTableauxDeBordWindow::OBJET_FOURNISSEURS("fournisseurs");
		const QString YerothTableauxDeBordWindow::GRAPHE_BAR_CHART("diagramme en bandes");
		const QString YerothTableauxDeBordWindow::GRAPHE_PIE_CHART("diagramme circulaire");
#endif

#ifdef YEROTH_ENGLISH_LANGUAGE
		const QString YerothTableauxDeBordWindow::OPERATION_GENERER("generate the");
		const QString YerothTableauxDeBordWindow::QUALITE_MEILLEURS("best business turnover");
		const QString YerothTableauxDeBordWindow::QUALITE_DERNIERS("least business turnover");
		const QString YerothTableauxDeBordWindow::OBJET_ARTICLES("products");
		const QString YerothTableauxDeBordWindow::OBJET_CATEGORIES("categories");
		const QString YerothTableauxDeBordWindow::OBJET_CAISSIERS("cashiers");
		const QString YerothTableauxDeBordWindow::OBJET_CLIENTS("customers");
		const QString YerothTableauxDeBordWindow::OBJET_FOURNISSEURS("suppliers");
		const QString YerothTableauxDeBordWindow::GRAPHE_BAR_CHART("bar chart");
		const QString YerothTableauxDeBordWindow::GRAPHE_PIE_CHART("pie chart");
#endif

#ifdef YEROTH_FRANCAIS_LANGUAGE
		const QString YerothTableauxDeBordWindow::OPERATION_GENERER_BILAN_COMPTABLE("générer le bilan comptable");
		const QString YerothTableauxDeBordWindow::OPERATION_GENERER_CHIFFRE_DAFFAIRE("générer le chiffre d'affaire");
#endif

#ifdef YEROTH_ENGLISH_LANGUAGE
		const QString YerothTableauxDeBordWindow::OPERATION_GENERER_BILAN_COMPTABLE("generate financial accounting report");
		const QString YerothTableauxDeBordWindow::OPERATION_GENERER_CHIFFRE_DAFFAIRE("generate the business turnover");
#endif

const QString YerothTableauxDeBordWindow::QUANTITE_2("2");
const QString YerothTableauxDeBordWindow::QUANTITE_3("3");
const QString YerothTableauxDeBordWindow::QUANTITE_4("4");
const QString YerothTableauxDeBordWindow::QUANTITE_5("5");
const QString YerothTableauxDeBordWindow::QUANTITE_6("6");
const QString YerothTableauxDeBordWindow::QUANTITE_7("7");
const QString YerothTableauxDeBordWindow::QUANTITE_8("8");
const QString YerothTableauxDeBordWindow::QUANTITE_9("9");

# ifdef YEROTH_FRANCAIS_LANGUAGE

		const QString YerothTableauxDeBordWindow::MOIS_1(MONTH_YEROTH_ERP_3_0_JANUARY_FR);
		const QString YerothTableauxDeBordWindow::MOIS_2(MONTH_YEROTH_ERP_3_0_FEBRUARY_FR);
		const QString YerothTableauxDeBordWindow::MOIS_3(MONTH_YEROTH_ERP_3_0_MARCH_FR);
		const QString YerothTableauxDeBordWindow::MOIS_4(MONTH_YEROTH_ERP_3_0_APRIL_FR);
		const QString YerothTableauxDeBordWindow::MOIS_5(MONTH_YEROTH_ERP_3_0_MAY_FR);
		const QString YerothTableauxDeBordWindow::MOIS_6(MONTH_YEROTH_ERP_3_0_JUNE_FR);
		const QString YerothTableauxDeBordWindow::MOIS_7(MONTH_YEROTH_ERP_3_0_JULY_FR);
		const QString YerothTableauxDeBordWindow::MOIS_8(MONTH_YEROTH_ERP_3_0_AUGUST_FR);
		const QString YerothTableauxDeBordWindow::MOIS_9(MONTH_YEROTH_ERP_3_0_SEPTEMBER_FR);
		const QString YerothTableauxDeBordWindow::MOIS_10(MONTH_YEROTH_ERP_3_0_OCTOBER_FR);
		const QString YerothTableauxDeBordWindow::MOIS_11(MONTH_YEROTH_ERP_3_0_NOVEMBER_FR);
		const QString YerothTableauxDeBordWindow::MOIS_12(MONTH_YEROTH_ERP_3_0_DECEMBER_FR);

# elif YEROTH_ENGLISH_LANGUAGE

		const QString YerothTableauxDeBordWindow::MOIS_1(MONTH_YEROTH_ERP_3_0_JANUARY_EN);
		const QString YerothTableauxDeBordWindow::MOIS_2(MONTH_YEROTH_ERP_3_0_FEBRUARY_EN);
		const QString YerothTableauxDeBordWindow::MOIS_3(MONTH_YEROTH_ERP_3_0_MARCH_EN);
		const QString YerothTableauxDeBordWindow::MOIS_4(MONTH_YEROTH_ERP_3_0_APRIL_EN);
		const QString YerothTableauxDeBordWindow::MOIS_5(MONTH_YEROTH_ERP_3_0_MAY_EN);
		const QString YerothTableauxDeBordWindow::MOIS_6(MONTH_YEROTH_ERP_3_0_JUNE_EN);
		const QString YerothTableauxDeBordWindow::MOIS_7(MONTH_YEROTH_ERP_3_0_JULY_EN);
		const QString YerothTableauxDeBordWindow::MOIS_8(MONTH_YEROTH_ERP_3_0_AUGUST_EN);
		const QString YerothTableauxDeBordWindow::MOIS_9(MONTH_YEROTH_ERP_3_0_SEPTEMBER_EN);
		const QString YerothTableauxDeBordWindow::MOIS_10(MONTH_YEROTH_ERP_3_0_OCTOBER_EN);
		const QString YerothTableauxDeBordWindow::MOIS_11(MONTH_YEROTH_ERP_3_0_NOVEMBER_EN);
		const QString YerothTableauxDeBordWindow::MOIS_12(MONTH_YEROTH_ERP_3_0_DECEMBER_EN);

# endif

const unsigned int YerothTableauxDeBordWindow::MAX_YEARS_REPORTS(10);

const double YerothTableauxDeBordWindow::STATS_MIN_VALUE(0.0009);

const QString YerothTableauxDeBordWindow::_WINDOW_TITLE(QString(QObject::trUtf8("%1 - %2"))
        .arg(YEROTH_ERP_WINDOW_TITLE, QObject::trUtf8("tableaux de bords")));

YerothTableauxDeBordWindow::YerothTableauxDeBordWindow()
    :YerothWindowsCommons(YerothTableauxDeBordWindow::_WINDOW_TITLE),
     _logger(new YerothLogger("YerothRapportsWindow")),
     _csvFileItemSize(0),
     _startYear(0),
     _curStocksVenduTableModel(&_allWindows->getSqlTableModel_stocks_vendu())
{
    setupUi(this);

    this->mySetupUi(this);

    QMESSAGE_BOX_STYLE_SHEET = QString("QMessageBox {background-color: rgb(%1);}"
                                       "QMessageBox QLabel {color: rgb(%2);}")
                               .arg(COLOUR_RGB_STRING_YEROTH_INDIGO_83_0_125,
                                    COLOUR_RGB_STRING_YEROTH_WHITE_255_255_255);

    setupDateTimeEdits_COMPARAISON_DES_CHIFFRES_DAFFAIRES();

    setupDateTimeEdits_BILAN_COMPTABLE();

    actionDeconnecter_utilisateur->setEnabled(false);
    actionAlertes->setEnabled(false);
    actionMenu->setEnabled(false);
    actionAdministration->setEnabled(false);
    actionQui_suis_je->setEnabled(false);

    pushButton_reinitialiser->disable(this);
    pushButton_generer->disable(this);

    pushButton_chiffre_affaire_generer->disable(this);
    pushButton_chiffre_affaire_reinitialiser->disable(this);

    _moisToNombre[MOIS_1] = 1;
    _moisToNombre[MOIS_2] = 2;
    _moisToNombre[MOIS_3] = 3;
    _moisToNombre[MOIS_4] = 4;
    _moisToNombre[MOIS_5] = 5;
    _moisToNombre[MOIS_6] = 6;
    _moisToNombre[MOIS_7] = 7;
    _moisToNombre[MOIS_8] = 8;
    _moisToNombre[MOIS_9] = 9;
    _moisToNombre[MOIS_10] = 10;
    _moisToNombre[MOIS_11] = 11;
    _moisToNombre[MOIS_12] = 12;

    setupTab_EVOLUTION_DU_CHIFFRE_DAFFAIRE();

    setupTab_COMPARAISON_DES_CHIFFRES_DAFFAIRES();

    setupTab_BILAN_COMPTABLE();

    // Menu actions
    connect( actionChanger_utilisateur, SIGNAL( triggered() ), this, SLOT( changer_utilisateur() ) );
    connect( actionAppeler_aide, SIGNAL(triggered()), this, SLOT(help()));
    connect( actionDeconnecter_utilisateur, SIGNAL(triggered()), this, SLOT(deconnecter_utilisateur()));
    connect( actionMenu, SIGNAL( triggered() ), this, SLOT( menu() ) );
    connect( actionFermeture, SIGNAL( triggered() ), this, SLOT( fermeture() ) );
    connect( actionA_propos, SIGNAL( triggered() ), this, SLOT( apropos() ) );
    connect( actionAlertes, SIGNAL(triggered()), this, SLOT(alertes()) );
    connect( actionInformationEntreprise, SIGNAL( triggered() ), this, SLOT( infosEntreprise() ) );
    connect( actionQui_suis_je, SIGNAL(triggered()), this, SLOT(qui_suis_je()) );


    changeLineEditEvolutionObjetsTextSetup(YerothTableauxDeBordWindow::OBJET_ARTICLES);


    connect( comboBox_evolution_objets,
    		 SIGNAL(currentTextChanged(const QString &)),
    		 this,
			 SLOT(changeLineEditEvolutionObjetsTextSetup(const QString &)));


#ifdef YEROTH_CLIENT
    actionAdministration->setEnabled(false);
#else
    //YEROTH_SERVER, YEROTH_STANDALONE, YEROTH_ACADEMIC_EVALUSATION_VERSION
    connect( actionAdministration, SIGNAL(triggered()), this, SLOT(administration()) );
#endif


    connect(tabWidget_rapports, SIGNAL(currentChanged(int)), this, SLOT(handleTabChanged(int)));

    handleTabChanged(SUJET_ACTION_BUSINESS_TURNOVER_PROGRESS);


    setupShortcuts();
}

YerothTableauxDeBordWindow::~YerothTableauxDeBordWindow ()
{
    delete _logger;
}


void YerothTableauxDeBordWindow::handleTabChanged(int index)
{
	actionGenererPDF->disconnect();
	actionReinitialiserRecherche->disconnect();

    if (index == SUJET_ACTION_BUSINESS_TURNOVER_PROGRESS)
    {
    	connect( actionGenererPDF, SIGNAL(triggered()), this, SLOT(choisirEvolutionDuChiffreDaffaire()) );
    	connect( actionReinitialiserRecherche, SIGNAL(triggered()), this, SLOT(reinitialiser_chiffre_affaire()) );
    }
    else if (index == SUJET_ACTION_BUSINESS_TURNOVER_COMPARISON)
    {
    	connect( actionGenererPDF, SIGNAL(triggered()), this, SLOT(generer()) );
    	connect( actionReinitialiserRecherche, SIGNAL(triggered()), this, SLOT(reinitialiser()) );
    }
}


void YerothTableauxDeBordWindow::setupDateTimeEdits_COMPARAISON_DES_CHIFFRES_DAFFAIRES()
{
    dateEdit_rapports_debut->setStartDate(GET_CURRENT_DATE);
    dateEdit_rapports_fin->setStartDate(GET_CURRENT_DATE);
}


void YerothTableauxDeBordWindow::setupDateTimeEdits_BILAN_COMPTABLE()
{
	dateEdit_bilan_comptable_debut->setStartDate(GET_CURRENT_DATE);
	dateEdit_bilan_comptable_fin->setStartDate(GET_CURRENT_DATE);
}


void YerothTableauxDeBordWindow::setupTab_EVOLUTION_DU_CHIFFRE_DAFFAIRE()
{
    comboBox_operations_chiffre->addItem(YerothTableauxDeBordWindow::OPERATION_GENERER_CHIFFRE_DAFFAIRE);

    comboBox_evolution_objets->addItem(YerothTableauxDeBordWindow::OBJET_ARTICLES);
    comboBox_evolution_objets->addItem(YerothTableauxDeBordWindow::OBJET_CAISSIERS);
    comboBox_evolution_objets->addItem(YerothTableauxDeBordWindow::OBJET_CATEGORIES);
    comboBox_evolution_objets->addItem(YerothTableauxDeBordWindow::OBJET_CLIENTS);
    comboBox_evolution_objets->addItem(YerothTableauxDeBordWindow::OBJET_FOURNISSEURS);

    comboBox_mois_debut_chiffre_affaire->addItem(YerothTableauxDeBordWindow::MOIS_1);
    comboBox_mois_debut_chiffre_affaire->addItem(YerothTableauxDeBordWindow::MOIS_2);
    comboBox_mois_debut_chiffre_affaire->addItem(YerothTableauxDeBordWindow::MOIS_3);
    comboBox_mois_debut_chiffre_affaire->addItem(YerothTableauxDeBordWindow::MOIS_4);
    comboBox_mois_debut_chiffre_affaire->addItem(YerothTableauxDeBordWindow::MOIS_5);
    comboBox_mois_debut_chiffre_affaire->addItem(YerothTableauxDeBordWindow::MOIS_6);
    comboBox_mois_debut_chiffre_affaire->addItem(YerothTableauxDeBordWindow::MOIS_7);
    comboBox_mois_debut_chiffre_affaire->addItem(YerothTableauxDeBordWindow::MOIS_8);
    comboBox_mois_debut_chiffre_affaire->addItem(YerothTableauxDeBordWindow::MOIS_9);
    comboBox_mois_debut_chiffre_affaire->addItem(YerothTableauxDeBordWindow::MOIS_10);
    comboBox_mois_debut_chiffre_affaire->addItem(YerothTableauxDeBordWindow::MOIS_11);
    comboBox_mois_debut_chiffre_affaire->addItem(YerothTableauxDeBordWindow::MOIS_12);

    comboBox_mois_fin_chiffre_affaire->addItem(YerothTableauxDeBordWindow::MOIS_1);
    comboBox_mois_fin_chiffre_affaire->addItem(YerothTableauxDeBordWindow::MOIS_2);
    comboBox_mois_fin_chiffre_affaire->addItem(YerothTableauxDeBordWindow::MOIS_3);
    comboBox_mois_fin_chiffre_affaire->addItem(YerothTableauxDeBordWindow::MOIS_4);
    comboBox_mois_fin_chiffre_affaire->addItem(YerothTableauxDeBordWindow::MOIS_5);
    comboBox_mois_fin_chiffre_affaire->addItem(YerothTableauxDeBordWindow::MOIS_6);
    comboBox_mois_fin_chiffre_affaire->addItem(YerothTableauxDeBordWindow::MOIS_7);
    comboBox_mois_fin_chiffre_affaire->addItem(YerothTableauxDeBordWindow::MOIS_8);
    comboBox_mois_fin_chiffre_affaire->addItem(YerothTableauxDeBordWindow::MOIS_9);
    comboBox_mois_fin_chiffre_affaire->addItem(YerothTableauxDeBordWindow::MOIS_10);
    comboBox_mois_fin_chiffre_affaire->addItem(YerothTableauxDeBordWindow::MOIS_11);
    comboBox_mois_fin_chiffre_affaire->addItem(YerothTableauxDeBordWindow::MOIS_12);
}


void YerothTableauxDeBordWindow::setupTab_COMPARAISON_DES_CHIFFRES_DAFFAIRES()
{
    comboBox_operations->addItem(YerothTableauxDeBordWindow::OPERATION_GENERER);

    comboBox_quantite->addItem(YerothTableauxDeBordWindow::QUANTITE_2);
    comboBox_quantite->addItem(YerothTableauxDeBordWindow::QUANTITE_3);
    comboBox_quantite->addItem(YerothTableauxDeBordWindow::QUANTITE_4);
    comboBox_quantite->addItem(YerothTableauxDeBordWindow::QUANTITE_5);
    comboBox_quantite->addItem(YerothTableauxDeBordWindow::QUANTITE_6);
    comboBox_quantite->addItem(YerothTableauxDeBordWindow::QUANTITE_7);
    comboBox_quantite->addItem(YerothTableauxDeBordWindow::QUANTITE_8);
    comboBox_quantite->addItem(YerothTableauxDeBordWindow::QUANTITE_9);

    comboBox_qualite->addItem(YerothTableauxDeBordWindow::QUALITE_MEILLEURS);
    comboBox_qualite->addItem(YerothTableauxDeBordWindow::QUALITE_DERNIERS);

    comboBox_objets->addItem(YerothTableauxDeBordWindow::OBJET_ARTICLES);
    comboBox_objets->addItem(YerothTableauxDeBordWindow::OBJET_CAISSIERS);
    comboBox_objets->addItem(YerothTableauxDeBordWindow::OBJET_CATEGORIES);
    comboBox_objets->addItem(YerothTableauxDeBordWindow::OBJET_CLIENTS);
    comboBox_objets->addItem(YerothTableauxDeBordWindow::OBJET_FOURNISSEURS);

    comboBox_type_graphes->addItem(YerothTableauxDeBordWindow::GRAPHE_BAR_CHART);
    comboBox_type_graphes->addItem(YerothTableauxDeBordWindow::GRAPHE_PIE_CHART);
}


void YerothTableauxDeBordWindow::setupTab_BILAN_COMPTABLE()
{
	comboBox_bilan_comptable_operation->addItem(YerothTableauxDeBordWindow::OPERATION_GENERER_BILAN_COMPTABLE);
}


void YerothTableauxDeBordWindow::rendreVisible(YerothSqlTableModel *stocksTableModel)
{
    _curStocksTableModel = stocksTableModel;

    _curStocksVenduTableModel = &_allWindows->getSqlTableModel_stocks_vendu();

    QString previousYearStr = comboBox_annee_chiffre_affaire->currentText();

    if (_startYear != YerothERPConfig::annee_depart_rapports_chiffre_affaire_value.toUInt())
    {
        _startYear = YerothERPConfig::annee_depart_rapports_chiffre_affaire_value.toUInt();

        comboBox_annee_chiffre_affaire->clear();

        for (unsigned int k = 0; k < MAX_YEARS_REPORTS; ++k)
        {
            comboBox_annee_chiffre_affaire->addItem(QString::number(_startYear + k));
        }
    }

    unsigned int previousYearIdx = comboBox_annee_chiffre_affaire->findText(previousYearStr);

    if (-1 != comboBox_annee_chiffre_affaire->findText(previousYearStr))
    {
        comboBox_annee_chiffre_affaire->setCurrentIndex(previousYearIdx);
    }

    this->setVisible(true);
}


void YerothTableauxDeBordWindow::definirCaissier()
{
    _logger->log("definirCaissier");

    actionDeconnecter_utilisateur->setEnabled(false);
    actionAlertes->setEnabled(false);
    actionMenu->setEnabled(false);
    actionQui_suis_je->setEnabled(false);

    /** Tab One *************************************/

    pushButton_chiffre_affaire_generer->disable(this);
    pushButton_chiffre_affaire_reinitialiser->disable(this);

    /** Tab Two *************************************/

    pushButton_reinitialiser->disable(this);
    pushButton_generer->disable(this);

    /** Tab Three *************************************/

    pushButton_bilan_comptable_reinitialiser->disable(this);
    pushButton_bilan_comptable_generer->disable(this);
}

void YerothTableauxDeBordWindow::definirManager()
{
    _logger->log("definirManager");

    actionDeconnecter_utilisateur->setEnabled(true);
    actionAlertes->setEnabled(true);
    actionMenu->setEnabled(true);
    actionAdministration->setEnabled(true);

    actionQui_suis_je->setEnabled(true);

    /** Tab One *************************************/

    pushButton_chiffre_affaire_generer->enable(this, SLOT( choisirEvolutionDuChiffreDaffaire() ));
    pushButton_chiffre_affaire_reinitialiser->enable(this, SLOT( reinitialiser_chiffre_affaire() ));

    /** Tab Two *************************************/

    pushButton_reinitialiser->enable(this, SLOT(reinitialiser()));
    pushButton_generer->enable(this, SLOT(generer()));

    /** Tab Three *************************************/

    pushButton_bilan_comptable_reinitialiser->enable(this, SLOT(reinitialiser_bilan_comptable()));
    pushButton_bilan_comptable_generer->enable(this, SLOT(bilanComptable()));
}


void YerothTableauxDeBordWindow::definirVendeur()
{
    _logger->log("definirVendeur");

    actionDeconnecter_utilisateur->setEnabled(false);
    actionAlertes->setEnabled(false);
    actionMenu->setEnabled(false);
    actionAdministration->setEnabled(false);
    actionQui_suis_je->setEnabled(false);

    /** Tab One *************************************/

    pushButton_reinitialiser->disable(this);
    pushButton_generer->disable(this);

    /** Tab Two *************************************/

    pushButton_chiffre_affaire_generer->disable(this);
    pushButton_chiffre_affaire_reinitialiser->disable(this);

    /** Tab Three *************************************/

    pushButton_bilan_comptable_reinitialiser->disable(this);
    pushButton_bilan_comptable_generer->disable(this);
}


void YerothTableauxDeBordWindow::definirGestionaireDesStocks()
{
    _logger->log("definirGestionaireDesStocks");

    actionDeconnecter_utilisateur->setEnabled(true);
    actionAlertes->setEnabled(true);
    actionMenu->setEnabled(true);
    actionAdministration->setEnabled(false);

    actionQui_suis_je->setEnabled(true);

    /** Tab One *************************************/

    pushButton_chiffre_affaire_generer->disable(this);
    pushButton_chiffre_affaire_reinitialiser->disable(this);

    /** Tab Two *************************************/

    pushButton_reinitialiser->enable(this, SLOT(reinitialiser()));
    pushButton_generer->enable(this, SLOT(generer()));

    /** Tab Three *************************************/

    pushButton_bilan_comptable_reinitialiser->disable(this);
    pushButton_bilan_comptable_generer->disable(this);
}


void YerothTableauxDeBordWindow::definirMagasinier()
{
    _logger->log("definirMagasinier");

    actionDeconnecter_utilisateur->setEnabled(false);
    actionAlertes->setEnabled(false);
    actionMenu->setEnabled(false);
    actionAdministration->setEnabled(false);
    actionQui_suis_je->setEnabled(false);

    /** Tab One *************************************/

    pushButton_chiffre_affaire_generer->disable(this);
    pushButton_chiffre_affaire_reinitialiser->disable(this);

    /** Tab Two *************************************/

    pushButton_reinitialiser->disable(this);
    pushButton_generer->disable(this);

    /** Tab Three *************************************/

    pushButton_bilan_comptable_reinitialiser->disable(this);
    pushButton_bilan_comptable_generer->disable(this);
}

void YerothTableauxDeBordWindow::definirPasDeRole()
{
    _logger->log("definirPasDeRole");

    actionDeconnecter_utilisateur->setEnabled(false);
    actionAlertes->setEnabled(false);
    actionMenu->setEnabled(false);
    actionAdministration->setEnabled(false);
    actionQui_suis_je->setEnabled(false);

    /** Tab One *************************************/

    pushButton_reinitialiser->disable(this);
    pushButton_generer->disable(this);

    /** Tab Two *************************************/

    pushButton_chiffre_affaire_generer->disable(this);
    pushButton_chiffre_affaire_reinitialiser->disable(this);

    /** Tab Three *************************************/

    pushButton_bilan_comptable_reinitialiser->disable(this);
    pushButton_bilan_comptable_generer->disable(this);
}

void YerothTableauxDeBordWindow::reinitialiser()
{
    _logger->log("reinitialiser");

    comboBox_quantite->resetYerothComboBox();
    comboBox_objets->resetYerothComboBox();
    comboBox_qualite->resetYerothComboBox();
    comboBox_type_graphes->resetYerothComboBox();

    dateEdit_rapports_debut->reset();
    dateEdit_rapports_fin->reset();
}

void YerothTableauxDeBordWindow::reinitialiser_chiffre_affaire()
{
    _logger->log("reinitialiser_chiffre_affaire");

    lineEdit_evolution_objets_value->myClear();

    comboBox_operations_chiffre->resetYerothComboBox();

    comboBox_mois_debut_chiffre_affaire->resetYerothComboBox();
    comboBox_mois_fin_chiffre_affaire->resetYerothComboBox();
    comboBox_annee_chiffre_affaire->resetYerothComboBox();
}


void YerothTableauxDeBordWindow::reinitialiser_bilan_comptable()
{
    _logger->log("reinitialiser_bilan_comptable");

    dateEdit_bilan_comptable_debut->reset();
    dateEdit_bilan_comptable_fin->reset();
}


void YerothTableauxDeBordWindow::meilleursStats(QString fileName,
        										QString fieldId,
												int size)
{
    _logger->log("meilleursStats");

    QString strQuery(QString("SELECT %1, (round(%2, 2) - round(%3, 2)) FROM %4 WHERE %5 >= '%6' AND %7 <= '%8'")
    					.arg(fieldId,
    						 YerothDatabaseTableColumn::MONTANT_TOTAL_VENTE,
    						 YerothDatabaseTableColumn::MONTANT_TVA,
							 _allWindows->STOCKS_VENDU,
							 YerothDatabaseTableColumn::DATE_VENTE,
							 DATE_TO_DB_FORMAT_STRING(dateEdit_rapports_debut->date()),
							 YerothDatabaseTableColumn::DATE_VENTE,
							 DATE_TO_DB_FORMAT_STRING(dateEdit_rapports_fin->date())));

    QSqlQuery query;

    int querySize = YerothUtils::execQuery(query, strQuery, _logger);

    //qDebug() << "++ strQuery: " << strQuery;

    //qDebug() << "++ querySize: " << querySize;//query.executedQuery();

    QString fieldIdValue;
    double montant_total_vente = 0.0;
    double total = 0.0;

    QList<YerothStatsItem *> nomEntrepriseFournisseurToVentes;

    if (querySize > 0)
    {
        while(query.next())
        {
            fieldIdValue = query.value(0).toString();
            montant_total_vente = query.value(1).toDouble();

            int idx = -1;
            for(int i = 0; i < nomEntrepriseFournisseurToVentes.size(); ++i)
            {
                if (YerothUtils::isEqualCaseInsensitive(nomEntrepriseFournisseurToVentes.value(i)->_itemName, fieldIdValue))
                {
                    idx = i;
                    break;
                }
            }

            if (-1 == idx)
            {
                YerothStatsItem *aNewItem = new YerothStatsItem(fieldIdValue, montant_total_vente);
                nomEntrepriseFournisseurToVentes.push_back(aNewItem);

                total += montant_total_vente;
            }
            else
            {
                YerothStatsItem *anItem = nomEntrepriseFournisseurToVentes.value(idx);
                anItem->_itemValue += montant_total_vente;

                total += montant_total_vente;
            }
        }
    }

    qSort(nomEntrepriseFournisseurToVentes.begin(), nomEntrepriseFournisseurToVentes.end(), YerothStatsItem::lessThan);

    QString csvFileContent;
    QString label;
    QString curValueStr;
    QString pourcentageStr;

    _reportTexFileEndString.clear();

    _reportTexFileEndString.append(YerothUtils::LATEX_IN_OUT_handleForeignAccents(
                                       QString(QObject::trUtf8("D\\'etails en %1:"))
                                       	   .arg(YerothERPConfig::currency)))
    					   .prepend("\\textbf{").append("}\n");

    _reportTexFileEndString.append("\\begin{enumerate}[1)]\n");

    _csvFileItemSize = 0;

    double pourcentage = 0;
    double curValue = 0;

    for(int j = nomEntrepriseFournisseurToVentes.size() - 1, k = 0; j > -1 && k < size; --j, ++k)
    {
        curValue = nomEntrepriseFournisseurToVentes.value(j)->_itemValue;

        curValueStr.clear();
        curValueStr.append(GET_CURRENCY_STRING_NUM(curValue));

        pourcentage = (curValue / total) * 100.0;
        pourcentageStr = QString::number(pourcentage, 'f', 3);

        /*qDebug() << QString("++ value: %1, name %2, total: %3, pourcentage: %4")
        				.arg(QString::number(caissierToVentes.value(j)->_itemValue, 'f', 9),
        					 caissierToVentes.value(j)->_itemName,
        					 QString::number(total, 'f', 9),
        					 QString::number(pourcentage, 'f', 9));*/

        label.clear();
        label.append(QString("\"%1\"")
        				.arg(YerothUtils::LATEX_IN_OUT_handleForeignAccents(nomEntrepriseFournisseurToVentes.at(j)->_itemName)));

        csvFileContent.prepend(QString("%1, %2\n")
        						.arg(label, pourcentageStr));

        _reportTexFileEndString.append(QString("\\item %1: %2\n")
        									.arg(label,
        										 YerothUtils::LATEX_IN_OUT_handleForeignAccents(curValueStr)));

        //qDebug() << "++ reportTexFileEndString: " << _reportTexFileEndString;

        pourcentage = 0;
        ++_csvFileItemSize;
    }

    _reportTexFileEndString.append("\\end{enumerate}");

    //qDebug() << "++ test, _csvFileItemSize: " << caissierToVentes.size();

    csvFileContent.prepend("Nom, Total\n");

    //clean up
    for (int k = 0; k < nomEntrepriseFournisseurToVentes.size(); ++k)
    {
        delete nomEntrepriseFournisseurToVentes.value(k);
    }

    QFile csvFile(fileName);

    if (!csvFile.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        _logger->log("meilleursStats", csvFile.errorString());
    }
    else
    {
        QTextStream outCsvFile(&csvFile);

        outCsvFile << csvFileContent;

        //qDebug() << QString("\t%1: \n%2").arg(fileName, csvFileContent);

        csvFile.close();
    }
}


void YerothTableauxDeBordWindow::derniersStats(QString fileName, QString fieldId, int size)
{
    _logger->log("derniersStats");

    QString strQuery(QString("SELECT %1, (round(%2, 2) - round(%3, 2)) FROM %4 WHERE %5 >= '%6' AND %7 <= '%8'")
    					.arg(fieldId,
    						 YerothDatabaseTableColumn::MONTANT_TOTAL_VENTE,
    						 YerothDatabaseTableColumn::MONTANT_TVA,
							 _allWindows->STOCKS_VENDU,
							 YerothDatabaseTableColumn::DATE_VENTE,
							 DATE_TO_DB_FORMAT_STRING(dateEdit_rapports_debut->date()),
							 YerothDatabaseTableColumn::DATE_VENTE,
							 DATE_TO_DB_FORMAT_STRING(dateEdit_rapports_fin->date())));

    QSqlQuery query;

    int querySize = YerothUtils::execQuery(query, strQuery, _logger);

    //qDebug() << "++ query: " << query.executedQuery() << ", querySize: " << querySize;

    QString fieldIdValue;

    double montant_total_vente = 0.0;
    double total = 0.0;

    QList<YerothStatsItem *> caissierToVentes;

    if (querySize > 0)
    {
        while(query.next())
        {
            fieldIdValue = query.value(0).toString();
            montant_total_vente = query.value(1).toDouble();

            if (fieldIdValue.isEmpty())
            {
                continue;
            }

            int idx = -1;
            for(int i = 0; i < caissierToVentes.size(); ++i)
            {
                if ( YerothUtils::isEqualCaseInsensitive(caissierToVentes.value(i)->_itemName, fieldIdValue))
                {
                    idx = i;
                    break;
                }
            }

            if (-1 == idx)
            {
                YerothStatsItem *aNewItem = new YerothStatsItem(fieldIdValue, montant_total_vente);
                caissierToVentes.push_back(aNewItem);

                total += montant_total_vente;
            }
            else
            {
                YerothStatsItem *anItem = caissierToVentes.value(idx);

                anItem->_itemValue += montant_total_vente;

                total += montant_total_vente;//tmpTotal;
            }
        }
    }

    qSort(caissierToVentes.begin(), caissierToVentes.end(), YerothStatsItem::lessThan);

    //Remove all items with a zero value
    for(int j = 0; j < caissierToVentes.size(); ++j)
    {
        if (0 == caissierToVentes.at(j)->_itemValue)
        {
            caissierToVentes.removeAt(j);
        }
    }

    QString csvFileContent;

    csvFileContent.append("Nom, Total\n");

    QString label;
    QString pourcentageStr;
    QString curValueStr;

    double pourcentage = 0;
    double curValue = 0;

    _csvFileItemSize = 0;

    _reportTexFileEndString.clear();

    _reportTexFileEndString.append(YerothUtils::LATEX_IN_OUT_handleForeignAccents(
                                       QString(QObject::trUtf8("D\\'etails en %1:"))
                                       	   .arg(YerothERPConfig::currency)));

    _reportTexFileEndString.prepend("\\textbf{").append("}\n");

    _reportTexFileEndString.append("\\begin{enumerate}[1)]\n");

    for(int j = 0, k = 0; j < caissierToVentes.size() && k < size; ++j)
    {
        curValue = caissierToVentes.value(j)->_itemValue;
        pourcentage = (curValue / total) * 100.0;

        /*qDebug() << QString("++ j:%1. value: %2, total: %3, pourcentage: %4")
        				.arg(QString::number(j, 'f', 2),
        					 QString::number(curValue, 'f', 2),
        					 QString::number(total, 'f', 2),
        					 QString::number(pourcentage, 'f', 2));*/

        //qDebug() << "++ pourcentage: " << QString::number(pourcentage, 'f', 9	);

        if (pourcentage <= STATS_MIN_VALUE)
        {
            continue;
        }

        pourcentageStr.append(QString::number(pourcentage, 'f', 3));

        curValueStr.append(GET_CURRENCY_STRING_NUM(curValue));

        label.append(QString("\"%1\"")
        				.arg(YerothUtils::LATEX_IN_OUT_handleForeignAccents(caissierToVentes.at(j)->_itemName)));

        csvFileContent.append(QString("%1, %2\n")
        						.arg(label, (pourcentage <= 0.001 ? "0.001" : pourcentageStr)));

        _reportTexFileEndString.append(QString("\\item %1: %2\n")
        									.arg(label,
        										 YerothUtils::LATEX_IN_OUT_handleForeignAccents(curValueStr)));

        //qDebug() << "++ reportTexFileEndString: " << _reportTexFileEndString;

        pourcentageStr.clear();
        curValueStr.clear();
        label.clear();
        ++k;
        ++_csvFileItemSize;
    }

    _reportTexFileEndString.append("\\end{enumerate}\n");

    //clean up
    for (int k = 0; k < caissierToVentes.size(); ++k)
    {
        delete caissierToVentes.value(k);
    }

    QFile csvFile(fileName);

    if (!csvFile.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        _logger->log("derniersStats", csvFile.errorString());
    }
    else
    {
        QTextStream outCsvFile(&csvFile);

        outCsvFile << csvFileContent;

        csvFile.close();
    }
}


void YerothTableauxDeBordWindow::rechercher()
{
    _logger->log("rechercher");

    QString objet(comboBox_objets->currentText());

    int size = comboBox_quantite->currentText().toInt();

    QStringList progArguments;
    QString tmpFilePrefix;

    QDir tempDir(YerothERPConfig::temporaryFilesDir);

    QString csvFile;
    QString pdfFileTitle;

#ifdef YEROTH_FRANCAIS_LANGUAGE
    pdfFileTitle.append("Les " + QString::number(size) + " ");
#endif

#ifdef YEROTH_ENGLISH_LANGUAGE
    pdfFileTitle.append("The " + QString::number(size) + " ");
#endif

    if (YerothTableauxDeBordWindow::QUALITE_MEILLEURS == comboBox_qualite->currentText())
    {
        if (YerothTableauxDeBordWindow::OBJET_ARTICLES == objet)
        {
            pdfFileTitle = YerothTableauxDeBordWindow::OBJET_ARTICLES;

#ifdef YEROTH_FRANCAIS_LANGUAGE
            tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("meilleurs-articles");
#endif

#ifdef YEROTH_ENGLISH_LANGUAGE
            tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("best-articles");
#endif

            csvFile = tmpFilePrefix + ".csv";

            csvFile.prepend(YerothERPConfig::temporaryFilesDir + "/");

            tempDir.remove(csvFile);

            statsMeilleursArticles(csvFile, size);
        }
        else if (YerothTableauxDeBordWindow::OBJET_CATEGORIES == objet)
        {
#ifdef YEROTH_FRANCAIS_LANGUAGE
            pdfFileTitle.replace(YerothTableauxDeBordWindow::QUALITE_MEILLEURS, "meilleures");
#endif

#ifdef YEROTH_ENGLISH_LANGUAGE
            pdfFileTitle.replace(YerothTableauxDeBordWindow::QUALITE_MEILLEURS, "best");
#endif

            pdfFileTitle = YerothTableauxDeBordWindow::OBJET_CATEGORIES;

#ifdef YEROTH_FRANCAIS_LANGUAGE
            tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("meilleures-categories");
#endif

#ifdef YEROTH_ENGLISH_LANGUAGE
            tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("best-categories");
#endif

            csvFile = tmpFilePrefix + ".csv";

            csvFile.prepend(YerothERPConfig::temporaryFilesDir + "/");

            tempDir.remove(csvFile);

            statsMeilleursCategories(csvFile, size);
        }
        else if (YerothTableauxDeBordWindow::OBJET_CAISSIERS == objet)
        {
            pdfFileTitle.append(YerothTableauxDeBordWindow::OBJET_CAISSIERS);

#ifdef YEROTH_FRANCAIS_LANGUAGE
            tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("meilleurs-caissiers");
#endif

#ifdef YEROTH_ENGLISH_LANGUAGE
            tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("best-cashiers");
#endif

            csvFile = tmpFilePrefix + ".csv";

            csvFile.prepend(YerothERPConfig::temporaryFilesDir + "/");

            tempDir.remove(csvFile);

            statsMeilleursCaissiers(csvFile, size);
        }
        else if (YerothTableauxDeBordWindow::OBJET_CLIENTS == objet)
        {
            pdfFileTitle.append(YerothTableauxDeBordWindow::OBJET_CLIENTS);

#ifdef YEROTH_FRANCAIS_LANGUAGE
            tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("meilleurs-clients");
#endif

#ifdef YEROTH_ENGLISH_LANGUAGE
            tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("best-customers");
#endif

            csvFile = tmpFilePrefix + ".csv";

            csvFile.prepend(YerothERPConfig::temporaryFilesDir + "/");

            tempDir.remove(csvFile);

            statsMeilleursClients(csvFile, size);
        }
        else if (YerothTableauxDeBordWindow::OBJET_FOURNISSEURS == objet)
        {
            pdfFileTitle.append(YerothTableauxDeBordWindow::OBJET_FOURNISSEURS);

#ifdef YEROTH_FRANCAIS_LANGUAGE
            tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("meilleurs-fournisseurs-ventes");
#endif

#ifdef YEROTH_ENGLISH_LANGUAGE
            tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("best-supplier-sale");
#endif

            csvFile = tmpFilePrefix + ".csv";

            csvFile.prepend(YerothERPConfig::temporaryFilesDir + "/");

            tempDir.remove(csvFile);

            statsMeilleursFournisseursVentes(csvFile, size);
        }

#ifdef YEROTH_FRANCAIS_LANGUAGE
        pdfFileTitle.append(" avec les chiffres d'affaires les plus \\'elev\\'es");
#endif

#ifdef YEROTH_ENGLISH_LANGUAGE
        pdfFileTitle.append(" giving the best financial income");
#endif

    }
    else
    {
        if (YerothTableauxDeBordWindow::OBJET_ARTICLES == objet)
        {
            pdfFileTitle.append(YerothTableauxDeBordWindow::OBJET_ARTICLES);

#ifdef YEROTH_FRANCAIS_LANGUAGE
            tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("derniers-articles");
#endif

#ifdef YEROTH_ENGLISH_LANGUAGE
            tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("last-articles");
#endif

            csvFile = tmpFilePrefix + ".csv";

            csvFile.prepend(YerothERPConfig::temporaryFilesDir + "/");

            tempDir.remove(csvFile);

            statsDerniersArticles(csvFile, size);
        }
        else if (YerothTableauxDeBordWindow::OBJET_CATEGORIES == objet)
        {
#ifdef YEROTH_FRANCAIS_LANGUAGE
            pdfFileTitle.replace(YerothTableauxDeBordWindow::QUALITE_DERNIERS, "derni\\`eres");
#endif

#ifdef YEROTH_ENGLISH_LANGUAGE
            pdfFileTitle.replace(YerothTableauxDeBordWindow::QUALITE_DERNIERS, "last");
#endif

            pdfFileTitle.append(YerothTableauxDeBordWindow::OBJET_CATEGORIES);

#ifdef YEROTH_FRANCAIS_LANGUAGE
            tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("dernieres-categories");
#endif

#ifdef YEROTH_ENGLISH_LANGUAGE
            tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("last-categories");
#endif

            csvFile = tmpFilePrefix + ".csv";

            csvFile.prepend(YerothERPConfig::temporaryFilesDir + "/");

            tempDir.remove(csvFile);

            statsDerniersCategories(csvFile, size);
        }
        else if (YerothTableauxDeBordWindow::OBJET_CAISSIERS == objet)
        {
            pdfFileTitle.append(YerothTableauxDeBordWindow::OBJET_CAISSIERS);

#ifdef YEROTH_FRANCAIS_LANGUAGE
            tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("derniers-caissiers");
#endif

#ifdef YEROTH_ENGLISH_LANGUAGE
            tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("last-cashiers");
#endif

            csvFile = tmpFilePrefix + ".csv";

            csvFile.prepend(YerothERPConfig::temporaryFilesDir  + "/");

            tempDir.remove(csvFile);

            statsDerniersCaissiers(csvFile, size);
        }
        else if (YerothTableauxDeBordWindow::OBJET_CLIENTS == objet)
        {
            pdfFileTitle.append(YerothTableauxDeBordWindow::OBJET_CLIENTS);

#ifdef YEROTH_FRANCAIS_LANGUAGE
            tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("derniers-clients");
#endif

#ifdef YEROTH_ENGLISH_LANGUAGE
            tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("last-customers");
#endif
            csvFile = tmpFilePrefix + ".csv";

            csvFile.prepend(YerothERPConfig::temporaryFilesDir + "/");

            tempDir.remove(csvFile);

            statsDerniersClients(csvFile, size);
        }
        else if (YerothTableauxDeBordWindow::OBJET_CLIENTS == objet)
        {
            pdfFileTitle.append(YerothTableauxDeBordWindow::OBJET_FOURNISSEURS);

#ifdef YEROTH_FRANCAIS_LANGUAGE
            tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("derniers-fournisseurs-ventes");
#endif

#ifdef YEROTH_ENGLISH_LANGUAGE
            tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("last-suppliers-sales");
#endif

            csvFile = tmpFilePrefix + ".csv";

            csvFile.prepend(YerothERPConfig::temporaryFilesDir + "/");

            tempDir.remove(csvFile);

            statsDerniersFournisseursVentes(csvFile, size);
        }

#ifdef YEROTH_FRANCAIS_LANGUAGE
        pdfFileTitle.append(" avec les chiffres d'affaires les moins \\'elev\\'es");
#endif

#ifdef YEROTH_ENGLISH_LANGUAGE
        pdfFileTitle.append(" giving the least financial income");
#endif

    }

    if (this->_csvFileItemSize <= 0)
    {
        QString retMsg(QObject::trUtf8("Il n'y a pas de données correspondante à la requête !\n"
        							   "Vérifier que les dates de début et de fin sont correctes !"));

        YerothQMessageBox::information(this,
                                      QObject::trUtf8("rankings - pas de données !"),
                                      retMsg);
        return ;
    }

    QString latexChartTemplate;
    QString latexChartFileNamePrefix;

#ifdef YEROTH_FRANCAIS_LANGUAGE
    if (YerothTableauxDeBordWindow::GRAPHE_BAR_CHART == comboBox_type_graphes->currentText())
    {
        latexChartTemplate.append(YerothUtils::FR_bar_chart_tex);
        latexChartFileNamePrefix.append(YerothERPConfig::temporaryFilesDir + "/" + tmpFilePrefix + "-bar-chart");
    }
    else if (YerothTableauxDeBordWindow::GRAPHE_PIE_CHART == comboBox_type_graphes->currentText())
    {
        latexChartTemplate.append(YerothUtils::FR_pie_chart_tex);
        latexChartFileNamePrefix.append(YerothERPConfig::temporaryFilesDir + "/" + tmpFilePrefix + "-pie-chart");
    }
#endif

#ifdef YEROTH_ENGLISH_LANGUAGE
    if (YerothTableauxDeBordWindow::GRAPHE_BAR_CHART == comboBox_type_graphes->currentText())
    {
        latexChartTemplate.append(YerothUtils::EN_bar_chart_tex);
        latexChartFileNamePrefix.append(YerothERPConfig::temporaryFilesDir + "/" + tmpFilePrefix + "-bar-chart");
    }
    else if (YerothTableauxDeBordWindow::GRAPHE_PIE_CHART == comboBox_type_graphes->currentText())
    {
        latexChartTemplate.append(YerothUtils::EN_pie_chart_tex);
        latexChartFileNamePrefix.append(YerothERPConfig::temporaryFilesDir + "/" + tmpFilePrefix + "-pie-chart");
    }
#endif

    QString pdfFile(latexChartFileNamePrefix + ".pdf");
    tempDir.remove(pdfFile);

    //qDebug() << "++ csvFile: " << csvFile;

    if (YerothTableauxDeBordWindow::GRAPHE_PIE_CHART == comboBox_type_graphes->currentText())
    {
        QString YEROTHCUTAWAY;

        for(int k = 1; k <= _csvFileItemSize; ++k)
        {
            YEROTHCUTAWAY.append(QString::number(k)).append(",");
            //qDebug() << "++ YEROTHCUTAWAY: " << YEROTHCUTAWAY;
        }

        YEROTHCUTAWAY.remove(YEROTHCUTAWAY.length()-1, 1);

        latexChartTemplate.replace("YEROTHCUTAWAY", YEROTHCUTAWAY);
    }

    YerothInfoEntreprise &infoEntreprise = _allWindows->getInfoEntreprise();

    QString statsDate(YerothUtils::LATEX_IN_OUT_handleForeignAccents(infoEntreprise.getVilleTex()));

    YerothUtils::getCurrentLocaleDate(statsDate);

    //qDebug() << "++ type fact. rapports: " << YerothConfig::typeOfFacturation;

    latexChartTemplate.replace("YEROTHPAPERSPEC", "a4paper");

    latexChartTemplate.replace("YEROTHCSVFILE", 					tmpFilePrefix + ".csv");
    latexChartTemplate.replace("YEROTHCHARTFIN", 				_reportTexFileEndString);

    latexChartTemplate.replace("YEROTHUTILISATEUR", 				_allWindows->getUser()->nom_completTex());
    latexChartTemplate.replace("YEROTHENTREPRISE", 				infoEntreprise.getNomCommercialTex());
    latexChartTemplate.replace("YEROTHBARCHARTTITLE", 			YerothUtils::LATEX_IN_OUT_handleForeignAccents(pdfFileTitle));
    latexChartTemplate.replace("YEROTHENTREPRISE", 				infoEntreprise.getNomCommercialTex());
    latexChartTemplate.replace("YEROTHACTIVITESENTREPRISE", 		infoEntreprise.getSecteursActivitesTex());
    latexChartTemplate.replace("YEROTHBOITEPOSTALE", 			infoEntreprise.getBoitePostal());
    latexChartTemplate.replace("YEROTHVILLE", 					infoEntreprise.getVilleTex());
    latexChartTemplate.replace("YEROTHPAYS", 					infoEntreprise.getPaysTex());
    latexChartTemplate.replace("YEROTHEMAIL", 					infoEntreprise.getEmailTex());
    latexChartTemplate.replace("YEROTHTELEPHONE", 				infoEntreprise.getTelephone());
    latexChartTemplate.replace("YEROTHDATE", 					statsDate);
    latexChartTemplate.replace("YEROTHVENTESDEBUT", 				DATE_TO_STRING(dateEdit_rapports_debut->date()));
    latexChartTemplate.replace("YEROTHVENTESFIN", 				DATE_TO_STRING(dateEdit_rapports_fin->date()));
    latexChartTemplate.replace("YEROTHHEUREGENERATION",			CURRENT_TIME);
    latexChartTemplate.replace("YEROTHCOMPTEBANCAIRENR", 		infoEntreprise.getNumeroCompteBancaire());
    latexChartTemplate.replace("YEROTHCONTRIBUABLENR", 			infoEntreprise.getNumeroDeContribuable());
    latexChartTemplate.replace("YEROTHAGENCECOMPTEBANCAIRE",		infoEntreprise.getAgenceCompteBancaireTex());

    QFile latexChartFile(latexChartFileNamePrefix + ".tex");

    if (latexChartFile.open(QFile::WriteOnly))
    {
        latexChartFile.write(latexChartTemplate.toUtf8());
    }

    latexChartFile.close();


    progArguments << "-interaction";
    progArguments << "nonstopmode";
    progArguments << latexChartFile.fileName();

    //qDebug() << "++ -> " << latexChartFile.fileName();

    if (!progArguments.empty())
    {
        QProcess aProcess;

        aProcess.setWorkingDirectory(YerothERPConfig::temporaryFilesDir);

        aProcess.start(YerothERPConfig::pathToPdfLatex(), progArguments);

        aProcess.waitForFinished(-1);

        progArguments.clear();
        progArguments << pdfFile;

        aProcess.startDetached(YerothERPConfig::pathToPdfReader, progArguments);
        aProcess.waitForFinished();
    }
}


void YerothTableauxDeBordWindow::changeLineEditEvolutionObjetsTextSetup(const QString &comboBoxEvolutionObjetsCurrentText)
{
	//qDebug() << QString("changeLineEditEvolutionObjetsTextSetup: %1")
	//				.arg(comboBoxEvolutionObjetsCurrentText);
	if (YerothUtils::isEqualCaseInsensitive(comboBoxEvolutionObjetsCurrentText, YerothTableauxDeBordWindow::OBJET_ARTICLES))
	{
		lineEdit_evolution_objets_value->setupMyQCompleter(YerothERPWindows::MARCHANDISES, 2, Qt::AscendingOrder, false);
	}
	else if (YerothUtils::isEqualCaseInsensitive(comboBoxEvolutionObjetsCurrentText, YerothTableauxDeBordWindow::OBJET_CAISSIERS))
	{
		lineEdit_evolution_objets_value->setupMyQCompleter(YerothERPWindows::USERS, 3, Qt::AscendingOrder, false);
	}
	else if (YerothUtils::isEqualCaseInsensitive(comboBoxEvolutionObjetsCurrentText, YerothTableauxDeBordWindow::OBJET_CATEGORIES))
	{
		lineEdit_evolution_objets_value->setupMyQCompleter(YerothERPWindows::CATEGORIES, 1, Qt::AscendingOrder, false);
	}
	else if (YerothUtils::isEqualCaseInsensitive(comboBoxEvolutionObjetsCurrentText, YerothTableauxDeBordWindow::OBJET_CLIENTS))
	{
		lineEdit_evolution_objets_value->setupMyQCompleter(YerothERPWindows::CLIENTS, 2, Qt::AscendingOrder, false);
	}
	else if (YerothUtils::isEqualCaseInsensitive(comboBoxEvolutionObjetsCurrentText, YerothTableauxDeBordWindow::OBJET_FOURNISSEURS))
	{
		lineEdit_evolution_objets_value->setupMyQCompleter(YerothERPWindows::FOURNISSEURS, 1, Qt::AscendingOrder, false);
	}
}


void YerothTableauxDeBordWindow::bilanComptable()
{
    _logger->log("bilanComptable");

    if (dateEdit_bilan_comptable_debut->date() > dateEdit_bilan_comptable_fin->date())
    {
        YerothQMessageBox::warning(this, QObject::tr("bilan comptable"),
                                  QObject::trUtf8("La date de 'début' doit être"
                                          " antérieur à la date de 'fin' !"));

        return ;
    }

	QSqlQuery query;

    QString strDetteClientelleQuery(QString("SELECT %1 FROM %2 WHERE %3 < '0'")
    									.arg(YerothDatabaseTableColumn::COMPTE_CLIENT,
    										 _allWindows->CLIENTS,
											 YerothDatabaseTableColumn::COMPTE_CLIENT));

//    qDebug() << QString("++ strAchatsQuery: %1")
//    				.arg(strAchatsQuery);

	int detteClientelleSize = YerothUtils::execQuery(query, strDetteClientelleQuery, _logger);

	double dette_clientelle = 0.0;

	double montant_total_dette_clientelle = 0.0;

    for( int k = 0; k < detteClientelleSize && query.next(); ++k)
    {
    	dette_clientelle = query.value(0).toDouble();

    	montant_total_dette_clientelle = montant_total_dette_clientelle + dette_clientelle;
    }

    montant_total_dette_clientelle = qFabs(montant_total_dette_clientelle);

//    qDebug() << QString("++ detteClientelleSize: %1, montant_total_dette_clientelle: %2")
//    				.arg(QString::number(detteClientelleSize),
//    					 QString::number(montant_total_dette_clientelle, 'f', 2));

    query.clear();

    QString strAchatsQuery(QString("SELECT %1, %2 FROM %3 WHERE %4 >= '%5' AND %6 <= '%7'")
    							.arg(YerothDatabaseTableColumn::PRIX_DACHAT,
    								 YerothDatabaseTableColumn::QUANTITE_TOTALE,
									 _allWindows->ACHATS,
									 YerothDatabaseTableColumn::DATE_ENTREE,
									 DATE_TO_DB_FORMAT_STRING(dateEdit_bilan_comptable_debut->date()),
									 YerothDatabaseTableColumn::DATE_ENTREE,
									 DATE_TO_DB_FORMAT_STRING(dateEdit_bilan_comptable_fin->date())));

//    qDebug() << QString("++ strAchatsQuery: %1")
//    				.arg(strAchatsQuery);

	int achatQuerySize = YerothUtils::execQuery(query, strAchatsQuery, _logger);

	double montant_total_achat = 0.0;

	double total_achat = 0.0;

	double prix_dachat = 0.0;

	double qte_total = 0.0;

    for( int k = 0; k < achatQuerySize && query.next(); ++k)
    {
    	prix_dachat = query.value(0).toDouble();

    	qte_total = query.value(1).toDouble();

    	total_achat = qte_total * prix_dachat;

    	montant_total_achat = montant_total_achat + total_achat;
    }

//    qDebug() << QString("++ achatQuerySize: %1, montant_total_achat: %2")
//    				.arg(QString::number(achatQuerySize),
//    					 QString::number(montant_total_achat, 'f', 2));

    query.clear();

    QString strVentesQuery(QString("SELECT %1, %2, %3, (%4 - %5) FROM stocks_vendu WHERE %6 >= '%7' AND %8 <= '%9'")
    							.arg(YerothDatabaseTableColumn::STOCKS_ID,
    								 YerothDatabaseTableColumn::QUANTITE_VENDUE,
									 YerothDatabaseTableColumn::REMISE_PRIX,
    								 YerothDatabaseTableColumn::MONTANT_TOTAL_VENTE,
									 YerothDatabaseTableColumn::MONTANT_TVA,
									 YerothDatabaseTableColumn::DATE_VENTE,
									 DATE_TO_DB_FORMAT_STRING(dateEdit_bilan_comptable_debut->date()),
									 YerothDatabaseTableColumn::DATE_VENTE,
									 DATE_TO_DB_FORMAT_STRING(dateEdit_bilan_comptable_fin->date())));

//    qDebug() << QString("++ strVentesQuery: %1")
//    				.arg(strVentesQuery);

	int ventesQuerySize = YerothUtils::execQuery(query, strVentesQuery, _logger);

	int stocks_id = -1;

	QMap<int, StockQteVendueEtRemiseTotalSurVente *> stocksidToqtevendue;

	double qte_vendue = 0.0;

	double remise_prix_vente = 0.0;

	double total_remise = 0.0;

	double total_vente = 0.0;

    double montant_total_vente = 0.0;

    for( int k = 0; k < ventesQuerySize && query.next(); ++k)
    {
    	stocks_id = query.value(0).toDouble();

    	qte_vendue = query.value(1).toDouble();

    	remise_prix_vente = query.value(2).toDouble();

    	total_remise = qte_vendue * remise_prix_vente;

    	total_vente = query.value(3).toDouble();

    	if (stocksidToqtevendue.contains(stocks_id))
    	{
    		StockQteVendueEtRemiseTotalSurVente *curValue = stocksidToqtevendue.value(stocks_id);

    		curValue->_qteVendue = curValue->_qteVendue + qte_vendue;
    		curValue->_remiseTotalSurVentes = curValue->_remiseTotalSurVentes + total_remise;

    		stocksidToqtevendue.insert(stocks_id, curValue);
    	}
    	else
    	{
    		StockQteVendueEtRemiseTotalSurVente *curValue = new StockQteVendueEtRemiseTotalSurVente;

    		curValue->_qteVendue = qte_vendue;
    		curValue->_remiseTotalSurVentes = curValue->_remiseTotalSurVentes + total_remise;

    		stocksidToqtevendue.insert(stocks_id, curValue);
    	}

    	montant_total_vente = montant_total_vente + total_vente;
    }

//    qDebug() << QString("++ ventesQuerySize: %1, montant_total_vente: %2")
//    				.arg(QString::number(ventesQuerySize),
//    					 QString::number(montant_total_vente, 'f', 2));

    query.clear();

    QString strVersementsQuery(QString("SELECT %1 FROM %2 WHERE %3 >= '%4' AND %5 <= '%6'")
    							.arg(YerothDatabaseTableColumn::MONTANT_PAYE,
    								 _allWindows->PAIEMENTS,
									 YerothDatabaseTableColumn::DATE_PAIEMENT,
									 DATE_TO_DB_FORMAT_STRING(dateEdit_bilan_comptable_debut->date()),
									 YerothDatabaseTableColumn::DATE_PAIEMENT,
									 DATE_TO_DB_FORMAT_STRING(dateEdit_bilan_comptable_fin->date())));

//    qDebug() << QString("++ strVersementsQuery: %1")
//    				.arg(strVersementsQuery);

	int versementsQuerySize = YerothUtils::execQuery(query, strVersementsQuery, _logger);

	double montant_paye = 0.0;

    double montant_total_versements = 0.0;

    for( int k = 0; k < versementsQuerySize && query.next(); ++k)
    {
    	montant_paye = query.value(0).toDouble();

    	montant_total_versements = montant_total_versements + montant_paye;
    }

//    qDebug() << QString("++ versementsQuerySize: %1, montant_total_versements: %2")
//    				.arg(QString::number(versementsQuerySize),
//    					 QString::number(montant_total_versements, 'f', 2));

    query.clear();

    double total_entrees = montant_total_vente + montant_total_versements;

    double total_sorties = montant_total_achat + montant_total_dette_clientelle;

    double balance = 0.0;
    double benefice_sur_vente_effectuees = 0.0;
    double chiffre_daffaire = 0.0;

    QMapIterator<int, StockQteVendueEtRemiseTotalSurVente *> it(stocksidToqtevendue);

    int querySize = -1;
    QString strQuery;

    while (it.hasNext())
    {
    	it.next();

        int stocks_id = it.key();
        StockQteVendueEtRemiseTotalSurVente *curValue = it.value();

        strQuery = QString("SELECT %1 FROM %2 WHERE %3 = '%4'")
        				.arg(YerothDatabaseTableColumn::MARGE_BENEFICIAIRE,
        					 _allWindows->ACHATS,
    						 YerothDatabaseTableColumn::STOCKS_ID,
							 QString::number(stocks_id));

    	int querySize = YerothUtils::execQuery(query, strQuery, _logger);

    	if (querySize > 0 && query.next())
    	{
    		double marge_beneficiaire = query.value(0).toDouble();

    		benefice_sur_vente_effectuees = benefice_sur_vente_effectuees + (curValue->_qteVendue * marge_beneficiaire);

    		benefice_sur_vente_effectuees = benefice_sur_vente_effectuees - curValue->_remiseTotalSurVentes;
    	}

    	delete curValue;
    }

    benefice_sur_vente_effectuees = benefice_sur_vente_effectuees - montant_total_dette_clientelle;

    if (benefice_sur_vente_effectuees <= 0)
    {
    	benefice_sur_vente_effectuees = 0;
    }

    chiffre_daffaire = montant_total_vente;

    balance = total_entrees - total_sorties;

//    qDebug() << QString("++ benefice: %1, chiffre_daffaire: %2, balance: %3")
//    				.arg(QString::number(benefice, 'f', 2),
//    					 QString::number(chiffre_daffaire, 'f', 2),
//    					 QString::number(balance, 'f', 2));

    QString latexFileNamePrefix("yeroth-erp-bilan-comptable");

#ifdef YEROTH_ENGLISH_LANGUAGE
    latexFileNamePrefix.clear();
    latexFileNamePrefix.append("yeroth-erp-financial-accounting-report");
#endif

    QString texDocument;

    YerothUtils::getLatexFinancialAccountingReportData(texDocument);

    YerothInfoEntreprise & infoEntreprise = YerothUtils::getAllWindows()->getInfoEntreprise();

    QString fileDate(YerothUtils::LATEX_IN_OUT_handleForeignAccents(infoEntreprise.getVilleTex()));

    YerothUtils::getCurrentLocaleDate(fileDate);

    QString bilanComptableDateDebut;
    QString bilanComptableDateFin;

#ifdef YEROTH_FRANCAIS_LANGUAGE
    bilanComptableDateDebut = QString("'%1'")
                    .arg(YerothUtils::LATEX_IN_OUT_handleForeignAccents(YerothUtils::frenchLocale.toString(dateEdit_bilan_comptable_debut->date())));

    bilanComptableDateFin = QString("'%1'")
                  .arg(YerothUtils::LATEX_IN_OUT_handleForeignAccents(YerothUtils::frenchLocale.toString(dateEdit_bilan_comptable_fin->date())));
#endif

#ifdef YEROTH_ENGLISH_LANGUAGE
    bilanComptableDateDebut = QString("'%1'")
                    .arg(YerothUtils::LATEX_IN_OUT_handleForeignAccents(YerothUtils::englishLocale.toString(dateEdit_bilan_comptable_debut->date())));

    bilanComptableDateFin = QString("'%1'")
                  .arg(YerothUtils::LATEX_IN_OUT_handleForeignAccents(YerothUtils::englishLocale.toString(dateEdit_bilan_comptable_fin->date())));
#endif

    texDocument.replace("YEROTHBILANCOMPTABLEDEBUT", bilanComptableDateDebut);

    texDocument.replace("YEROTHBILANCOMPTABLEFIN", bilanComptableDateFin);

    texDocument.replace("YEROTHBILANCOMPTABLEVENTESDEVISE", GET_CURRENCY_STRING_NUM_FOR_LATEX(montant_total_vente));

    texDocument.replace("YEROTHBILANCOMPTABLEVERSEMENTSCLIENTSDEVISE", GET_CURRENCY_STRING_NUM_FOR_LATEX(montant_total_versements));

    texDocument.replace("YEROTHBILANCOMPTABLETOTALENTREESDEVISE", GET_CURRENCY_STRING_NUM_FOR_LATEX(total_entrees));

    texDocument.replace("YEROTHBILANCOMPTABLEDETTECLIENTELLEDEVISE", GET_CURRENCY_STRING_NUM_FOR_LATEX(montant_total_dette_clientelle));

    texDocument.replace("YEROTHBILANCOMPTABLEACHATSDEVISE", GET_CURRENCY_STRING_NUM_FOR_LATEX(montant_total_achat));

    texDocument.replace("YEROTHBILANCOMPTABLETOTALSORTIESDEVISE", GET_CURRENCY_STRING_NUM_FOR_LATEX(total_sorties));

    QString balanceDeviseLatexStr(GET_CURRENCY_STRING_NUM_FOR_LATEX(balance));
    if (balance > 0)
    {
    	balanceDeviseLatexStr = YerothUtils::colorLatexTextInput("yerothColorGreen", balanceDeviseLatexStr);
    }
    else
    {
    	balanceDeviseLatexStr = YerothUtils::colorLatexTextInput("yerothColorRed", balanceDeviseLatexStr);
    }

    texDocument.replace("YEROTHBILANCOMPTABLEBALANCEDEVISE", balanceDeviseLatexStr);

    texDocument.replace("YEROTHBILANCOMPTABLEBENEFICEDEVISE", GET_CURRENCY_STRING_NUM_FOR_LATEX(benefice_sur_vente_effectuees));

    texDocument.replace("YEROTHBILANCOMPTABLECHIFFREDAFFAIREDEVISE", GET_CURRENCY_STRING_NUM_FOR_LATEX(chiffre_daffaire));

    texDocument.replace("YEROTHPAPERSPEC", "a4paper");
    texDocument.replace("YEROTHENTREPRISE", infoEntreprise.getNomCommercialTex());
    texDocument.replace("YEROTHACTIVITESENTREPRISE", infoEntreprise.getSecteursActivitesTex());
    texDocument.replace("YEROTHBOITEPOSTALE", infoEntreprise.getBoitePostal());
    texDocument.replace("YEROTHVILLE", infoEntreprise.getVilleTex());
    texDocument.replace("YEROTHPAYS", infoEntreprise.getPaysTex());
    texDocument.replace("YEROTHEMAIL", infoEntreprise.getEmailTex());
    texDocument.replace("YEROTHTELEPHONE", infoEntreprise.getTelephone());
    texDocument.replace("YEROTHDATE", fileDate);
    texDocument.replace("YEROTHNOMUTILISATEUR", _allWindows->getUser()->nom_completTex());
    texDocument.replace("YEROTHHEUREDIMPRESSION", CURRENT_TIME);
    texDocument.replace("YEROTHCOMPTEBANCAIRENR", infoEntreprise.getNumeroCompteBancaire());
    texDocument.replace("YEROTHCONTRIBUABLENR", infoEntreprise.getNumeroDeContribuable());
    texDocument.replace("YEROTHAGENCECOMPTEBANCAIRE", infoEntreprise.getAgenceCompteBancaireTex());

    QString yerothPrefixFileName;

    yerothPrefixFileName.append(YerothUtils::getUniquePrefixFileInTemporaryFilesDir(latexFileNamePrefix));

	//qDebug() << "++\n" << texDocument;

    QFile tmpLatexFile(QString("%1tex")
    					.arg(yerothPrefixFileName));

    YerothUtils::writeStringToQFilewithUTF8Encoding(tmpLatexFile, texDocument);

    QString pdfCustomerDataFileName(YerothERPProcess::compileLatex(yerothPrefixFileName));

    if (pdfCustomerDataFileName.isEmpty())
    {
    	return ;
    }

    YerothERPProcess::startPdfViewerProcess(pdfCustomerDataFileName);
}


void YerothTableauxDeBordWindow::analyseComparee()
{
    _logger->log("analyseComparee");

    if (comboBox_mois_debut_chiffre_affaire->currentIndex() >
            comboBox_mois_fin_chiffre_affaire->currentIndex())
    {
        YerothQMessageBox::warning(this, QObject::trUtf8("analyse comparée"),
                                  QObject::trUtf8("Le mois de 'début' doit être"
                                          " antérieur au mois de 'fin' !"));
    }

    //"yyyy-MM-dd"; D.toString(YerothUtils::DB_DATE_FORMAT)

    QString annee = comboBox_annee_chiffre_affaire->currentText();
    QString mois_debut = comboBox_mois_debut_chiffre_affaire->currentText();
    QString mois_fin = comboBox_mois_fin_chiffre_affaire->currentText();

    int moisDebut = _moisToNombre[mois_debut];
    int moisFin = _moisToNombre[mois_fin];

    QString dateDebut;
    dateDebut.append(annee)
    .append("-")
    .append(QString::number(moisDebut))
    .append("-01");

    QDate qDateDebut(annee.toInt(), moisDebut, 1);
    QDate qDateFin;

    QString dateFin;
    bool moisFinAjuster = false;

    if (_moisToNombre[YerothTableauxDeBordWindow::MOIS_12] == moisFin)
    {
        dateFin.append(annee)
        .append("-")
        .append(QString::number(moisFin))
        .append("-31");

        qDateFin.setDate(annee.toInt(), moisFin, 31);
    }
    else
    {
        moisFin += 1;
        moisFinAjuster = true;

        dateFin.append(annee)
        .append("-")
        .append(QString::number(moisFin))
        .append("-01");

        qDateFin.setDate(annee.toInt(), moisFin, 1);
    }

    //qDebug() << "calculerChiffresDaffaireMois\n\t" <<
    //				QString("annee: %1, mois_debut: %2, mois fin ajuster: %3, mois_fin: %4")
    //					.arg(annee,
    //						 QString::number(moisDebut),
    //						 BOOL_TO_STRING(moisFinAjuster),
    //						 QString::number(moisFin));

    QString strAchatQuery;

    strAchatQuery.append(QString("SELECT %1, %2, %3 FROM %4 WHERE %5 >= '%6' AND ")
    						.arg(YerothDatabaseTableColumn::DATE_ENTREE,
    							 YerothDatabaseTableColumn::PRIX_DACHAT,
								 YerothDatabaseTableColumn::QUANTITE_TOTALE,
								 _allWindows->ACHATS,
								 YerothDatabaseTableColumn::DATE_ENTREE,
								 dateDebut));

    if (moisFinAjuster)
    {
    	strAchatQuery.append(QString("%1 < '%2'")
    							.arg(YerothDatabaseTableColumn::DATE_ENTREE,
    								 dateFin));
    }
    else
    {
    	strAchatQuery.append(QString("%1 <= '%2'")
    							.arg(YerothDatabaseTableColumn::DATE_ENTREE,
    								 dateFin));
    }

    //qDebug() << "calculerChiffresDaffaireMois - analyse comparee \n\t" <<
    //				QString("strAchatQuery: %1 | annee: %2, mois_debut: %3, mois fin ajuster: %4, mois_fin: %5")
    //					.arg(strAchatQuery,
    //						 annee,
    //						 QString::number(moisDebut),
    //						 BOOL_TO_STRING(moisFinAjuster),
    //						 QString::number(moisFin));


    QString strQuery(QString("SELECT date_vente, (montant_total_vente - montant_tva) "));

    strQuery.append("FROM ")
    		.append(_allWindows->STOCKS_VENDU)
			.append(" WHERE date_vente >= '")
			.append(dateDebut)
			.append("' AND ");

    if (moisFinAjuster)
    {
        strQuery.append("date_vente < '")
        .append(dateFin)
        .append("'");
    }
    else
    {
        strQuery.append("date_vente <= '")
        .append(dateFin)
        .append("'");
    }

    QString AUCUN_FILTRE(QObject::tr("aucun filtre !"));

    bool filtreActif = false;

    QString yerothFiltre;

    QString databaseTableColumn;

    QString comboBoxEvolutionObjetsCurrentText(comboBox_evolution_objets->currentText());

    QString textFromLineEditEvolutionSujets(lineEdit_evolution_objets_value->text());

	if (YerothUtils::isEqualCaseInsensitive(comboBoxEvolutionObjetsCurrentText, YerothTableauxDeBordWindow::OBJET_CAISSIERS))
	{
		filtreActif = true;

		yerothFiltre.append(YerothUtils::LATEX_IN_OUT_handleForeignAccents(QString("filtre: %1 = %2")
								.arg(YerothTableauxDeBordWindow::OBJET_CAISSIERS,
									 textFromLineEditEvolutionSujets)));

		databaseTableColumn = YerothDatabaseTableColumn::NOM_CAISSIER;
	}
	else if (YerothUtils::isEqualCaseInsensitive(comboBoxEvolutionObjetsCurrentText, YerothTableauxDeBordWindow::OBJET_CATEGORIES))
	{
		filtreActif = true;

		yerothFiltre.append(YerothUtils::LATEX_IN_OUT_handleForeignAccents(QString("filtre: %1 = %2")
								.arg(YerothTableauxDeBordWindow::OBJET_CATEGORIES,
									 textFromLineEditEvolutionSujets)));

		databaseTableColumn = YerothDatabaseTableColumn::CATEGORIE;
	}
	else if (YerothUtils::isEqualCaseInsensitive(comboBoxEvolutionObjetsCurrentText, YerothTableauxDeBordWindow::OBJET_CLIENTS))
	{
		filtreActif = true;

		yerothFiltre.append(QString("filtre: %1 = %2")
								.arg(YerothTableauxDeBordWindow::OBJET_CLIENTS,
									 textFromLineEditEvolutionSujets));

		databaseTableColumn = YerothDatabaseTableColumn::NOM_ENTREPRISE_CLIENT;

	}
	else if (YerothUtils::isEqualCaseInsensitive(comboBoxEvolutionObjetsCurrentText, YerothTableauxDeBordWindow::OBJET_ARTICLES))
	{
		if (textFromLineEditEvolutionSujets.isEmpty())
		{
			filtreActif = false;

			yerothFiltre.append(AUCUN_FILTRE);
		}
		else
		{
			filtreActif = true;

			yerothFiltre.append(QString("filtre: %1 = %2")
									.arg(YerothTableauxDeBordWindow::OBJET_ARTICLES,
										 textFromLineEditEvolutionSujets));

			databaseTableColumn = YerothDatabaseTableColumn::DESIGNATION;
		}
	}
	else if (YerothUtils::isEqualCaseInsensitive(comboBoxEvolutionObjetsCurrentText, YerothTableauxDeBordWindow::OBJET_FOURNISSEURS))
	{

		filtreActif = true;

		yerothFiltre.append(QString("filtre: %1 = %2")
								.arg(YerothTableauxDeBordWindow::OBJET_FOURNISSEURS,
									 textFromLineEditEvolutionSujets));

		databaseTableColumn = YerothDatabaseTableColumn::NOM_ENTREPRISE_FOURNISSEUR;

	}

    //qDebug() << QString("databaseTableColumn: %1, currentTextFromComboBoxEvolutionSujets: %2")
    //				.arg(databaseTableColumn, comboBox_evolution_objets->currentText());



	if (true == filtreActif)
	{
		if (textFromLineEditEvolutionSujets.isEmpty())
		{
	        YerothQMessageBox::warning(this, QObject::trUtf8("Fenêtre des rapports"),
	                                  QObject::trUtf8("Vous devez spécifier un paramètre dans"
	                                          	  	  " le champs de texte 'nom' !"));
			return ;
		}
		else
		{
			if (!databaseTableColumn.isEmpty())
			{
		    	strQuery.append(QString(" AND %1 = '%2'")
		    						.arg(databaseTableColumn,
		    								textFromLineEditEvolutionSujets));
			}
		}
	}

    //qDebug() << "++ query: " << strQuery;

	QSqlQuery achatQuery;

	QSqlQuery query;

	double montant_total_achat = 0.0;

    double montant_total_vente = 0.0;

    int achatQuerySize = YerothUtils::execQuery(achatQuery, strAchatQuery, _logger);

    int querySize = YerothUtils::execQuery(query, strQuery, _logger);

    if (0 >= achatQuerySize || 0 >= querySize)
    {

    	YerothQMessageBox::information(this,
    			QObject::trUtf8("Yeroth-erp-3.0 - rapports - analyse comparée - pas de données !"),
				QObject::trUtf8(("Il n'y a pas de données correspondante à la requête !\n"
								 "Vérifier que les dates de début et de fin, "
								 "ainsi que l'année sont correctes !")));
    	return ;
    }

    QMap<int, double> monthToAchatsTotalAmount;

    QMap<int, double> monthToVentesTotalAmount;

    int curMonth = 0;
    QDate curDate;

    double prix_dachat = 0.0;

    double quantite_total_achat = 0.0;

    double maxAmountAchats = 0.0;
    double maxAmount = 0.0;

    double sommeTotalAchatMois = 0.0;
    double sommeTotalVenteMois = 0.0;

    for(int k = moisDebut; k <= moisFin; ++k)
    {
    	sommeTotalAchatMois += monthToAchatsTotalAmount[k];
    	sommeTotalVenteMois += monthToVentesTotalAmount[k];
    }

    for( int k = 0; k < achatQuerySize && achatQuery.next(); ++k)
    {
        curDate = achatQuery.value(0).toDate();
        curMonth = curDate.month();

        prix_dachat = achatQuery.value(1).toDouble();

        quantite_total_achat = achatQuery.value(2).toDouble();

        montant_total_achat = quantite_total_achat * prix_dachat;

        //qDebug() << "++ curDate: " << curDate
        //		 << ", curMonth: " << curMonth
        //		 << ", montant_total_vente: " << montant_total_vente;

        if (0 != curMonth)
        {
        	monthToAchatsTotalAmount[curMonth] += montant_total_achat;

            if (maxAmountAchats < monthToAchatsTotalAmount[curMonth])
            {
            	maxAmountAchats = monthToAchatsTotalAmount[curMonth];
            }
            /*qDebug() << ", curMonth: " << curMonth
            		 << ", montant_total_vente: " << monthToVentesTotalAmount[curMonth];*/
        }
    }

    for( int k = 0; k < querySize && query.next(); ++k)
    {
        curDate = query.value(0).toDate();
        curMonth = curDate.month();

        montant_total_vente = query.value(1).toDouble();

        //qDebug() << "++ curDate: " << curDate
        //		 << ", curMonth: " << curMonth
        //		 << ", montant_total_vente: " << montant_total_vente;

        if (0 != curMonth)
        {
            monthToVentesTotalAmount[curMonth] += montant_total_vente;

            if (maxAmount < monthToVentesTotalAmount[curMonth])
            {
                maxAmount = monthToVentesTotalAmount[curMonth];
            }
            /*qDebug() << ", curMonth: " << curMonth
            		 << ", montant_total_vente: " << monthToVentesTotalAmount[curMonth];*/
        }
    }

    _reportTexFileEndString.clear();

#ifdef YEROTH_FRANCAIS_LANGUAGE
    _reportTexFileEndString.append(YerothUtils::LATEX_IN_OUT_handleForeignAccents(
                                       QString("D\\'etails en %1:")
                                       .arg(YerothERPConfig::currency)));
#endif

#ifdef YEROTH_ENGLISH_LANGUAGE
    _reportTexFileEndString.append(YerothUtils::LATEX_IN_OUT_handleForeignAccents(
                                       QString("Details in %1:")
                                       .arg(YerothERPConfig::currency)));
#endif

    _reportTexFileEndString.prepend("\\textbf{").append("}\n");
    _reportTexFileEndString.append("\\begin{enumerate}[1)]\n");

    //Fill in the PDF file which amount of money for which month.

    if (moisFinAjuster)
    {
        moisFin -= 1;
    }

    for( int k = moisDebut; k <= moisFin; ++k)
    {
        _reportTexFileEndString.append("\\item \\textbf{")

#ifdef YEROTH_FRANCAIS_LANGUAGE
		.append(QString("%1}: ventes $\\rightarrow %2$, achats $\\rightarrow %3$\n")
					.arg(YerothUtils::LATEX_IN_OUT_handleForeignAccents(YerothUtils::frenchLocale.monthName(k)),
						 YerothUtils::LATEX_IN_OUT_handleForeignAccents(GET_CURRENCY_STRING_NUM(monthToVentesTotalAmount[k])),
						 YerothUtils::LATEX_IN_OUT_handleForeignAccents(GET_CURRENCY_STRING_NUM(monthToAchatsTotalAmount[k]))));
#endif

#ifdef YEROTH_ENGLISH_LANGUAGE
		.append(QString("%1}: sales $\\rightarrow %2$, buyings $\\rightarrow %3$\n")
					.arg(YerothUtils::LATEX_IN_OUT_handleForeignAccents(YerothUtils::frenchLocale.monthName(k)),
						 YerothUtils::LATEX_IN_OUT_handleForeignAccents(GET_CURRENCY_STRING_NUM(monthToVentesTotalAmount[k])),
						 YerothUtils::LATEX_IN_OUT_handleForeignAccents(GET_CURRENCY_STRING_NUM(monthToAchatsTotalAmount[k]))));
#endif
    }

    _reportTexFileEndString.append("\\end{enumerate}\n");

    //qDebug() << "++ END";

    QString barItems;

    double ratioAchats = 0;
    double ratio = 0;

    double maxAmountAll = maxAmount + maxAmountAchats;

    const int TICKS = 100;
    const double MAX_RATIO = 900.0;

    for(int k = moisDebut; k <= moisFin; ++k)
    {
    	if (monthToVentesTotalAmount.contains(k))
    	{
    		//qDebug() << QString("month: %1, amount: %2")
    		//						.arg(QString::number(k),
    		//								QString::number(monthToVentesTotalAmount[3], 'f', 2));
    		ratioAchats = (monthToAchatsTotalAmount[k] * MAX_RATIO) / maxAmountAll;

    		ratio = (monthToVentesTotalAmount[k] * MAX_RATIO) / maxAmountAll;

    		//qDebug() << QString("++ mois(k): %1, max amount achats: %2, ratio achat: %3, max amount ventes: %4, ratio vente: %5")
    		//				.arg(QString::number(k),
    		//						QString::number(monthToAchatsTotalAmount[k], 'f', 2),
			//						QString::number(ratioAchats, 'f', 2),
    		//						QString::number(monthToVentesTotalAmount[k], 'f', 2),
			//						QString::number(ratio, 'f', 2));

    		barItems.append(QString("\\baritem{%1}{%2}{gray}\n		\\subbaritem{}{%3}{purplish}\n")
    				.arg(YerothUtils::LATEX_IN_OUT_handleForeignAccents(GET_MONTH_NAME_LOCALIZED(k)),
    						QString::number(ratio, 'f', 2),
							QString::number(ratioAchats, 'f', 2)));
    	}
    }


    //qDebug() << "++ barItems: " << barItems;

    QProcess aProcess;

    aProcess.setWorkingDirectory(YerothERPConfig::temporaryFilesDir);

    QString texDocument;
    texDocument.append(YerothUtils::_1a_tex);

    texDocument.replace("YEROTHBARPERGROUP", "\\barspergroup{2}");
    texDocument.replace("YEROTHBARITEMS", 				barItems);
    texDocument.replace("YEROTHTICKS", 					QString::number(TICKS));

#ifdef YEROTH_FRANCAIS_LANGUAGE
    texDocument.replace("YEROTHLEGENDANALYSECOMPAREE", 	"\\diagLegenditem{Ratio des achats du mois.}{purplish}");
    texDocument.replace("YEROTHDIAGRAMMETITRE", 		"Ratio du chiffre d'affaire du mois.");
    texDocument.replace("YEROTHNIVEAUCHIFFREAFFAIRE", 	"Niveau du chiffre d'affaire");
#endif

#ifdef YEROTH_ENGLISH_LANGUAGE
    texDocument.replace("YEROTHLEGENDANALYSECOMPAREE", 	"\\diagLegenditem{Ratio of the monthly buyings.}{purplish}");
    texDocument.replace("YEROTHDIAGRAMMETITRE", 		"Ratio of the monthly income.");
    texDocument.replace("YEROTHNIVEAUCHIFFREAFFAIRE", 	"Income Level");
#endif

    QString fileName1(YerothERPConfig::temporaryFilesDir + "/1a.tex");

    QFile tmpFile1(fileName1);

    if (tmpFile1.open(QFile::WriteOnly))
    {
        tmpFile1.write(texDocument.toUtf8());
    }
    tmpFile1.close();

    YerothInfoEntreprise &infoEntreprise = _allWindows->getInfoEntreprise();

    QString texDocument2;

#ifdef YEROTH_FRANCAIS_LANGUAGE
    texDocument2.append(YerothUtils::FR_bar_diag_tex);
#endif

#ifdef YEROTH_ENGLISH_LANGUAGE
    texDocument2.append(YerothUtils::EN_bar_diag_tex);
#endif


    QString factureDate(YerothUtils::LATEX_IN_OUT_handleForeignAccents(infoEntreprise.getVilleTex()));
    YerothUtils::getCurrentLocaleDate(factureDate);

    QString longDateDebut;
    QString longDateFin;

#ifdef YEROTH_FRANCAIS_LANGUAGE
    longDateDebut = QString("'%1'")
                    .arg(YerothUtils::LATEX_IN_OUT_handleForeignAccents(YerothUtils::frenchLocale.toString(qDateDebut)));

    longDateFin = QString("'%1'")
                  .arg(YerothUtils::LATEX_IN_OUT_handleForeignAccents(YerothUtils::frenchLocale.toString(qDateFin)));
#endif

#ifdef YEROTH_ENGLISH_LANGUAGE
    longDateDebut = QString("'%1'")
                    .arg(YerothUtils::LATEX_IN_OUT_handleForeignAccents(YerothUtils::englishLocale.toString(qDateDebut)));

    longDateFin = QString("'%1'")
                  .arg(YerothUtils::LATEX_IN_OUT_handleForeignAccents(YerothUtils::englishLocale.toString(qDateFin)));
#endif

    //qDebug() << "++ type fact. rapports - chiffe affaire: " << YerothConfig::typeOfFacturation;


    texDocument2.replace("YEROTHPAPERSPEC", "a4paper");

	if (YerothUtils::isEqualCaseInsensitive(comboBoxEvolutionObjetsCurrentText, "caissiers")  ||
		YerothUtils::isEqualCaseInsensitive(comboBoxEvolutionObjetsCurrentText, "catégories") ||
		YerothUtils::isEqualCaseInsensitive(comboBoxEvolutionObjetsCurrentText, "clients"))
	{
		if (textFromLineEditEvolutionSujets.isEmpty())
		{
			yerothFiltre.clear();
			yerothFiltre.append(QObject::tr("aucun filtre !"));
		}
	}
	else
	{
		yerothFiltre = YerothUtils::LATEX_IN_OUT_handleForeignAccents(yerothFiltre);
	}

	texDocument2.replace("YEROTHMENTION", QObject::trUtf8("[analyse compar\\'ee]"));

    texDocument2.replace("YEROTHFILTRE", yerothFiltre.prepend("''").append("''"));

    texDocument2.replace("YEROTHCHIFFREAFFAIREDATEDEBUT",longDateDebut);
    texDocument2.replace("YEROTHCHIFFREAFFAIREDATEFIN", 	longDateFin);
    texDocument2.replace("YEROTHCHARTFIN", 				_reportTexFileEndString);
    texDocument2.replace("YEROTHENTREPRISE", 			infoEntreprise.getNomCommercialTex());
    texDocument2.replace("YEROTHACTIVITESENTREPRISE", 	infoEntreprise.getSecteursActivitesTex());
    texDocument2.replace("YEROTHBOITEPOSTALE", 			infoEntreprise.getBoitePostal());
    texDocument2.replace("YEROTHVILLE", 					infoEntreprise.getVilleTex());
    texDocument2.replace("YEROTHPAYS", 					infoEntreprise.getPaysTex());
    texDocument2.replace("YEROTHEMAIL", 					infoEntreprise.getEmailTex());
    texDocument2.replace("YEROTHTELEPHONE", 				infoEntreprise.getTelephone());
    texDocument2.replace("YEROTHDATE", 					factureDate);
    texDocument2.replace("YEROTHNOMUTILISATEUR",			_allWindows->getUser()->nom_completTex());
    texDocument2.replace("YEROTHHEUREDIMPRESSION",		CURRENT_TIME);
    texDocument2.replace("YEROTHCOMPTEBANCAIRENR", 		infoEntreprise.getNumeroCompteBancaire());
    texDocument2.replace("YEROTHCONTRIBUABLENR", 		infoEntreprise.getNumeroDeContribuable());
    texDocument2.replace("YEROTHAGENCECOMPTEBANCAIRE",	infoEntreprise.getAgenceCompteBancaireTex());

    texDocument2.replace("1a.tex", fileName1);

#ifdef YEROTH_FRANCAIS_LANGUAGE
    texDocument2.replace("YEROTHTITREDOCUMENT",
                         QString("Diagramme r\\'epr\\'esentatif des chiffres"
                                 " d'affaire du %1 au %2.").arg(longDateDebut, longDateFin));
#endif

#ifdef YEROTH_ENGLISH_LANGUAGE
    texDocument2.replace("YEROTHTITREDOCUMENT",
                         QString("Chart illustrating the income from %1 to %2.")
                         .arg(longDateDebut, longDateFin));
#endif

    //qDebug() << "++ test: " << texDocument2;

    YerothUtils::LATEX_IN_OUT_handleForeignAccents(texDocument2);

    QString fileName(FILE_NAME_USERID_CURRENT_TIME("evolution-chiffre-affaire"));
    fileName.append(".");

    QString tmpFilePrefix(YerothERPConfig::temporaryFilesDir + "/" + fileName);

    QFile tmpFile(tmpFilePrefix + "tex");
    if (tmpFile.open(QFile::WriteOnly))
    {
        tmpFile.write(texDocument2.toUtf8());
    }
    tmpFile.close();

    //qDebug() << "++ tmpFile: " << tmpFile.fileName();

    QStringList progArguments;
    QString texRm(tmpFile.fileName().remove(".tex"));
    progArguments << texRm;

    //qDebug() << "++ file name to latex compile bar diag: " << texRm;

	aProcess.start(YerothERPConfig::pathToLatex(), progArguments);
	aProcess.waitForFinished(-1);

	progArguments.clear();
	progArguments << QString("%1.dvi").arg(texRm);

	aProcess.start(YerothERPConfig::pathToDvips(), progArguments);
	aProcess.waitForFinished(-1);

	progArguments.clear();
	progArguments << QString("%1.ps").arg(texRm);
	progArguments << QString("%1.pdf").arg(texRm);

	aProcess.start(YerothERPConfig::pathToPs2Pdf(), progArguments);
	aProcess.waitForFinished(-1);

    progArguments.clear();
    progArguments << tmpFilePrefix + "pdf";

    aProcess.startDetached(YerothERPConfig::pathToPdfReader, progArguments);

    //emit startPdf(1);
    //qDebug() << "\texit status evince: " << _aProcess->exitStatus();
}


void YerothTableauxDeBordWindow::calculerChiffresDaffaireMois()
{
    _logger->log("calculerChiffresDaffaireMois");

    if (comboBox_mois_debut_chiffre_affaire->currentIndex() >
            comboBox_mois_fin_chiffre_affaire->currentIndex())
    {
        YerothQMessageBox::warning(this, QObject::trUtf8("évolution du chiffre d'affaire"),
                                  QObject::trUtf8("Le mois de 'début' doit être"
                                          " antérieur au mois de 'fin' !"));
    }

    //"yyyy-MM-dd"; D.toString(YerothUtils::DB_DATE_FORMAT)

    QString annee = comboBox_annee_chiffre_affaire->currentText();
    QString mois_debut = comboBox_mois_debut_chiffre_affaire->currentText();
    QString mois_fin = comboBox_mois_fin_chiffre_affaire->currentText();

    int moisDebut = _moisToNombre[mois_debut];
    int moisFin = _moisToNombre[mois_fin];

    QString dateDebut;
    dateDebut.append(annee)
    .append("-")
    .append(QString::number(moisDebut))
    .append("-01");

    QDate qDateDebut(annee.toInt(), moisDebut, 1);
    QDate qDateFin;

    QString dateFin;
    bool moisFinAjuster = false;

    if (_moisToNombre[YerothTableauxDeBordWindow::MOIS_12] == moisFin)
    {
        dateFin.append(annee)
        .append("-")
        .append(QString::number(moisFin))
        .append("-31");

        qDateFin.setDate(annee.toInt(), moisFin, 31);
    }
    else
    {
        moisFin += 1;
        moisFinAjuster = true;

        dateFin.append(annee)
        .append("-")
        .append(QString::number(moisFin))
        .append("-01");

        qDateFin.setDate(annee.toInt(), moisFin, 1);
    }

//    qDebug() << "calculerChiffresDaffaireMois\n\t" <<
//    				QString("annee: %1, mois_debut: %2, mois fin ajuster: %3, mois_fin: %4")
//    					.arg(annee,
//    						 QString::number(moisDebut),
//    						 BOOL_TO_STRING(moisFinAjuster),
//    						 QString::number(moisFin));

    QString strQuery(QString("SELECT date_vente, (montant_total_vente - montant_tva) "));

    strQuery.append("FROM ")
    		.append(_allWindows->STOCKS_VENDU)
			.append(" WHERE date_vente >= '")
			.append(dateDebut)
			.append("' AND ");

    if (moisFinAjuster)
    {
        strQuery.append("date_vente < '")
        .append(dateFin)
        .append("'");
    }
    else
    {
        strQuery.append("date_vente <= '")
        .append(dateFin)
        .append("'");
    }

    bool filtreActif = false;

    QString AUCUN_FILTRE(QObject::tr("aucun filtre !"));

    QString yerothFiltre;

    QString databaseTableColumn;

    QString comboBoxEvolutionObjetsCurrentText(comboBox_evolution_objets->currentText());

    QString textFromLineEditEvolutionSujets(lineEdit_evolution_objets_value->text());

	if (YerothUtils::isEqualCaseInsensitive(comboBoxEvolutionObjetsCurrentText, YerothTableauxDeBordWindow::OBJET_CAISSIERS))
	{
		filtreActif = true;

		yerothFiltre.append(QString("filtre: %1 = %2")
								.arg(YerothTableauxDeBordWindow::OBJET_CAISSIERS,
									 textFromLineEditEvolutionSujets));

		databaseTableColumn = YerothDatabaseTableColumn::NOM_CAISSIER;
	}
	else if (YerothUtils::isEqualCaseInsensitive(comboBoxEvolutionObjetsCurrentText, YerothTableauxDeBordWindow::OBJET_CATEGORIES))
	{
		filtreActif = true;

		yerothFiltre.append(QString("filtre: %1 = %2")
								.arg(YerothTableauxDeBordWindow::OBJET_CATEGORIES,
									 textFromLineEditEvolutionSujets));

		databaseTableColumn = YerothDatabaseTableColumn::CATEGORIE;
	}
	else if (YerothUtils::isEqualCaseInsensitive(comboBoxEvolutionObjetsCurrentText, YerothTableauxDeBordWindow::OBJET_CLIENTS))
	{
		filtreActif = true;

		yerothFiltre.append(QString("filtre: %1 = %2")
								.arg(YerothTableauxDeBordWindow::OBJET_CLIENTS,
								     textFromLineEditEvolutionSujets));

		databaseTableColumn = YerothDatabaseTableColumn::NOM_ENTREPRISE_CLIENT;

	}
	else if (YerothUtils::isEqualCaseInsensitive(comboBoxEvolutionObjetsCurrentText, YerothTableauxDeBordWindow::OBJET_ARTICLES))
	{
		if (textFromLineEditEvolutionSujets.isEmpty())
		{
			filtreActif = false;

			yerothFiltre.append(AUCUN_FILTRE);
		}
		else
		{
			filtreActif = true;

			yerothFiltre.append(QString("filtre: %1 = %2")
									.arg(YerothTableauxDeBordWindow::OBJET_ARTICLES,
									 	 textFromLineEditEvolutionSujets));

			databaseTableColumn = YerothDatabaseTableColumn::DESIGNATION;
		}
	}
	else if (YerothUtils::isEqualCaseInsensitive(comboBoxEvolutionObjetsCurrentText, YerothTableauxDeBordWindow::OBJET_FOURNISSEURS))
	{

		filtreActif = true;

		yerothFiltre.append(QString("filtre: %1 = %2")
								.arg(YerothTableauxDeBordWindow::OBJET_FOURNISSEURS,
									 textFromLineEditEvolutionSujets));

		databaseTableColumn = YerothDatabaseTableColumn::NOM_ENTREPRISE_FOURNISSEUR;

	}

    //qDebug() << QString("databaseTableColumn: %1, currentTextFromComboBoxEvolutionSujets: %2")
    //				.arg(databaseTableColumn, comboBox_evolution_objets->currentText());



	if (true == filtreActif)
	{
		if (textFromLineEditEvolutionSujets.isEmpty())
		{
	        YerothQMessageBox::warning(this, QObject::trUtf8("Fenêtre des rapports"),
	                                  QObject::trUtf8("Vous devez spécifier un paramètre dans"
	                                          	  	  " le champs de texte 'nom' !"));
			return ;
		}
		else
		{
			if (!databaseTableColumn.isEmpty())
			{
		    	strQuery.append(QString(" AND %1 = '%2'")
		    						.arg(databaseTableColumn,
		    								textFromLineEditEvolutionSujets));
			}
		}
	}


    //qDebug() << "++ query: " << strQuery;

	QSqlQuery query;

    double montant_total_vente = 0.0;

    int querySize = YerothUtils::execQuery(query, strQuery, _logger);

    if (0 >= querySize)
    {

        YerothQMessageBox::information(this,
                                      QObject::trUtf8("évolution du chiffre d'affaire - pas de données !"),
                                      QObject::trUtf8(("Il n'y a pas de données correspondante à la requête !\n"
                                              	  	   "Vérifier que les dates de début et de fin, "
                                              	  	   "ainsi que l'année sont correctes !")));
        return ;
    }

    QMap<int, double> monthToVentesTotalAmount;

    int curMonth = 0;
    QDate curDate;

    double maxAmount = 0.0;

    for( int k = 0; k < querySize && query.next(); ++k)
    {
        curDate = query.value(0).toDate();
        curMonth = curDate.month();

        montant_total_vente = query.value(1).toDouble();

        //qDebug() << "++ curDate: " << curDate
        //		 << ", curMonth: " << curMonth
        //		 << ", montant_total_vente: " << montant_total_vente;

        if (0 != curMonth)
        {
            monthToVentesTotalAmount[curMonth] += montant_total_vente;

            if (maxAmount < monthToVentesTotalAmount[curMonth])
            {
                maxAmount = monthToVentesTotalAmount[curMonth];
            }
            /*qDebug() << ", curMonth: " << curMonth
            		 << ", montant_total_vente: " << monthToVentesTotalAmount[curMonth];*/
        }
    }

    _reportTexFileEndString.clear();

#ifdef YEROTH_FRANCAIS_LANGUAGE
    _reportTexFileEndString.append(YerothUtils::LATEX_IN_OUT_handleForeignAccents(
                                       QString("D\\'etails en %1:")
                                       .arg(YerothERPConfig::currency)));
#endif

#ifdef YEROTH_ENGLISH_LANGUAGE
    _reportTexFileEndString.append(YerothUtils::LATEX_IN_OUT_handleForeignAccents(
                                       QString("Details in %1:")
                                       .arg(YerothERPConfig::currency)));
#endif

    _reportTexFileEndString.prepend("\\textbf{").append("}\n");
    _reportTexFileEndString.append("\\begin{enumerate}[1)]\n");

    //Fill in the PDF file which amount of money for which month.

    if (moisFinAjuster)
    {
        moisFin -= 1;
    }

    for( int k = moisDebut; k <= moisFin; ++k)
    {
        _reportTexFileEndString.append("\\item ")
#ifdef YEROTH_FRANCAIS_LANGUAGE
        .append(YerothUtils::LATEX_IN_OUT_handleForeignAccents(YerothUtils::frenchLocale.monthName(k)))
#endif

#ifdef YEROTH_ENGLISH_LANGUAGE
        .append(YerothUtils::LATEX_IN_OUT_handleForeignAccents(YerothUtils::englishLocale.monthName(k)))
#endif
        .append(": $")
        .append(YerothUtils::LATEX_IN_OUT_handleForeignAccents(GET_CURRENCY_STRING_NUM(monthToVentesTotalAmount[k])))
        .append("$\n");
    }

    _reportTexFileEndString.append("\\end{enumerate}\n");

    //qDebug() << "++ END";

    QString barItems;

    double ratio = 0;

    double sommeTotalMois = 0.0;

    const int TICKS = 100;
    const double MAX_RATIO = 900.0;

    for(int k = moisDebut; k <= moisFin; ++k)
    {
    	sommeTotalMois += monthToVentesTotalAmount[k];
    }

    for(int k = moisDebut; k <= moisFin; ++k)
    {
    	if (monthToVentesTotalAmount.contains(k))
    	{
    		ratio = (monthToVentesTotalAmount[k] * MAX_RATIO) / sommeTotalMois;

    		//qDebug() << QString("++ mois(k): %1, max amount: %2, ratio: %3")
    		//				.arg(QString::number(k),
    		//						QString::number(monthToVentesTotalAmount[k], 'f', 2),
    		//						QString::number(ratio, 'f', 2));

    		barItems.append(QString("\\baritem{%1}{%2}{gray}\n")
    				.arg(YerothUtils::LATEX_IN_OUT_handleForeignAccents(GET_MONTH_NAME_LOCALIZED(k)),
    						QString::number(ratio, 'f', 2)));
    	}
    }

    //qDebug() << "++ barItems: " << barItems;

    QProcess aProcess;

    aProcess.setWorkingDirectory(YerothERPConfig::temporaryFilesDir);

    QString texDocument;
    texDocument.append(YerothUtils::_1a_tex);


    texDocument.replace("YEROTHBARPERGROUP", "");
    texDocument.replace("YEROTHBARITEMS", 				barItems);
    texDocument.replace("YEROTHTICKS", 					QString::number(TICKS));

#ifdef YEROTH_FRANCAIS_LANGUAGE
    texDocument.replace("YEROTHLEGENDANALYSECOMPAREE", 	"");
    texDocument.replace("YEROTHDIAGRAMMETITRE", 		"Ratio du chiffre d'affaire du mois.");
    texDocument.replace("YEROTHNIVEAUCHIFFREAFFAIRE", 	"Niveau du chiffre d'affaire");
#endif

#ifdef YEROTH_ENGLISH_LANGUAGE
    texDocument.replace("YEROTHLEGENDANALYSECOMPAREE", 	"");
    texDocument.replace("YEROTHDIAGRAMMETITRE", 			"Ratio of the monthly income.");
    texDocument.replace("YEROTHNIVEAUCHIFFREAFFAIRE", 	"Income Level");
#endif

    QString fileName1(YerothERPConfig::temporaryFilesDir + "/1a.tex");

    QFile tmpFile1(fileName1);

    if (tmpFile1.open(QFile::WriteOnly))
    {
        tmpFile1.write(texDocument.toUtf8());
    }
    tmpFile1.close();

    YerothInfoEntreprise &infoEntreprise = _allWindows->getInfoEntreprise();

    QString texDocument2;

#ifdef YEROTH_FRANCAIS_LANGUAGE
    texDocument2.append(YerothUtils::FR_bar_diag_tex);
#endif

#ifdef YEROTH_ENGLISH_LANGUAGE
    texDocument2.append(YerothUtils::EN_bar_diag_tex);
#endif


    QString factureDate(YerothUtils::LATEX_IN_OUT_handleForeignAccents(infoEntreprise.getVilleTex()));
    YerothUtils::getCurrentLocaleDate(factureDate);

    QString longDateDebut;
    QString longDateFin;

#ifdef YEROTH_FRANCAIS_LANGUAGE
    longDateDebut = QString("'%1'")
                    .arg(YerothUtils::LATEX_IN_OUT_handleForeignAccents(YerothUtils::frenchLocale.toString(qDateDebut)));

    longDateFin = QString("'%1'")
                  .arg(YerothUtils::LATEX_IN_OUT_handleForeignAccents(YerothUtils::frenchLocale.toString(qDateFin)));
#endif

#ifdef YEROTH_ENGLISH_LANGUAGE
    longDateDebut = QString("'%1'")
                    .arg(YerothUtils::LATEX_IN_OUT_handleForeignAccents(YerothUtils::englishLocale.toString(qDateDebut)));

    longDateFin = QString("'%1'")
                  .arg(YerothUtils::LATEX_IN_OUT_handleForeignAccents(YerothUtils::englishLocale.toString(qDateFin)));
#endif

    //qDebug() << "++ type fact. rapports - chiffe affaire: " << YerothConfig::typeOfFacturation;


    texDocument2.replace("YEROTHPAPERSPEC", "a4paper");

	if (YerothUtils::isEqualCaseInsensitive(comboBoxEvolutionObjetsCurrentText, YerothTableauxDeBordWindow::OBJET_CAISSIERS)  ||
		YerothUtils::isEqualCaseInsensitive(comboBoxEvolutionObjetsCurrentText, YerothTableauxDeBordWindow::OBJET_CATEGORIES) ||
		YerothUtils::isEqualCaseInsensitive(comboBoxEvolutionObjetsCurrentText, YerothTableauxDeBordWindow::OBJET_CLIENTS))
	{
		if (textFromLineEditEvolutionSujets.isEmpty())
		{
			yerothFiltre.clear();
			yerothFiltre.append(QObject::tr("aucun filtre !"));
		}
	}
	else
	{
		yerothFiltre = YerothUtils::LATEX_IN_OUT_handleForeignAccents(yerothFiltre);
	}

	texDocument2.replace("YEROTHMENTION", "");

    texDocument2.replace("YEROTHFILTRE", yerothFiltre.prepend("''").append("''"));

    texDocument2.replace("YEROTHCHIFFREAFFAIREDATEDEBUT",longDateDebut);
    texDocument2.replace("YEROTHCHIFFREAFFAIREDATEFIN", 	longDateFin);
    texDocument2.replace("YEROTHCHARTFIN", 				_reportTexFileEndString);
    texDocument2.replace("YEROTHENTREPRISE", 			infoEntreprise.getNomCommercialTex());
    texDocument2.replace("YEROTHACTIVITESENTREPRISE", 	infoEntreprise.getSecteursActivitesTex());
    texDocument2.replace("YEROTHBOITEPOSTALE", 			infoEntreprise.getBoitePostal());
    texDocument2.replace("YEROTHVILLE", 					infoEntreprise.getVilleTex());
    texDocument2.replace("YEROTHPAYS", 					infoEntreprise.getPaysTex());
    texDocument2.replace("YEROTHEMAIL", 					infoEntreprise.getEmailTex());
    texDocument2.replace("YEROTHTELEPHONE", 				infoEntreprise.getTelephone());
    texDocument2.replace("YEROTHDATE", 					factureDate);
    texDocument2.replace("YEROTHNOMUTILISATEUR",			_allWindows->getUser()->nom_completTex());
    texDocument2.replace("YEROTHHEUREDIMPRESSION",		CURRENT_TIME);
    texDocument2.replace("YEROTHCOMPTEBANCAIRENR", 		infoEntreprise.getNumeroCompteBancaire());
    texDocument2.replace("YEROTHCONTRIBUABLENR", 		infoEntreprise.getNumeroDeContribuable());
    texDocument2.replace("YEROTHAGENCECOMPTEBANCAIRE",	infoEntreprise.getAgenceCompteBancaireTex());

    texDocument2.replace("1a.tex", fileName1);

#ifdef YEROTH_FRANCAIS_LANGUAGE
    texDocument2.replace("YEROTHTITREDOCUMENT",
                         QString("Diagramme r\\'epr\\'esentatif des chiffres"
                                 " d'affaire du %1 au %2.").arg(longDateDebut, longDateFin));
#endif

#ifdef YEROTH_ENGLISH_LANGUAGE
    texDocument2.replace("YEROTHTITREDOCUMENT",
                         QString("Chart illustrating the income from %1 to %2.")
                         .arg(longDateDebut, longDateFin));
#endif

    //qDebug() << "++ test: " << texDocument2;

    YerothUtils::LATEX_IN_OUT_handleForeignAccents(texDocument2);

    QString fileName(FILE_NAME_USERID_CURRENT_TIME("evolution-chiffre-affaire"));
    fileName.append(".");

    QString tmpFilePrefix(YerothERPConfig::temporaryFilesDir + "/" + fileName);

    QFile tmpFile(tmpFilePrefix + "tex");
    if (tmpFile.open(QFile::WriteOnly))
    {
        tmpFile.write(texDocument2.toUtf8());
    }
    tmpFile.close();

    //qDebug() << "++ tmpFile: " << tmpFile.fileName();

    QStringList progArguments;
    QString texRm(tmpFile.fileName().remove(".tex"));
    progArguments << texRm;

    //qDebug() << "++ file name to latex compile bar diag: " << texRm;

	aProcess.start(YerothERPConfig::pathToLatex(), progArguments);
	aProcess.waitForFinished(-1);

	progArguments.clear();
	progArguments << QString("%1.dvi").arg(texRm);

	aProcess.start(YerothERPConfig::pathToDvips(), progArguments);
	aProcess.waitForFinished(-1);

	progArguments.clear();
	progArguments << QString("%1.ps").arg(texRm);
	progArguments << QString("%1.pdf").arg(texRm);

	aProcess.start(YerothERPConfig::pathToPs2Pdf(), progArguments);
	aProcess.waitForFinished(-1);

    progArguments.clear();
    progArguments << tmpFilePrefix + "pdf";

    //qDebug() << "++ test it: " << tmpFilePrefix + "pdf";

    aProcess.startDetached(YerothERPConfig::pathToPdfReader, progArguments);

    //qDebug() << "\texit status evince: " << _aProcess->exitStatus();
}


void YerothTableauxDeBordWindow::choisirEvolutionDuChiffreDaffaire()
{
	if (checkBox_analyse_comparee->isChecked())
	{
		analyseComparee();
	}
	else
	{
		calculerChiffresDaffaireMois();
	}
}


void YerothTableauxDeBordWindow::setupShortcuts()
{
    this->setupShortcutActionMessageDaide 	(*actionAppeler_aide);
    this->setupShortcutActionQuiSuisJe		(*actionQui_suis_je);
}

