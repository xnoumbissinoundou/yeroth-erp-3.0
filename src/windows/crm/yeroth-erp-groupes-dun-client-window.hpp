/*
 * yeroth-erp-groupes-dun-client-window.hpp
 *
 *      Author: DR. XAVIER NOUMBISSI NOUNDOU
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

    virtual void definirCaissier();

    virtual void definirManager();

    virtual void definirVendeur();

    virtual void definirGestionaireDesStocks();

    virtual void definirMagasinier();

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

	bool export_csv_file();

	virtual bool imprimer_pdf_document();

protected:

    virtual void contextMenuEvent(QContextMenuEvent *event);

    virtual void setupShortcuts();

protected slots:

	inline virtual void disableExporterAuFormatCsv()
	{
		actionExporter_au_format_csv->setVisible(false);
	}

	inline virtual void enableExporterAuFormatCsv()
	{
		actionExporter_au_format_csv->setVisible(true);
	}

	inline virtual void disableImprimer()
	{
		actionAfficherPDF->setVisible(false);
	}

	inline virtual void enableImprimer()
	{
		actionAfficherPDF->setVisible(true);
	}

private slots:

	void supprimer_appartenance();

	void afficher_au_detail(const QModelIndex &modelIndex);

	inline void afficher_au_detail()
	{
		afficher_au_detail(tableWidget_groupes_dun_client->currentIndex());
	}

	void afficher_tous_les_groupes_du_client();

private:

	void setupLineEdits();

    void setupLineEditsQCompleters();


    YerothLogger			*_logger;

    YerothSqlTableModel 	*_curClientsTableModel;

    int						_curClientDBID;
};

#endif /* SRC_WINDOWS_YEROTH_ERP_GROUPE_DUN_CLIENT_WINDOW_HPP_ */
