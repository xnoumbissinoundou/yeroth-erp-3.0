/*
 * yeroth-erp-groupes-de-clients-window.hpp
 *
 *      Author: DR. XAVIER NOUMBISSI NOUNDOU
 */

#ifndef YEROTH_ERP_GROUPES_DE_CLIENTS_WINDOW_HPP_
#define YEROTH_ERP_GROUPES_DE_CLIENTS_WINDOW_HPP_

#include "../../../ui_yeroth-erp-groupes-de-clients-window.h"

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

class YerothGroupesDeClientsWindow : public YerothWindowsCommons,
						   	   	   	    private Ui_YerothERPGroupesDeClientsWindow,
										public YerothAbstractClassYerothSearchWindow
{
    Q_OBJECT

public:

	YEROTH_CLASS_OPERATORS

    YerothGroupesDeClientsWindow();

    virtual ~YerothGroupesDeClientsWindow();

    inline virtual QToolBar * getQMainWindowToolBar()
    {
    	return toolBar_groupes_de_clientsWindow;
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

    MACRO_TO_DEFINE_VIEWING_PAGE_NUMBER_FOR_TABLEVIEW(label_groupes_de_clients_numero_page_derniere,
    												  label_groupes_de_clients_numero_page_courante)

    virtual void rendreVisible(YerothSqlTableModel *stocksTableModel);

public slots:

	MACRO_TO_DEFINE_VIEWING_POINTERS_PAGE_SLOTS(tableView_groupes_de_clients)

	inline virtual void apropos()
	{
		YerothWindowsCommons::apropos(this);
	}

	inline virtual void help()
	{
		YerothQMessageBox::information(this,
							 QObject::tr("aide"),
							 QObject::trUtf8("1) Sélectionner un 'groupe de clients' dans le"
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

	void supprimer_PLUSIEURS_groupes_de_clients(YerothSqlTableModel &aClientGroupTableModel);

	void supprimer_groupe_de_clients();

	void afficher_groupes_de_clients(YerothSqlTableModel &aClientGroupTableModel);

	inline void afficher_groupes_de_clients()
	{
		afficher_groupes_de_clients(*_curClientGroupTableModel);
	}

	bool filtrer_groupes_de_clients();

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

    QString 				_clientGroupDateCreationFilter;

    QFont 					*_pushButton_groupes_de_clients_filtrer_font;

    YerothSqlTableModel 	*_curClientGroupTableModel;
};

#endif /* YEROTH_ERP_GROUPES_DE_CLIENTS_WINDOW_HPP_ */
