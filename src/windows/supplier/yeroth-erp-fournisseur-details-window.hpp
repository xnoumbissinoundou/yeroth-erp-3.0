/*
 * yeroth-erp-fournisseur-details-window.hpp
 *
 *      Author: PR. XAVIER NOUMBISSI NOUNDOU
 */

#ifndef SRC_WINDOWS_YEROTH_FOURNISSEUR_DETAILS_WINDOW_HPP_
#define SRC_WINDOWS_YEROTH_FOURNISSEUR_DETAILS_WINDOW_HPP_

#include "../../../ui_yeroth-erp-fournisseur-details-window.h"


#include "src/utils/yeroth-erp-logger.hpp"

#include "src/windows/yeroth-erp-window-commons.hpp"


#include <QtWidgets/QMessageBox>


class QContextMenuEvent;

class YerothERPWindows;
class YerothSqlTableModel;
class YerothLogger;

class YerothFournisseurDetailsWindow : public YerothWindowsCommons, private Ui_YerothFournisseurDetailsWindow
{
    Q_OBJECT

public:

	YEROTH_CLASS_OPERATORS

	YerothFournisseurDetailsWindow();

	inline ~YerothFournisseurDetailsWindow()
	{
		delete _logger;
	}

	inline QToolBar * getQMainWindowToolBar()
	{
		return toolBar_fournisseurDetailsWindow;
	}

    inline virtual void definirCaissier()
    {
    	definirPasDeRole();
    }

    void definirManager();

    inline virtual void definirVendeur()
    {
    	definirPasDeRole();
    }

    inline virtual void definirGestionaireDesStocks()
    {
    	definirPasDeRole();
    }

    inline virtual void definirMagasinier()
    {
    	definirPasDeRole();
    }

    void definirPasDeRole();

    void rendreInvisible();

    void rendreVisible(YerothSqlTableModel *fournisseurTableModel,
					   YerothSqlTableModel *stocksTableModel);

public slots:

	virtual bool imprimer_pdf_document();

	inline virtual void apropos()
	{
		YerothWindowsCommons::apropos(this);
	}

	inline virtual void help()
	{
		YerothQMessageBox::information(this,
						 QObject::trUtf8("aide"),
						 QObject::trUtf8("Vous pouvez modifier les détails de ce fournisseur"
										 " en cliquant sur 'Modifier' !"));
	}

protected:

    virtual void setupShortcuts();

private slots:

	void modifierFournisseur();

	void supprimerFournisseur();

private:

    void setupLineEdits();

    void showFournisseurDetail();

    void checkCourrierAlerts();


    YerothLogger			*_logger;

    YerothSqlTableModel 	*_curFournisseurTableModel;
};


#endif /* SRC_WINDOWS_YEROTH_FOURNISSEUR_DETAILS_WINDOW_HPP_ */
