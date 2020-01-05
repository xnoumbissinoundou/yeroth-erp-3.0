/*
 * yeroth-erp-clients-detail-window.hpp
 *
 *      Author: Xavier NOUMBISSI NOUNDOU, Dipl.-Inf., Ph.D. (ABD)
 */

#ifndef SRC_WINDOWS_YEROTH_CLIENTS_DETAIL_WINDOW_HPP_
#define SRC_WINDOWS_YEROTH_CLIENTS_DETAIL_WINDOW_HPP_

#include "../../ui_yeroth-erp-clients-detail-window.h"

#include "src/utils/yeroth-erp-logger.hpp"

#include "yeroth-erp-window-commons.hpp"

#include <QtWidgets/QMessageBox>


class QContextMenuEvent;

class YerothERPWindows;
class YerothSqlTableModel;
class YerothLogger;

class YerothClientsDetailWindow : public YerothWindowsCommons, private Ui_YerothClientsDetailWindow
{
    Q_OBJECT

public:

	YEROTH_CLASS_OPERATORS

	YerothClientsDetailWindow();

	inline ~YerothClientsDetailWindow()
	{
		delete _logger;
	}

	inline QToolBar * getQMainWindowToolBar()
	{
		return toolBar_clientsDetailWindow;
	}

    void definirCaissier();

    void definirManager();

    virtual void definirVendeur();

    virtual void definirGestionaireDesStocks();

    void definirMagasinier();

    void definirPasDeRole();

    void rendreInvisible();

    void rendreVisible(int lastSelectedRow,
    				   YerothSqlTableModel *clientTableModel,
					   YerothSqlTableModel *stocksTableModel);

public slots:

	virtual bool imprimer_document();

	inline virtual void apropos()
	{
		YerothWindowsCommons::apropos(this);
	}

	inline virtual void help()
	{
		YerothQMessageBox::information(this,
						 QObject::trUtf8("aide"),
						 QObject::trUtf8("Vous pouvez retourner à la page précédente en "
										 "cliquant sur 'Retour'!"));
	}

protected:

    virtual void setupShortcuts();

private slots:

	void modifierCompteClient();

	void supprimerCompteClient();

private:

    void setupLineEdits();

    void showClientDetail(int lastSelectedRow);

    void checkCourrierAlerts();

	static void getLatexCustomerData(QString &texDocumentString_in_out);

    static const QString 	_WINDOW_TITLE;

    int 					_clientLastSelectedRow;

    YerothSqlTableModel 	*_curClientTableModel;

    YerothLogger			*_logger;

    static const unsigned int INDEX_ZERO;
};


#endif /* SRC_WINDOWS_YEROTH_CLIENTS_DETAIL_WINDOW_HPP_ */
