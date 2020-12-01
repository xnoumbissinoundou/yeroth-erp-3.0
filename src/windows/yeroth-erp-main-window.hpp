/*
 * yeroth-erp-main-window.hpp
 *
 *      Author: DR. XAVIER NOUMBISSI NOUNDOU
 */

#ifndef SRC_YEROTH_MAIN_WINDOW_HPP_
#define SRC_YEROTH_MAIN_WINDOW_HPP_

#include "../../ui_yeroth-erp-main-window.h"

#include "src/include/yeroth-erp-3-0-software.text-configuration.hpp"

#include "yeroth-erp-window-commons.hpp"


#include <QtCore/QDebug>

#include <QtWidgets/QMessageBox>


class YerothERPWindows;
class YerothSqlTableModel;
class YerothLogger;
class YerothUtils;
class YerothWindowsCommons;
class YerothPOSUser;

class YerothMainWindow : public YerothWindowsCommons, public Ui_YerothMainWindow
{
    Q_OBJECT

public:

	YEROTH_CLASS_OPERATORS

    YerothMainWindow();

	inline ~YerothMainWindow()
	{
	    delete _logger;
	}

    inline virtual QToolBar * getQMainWindowToolBar()
    {
    	return toolBar_mainWindow;
    }

    virtual void definirCaissier();

    virtual void definirManager();

    virtual void definirVendeur();

    virtual void definirGestionaireDesStocks();

    virtual void definirAdministrateur();

    virtual void definirMagasinier();

    virtual void definirPasDeRole();

    void setup_YEROTH_ERP_3_0_SERVER_PARAMETERS_DISPLAY();

    void rendreVisibleLocalTOCLASS(bool isDatabaseOpened = false);

public slots:

	inline virtual void apropos()
	{
		YerothWindowsCommons::apropos(this);
	}

	virtual void help();

    virtual void deconnecter_utilisateur();

    void cleanupAllOtherWindows();

protected:

    virtual void setupShortcuts();

private:

    YerothLogger				*_logger;
};

#endif /* SRC_YEROTH_MAIN_WINDOW_HPP_ */
