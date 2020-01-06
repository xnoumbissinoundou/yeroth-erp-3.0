/*
 * yeroth-erp-tableaux-de-bord-window.hpp
 *
 *      Author: Xavier NOUMBISSI NOUNDOU, Dipl.-Inf., Ph.D. (ABD)
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

class YerothTableauxDeBordWindow : public YerothWindowsCommons, private Ui_YerothRapportsWindow
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

    void setupTabOne();

    void setupTabTwo();

    void setupTabThree();

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
        _curStocksVenduTableModel->yerothSetFilter(_searchFilter);
    }

    inline void generer()
    {
        rechercher();
    }

    void meilleursStats(QString fileName, QString fieldId, int size);

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

    void changeLineEditEvolutionObjetsTextSetup(const QString &comboBoxEvolutionObjetsCurrentText);

    void bilanComptable();

    void analyseComparee();

    void calculerChiffresDaffaireMois();

    void actionTabOne();

protected:

    virtual void setupShortcuts();


private slots:

	void handleTabChanged(int index);

private:

    enum TabIndexes
    {
    	SUJET_ACTION_BUSINESS_TURNOVER_PROGRESS 	= 0,
		SUJET_ACTION_BUSINESS_TURNOVER_COMPARISON	= 1
    };

    void setupDateTimeEditsTabTwo();

    void setupDateTimeEditsTabThree();

    class YerothStatsItem
	{
	public:
    	QString _itemName;
    	double  _itemValue;

    	inline YerothStatsItem(QString itemName, double itemValue)
    	:_itemName(itemName),
		 _itemValue(itemValue)
    	{
    	}

    	 inline static bool lessThan(const YerothStatsItem *anItem1, const YerothStatsItem *anItem2)
    	 {
    	     return anItem1->_itemValue < anItem2->_itemValue;
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

	static const QString 	_WINDOW_TITLE;

	YerothLogger				*_logger;

	QString					_searchFilter;
	QString 				_reportTexFileEndString;

	QMap<QString, int> 		_moisToNombre;

	unsigned int 			_csvFileItemSize;
	unsigned int 			_startYear;

	YerothSqlTableModel 		*_curStocksVenduTableModel;
};


#endif /* YEROTH_BUSINESS_REPORTS_WINDOW_HPP_ */
