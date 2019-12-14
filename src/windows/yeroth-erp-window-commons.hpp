/*
 * yeroth-erp-windows-commons.hpp
 *
 *      Author: Xavier NOUMBISSI NOUNDOU, Dipl.-Inf., Ph.D. (ABD)
 */

#ifndef SRC_YEROTH_ERP_WINDOWS_COMMONS_HPP_
#define SRC_YEROTH_ERP_WINDOWS_COMMONS_HPP_

#include "src/include/yeroth-erp-3-0-software.text-configuration.hpp"

#include "src/utils/yeroth-erp-logger.hpp"

#include "src/widgets/yeroth-erp-qmessage-box.hpp"

#include "src/utils/yeroth-erp-utils.hpp"

#include <QtWidgets/QAction>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMessageBox>

class YerothERPWindows;

class YerothWindowsCommons : public QMainWindow
{
    Q_OBJECT

public:

	YEROTH_CLASS_OPERATORS

	inline YerothWindowsCommons(QString windowName)
	:_curStocksTableModel(0),
	 _windowName(windowName)
	{
		QMESSAGE_BOX_STYLE_SHEET = QString("QMessageBox {background-color: rgb(%1);}")
									.arg(COLOUR_RGB_STRING_YEROTH_ORANGE_243_162_0);
	}

	inline ~YerothWindowsCommons(){}

	inline virtual void YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(QAction *anAction,
													   	   	  bool aBooleanValue)
	{
		anAction->setVisible(aBooleanValue);
	}

	inline virtual QToolBar * getQMainWindowToolBar()
	{
		return 0;
	}

	inline QString getWindowName()
	{
		return _windowName;
	}

	inline virtual QString qMessageBoxStyleSheet()
	{
		return QMESSAGE_BOX_STYLE_SHEET;
	}

	inline static QPoint * getCenterPosition()
	{
		return _centerPosition;
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

	virtual void getManuelUtilisateurPDF();

	virtual void infosEntreprise();

	virtual void achats();

	virtual void afficherStocks();

	virtual void afficherMarchandises();

	inline virtual void modifier_les_articles()
	{
		this->afficherStocks();
	}

	virtual void alertes();

	virtual void clients();

	inline virtual void courrier()
	{
		this->alertes();
	}

	virtual void sortir();

	virtual void tableaux_de_bords();

	virtual void transactions();

	virtual void entrer();

	virtual void paiements();

	virtual void ventes();

	virtual void vendre();

	virtual void menu();

	virtual void administration();

	virtual void qui_suis_je();

	inline virtual bool imprimer_document()
	{
		return false;
	}

    virtual void changer_utilisateur();

    virtual void deconnecter_utilisateur();

    inline virtual void apropos(YerothWindowsCommons *aCallingWindow)
    {
    	YerothQMessageBox::about(aCallingWindow,
    					   YerothUtils::APPLICATION_NAME,
						   YerothUtils::APROPOS_MSG);
    }

    inline void fermeture()
    {
    	this->close();
    }

    inline virtual void help(){}

protected:

	void mySetupUi(QMainWindow *aWindow);

    virtual void setupShortcuts(){}

    inline void setupShortcutActionMessageDaide(QAction &anAction) const
    {
    	anAction.setShortcut(YerothUtils::MESSAGE_DAIDE_QKEYSEQUENCE);
    }

    inline void setupShortcutActionAfficherPDF(QAction &anAction) const
    {
    	anAction.setShortcut(YerothUtils::IMPRIMER_QKEYSEQUENCE);
    }

    inline void setupShortcutActionQuiSuisJe(QAction &anAction) const
    {
    	anAction.setShortcut(YerothUtils::REINITIALISER_QUI_SUI_JE_QKEYSEQUENCE);
    }

    static YerothERPWindows		*_allWindows;

    YerothSqlTableModel 		*_curStocksTableModel;

    QString 				QMESSAGE_BOX_STYLE_SHEET;

    const QString 			_windowName;

private:

    static QPoint			*_centerPosition;
};

#endif /* SRC_YEROTH_ERP_WINDOWS_COMMONS_HPP_ */
