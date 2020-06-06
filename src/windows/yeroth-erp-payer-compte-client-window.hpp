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



class YerothPayerCompteClientWindow : public YerothWindowsCommons,
									  private Ui_YerothPayerCompteClientWindow
{
    Q_OBJECT

	class PaymentInfo
	{
	public:

		YEROTH_CLASS_OPERATORS

		inline PaymentInfo()
		:type_de_paiement(0),
		 compte_client(0.0),
		 montant_paye(0.0)
		{
		}

		inline ~PaymentInfo()
		{
		}

		QString intitule_du_compte_bancaire;
		QString nom_entreprise;
		QString nom_encaisseur;
		int type_de_paiement;
		QString notes;
		QString reference;
		QDate date_paiement;

		double compte_client;
		double montant_paye;
	};

public:

	YEROTH_CLASS_OPERATORS

    YerothPayerCompteClientWindow();

	inline virtual ~YerothPayerCompteClientWindow()
	{
	}

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

protected:

	virtual void hideEvent(QHideEvent * hideEvent);

protected slots:

	void handleComboBoxClients_Typedepaiement_TextChanged(const QString &currentText);

private slots:

	void handleReferenceChange(const QString &referenceEngagement);

	void afficher_detail_client();

	void updateStocksVeduTable(PaymentInfo &paymentInfo);

	bool createPaymentForCustomerAccount(PaymentInfo &paymentInfo);

	void reinitialiser_donnees_de_paiement_au_compteclient();

	void reinitialiser_donnees_de_consultation_des_transactions_financieres();

	void private_slot_afficher_les_transactions_dun_client();

  	bool putCashIntoCustomerAccount();

private:

    void setupLineEdits();

    void setupLineEditsQCompleters();

    void setupDateTimeEdits();

  	void populatePayerAuCompteClientsComboBoxes();


    YerothSqlTableModel 	*_curClientTableModel;

    QString					_curCompanyName;

    double					_curReferenceEngagementResteAPayer;

    int 					_clientLastSelectedRow;
};

#endif /* SRC_WINDOWS_YEROTH_PAYER_COMPTE_CLIENT_WINDOW_HPP_ */
