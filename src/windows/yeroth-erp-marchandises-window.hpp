/*
 * yeroth-erp-marchandises-window.hpp
 *
 *      Author: Dipl.-Inf. XAVIER NOUMBISSI NOUNDOU
 */

#ifndef YEROTH_ERP_INVENTAIRE_DES_STOCKS_WINDOW_HPP_
#define YEROTH_ERP_INVENTAIRE_DES_STOCKS_WINDOW_HPP_

#include "../../ui_yeroth-erp-marchandises-window.h"


#include <QtCore/QDebug>

#include <QtWidgets/QMessageBox>

#include <QtGui/QContextMenuEvent>


#include "yeroth-erp-window-commons.hpp"


class QStandardItemModel;
class QContextMenuEvent;
class QProcess;

class YerothSqlTableModel;


class YerothMarchandisesWindow : public YerothWindowsCommons,
								 private Ui_YerothStocksDeSecuriteWindow,
								 public YerothAbstractClassYerothSearchWindow
{
    Q_OBJECT

public:

	YEROTH_CLASS_OPERATORS

    YerothMarchandisesWindow();

    virtual ~YerothMarchandisesWindow();

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

    inline void setCheckBoxServices(bool checked)
    {
    	checkBox_services->setChecked(checked);
    }

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

	virtual bool imprimer_pdf_document();

    virtual void afficherMarchandises(YerothSqlTableModel &aYerothSqlTableModel);

	inline void afficherMarchandises()
	{
		afficherMarchandises(*_curMarchandisesTableModel);
	}

    void afficher_stock_selectioner(const QString &stockName);

    void supprimer_ce_stock();

    void reinitialiser_elements_filtrage();

    void reinitialiser_recherche();

    inline int getLastListerSelectedRow()
    {
    	return tableView_marchandises->lastSelectedRow();
    }

    inline void setLastListerSelectedRow(int row)
    {
    	tableView_marchandises->setLastSelectedRow(row);
    }

    static double getValeurTotaleDinventaireEnStock(QString categorie, QString designation);

    static double getQuantiteTotaleEnStock(QString categorie, QString designation);

	double getQuantiteTotaleEnStock(const QModelIndex &aQModelIndex);

protected:

	virtual void reinitialiser_champs_db_visibles();

    void contextMenuEvent(QContextMenuEvent *event);

    virtual void setupShortcuts();

protected slots:

    virtual void slot_reinitialiser_champs_db_visibles();

	virtual void textChangedSearchLineEditsQCompleters();

private slots:

	void handle_services_checkBox(int state);

	bool filtrer_empty_product_stock();

	bool filtrer();

public:

    double 					_valeurTheoriqueDinventaire;

    double 					_qteTotaleDarticlesEnStock;

private:

	void localSetupLineEditsQCompleters();

	void set_filtrer_font();

    void populateMarchandisesComboBoxes();

    void setupLineEdits();


    YerothLogger			*_logger;

    bool					_currentlyFiltered;

    int 					_lastSelectedRow;

    QFont 					*_pushButton_filtrer_font;

    YerothSqlTableModel 	*_curMarchandisesTableModel;
};

#endif /* YEROTH_ERP_INVENTAIRE_DES_STOCKS_WINDOW_HPP_ */
