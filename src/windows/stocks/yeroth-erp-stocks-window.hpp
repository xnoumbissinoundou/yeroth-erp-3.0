/*
 * yeroth-erp-stocks-window.hpp
 *
 *      Author: DR. XAVIER NOUMBISSI NOUNDOU
 */

#ifndef YEROTH_ERP_STOCKS_WINDOW_HPP_
#define YEROTH_ERP_STOCKS_WINDOW_HPP_

#include "../../../ui_yeroth-erp-stocks-window.h"

#include <QtCore/QDebug>

#include <QtCore/QVector>

#include <QtWidgets/QMessageBox>

#include <QtGui/QContextMenuEvent>

#include "src/windows/yeroth-erp-window-commons.hpp"


class YerothPOSDialogCommons;
class QStandardItemModel;
class QContextMenuEvent;
class QProcess;

class YerothSqlTableModel;

class YerothStocksWindow : public YerothWindowsCommons,
						   private Ui_YerothStocksWindow,
						   public YerothAbstractClassYerothSearchWindow
{
    Q_OBJECT

public:

	YEROTH_CLASS_OPERATORS

    YerothStocksWindow();

    virtual ~YerothStocksWindow();

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

    MACRO_TO_DEFINE_VIEWING_PAGE_NUMBER_FOR_TABLEVIEW(label_stocks_numero_page_derniere,
    												  label_stocks_numero_page_courante)

    virtual void rendreVisible(YerothSqlTableModel *stocksTableModel);

public slots:

	MACRO_TO_DEFINE_VIEWING_POINTERS_PAGE_SLOTS(tableView_stocks)

	void gererChoixStrategieGestionDesStocks();

	inline virtual void apropos()
	{
		YerothWindowsCommons::apropos(this);
	}

	inline virtual void help()
	{
		YerothQMessageBox::information(this,
							 QObject::trUtf8("aide"),
							 QObject::trUtf8("1. Sélectionnez un stock dans le tableau des stocks,"
											 " ensuite cliquer sur l'opération que vous souhaitez"
											 " réaliser !\n\n"
											 "2. La valeur d'inventaire d'un stock (VI) "
											 "est calculée comme suit:\n"
											 " [VI = prix_dachat * quantite_en_stock] !"));
	}

	bool export_csv_file();

	virtual bool imprimer_pdf_document();

    virtual void entrer();

    void lister_les_elements_du_tableau(YerothSqlTableModel &sqlTableModel,
    									QString localVisibleStrategy = YerothUtils::EMPTY_STRING);

    virtual void lister_les_elements_du_tableau(QString strategieGlobale = YerothUtils::EMPTY_STRING);

    virtual void modifier_les_articles();

    void afficher_au_detail();

    void afficher_au_detail(const QModelIndex &modelIndex);

    void supprimer_PLUSIEURS_Stocks(YerothSqlTableModel &aStocksTableModel);

    void supprimer_ce_stock(QString aStockID = YerothUtils::EMPTY_STRING,
    						bool _reEntrant = false);

    void refineYerothLineEdits();

    void reinitialiser_elements_filtrage();

    void reinitialiser_recherche();

    void set_filtrer_font();

private slots:

	void private_slot_afficher_historique_du_stock();

	bool filtrer_stocks_en_alerte_de_stock();

	bool filtrer_stocks_perimes_seulement();

	inline void afficher_stocks()
	{
		textChangedSearchLineEditsQCompleters();
	}

	bool filtrer_stocks();

protected:

	virtual void reinitialiser_colones_db_visibles();

    void contextMenuEvent(QContextMenuEvent *event);

    virtual void hideEvent(QHideEvent * hideEvent);

    virtual void setupShortcuts();

    void resetFilter();

protected slots:

    virtual void slot_reinitialiser_colones_db_visibles();

    virtual void textChangedSearchLineEditsQCompleters();

private:

    void activerComboBoxStrategieDeGestionDesStocks();

    void desactiverComboBoxStrategieDeGestionDesStocks();

    inline void setComboBoxStrategieDeStocks()
    {
    	comboBox_strategie_de_stocks
    		->setCurrentIndex(YerothUtils::getComboBoxDatabaseQueryValue(YerothERPConfig::salesStrategy,
    						  YerothUtils::_strategieindexToUserViewString));
    }

    void disableValeurDinventaire();

    void enableValeurDinventaire_ONLY_MANAGER();

    void populateComboBoxes();

    void setupLineEdits();

    void setupDateTimeEdits();

    QString					_localStrategy;

    YerothLogger			*_logger;

    QStringList				_historiqueStock;

    QString 				_stocksDateEntreeFilter;

    QFont 					*_pushButton_stocks_filtrer_font;
};

#endif /* YEROTH_ERP_STOCKS_WINDOW_HPP_ */
