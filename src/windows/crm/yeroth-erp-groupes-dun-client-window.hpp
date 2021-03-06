/*
 * yeroth-erp-groupes-dun-client-window.hpp
 *
 *      Author: PR. XAVIER NOUMBISSI NOUNDOU
 */

#ifndef SRC_WINDOWS_YEROTH_ERP_GROUPE_DUN_CLIENT_WINDOW_HPP_
#define SRC_WINDOWS_YEROTH_ERP_GROUPE_DUN_CLIENT_WINDOW_HPP_

#include "../../../ui_yeroth-erp-groupes-dun-client-window.h"


#include "src/windows/yeroth-erp-window-commons.hpp"


#include <QtCore/QDebug>

#include <QtWidgets/QMessageBox>

#include <QtGui/QContextMenuEvent>


class YerothSqlTableModel;


class QContextMenuEvent;


class YerothGroupesDunClientWindow : public YerothWindowsCommons,
							   	   	 private Ui_YerothGroupesDunClientWindow
{
    Q_OBJECT

public:

	YEROTH_CLASS_OPERATORS

    YerothGroupesDunClientWindow();

	inline ~YerothGroupesDunClientWindow()
	{
	    delete _logger;
	}

    inline virtual QToolBar * getQMainWindowToolBar()
    {
    	return toolBar_menuGroupesDunClientWindow;
    }

    inline virtual void definirCaissier()
    {
    	definirPasDeRole();
    }

    virtual void definirManager();

    virtual void definirVendeur();

    inline virtual void definirGestionaireDesStocks()
    {
    	definirPasDeRole();
    }

    inline virtual void definirMagasinier()
    {
    	definirPasDeRole();
    }

    virtual void definirPasDeRole();

	virtual void rendreInvisible();

    virtual void rendreVisible(YerothSqlTableModel *clientTableModel,
			  	  	  	  	   YerothSqlTableModel *stocksTableModel);

public slots:

	inline virtual void apropos()
	{
		YerothWindowsCommons::apropos(this);
	}

	inline virtual void help()
	{
		YerothQMessageBox::information(this,
							 QObject::trUtf8("aide"),
							 QObject::trUtf8("Sélectionner un compte client dans le tableau "
											 "des groupes d'1 client, ensuite "
											 "cliquer sur l'opération que vous souhaitez réaliser !"));
	}

protected:

    virtual void contextMenuEvent(QContextMenuEvent *event);

    inline virtual void setupShortcuts()
    {
        setupShortcutActionQuiSuisJe(*actionQui_suis_je);
    }

private slots:

	inline void ajouter_appartenance(const QString &un_groupe_de_clients)
	{
		YerothUtils::
			run_FUNCTION_ROUNDED_WITH_DB_TRANSACTION(
					this,
					&un_groupe_de_clients,
					&YerothGroupesDunClientWindow::executer_ajouter_appartenance);
	}

	void executer_ajouter_appartenance(const QString *un_groupe_de_clients);

	inline void retirer_ce_client_du_groupe_selectionne()
	{
		YerothUtils::
			run_FUNCTION_ROUNDED_WITH_DB_TRANSACTION_WITH_ROLLBACK(
					this,
					&YerothGroupesDunClientWindow::EXECUTER_retirer_ce_client_du_groupe_selectionne);
	}

	bool EXECUTER_retirer_ce_client_du_groupe_selectionne();

	void afficher_au_detail(const QModelIndex &modelIndex);

	inline void afficher_au_detail()
	{
		afficher_au_detail(tableWidget_groupes_dun_client->currentIndex());
	}

	void afficher_tous_les_groupes_du_client();

private slots:

	void enable_yeroth_widgets_ON_POSITIVE_QTABLE_WIDGET_ROW_COUNT();

private:

	void disable_yeroth_widgets();

	void setupLineEdits();

    void setupLineEditsQCompleters();


    YerothLogger			*_logger;

    YerothSqlTableModel 	*_curClientsTableModel;

    QString					_curClient_NOM_ENTREPRISE;

    QString					_curClientDBID;
};

#endif /* SRC_WINDOWS_YEROTH_ERP_GROUPE_DUN_CLIENT_WINDOW_HPP_ */
