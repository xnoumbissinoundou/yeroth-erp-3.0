/*
 * yeroth-erp-programmes-de-fidelite-clients-window.hpp
 *
 *      Author: DR. XAVIER NOUMBISSI NOUNDOU
 */

#ifndef YEROTH_ERP_PROGRAMMES_DE_FIDELITE_CLIENTS_WINDOW_HPP_
#define YEROTH_ERP_PROGRAMMES_DE_FIDELITE_CLIENTS_WINDOW_HPP_

#include "../../../ui_yeroth-erp-programmes-de-fidelite-clients-window.h"

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

class YerothERPProgrammesDeFideliteClientsWindow : public YerothWindowsCommons,
						   	   	   	 	 	 	   private Ui_YerothERPProgrammesDeFideliteClientsWindow,
												   public YerothAbstractClassYerothSearchWindow
{
    Q_OBJECT

public:

	YEROTH_CLASS_OPERATORS

    YerothERPProgrammesDeFideliteClientsWindow();

    virtual ~YerothERPProgrammesDeFideliteClientsWindow();

    inline virtual QToolBar * getQMainWindowToolBar()
    {
    	return toolBar_programmes_de_fidelite_clientsWindow;
    }

    virtual void definirCaissier();

    virtual void definirManager();

    virtual void definirVendeur();

    virtual void definirGestionaireDesStocks();

    virtual void definirMagasinier();

    virtual void definirPasDeRole();

    MACRO_TO_DEFINE_VIEWING_PAGE_NUMBER_FOR_TABLEVIEW(label_programmes_de_fidelite_clients_numero_page_derniere,
    												  label_programmes_de_fidelite_clients_numero_page_courante)

    virtual void rendreVisible(YerothSqlTableModel *stocksTableModel);

public slots:

	MACRO_TO_DEFINE_VIEWING_POINTERS_PAGE_SLOTS(tableView_programmes_de_fidelite_clients)

	inline virtual void apropos()
	{
		YerothWindowsCommons::apropos(this);
	}

	inline virtual void help()
	{
		YerothQMessageBox::information(this,
							 QObject::tr("aide"),
							 QObject::trUtf8("1) Sélectionner un 'programme de fidélité clients' dans le"
									 	 	 " tableau de cette page, ensuite cliquer sur"
									 	 	 " l'opération que vous souhaitez"
											 " réaliser !"));
	}

	bool export_csv_file();

	virtual bool imprimer_pdf_document();

    void afficher_au_detail();

	inline void afficher_au_detail(const QModelIndex &modelIndex)
	{
		afficher_au_detail();
	}

    void refineYerothLineEdits();

    void reinitialiser_elements_filtrage();

    void reinitialiser_recherche();

    void set_filtrer_font();

private slots:

	virtual void afficher_programmes_de_fidelite_clients(YerothSqlTableModel &client_ROYALTY_PROGRAM_TableModel);

	bool filtrer_programmes_de_fidelite_client();

protected:

	virtual void reinitialiser_champs_db_visibles();

    void contextMenuEvent(QContextMenuEvent *event);

    virtual void setupShortcuts();

    void resetFilter();

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

private:

    void populateComboBoxes();

    void setupLineEdits();

    void setupDateTimeEdits();

    QString					_localStrategy;

    YerothLogger			*_logger;

    QStringList				_historiqueStock;

    QString 				_client_ROYALTY_PROGRAM_DateCreationFilter;

    QFont 					*_pushButton_programmes_de_fidelite_clients_filtrer_font;

    YerothSqlTableModel 	*_curClient_ROYALTY_PROGRAM_TableModel;
};

#endif /* YEROTH_ERP_PROGRAMMES_DE_FIDELITE_CLIENTS_WINDOW_HPP_ */
