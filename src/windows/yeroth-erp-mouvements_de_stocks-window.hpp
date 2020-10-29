/*
 * yeroth-erp-mouvements_de_stocks-window.hpp
 *
 *      Author: DIPL.-INF. XAVIER NOUMBISSI NOUNDOU
 */

#ifndef MOUVEMENTSDESTOCKS_WINDOW_HPP_
#define MOUVEMENTSDESTOCKS_WINDOW_HPP_


# include "../../ui_yeroth-erp-mouvements_de_stocks-window.h"


#include "yeroth-erp-window-commons.hpp"


# include <QtWidgets/QMessageBox>


class YerothUtils;
class YerothSqlTableModel;
class QProcess;

class YerothMouvementsDeStocksWindow : public YerothWindowsCommons,
								 	   private Ui_YerothMouvementsDeStocksWindow
{
    Q_OBJECT

public:

	YEROTH_CLASS_OPERATORS

	YerothMouvementsDeStocksWindow();

    inline ~YerothMouvementsDeStocksWindow()
    {
    	delete _logger;
    }

    inline virtual QToolBar * getQMainWindowToolBar()
    {
    	return toolBar_mouvementsDeStocksWindow;
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

    void setFilter();

    void resetFilter();

    void rechercher();

    void reinitialiser_recherche();

protected slots:

	virtual void slot_reinitialiser_champs_db_visibles();

    inline virtual void disableExporterAuFormatCsv()
    {
    	actionExporter_au_format_csv->setVisible(false);
    }

    inline virtual void enableExporterAuFormatCsv()
    {
    	actionExporter_au_format_csv->setVisible(true);
    }

    inline virtual void disableImprimer()
    {
    	actionAfficherPDF->setVisible(false);
    }

    inline virtual void enableImprimer()
    {
    	actionAfficherPDF->setVisible(true);
    }

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


    YerothLogger			*_logger;

    QString 				_searchFilter;

    YerothSqlTableModel 	*_curMouvementsDeStocksTableModel;
};


#endif /* MOUVEMENTSDESTOCKS_WINDOW_HPP_ */
