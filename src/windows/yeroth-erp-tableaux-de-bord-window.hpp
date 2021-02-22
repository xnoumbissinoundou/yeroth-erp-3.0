/*
 * yeroth-erp-tableaux-de-bord-window.hpp
 *
 *      Author: DR. XAVIER NOUMBISSI NOUNDOU
 */

#ifndef YEROTH_BUSINESS_REPORTS_WINDOW_HPP_
#define YEROTH_BUSINESS_REPORTS_WINDOW_HPP_

#include "../../ui_yeroth-erp-tableaux-de-bord-window.h"


#include "yeroth-erp-window-commons.hpp"


#include "src/utils/yeroth-erp-database-table-column.hpp"


class Yeroth_MAP_COMPLEX_Item;

class QProcess;

class YerothUtils;
class YerothLogger;
class YerothSqlTableModel;
class YerothERPWindows;

class YerothTableauxDeBordWindow : public YerothWindowsCommons,
								   private Ui_YerothRapportsWindow
{
    Q_OBJECT

public:

	YEROTH_CLASS_OPERATORS

	YerothTableauxDeBordWindow();

    virtual ~YerothTableauxDeBordWindow();

    inline virtual QToolBar * getQMainWindowToolBar()
    {
    	return toolBar_rapportsWindow;
    }

    virtual void definirCaissier();

    virtual void definirManager();

    virtual void definirVendeur();

    virtual void definirGestionaireDesStocks();

    virtual void definirMagasinier();

    virtual void definirPasDeRole();

    void setupTab_EVOLUTION_DU_CHIFFRE_DAFFAIRE();

    void setupTab_COMPARAISON_DES_CHIFFRES_DAFFAIRES();

    inline void setupTab_BILAN_COMPTABLE()
    {
    	comboBox_bilan_comptable_operation->addItem(YerothTableauxDeBordWindow::OPERATION_GENERER_BILAN_COMPTABLE);
    }

    virtual void rendreVisible(YerothSqlTableModel *stocksTableModel);

public slots:

	inline virtual void apropos()
	{
		YerothWindowsCommons::apropos(this);
	}

	inline virtual void help()
	{
		YerothQMessageBox::information(this,
						 QObject::trUtf8("aide"),
						 QObject::trUtf8("Sélectionner les critères commerciaux que vous désirez,"
										 " ensuite cliquez sur le boutton 'générer le fichier PDF'"
										 " pour obtenir le fichier PDF correspondant !"));
	}

    void reinitialiser();

    void reinitialiser_chiffre_affaire();

    void reinitialiser_bilan_comptable();

    inline void setFilter()
    {
        _curStocksVenduTableModel->yerothSetFilter_WITH_where_clause(_searchFilter);
    }

    void generer();

    void quantite_max_stats(QString fileName, QString fieldId, int size, bool service = false);

    void quantite_moindre_stats(QString fileName, QString fieldId, int size, bool service = false);

    void meilleursStats(QString fileName, QString fieldId, int size, bool service = false);

    void ZERO_stats_stocks(QString fileName, QString fieldId);

    void ZERO_stats(QString fileName, QString fieldId);

    void derniersStats(QString fileName, QString fieldId, int size, bool service = false);


    //** statsQuantiteMax*
    inline void statsQuantiteMaxServices(QString fileName, int size)
    {
    	quantite_max_stats(fileName, YerothDatabaseTableColumn::DESIGNATION, size, true);
    }

    inline void statsQuantiteMaxClients(QString fileName, int size, bool service = false)
    {
    	quantite_max_stats(fileName, YerothDatabaseTableColumn::NOM_ENTREPRISE_CLIENT, size);
    }

    inline void statsQuantiteMaxFournisseursVentes(QString fileName, int size, bool service = false)
    {
    	quantite_max_stats(fileName, YerothDatabaseTableColumn::NOM_ENTREPRISE_FOURNISSEUR, size);
    }

    inline void statsQuantiteMaxCaissiers(QString fileName, int size, bool service = false)
    {
    	quantite_max_stats(fileName, YerothDatabaseTableColumn::NOM_CAISSIER, size);
    }

    inline void statsQuantiteMaxCategories(QString fileName, int size, bool service = false)
    {
    	quantite_max_stats(fileName, YerothDatabaseTableColumn::CATEGORIE, size);
    }

    inline void statsQuantiteMaxArticles(QString fileName, int size, bool service = false)
    {
    	quantite_max_stats(fileName, YerothDatabaseTableColumn::DESIGNATION, size);
    }


    //** statsQuantiteMoindre*
    inline void statsQuantiteMoindreServices(QString fileName, int size)
    {
    	quantite_moindre_stats(fileName, YerothDatabaseTableColumn::DESIGNATION, size, true);
    }

    inline void statsQuantiteMoindreClients(QString fileName, int size, bool service = false)
    {
    	quantite_moindre_stats(fileName, YerothDatabaseTableColumn::NOM_ENTREPRISE_CLIENT, size);
    }

    inline void statsQuantiteMoindreFournisseursVentes(QString fileName, int size, bool service = false)
    {
    	quantite_moindre_stats(fileName, YerothDatabaseTableColumn::NOM_ENTREPRISE_FOURNISSEUR, size);
    }

    inline void statsQuantiteMoindreCaissiers(QString fileName, int size, bool service = false)
    {
    	quantite_moindre_stats(fileName, YerothDatabaseTableColumn::NOM_CAISSIER, size);
    }

    inline void statsQuantiteMoindreCategories(QString fileName, int size, bool service = false)
    {
    	quantite_moindre_stats(fileName, YerothDatabaseTableColumn::CATEGORIE, size);
    }

    inline void statsQuantiteMoindreArticles(QString fileName, int size, bool service = false)
    {
    	quantite_moindre_stats(fileName, YerothDatabaseTableColumn::DESIGNATION, size);
    }


    //** statsMeilleurs*
    inline void statsMeilleursClients(QString fileName, int size, bool service = false)
    {
    	meilleursStats(fileName, YerothDatabaseTableColumn::NOM_ENTREPRISE_CLIENT, size);
    }

    inline void statsMeilleursFournisseursVentes(QString fileName, int size)
    {
    	meilleursStats(fileName, YerothDatabaseTableColumn::NOM_ENTREPRISE_FOURNISSEUR, size);
    }

    inline void statsMeilleursCaissiers(QString fileName, int size)
    {
    	meilleursStats(fileName, YerothDatabaseTableColumn::NOM_CAISSIER, size);
    }

    inline void statsMeilleursServices(QString fileName, int size)
    {
    	meilleursStats(fileName, YerothDatabaseTableColumn::DESIGNATION, size, true);
    }

    inline void statsMeilleursArticles(QString fileName, int size)
    {
    	meilleursStats(fileName, YerothDatabaseTableColumn::DESIGNATION, size);
    }

    inline void statsMeilleursCategories(QString fileName, int size)
    {
    	meilleursStats(fileName, YerothDatabaseTableColumn::CATEGORIE, size);
    }


    //** ZERO_stat_*
    inline void statsZERO_Clients(QString fileName)
    {
//    	ZERO_stats(fileName, YerothDatabaseTableColumn::NOM_ENTREPRISE_CLIENT);
    }

    inline void statsZERO_FournisseursVentes(QString fileName)
    {
    	ZERO_stats(fileName, YerothDatabaseTableColumn::NOM_ENTREPRISE_FOURNISSEUR);
    }

    inline void statsZERO_Caissiers(QString fileName)
    {
//    	ZERO_stats(fileName, YerothDatabaseTableColumn::NOM_CAISSIER);
    }

    inline void statsZERO_Articles(QString fileName)
    {
    	ZERO_stats_stocks(fileName, YerothDatabaseTableColumn::DESIGNATION);
    }

    inline void statsZERO_Categories(QString fileName)
    {
    	ZERO_stats(fileName, YerothDatabaseTableColumn::CATEGORIE);
    }


    //** statsDerniers*
    inline void statsDerniersClients(QString fileName, int size)
    {
    	derniersStats(fileName, YerothDatabaseTableColumn::NOM_ENTREPRISE_CLIENT, size);
    }

    inline void statsDerniersFournisseursVentes(QString fileName, int size)
    {
    	derniersStats(fileName, YerothDatabaseTableColumn::NOM_ENTREPRISE_FOURNISSEUR, size);
    }

    inline void statsDerniersCaissiers(QString fileName, int size)
    {
    	derniersStats(fileName, YerothDatabaseTableColumn::NOM_CAISSIER, size);
    }

    inline void statsDerniersServices(QString fileName, int size)
    {
    	derniersStats(fileName, YerothDatabaseTableColumn::DESIGNATION, size, true);
    }

    inline void statsDerniersArticles(QString fileName, int size)
    {
    	derniersStats(fileName, YerothDatabaseTableColumn::DESIGNATION, size);
    }

    inline void statsDerniersCategories(QString fileName, int size)
    {
    	derniersStats(fileName, YerothDatabaseTableColumn::CATEGORIE, size);
    }

    void rechercher();

    virtual bool export_csv_file();

    void remove_BAR_PIE_CHART_OPTION_FOR_ZERO_BUSINESS_TURNOVER(const QString &comboBoxQualiteCurrentText);

    void changeLineEditEvolutionObjetsTextSetup(const QString &comboBoxEvolutionObjetsCurrentText);

    void bilanComptable();

    void analyseComparee();

    void compterLesArticlesVendusParQuantite();

    void calculerChiffresDaffaireMois();

    void choisirEvolutionDuChiffreDaffaire();

protected:

    virtual void setupShortcuts();


private slots:

	void handleTabChanged(int index);

private:

    class StockQteVendueEtRemiseTotalSurVente
    {
    public:
    	inline StockQteVendueEtRemiseTotalSurVente()
    	:_qteVendue(0),
		 _remiseTotalSurVentes(0)
    	{}

    	inline StockQteVendueEtRemiseTotalSurVente(double qteVendue, double remiseTotalSurVentes)
    	:_qteVendue(qteVendue),
		 _remiseTotalSurVentes(remiseTotalSurVentes)
    	{
    	}

    	double _qteVendue;
    	double _remiseTotalSurVentes;
    };

    enum TabIndexes
    {
    	SUJET_ACTION_FINANCIAL_ACCOUNTING_REPORT_GENERATION = 0,
		SUJET_ACTION_BUSINESS_TURNOVER_COMPARISON 			= 1,
		SUJET_ACTION_BUSINESS_TURNOVER_PROGRESS				= 2
    };

    void setupDateTimeEdits_COMPARAISON_DES_CHIFFRES_DAFFAIRES();

    void setupDateTimeEdits_BILAN_COMPTABLE();


	static const QString OPERATION_GENERER;
	static const QString OPERATION_GENERER_CHIFFRE_DAFFAIRE;
	static const QString OPERATION_GENERER_BILAN_COMPTABLE;


	static const unsigned int QUANTITE_9;

	static const QString MOIS_1;
	static const QString MOIS_2;
	static const QString MOIS_3;
	static const QString MOIS_4;
	static const QString MOIS_5;
	static const QString MOIS_6;
	static const QString MOIS_7;
	static const QString MOIS_8;
	static const QString MOIS_9;
	static const QString MOIS_10;
	static const QString MOIS_11;
	static const QString MOIS_12;

	static const QString QUALITE_PLUS_VENDU_PAR_QUANTITE;
	static const QString QUALITE_MOINS_VENDU_PAR_QUANTITE;
	static const QString QUALITE_MEILLEURS;
	static const QString QUALITE_ZERO;
	static const QString QUALITE_DERNIERS;

	static const QString OBJET_SERVICES;
	static const QString OBJET_ARTICLES;
	static const QString OBJET_CATEGORIES;
	static const QString OBJET_CAISSIERS;
	static const QString OBJET_CLIENTS;
	static const QString OBJET_FOURNISSEURS;

	static const QString GRAPHE_BAR_CHART;
	static const QString GRAPHE_PIE_CHART;

	static const double STATS_MIN_VALUE;

	static const unsigned int MAX_YEARS_REPORTS;


	YerothLogger			*_logger;

	QString					_searchFilter;

	QString					_curDetailPDFFileTitleString;

	QString 				_reportTexFileEndString;

	QMap<QString, int> 		_moisToNombre;

	int						_objetServiceLastIndex;

	int						_objetClientLastIndex;

	unsigned int 			_csvFileItemSize;
	unsigned int 			_startYear;

	YerothSqlTableModel 	*_curStocksVenduTableModel;
};


#endif /* YEROTH_BUSINESS_REPORTS_WINDOW_HPP_ */
