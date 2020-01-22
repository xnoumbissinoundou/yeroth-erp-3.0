/*
 * yeroth-erp-achats-window.hpp
 *
 *      Author: Xavier NOUMBISSI NOUNDOU, Ph.D. (ABD), Ph.D. (ABD)
 */

#ifndef YEROTH_ERP_ACHATS_WINDOW_HPP_
#define YEROTH_ERP_ACHATS_WINDOW_HPP_

#include "../../ui_yeroth-erp-achats-window.h"

#include "src/windows/yeroth-erp-search-form.hpp"

#include <QtCore/QDebug>

#include <QtWidgets/QMessageBox>

#include <QtGui/QContextMenuEvent>

#include "yeroth-erp-window-commons.hpp"

class QStandardItemModel;
class QContextMenuEvent;
class QProcess;

class YerothSqlTableModel;

class YerothAchatsWindow : public YerothWindowsCommons, private Ui_YerothAchatsWindow
{
    Q_OBJECT

public:

	YEROTH_CLASS_OPERATORS

    YerothAchatsWindow();

    virtual ~YerothAchatsWindow();

    inline bool isCurrentlyFiltered()
    {
    	return _currentlyFiltered;
    }

    void setCurrentlyFiltered(bool currentlyFiltered);

    inline virtual QToolBar * getQMainWindowToolBar()
    {
    	return toolBar_achatsWindow;
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
							 QObject::trUtf8("Sélectionner un achat dans le tableau des achats, ensuite cliquer "
											 "sur l'opération que vous souhaitez réaliser !"));
	}

	bool export_csv_file();

	static void getAchatsListingTexDocumentString(QString &texDocumentString_in_out,
												  QString &printString);

	static void getAchatsListingTexTableString(QString &texTable_in_out,
	        								   QStandardItemModel &tableStandardItemModel,
											   QList<int> &columnsToIgnore,
											   int fromRowIndex,
											   int toRowIndex,
											   bool lastPage);

	virtual bool imprimer_document();

    virtual void afficherAchats(YerothSqlTableModel &achatSqlTableModel);

    void afficher_stock_selectioner(const QString &stockName);

    void afficher_au_detail();

    void afficher_au_detail(const QModelIndex &modelIndex);

    inline void rechercher()
    {
    	_searchAchatsWidget->rendreVisible();
    }

    void reinitialiser_elements_filtrage();

    void reinitialiser_recherche();

    void set_filtrer_font();

    void set_rechercher_font();

    inline int getLastListerSelectedRow()
    {
    	return tableView_achats->lastSelectedRow();
    }

    inline void setLastListerSelectedRow(int row)
    {
    	tableView_achats->setLastSelectedRow(row);
    }

    inline void setSearchFormSqlTableModel(YerothSqlTableModel *searchFormSqlTableModel)
    {
    	_searchAchatsTableModel = searchFormSqlTableModel;
    }

    void updateLineEditDesignation();

private slots:

	bool filtrer_achats();

protected:

	void contextMenuEvent(QContextMenuEvent *event);

    virtual void hideEvent(QHideEvent * hideEvent);

    virtual void setupShortcuts();

private:

    void populateComboBoxes();

    inline void setupLineEdits()
    {
    	lineEdit_element_achats_resultat->setValidator(&YerothUtils::DoubleValidator);

    	updateLineEditDesignation();
    }

    static unsigned int PDF_LISTING_COLUMN_STOCKS_ID;

    static unsigned int PDF_LISTING_COLUMN_REFERENCE;

    static unsigned int PDF_LISTING_COLUMN_DESIGNATION;

    static unsigned int PDF_LISTING_COLUMN_CATEGORIE;

    static const QString 	_WINDOW_TITLE;

    YerothLogger			*_logger;

    bool					_currentlyFiltered;

    QProcess				*_aProcess;

    QFont 					*_pushButton_achats_filtrer_font;

    QFont 					*_pushButton_RechercherFont;

    QFont 					*_action_RechercherFont;

    YerothSearchForm 		*_searchAchatsWidget;

    YerothSqlTableModel 	*_curAchatSqlTableModel;

    YerothSqlTableModel 	*_searchAchatsTableModel;
};

#endif /* YEROTH_ERP_ACHATS_WINDOW_HPP_ */
