/*
 * yeroth-erp-payer-compte-client-window.hpp
 *
 *      Author: PR. XAVIER NOUMBISSI NOUNDOU
 */

#ifndef SRC_WINDOWS_YEROTH_PAYER_COMPTE_CLIENT_WINDOW_HPP_
#define SRC_WINDOWS_YEROTH_PAYER_COMPTE_CLIENT_WINDOW_HPP_

#include "../../../ui_yeroth-erp-payer-compte-client-window.h"

#include "src/windows/yeroth-erp-window-commons.hpp"


#include <QtCore/QDebug>

#include <QtWidgets/QMessageBox>

#include <QtGui/QContextMenuEvent>


class QStandardItemModel;
class QContextMenuEvent;
class QProcess;


class YerothERPPaymentProcessingInformation;
class YerothSqlTableModel;



class YerothPayerCompteClientWindow : public YerothWindowsCommons,
									  private Ui_YerothPayerCompteClientWindow
{
    Q_OBJECT

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

    void rendreVisible(YerothSqlTableModel *clientTableModel,
					   YerothSqlTableModel *stocksTableModel);

    virtual void rendreInvisible();

public slots:

	inline virtual void apropos()
	{
		YerothWindowsCommons::apropos(this);
	}

protected:

	virtual void hideEvent(QHideEvent * hideEvent);

protected slots:

	void handleComboBoxClients_Typedepaiement_TextChanged(const QString &currentText);

private slots:

	void handleReferenceChange(const QString &referenceEngagement);

	void afficher_detail_client();

	void updateStocksVenduTable(YerothERPPaymentProcessingInformation &payment_processing_info);

	bool createPaymentForCustomerAccount(YerothERPPaymentProcessingInformation &payment_processing_info);

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
};

#endif /* SRC_WINDOWS_YEROTH_PAYER_COMPTE_CLIENT_WINDOW_HPP_ */
