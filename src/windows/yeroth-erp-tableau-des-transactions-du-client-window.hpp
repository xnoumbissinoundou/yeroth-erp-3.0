/*
 * yeroth-erp-pointdevente-liststocks-window.hpp
 *
 *      Author: Xavier NOUMBISSI NOUNDOU, Dipl.-Inf., Ph.D. (ABD)
 */

#ifndef SRC_YEROTH_COMPTES_CLIENTS_TRANSACTIONS_DUN_CLIENT_WINDOW_HPP_
#define SRC_YEROTH_COMPTES_CLIENTS_TRANSACTIONS_DUN_CLIENT_WINDOW_HPP_

#include "../../ui_yeroth-erp-tableau-des-transactions-du-client-window.h"

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

	virtual bool imprimer_pdf_document();

private:


    QDate 					_curDateDebutTransactions;

	QDate 					_curDateFinTransactions;

    QString					_clientCompanyName;
};

#endif /* SRC_YEROTH_COMPTES_CLIENTS_TRANSACTIONS_DUN_CLIENT_WINDOW_HPP_ */
