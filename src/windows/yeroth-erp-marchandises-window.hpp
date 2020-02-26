/*
 * yeroth-erp-marchandises-window.hpp
 *
 *      Author: Xavier NOUMBISSI NOUNDOU, Dipl.-Inf., Ph.D. (ABD)
 */

#ifndef YEROTH_ERP_INVENTAIRE_DES_STOCKS_WINDOW_HPP_
#define YEROTH_ERP_INVENTAIRE_DES_STOCKS_WINDOW_HPP_

#include "../../ui_yeroth-erp-marchandises-window.h"

#include "src/windows/yeroth-erp-search-form.hpp"

#include <QtCore/QDebug>

#include <QtWidgets/QMessageBox>

#include <QtGui/QContextMenuEvent>

#include "yeroth-erp-window-commons.hpp"


class QStandardItemModel;
class QContextMenuEvent;
class QProcess;

class YerothSqlTableModel;


class YerothMarchandisesWindow : public YerothWindowsCommons, private Ui_YerothStocksDeSecuriteWindow
{
    Q_OBJECT

public:

	YEROTH_CLASS_OPERATORS

    YerothMarchandisesWindow();

    virtual ~YerothMarchandisesWindow();

    inline bool isCurrentlyFiltered()
    {
    	return _currentlyFiltered;
    }

    void setCurrentlyFiltered(bool currentlyFiltered);

    inline virtual QToolBar * getQMainWindowToolBar()
    {
    	return toolBar_inventaireDesStocksWindow;
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

	static void getInventoryStocksListingTexTableString(QString &texTable_in_out,
											   	   	    QStandardItemModel &tableStandardItemModel,
														QList<int> &dbFieldNameOfTypeString,
														QList<int> &columnsToIgnore,
														int fromRowIndex,
														int toRowIndex,
														bool lastPage);

	static void getMarchandisesTexDocumentString(QString &texDocumentString_in_out,
			   	   	   	   	   	   	   	   	   	 QString &printString);

	virtual bool imprimer_document();

    virtual void afficherMarchandises(YerothSqlTableModel &aYerothSqlTableModel);

	inline void afficherMarchandises()
	{
		afficherMarchandises(*_curInventaireDesStocksTableModel);
	}

    void afficher_stock_selectioner(const QString &stockName);

    void supprimer_ce_stock();

    inline void rechercher()
    {
    	_searchMarchandisesWidget->rendreVisible();
    }

    void reinitialiser_elements_filtrage();

    void reinitialiser_recherche();

    void set_rechercher_font();

    inline int getLastListerSelectedRow()
    {
    	return tableView_marchandises->lastSelectedRow();
    }

    inline void setLastListerSelectedRow(int row)
    {
    	tableView_marchandises->setLastSelectedRow(row);
    }

    void setSearchFormSqlTableModel(YerothSqlTableModel *searchFormSqlTableModel);

    void updateLineEditDesignation();

    static double getValeurDinventaireEnStock(QString categorie, QString designation);

    static double getQuantiteTotalEnStock(QString categorie, QString designation);

	double getQuantiteTotalEnStock(const QModelIndex &aQModelIndex);

protected slots:

	virtual void slot_reinitialiser_champs_db_visibles();

protected:

	virtual void reinitialiser_champs_db_visibles();

    void contextMenuEvent(QContextMenuEvent *event);

    inline virtual void hideEvent(QHideEvent * hideEvent)
    {
    	_searchMarchandisesWidget->rendreInvisible();
    }

    virtual void setupShortcuts();

private slots:

	bool filtrer_empty_product_stock();

	bool filtrer();

private:

	void set_filtrer_font();

    void populateInventaireDesStocksComboBoxes();

    void setupLineEditsQCompleters();

    void setupLineEdits();

    static const QString 	_WINDOW_TITLE;

    YerothLogger				*_logger;

    bool					_currentlyFiltered;

    int 					_lastSelectedRow;

    QFont 					*_pushButton_filtrer_font;

    QFont 					*_action_RechercherFont;

    QFont 					*_pushButton_RechercherFont;

    YerothSearchForm 		*_searchMarchandisesWidget;

    YerothSqlTableModel 		*_searchMarchandisesTableModel;

    YerothSqlTableModel 		*_curInventaireDesStocksTableModel;
};

#endif /* YEROTH_ERP_INVENTAIRE_DES_STOCKS_WINDOW_HPP_ */
