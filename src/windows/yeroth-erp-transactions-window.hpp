/*
 * yeroth-erp-transactions-window.hpp
 *
 *      Author: Xavier NOUMBISSI NOUNDOU, Dipl.-Inf., Ph.D. (ABD)
 */

#ifndef YEROTH_TRANSACTIONS_WINDOW_HPP_
#define YEROTH_TRANSACTIONS_WINDOW_HPP_


# include "../../ui_yeroth-erp-transactions-window.h"


#include "yeroth-erp-window-commons.hpp"


# include <QtWidgets/QMessageBox>


class YerothUtils;
class YerothSqlTableModel;
class QProcess;

class YerothTransactionsWindow : public YerothWindowsCommons,
								 private Ui_YerothTransactionsWindow
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

    virtual bool imprimer_pdf_document();

    inline void handleTabChanged(int index)
    {
    	rechercher();
    }

    void setLastListerSelectedRow(int row);

    void setFilter();

    void resetFilter();

    void rechercher();

    void reinitialiser_recherche();

protected slots:

    virtual void slot_reinitialiser_champs_db_visibles();

protected:

	virtual void reinitialiser_champs_db_visibles();

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


    YerothLogger				*_logger;

    QString 				_searchFilter;

    YerothSqlTableModel 		*_curTransactionsTableModel;
};


#endif /* YEROTH_TRANSACTIONS_WINDOW_HPP_ */
