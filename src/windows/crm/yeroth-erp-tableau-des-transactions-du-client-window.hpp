/*
 * yeroth-erp-tableau-des-transactions-du-client-window.hpp
 *
 *      Author: DR. XAVIER NOUMBISSI NOUNDOU
 */

#ifndef SRC_YEROTH_COMPTES_CLIENTS_TRANSACTIONS_DUN_CLIENT_WINDOW_HPP_
#define SRC_YEROTH_COMPTES_CLIENTS_TRANSACTIONS_DUN_CLIENT_WINDOW_HPP_

#include "../../../ui_yeroth-erp-tableau-des-transactions-du-client-window.h"

#include "src/yeroth-erp-windows.hpp"

#include "src/windows/yeroth-erp-window-commons.hpp"


class YerothTableauDesTransactionsDuClientWindow : public YerothWindowsCommons,
												   private Ui_YerothTableauDesTransactionsDuClientWindow
{
	Q_OBJECT

public:

	YEROTH_CLASS_OPERATORS

	YerothTableauDesTransactionsDuClientWindow();

	inline ~YerothTableauDesTransactionsDuClientWindow(){}

	inline virtual QToolBar * getQMainWindowToolBar()
	{
		return toolBar_tableauDesTransactionDuClient;
	}

	void listerTransactionsDunClient(QDate dateDebutTransactions,
									 QDate dateFinTransactions,
									 QString clientCompanyName,
									 QSqlQuery &sqlClientTransactionsUnionQuery);

public slots:

	bool export_csv_file();

	virtual bool imprimer_pdf_document();

protected:

	inline virtual void resizeEvent(QResizeEvent *event)
	{
		tableView_tableau_des_transactions_du_client->resize(event->size());
	}

private:


    QDate 					_curDateDebutTransactions;

	QDate 					_curDateFinTransactions;

    QString					_clientCompanyName;
};

#endif /* SRC_YEROTH_COMPTES_CLIENTS_TRANSACTIONS_DUN_CLIENT_WINDOW_HPP_ */
