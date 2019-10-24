/*
 * yeroth-erp-admin-windows-commons.hpp
 *
 *  Created on: Mar 6, 2018
 *      Author: Dipl.-Inf. Xavier NOUMBISSI NOUNDOU, Ph.D. (ABD)
 *      Email:  xnoundou7@gmail.com
 */

#ifndef SRC_YEROTH_ERP_ADMIN_WINDOWS_COMMONS_HPP_
#define SRC_YEROTH_ERP_ADMIN_WINDOWS_COMMONS_HPP_

#include "src/include/yeroth-erp-3-0-software.text-configuration.hpp"

#include "src/users/yeroth-erp-users.hpp"

#include "src/utils/yeroth-erp-logger.hpp"

#include "src/widgets/yeroth-erp-qmessage-box.hpp"

#include "src/utils/yeroth-erp-utils.hpp"

#include <QtWidgets/QAction>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMessageBox>

class YerothERPWindows;

class YerothPOSAdminWindowsCommons : public QMainWindow
{
    Q_OBJECT

public:

	YEROTH_CLASS_OPERATORS

	inline YerothPOSAdminWindowsCommons(QString windowName)
	:_curStocksTableModel(0),
	 _windowName(windowName)
	{
	}

	inline ~YerothPOSAdminWindowsCommons()
	{
	}

	inline virtual QString qMessageBoxStyleSheet()
	{
		return QMESSAGE_BOX_STYLE_SHEET;
	}

	inline QString getWindowName()
	{
		return _windowName;
	}

	inline virtual QToolBar * getQMainWindowToolBar()
	{
		return 0;
	}

	inline virtual void YEROTH_ERP_ADMIN_WRAPPER_QACTION_SET_ENABLED(QAction *anAction,
													   	   	  	    bool aBooleanValue)
	{
		anAction->setVisible(aBooleanValue);
	}

	inline static void setCenterPosition(QPoint *desktopCenterPosition)
	{
		_centerPosition = desktopCenterPosition;
	}

	inline static void setAllWindows(YerothERPWindows *allWindows)
	{
		_allWindows = allWindows;
	}

    virtual void definirCaissier(){}

    virtual void definirManager(){}

    virtual void definirVendeur(){}

    virtual void definirGestionaireDesStocks(){}

    virtual void definirMagasinier(){}

    virtual void definirPasDeRole(){}

    virtual void rendreVisible(YerothSqlTableModel *stocksTableModel);

	inline virtual void rendreInvisible()
	{
		this->setVisible(false);
	}

	inline static YerothERPWindows *getAllWindows()
	{
		return _allWindows;
	}

public slots:

	virtual void menu();

	virtual void retour_menu_principal();

	inline virtual bool imprimer_document()
	{
		return false;
	}

    virtual void changer_utilisateur();

    virtual void deconnecter_utilisateur();

    inline virtual YerothSqlTableModel *getStocksTableModel()
    {
    	return _curStocksTableModel;
    }

    inline virtual void fermeture()
    {
    	this->close();
    }

    inline virtual void apropos()
    {
    	YerothQMessageBox::about(this, YerothUtils::APPLICATION_NAME, YerothUtils::APROPOS_MSG);
    }

	virtual void infosEntreprise();

	virtual void qui_suis_je();

    inline virtual void help(){}

	void getManuelUtilisateurPDF();

protected:

    void mySetupUi(QMainWindow *aWindow);

    static YerothERPWindows		*_allWindows;

    YerothSqlTableModel 		*_curStocksTableModel;

    QString 				QMESSAGE_BOX_STYLE_SHEET;

    const QString 			_windowName;

    static QPoint			*_centerPosition;
};

#endif /* SRC_YEROTH_ERP_ADMIN_WINDOWS_COMMONS_HPP_ */
