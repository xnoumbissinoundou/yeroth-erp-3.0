/*
 * yeroth-erp-marchandises-window.hpp
 *
 *      Author: Xavier NOUMBISSI NOUNDOU, Dipl.-Inf., Ph.D. (ABD)
 */

#ifndef SRC_WINDOWS_YEROTH_ERP_CLIENTS_WINDOW_HPP_
#define SRC_WINDOWS_YEROTH_ERP_CLIENTS_WINDOW_HPP_

#include "../../ui_yeroth-erp-clients-window.h"


#include "yeroth-erp-window-commons.hpp"


#include <QtCore/QDebug>

#include <QtWidgets/QMessageBox>

#include <QtGui/QContextMenuEvent>


class QStandardItemModel;
class QContextMenuEvent;
class QProcess;

class YerothSqlTableModel;

class YerothERPClientsWindow : public YerothWindowsCommons,
							   private Ui_YerothERPClientsWindow,
							   public YerothAbstractClassYerothSearchWindow
{
    Q_OBJECT

public:

	YEROTH_CLASS_OPERATORS

    YerothERPClientsWindow();

    virtual ~YerothERPClientsWindow();

    inline virtual QToolBar * getQMainWindowToolBar()
    {
    	return toolBar_clientsWindow;
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

	inline virtual void apropos()
	{
		YerothWindowsCommons::apropos(this);
	}

	inline virtual void help()
	{
		YerothQMessageBox::information(this,
							 QObject::trUtf8("aide"),
							 QObject::trUtf8("Sélectionner un compte client dans le tableau "
											 "des comptes clients, ensuite cliquer sur l'opération que vous souhaitez réaliser !"));
	}

	bool export_csv_file();

	static void getComptesClientsListingTexTableString(QString &texTable_in_out,
	        								   	   	   QStandardItemModel &tableStandardItemModel,
													   QList<int> &dbFieldNameOfTypeString,
													   QList<int> &columnsToIgnore,
													   int fromRowIndex,
													   int toRowIndex,
													   bool lastPage);

	static void getComptesClientsTexDocumentString(QString &texDocumentString_in_out,
			   	   	   	   	   	   	   	   	   	   QString &printString);

	virtual bool imprimer_pdf_document();

    inline int getLastListerSelectedRow()
    {
    	return tableView_clients->lastSelectedRow();
    }

    inline void setLastListerSelectedRow(int row)
    {
    	tableView_clients->setLastSelectedRow(row);
    }

    virtual void afficherClients(YerothSqlTableModel &clientSqlTableModel);

    inline void afficherClients()
    {
    	afficherClients(*_curClientsTableModel);
    }

    void afficher_nom_entreprise_selectioner(const QString &nomEntreprise);

protected:

    virtual void reinitialiser_champs_db_visibles();

    void contextMenuEvent(QContextMenuEvent *event);

    virtual void hideEvent(QHideEvent * hideEvent);

    virtual void setupShortcuts();

    virtual void set_filtrer_font();

protected slots:

    virtual void slot_reinitialiser_champs_db_visibles();

    virtual void textChangedSearchLineEditsQCompleters();

private slots:

	void private_payer_au_compteclient();

	void private_slot_afficher_les_transactions_dun_client();

	void modifierCompteClient();

	void supprimerCompteClient();

	void afficher_au_detail();

	void afficher_au_detail(const QModelIndex & modelIndex);

	bool filtrer();

    void reinitialiser_elements_filtrage();

    void reinitialiser_recherche();

private:

    void populateClientsComboBoxes();

    void setupLineEdits();


    static const QString 	_WINDOW_TITLE;

    YerothLogger			*_logger;

    int 					_lastSelectedRow;

    QFont 					*_pushButton_filtrer_font;

    YerothSqlTableModel 	*_curClientsTableModel;
};

#endif /* SRC_WINDOWS_YEROTH_ERP_CLIENTS_WINDOW_HPP_ */
