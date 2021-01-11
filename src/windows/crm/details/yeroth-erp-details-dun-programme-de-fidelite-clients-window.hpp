/*
 * yeroth-erp-details-dun-programme-de-fidelite-clients-window.hpp
 *
 *      Author: DR. XAVIER NOUMBISSI NOUNDOU
 */

#ifndef SRC_WINDOWS_YEROTH_CLIENTS_DETAILS_DUN_PROGRAMME_DE_FIDELITE_CLIENTS_WINDOW_HPP_
#define SRC_WINDOWS_YEROTH_CLIENTS_DETAILS_DUN_PROGRAMME_DE_FIDELITE_CLIENTS_WINDOW_HPP_

#include "../../../../ui_yeroth-erp-details-dun-programme-de-fidelite-clients-window.h"

#include "src/utils/yeroth-erp-logger.hpp"

#include "src/windows/yeroth-erp-window-commons.hpp"

#include <QtWidgets/QMessageBox>


class QContextMenuEvent;

class YerothERPWindows;
class YerothSqlTableModel;
class YerothLogger;

class YerothDetailsDunProgrammeDeFideliteClientsWindow : public YerothWindowsCommons,
								  	  	  	  	  	  	 private Ui_YerothDetailsDunProgrammeDeFideliteClientsWindow
{
    Q_OBJECT

public:

	YEROTH_CLASS_OPERATORS

	YerothDetailsDunProgrammeDeFideliteClientsWindow();

	inline ~YerothDetailsDunProgrammeDeFideliteClientsWindow()
	{
		delete _logger;
	}

	inline QToolBar * getQMainWindowToolBar()
	{
		return toolBar_YerothDetailsDunProgrammeDeFideliteClientsWindow;
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
						 QObject::trUtf8("Vous pouvez modifier les détails de ce programme de fidélité clients "
										 "en cliquant sur 'Modifier' !"));
	}

protected:

    virtual void setupShortcuts();

private:

    void setupLineEdits();

    void showClientROYALTY_PROGRAM_DETAIL();


    YerothLogger			*_logger;

    YerothSqlTableModel 	*_curClientTableModel;
};


#endif /* SRC_WINDOWS_YEROTH_CLIENTS_DETAILS_DUN_PROGRAMME_DE_FIDELITE_CLIENTS_WINDOW_HPP_ */
