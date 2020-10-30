/*
 * yeroth-erp-achats-window.hpp
 *
 *      Author: DIPL.-INF. XAVIER NOUMBISSI NOUNDOU
 */

#ifndef YEROTH_ERP_ACHATS_WINDOW_HPP_
#define YEROTH_ERP_ACHATS_WINDOW_HPP_

#include "../../ui_yeroth-erp-achats-window.h"


#include <QtCore/QDebug>

#include <QtWidgets/QMessageBox>

#include <QtGui/QContextMenuEvent>


#include "yeroth-erp-window-commons.hpp"


class QStandardItemModel;
class QContextMenuEvent;
class QProcess;

class YerothSqlTableModel;

class YerothAchatsWindow : public YerothWindowsCommons,
						   private Ui_YerothAchatsWindow,
						   public YerothAbstractClassYerothSearchWindow
{
    Q_OBJECT

public:

	YEROTH_CLASS_OPERATORS

    YerothAchatsWindow();

    virtual ~YerothAchatsWindow();

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

    MACRO_TO_DEFINE_VIEWING_PAGE_NUMBER_FOR_TABLEVIEW(label_achats_numero_page_derniere,
    												  label_achats_numero_page_courante)

    virtual void rendreVisible(YerothSqlTableModel *stocksTableModel);

public slots:

	MACRO_TO_DEFINE_VIEWING_POINTERS_PAGE_SLOTS(tableView_achats)

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

    virtual bool imprimer_pdf_document();

    virtual void afficherAchats(YerothSqlTableModel &achatSqlTableModel);

    inline void afficherAchats()
    {
    	afficherAchats(*_curAchatSqlTableModel);
    }

    void afficher_stock_selectioner(const QString &stockName);

    void afficher_au_detail();

    void afficher_au_detail(const QModelIndex &modelIndex);

    void reinitialiser_elements_filtrage();

    void reinitialiser_recherche();

    void set_filtrer_font();

protected:

	virtual void reinitialiser_champs_db_visibles();

	void contextMenuEvent(QContextMenuEvent *event);

    virtual void setupShortcuts();

 protected slots:

    virtual void slot_reinitialiser_champs_db_visibles();

    virtual void textChangedSearchLineEditsQCompleters();

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

private slots:

    bool filtrer_achats();

private:

    void populateComboBoxes();

    void setupLineEdits();

    static unsigned int PDF_LISTING_COLUMN_STOCKS_ID;

    static unsigned int PDF_LISTING_COLUMN_REFERENCE;

    static unsigned int PDF_LISTING_COLUMN_DESIGNATION;

    static unsigned int PDF_LISTING_COLUMN_CATEGORIE;

    YerothLogger			*_logger;

    QFont 					*_pushButton_achats_filtrer_font;

    YerothSqlTableModel 	*_curAchatSqlTableModel;
};

#endif /* YEROTH_ERP_ACHATS_WINDOW_HPP_ */
