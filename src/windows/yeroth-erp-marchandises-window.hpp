/*
 * yeroth-erp-marchandises-window.hpp
 *
 *      Author: Dipl.-Inf. XAVIER NOUMBISSI NOUNDOU
 */

#ifndef YEROTH_ERP_MARCHANDISES_WINDOW_HPP_
#define YEROTH_ERP_MARCHANDISES_WINDOW_HPP_

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
								 private Ui_YerothERPMarchandisesWindow,
								 public YerothAbstractClassYerothSearchWindow
{
    Q_OBJECT

public:

	YEROTH_CLASS_OPERATORS

    YerothMarchandisesWindow();

    virtual ~YerothMarchandisesWindow();

    inline virtual QToolBar * getQMainWindowToolBar()
    {
    	return toolBar_marchandisesWindow;
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

    MACRO_TO_DEFINE_VIEWING_PAGE_NUMBER_FOR_TABLEVIEW(label_marchandises_numero_page_derniere,
    												  label_marchandises_numero_page_courante)

    virtual void rendreVisible(YerothSqlTableModel *stocksTableModel);

public slots:

	inline virtual void apropos()
	{
		YerothWindowsCommons::apropos(this);
	}

	inline virtual void help()
	{
		YerothQMessageBox::information(this,
							 QObject::trUtf8("aide"),
							 QObject::trUtf8("1) Sélectionner un marchandise dans le tableau "
									 	 	 "de marchandises, ensuite cliquer "
											 "sur l'opération que vous souhaitez réaliser !\n\n"
											 "2) La valeur d'inventaire d'une marchandise (VI) "
											 "est calculée comme suit:\n"
											 " [VI = prix_dachat * quantite_en_stock] !"));
	}

	bool export_csv_file();

	virtual bool imprimer_pdf_document();

    virtual void afficherMarchandises(YerothSqlTableModel &aYerothSqlTableModel);

	inline void afficherMarchandises()
	{
		afficherMarchandises(*_curMarchandisesTableModel);
	}

    void afficher_stock_selectioner(const QString &stockName);

    void modifier_cette_reference();

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

	inline YerothLogger *getLogger()
	{
		return _logger;
	}

protected:

	virtual void reinitialiser_champs_db_visibles();

    void contextMenuEvent(QContextMenuEvent *event);

    virtual void setupShortcuts();

protected slots:

    virtual void slot_reinitialiser_champs_db_visibles();

	virtual void textChangedSearchLineEditsQCompleters();

private slots:

	void handle_services_checkBox(int state);

	bool slot_filtrer_non_empty_product_stock();

	bool slot_filtrer_empty_product_stock();

	bool slot_filtrer();

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

#endif /* YEROTH_ERP_MARCHANDISES_WINDOW_HPP_ */
