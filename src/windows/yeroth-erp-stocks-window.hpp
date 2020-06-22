/*
 * yeroth-erp-stocks-window.hpp
 *
 *      Author: Dipl.-Inf. XAVIER NOUMBISSI NOUNDOU
 */

#ifndef YEROTH_ERP_STOCKS_WINDOW_HPP_
#define YEROTH_ERP_STOCKS_WINDOW_HPP_

#include "../../ui_yeroth-erp-stocks-window.h"

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

    virtual void rendreVisible(YerothSqlTableModel *stocksTableModel);

    virtual void rendreInvisible();

public slots:

	void gererChoixStrategieGestionDesStocks();

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

	virtual bool imprimer_pdf_document();

    virtual void entrer();

    void afficherStocks(YerothSqlTableModel &sqlTableModel, QString localVisibleStrategy  = YerothUtils::EMPTY_STRING);

    virtual void afficherStocks(QString strategieGlobale = YerothUtils::EMPTY_STRING);

    virtual void modifier_les_articles();

    void afficher_au_detail();

    void afficher_au_detail(const QModelIndex &modelIndex);

    void connecter_localisation();

    void deconnecter_localisation();

    void supprimer_ce_stock();

    void reinitialiser_elements_filtrage();

    void reinitialiser_recherche();

    void set_filtrer_font();

    inline int getLastListerSelectedRow()
    {
    	return tableView_stocks->lastSelectedRow();
    }

    inline void setLastListerSelectedRow(int row)
    {
    	tableView_stocks->setLastSelectedRow(row);
    }

    bool SQL_TABLE_STOCKS_VENDU_EMPTY();

private slots:

	void private_slot_afficher_historique_du_stock();

	bool filtrer_stocks_perimes_seulement();

	bool filtrer_stocks();

	inline void setRechercheCodebarArticleFocus()
	{
		lineEdit_recherche_reference->setFocus();
	}

protected:

	virtual void reinitialiser_champs_db_visibles();

    void contextMenuEvent(QContextMenuEvent *event);

    virtual void hideEvent(QHideEvent * hideEvent);

    virtual void setupShortcuts();

protected slots:

    virtual void slot_reinitialiser_champs_db_visibles();

    virtual void textChangedSearchLineEditsQCompleters();

private:

    void activerComboBoxStrategieDeGestionDesStocks();

    void desactiverComboBoxStrategieDeGestionDesStocks();

    void populateComboBoxes();

    void setComboBoxStrategieDeStocks();

    void setupLineEdits();


    QString					_localStrategy;

    YerothLogger			*_logger;

    QStringList				_historiqueStock;

    QAction					*_actionRechercheArticleCodebar;

    QProcess				*_aProcess;

    QFont 					*_pushButton_stocks_filtrer_font;

    YerothSqlTableModel 	*_searchStocksTableModel;
};

#endif /* YEROTH_ERP_STOCKS_WINDOW_HPP_ */
