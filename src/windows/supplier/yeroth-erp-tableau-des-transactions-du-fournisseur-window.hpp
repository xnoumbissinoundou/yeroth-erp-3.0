/*
 * yeroth-erp-tableau-des-transactions-du-fournisseur-window.hpp
 *
 *      Author: DR. XAVIER NOUMBISSI NOUNDOU
 */

#ifndef SRC_YEROTH_COMPTES_CLIENTS_TRANSACTIONS_DUN_FOURNISSEUR_WINDOW_HPP_
#define SRC_YEROTH_COMPTES_CLIENTS_TRANSACTIONS_DUN_FOURNISSEUR_WINDOW_HPP_

#include "../../../ui_yeroth-erp-tableau-des-transactions-du-fournisseur-window.h"


#include "src/windows/yeroth-erp-window-commons.hpp"


class YerothTableauDesTransactionsDuFournisseurWindow : public YerothWindowsCommons,
												   	    private Ui_YerothTableauDesTransactionsDuFournisseurWindow
{
	Q_OBJECT

public:

	YEROTH_CLASS_OPERATORS

	YerothTableauDesTransactionsDuFournisseurWindow();

	inline ~YerothTableauDesTransactionsDuFournisseurWindow(){}

	inline virtual QToolBar * getQMainWindowToolBar()
	{
		return toolBar_tableauDesTransactionDuFournisseur;
	}

	void listerTransactionsDunFournisseur(QDate dateDebutTransactions,
									 	  QDate dateFinTransactions,
										  QString fournisseurCompanyName,
										  QSqlQuery &sqlFournisseurTransactionsUnionQuery);

public slots:

	bool export_csv_file();

	virtual bool imprimer_pdf_document();

protected:

	virtual void setupShortcuts();

	inline void virtual resizeEvent(QResizeEvent *event)
	{
		tableView_tableau_des_transactions_du_fournisseur->resize(event->size());
	}

private:


    QDate 					_curDateDebutTransactions;

	QDate 					_curDateFinTransactions;

    QString					_fournisseurCompanyName;
};

#endif /* SRC_YEROTH_COMPTES_CLIENTS_TRANSACTIONS_DUN_FOURNISSEUR_WINDOW_HPP_ */
