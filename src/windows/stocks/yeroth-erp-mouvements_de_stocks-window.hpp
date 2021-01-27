/*
 * yeroth-erp-mouvements_de_stocks-window.hpp
 *
 *      Author: DR. XAVIER NOUMBISSI NOUNDOU
 */

#ifndef MOUVEMENTSDESTOCKS_WINDOW_HPP_
#define MOUVEMENTSDESTOCKS_WINDOW_HPP_


# include "../../../ui_yeroth-erp-mouvements_de_stocks-window.h"


#include "src/windows/yeroth-erp-window-commons.hpp"


# include <QtWidgets/QMessageBox>


class YerothUtils;
class YerothSqlTableModel;
class QProcess;

class YerothMouvementsDeStocksWindow : public YerothWindowsCommons,
								 	   private Ui_YerothMouvementsDeStocksWindow,
									   public YerothAbstractClassYerothSearchWindow
{
    Q_OBJECT

public:

	YEROTH_CLASS_OPERATORS

	YerothMouvementsDeStocksWindow();

    ~YerothMouvementsDeStocksWindow();

    inline virtual QToolBar * getQMainWindowToolBar()
    {
    	return toolBar_mouvementsDeStocksWindow;
    }

    MACRO_TO_DEFINE_VIEWING_PAGE_NUMBER_FOR_TABLEVIEW(_curLabel_mouvementsDeStocks_numero_page_derniere,
    												  _curLabel_mouvementsDeStocks_numero_page_courante)

    virtual void rendreVisible(YerothSqlTableModel *stocksTableModel);

    void lister_les_elements_du_tableau(const QString &aSearchFilter = YerothUtils::EMPTY_STRING);

    virtual void definirCaissier();

    virtual void definirManager();

    virtual void definirVendeur();

    virtual void definirGestionaireDesStocks();

    virtual void definirMagasinier();

    virtual void definirPasDeRole();

public slots:

	MACRO_TO_DEFINE_VIEWING_POINTERS_PAGE_SLOTS(_curMouvementsDeStocksTableView)

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

    void handleTabChanged(int index);

    void setFilter();

    void resetFilter();

	inline void afficher_stocks_sorties_OU_transferes()
	{
		textChangedSearchLineEditsQCompleters();
	}

	void refineYerothLineEdits();

    void reinitialiser_recherche();

protected slots:

	virtual void slot_reinitialiser_colones_db_visibles();

	virtual void textChangedSearchLineEditsQCompleters();

protected:

	virtual void reinitialiser_colones_db_visibles();

    virtual void setupShortcuts();

private slots:

	bool filtrer_stocks_sorties_OU_transferes();

private:

    enum TabIndexes
    {
    	SUJET_ACTION_SORTIES_STOCKS 	= 0,
		SUJET_ACTION_TRANSFERTS_STOCKS	= 1
    };

    void setupLineEdits();

    void switchTableWidgetTAB_CONTENT_ELEMENTS(enum TabIndexes aTabIndex);

    void setupDateTimeEdits();


    YerothLogger			*_logger;

    QLabel 					*_curLabel_mouvementsDeStocks_numero_page_derniere;

    QLabel					*_curLabel_mouvementsDeStocks_numero_page_courante;

    QLineEdit				*_curLineEdit_mouvementsDeStocks_terme_recherche;

    QLineEdit				*_curLineEdit_mouvementsDeStocks_element_de_AAA_resultat;

    YerothDateTimeEdit		*_curDateEdit_debut;

    YerothDateTimeEdit		*_curDateEdit_fin;

    YerothTableViewWITHpagination 	*_curMouvementsDeStocksTableView;

    QString 				_stocksSorties_OU_transferes_DateFilter;

    YerothSqlTableModel 	*_curMouvementsDeStocksTableModel;
};


#endif /* MOUVEMENTSDESTOCKS_WINDOW_HPP_ */
