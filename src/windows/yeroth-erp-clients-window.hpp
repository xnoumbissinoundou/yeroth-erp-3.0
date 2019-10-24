/*
 * yeroth-erp-marchandises-window.hpp
 *
 *      Author: Dipl.-Inf. Xavier NOUMBISSI NOUNDOU, Ph.D. (ABD)
 */

#ifndef SRC_WINDOWS_YEROTH_ERP_CLIENTS_WINDOW_HPP_
#define SRC_WINDOWS_YEROTH_ERP_CLIENTS_WINDOW_HPP_

#include "../../ui_yeroth-erp-clients-window.h"

#include <QtCore/QDebug>

#include <QtWidgets/QMessageBox>

#include <QtGui/QContextMenuEvent>

#include "yeroth-erp-window-commons.hpp"


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
	QString nom_payeur;
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

	virtual bool imprimer_document();

    virtual void afficherClients(YerothSqlTableModel &clientSqlTableModel);

    void afficherClients();

    void afficher_nom_entreprise_selectioner(const QString &nomEntreprise);

    void reinitialiser_elements_filtrage();

    inline int getLastListerSelectedRow()
    {
    	return tableView_clients->lastSelectedRow();
    }

    inline void setLastListerSelectedRow(int row)
    {
    	tableView_clients->setLastSelectedRow(row);
    }

protected:

    virtual void setupShortcuts();

private slots:

	void updateLineEditRechercheNomEntreprise();

	bool createHistoryPaymentForCustomerAccount(HistoryPaymentInfo &paymentInfo);

  	bool putCashIntoCustomerAccount();

	bool filtrer();

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

    YerothSqlTableModel 	*_clientStockTableModel;
};

#endif /* SRC_WINDOWS_YEROTH_ERP_CLIENTS_WINDOW_HPP_ */
