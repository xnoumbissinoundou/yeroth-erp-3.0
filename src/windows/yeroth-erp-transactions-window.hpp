/*
 * yeroth-erp-transactions-window.hpp
 *
 *      Author: Xavier NOUMBISSI NOUNDOU, Dipl.-Inf., Ph.D. (ABD)
 */

#ifndef YEROTH_TRANSACTIONS_WINDOW_HPP_
#define YEROTH_TRANSACTIONS_WINDOW_HPP_


# include "../../ui_yeroth-erp-transactions-window.h"

# include <QtWidgets/QMessageBox>
#include "yeroth-erp-window-commons.hpp"

class YerothUtils;
class YerothSqlTableModel;
class QProcess;

class YerothTransactionsWindow : public YerothWindowsCommons, private Ui_YerothTransactionsWindow
{
    Q_OBJECT

public:

	YEROTH_CLASS_OPERATORS

	YerothTransactionsWindow();

    inline ~YerothTransactionsWindow()
    {
    	delete _logger;
    }

    inline virtual QToolBar * getQMainWindowToolBar()
    {
    	return toolBar_transactionsWindow;
    }

    virtual void rendreVisible(YerothSqlTableModel *stocksTableModel);

    void lister_les_elements_du_tableau(QString aSearchFilter);

    virtual void definirCaissier();

    virtual void definirManager();

    virtual void definirVendeur();

    virtual void definirGestionaireDesStocks();

    virtual void definirMagasinier();

    virtual void definirPasDeRole();

public slots:

	inline virtual void apropos()
	{
		YerothWindowsCommons::apropos(this);
	}

	inline virtual void help()
	{
		YerothQMessageBox::information(this,
						 QObject::trUtf8("aide"),
						 QObject::trUtf8(""));
	}

	bool export_csv_file();

	void getJournalDesTransactionsTexTableString(QString &texTable_in_out,
			  	  	  	   	   	   	   	    QStandardItemModel &tableStandardItemModel,
										    QList<int> &columnsToIgnore,
										    int fromRowIndex,
										    int toRowIndex,
										    bool lastPage);

    void imprimer_journal_transactions();

    void handleTabChanged(int index);

    void setLastListerSelectedRow(int row);

    void setFilter();

    void resetFilter();

    void rechercher();

    void reinitialiser_recherche();

protected:

    virtual void setupShortcuts();

private:

    void setupLineEdits();

    void setupLineEditsQCompleters();

    enum TabIndexes
    {
    	SUJET_ACTION_SORTIES_STOCKS 	= 0,
		SUJET_ACTION_TRANSFERTS_STOCKS	= 1
    };

    void setupDateTimeEdits();

    static const QString 	_WINDOW_TITLE;

    YerothLogger				*_logger;

    QString 				_searchFilter;

    YerothSqlTableModel 		*_curTransactionsTableModel;
};


#endif /* YEROTH_TRANSACTIONS_WINDOW_HPP_ */
