/*
 * yeroth-erp-tableaux-de-bord-window.hpp
 *
 *      Author: DIPL.-INF. XAVIER NOUMBISSI NOUNDOU
 */

#ifndef YEROTH_BUSINESS_REPORTS_WINDOW_HPP_
#define YEROTH_BUSINESS_REPORTS_WINDOW_HPP_

#include "../../ui_yeroth-erp-tableaux-de-bord-window.h"


#include "yeroth-erp-window-commons.hpp"


#include "src/utils/yeroth-erp-database-table-column.hpp"


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

    ~YerothTableauxDeBordWindow();

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

    void setupTab_BILAN_COMPTABLE();

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

    inline void generer()
    {
        rechercher();
    }

    void meilleursStats(QString fileName, QString fieldId, int size);

    void ZERO_stats_stocks(QString fileName, QString fieldId);

    void ZERO_stats(QString fileName, QString fieldId);

    void derniersStats(QString fileName, QString fieldId, int size);

    inline void statsMeilleursClients(QString fileName, int size)
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

    inline void statsMeilleursArticles(QString fileName, int size)
    {
    	meilleursStats(fileName, YerothDatabaseTableColumn::DESIGNATION, size);
    }

    inline void statsMeilleursCategories(QString fileName, int size)
    {
    	meilleursStats(fileName, YerothDatabaseTableColumn::CATEGORIE, size);
    }

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
    	SUJET_ACTION_BUSINESS_TURNOVER_PROGRESS 	= 0,
		SUJET_ACTION_BUSINESS_TURNOVER_COMPARISON	= 1
    };

    void setupDateTimeEdits_COMPARAISON_DES_CHIFFRES_DAFFAIRES();

    void setupDateTimeEdits_BILAN_COMPTABLE();

    class YerothStatsItem
	{
	public:
    	QString _itemName;
    	double  _itemValue;
    	double  _itemSecondValue;

    	inline YerothStatsItem(QString itemName, double itemValue)
    	:_itemName(itemName),
		 _itemValue(itemValue),
		 _itemSecondValue(0.0)
    	{
    	}

    	inline static bool lessThan(const YerothStatsItem *anItem1, const YerothStatsItem *anItem2)
    	{
    		return anItem1->_itemValue < anItem2->_itemValue;
    	}

    	inline static bool lessThan_second_value(const YerothStatsItem *anItem1, const YerothStatsItem *anItem2)
    	{
    		return anItem1->_itemSecondValue < anItem2->_itemSecondValue;
    	}
	};

	static const QString OPERATION_GENERER;
	static const QString OPERATION_GENERER_CHIFFRE_DAFFAIRE;
	static const QString OPERATION_GENERER_BILAN_COMPTABLE;

	static const QString QUANTITE_2;
	static const QString QUANTITE_3;
	static const QString QUANTITE_4;
	static const QString QUANTITE_5;
	static const QString QUANTITE_6;
	static const QString QUANTITE_7;
	static const QString QUANTITE_8;
	static const QString QUANTITE_9;

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

	static const QString QUALITE_MEILLEURS;
	static const QString QUALITE_ZERO;
	static const QString QUALITE_DERNIERS;

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
	QString 				_reportTexFileEndString;

	QMap<QString, int> 		_moisToNombre;

	int						_objetClientLastIndex;

	unsigned int 			_csvFileItemSize;
	unsigned int 			_startYear;

	YerothSqlTableModel 	*_curStocksVenduTableModel;
};


#endif /* YEROTH_BUSINESS_REPORTS_WINDOW_HPP_ */
