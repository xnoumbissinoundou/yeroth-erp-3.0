/*
 * yeroth-erp-stocks-window.hpp
 *
 *      Author: Xavier NOUMBISSI NOUNDOU, Dipl.-Inf., Ph.D. (ABD)
 */

#ifndef YEROTH_ERP_STOCKS_WINDOW_HPP_
#define YEROTH_ERP_STOCKS_WINDOW_HPP_

#include "../../ui_yeroth-erp-stocks-window.h"

#include "src/windows/yeroth-erp-search-form.hpp"

#include <QtCore/QDebug>

#include <QtCore/QVector>

#include <QtWidgets/QMessageBox>

#include <QtGui/QContextMenuEvent>

#include "yeroth-erp-window-commons.hpp"


class YerothPOSDialogCommons;
class QStandardItemModel;
class QContextMenuEvent;
class QProcess;

class YerothSqlTableModel;

class YerothStocksWindow : public YerothWindowsCommons, private Ui_YerothStocksWindow
{
    Q_OBJECT

public:

	YEROTH_CLASS_OPERATORS

    YerothStocksWindow();

    virtual ~YerothStocksWindow();

    inline bool isCurrentlyFiltered()
    {
    	return _currentlyFiltered;
    }

    void setCurrentlyFiltered(bool currentlyFiltered);

    inline virtual QToolBar * getQMainWindowToolBar()
    {
    	return toolBar_stocksWindow;
    }

    virtual void definirCaissier();

    virtual void definirManager();

    virtual void definirVendeur();

    virtual void definirGestionaireDesStocks();

    virtual void definirMagasinier();

    virtual void definirPasDeRole();

    virtual void rendreVisible(YerothSqlTableModel *stocksTableModel);

    virtual void rendreInvisible();

public slots:

	inline void gererChoixStrategieGestionDesStocks()
	{
		afficherStocks();
	}

	inline virtual void apropos()
	{
		YerothWindowsCommons::apropos(this);
	}

	inline virtual void help()
	{
		YerothQMessageBox::information(this,
							 QObject::trUtf8("aide"),
							 QObject::trUtf8("Sélectionner un stock dans le tableau des stocks, ensuite cliquer "
											 "sur l'opération que vous souhaitez réaliser !"));
	}

	bool export_csv_file();

	static void getStocksListingTexDocumentString(QString &texDocumentString_in_out,
												  QString &printString);

	static void getStocksListingTexTableString(QString &texTable_in_out,
											   QStandardItemModel &tableStandardItemModel,
											   QList<int> &dbFieldNameOfTypeString,
											   QList<int> &columnsToIgnore,
											   int fromRowIndex,
											   int toRowIndex,
											   bool lastPage);

	virtual bool imprimer_document();

    virtual void entrer();

    void afficherStocks(YerothSqlTableModel &sqlTableModel, QString localVisibleStrategy  = YerothUtils::EMPTY_STRING);

    virtual void afficherStocks();

    virtual void modifier_les_articles();

    void afficher_au_detail();

    void afficher_au_detail(const QModelIndex &modelIndex);

    void afficher_stock_selectioner_bar_code(const QString &stockBarCode);

    void afficher_stock_selectioner(const QString &stockName);

    void connecter_localisation();

    void deconnecter_localisation();

    void supprimer_ce_stock();

    inline void rechercher()
    {
    	_searchStocksWidget->rendreVisible();
    }

    void reinitialiser_elements_filtrage();

    void reinitialiser_recherche();

    void set_filtrer_font();

    void set_rechercher_font();

    inline int getLastListerSelectedRow()
    {
    	return tableView_stocks->lastSelectedRow();
    }

    inline void setLastListerSelectedRow(int row)
    {
    	tableView_stocks->setLastSelectedRow(row);
    }

    inline void setSearchFormSqlTableModel(YerothSqlTableModel *searchFormSqlTableModel)
    {
    	_searchStocksTableModel = searchFormSqlTableModel;
    }

    void updateLineEditRechercherCodeBar();

    bool SQL_TABLE_STOCKS_VENDU_EMPTY();

private slots:

	void private_slot_afficher_historique_du_stock();

	bool filtrer_stocks_perimes_seulement();

	bool filtrer_stocks();

	inline void setRechercheCodebarArticleFocus()
	{
		lineEdit_recherche_reference->setFocus();
	}

protected slots:

	virtual void slot_reinitialiser_champs_db_visibles();

protected:

	virtual void reinitialiser_champs_db_visibles();

    void contextMenuEvent(QContextMenuEvent *event);

    virtual void hideEvent(QHideEvent * hideEvent);

    virtual void setupShortcuts();

private:


    typedef enum enum_comboBoxStrategyIndex
	{
    	STRATEGIE_VENTE_SORTIE_ALL 		= 0,
    	STRATEGIE_VENTE_SORTIE_DPF_DPO 	= 1,
    	STRATEGIE_VENTE_SORTIE_FIFO 	= 2,
    	STRATEGIE_VENTE_SORTIE_LIFO 	= 3
	}
    comboBoxStrategyIndex;

    void activerComboBoxStrategieDeGestionDesStocks();

    void desactiverComboBoxStrategieDeGestionDesStocks();

    void populateComboBoxes();

    void setComboBoxStrategieDeStocks();

    void setupLineEdits();

    static const QString 	_WINDOW_TITLE;



    YerothLogger			*_logger;

    QStringList				_historiqueStock;

    bool					_currentlyFiltered;

    QAction					*_actionRechercheArticleCodebar;

    QProcess				*_aProcess;

    QFont 					*_pushButton_stocks_filtrer_font;

    QFont 					*_pushButton_RechercherFont;

    QFont 					*_action_RechercherFont;

    YerothSearchForm 		*_searchStocksWidget;

    YerothSqlTableModel 	*_searchStocksTableModel;
};

#endif /* YEROTH_ERP_STOCKS_WINDOW_HPP_ */
