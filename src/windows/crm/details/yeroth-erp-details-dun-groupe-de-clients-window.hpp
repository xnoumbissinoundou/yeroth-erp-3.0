/*
 * yeroth-erp-details-dun-groupe-de-clients-window.hpp
 *
 *      Author: PR. XAVIER NOUMBISSI NOUNDOU
 */

#ifndef YEROTH_CLIENTS_DETAILS_DUN_GROUPE_DE_CLIENTS_WINDOW_HPP_
#define YEROTH_CLIENTS_DETAILS_DUN_GROUPE_DE_CLIENTS_WINDOW_HPP_

#include "../../../../ui_yeroth-erp-details-dun-groupe-de-clients-window.h"

#include "src/utils/yeroth-erp-logger.hpp"

#include "src/windows/yeroth-erp-window-commons.hpp"

#include <QtWidgets/QMessageBox>


class QContextMenuEvent;

class YerothERPWindows;
class YerothSqlTableModel;
class YerothLogger;

class YerothDetailsDunGroupeDeClientsWindow : public YerothWindowsCommons,
								  	  	  	  private Ui_YerothDetailsDunGroupeDeClientsWindow
{
    Q_OBJECT

public:

	YEROTH_CLASS_OPERATORS

	YerothDetailsDunGroupeDeClientsWindow();

	inline ~YerothDetailsDunGroupeDeClientsWindow()
	{
		delete _logger;
	}

	inline QToolBar * getQMainWindowToolBar()
	{
		return toolBar_detailsDunGroupeDeClientsWindow;
	}

	virtual void definirCaissier();

	virtual void definirManager();

    virtual void definirVendeur();

    virtual void definirGestionaireDesStocks();

    virtual void definirMagasinier();

    virtual void definirPasDeRole();

    virtual void rendreInvisible();

    virtual void rendreVisible(YerothSqlTableModel *aClientTableModel,
    						   YerothSqlTableModel *aClientGroupTableModel,
					   	   	   YerothSqlTableModel *stocksTableModel,
							   const QString &clientGroup_db_ID = YerothUtils::EMPTY_STRING);

    virtual void rendreVisible(YerothSqlTableModel *clientTableModel,
    						   YerothSqlTableModel *stocksTableModel,
							   const QString &clientGroup_db_ID = YerothUtils::EMPTY_STRING);

public slots:

	inline virtual void apropos()
	{
		YerothWindowsCommons::apropos(this);
	}

	inline virtual void help()
	{
		YerothQMessageBox::information(this,
						 QObject::trUtf8("aide"),
						 QObject::trUtf8("Vous pouvez modifier les détails de ce programme de fidélité clients "
										 "en cliquant sur 'Modifier' !"));
	}

protected:

    virtual void setupShortcuts();

private slots:

	void afficher_groupes_dun_client();

	void modifier_un_groupe_de_clients();

private:

    void setupLineEdits();

    void showClientGroup_DETAIL(const QString &clientGroup_db_ID = YerothUtils::EMPTY_STRING);


    YerothLogger			*_logger;

    YerothSqlTableModel 	*_curClientTableModel;

    YerothSqlTableModel 	*_curClientGroupTableModel;
};


#endif /* YEROTH_CLIENTS_DETAILS_DUN_GROUPE_DE_CLIENTS_WINDOW_HPP_ */
