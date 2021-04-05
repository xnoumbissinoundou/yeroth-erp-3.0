/*
 * yeroth-erp-tableaux-de-bord-window.cpp
 *
 *      Author: PR. XAVIER NOUMBISSI NOUNDOU
 */
#include "yeroth-erp-tableaux-de-bord-window.hpp"


#include "src/yeroth-erp-windows.hpp"

#include "src/users/yeroth-erp-users.hpp"

#include "src/process/yeroth-erp-process.hpp"

#include "src/utils/yeroth-erp-logger.hpp"

#include "src/utils/yeroth-erp-config.hpp"

#include "src/utils/yeroth-erp-map-COMPLEX-ITEM.hpp"

#include "src/utils/yeroth-erp-utils.hpp"


#include <unistd.h>

#include <QtWidgets/QFileDialog>

#include <QtCore/QDateTime>

#include <QtCore/QMapIterator>

#include <QtCore/QtMath>

#include <QtCore/QPair>

#include <QtSql/QSqlQuery>

#include <QtSql/QSqlError>

#ifdef YEROTH_FRANCAIS_LANGUAGE
		const QString YerothTableauxDeBordWindow::OPERATION_GENERER("générer les");
		const QString YerothTableauxDeBordWindow::QUALITE_PLUS_VENDU_PAR_QUANTITE("avec les quantités les plus vendues");
		const QString YerothTableauxDeBordWindow::QUALITE_MOINS_VENDU_PAR_QUANTITE("avec les quantités les moins vendues");
		const QString YerothTableauxDeBordWindow::QUALITE_MEILLEURS("avec les chiffres d'affaires les plus élevés");
		const QString YerothTableauxDeBordWindow::QUALITE_ZERO("avec les chiffres d'affaires zéro");
		const QString YerothTableauxDeBordWindow::QUALITE_DERNIERS("avec les chiffres d'affaires les moins élevés");
		const QString YerothTableauxDeBordWindow::OBJET_ARTICLES("articles");
		const QString YerothTableauxDeBordWindow::OBJET_CATEGORIES("catégories");
		const QString YerothTableauxDeBordWindow::OBJET_CAISSIERS("caissiers");
		const QString YerothTableauxDeBordWindow::OBJET_CLIENTS("clients");
		const QString YerothTableauxDeBordWindow::OBJET_FOURNISSEURS("fournisseurs");
		const QString YerothTableauxDeBordWindow::OBJET_SERVICES("services");
		const QString YerothTableauxDeBordWindow::GRAPHE_BAR_CHART("diagramme en bandes");
		const QString YerothTableauxDeBordWindow::GRAPHE_PIE_CHART("diagramme circulaire");
#endif

#ifdef YEROTH_ENGLISH_LANGUAGE
		const QString YerothTableauxDeBordWindow::OPERATION_GENERER("generate the");
		const QString YerothTableauxDeBordWindow::QUALITE_PLUS_VENDU_PAR_QUANTITE("highest sold quantity");
		const QString YerothTableauxDeBordWindow::QUALITE_MOINS_VENDU_PAR_QUANTITE("lowest sold quantity");
		const QString YerothTableauxDeBordWindow::QUALITE_MEILLEURS("best business turnover");
		const QString YerothTableauxDeBordWindow::QUALITE_ZERO("zero business turnover");
		const QString YerothTableauxDeBordWindow::QUALITE_DERNIERS("least business turnover");
		const QString YerothTableauxDeBordWindow::OBJET_ARTICLES("products");
		const QString YerothTableauxDeBordWindow::OBJET_CATEGORIES("categories");
		const QString YerothTableauxDeBordWindow::OBJET_CAISSIERS("cashiers");
		const QString YerothTableauxDeBordWindow::OBJET_CLIENTS("customers");
		const QString YerothTableauxDeBordWindow::OBJET_FOURNISSEURS("suppliers");
		const QString YerothTableauxDeBordWindow::OBJET_SERVICES("services");
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


const unsigned int YerothTableauxDeBordWindow::QUANTITE_9 = 9;


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


YerothTableauxDeBordWindow::YerothTableauxDeBordWindow()
    :YerothWindowsCommons(),
     _logger(new YerothLogger("YerothRapportsWindow")),
	 _objetServiceLastIndex(-1),
	 _objetClientLastIndex(-1),
     _csvFileItemSize(0),
     _startYear(0),
     _curStocksVenduTableModel(&_allWindows->getSqlTableModel_stocks_vendu())
{
    _windowName = QString("%1 - %2")
    				.arg(YEROTH_ERP_WINDOW_TITLE,
    					 QObject::trUtf8("tableaux de bords"));

    setupUi(this);

    mySetupUi(this);

    QMESSAGE_BOX_STYLE_SHEET = QString("QMessageBox {background-color: rgb(%1);}"
                                       "QMessageBox QLabel {color: rgb(%2);}")
                            		.arg(COLOUR_RGB_STRING_YEROTH_INDIGO_83_0_125,
                            			 COLOUR_RGB_STRING_YEROTH_WHITE_255_255_255);


    setupDateTimeEdits_EVOLUTION_DES_CHIFFRES_DAFFAIRES();


    setupDateTimeEdits_COMPARAISON_DES_CHIFFRES_DAFFAIRES();


    setupDateTimeEdits_BILAN_COMPTABLE();


    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionExporter_au_format_csv, false);

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


    // Menu actions
    connect( actionExporter_au_format_csv, SIGNAL(triggered()), this, SLOT(export_csv_file()));
    connect( actionChanger_utilisateur, SIGNAL( triggered() ), this, SLOT( changer_utilisateur() ) );
    connect( actionAppeler_aide, SIGNAL(triggered()), this, SLOT(help()));
    connect( actionDeconnecter_utilisateur, SIGNAL(triggered()), this, SLOT(deconnecter_utilisateur()));
    connect( actionMenu, SIGNAL( triggered() ), this, SLOT( menu() ) );
    connect( actionFermeture, SIGNAL( triggered() ), this, SLOT( fermeture() ) );
    connect( actionA_propos, SIGNAL( triggered() ), this, SLOT( apropos() ) );
    connect( actionAlertes, SIGNAL(triggered()), this, SLOT(alertes()) );
    connect( actionInformationEntreprise, SIGNAL( triggered() ), this, SLOT( infosEntreprise() ) );
    connect( actionQui_suis_je, SIGNAL(triggered()), this, SLOT(qui_suis_je()) );


    connect(radioButton_mensuel,
    		SIGNAL(toggled(bool)),
			this,
			SLOT(handle_tab_business_turnover_progress_radio_button(bool)));

    connect(radioButton_jour_semaine,
    		SIGNAL(toggled(bool)),
			this,
			SLOT(handle_tab_business_turnover_progress_radio_button(bool)));


    connect( comboBox_qualite,
    		 SIGNAL(currentTextChanged(const QString &)),
    		 this,
			 SLOT(remove_BAR_PIE_CHART_OPTION_FOR_ZERO_BUSINESS_TURNOVER(const QString &)));


    changeLineEditEvolutionObjetsTextSetup(YerothTableauxDeBordWindow::OBJET_ARTICLES);

    connect( comboBox_evolution_objets,
    		 SIGNAL(currentTextChanged(const QString &)),
    		 this,
			 SLOT(changeLineEditEvolutionObjetsTextSetup(const QString &)));


    setupTab_BILAN_COMPTABLE();

    setupTab_COMPARAISON_DES_CHIFFRES_DAFFAIRES();

    setupTab_EVOLUTION_DU_CHIFFRE_DAFFAIRE();


#ifdef YEROTH_CLIENT
    actionAdministration->setEnabled(false);
#else
    //YEROTH_SERVER, YEROTH_STANDALONE, YEROTH_ACADEMIC_EVALUSATION_VERSION
    connect( actionAdministration, SIGNAL(triggered()), this, SLOT(administration()) );
#endif


    connect(tabWidget_rapports, SIGNAL(currentChanged(int)), this, SLOT(handleTabChanged(int)));


    checkBox_analyse_comparee->setChecked(true);


    handleTabChanged(SUJET_ACTION_FINANCIAL_ACCOUNTING_REPORT_GENERATION);


    setupShortcuts();
}


void YerothTableauxDeBordWindow::handle_tab_business_turnover_progress_radio_button(bool toggled)
{
	if (radioButton_mensuel->isChecked())
	{
//		checkBox_analyse_comparee->setEnabled(true);

		handle_enabled_chiffre_daffaire_mois(true);

		handle_enabled_chiffre_daffaire_jour_semaine(false);
	}
	else if (radioButton_jour_semaine->isChecked())
	{
//		checkBox_analyse_comparee->setEnabled(false);

		handle_enabled_chiffre_daffaire_jour_semaine(true);

		handle_enabled_chiffre_daffaire_mois(false);
	}
}


void YerothTableauxDeBordWindow::handleTabChanged(int index)
{
	YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionExporter_au_format_csv, false);

	actionAfficherPDF->disconnect();
	actionGenererPDF->disconnect();
	actionReinitialiserRecherche->disconnect();

    if (index == SUJET_ACTION_FINANCIAL_ACCOUNTING_REPORT_GENERATION)
    {
    	setWindowTitle(QObject::trUtf8("%1 - bilan comptable")
    						.arg(_windowName));

        connect( actionAfficherPDF, SIGNAL(triggered()), this, SLOT(bilanComptable()) );
    	connect( actionGenererPDF, SIGNAL(triggered()), this, SLOT(bilanComptable()) );
    	connect( actionReinitialiserRecherche, SIGNAL(triggered()), this, SLOT(reinitialiser_bilan_comptable()) );
    }
    else if (index == SUJET_ACTION_BUSINESS_TURNOVER_COMPARISON)
    {
    	remove_BAR_PIE_CHART_OPTION_FOR_ZERO_BUSINESS_TURNOVER(comboBox_qualite->currentText());

    	setWindowTitle(QObject::trUtf8("%1 - comparaison des chiffres d'affaires")
    						.arg(_windowName));

    	connect( actionAfficherPDF, SIGNAL(triggered()), this, SLOT(generer()) );
    	connect( actionGenererPDF, SIGNAL(triggered()), this, SLOT(generer()) );
    	connect( actionReinitialiserRecherche, SIGNAL(triggered()), this, SLOT(reinitialiser()) );
    }
    else // SUJET_ACTION_BUSINESS_TURNOVER_PROGRESS
    {
    	setWindowTitle(QObject::trUtf8("%1 - évolution du chiffre d'affaire")
    						.arg(_windowName));

        connect( actionAfficherPDF, SIGNAL(triggered()), this, SLOT(choisirEvolutionDuChiffreDaffaire()) );
    	connect( actionGenererPDF, SIGNAL(triggered()), this, SLOT(choisirEvolutionDuChiffreDaffaire()) );
    	connect( actionReinitialiserRecherche, SIGNAL(triggered()), this, SLOT(reinitialiser_chiffre_affaire()) );
    }
}


void YerothTableauxDeBordWindow::setupDateTimeEdits_EVOLUTION_DES_CHIFFRES_DAFFAIRES()
{
	dateEdit_chiffre_daffaire_jour_semaine_debut->setStartDate(GET_CURRENT_DATE);
	dateEdit_chiffre_daffaire_jour_semaine_fin->setStartDate(GET_CURRENT_DATE);
}


void YerothTableauxDeBordWindow::setupDateTimeEdits_COMPARAISON_DES_CHIFFRES_DAFFAIRES()
{
    dateEdit_rapports_debut->setStartDate(YerothERPConfig::GET_YEROTH_BUSINESS_DASHBOARD_DEFAULT_START_DATE());
    dateEdit_rapports_fin->setStartDate(GET_CURRENT_DATE);
}


void YerothTableauxDeBordWindow::setupDateTimeEdits_BILAN_COMPTABLE()
{
	dateEdit_bilan_comptable_debut->setStartDate(YerothERPConfig::GET_YEROTH_BUSINESS_DASHBOARD_DEFAULT_START_DATE());
	dateEdit_bilan_comptable_fin->setStartDate(GET_CURRENT_DATE);
}


void YerothTableauxDeBordWindow::setupTab_EVOLUTION_DU_CHIFFRE_DAFFAIRE()
{
	radioButton_jour_semaine->setChecked(true);

	handle_enabled_chiffre_daffaire_mois(false);

	handle_enabled_chiffre_daffaire_jour_semaine(true);

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

    comboBox_qualite->addItem(YerothTableauxDeBordWindow::QUALITE_PLUS_VENDU_PAR_QUANTITE);
    comboBox_qualite->addItem(YerothTableauxDeBordWindow::QUALITE_MOINS_VENDU_PAR_QUANTITE);
    comboBox_qualite->addItem(YerothTableauxDeBordWindow::QUALITE_MEILLEURS);
    comboBox_qualite->addItem(YerothTableauxDeBordWindow::QUALITE_DERNIERS);
    comboBox_qualite->addItem(YerothTableauxDeBordWindow::QUALITE_ZERO);

    comboBox_objets->addItem(YerothTableauxDeBordWindow::OBJET_ARTICLES);
    comboBox_objets->addItem(YerothTableauxDeBordWindow::OBJET_CAISSIERS);
    comboBox_objets->addItem(YerothTableauxDeBordWindow::OBJET_CATEGORIES);
    comboBox_objets->addItem(YerothTableauxDeBordWindow::OBJET_CLIENTS);
    comboBox_objets->addItem(YerothTableauxDeBordWindow::OBJET_FOURNISSEURS);
    comboBox_objets->addItem(YerothTableauxDeBordWindow::OBJET_SERVICES);

    comboBox_type_graphes->addItem(YerothTableauxDeBordWindow::GRAPHE_BAR_CHART);
    comboBox_type_graphes->addItem(YerothTableauxDeBordWindow::GRAPHE_PIE_CHART);
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

    setVisible(true);
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

    lineEdit_quantite->clear();

    comboBox_objets->resetYerothComboBox();
    comboBox_qualite->resetYerothComboBox();
    comboBox_type_graphes->resetYerothComboBox();

    dateEdit_rapports_debut->reset();
    dateEdit_rapports_fin->reset();
}

void YerothTableauxDeBordWindow::reinitialiser_chiffre_affaire()
{
    _logger->log("reinitialiser_chiffre_affaire");

    checkBox_analyse_comparee->setChecked(true);

    lineEdit_evolution_objets_value->myClear();

    comboBox_operations_chiffre->resetYerothComboBox();

    dateEdit_chiffre_daffaire_jour_semaine_debut->reset();
    dateEdit_chiffre_daffaire_jour_semaine_fin->reset();

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


void YerothTableauxDeBordWindow::generer()
{
    if (YerothTableauxDeBordWindow::QUALITE_PLUS_VENDU_PAR_QUANTITE == comboBox_qualite->currentText() ||
    	YerothTableauxDeBordWindow::QUALITE_MOINS_VENDU_PAR_QUANTITE == comboBox_qualite->currentText() )
    {
    	compterLesArticlesVendusParQuantite();
    }
    else
    {
        rechercher();
    }
}


void YerothTableauxDeBordWindow::quantite_max_stats(QString fileName,
													QString fieldId,
													int size,
													bool service /* = false */)
{
    _logger->log("quantite_max_stats");

    QString strQuery;

    if (!service)
    {
    	strQuery.append(QString("SELECT %1, %2, (round(%3, 2) - round(%4, 2)) FROM %5 WHERE %6 >= '%7' AND %8 <= '%9'")
    					.arg(fieldId,
    						 YerothDatabaseTableColumn::QUANTITE_VENDUE,
							 YerothDatabaseTableColumn::MONTANT_TOTAL_VENTE,
							 YerothDatabaseTableColumn::MONTANT_TVA,
    						 YerothDatabase::STOCKS_VENDU,
							 YerothDatabaseTableColumn::DATE_VENTE,
							 DATE_TO_DB_FORMAT_STRING(dateEdit_rapports_debut->date()),
							 YerothDatabaseTableColumn::DATE_VENTE,
							 DATE_TO_DB_FORMAT_STRING(dateEdit_rapports_fin->date())));
    }
    else //service == true
    {
    	strQuery.append(QString("SELECT %1, %2, (round(%3, 2) - round(%4, 2)) FROM %5 WHERE (date_vente >= '%6' AND %7 <= '%8') AND %9=1")
    					.arg(fieldId,
    						 YerothDatabaseTableColumn::QUANTITE_VENDUE,
							 YerothDatabaseTableColumn::MONTANT_TOTAL_VENTE,
							 YerothDatabaseTableColumn::MONTANT_TVA,
    						 YerothDatabase::SERVICES_COMPLETES,
							 DATE_TO_DB_FORMAT_STRING(dateEdit_rapports_debut->date()),
							 YerothDatabaseTableColumn::DATE_VENTE,
							 DATE_TO_DB_FORMAT_STRING(dateEdit_rapports_fin->date()),
							 YerothDatabaseTableColumn::IS_SERVICE));
    }

    QSqlQuery query;

    unsigned int SIZE_FOR_CSV_FILE_CONTENT = (size > QUANTITE_9) ? QUANTITE_9 : size;

    int querySize = YerothUtils::execQuery(query, strQuery, _logger);

    QString fieldIdValue;
    double total_quantite_vendue= 0.0;
    double quantite_vendue = 0.0;

    double montant_total_vente = 0.0;
    double montant_total_TOUTES_vente = 0.0;

    QList<Yeroth_MAP_COMPLEX_Item *> nomEntrepriseFournisseurToVentes;

    if (querySize > 0)
    {
    	while(query.next())
    	{
    		fieldIdValue = query.value(0).toString();
    		quantite_vendue = query.value(1).toDouble();
    		montant_total_vente = query.value(2).toDouble();

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
    			Yeroth_MAP_COMPLEX_Item *aNewItem = new Yeroth_MAP_COMPLEX_Item(fieldIdValue,
    														    quantite_vendue,
																montant_total_vente);

    			nomEntrepriseFournisseurToVentes.push_back(aNewItem);
    		}
    		else
    		{
    			Yeroth_MAP_COMPLEX_Item *anItem = nomEntrepriseFournisseurToVentes.value(idx);

    			anItem->_itemValue += quantite_vendue;

    			anItem->_itemSecondValue += montant_total_vente;
    		}

    		montant_total_TOUTES_vente += montant_total_vente;

    		total_quantite_vendue += quantite_vendue;
    	}
    }

    qSort(nomEntrepriseFournisseurToVentes.begin(),
    	  nomEntrepriseFournisseurToVentes.end(),
		  Yeroth_MAP_COMPLEX_Item::lessThan);

    //Remove all items with a zero value
	for(int j = 0; j < nomEntrepriseFournisseurToVentes.size(); ++j)
	{
		if (0 == nomEntrepriseFournisseurToVentes.at(j)->_itemValue)
		{
			nomEntrepriseFournisseurToVentes.removeAt(j);
		}
	}

    QString csvFileContent;
    QString label;
    QString curValueStr;
    QString curSecondValueStr;
    QString pourcentageStr;

    _reportTexFileEndString.clear();

    _reportTexFileEndString.append(YerothUtils::LATEX_IN_OUT_handleForeignAccents(
    		QObject::trUtf8("D\\'etails par quantit\\'es les plus vendues:")))
        			.prepend("\\textbf{").append("}\n");

    _reportTexFileEndString.append("\\begin{enumerate}[1.]\n");

    _csvFileItemSize = 0;

    double pourcentage = 0;
    double curValue = 0;

    for(int j = nomEntrepriseFournisseurToVentes.size() - 1, k = 0; j > -1 && k < size; --j, ++k)
    {
    	curValue = nomEntrepriseFournisseurToVentes.value(j)->_itemValue;

    	curValueStr = GET_DOUBLE_STRING(curValue);

    	curSecondValueStr = GET_CURRENCY_STRING_NUM(nomEntrepriseFournisseurToVentes.value(j)->_itemSecondValue);

    	pourcentage = (curValue / total_quantite_vendue) * 100.0;
    	pourcentageStr = QString::number(pourcentage, 'f', 3);

    	/*qDebug() << QString("++ value: %1, name %2, total: %3, pourcentage: %4")
            				.arg(QString::number(caissierToVentes.value(j)->_itemValue, 'f', 9),
            					 caissierToVentes.value(j)->_itemName,
            					 QString::number(total, 'f', 9),
            					 QString::number(pourcentage, 'f', 9));*/

    	label = QString("\"%1\"")
    				.arg(YerothUtils::LATEX_IN_OUT_handleForeignAccents(nomEntrepriseFournisseurToVentes.at(j)->_itemName));

    	if (k < SIZE_FOR_CSV_FILE_CONTENT)
    	{
    		csvFileContent.prepend(QString("%1, %2\n")
    				.arg(label, pourcentageStr));
    		++_csvFileItemSize;
    	}

    	_reportTexFileEndString.append(QString("\\item %1: \\textbf{%2} (chiffre d'affaire: %3)\n")
    			.arg(label,
    				 YerothUtils::LATEX_IN_OUT_handleForeignAccents(curValueStr),
					 YerothUtils::LATEX_IN_OUT_handleForeignAccents(curSecondValueStr)));

    	//qDebug() << "++ reportTexFileEndString: " << _reportTexFileEndString;

    	pourcentage = 0;
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
    	_logger->log("quantite_max_stats", csvFile.errorString());
    }
    else
    {
    	QTextStream outCsvFile(&csvFile);

    	outCsvFile << csvFileContent;

    	//qDebug() << QString("\t%1: \n%2").arg(fileName, csvFileContent);

    	csvFile.close();
    }
}


void YerothTableauxDeBordWindow::quantite_moindre_stats(QString fileName,
														QString fieldId,
														int size,
														bool service /* = false */)
{
    _logger->log("quantite_moindre_stats");

    QString strQuery;

    if (!service)
    {
    	strQuery.append(QString("SELECT %1, %2, (round(%3, 2) - round(%4, 2)) FROM %5 WHERE %6 >= '%7' AND %8 <= '%9'")
    					.arg(fieldId,
    						 YerothDatabaseTableColumn::QUANTITE_VENDUE,
							 YerothDatabaseTableColumn::MONTANT_TOTAL_VENTE,
							 YerothDatabaseTableColumn::MONTANT_TVA,
    						 YerothDatabase::STOCKS_VENDU,
							 YerothDatabaseTableColumn::DATE_VENTE,
							 DATE_TO_DB_FORMAT_STRING(dateEdit_rapports_debut->date()),
							 YerothDatabaseTableColumn::DATE_VENTE,
							 DATE_TO_DB_FORMAT_STRING(dateEdit_rapports_fin->date())));
    }
    else //service == true
    {
    	strQuery.append(QString("SELECT %1, %2, (round(%3, 2) - round(%4, 2)) FROM %5 WHERE (date_vente >= '%6' AND %7 <= '%8') AND %9=1")
    					.arg(fieldId,
    						 YerothDatabaseTableColumn::QUANTITE_VENDUE,
							 YerothDatabaseTableColumn::MONTANT_TOTAL_VENTE,
							 YerothDatabaseTableColumn::MONTANT_TVA,
    						 YerothDatabase::SERVICES_COMPLETES,
							 DATE_TO_DB_FORMAT_STRING(dateEdit_rapports_debut->date()),
							 YerothDatabaseTableColumn::DATE_VENTE,
							 DATE_TO_DB_FORMAT_STRING(dateEdit_rapports_fin->date()),
							 YerothDatabaseTableColumn::IS_SERVICE));
    }

    QSqlQuery query;

    unsigned int SIZE_FOR_CSV_FILE_CONTENT = (size > QUANTITE_9) ? QUANTITE_9 : size;

    int querySize = YerothUtils::execQuery(query, strQuery, _logger);

    QString fieldIdValue;
    double total_quantite_vendue= 0.0;
    double quantite_vendue = 0.0;

    double montant_total_vente = 0.0;
    double montant_total_TOUTES_vente = 0.0;

    QList<Yeroth_MAP_COMPLEX_Item *> nomEntrepriseFournisseurToVentes;

    if (querySize > 0)
    {
    	while(query.next())
    	{
    		fieldIdValue = query.value(0).toString();
    		quantite_vendue = query.value(1).toDouble();
    		montant_total_vente = query.value(2).toDouble();

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
    			Yeroth_MAP_COMPLEX_Item *aNewItem = new Yeroth_MAP_COMPLEX_Item(fieldIdValue,
    														    quantite_vendue,
																montant_total_vente);

    			nomEntrepriseFournisseurToVentes.push_back(aNewItem);
    		}
    		else
    		{
    			Yeroth_MAP_COMPLEX_Item *anItem = nomEntrepriseFournisseurToVentes.value(idx);

    			anItem->_itemValue += quantite_vendue;

    			anItem->_itemSecondValue += montant_total_vente;
    		}

    		montant_total_TOUTES_vente += montant_total_vente;

    		total_quantite_vendue += quantite_vendue;
    	}
    }

    qSort(nomEntrepriseFournisseurToVentes.begin(),
    	  nomEntrepriseFournisseurToVentes.end(),
		  Yeroth_MAP_COMPLEX_Item::lessThan);

    //Remove all items with a zero value
	for(int j = 0; j < nomEntrepriseFournisseurToVentes.size(); ++j)
	{
		if (0 == nomEntrepriseFournisseurToVentes.at(j)->_itemValue)
		{
			nomEntrepriseFournisseurToVentes.removeAt(j);
		}
	}

    QString csvFileContent;
    QString label;
    QString curValueStr;
    QString curSecondValueStr;
    QString pourcentageStr;

    _reportTexFileEndString.clear();

    _reportTexFileEndString.append(YerothUtils::LATEX_IN_OUT_handleForeignAccents(
    		QObject::trUtf8("D\\'etails par quantit\\'es les moins vendues:")))
        			.prepend("\\textbf{").append("}\n");

    _reportTexFileEndString.append("\\begin{enumerate}[1.]\n");

    _csvFileItemSize = 0;

    double pourcentage = 0;
    double curValue = 0;

    for(int j = 0, k = 0; j < nomEntrepriseFournisseurToVentes.size() && k < size; ++j, ++k)
    {
    	curValue = nomEntrepriseFournisseurToVentes.value(j)->_itemValue;

    	curValueStr = GET_DOUBLE_STRING(curValue);

    	curSecondValueStr = GET_CURRENCY_STRING_NUM(nomEntrepriseFournisseurToVentes.value(j)->_itemSecondValue);

    	pourcentage = (curValue / total_quantite_vendue) * 100.0;
    	pourcentageStr = QString::number(pourcentage, 'f', 3);

    	/*qDebug() << QString("++ value: %1, name %2, total: %3, pourcentage: %4")
            				.arg(QString::number(caissierToVentes.value(j)->_itemValue, 'f', 9),
            					 caissierToVentes.value(j)->_itemName,
            					 QString::number(total, 'f', 9),
            					 QString::number(pourcentage, 'f', 9));*/

    	label = QString("\"%1\"")
    				.arg(YerothUtils::LATEX_IN_OUT_handleForeignAccents(nomEntrepriseFournisseurToVentes.at(j)->_itemName));

    	if (k < SIZE_FOR_CSV_FILE_CONTENT)
    	{
    		csvFileContent.prepend(QString("%1, %2\n")
    				.arg(label, pourcentageStr));
    		++_csvFileItemSize;
    	}

    	_reportTexFileEndString.append(QString("\\item %1: \\textbf{%2} (chiffre d'affaire: %3)\n")
    			.arg(label,
    				 YerothUtils::LATEX_IN_OUT_handleForeignAccents(curValueStr),
					 YerothUtils::LATEX_IN_OUT_handleForeignAccents(curSecondValueStr)));

    	//qDebug() << "++ reportTexFileEndString: " << _reportTexFileEndString;

    	pourcentage = 0;
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
    	_logger->log("quantite_moindre_stats", csvFile.errorString());
    }
    else
    {
    	QTextStream outCsvFile(&csvFile);

    	outCsvFile << csvFileContent;

    	//qDebug() << QString("\t%1: \n%2").arg(fileName, csvFileContent);

    	csvFile.close();
    }
}


void YerothTableauxDeBordWindow::meilleursStats(QString fileName,
        										QString fieldId,
												int size,
												bool service /* = false */)
{
    _logger->log("meilleursStats");

    QString strQuery;

    if (!service)
    {
    	strQuery.append(QString("SELECT %1, %2, (round(%3, 2) - round(%4, 2)) FROM %5 WHERE %6 >= '%7' AND %8 <= '%9'")
    					.arg(fieldId,
    						 YerothDatabaseTableColumn::QUANTITE_VENDUE,
    						 YerothDatabaseTableColumn::MONTANT_TOTAL_VENTE,
    						 YerothDatabaseTableColumn::MONTANT_TVA,
							 YerothDatabase::STOCKS_VENDU,
							 YerothDatabaseTableColumn::DATE_VENTE,
							 DATE_TO_DB_FORMAT_STRING(dateEdit_rapports_debut->date()),
							 YerothDatabaseTableColumn::DATE_VENTE,
							 DATE_TO_DB_FORMAT_STRING(dateEdit_rapports_fin->date())));
    }
    else //service == true
    {
    	strQuery.append(QString("SELECT %1, %2, (round(%3, 2) - round(%4, 2)) FROM %5"
    							" WHERE date_vente >= '%6' AND date_vente <= '%7' AND"
    							" %8=0 AND %9=1")
    					.arg(fieldId,
    						 YerothDatabaseTableColumn::QUANTITE_VENDUE,
    						 YerothDatabaseTableColumn::MONTANT_TOTAL_VENTE,
    						 YerothDatabaseTableColumn::MONTANT_TVA,
							 YerothDatabase::SERVICES_COMPLETES,
							 DATE_TO_DB_FORMAT_STRING(dateEdit_rapports_debut->date()),
							 DATE_TO_DB_FORMAT_STRING(dateEdit_rapports_fin->date()),
							 YerothDatabaseTableColumn::MONTANT_A_REMBOURSER,
							 YerothDatabaseTableColumn::IS_SERVICE));
    }

    QSqlQuery query;

    unsigned int SIZE_FOR_CSV_FILE_CONTENT = (size > QUANTITE_9) ? QUANTITE_9 : size;

    int querySize = YerothUtils::execQuery(query, strQuery, _logger);

//    qDebug() << "++ strQuery: " << strQuery;
//
//    qDebug() << "++ querySize: " << querySize;

    QString fieldIdValue;
    double montant_total_vente = 0.0;
    double total = 0.0;

    double quantite_vendue = 0.0;

    QList<Yeroth_MAP_COMPLEX_Item *> nomEntrepriseFournisseurToVentes;

    if (querySize > 0)
    {
        while(query.next())
        {
            fieldIdValue = query.value(0).toString();
            quantite_vendue = query.value(1).toDouble();
            montant_total_vente = query.value(2).toDouble();

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
                Yeroth_MAP_COMPLEX_Item *aNewItem = new Yeroth_MAP_COMPLEX_Item(fieldIdValue,
                												montant_total_vente,
																quantite_vendue);
                nomEntrepriseFournisseurToVentes.push_back(aNewItem);
            }
            else
            {
                Yeroth_MAP_COMPLEX_Item *anItem = nomEntrepriseFournisseurToVentes.value(idx);
                anItem->_itemValue += montant_total_vente;
                anItem->_itemSecondValue += quantite_vendue;
            }

            total += montant_total_vente;
        }
    }

    qSort(nomEntrepriseFournisseurToVentes.begin(),
    	  nomEntrepriseFournisseurToVentes.end(),
		  Yeroth_MAP_COMPLEX_Item::lessThan);

    //Remove all items with a zero value
    for(int j = 0; j < nomEntrepriseFournisseurToVentes.size(); ++j)
    {
        if (0 == nomEntrepriseFournisseurToVentes.at(j)->_itemValue)
        {
        	nomEntrepriseFournisseurToVentes.removeAt(j);
        }
    }

    QString csvFileContent;
    QString label;
    QString curValueStr;
    QString curSecondValueStr;
    QString pourcentageStr;

    _reportTexFileEndString.clear();

    _reportTexFileEndString.append(YerothUtils::LATEX_IN_OUT_handleForeignAccents(
                                       QObject::trUtf8("D\\'etails en %1:")
                                       	   .arg(YerothERPConfig::currency)))
    					   .prepend("\\textbf{").append("}\n");

    _reportTexFileEndString.append("\\begin{enumerate}[1.]\n");

    _csvFileItemSize = 0;

    double pourcentage = 0;
    double curValue = 0;

    for(int j = nomEntrepriseFournisseurToVentes.size() - 1, k = 0; j > -1 && k < size; --j, ++k)
    {
        curValue = nomEntrepriseFournisseurToVentes.value(j)->_itemValue;

        curValueStr = GET_CURRENCY_STRING_NUM(curValue);

        curSecondValueStr = GET_DOUBLE_STRING(nomEntrepriseFournisseurToVentes.value(j)->_itemSecondValue);

        pourcentage = (curValue / total) * 100.0;
        pourcentageStr = QString::number(pourcentage, 'f', 3);

        /*qDebug() << QString("++ value: %1, name %2, total: %3, pourcentage: %4")
        				.arg(QString::number(caissierToVentes.value(j)->_itemValue, 'f', 9),
        					 caissierToVentes.value(j)->_itemName,
        					 QString::number(total, 'f', 9),
        					 QString::number(pourcentage, 'f', 9));*/

        label = QString("\"%1\"")
        			.arg(YerothUtils::LATEX_IN_OUT_handleForeignAccents(nomEntrepriseFournisseurToVentes.at(j)->_itemName));

        if (k < SIZE_FOR_CSV_FILE_CONTENT)
        {
        	csvFileContent.prepend(QString("%1, %2\n")
        							.arg(label, pourcentageStr));
            ++_csvFileItemSize;
        }

        _reportTexFileEndString.append(QString("\\item %1: \\textbf{%2} (quantit\\'e vendue: %3)\n")
        									.arg(label,
        										 YerothUtils::LATEX_IN_OUT_handleForeignAccents(curValueStr),
												 YerothUtils::LATEX_IN_OUT_handleForeignAccents(curSecondValueStr)));

        //qDebug() << "++ reportTexFileEndString: " << _reportTexFileEndString;

        pourcentage = 0;
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


void YerothTableauxDeBordWindow::ZERO_stats_stocks(QString fileName,
												   QString fieldId)
{
    _logger->log("ZERO_stats_stocks");

    QString strQuery(QString("SELECT %1, prix_dachat, quantite_totale "
    						 "FROM %2 s "
    						 "WHERE s.%3 NOT IN "
    						 	 	"(select %4 "
    						 	 	"from %5 sv "
    						 	 	"where sv.%6 = s.%7 "
    						 	 	"AND date_vente >= '%8' AND date_vente <= '%9')")
    					.arg(fieldId,
							 YerothDatabase::STOCKS,
							 fieldId,
							 fieldId,
							 YerothDatabase::STOCKS_VENDU,
							 fieldId,
							 fieldId,
							 DATE_TO_DB_FORMAT_STRING(dateEdit_rapports_debut->date()),
							 DATE_TO_DB_FORMAT_STRING(dateEdit_rapports_fin->date())));


//    QDEBUG_STRING_OUTPUT_2("YerothTableauxDeBordWindow::ZERO_stats, strQuery", strQuery);

    QSqlQuery query;

    int querySize = YerothUtils::execQuery(query, strQuery, _logger);

//    QDEBUG_STRING_OUTPUT_2_N("querySize", querySize);

    double prix_dachat = 0.0;

    double valeur_dinventaire = 0.0;

    double quantite_totale = 0.0;

    QString fieldIdValue;

    Yeroth_MAP_COMPLEX_Item *anItem = 0;

    QList<Yeroth_MAP_COMPLEX_Item *> allItems;

    if (querySize > 0)
    {
        while(query.next())
        {
        	fieldIdValue = query.value(0).toString();

        	prix_dachat = query.value(1).toDouble();

        	quantite_totale = query.value(2).toDouble();

            int idx = -1;
            for(int i = 0; i < allItems.size(); ++i)
            {
                if (YerothUtils::isEqualCaseInsensitive(allItems.value(i)->_itemName, fieldIdValue))
                {
                    idx = i;
                    break;
                }
            }


            if (-1 == idx)
            {
            	anItem = new Yeroth_MAP_COMPLEX_Item(fieldIdValue, 0.0);

            	anItem->_itemName = fieldIdValue;

                allItems.push_back(anItem);
            }
            else
            {
            	anItem = allItems.value(idx);
            }

            valeur_dinventaire = (quantite_totale * prix_dachat);

            anItem->_itemValue += quantite_totale;

            anItem->_itemSecondValue += valeur_dinventaire;
        }
    }

    qSort(allItems.begin(), allItems.end(), Yeroth_MAP_COMPLEX_Item::lessThan_second_value);

    QString csvFileContent;

    _reportTexFileEndString.clear();

    _reportTexFileEndString.append("\\begin{enumerate}[1.]\n");

    _csvFileItemSize = 0;

    double valeur_dinventaire_toute_marchandise = 0.0;

    QString label;
    QString value;

    for(int j = allItems.size() - 1, k = 0; j > -1; --j, ++k)
    {
        label.clear();

        label.append(QString("\"%1\"")
        				.arg(YerothUtils::LATEX_IN_OUT_handleForeignAccents(allItems.at(j)->_itemName)));

        value.clear();

        value.append(YerothUtils::LATEX_IN_OUT_handleForeignAccents(
        		GET_DOUBLE_STRING(allItems.at(j)->_itemValue)));

        valeur_dinventaire = allItems.at(j)->_itemSecondValue;

        valeur_dinventaire_toute_marchandise += valeur_dinventaire;

        csvFileContent.prepend(QString("%1; %2; %3; %4\n")
        						.arg(allItems.at(j)->_itemName,
        							 value,
									 GET_DOUBLE_STRING(valeur_dinventaire),
									 QString::number(0.0)));

        _reportTexFileEndString.append(QObject::trUtf8("\\item %1 | "
        											   "Qté en stock: \"%2\" | "
        											   "Valeur d'inventaire: \"%3\"\n")
        									.arg(label,
        										 value,
												 GET_CURRENCY_STRING_NUM(valeur_dinventaire)));

        //qDebug() << "++ reportTexFileEndString: " << _reportTexFileEndString;
        ++_csvFileItemSize;
    }

    _reportTexFileEndString.append("\\end{enumerate}"
    							   "\\vspace{1em}");

    _reportTexFileEndString.append(QObject::tr("\\underline{\\textbf{SOMME (VALEUR D'INVENTAIRE TOUT STOCK): %1}}")
    									.arg(GET_CURRENCY_STRING_NUM(valeur_dinventaire_toute_marchandise)));

    //qDebug() << "++ test, _csvFileItemSize: " << caissierToVentes.size();

    csvFileContent.prepend(QObject::trUtf8("Nom; Quantité totale en stock; Valeur d'inventaire (%1); Total chiffre d'affaire\n")
    								.arg(YerothERPConfig::currency));

    //clean up
    for (int k = 0; k < allItems.size(); ++k)
    {
        delete allItems.value(k);
    }

    QFile csvFile(fileName);

     if (!csvFile.open(QIODevice::WriteOnly | QIODevice::Truncate))
     {
         _logger->log("ZERO_stats", csvFile.errorString());
     }
     else
     {
         QTextStream outCsvFile(&csvFile);

         outCsvFile << csvFileContent;

         //qDebug() << QString("\t%1: \n%2").arg(fileName, csvFileContent);

         csvFile.close();
     }
}



void YerothTableauxDeBordWindow::ZERO_stats(QString fileName,
											QString fieldId)
{
    _logger->log("ZERO_stats");

    QString strQuery(QString("SELECT %1, quantite_totale "
    						 "FROM %2 s "
    						 "WHERE s.%3 NOT IN "
    						 	 	"(select %4 "
    						 	 	"from %5 sv "
    						 	 	"where sv.%6 = s.%7 "
    						 	 	"AND date_vente >= '%8' AND date_vente <= '%9')")
    					.arg(fieldId,
							 YerothDatabase::STOCKS,
							 fieldId,
							 fieldId,
							 YerothDatabase::STOCKS_VENDU,
							 fieldId,
							 fieldId,
							 DATE_TO_DB_FORMAT_STRING(dateEdit_rapports_debut->date()),
							 DATE_TO_DB_FORMAT_STRING(dateEdit_rapports_fin->date())));


//    QDEBUG_STRING_OUTPUT_2("YerothTableauxDeBordWindow::ZERO_stats, strQuery", strQuery);

    QSqlQuery query;

    int querySize = YerothUtils::execQuery(query, strQuery, _logger);

    double quantite_totale = 0.0;

    QString fieldIdValue;

    Yeroth_MAP_COMPLEX_Item *anItem = 0;

    QList<Yeroth_MAP_COMPLEX_Item *> allItems;

    if (querySize > 0)
    {
        while(query.next())
        {
        	fieldIdValue = query.value(0).toString();

        	quantite_totale = query.value(1).toDouble();

            int idx = -1;
            for(int i = 0; i < allItems.size(); ++i)
            {
                if (YerothUtils::isEqualCaseInsensitive(allItems.value(i)->_itemName, fieldIdValue))
                {
                    idx = i;
                    break;
                }
            }

            if (-1 == idx)
            {
            	anItem = new Yeroth_MAP_COMPLEX_Item(fieldIdValue, 0.0);

                anItem->_itemName = fieldIdValue;

                allItems.push_back(anItem);
            }
            else
            {
            	anItem = allItems.value(idx);
            }

            anItem->_itemValue += quantite_totale;
        }
    }

    qSort(allItems.begin(), allItems.end(), Yeroth_MAP_COMPLEX_Item::lessThan);

    QString csvFileContent;

    _reportTexFileEndString.clear();

    _reportTexFileEndString.append("\\begin{enumerate}[1.]\n");

    _csvFileItemSize = 0;

    double valeur_dinventaire_toute_marchandise = 0.0;

    QString label;
    QString value;

    for(int j = allItems.size() - 1, k = 0; j > -1; --j, ++k)
    {
        label = QString("\"%1\"")
        			.arg(YerothUtils::LATEX_IN_OUT_handleForeignAccents(allItems.at(j)->_itemName));

        value = YerothUtils::LATEX_IN_OUT_handleForeignAccents(
        			GET_DOUBLE_STRING(allItems.at(j)->_itemValue));

        csvFileContent.prepend(QString("%1; %2; %3\n")
        						.arg(label,
        							 value,
									 QString::number(0.0)));

        _reportTexFileEndString.append(QObject::trUtf8("\\item %1 | "
        											   "Qté en stock: \"%2\"\n")
        									.arg(label,
        										 value));

        //qDebug() << "++ reportTexFileEndString: " << _reportTexFileEndString;
        ++_csvFileItemSize;
    }

    _reportTexFileEndString.append("\\end{enumerate}");

    //qDebug() << "++ test, _csvFileItemSize: " << caissierToVentes.size();

    csvFileContent.prepend(QObject::trUtf8("Nom; Quantité totale en stock; Total chiffre d'affaire\n"));

    //clean up
    for (int k = 0; k < allItems.size(); ++k)
    {
        delete allItems.value(k);
    }

    QFile csvFile(fileName);

     if (!csvFile.open(QIODevice::WriteOnly | QIODevice::Truncate))
     {
         _logger->log("ZERO_stats", csvFile.errorString());
     }
     else
     {
         QTextStream outCsvFile(&csvFile);

         outCsvFile << csvFileContent;

         //qDebug() << QString("\t%1: \n%2").arg(fileName, csvFileContent);

         csvFile.close();
     }
}


void YerothTableauxDeBordWindow::derniersStats(QString fileName,
											   QString fieldId,
											   int size,
											   bool service /* = false */)
{
    _logger->log("derniersStats");

    QString strQuery;

    if (!service)
    {
    	strQuery.append(QString("SELECT %1, %2, (round(%3, 2) - round(%4, 2)) FROM %5 WHERE %6 >= '%7' AND %8 <= '%9'")
    					.arg(fieldId,
    						 YerothDatabaseTableColumn::QUANTITE_VENDUE,
    						 YerothDatabaseTableColumn::MONTANT_TOTAL_VENTE,
    						 YerothDatabaseTableColumn::MONTANT_TVA,
							 YerothDatabase::STOCKS_VENDU,
							 YerothDatabaseTableColumn::DATE_VENTE,
							 DATE_TO_DB_FORMAT_STRING(dateEdit_rapports_debut->date()),
							 YerothDatabaseTableColumn::DATE_VENTE,
							 DATE_TO_DB_FORMAT_STRING(dateEdit_rapports_fin->date())));
    }
    else
    {
    	strQuery.append(QString("SELECT %1, %2, (round(%3, 2) - round(%4, 2)) FROM %5"
    							" WHERE date_vente >= '%6' AND date_vente <= '%7' AND"
    							" %8=0 AND %9=1")
    					.arg(fieldId,
    						 YerothDatabaseTableColumn::QUANTITE_VENDUE,
    						 YerothDatabaseTableColumn::MONTANT_TOTAL_VENTE,
    						 YerothDatabaseTableColumn::MONTANT_TVA,
							 YerothDatabase::SERVICES_COMPLETES,
							 DATE_TO_DB_FORMAT_STRING(dateEdit_rapports_debut->date()),
							 DATE_TO_DB_FORMAT_STRING(dateEdit_rapports_fin->date()),
							 YerothDatabaseTableColumn::MONTANT_A_REMBOURSER,
							 YerothDatabaseTableColumn::IS_SERVICE));
    }

    QSqlQuery query;

    unsigned int SIZE_FOR_CSV_FILE_CONTENT = (size > QUANTITE_9) ? QUANTITE_9 : size;

    int querySize = YerothUtils::execQuery(query, strQuery, _logger);

//    qDebug() << "++ query: " << query.executedQuery() << ", querySize: " << querySize;

    QString fieldIdValue;

    double montant_total_vente = 0.0;
    double total = 0.0;

    double quantite_vendue = 0.0;

    QList<Yeroth_MAP_COMPLEX_Item *> caissierToVentes;

    if (querySize > 0)
    {
        while(query.next())
        {
            fieldIdValue = query.value(0).toString();

            quantite_vendue = query.value(1).toDouble();

            montant_total_vente = query.value(2).toDouble();

            if (fieldIdValue.isEmpty())
            {
                continue;
            }

            int idx = -1;
            for(int i = 0; i < caissierToVentes.size(); ++i)
            {
                if (YerothUtils::isEqualCaseInsensitive(caissierToVentes.value(i)->_itemName, fieldIdValue))
                {
                    idx = i;
                    break;
                }
            }

            if (-1 == idx)
            {
                Yeroth_MAP_COMPLEX_Item *aNewItem = new Yeroth_MAP_COMPLEX_Item(fieldIdValue,
                												montant_total_vente,
																quantite_vendue);
                caissierToVentes.push_back(aNewItem);
            }
            else
            {
                Yeroth_MAP_COMPLEX_Item *anItem = caissierToVentes.value(idx);

                anItem->_itemValue += montant_total_vente;

                anItem->_itemSecondValue += quantite_vendue;
            }

            total += montant_total_vente;
        }
    }

    qSort(caissierToVentes.begin(),
    	  caissierToVentes.end(),
		  Yeroth_MAP_COMPLEX_Item::lessThan);

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
    QString curSecondValueStr;

    double pourcentage = 0;
    double curValue = 0;

    _csvFileItemSize = 0;

    _reportTexFileEndString = YerothUtils::LATEX_IN_OUT_handleForeignAccents(
                                       QObject::trUtf8("D\\'etails en %1:")
                                       	   .arg(YerothERPConfig::currency));

    _reportTexFileEndString.prepend("\\textbf{").append("}\n");

    _reportTexFileEndString.append("\\begin{enumerate}[1.]\n");

    for(int j = 0, k = 0; j < caissierToVentes.size() && k < size; ++j)
    {
        curValue = caissierToVentes.value(j)->_itemValue;

        pourcentage = (curValue / total) * 100.0;

//        qDebug() << QString("++ j:%1. value: %2, total: %3, pourcentage: %4")
//        				.arg(QString::number(j, 'f', 2),
//        					 QString::number(curValue, 'f', 2),
//        					 QString::number(total, 'f', 2),
//        					 QString::number(pourcentage, 'f', 2));
//
//        qDebug() << "++ pourcentage: " << QString::number(pourcentage, 'f', 9);

        if (pourcentage <= STATS_MIN_VALUE)
        {
            continue;
        }

        pourcentageStr = QString::number(pourcentage, 'f', 3);

        curValueStr = GET_CURRENCY_STRING_NUM(curValue);

        curSecondValueStr = GET_DOUBLE_STRING(caissierToVentes.value(j)->_itemSecondValue);

        label = QString("\"%1\"")
        			.arg(YerothUtils::LATEX_IN_OUT_handleForeignAccents(caissierToVentes.at(j)->_itemName));

        if (k < SIZE_FOR_CSV_FILE_CONTENT)
        {
        	csvFileContent.append(QString("%1, %2\n")
        							.arg(label, (pourcentage <= 0.001 ? "0.001" : pourcentageStr)));
            ++_csvFileItemSize;
        }

        _reportTexFileEndString.append(QString("\\item %1: \\textbf{%2} (quantit\\'e vendue: %3)\n")
        									.arg(label,
        										 YerothUtils::LATEX_IN_OUT_handleForeignAccents(curValueStr),
												 YerothUtils::LATEX_IN_OUT_handleForeignAccents(curSecondValueStr)));

        //qDebug() << "++ reportTexFileEndString: " << _reportTexFileEndString;
        ++k;
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

    int size = lineEdit_quantite->text().toInt();

    QStringList progArguments;
    QString tmpFilePrefix;

    QDir tempDir(YerothERPConfig::temporaryFilesDir);

    QString csvFile;
    QString pdfFileTitle;

    if (YerothTableauxDeBordWindow::QUALITE_MEILLEURS == comboBox_qualite->currentText() ||
    	YerothTableauxDeBordWindow::QUALITE_DERNIERS == comboBox_qualite->currentText() )
    {
#ifdef YEROTH_FRANCAIS_LANGUAGE
    	pdfFileTitle.append(QString("Les %1 ")
    							.arg(lineEdit_quantite->text()));
#endif

#ifdef YEROTH_ENGLISH_LANGUAGE
    	pdfFileTitle.append(QString("The %1 ")
    							.arg(lineEdit_quantite->text()));
#endif
    }
    else
    {
#ifdef YEROTH_FRANCAIS_LANGUAGE
    	pdfFileTitle.append("Les ");
#endif

#ifdef YEROTH_ENGLISH_LANGUAGE
    	pdfFileTitle.append("The ");
#endif
    }

    if (YerothTableauxDeBordWindow::QUALITE_MEILLEURS == comboBox_qualite->currentText())
    {
        if (YerothTableauxDeBordWindow::OBJET_SERVICES == objet)
        {
            pdfFileTitle.append(YerothTableauxDeBordWindow::OBJET_SERVICES);

#ifdef YEROTH_FRANCAIS_LANGUAGE
            tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("meilleurs-services");
#endif

#ifdef YEROTH_ENGLISH_LANGUAGE
            tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("best-services");
#endif

            csvFile = tmpFilePrefix + ".csv";

            csvFile.prepend(YerothERPConfig::temporaryFilesDir + "/");

            tempDir.remove(csvFile);

            statsMeilleursServices(csvFile, size);
        }
        else if (YerothTableauxDeBordWindow::OBJET_ARTICLES == objet)
        {
        	pdfFileTitle.append(YerothTableauxDeBordWindow::OBJET_ARTICLES);

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

            pdfFileTitle.append(YerothTableauxDeBordWindow::OBJET_CATEGORIES);

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

    else if (YerothTableauxDeBordWindow::QUALITE_ZERO == comboBox_qualite->currentText())
    {
        if (YerothTableauxDeBordWindow::OBJET_ARTICLES == objet)
        {
            pdfFileTitle.append(YerothTableauxDeBordWindow::OBJET_ARTICLES);

            tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("zero-articles");

            csvFile = tmpFilePrefix + ".csv";

            csvFile.prepend(YerothERPConfig::temporaryFilesDir + "/");

            tempDir.remove(csvFile);

            statsZERO_Articles(csvFile);
        }
        else if (YerothTableauxDeBordWindow::OBJET_CATEGORIES == objet)
        {
            pdfFileTitle.append(YerothTableauxDeBordWindow::OBJET_CATEGORIES);

            tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("zero-categories");

            csvFile = tmpFilePrefix + ".csv";

            csvFile.prepend(YerothERPConfig::temporaryFilesDir + "/");

            tempDir.remove(csvFile);

            statsZERO_Categories(csvFile);
        }
        else if (YerothTableauxDeBordWindow::OBJET_CAISSIERS == objet)
        {
            pdfFileTitle.append(YerothTableauxDeBordWindow::OBJET_CAISSIERS);

#ifdef YEROTH_FRANCAIS_LANGUAGE
            tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("zero-caissiers");
#endif

#ifdef YEROTH_ENGLISH_LANGUAGE
            tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("zero-cashiers");
#endif

            csvFile = tmpFilePrefix + ".csv";

            csvFile.prepend(YerothERPConfig::temporaryFilesDir  + "/");

            tempDir.remove(csvFile);

            statsZERO_Caissiers(csvFile);
        }
        else if (YerothTableauxDeBordWindow::OBJET_CLIENTS == objet)
        {
            pdfFileTitle.append(YerothTableauxDeBordWindow::OBJET_CLIENTS);

#ifdef YEROTH_FRANCAIS_LANGUAGE
            tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("zero-chiffre-daffaire-clients");
#endif

#ifdef YEROTH_ENGLISH_LANGUAGE
            tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("zero-turnover-customers");
#endif
            csvFile = tmpFilePrefix + ".csv";

            csvFile.prepend(YerothERPConfig::temporaryFilesDir + "/");

            tempDir.remove(csvFile);

            statsZERO_Clients(csvFile);
        }
        else if (YerothTableauxDeBordWindow::OBJET_FOURNISSEURS == objet)
        {
            pdfFileTitle.append(YerothTableauxDeBordWindow::OBJET_FOURNISSEURS);

#ifdef YEROTH_FRANCAIS_LANGUAGE
            tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("zero-fournisseurs-ventes");
#endif

#ifdef YEROTH_ENGLISH_LANGUAGE
            tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("zero-suppliers-sales");
#endif

            csvFile = tmpFilePrefix + ".csv";

            csvFile.prepend(YerothERPConfig::temporaryFilesDir + "/");

            tempDir.remove(csvFile);

            statsZERO_FournisseursVentes(csvFile);
        }

#ifdef YEROTH_FRANCAIS_LANGUAGE
        pdfFileTitle.append(QString(" (quantité: %1) avec les chiffres d'affaires nuls (0)")
        						.arg(GET_NUM_STRING(_csvFileItemSize)));
#endif

#ifdef YEROTH_ENGLISH_LANGUAGE
        pdfFileTitle.append(QString(" (quantity: %1) with NO (zero) financial income")
        						.arg(GET_NUM_STRING(_csvFileItemSize)));
#endif
    }

    else if (YerothTableauxDeBordWindow::QUALITE_DERNIERS == comboBox_qualite->currentText())
    {
        if (YerothTableauxDeBordWindow::OBJET_SERVICES == objet)
        {
            pdfFileTitle.append(YerothTableauxDeBordWindow::OBJET_SERVICES);

#ifdef YEROTH_FRANCAIS_LANGUAGE
            tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("derniers-services");
#endif

#ifdef YEROTH_ENGLISH_LANGUAGE
            tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("last-services");
#endif

            csvFile = tmpFilePrefix + ".csv";

            csvFile.prepend(YerothERPConfig::temporaryFilesDir + "/");

            tempDir.remove(csvFile);

            statsDerniersServices(csvFile, size);
        }
        else if (YerothTableauxDeBordWindow::OBJET_ARTICLES == objet)
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
        else if (YerothTableauxDeBordWindow::OBJET_FOURNISSEURS == objet)
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

    if (_csvFileItemSize <= 0)
    {
        QString retMsg(QObject::trUtf8("Il n'y a pas de données correspondante à la requête !\n"
        							   "Vérifier que les dates de début et de fin sont correctes !"));

        YerothQMessageBox::information(this,
                                      QObject::trUtf8("rankings - pas de données !"),
                                      retMsg);
        _csvFileItemSize = 0;

        return ;
    }

    QString latexChartTemplate;
    QString latexChartFileNamePrefix;

#ifdef YEROTH_FRANCAIS_LANGUAGE
	if (YerothTableauxDeBordWindow::QUALITE_ZERO == comboBox_qualite->currentText())
	{
		latexChartTemplate.append(YerothUtils::FR_ZERO_ventes_tex);

		latexChartFileNamePrefix.append(YerothERPConfig::temporaryFilesDir + "/" + tmpFilePrefix + "_ZERO");
	}
	else if (YerothTableauxDeBordWindow::GRAPHE_BAR_CHART == comboBox_type_graphes->currentText())
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
	if (YerothTableauxDeBordWindow::QUALITE_ZERO == comboBox_qualite->currentText())
	{
		latexChartTemplate.append(YerothUtils::EN_ZERO_ventes_tex);

		latexChartFileNamePrefix.append(YerothERPConfig::temporaryFilesDir + "/" + tmpFilePrefix + "_ZERO");
	}
	else if (YerothTableauxDeBordWindow::GRAPHE_BAR_CHART == comboBox_type_graphes->currentText())
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

    QString statsDate(YerothUtils::LATEX_IN_OUT_handleForeignAccents(infoEntreprise.getVille_LATEX()));

    YerothUtils::getCurrentLocaleDate(statsDate);

    //qDebug() << "++ type fact. rapports: " << YerothConfig::typeOfFacturation;

    latexChartTemplate.replace("YEROTHPAPERSPEC", "a4paper");

    latexChartTemplate.replace("YEROTHCSVFILE", 					tmpFilePrefix + ".csv");
    latexChartTemplate.replace("YEROTHCHARTFIN", 				_reportTexFileEndString);

    latexChartTemplate.replace("YEROTHUTILISATEUR", QString("%1 %2")
       													.arg(YerothUtils::getAllWindows()->getUser()->titreTex(),
       														 YerothUtils::getAllWindows()->getUser()->nom_completTex()));

    latexChartTemplate.replace("YEROTHENTREPRISE", 				infoEntreprise.getNomCommercial_LATEX());
    latexChartTemplate.replace("YEROTHBARCHARTTITLE", 			YerothUtils::LATEX_IN_OUT_handleForeignAccents(pdfFileTitle));
    latexChartTemplate.replace("YEROTHENTREPRISE", 				infoEntreprise.getNomCommercial_LATEX());
    latexChartTemplate.replace("YEROTHACTIVITESENTREPRISE", 		infoEntreprise.getSecteursActivitesTex());
    latexChartTemplate.replace("YEROTHBOITEPOSTALE", 			infoEntreprise.getBoitePostal());
    latexChartTemplate.replace("YEROTHVILLE", 					infoEntreprise.getVille_LATEX());
    latexChartTemplate.replace("YEROTHPAYS", 					infoEntreprise.getPaysTex());
    latexChartTemplate.replace("YEROTHEMAIL", 					infoEntreprise.getEmail_LATEX());
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

    _csvFileItemSize = 0;
}


bool YerothTableauxDeBordWindow::export_csv_file()
{
	QString comboBoxQualiteCurrentText(comboBox_qualite->currentText());

	if (! YerothUtils::isEqualCaseInsensitive(YerothTableauxDeBordWindow::QUALITE_ZERO,
											  comboBoxQualiteCurrentText))
	{
		return false;
	}

	if (! YerothUtils::isEqualCaseInsensitive(YerothTableauxDeBordWindow::OBJET_ARTICLES,
											  comboBox_objets->currentText()))
	{
		return false;
	}

	QString csvFileName("articles-zero-chiffre-daffaire");

    QString yerothStocksListingCSVFileName(QString("%1/%2")
    										.arg(YerothERPConfig::temporaryFilesDir,
    											 csvFileName));

    yerothStocksListingCSVFileName = FILE_NAME_USERID_CURRENT_TIME(yerothStocksListingCSVFileName);

    yerothStocksListingCSVFileName =
    		QFileDialog::getSaveFileName(this,
    									 QObject::tr("Saisir le nom du fichier '.csv'"),
										 yerothStocksListingCSVFileName,
										 QObject::trUtf8("articles avec chiffre d'affaire nul \"*.csv\" (*.csv)"));

    statsZERO_Articles(yerothStocksListingCSVFileName.append(".csv"));

    _reportTexFileEndString.clear();

	return true;
}


void YerothTableauxDeBordWindow::remove_BAR_PIE_CHART_OPTION_FOR_ZERO_BUSINESS_TURNOVER(const QString &comboBoxQualiteCurrentText)
{
	//qDebug() << QString("remove_BAR_PIE_CHART_OPTION_FOR_ZERO_BUSINESS_TURNOVER: %1")
	//				.arg(comboBoxQualiteCurrentText);
	if (YerothUtils::isEqualCaseInsensitive(YerothTableauxDeBordWindow::QUALITE_ZERO,
											comboBoxQualiteCurrentText))
	{
		YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionExporter_au_format_csv, true);

		_objetServiceLastIndex = comboBox_objets->
				findText(YerothTableauxDeBordWindow::OBJET_SERVICES);

		if (-1 != _objetServiceLastIndex)
		{
			comboBox_objets->removeItem(_objetServiceLastIndex);
		}

		_objetClientLastIndex = comboBox_objets->
				findText(YerothTableauxDeBordWindow::OBJET_CLIENTS);

		if (-1 != _objetClientLastIndex)
		{
			comboBox_objets->removeItem(_objetClientLastIndex);
		}

		label_comparaison_chiffres_daffaires_quantite->setVisible(false);

		lineEdit_quantite->setVisible(false);
		comboBox_type_graphes->setVisible(false);
	}
	else
	{
		YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionExporter_au_format_csv, false);

		if (-1 != _objetServiceLastIndex)
		{
			comboBox_objets->insertItem(_objetServiceLastIndex,
					YerothTableauxDeBordWindow::OBJET_SERVICES);

			_objetServiceLastIndex = -1;
		}

		if (-1 != _objetClientLastIndex)
		{
			comboBox_objets->insertItem(_objetClientLastIndex,
					YerothTableauxDeBordWindow::OBJET_CLIENTS);

			_objetClientLastIndex = -1;
		}

		label_comparaison_chiffres_daffaires_quantite->setVisible(true);

		lineEdit_quantite->setVisible(true);
		comboBox_type_graphes->setVisible(true);
	}
}


void YerothTableauxDeBordWindow::changeLineEditEvolutionObjetsTextSetup(const QString &comboBoxEvolutionObjetsCurrentText)
{
	//qDebug() << QString("changeLineEditEvolutionObjetsTextSetup: %1")
	//				.arg(comboBoxEvolutionObjetsCurrentText);
	if (YerothUtils::isEqualCaseInsensitive(comboBoxEvolutionObjetsCurrentText, YerothTableauxDeBordWindow::OBJET_ARTICLES))
	{
		lineEdit_evolution_objets_value->setupMyQCompleter(YerothDatabase::MARCHANDISES, 2, Qt::AscendingOrder, false);
	}
	else if (YerothUtils::isEqualCaseInsensitive(comboBoxEvolutionObjetsCurrentText, YerothTableauxDeBordWindow::OBJET_CAISSIERS))
	{
		lineEdit_evolution_objets_value->setupMyQCompleter(YerothDatabase::USERS, 3, Qt::AscendingOrder, false);
	}
	else if (YerothUtils::isEqualCaseInsensitive(comboBoxEvolutionObjetsCurrentText, YerothTableauxDeBordWindow::OBJET_CATEGORIES))
	{
		lineEdit_evolution_objets_value->setupMyQCompleter(YerothDatabase::CATEGORIES, 1, Qt::AscendingOrder, false);
	}
	else if (YerothUtils::isEqualCaseInsensitive(comboBoxEvolutionObjetsCurrentText, YerothTableauxDeBordWindow::OBJET_CLIENTS))
	{
		lineEdit_evolution_objets_value->setupMyQCompleter(YerothDatabase::CLIENTS, 2, Qt::AscendingOrder, false);
	}
	else if (YerothUtils::isEqualCaseInsensitive(comboBoxEvolutionObjetsCurrentText, YerothTableauxDeBordWindow::OBJET_FOURNISSEURS))
	{
		lineEdit_evolution_objets_value->setupMyQCompleter(YerothDatabase::FOURNISSEURS, 1, Qt::AscendingOrder, false);
	}
}


void YerothTableauxDeBordWindow::bilanComptable()
{
    _logger->log("bilanComptable");

    if (dateEdit_bilan_comptable_debut->date() > dateEdit_bilan_comptable_fin->date())
    {
        YerothQMessageBox::warning(this, QObject::tr("bilan comptable"),
                                  QObject::trUtf8("La date de 'début' doit être"
                                          " antérieure à la date de 'fin' !"));

        return ;
    }

	QSqlQuery query;

    QString strDetteClientelleQuery(QString("SELECT %1 FROM %2 WHERE %3 < 0")
    									.arg(YerothDatabaseTableColumn::COMPTE_CLIENT,
    										 YerothDatabase::CLIENTS,
											 YerothDatabaseTableColumn::COMPTE_CLIENT));

//    qDebug() << QString("++ strAchatsQuery: %1")
//    				.arg(strAchatsQuery);

	int detteClientelleSize = YerothUtils::execQuery(query, strDetteClientelleQuery, _logger);

	double dette_clientelle = 0.0;

	double montant_total_dette_clientelle = 0.0;

    for( int k = 0; k < detteClientelleSize && query.next(); ++k)
    {
    	dette_clientelle = query.value(YerothDatabaseTableColumn::COMPTE_CLIENT).toDouble();

    	montant_total_dette_clientelle = montant_total_dette_clientelle + dette_clientelle;
    }

//    qDebug() << QString("++ detteClientelleSize: %1, montant_total_dette_clientelle: %2")
//    				.arg(QString::number(detteClientelleSize),
//    					 QString::number(montant_total_dette_clientelle, 'f', 2));

    query.clear();

    QString strAchatsQuery(QString("SELECT %1, %2 FROM %3 WHERE %4 >= '%5' AND %6 <= '%7'")
    							.arg(YerothDatabaseTableColumn::PRIX_DACHAT,
    								 YerothDatabaseTableColumn::QUANTITE_TOTALE,
									 YerothDatabase::ACHATS,
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

    // We negate stock item buying amount because it is
    // an expense of the company !
    montant_total_achat = -1 * montant_total_achat;

//    qDebug() << QString("++ achatQuerySize: %1, montant_total_achat: %2")
//    				.arg(QString::number(achatQuerySize),
//    					 QString::number(montant_total_achat, 'f', 2));


    //CALCUL TVA ENGRANGE POUR SERVICES NON STOCKES.

    query.clear();

    QString strVentes__SERVICES_NON_STOCKES_charge_TVA__Query =
    		QString("SELECT %1 FROM %2 WHERE %3 >= '%4' AND %5 <= '%6'")
			.arg(YerothDatabaseTableColumn::MONTANT_TVA,
					YerothDatabase::SERVICES_COMPLETES,
					YerothDatabaseTableColumn::DATE_VENTE,
					DATE_TO_DB_FORMAT_STRING(dateEdit_bilan_comptable_debut->date()),
					YerothDatabaseTableColumn::DATE_VENTE,
					DATE_TO_DB_FORMAT_STRING(dateEdit_bilan_comptable_fin->date()));

    //    qDebug() << QString("++ strVentesQuery: %1")
    //    				.arg(strVentesQuery);

    int ventes__SERVICES_NON_STOCKES_TVA_QuerySize =
    		YerothUtils::execQuery(query, strVentes__SERVICES_NON_STOCKES_charge_TVA__Query, _logger);


    double montant_tva = 0.0;

    double montant_TOTAL_TVA_COLLECTE = 0.0;

    for( int k = 0; k < ventes__SERVICES_NON_STOCKES_TVA_QuerySize && query.next(); ++k)
    {
    	montant_tva = query.value(YerothDatabaseTableColumn::MONTANT_TVA).toDouble();

    	montant_TOTAL_TVA_COLLECTE = montant_TOTAL_TVA_COLLECTE + montant_tva;
    }



    //VENTES D'ARTICLES PHYSIQUES
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
    	stocks_id = query.value(YerothDatabaseTableColumn::STOCKS_ID).toInt();

    	qte_vendue = query.value(YerothDatabaseTableColumn::QUANTITE_VENDUE).toDouble();

    	remise_prix_vente = query.value(YerothDatabaseTableColumn::REMISE_PRIX).toDouble();

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

    QString strVersementsQuery(QString("SELECT %1, %2 FROM %3 WHERE (%4 IS NOT NULL) AND (%5 >= '%6') AND (%7 <= '%8')")
    							.arg(YerothDatabaseTableColumn::MONTANT_PAYE,
    								 YerothDatabaseTableColumn::TYPE_DE_PAIEMENT,
    								 YerothDatabase::PAIEMENTS,
    								 YerothDatabaseTableColumn::COMPTE_CLIENT,
									 YerothDatabaseTableColumn::DATE_PAIEMENT,
									 DATE_TO_DB_FORMAT_STRING(dateEdit_bilan_comptable_debut->date()),
									 YerothDatabaseTableColumn::DATE_PAIEMENT,
									 DATE_TO_DB_FORMAT_STRING(dateEdit_bilan_comptable_fin->date())));

//    qDebug() << QString("++ strVersementsQuery: %1")
//    				.arg(strVersementsQuery);

	int versementsQuerySize = YerothUtils::execQuery(query, strVersementsQuery, _logger);

	int type_de_paiement = YerothUtils::ENCAISSEMENT_INDEFINI;

	double montant_paye = 0.0;

    double montant_total_versements = 0.0;


	double montant_paye__PAR_FIDELITE_CLIENTS = 0.0;

    double montant_total_versements__PAR_FIDELITE_CLIENTS = 0.0;


    for( int k = 0; k < versementsQuerySize && query.next(); ++k)
    {
    	type_de_paiement = query.value(YerothDatabaseTableColumn::TYPE_DE_PAIEMENT).toInt();

    	if (YerothUtils::is_montant_payer_AU_CLIENT__PAR_FIDELITE__valide(type_de_paiement))
    	{
    		montant_paye__PAR_FIDELITE_CLIENTS =
    				query.value(YerothDatabaseTableColumn::MONTANT_PAYE).toDouble();

    		montant_total_versements__PAR_FIDELITE_CLIENTS =
    				montant_total_versements__PAR_FIDELITE_CLIENTS +
					montant_paye__PAR_FIDELITE_CLIENTS;
    	}

    	if (YerothUtils::is_montant_payer_par_le_client_valide(type_de_paiement))
    	{
    		montant_paye = query.value(YerothDatabaseTableColumn::MONTANT_PAYE).toDouble();

    		montant_total_versements = montant_total_versements + montant_paye;
    	}
    }

//    qDebug() << QString("++ versementsQuerySize: %1, montant_total_versements: %2")
//    				.arg(QString::number(versementsQuerySize),
//    					 QString::number(montant_total_versements, 'f', 2));

    query.clear();

    QString strPaiementsAuComptesFournisseursQuery(QString("SELECT %1, %2 FROM %3 WHERE (%4 IS NOT NULL) AND (%5 >= '%6') AND (%7 <= '%8')")
    							.arg(YerothDatabaseTableColumn::MONTANT_PAYE,
    								 YerothDatabaseTableColumn::TYPE_DE_PAIEMENT,
    								 YerothDatabase::PAIEMENTS,
    								 YerothDatabaseTableColumn::COMPTE_FOURNISSEUR,
									 YerothDatabaseTableColumn::DATE_PAIEMENT,
									 DATE_TO_DB_FORMAT_STRING(dateEdit_bilan_comptable_debut->date()),
									 YerothDatabaseTableColumn::DATE_PAIEMENT,
									 DATE_TO_DB_FORMAT_STRING(dateEdit_bilan_comptable_fin->date())));

//    qDebug() << QString("++ strVersementsQuery: %1")
//    				.arg(strVersementsQuery);

	int paiementsAuComptesFournisseursQuerySize =
			YerothUtils::execQuery(query,
								   strPaiementsAuComptesFournisseursQuery,
								   _logger);

	type_de_paiement = YerothUtils::DECAISSEMENT_INDEFINI;


	double montant_paye_au_fournisseur = 0.0;

    double montant_total_paiements_aux_fournisseurs = 0.0;

    for( int k = 0; k < paiementsAuComptesFournisseursQuerySize && query.next(); ++k)
    {
    	type_de_paiement = query.value(YerothDatabaseTableColumn::TYPE_DE_PAIEMENT).toInt();

    	if (YerothUtils::is_montant_payer_au_fournisseur_valide(type_de_paiement))
    	{
    		montant_paye_au_fournisseur = query.value(YerothDatabaseTableColumn::MONTANT_PAYE).toDouble();

    		montant_total_paiements_aux_fournisseurs = montant_total_paiements_aux_fournisseurs + montant_paye_au_fournisseur;
    	}
    }

//    qDebug() << QString("++ paiementsAuComptesFournisseursQuerySize: %1, montant_total_paiements_aux_fournisseurs: %2")
//    				.arg(QString::number(paiementsAuComptesFournisseursQuerySize),
//    					 QString::number(montant_total_paiements_aux_fournisseurs, 'f', 2));

    query.clear();


    double total_entrees = montant_total_vente +
    					   montant_total_versements;


    double total_sorties = montant_total_versements__PAR_FIDELITE_CLIENTS +
    					   montant_TOTAL_TVA_COLLECTE 					  +
    					   montant_total_achat 							  +
    					   montant_total_paiements_aux_fournisseurs 	  +
						   montant_total_dette_clientelle;


    double achats_depenses_financieres_effectues = montant_total_paiements_aux_fournisseurs;


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
        					 YerothDatabase::ACHATS,
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

    benefice_sur_vente_effectuees = benefice_sur_vente_effectuees -
    								montant_total_dette_clientelle;

    benefice_sur_vente_effectuees = benefice_sur_vente_effectuees -
    								qFabs(montant_total_versements__PAR_FIDELITE_CLIENTS);

    if (benefice_sur_vente_effectuees <= 0)
    {
    	benefice_sur_vente_effectuees = 0;
    }

    chiffre_daffaire = montant_total_vente;

    balance = total_entrees - qFabs(total_sorties);

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

    QString fileDate(YerothUtils::LATEX_IN_OUT_handleForeignAccents(infoEntreprise.getVille_LATEX()));

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

    texDocument.replace("YEROTHBILANCOMPTABLETVAENGRANGE", GET_CURRENCY_STRING_NUM_FOR_LATEX(montant_TOTAL_TVA_COLLECTE));

    texDocument.replace("YEROTHBILANCOMPTABLETOTALSORTIESDEVISE", GET_CURRENCY_STRING_NUM_FOR_LATEX(total_sorties));

    QString balanceDeviseLatexStr(GET_CURRENCY_STRING_NUM_FOR_LATEX(balance));

    if (balance >= 0)
    {
    	balanceDeviseLatexStr = YerothUtils::colorLatexTextInput("yerothColorGreen", balanceDeviseLatexStr);
    }
    else
    {
    	balanceDeviseLatexStr = YerothUtils::colorLatexTextInput("yerothColorRed", balanceDeviseLatexStr);
    }

    texDocument.replace("YEROTHBILANCOMPTABLEBALANCEDEVISE", balanceDeviseLatexStr);

	texDocument.replace("YEROTHBILANCOMPTABLEPROGRAMMEFIDELITECLIENTS",
			GET_CURRENCY_STRING_NUM_FOR_LATEX(montant_total_versements__PAR_FIDELITE_CLIENTS));

	texDocument.replace("YEROTHBILANCOMPTABLECHARGESDEPENSESFINANCIERES", GET_CURRENCY_STRING_NUM_FOR_LATEX(achats_depenses_financieres_effectues));

    texDocument.replace("YEROTHBILANCOMPTABLEBENEFICEDEVISE", GET_CURRENCY_STRING_NUM_FOR_LATEX(benefice_sur_vente_effectuees));

    texDocument.replace("YEROTHBILANCOMPTABLECHIFFREDAFFAIREDEVISE", GET_CURRENCY_STRING_NUM_FOR_LATEX(total_entrees));

    texDocument.replace("YEROTHPAPERSPEC", "a4paper");
    texDocument.replace("YEROTHENTREPRISE", infoEntreprise.getNomCommercial_LATEX());
    texDocument.replace("YEROTHACTIVITESENTREPRISE", infoEntreprise.getSecteursActivitesTex());
    texDocument.replace("YEROTHBOITEPOSTALE", infoEntreprise.getBoitePostal());
    texDocument.replace("YEROTHVILLE", infoEntreprise.getVille_LATEX());
    texDocument.replace("YEROTHPAYS", infoEntreprise.getPaysTex());
    texDocument.replace("YEROTHEMAIL", infoEntreprise.getEmail_LATEX());
    texDocument.replace("YEROTHTELEPHONE", infoEntreprise.getTelephone());
    texDocument.replace("YEROTHDATE", fileDate);

    texDocument.replace("YEROTHNOMUTILISATEUR", QString("%1 %2")
       												.arg(YerothUtils::getAllWindows()->getUser()->titreTex(),
       													 YerothUtils::getAllWindows()->getUser()->nom_completTex()));

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


void YerothTableauxDeBordWindow::analyse_comparee_jour_semaine()
{
	_logger->log("analyse_comparee_jour_semaine");

	if (dateEdit_chiffre_daffaire_jour_semaine_fin->date() <
		dateEdit_chiffre_daffaire_jour_semaine_debut->date())
	{
		YerothQMessageBox::warning(this, QObject::trUtf8("évolution du chiffre d'affaire"),
				QObject::trUtf8("Le jour de 'début' doit être "
						"antérieur au jour de la 'fin' !"));
		return ;
	}

	QMap<int, double> dayOfWeek__TO__purchases;

	QMap<int, double> dayOfWeek__TO__businessturnover;

	qint64 dates_range = dateEdit_chiffre_daffaire_jour_semaine_debut->date()
    					.daysTo(dateEdit_chiffre_daffaire_jour_semaine_fin->date()) + 1;


	QString string_achats_semaine_query;

	QString string_chiffre_daffaire_semaine_query;

	int current_day_of_week;

	QSqlQuery qsql_query;

	QDateTime current_day_date = dateEdit_chiffre_daffaire_jour_semaine_debut->dateTime();

	double a_temp_day_of_week_purchases = 0.0;

	double a_temp_day_of_week_business_turnover = 0.0;

	double current_day_purchases = 0.0;

	double current_day_business_turnover = 0.0;

	int current_query_size;

	qint64 i = 0;

	bool filtreActif = false;

	QString AUCUN_FILTRE(QObject::tr("aucun filtre !"));

	QString yerothFiltre;

	QString databaseTableColumn;

	QString comboBoxEvolutionObjetsCurrentText(comboBox_evolution_objets->currentText());

	QString textFromLineEditEvolutionSujets(lineEdit_evolution_objets_value->text());

	do
	{
		current_day_of_week = current_day_date.date().dayOfWeek();

		string_achats_semaine_query =
				QString("SELECT (%1 * %2) FROM %3 WHERE %4='%5'")
				.arg(YerothDatabaseTableColumn::PRIX_DACHAT,
					 YerothDatabaseTableColumn::QUANTITE_TOTALE,
					 YerothDatabase::ACHATS,
					 YerothDatabaseTableColumn::DATE_ENTREE,
					 DATE_TO_DB_FORMAT_STRING(current_day_date));


		string_chiffre_daffaire_semaine_query =
				QString("SELECT (%1 - %2) FROM %3 WHERE %4='%5'")
				.arg(YerothDatabaseTableColumn::MONTANT_TOTAL_VENTE,
						YerothDatabaseTableColumn::MONTANT_TVA,
						YerothDatabase::STOCKS_VENDU,
						YerothDatabaseTableColumn::DATE_VENTE,
						DATE_TO_DB_FORMAT_STRING(current_day_date));


		bool filtreActif = false;

		AUCUN_FILTRE = QObject::tr("aucun filtre !");

		yerothFiltre.clear();

		databaseTableColumn.clear();

		comboBoxEvolutionObjetsCurrentText = comboBox_evolution_objets->currentText();

		textFromLineEditEvolutionSujets = lineEdit_evolution_objets_value->text();


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

		//		qDebug() << QString("databaseTableColumn: %1, currentTextFromComboBoxEvolutionSujets: %2")
		//						.arg(databaseTableColumn, comboBox_evolution_objets->currentText());

		if (true == filtreActif)
		{
			if (textFromLineEditEvolutionSujets.isEmpty())
			{
				YerothQMessageBox::warning(this, QObject::trUtf8("paramètre manquant"),
						QObject::trUtf8("Vous devez spécifier un paramètre dans"
								" le champs de texte 'nom' !"));
				return ;
			}
			else
			{
				if (!databaseTableColumn.isEmpty())
				{
					string_achats_semaine_query.append(QString(" AND %1='%2'")
							.arg(databaseTableColumn,
								 textFromLineEditEvolutionSujets));

					string_chiffre_daffaire_semaine_query.append(QString(" AND %1='%2'")
							.arg(databaseTableColumn,
								 textFromLineEditEvolutionSujets));
				}
			}
		}

//		qDebug() << "++ string_achats_semaine_query: " << string_achats_semaine_query;

		qsql_query.clear();

		current_query_size = YerothUtils::execQuery(qsql_query, string_achats_semaine_query);

		if (current_query_size > 0)
		{
			while (qsql_query.next())
			{
				current_day_purchases = qsql_query.value(0).toDouble();

				QString STRING_current_day_of_week =
						YerothUtils::GET_DAYOFWEEK_FROM_QT_INT_CONSTANT(current_day_of_week);

				if (! YerothUtils::isEqualCaseInsensitive(YerothUtils::EMPTY_STRING, STRING_current_day_of_week))
				{
					a_temp_day_of_week_purchases =
							dayOfWeek__TO__purchases[current_day_of_week];

					dayOfWeek__TO__purchases.insert(current_day_of_week,
							current_day_purchases +
							a_temp_day_of_week_purchases);
				}
			}
		}

//		qDebug() << "++ string_chiffre_daffaire_semaine_query: " << string_chiffre_daffaire_semaine_query;

		qsql_query.clear();

		current_query_size = YerothUtils::execQuery(qsql_query, string_chiffre_daffaire_semaine_query);

		if (current_query_size > 0)
		{
			while (qsql_query.next())
			{
				current_day_business_turnover = qsql_query.value(0).toDouble();

				QString STRING_current_day_of_week =
						YerothUtils::GET_DAYOFWEEK_FROM_QT_INT_CONSTANT(current_day_of_week);

				if (! YerothUtils::isEqualCaseInsensitive(YerothUtils::EMPTY_STRING, STRING_current_day_of_week))
				{
					a_temp_day_of_week_business_turnover =
							dayOfWeek__TO__businessturnover[current_day_of_week];

					dayOfWeek__TO__businessturnover.insert(current_day_of_week,
							current_day_business_turnover +
							a_temp_day_of_week_business_turnover);
				}
			}
		}

		current_day_date = current_day_date.addDays(1);

		++i;
	}
	while(i < dates_range);

	//	qDebug() << dayOfWeek__TO__businessturnover;

	if (dayOfWeek__TO__purchases.isEmpty() 			||
		dayOfWeek__TO__businessturnover.isEmpty()	||
		dayOfWeek__TO__purchases.size() != dayOfWeek__TO__businessturnover.size())
	{
		YerothQMessageBox::information(this,
				QObject::trUtf8("pas de données"),
				QObject::trUtf8("Il n'y a pas de données correspondante à la requête !\n"
								"Vérifier que les dates de début et de fin, "
								"sont correctes !"));
		return ;
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
	_reportTexFileEndString.append("\\begin{enumerate}[1.]\n");

	//Fill in the PDF file which amount of money for each day of week.

	double somme_totale_ventes_jour_semaine = 0.0;

	QMap<int, QString> ventes_achats_info;

	QMapIterator<int, double> it(dayOfWeek__TO__businessturnover);

	while(it.hasNext())
	{
		it.next();

		somme_totale_ventes_jour_semaine += it.value();

//		_reportTexFileEndString.append("\\item \\textbf{")

#ifdef YEROTH_FRANCAIS_LANGUAGE
		ventes_achats_info.insert(it.key(),
								  QString("\\item \\textbf{%1}: ventes $\\rightarrow %2$, ")
									 .arg(YerothUtils::LATEX_IN_OUT_handleForeignAccents(YerothUtils::GET_DAYOFWEEK_FROM_QT_INT_CONSTANT(it.key())),
										  YerothUtils::LATEX_IN_OUT_handleForeignAccents(GET_CURRENCY_STRING_NUM(it.value()))));
#else //YEROTH_ENGLISH_LANGUAGE
		ventes_achats_info.insert(it.key(),
								   QString("\\item \\textbf{%1}: sales $\\rightarrow %2$, ")
								   	   .arg(YerothUtils::LATEX_IN_OUT_handleForeignAccents(YerothUtils::GET_DAYOFWEEK_FROM_QT_INT_CONSTANT(it.key())),
								   			YerothUtils::LATEX_IN_OUT_handleForeignAccents(GET_CURRENCY_STRING_NUM(it.value()))));
#endif
	}

//	QDEBUG_STRING_OUTPUT_2_N("somme_totale_ventes_jour_semaine", somme_totale_ventes_jour_semaine);

	double somme_totale_achats_jour_semaine = 0.0;

	QString current_prefix_string_vente_info;

	QMapIterator<int, double> it_achats(dayOfWeek__TO__purchases);

	for (int z = 0; z < ventes_achats_info.size(); ++z)
	{
		if (it_achats.hasNext())
		{
			it_achats.next();

			somme_totale_achats_jour_semaine += it_achats.value();

			current_prefix_string_vente_info = ventes_achats_info.value(it_achats.key());

			if (! current_prefix_string_vente_info.isEmpty())
			{
			#ifdef YEROTH_FRANCAIS_LANGUAGE
				current_prefix_string_vente_info.append(
						QString("achats $\\rightarrow %1$\n")
						.arg(YerothUtils::LATEX_IN_OUT_handleForeignAccents(GET_CURRENCY_STRING_NUM(it_achats.value()))));

			#else //YEROTH_ENGLISH_LANGUAGE
				current_prefix_string_vente_info.append(
						QString("sales $\\rightarrow %1$\n")
						.arg(YerothUtils::LATEX_IN_OUT_handleForeignAccents(GET_CURRENCY_STRING_NUM(it_achats.value()))));
			#endif

				ventes_achats_info.insert(it_achats.key(), current_prefix_string_vente_info);

				_reportTexFileEndString.append(ventes_achats_info.value(it_achats.key()));
			}
			else
			{
				YerothQMessageBox::information(this,
						QObject::trUtf8("pas de données"),
						QObject::trUtf8("Il n'y a pas de données correspondante à la requête !\n"
								"Vérifier que les dates de début et de fin, "
								"sont correctes !"));
				return ;
			}
		}
	}

	_reportTexFileEndString.append("\\end{enumerate}\n");

	QMap<int, QString> barItems;

	double MAX_AMOUNT_ACHATS_VENTES = somme_totale_achats_jour_semaine +
									  somme_totale_ventes_jour_semaine;

	double ratio_achats = 0.0;

	double ratio_ventes = 0.0;

	const int TICKS = 100;

	const double MAX_RATIO = 900.0;

	it.toFront();

	for (int x = 0; x < dayOfWeek__TO__businessturnover.size(); ++x)
	{
		if (it.hasNext())
		{
			it.next();

			ratio_ventes = (it.value() * MAX_RATIO) / MAX_AMOUNT_ACHATS_VENTES;

			barItems.insert(it.key(), QString("\\baritem{%1}{%2}{gray}\n")
					.arg(YerothUtils::LATEX_IN_OUT_handleForeignAccents(YerothUtils::GET_DAYOFWEEK_FROM_QT_INT_CONSTANT(it.key())),
							QString::number(ratio_ventes, 'f', 2)));
		}
	}

//	QDEBUG_QSTRINGLIST_OUTPUT("barItems - VENTES", barItems.values().join(" "));

	int j_it_achats = 0;

	int max_size = barItems.size();

	it_achats.toFront();

	QString sub_bar_achat;

	for (int l = 0; l < dayOfWeek__TO__businessturnover.size(); ++l)
	{
		if (it_achats.hasNext())
		{
			it_achats.next();

			ratio_achats = (it_achats.value() * MAX_RATIO) / MAX_AMOUNT_ACHATS_VENTES;

			sub_bar_achat = QString(" \\subbaritem{}{%1}{purplish}\n")
															.arg(QString::number(ratio_achats, 'f', 2));

			barItems.insert(it_achats.key(), QString("%1%2")
												.arg(barItems.value(it_achats.key()),
													 sub_bar_achat));
		}
	}

//	qDebug() << "++ barItems: " << barItems.values().join(YerothUtils::EMPTY_STRING);

	QProcess aProcess;

	aProcess.setWorkingDirectory(YerothERPConfig::temporaryFilesDir);

	QString texDocument;
	texDocument.append(YerothUtils::_1a_tex);


	texDocument.replace("YEROTHBARPERGROUP", "");
	texDocument.replace("YEROTHBARITEMS", 				barItems.values().join(YerothUtils::EMPTY_STRING));
	texDocument.replace("YEROTHTICKS", 					QString::number(TICKS));

#ifdef YEROTH_FRANCAIS_LANGUAGE
    texDocument.replace("YEROTHLEGENDANALYSECOMPAREE", 	"\\diagLegenditem{Ratio des achats jour-semaine.}{purplish}");
    texDocument.replace("YEROTHDIAGRAMMETITRE", 		"Ratio du chiffre d'affaire jour-semaine.");
    texDocument.replace("YEROTHNIVEAUCHIFFREAFFAIRE", 	"Niveau du chiffre d'affaire");
#endif

#ifdef YEROTH_ENGLISH_LANGUAGE
    texDocument.replace("YEROTHLEGENDANALYSECOMPAREE", 	"\\diagLegenditem{Ratio of days of week purchasing.}{purplish}");
    texDocument.replace("YEROTHDIAGRAMMETITRE", 		"Ratio of days of week income.");
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


QString factureDate(YerothUtils::LATEX_IN_OUT_handleForeignAccents(infoEntreprise.getVille_LATEX()));
YerothUtils::getCurrentLocaleDate(factureDate);

QString longDateDebut;
QString longDateFin;

#ifdef YEROTH_FRANCAIS_LANGUAGE
longDateDebut = QString("'%1'")
    	                    		.arg(YerothUtils::LATEX_IN_OUT_handleForeignAccents(
    	                    				YerothUtils::frenchLocale.toString(dateEdit_chiffre_daffaire_jour_semaine_debut->date())));

longDateFin = QString("'%1'")
    	                		  .arg(YerothUtils::LATEX_IN_OUT_handleForeignAccents(
    	                				  YerothUtils::frenchLocale.toString(dateEdit_chiffre_daffaire_jour_semaine_fin->date())));
#endif

#ifdef YEROTH_ENGLISH_LANGUAGE
longDateDebut = QString("'%1'")
    	                    		.arg(YerothUtils::LATEX_IN_OUT_handleForeignAccents(
    	                    				YerothUtils::englishLocale.toString(dateEdit_chiffre_daffaire_jour_semaine_debut->date())));

longDateFin = QString("'%1'")
    	                		  .arg(YerothUtils::LATEX_IN_OUT_handleForeignAccents(
    	                				  YerothUtils::englishLocale.toString(dateEdit_chiffre_daffaire_jour_semaine_fin->date())));
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

texDocument2.replace("YEROTHCHIFFREAFFAIREDATEDEBUT",	longDateDebut);
texDocument2.replace("YEROTHCHIFFREAFFAIREDATEFIN", 	longDateFin);
texDocument2.replace("YEROTHCHARTFIN", 					_reportTexFileEndString);
texDocument2.replace("YEROTHENTREPRISE", 				infoEntreprise.getNomCommercial_LATEX());
texDocument2.replace("YEROTHACTIVITESENTREPRISE", 		infoEntreprise.getSecteursActivitesTex());
texDocument2.replace("YEROTHBOITEPOSTALE", 				infoEntreprise.getBoitePostal());
texDocument2.replace("YEROTHVILLE", 					infoEntreprise.getVille_LATEX());
texDocument2.replace("YEROTHPAYS", 						infoEntreprise.getPaysTex());
texDocument2.replace("YEROTHEMAIL", 					infoEntreprise.getEmail_LATEX());
texDocument2.replace("YEROTHTELEPHONE", 				infoEntreprise.getTelephone());
texDocument2.replace("YEROTHDATE", 						factureDate);

texDocument2.replace("YEROTHNOMUTILISATEUR", QString("%1 %2")
		.arg(YerothUtils::getAllWindows()->getUser()->titreTex(),
				YerothUtils::getAllWindows()->getUser()->nom_completTex()));

texDocument2.replace("YEROTHHEUREDIMPRESSION",		CURRENT_TIME);
texDocument2.replace("YEROTHCOMPTEBANCAIRENR", 		infoEntreprise.getNumeroCompteBancaire());
texDocument2.replace("YEROTHCONTRIBUABLENR", 		infoEntreprise.getNumeroDeContribuable());
texDocument2.replace("YEROTHAGENCECOMPTEBANCAIRE",	infoEntreprise.getAgenceCompteBancaireTex());

texDocument2.replace("1a.tex", fileName1);

#ifdef YEROTH_FRANCAIS_LANGUAGE
texDocument2.replace("YEROTHTITREDOCUMENT",
		QString("Diagramme r\\'epr\\'esentatif des chiffres"
				" d'affaire par jour de la semaine du %1 au %2.").arg(longDateDebut, longDateFin));
#endif

#ifdef YEROTH_ENGLISH_LANGUAGE
texDocument2.replace("YEROTHTITREDOCUMENT",
		QString("Chart illustrating days of week income from %1 to %2.")
		.arg(longDateDebut, longDateFin));
#endif

//qDebug() << "++ test: " << texDocument2;

YerothUtils::LATEX_IN_OUT_handleForeignAccents(texDocument2);

QString fileName(FILE_NAME_USERID_CURRENT_TIME("evolution-chiffre-affaire-jour-semaine"));
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

//	    qDebug() << "++ file name to latex compile bar diag: " << texRm;

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

//	    qDebug() << "++ test it: " << tmpFilePrefix + "pdf";

aProcess.startDetached(YerothERPConfig::pathToPdfReader, progArguments);

//qDebug() << "\texit status evince: " << _aProcess->exitStatus();
}


void YerothTableauxDeBordWindow::analyse_comparee_mensuelle()
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
								 YerothDatabase::ACHATS,
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
    		.append(YerothDatabase::STOCKS_VENDU)
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
	        YerothQMessageBox::warning(this, QObject::trUtf8("paramètre manquant"),
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
		    								YerothUtils::LATEX_IN_OUT_handleForeignAccents(textFromLineEditEvolutionSujets)));
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
    			QObject::trUtf8("analyse comparée"),
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
    _reportTexFileEndString.append("\\begin{enumerate}[1.]\n");

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
					.arg(YerothUtils::LATEX_IN_OUT_handleForeignAccents(YerothUtils::englishLocale.monthName(k)),
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


    QString factureDate(YerothUtils::LATEX_IN_OUT_handleForeignAccents(infoEntreprise.getVille_LATEX()));
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
    texDocument2.replace("YEROTHENTREPRISE", 			infoEntreprise.getNomCommercial_LATEX());
    texDocument2.replace("YEROTHACTIVITESENTREPRISE", 	infoEntreprise.getSecteursActivitesTex());
    texDocument2.replace("YEROTHBOITEPOSTALE", 			infoEntreprise.getBoitePostal());
    texDocument2.replace("YEROTHVILLE", 					infoEntreprise.getVille_LATEX());
    texDocument2.replace("YEROTHPAYS", 					infoEntreprise.getPaysTex());
    texDocument2.replace("YEROTHEMAIL", 					infoEntreprise.getEmail_LATEX());
    texDocument2.replace("YEROTHTELEPHONE", 				infoEntreprise.getTelephone());
    texDocument2.replace("YEROTHDATE", 					factureDate);

    texDocument2.replace("YEROTHNOMUTILISATEUR", QString("%1 %2")
       												.arg(YerothUtils::getAllWindows()->getUser()->titreTex(),
       													 YerothUtils::getAllWindows()->getUser()->nom_completTex()));

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


void YerothTableauxDeBordWindow::compterLesArticlesVendusParQuantite()
{
	QString objet(comboBox_objets->currentText());

	int size = lineEdit_quantite->text().toInt();

	QStringList progArguments;
	QString tmpFilePrefix;

	QDir tempDir(YerothERPConfig::temporaryFilesDir);

	QString csvFile;
	QString pdfFileTitle;

	if (YerothTableauxDeBordWindow::QUALITE_PLUS_VENDU_PAR_QUANTITE == comboBox_qualite->currentText() ||
		YerothTableauxDeBordWindow::QUALITE_MOINS_VENDU_PAR_QUANTITE == comboBox_qualite->currentText() )
	{
#ifdef YEROTH_FRANCAIS_LANGUAGE
		pdfFileTitle.append(QString("Les %1 ")
				.arg(lineEdit_quantite->text()));
#endif

#ifdef YEROTH_ENGLISH_LANGUAGE
		pdfFileTitle.append(QString("The %1 ")
				.arg(lineEdit_quantite->text()));
#endif
	}
	else
	{
#ifdef YEROTH_FRANCAIS_LANGUAGE
		pdfFileTitle.append("Les ");
#endif

#ifdef YEROTH_ENGLISH_LANGUAGE
		pdfFileTitle.append("The ");
#endif
	}

	if (YerothTableauxDeBordWindow::QUALITE_PLUS_VENDU_PAR_QUANTITE == comboBox_qualite->currentText())
	{
		if (YerothTableauxDeBordWindow::OBJET_SERVICES == objet)
		{
			pdfFileTitle.append(YerothTableauxDeBordWindow::OBJET_SERVICES);

#ifdef YEROTH_FRANCAIS_LANGUAGE
			tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("les-plus-vendus-par-quantite-services");
#endif

#ifdef YEROTH_ENGLISH_LANGUAGE
			tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("best-sold-by-quantity-services");
#endif

			csvFile = tmpFilePrefix + ".csv";

			csvFile.prepend(YerothERPConfig::temporaryFilesDir + "/");

			tempDir.remove(csvFile);

			statsQuantiteMaxServices(csvFile, size);
		}
		else if (YerothTableauxDeBordWindow::OBJET_ARTICLES == objet)
		{
			pdfFileTitle.append(YerothTableauxDeBordWindow::OBJET_ARTICLES);

#ifdef YEROTH_FRANCAIS_LANGUAGE
			tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("les-plus-vendus-par-quantite-articles");
#endif

#ifdef YEROTH_ENGLISH_LANGUAGE
			tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("best-sold-by-quantity-articles");
#endif

			csvFile = tmpFilePrefix + ".csv";

			csvFile.prepend(YerothERPConfig::temporaryFilesDir + "/");

			tempDir.remove(csvFile);

			statsQuantiteMaxArticles(csvFile, size);
		}
		else if (YerothTableauxDeBordWindow::OBJET_CATEGORIES == objet)
		{
#ifdef YEROTH_FRANCAIS_LANGUAGE
			pdfFileTitle.replace(YerothTableauxDeBordWindow::QUALITE_MEILLEURS, "meilleures");
#endif

#ifdef YEROTH_ENGLISH_LANGUAGE
			pdfFileTitle.replace(YerothTableauxDeBordWindow::QUALITE_MEILLEURS, "best");
#endif

			pdfFileTitle.append(YerothTableauxDeBordWindow::OBJET_CATEGORIES);

#ifdef YEROTH_FRANCAIS_LANGUAGE
			tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("les-plus-vendus-par-quantite-categories");
#endif

#ifdef YEROTH_ENGLISH_LANGUAGE
			tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("best-sold-by-quantity-categories");
#endif

			csvFile = tmpFilePrefix + ".csv";

			csvFile.prepend(YerothERPConfig::temporaryFilesDir + "/");

			tempDir.remove(csvFile);

			statsQuantiteMaxCategories(csvFile, size);
		}
		else if (YerothTableauxDeBordWindow::OBJET_CAISSIERS == objet)
		{
			pdfFileTitle.append(YerothTableauxDeBordWindow::OBJET_CAISSIERS);

#ifdef YEROTH_FRANCAIS_LANGUAGE
			tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("les-plus-vendus-par-quantite-caissiers");
#endif

#ifdef YEROTH_ENGLISH_LANGUAGE
			tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("best-sold-by-quantity-cashiers");
#endif

			csvFile = tmpFilePrefix + ".csv";

			csvFile.prepend(YerothERPConfig::temporaryFilesDir + "/");

			tempDir.remove(csvFile);

			statsQuantiteMaxCaissiers(csvFile, size);
		}
		else if (YerothTableauxDeBordWindow::OBJET_CLIENTS == objet)
		{
			pdfFileTitle.append(YerothTableauxDeBordWindow::OBJET_CLIENTS);

#ifdef YEROTH_FRANCAIS_LANGUAGE
			tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("les-plus-vendus-par-quantite-clients");
#endif

#ifdef YEROTH_ENGLISH_LANGUAGE
			tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("best-sold-by-quantity-customers");
#endif

			csvFile = tmpFilePrefix + ".csv";

			csvFile.prepend(YerothERPConfig::temporaryFilesDir + "/");

			tempDir.remove(csvFile);

			statsQuantiteMaxClients(csvFile, size);
		}
		else if (YerothTableauxDeBordWindow::OBJET_FOURNISSEURS == objet)
		{
			pdfFileTitle.append(YerothTableauxDeBordWindow::OBJET_FOURNISSEURS);

#ifdef YEROTH_FRANCAIS_LANGUAGE
			tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("les-plus-vendus-par-quantite-fournisseurs-ventes");
#endif

#ifdef YEROTH_ENGLISH_LANGUAGE
			tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("best-sold-by-quantity-supplier-sale");
#endif

			csvFile = tmpFilePrefix + ".csv";

			csvFile.prepend(YerothERPConfig::temporaryFilesDir + "/");

			tempDir.remove(csvFile);

			statsQuantiteMaxFournisseursVentes(csvFile, size);
		}

#ifdef YEROTH_FRANCAIS_LANGUAGE
		pdfFileTitle.append(" avec les quantit\\'es vendues les plus \\'elev\\'ees");
#endif

#ifdef YEROTH_ENGLISH_LANGUAGE
		pdfFileTitle.append(" giving the best sold by quantity");
#endif

	}
	else if (YerothTableauxDeBordWindow::QUALITE_MOINS_VENDU_PAR_QUANTITE == comboBox_qualite->currentText())
	{
		if (YerothTableauxDeBordWindow::OBJET_SERVICES == objet)
		{
			pdfFileTitle.append(YerothTableauxDeBordWindow::OBJET_SERVICES);

#ifdef YEROTH_FRANCAIS_LANGUAGE
			tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("les-moins-vendus-par-quantite-services");
#endif

#ifdef YEROTH_ENGLISH_LANGUAGE
			tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("least-sold-by-quantity-services");
#endif

			csvFile = tmpFilePrefix + ".csv";

			csvFile.prepend(YerothERPConfig::temporaryFilesDir + "/");

			tempDir.remove(csvFile);

			statsQuantiteMoindreServices(csvFile, size);
		}
		else if (YerothTableauxDeBordWindow::OBJET_ARTICLES == objet)
		{
			pdfFileTitle.append(YerothTableauxDeBordWindow::OBJET_ARTICLES);

#ifdef YEROTH_FRANCAIS_LANGUAGE
			tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("les-moins-vendus-par-quantite-articles");
#endif

#ifdef YEROTH_ENGLISH_LANGUAGE
			tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("least-sold-by-quantity-articles");
#endif

			csvFile = tmpFilePrefix + ".csv";

			csvFile.prepend(YerothERPConfig::temporaryFilesDir + "/");

			tempDir.remove(csvFile);

			statsQuantiteMoindreArticles(csvFile, size);
		}
		else if (YerothTableauxDeBordWindow::OBJET_CATEGORIES == objet)
		{
#ifdef YEROTH_FRANCAIS_LANGUAGE
			pdfFileTitle.replace(YerothTableauxDeBordWindow::QUALITE_MOINS_VENDU_PAR_QUANTITE, "les moins");
#endif

#ifdef YEROTH_ENGLISH_LANGUAGE
			pdfFileTitle.replace(YerothTableauxDeBordWindow::QUALITE_MOINS_VENDU_PAR_QUANTITE, "least");
#endif

			pdfFileTitle.append(YerothTableauxDeBordWindow::OBJET_CATEGORIES);

#ifdef YEROTH_FRANCAIS_LANGUAGE
			tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("les-moins-vendus-par-quantite-categories");
#endif

#ifdef YEROTH_ENGLISH_LANGUAGE
			tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("least-sold-by-quantity-categories");
#endif

			csvFile = tmpFilePrefix + ".csv";

			csvFile.prepend(YerothERPConfig::temporaryFilesDir + "/");

			tempDir.remove(csvFile);

			statsQuantiteMoindreCategories(csvFile, size);
		}
		else if (YerothTableauxDeBordWindow::OBJET_CAISSIERS == objet)
		{
			pdfFileTitle.append(YerothTableauxDeBordWindow::OBJET_CAISSIERS);

#ifdef YEROTH_FRANCAIS_LANGUAGE
			tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("les-moins-vendus-par-quantite-caissiers");
#endif

#ifdef YEROTH_ENGLISH_LANGUAGE
			tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("least-sold-by-quantity-cashiers");
#endif

			csvFile = tmpFilePrefix + ".csv";

			csvFile.prepend(YerothERPConfig::temporaryFilesDir + "/");

			tempDir.remove(csvFile);

			statsQuantiteMoindreCaissiers(csvFile, size);
		}
		else if (YerothTableauxDeBordWindow::OBJET_CLIENTS == objet)
		{
			pdfFileTitle.append(YerothTableauxDeBordWindow::OBJET_CLIENTS);

#ifdef YEROTH_FRANCAIS_LANGUAGE
			tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("les-moins-vendus-par-quantite-clients");
#endif

#ifdef YEROTH_ENGLISH_LANGUAGE
			tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("least-sold-by-quantity-customers");
#endif

			csvFile = tmpFilePrefix + ".csv";

			csvFile.prepend(YerothERPConfig::temporaryFilesDir + "/");

			tempDir.remove(csvFile);

			statsQuantiteMoindreClients(csvFile, size);
		}
		else if (YerothTableauxDeBordWindow::OBJET_FOURNISSEURS == objet)
		{
			pdfFileTitle.append(YerothTableauxDeBordWindow::OBJET_FOURNISSEURS);

#ifdef YEROTH_FRANCAIS_LANGUAGE
			tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("les-moins-vendus-par-quantite-fournisseurs-ventes");
#endif

#ifdef YEROTH_ENGLISH_LANGUAGE
			tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("least-sold-by-quantity-supplier-sale");
#endif

			csvFile = tmpFilePrefix + ".csv";

			csvFile.prepend(YerothERPConfig::temporaryFilesDir + "/");

			tempDir.remove(csvFile);

			statsQuantiteMoindreFournisseursVentes(csvFile, size);
		}

#ifdef YEROTH_FRANCAIS_LANGUAGE
		pdfFileTitle.append(" avec les quantit\\'es vendues les moins \\'elev\\'ees");
#endif

#ifdef YEROTH_ENGLISH_LANGUAGE
		pdfFileTitle.append(" giving the least sold by quantity");
#endif

	}

	if (_csvFileItemSize <= 0)
	{
		QString retMsg(QObject::trUtf8("Il n'y a pas de données correspondante à la requête !\n"
				"Vérifier que les dates de début et de fin sont correctes !"));

		YerothQMessageBox::information(this,
				QObject::trUtf8("rankings - pas de données !"),
				retMsg);
		_csvFileItemSize = 0;

		return ;
	}

	QString latexChartTemplate;
	QString latexChartFileNamePrefix;

#ifdef YEROTH_FRANCAIS_LANGUAGE
	if (YerothTableauxDeBordWindow::QUALITE_ZERO == comboBox_qualite->currentText())
	{
		latexChartTemplate.append(YerothUtils::FR_ZERO_ventes_tex);

		latexChartFileNamePrefix.append(YerothERPConfig::temporaryFilesDir + "/" + tmpFilePrefix + "_ZERO");
	}
	else if (YerothTableauxDeBordWindow::GRAPHE_BAR_CHART == comboBox_type_graphes->currentText())
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
	if (YerothTableauxDeBordWindow::QUALITE_ZERO == comboBox_qualite->currentText())
	{
		latexChartTemplate.append(YerothUtils::EN_ZERO_ventes_tex);

		latexChartFileNamePrefix.append(YerothERPConfig::temporaryFilesDir + "/" + tmpFilePrefix + "_ZERO");
	}
	else if (YerothTableauxDeBordWindow::GRAPHE_BAR_CHART == comboBox_type_graphes->currentText())
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

	QString statsDate(YerothUtils::LATEX_IN_OUT_handleForeignAccents(infoEntreprise.getVille_LATEX()));

	YerothUtils::getCurrentLocaleDate(statsDate);

	//qDebug() << "++ type fact. rapports: " << YerothConfig::typeOfFacturation;

	latexChartTemplate.replace("YEROTHPAPERSPEC", "a4paper");

	latexChartTemplate.replace("YEROTHCSVFILE", 					tmpFilePrefix + ".csv");
	latexChartTemplate.replace("YEROTHCHARTFIN", 				_reportTexFileEndString);

	latexChartTemplate.replace("YEROTHUTILISATEUR", QString("%1 %2")
			.arg(YerothUtils::getAllWindows()->getUser()->titreTex(),
					YerothUtils::getAllWindows()->getUser()->nom_completTex()));

	latexChartTemplate.replace("YEROTHENTREPRISE", 				infoEntreprise.getNomCommercial_LATEX());
	latexChartTemplate.replace("YEROTHBARCHARTTITLE", 			YerothUtils::LATEX_IN_OUT_handleForeignAccents(pdfFileTitle));
	latexChartTemplate.replace("YEROTHENTREPRISE", 				infoEntreprise.getNomCommercial_LATEX());
	latexChartTemplate.replace("YEROTHACTIVITESENTREPRISE", 		infoEntreprise.getSecteursActivitesTex());
	latexChartTemplate.replace("YEROTHBOITEPOSTALE", 			infoEntreprise.getBoitePostal());
	latexChartTemplate.replace("YEROTHVILLE", 					infoEntreprise.getVille_LATEX());
	latexChartTemplate.replace("YEROTHPAYS", 					infoEntreprise.getPaysTex());
	latexChartTemplate.replace("YEROTHEMAIL", 					infoEntreprise.getEmail_LATEX());
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

	_csvFileItemSize = 0;
}


void YerothTableauxDeBordWindow::handle_enabled_chiffre_daffaire_jour_semaine(bool enabled)
{
	label_jour_semaine_debut_chiffre_affaire->setVisible(enabled);
	label_jour_semaine_fin_chiffre_affaire->setVisible(enabled);

	dateEdit_chiffre_daffaire_jour_semaine_debut->setVisible(enabled);
	dateEdit_chiffre_daffaire_jour_semaine_fin->setVisible(enabled);
}


void YerothTableauxDeBordWindow::handle_enabled_chiffre_daffaire_mois(bool enabled)
{
	label_annee_chiffre_affaire->setVisible(enabled);
	label_mois_debut_chiffre_affaire->setVisible(enabled);
	label_mois_fin_chiffre_affaire->setVisible(enabled);

	comboBox_annee_chiffre_affaire->setVisible(enabled);
	comboBox_mois_debut_chiffre_affaire->setVisible(enabled);
	comboBox_mois_fin_chiffre_affaire->setVisible(enabled);
}


void YerothTableauxDeBordWindow::calculer_chiffre_daffaire_jour_semaine()
{
//	QDEBUG_STRING_OUTPUT_1("calculer_chiffre_daffaire_jour_semaine");

	_logger->log("calculer_chiffre_daffaire_jour_semaine");

	if (dateEdit_chiffre_daffaire_jour_semaine_fin->date() <
		dateEdit_chiffre_daffaire_jour_semaine_debut->date())
	{
        YerothQMessageBox::warning(this, QObject::trUtf8("évolution du chiffre d'affaire"),
                                   QObject::trUtf8("Le jour de 'début' doit être "
                                		   	   	   "antérieur au jour de la 'fin' !"));
        return ;
	}

	QMap<int, double> dayOfWeek__TO__businessturnover;

	qint64 dates_range = dateEdit_chiffre_daffaire_jour_semaine_debut->date()
			.daysTo(dateEdit_chiffre_daffaire_jour_semaine_fin->date()) + 1;


	QString string_chiffre_daffaire_semaine_query;

	int current_day_of_week;

	QSqlQuery qsql_query;

	QDateTime current_day_date = dateEdit_chiffre_daffaire_jour_semaine_debut->dateTime();

	double a_temp_day_of_week_business_turnover = 0.0;

	double current_day_business_turnover = 0.0;

	int current_query_size;

	qint64 i = 0;

	bool filtreActif = false;

	QString AUCUN_FILTRE(QObject::tr("aucun filtre !"));

	QString yerothFiltre;

	QString databaseTableColumn;

	QString comboBoxEvolutionObjetsCurrentText(comboBox_evolution_objets->currentText());

	QString textFromLineEditEvolutionSujets(lineEdit_evolution_objets_value->text());

	do
	{
		current_day_of_week = current_day_date.date().dayOfWeek();

		string_chiffre_daffaire_semaine_query =
				QString("SELECT (%1 - %2) FROM %3 WHERE %4='%5'")
				.arg(YerothDatabaseTableColumn::MONTANT_TOTAL_VENTE,
						YerothDatabaseTableColumn::MONTANT_TVA,
						YerothDatabase::STOCKS_VENDU,
						YerothDatabaseTableColumn::DATE_VENTE,
						DATE_TO_DB_FORMAT_STRING(current_day_date));


		bool filtreActif = false;

		AUCUN_FILTRE = QObject::tr("aucun filtre !");

		yerothFiltre.clear();

		databaseTableColumn.clear();

		comboBoxEvolutionObjetsCurrentText = comboBox_evolution_objets->currentText();

		textFromLineEditEvolutionSujets = lineEdit_evolution_objets_value->text();


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

//		qDebug() << QString("databaseTableColumn: %1, currentTextFromComboBoxEvolutionSujets: %2")
//						.arg(databaseTableColumn, comboBox_evolution_objets->currentText());

		if (true == filtreActif)
		{
			if (textFromLineEditEvolutionSujets.isEmpty())
			{
				YerothQMessageBox::warning(this, QObject::trUtf8("paramètre manquant"),
						QObject::trUtf8("Vous devez spécifier un paramètre dans"
								" le champs de texte 'nom' !"));
				return ;
			}
			else
			{
				if (!databaseTableColumn.isEmpty())
				{
					string_chiffre_daffaire_semaine_query.append(QString(" AND %1 = '%2'")
																	.arg(databaseTableColumn,
																		 textFromLineEditEvolutionSujets));
				}
			}
		}

//		qDebug() << "++ string_chiffre_daffaire_semaine_query: " << string_chiffre_daffaire_semaine_query;

		qsql_query.clear();

		current_query_size = YerothUtils::execQuery(qsql_query, string_chiffre_daffaire_semaine_query);

		if (current_query_size > 0)
		{
			while (qsql_query.next())
			{
				current_day_business_turnover = qsql_query.value(0).toDouble();

				QString STRING_current_day_of_week =
						YerothUtils::GET_DAYOFWEEK_FROM_QT_INT_CONSTANT(current_day_of_week);

				if (! YerothUtils::isEqualCaseInsensitive(YerothUtils::EMPTY_STRING, STRING_current_day_of_week))
				{
					a_temp_day_of_week_business_turnover =
							dayOfWeek__TO__businessturnover[current_day_of_week];

					dayOfWeek__TO__businessturnover.insert(current_day_of_week,
							current_day_business_turnover +
							a_temp_day_of_week_business_turnover);
				}
			}
		}

		current_day_date = current_day_date.addDays(1);

		++i;
	}
	while(i < dates_range);

//	qDebug() << dayOfWeek__TO__businessturnover;

	if (dayOfWeek__TO__businessturnover.isEmpty())
	{
        YerothQMessageBox::information(this,
                                      QObject::trUtf8("pas de données"),
                                      QObject::trUtf8("Il n'y a pas de données correspondante à la requête !\n"
                                              	  	  "Vérifier que les dates de début et de fin, "
                                              	  	  "sont correctes !"));
        return ;
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
	    _reportTexFileEndString.append("\\begin{enumerate}[1.]\n");

	    //Fill in the PDF file which amount of money for each day of week.

	    double somme_totale_ventes_jour_semaine = 0.0;

	    QMapIterator<int, double> it(dayOfWeek__TO__businessturnover);

	    while(it.hasNext())
	    {
	    	it.next();

	    	somme_totale_ventes_jour_semaine += it.value();

	    	_reportTexFileEndString.append("\\item ")

			#ifdef YEROTH_FRANCAIS_LANGUAGE
	    	    .append(YerothUtils::LATEX_IN_OUT_handleForeignAccents(
	    	    		YerothUtils::GET_DAYOFWEEK_FROM_QT_INT_CONSTANT(it.key())))
			#endif

			#ifdef YEROTH_ENGLISH_LANGUAGE
				.append(YerothUtils::LATEX_IN_OUT_handleForeignAccents(
						YerothUtils::GET_DAYOFWEEK_FROM_QT_INT_CONSTANT(it.key())))
			#endif
				.append(QString(": $%1$\n")
							.arg(YerothUtils::LATEX_IN_OUT_handleForeignAccents(GET_CURRENCY_STRING_NUM(it.value()))));
	    }

//	    QDEBUG_STRING_OUTPUT_2_N("somme_totale_jour_semaine", somme_totale_jour_semaine);

	    _reportTexFileEndString.append("\\end{enumerate}\n");

	    QString barItems;

	    double ratio = 0;

	    const int TICKS = 100;
	    const double MAX_RATIO = 900.0;

	    it.toFront();

	    while(it.hasNext())
	    {
	    	it.next();

	    	ratio = (it.value() * MAX_RATIO) / somme_totale_ventes_jour_semaine;

//	    	qDebug() << QString("++ jour semaine: %1, max amount: %2, ratio: %3")
//	    					.arg(YerothUtils::GET_DAYOFWEEK_FROM_QT_INT_CONSTANT(it.key()),
//	    						 QString::number(somme_totale_jour_semaine, 'f', 2),
//	    						 QString::number(ratio, 'f', 2));

	    	barItems.append(QString("\\baritem{%1}{%2}{gray}\n")
	    			.arg(YerothUtils::LATEX_IN_OUT_handleForeignAccents(
	    				 YerothUtils::GET_DAYOFWEEK_FROM_QT_INT_CONSTANT(it.key())),
	    					QString::number(ratio, 'f', 2)));
	    }


//	    qDebug() << "++ barItems: " << barItems;

	    QProcess aProcess;

	    aProcess.setWorkingDirectory(YerothERPConfig::temporaryFilesDir);

	    QString texDocument;
	    texDocument.append(YerothUtils::_1a_tex);


	    texDocument.replace("YEROTHBARPERGROUP", "");
	    texDocument.replace("YEROTHBARITEMS", 				barItems);
	    texDocument.replace("YEROTHTICKS", 					QString::number(TICKS));

	#ifdef YEROTH_FRANCAIS_LANGUAGE
	    texDocument.replace("YEROTHLEGENDANALYSECOMPAREE", 	"");
	    texDocument.replace("YEROTHDIAGRAMMETITRE", 		"Ratio du chiffre d'affaire jour-semaine.");
	    texDocument.replace("YEROTHNIVEAUCHIFFREAFFAIRE", 	"Niveau du chiffre d'affaire");
	#endif

	#ifdef YEROTH_ENGLISH_LANGUAGE
	    texDocument.replace("YEROTHLEGENDANALYSECOMPAREE", 	"");
	    texDocument.replace("YEROTHDIAGRAMMETITRE", 		"Ratio of days of week income.");
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


	    QString factureDate(YerothUtils::LATEX_IN_OUT_handleForeignAccents(infoEntreprise.getVille_LATEX()));
	    YerothUtils::getCurrentLocaleDate(factureDate);

	    QString longDateDebut;
	    QString longDateFin;

	#ifdef YEROTH_FRANCAIS_LANGUAGE
	    longDateDebut = QString("'%1'")
	                    .arg(YerothUtils::LATEX_IN_OUT_handleForeignAccents(
	                    		YerothUtils::frenchLocale.toString(dateEdit_chiffre_daffaire_jour_semaine_debut->date())));

	    longDateFin = QString("'%1'")
	                  .arg(YerothUtils::LATEX_IN_OUT_handleForeignAccents(
	                		  YerothUtils::frenchLocale.toString(dateEdit_chiffre_daffaire_jour_semaine_fin->date())));
	#endif

	#ifdef YEROTH_ENGLISH_LANGUAGE
	    longDateDebut = QString("'%1'")
	                    .arg(YerothUtils::LATEX_IN_OUT_handleForeignAccents(
	                    		YerothUtils::englishLocale.toString(dateEdit_chiffre_daffaire_jour_semaine_debut->date())));

	    longDateFin = QString("'%1'")
	                  .arg(YerothUtils::LATEX_IN_OUT_handleForeignAccents(
	                		  YerothUtils::englishLocale.toString(dateEdit_chiffre_daffaire_jour_semaine_fin->date())));
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

	    texDocument2.replace("YEROTHCHIFFREAFFAIREDATEDEBUT",	longDateDebut);
	    texDocument2.replace("YEROTHCHIFFREAFFAIREDATEFIN", 	longDateFin);
	    texDocument2.replace("YEROTHCHARTFIN", 					_reportTexFileEndString);
	    texDocument2.replace("YEROTHENTREPRISE", 				infoEntreprise.getNomCommercial_LATEX());
	    texDocument2.replace("YEROTHACTIVITESENTREPRISE", 		infoEntreprise.getSecteursActivitesTex());
	    texDocument2.replace("YEROTHBOITEPOSTALE", 				infoEntreprise.getBoitePostal());
	    texDocument2.replace("YEROTHVILLE", 					infoEntreprise.getVille_LATEX());
	    texDocument2.replace("YEROTHPAYS", 						infoEntreprise.getPaysTex());
	    texDocument2.replace("YEROTHEMAIL", 					infoEntreprise.getEmail_LATEX());
	    texDocument2.replace("YEROTHTELEPHONE", 				infoEntreprise.getTelephone());
	    texDocument2.replace("YEROTHDATE", 						factureDate);

	    texDocument2.replace("YEROTHNOMUTILISATEUR", QString("%1 %2")
	       												.arg(YerothUtils::getAllWindows()->getUser()->titreTex(),
	       													 YerothUtils::getAllWindows()->getUser()->nom_completTex()));

	    texDocument2.replace("YEROTHHEUREDIMPRESSION",		CURRENT_TIME);
	    texDocument2.replace("YEROTHCOMPTEBANCAIRENR", 		infoEntreprise.getNumeroCompteBancaire());
	    texDocument2.replace("YEROTHCONTRIBUABLENR", 		infoEntreprise.getNumeroDeContribuable());
	    texDocument2.replace("YEROTHAGENCECOMPTEBANCAIRE",	infoEntreprise.getAgenceCompteBancaireTex());

	    texDocument2.replace("1a.tex", fileName1);

	#ifdef YEROTH_FRANCAIS_LANGUAGE
	    texDocument2.replace("YEROTHTITREDOCUMENT",
	                         QString("Diagramme r\\'epr\\'esentatif des chiffres"
	                                 " d'affaire par jour de la semaine du %1 au %2.").arg(longDateDebut, longDateFin));
	#endif

	#ifdef YEROTH_ENGLISH_LANGUAGE
	    texDocument2.replace("YEROTHTITREDOCUMENT",
	                         QString("Chart illustrating days of week income from %1 to %2.")
	                         .arg(longDateDebut, longDateFin));
	#endif

	    //qDebug() << "++ test: " << texDocument2;

	    YerothUtils::LATEX_IN_OUT_handleForeignAccents(texDocument2);

	    QString fileName(FILE_NAME_USERID_CURRENT_TIME("evolution-chiffre-affaire-jour-semaine"));
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

//	    qDebug() << "++ file name to latex compile bar diag: " << texRm;

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

//	    qDebug() << "++ test it: " << tmpFilePrefix + "pdf";

	    aProcess.startDetached(YerothERPConfig::pathToPdfReader, progArguments);

	    //qDebug() << "\texit status evince: " << _aProcess->exitStatus();
}


void YerothTableauxDeBordWindow::calculer_chiffre_daffaire_mois()
{
    _logger->log("calculer_chiffre_daffaire_mois");

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

    QString strQuery(QString("SELECT %1, (%2 - %3) FROM %4 WHERE %5 >= '%6' AND ")
    					.arg(YerothDatabaseTableColumn::DATE_VENTE,
    						 YerothDatabaseTableColumn::MONTANT_TOTAL_VENTE,
							 YerothDatabaseTableColumn::MONTANT_TVA,
							 YerothDatabase::STOCKS_VENDU,
							 YerothDatabaseTableColumn::DATE_VENTE,
							 dateDebut));

    if (moisFinAjuster)
    {
        strQuery.append(QString("%1 < '%2'")
        				 .arg(YerothDatabaseTableColumn::DATE_VENTE,
        					  dateFin));
    }
    else
    {
        strQuery.append(QString("%1 <= '%2'")
        				 .arg(YerothDatabaseTableColumn::DATE_VENTE,
        					  dateFin));
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
	        YerothQMessageBox::warning(this, QObject::trUtf8("paramètre manquant"),
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
		    								YerothUtils::LATEX_IN_OUT_handleForeignAccents(textFromLineEditEvolutionSujets)));
			}
		}
	}


//    qDebug() << "++ query: " << strQuery;

	QSqlQuery query;

    double montant_total_vente = 0.0;

    int querySize = YerothUtils::execQuery(query, strQuery, _logger);

    if (0 >= querySize)
    {

        YerothQMessageBox::information(this,
                                      QObject::trUtf8("pas de données"),
                                      QObject::trUtf8("Il n'y a pas de données correspondante à la requête !\n"
                                              	  	  "Vérifier que les dates de début et de fin, "
                                              	  	  "ainsi que l'année sont correctes !"));
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
    _reportTexFileEndString.append("\\begin{enumerate}[1.]\n");

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


    QString factureDate(YerothUtils::LATEX_IN_OUT_handleForeignAccents(infoEntreprise.getVille_LATEX()));
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
    texDocument2.replace("YEROTHENTREPRISE", 			infoEntreprise.getNomCommercial_LATEX());
    texDocument2.replace("YEROTHACTIVITESENTREPRISE", 	infoEntreprise.getSecteursActivitesTex());
    texDocument2.replace("YEROTHBOITEPOSTALE", 			infoEntreprise.getBoitePostal());
    texDocument2.replace("YEROTHVILLE", 					infoEntreprise.getVille_LATEX());
    texDocument2.replace("YEROTHPAYS", 					infoEntreprise.getPaysTex());
    texDocument2.replace("YEROTHEMAIL", 					infoEntreprise.getEmail_LATEX());
    texDocument2.replace("YEROTHTELEPHONE", 				infoEntreprise.getTelephone());
    texDocument2.replace("YEROTHDATE", 					factureDate);

    texDocument2.replace("YEROTHNOMUTILISATEUR", QString("%1 %2")
       												.arg(YerothUtils::getAllWindows()->getUser()->titreTex(),
       													 YerothUtils::getAllWindows()->getUser()->nom_completTex()));

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
		if (radioButton_jour_semaine->isChecked())
		{
			analyse_comparee_jour_semaine();
		}
		else if (radioButton_mensuel->isChecked())
		{
			analyse_comparee_mensuelle();
		}
	}
	else
	{
		if (radioButton_jour_semaine->isChecked())
		{
			calculer_chiffre_daffaire_jour_semaine();
		}
		else if (radioButton_mensuel->isChecked())
		{
			calculer_chiffre_daffaire_mois();
		}
	}
}


void YerothTableauxDeBordWindow::setupShortcuts()
{
    setupShortcutActionMessageDaide 	(*actionAppeler_aide);
    setupShortcutActionQuiSuisJe		(*actionQui_suis_je);
}

