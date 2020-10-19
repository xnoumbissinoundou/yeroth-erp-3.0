/*
 * yeroth-erp-payer-fournisseur-window.hpp
 *
 *      Author: DIPL.-INF. XAVIER NOUMBISSI NOUNDOU
 */

#ifndef SRC_WINDOWS_YEROTH_PAYER_FOURNISSEUR_WINDOW_HPP_
#define SRC_WINDOWS_YEROTH_PAYER_FOURNISSEUR_WINDOW_HPP_

#include "../../ui_yeroth-erp-payer-fournisseur-window.h"

#include "yeroth-erp-window-commons.hpp"


#include <QtCore/QDebug>

#include <QtWidgets/QMessageBox>

#include <QtGui/QContextMenuEvent>


class QStandardItemModel;
class QContextMenuEvent;
class QProcess;

class YerothSqlTableModel;



class YerothPayerFournisseurWindow : public YerothWindowsCommons,
									  private Ui_YerothPayerFournisseurWindow
{
    Q_OBJECT

	class PaymentInfo
	{
	public:

		YEROTH_CLASS_OPERATORS

		inline PaymentInfo()
		:type_de_paiement(0),
		 fournisseur(0.0),
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

		double fournisseur;
		double montant_paye;
	};

public:

	YEROTH_CLASS_OPERATORS

    YerothPayerFournisseurWindow();

	inline virtual ~YerothPayerFournisseurWindow()
	{
	}

    inline virtual QToolBar * getQMainWindowToolBar()
    {
    	return toolBar_payer_fournisseurWindow;
    }

    virtual void definirCaissier();

    virtual void definirManager();

    virtual void definirVendeur();

    virtual void definirGestionaireDesStocks();

    virtual void definirMagasinier();

    virtual void definirPasDeRole();

    void updateLineEdits();

    void rendreVisible(int lastSelectedRow__ID,
    				   YerothSqlTableModel *fournisseurTableModel,
					   YerothSqlTableModel *stocksTableModel);

    virtual void rendreInvisible();

public slots:

	inline virtual void apropos()
	{
		YerothWindowsCommons::apropos(this);
	}

	static void getPayerAuFournisseurListingTexTableString(QString &texTable_in_out,
	        								   	   	   	    QStandardItemModel &tableStandardItemModel,
															QList<int> &dbFieldNameOfTypeString,
															QList<int> &columnsToIgnore,
															int fromRowIndex,
															int toRowIndex,
															bool lastPage);

	static void getPayerAuFournisseurTexDocumentString(QString &texDocumentString_in_out,
			   	   	   	   	   	   	   	   	   	   	    QString &printString);

protected:

	virtual void hideEvent(QHideEvent * hideEvent);

protected slots:

	void handleComboBoxFournisseurs_Typedepaiement_TextChanged(const QString &currentText);

private slots:

	void handleReferenceChange(const QString &referenceEngagement);

	void afficher_detail_fournisseur();

	void updateStocksVeduTable(PaymentInfo &paymentInfo);

	bool createPaymentForCustomerAccount(PaymentInfo &paymentInfo);

	void reinitialiser_donnees_de_paiement_au_comptefournisseur();

	void reinitialiser_donnees_de_consultation_des_transactions_financieres();

	void private_slot_afficher_les_transactions_dun_fournisseur();

  	bool putCashIntoCustomerAccount();

private:

    void setupLineEdits();

    void setupLineEditsQCompleters();

    void setupDateTimeEdits();

  	void populatePayerAuFournisseursComboBoxes();


    YerothSqlTableModel 	*_curFournisseurTableModel;

    QString					_curCompanyName;

    double					_curReferenceEngagementResteAPayer;

    int 					_fournisseurLastSelectedRow;
};

#endif /* SRC_WINDOWS_YEROTH_PAYER_FOURNISSEUR_WINDOW_HPP_ */
