/*
 * yeroth-erp-programmes-de-fidelite-clients-window.hpp
 *
 *      Author: PR. XAVIER NOUMBISSI NOUNDOU
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

    inline virtual void definirCaissier()
    {
            definirPasDeRole();
    }

    virtual void definirManager();

    inline virtual void definirVendeur()
    {
            definirPasDeRole();
    }

    inline virtual void definirGestionaireDesStocks()
    {
            definirPasDeRole();
    }

    inline virtual void definirMagasinier()
    {
        definirPasDeRole();
    }

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

	virtual QString get_latex_template_print_pdf_content();

    void afficher_au_detail();

	inline void afficher_au_detail(const QModelIndex &modelIndex)
	{
		afficher_au_detail();
	}

    void refineYerothLineEdits();

    bool filtrer();

    void reinitialiser_elements_filtrage();

    void reinitialiser_recherche();

    void set_filtrer_font();

private slots:

	void supprimer_PLUSIEURS_programmes_de_fidelite_clients(YerothSqlTableModel &a_LOYALTY_PROGRAM_TableModel);

	void supprimer_un_programme_de_fidelite_clients();

	void afficher_programmes_de_fidelite_clients(YerothSqlTableModel &client_LOYALTY_PROGRAM_TableModel);

	inline void afficher_programmes_de_fidelite_clients()
	{
		afficher_programmes_de_fidelite_clients(*_curClient_LOYALTY_PROGRAM_TableModel);
	}

	bool filtrer_programmes_de_fidelite_client();

protected:

	virtual void reinitialiser_colones_db_visibles();

    virtual void contextMenuEvent(QContextMenuEvent *event);

    virtual void setupShortcuts();

    void resetFilter();

protected slots:

    virtual void slot_reinitialiser_colones_db_visibles();

    virtual void textChangedSearchLineEditsQCompleters();

private:

    void populateComboBoxes();

    void setupLineEdits();

    void setupDateTimeEdits();

    QString					_localStrategy;

    YerothLogger			*_logger;

    QStringList				_historiqueStock;

    QString 				_client_LOYALTY_PROGRAM_DateCreationFilter;

    QFont 					*_pushButton_filtrer_font;

    YerothSqlTableModel 	*_curClient_LOYALTY_PROGRAM_TableModel;
};

#endif /* YEROTH_ERP_PROGRAMMES_DE_FIDELITE_CLIENTS_WINDOW_HPP_ */
