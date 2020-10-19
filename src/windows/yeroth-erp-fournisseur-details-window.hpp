/*
 * yeroth-erp-fournisseur-details-window.hpp
 *
 *      Author: DIPL.-INF. XAVIER NOUMBISSI NOUNDOU
 */

#ifndef SRC_WINDOWS_YEROTH_FOURNISSEUR_DETAILS_WINDOW_HPP_
#define SRC_WINDOWS_YEROTH_FOURNISSEUR_DETAILS_WINDOW_HPP_

#include "../../ui_yeroth-erp-fournisseur-details-window.h"

#include "src/utils/yeroth-erp-logger.hpp"

#include "yeroth-erp-window-commons.hpp"

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

    void definirCaissier();

    void definirManager();

    virtual void definirVendeur();

    virtual void definirGestionaireDesStocks();

    void definirMagasinier();

    void definirPasDeRole();

    void rendreInvisible();

    void rendreVisible(int lastSelectedRow__ID,
    				   YerothSqlTableModel *fournisseurTableModel,
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

	void private_payer_au_fournisseur();

	void modifierFournisseur();

	void supprimerFournisseur();

private:

    void setupLineEdits();

    void showFournisseurDetail(int lastSelectedRow__ID);

    void checkCourrierAlerts();


    YerothLogger			*_logger;

    YerothSqlTableModel 	*_curFournisseurTableModel;

    int 					_fournisseurLastSelectedRow;
};


#endif /* SRC_WINDOWS_YEROTH_FOURNISSEUR_DETAILS_WINDOW_HPP_ */
