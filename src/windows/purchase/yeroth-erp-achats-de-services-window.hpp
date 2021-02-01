/*
 * yeroth-erp-achats-de-services-window.hpp
 *
 *      Author: DR. XAVIER NOUMBISSI NOUNDOU
 */

#ifndef YEROTH_ERP_ACHATS_DE_SERVICES_WINDOW_HPP_
#define YEROTH_ERP_ACHATS_DE_SERVICES_WINDOW_HPP_

#include "../../../ui_yeroth-erp-achats-de-services-window.h"


#include <QtCore/QDebug>

#include <QtWidgets/QMessageBox>

#include <QtGui/QContextMenuEvent>


#include "src/windows/yeroth-erp-window-commons.hpp"


class QStandardItemModel;
class QContextMenuEvent;
class QProcess;

class YerothSqlTableModel;

class YerothAchatsDEServicesWindow : public YerothWindowsCommons,
						   	   	   	 private Ui_YerothAchatsDEServicesWindow,
									 public YerothAbstractClassYerothSearchWindow
{
    Q_OBJECT

public:

	YEROTH_CLASS_OPERATORS

    YerothAchatsDEServicesWindow();

    virtual ~YerothAchatsDEServicesWindow();

    inline virtual QToolBar * getQMainWindowToolBar()
    {
    	return toolBar_achats_de_servicesWindow;
    }

    virtual void definirCaissier();

    virtual void definirManager();

    virtual void definirVendeur();

    virtual void definirGestionaireDesStocks();

    virtual void definirMagasinier();

    virtual void definirPasDeRole();

    MACRO_TO_DEFINE_VIEWING_PAGE_NUMBER_FOR_TABLEVIEW(label_achats_de_services_numero_page_derniere,
    												  label_achats_de_services_numero_page_courante)

    virtual void rendreVisible(YerothSqlTableModel *stocksTableModel);

public slots:

	MACRO_TO_DEFINE_VIEWING_POINTERS_PAGE_SLOTS(tableView_achats_de_services)

	inline virtual void apropos()
	{
		YerothWindowsCommons::apropos(this);
	}

	inline virtual void help()
	{
		YerothQMessageBox::information(this,
							 QObject::trUtf8("aide"),
							 QObject::trUtf8("Sélectionner un achat dans le tableau des achats de services, ensuite cliquer "
											 "sur l'opération que vous souhaitez réaliser !"));
	}

	bool export_csv_file();

    virtual bool imprimer_pdf_document();

    virtual void lister_les_elements_du_tableau(YerothSqlTableModel &achatSqlTableModel);

    inline void lister_les_elements_du_tableau()
    {
    	lister_les_elements_du_tableau(*_curAchatsDEServicesSqlTableModel);
    }

    void afficher_stock_selectioner(const QString &stockName);

    void afficher_au_detail();

    void afficher_au_detail(const QModelIndex &modelIndex);

    void refineYerothLineEdits();

    void reinitialiser_elements_filtrage();

    void reinitialiser_recherche();

    void set_filtrer_font();

    bool supprimer_un_achat_au_fournisseur();

protected:

	virtual void reinitialiser_colones_db_visibles();

	virtual void contextMenuEvent(QContextMenuEvent *event);

    virtual void setupShortcuts();

    void resetFilter(YerothSqlTableModel *achatsTableModel);

 protected slots:

    virtual void slot_reinitialiser_colones_db_visibles();

    virtual void textChangedSearchLineEditsQCompleters();

private slots:

	inline void afficher_achats()
	{
		textChangedSearchLineEditsQCompleters();
	}

    bool filtrer_achats();

private:

    void populateComboBoxes();

    void setupLineEdits();

    void setupDateTimeEdits();

    static unsigned int PDF_LISTING_COLUMN_STOCKS_ID;

    static unsigned int PDF_LISTING_COLUMN_REFERENCE;

    static unsigned int PDF_LISTING_COLUMN_DESIGNATION;

    static unsigned int PDF_LISTING_COLUMN_CATEGORIE;

    YerothLogger			*_logger;

    QFont 					*_pushButton_achats_de_services_filtrer_font;

    QString 				_achats_de_servicesDateFilter;

    YerothSqlTableModel 	*_curAchatsDEServicesSqlTableModel;
};

#endif /* YEROTH_ERP_ACHATS_DE_SERVICES_WINDOW_HPP_ */