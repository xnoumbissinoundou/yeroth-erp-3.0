/*
 * yeroth-erp-payer-compte-client-window.hpp
 *
 *      Author: Xavier NOUMBISSI NOUNDOU, Dipl.-Inf., Ph.D. (ABD)
 */

#ifndef SRC_WINDOWS_YEROTH_PAYER_COMPTE_CLIENT_WINDOW_HPP_
#define SRC_WINDOWS_YEROTH_PAYER_COMPTE_CLIENT_WINDOW_HPP_

#include "../../ui_yeroth-erp-payer-compte-client-window.h"

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
	QString notes;
	QString engagement;
	QDate date_paiement;

	double compte_client;
	double montant_paye;
};

class YerothPayerCompteClientWindow : public YerothWindowsCommons, private Ui_YerothPayerCompteClientWindow
{
    Q_OBJECT

public:

	YEROTH_CLASS_OPERATORS

    YerothPayerCompteClientWindow();

    virtual ~YerothPayerCompteClientWindow();

    inline virtual QToolBar * getQMainWindowToolBar()
    {
    	return toolBar_payer_compte_clientWindow;
    }

    virtual void definirCaissier();

    virtual void definirManager();

    virtual void definirVendeur();

    virtual void definirGestionaireDesStocks();

    virtual void definirMagasinier();

    virtual void definirPasDeRole();

    void updateLineEdits();

    void rendreVisible(int lastSelectedRow,
    				   YerothSqlTableModel *clientTableModel,
					   YerothSqlTableModel *stocksTableModel);

    virtual void rendreInvisible();

public slots:

	inline virtual void apropos()
	{
		YerothWindowsCommons::apropos(this);
	}

	static void getPayerAuCompteClientListingTexTableString(QString &texTable_in_out,
	        								   	   	   	    QStandardItemModel &tableStandardItemModel,
															QList<int> &dbFieldNameOfTypeString,
															QList<int> &columnsToIgnore,
															int fromRowIndex,
															int toRowIndex,
															bool lastPage);

	static void getPayerAuCompteClientTexDocumentString(QString &texDocumentString_in_out,
			   	   	   	   	   	   	   	   	   	   	    QString &printString);
private slots:

	void afficher_detail_client();

	bool createHistoryPaymentForCustomerAccount(HistoryPaymentInfo &paymentInfo);

	inline void annuler_paiement_au_compteclient()
	{
		clients();
	}

  	bool putCashIntoCustomerAccount();

private:

  	void populatePayerAuCompteClientsComboBoxes();

    void setupLineEdits();

    void setupLineEditsQCompleters();

    static const QString 	_WINDOW_TITLE;

    YerothSqlTableModel 	*_curClientTableModel;

    QString					_curCompanyName;

    int 					_clientLastSelectedRow;
};

#endif /* SRC_WINDOWS_YEROTH_PAYER_COMPTE_CLIENT_WINDOW_HPP_ */
