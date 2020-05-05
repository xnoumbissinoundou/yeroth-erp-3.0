/*
 * yeroth-erp-pointdevente-liststocks-window..hpp
 *
 *      Author: Xavier NOUMBISSI NOUNDOU, Dipl.-Inf., Ph.D. (ABD)
 */

#ifndef SRC_YEROTH_COMPTES_CLIENTS_TRANSACTIONS_DUN_CLIENT_WINDOW_HPP_
#define SRC_YEROTH_COMPTES_CLIENTS_TRANSACTIONS_DUN_CLIENT_WINDOW_HPP_

#include "../../ui_yeroth-erp-tableau-des-transactions-du-client-window.h"

#include "src/yeroth-erp-windows.hpp"

#include "src/windows/yeroth-erp-window-commons.hpp"



class YerothTableauDesTransactionsDuClientWindow : public YerothWindowsCommons, private Ui_YerothTableauDesTransactionsDuClientWindow
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

	static void getTransactionsDunClientTexTableString(QString &texTable_in_out,
	       									    	   QStandardItemModel &tableStandardItemModel,
													   QList<int> &dbFieldNameOfTypeString,
													   QList<int> &columnsToIgnore,
													   int fromRowIndex,
													   int toRowIndex,
													   bool lastPage);

	static void getTransactionsDunClientTexDocumentString(QString &texDocumentString_in_out,
	        									   	   	  QString &printString);

public slots:

	virtual bool imprimer_pdf_document();

private:

    static const QString 	_WINDOW_TITLE;

    QDate 					_curDateDebutTransactions;

	QDate 					_curDateFinTransactions;

    QString					_clientCompanyName;
};

#endif /* SRC_YEROTH_COMPTES_CLIENTS_TRANSACTIONS_DUN_CLIENT_WINDOW_HPP_ */
