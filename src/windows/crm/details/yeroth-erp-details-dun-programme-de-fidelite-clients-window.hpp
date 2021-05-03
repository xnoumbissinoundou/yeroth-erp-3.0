/*
 * yeroth-erp-details-dun-programme-de-fidelite-clients-window.hpp
 *
 *      Author: PR. XAVIER NOUMBISSI NOUNDOU
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

	inline virtual void definirCaissier()
	{
	    definirPasDeRole();
	}

	virtual void definirManager();

	inline virtual void definirVendeur()
	{
		definirManager();
	}

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

    virtual void rendreVisible(YerothSqlTableModel *curClient_LOYALTY_PROGRAM_TableModel,
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

    void showClientLOYALTY_PROGRAM_DETAIL();


    YerothLogger			*_logger;

    YerothSqlTableModel 	*_curClient_LOYALTY_PROGRAM_TableModel;
};


#endif /* SRC_WINDOWS_YEROTH_CLIENTS_DETAILS_DUN_PROGRAMME_DE_FIDELITE_CLIENTS_WINDOW_HPP_ */
