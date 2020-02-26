/*
 * yeroth-erp-marchandises-window.hpp
 *
 *      Author: Xavier NOUMBISSI NOUNDOU, Dipl.-Inf., Ph.D. (ABD)
 */

#ifndef SRC_WINDOWS_YEROTH_ERP_CLIENTS_WINDOW_HPP_
#define SRC_WINDOWS_YEROTH_ERP_CLIENTS_WINDOW_HPP_

#include "../../ui_yeroth-erp-clients-window.h"

#include "src/windows/yeroth-erp-search-form.hpp"

#include "yeroth-erp-window-commons.hpp"


#include <QtCore/QDebug>

#include <QtWidgets/QMessageBox>

#include <QtGui/QContextMenuEvent>


class QStandardItemModel;
class QContextMenuEvent;
class QProcess;

class YerothSqlTableModel;

class HistoryPaymentInfo
{
public:

	YEROTH_CLASS_OPERATORS

	inline HistoryPaymentInfo()
	:compte_client(0.0),
	 montant_paye(0.0)
	{
	}

	inline ~HistoryPaymentInfo()
	{
	}

	QString nom_entreprise;
	QString nom_encaisseur;
	QString type_de_paiement;
	QDate date_paiement;

	double compte_client;
	double montant_paye;
};

class YerothERPClientsWindow : public YerothWindowsCommons, private Ui_YerothERPClientsWindow
{
    Q_OBJECT

public:

	YEROTH_CLASS_OPERATORS

    YerothERPClientsWindow();

    virtual ~YerothERPClientsWindow();

    inline bool isCurrentlyFiltered()
    {
    	return _currentlyFiltered;
    }

    void setCurrentlyFiltered(bool currentlyFiltered);

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
							 QObject::trUtf8("Sélectionner un stock dans le tableau des stocks, ensuite cliquer "
											 "sur l'opération que vous souhaitez réaliser !"));
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

	virtual bool imprimer_document();

	inline void setSearchFormSqlTableModel(YerothSqlTableModel *searchFormSqlTableModel)
    {
    	_searchClientsTableModel = searchFormSqlTableModel;
    }

    inline int getLastListerSelectedRow()
    {
    	return tableView_clients->lastSelectedRow();
    }

    inline void setLastListerSelectedRow(int row)
    {
    	tableView_clients->setLastSelectedRow(row);
    }

    virtual void afficherClients(YerothSqlTableModel &clientSqlTableModel);

    void afficherClients();

    void afficher_nom_entreprise_selectioner(const QString &nomEntreprise);

protected slots:

    virtual void slot_reinitialiser_champs_db_visibles();

protected:

    virtual void reinitialiser_champs_db_visibles();

    void contextMenuEvent(QContextMenuEvent *event);

    virtual void hideEvent(QHideEvent * hideEvent);

    virtual void setupShortcuts();

private slots:

	void private_slot_afficher_les_transactions_dun_client();

	void modifierCompteClient();

	void supprimerCompteClient();

	void afficher_au_detail();

	void afficher_au_detail(const QModelIndex & modelIndex);

	void updateLineEditRechercheNomEntreprise();

	bool createHistoryPaymentForCustomerAccount(HistoryPaymentInfo &paymentInfo);

  	bool putCashIntoCustomerAccount();

	bool filtrer();

    inline void rechercher()
    {
    	_searchClientsWidget->rendreVisible();
    }

    void reinitialiser_elements_filtrage();

    void reinitialiser_recherche();

	void set_rechercher_font();

private:

	void set_filtrer_font();

    void populateClientsComboBoxes();

    void setupLineEdits();

    void setupLineEditsQCompleters();

    static const QString 	_WINDOW_TITLE;

    YerothLogger			*_logger;

    bool					_currentlyFiltered;

    int 					_lastSelectedRow;

    QFont 					*_pushButton_filtrer_font;

    QFont 					*_action_RechercherFont;

    YerothSearchForm 		*_searchClientsWidget;

    YerothSqlTableModel 	*_curClientsTableModel;

    YerothSqlTableModel 	*_searchClientsTableModel;
};

#endif /* SRC_WINDOWS_YEROTH_ERP_CLIENTS_WINDOW_HPP_ */
