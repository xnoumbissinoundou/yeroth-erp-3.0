/*
 * yeroth-erp-creer-fournisseur-window.hpp
 *
 *      Author: PR. XAVIER NOUMBISSI NOUNDOU
 */

#ifndef YEROTH_CREER_FOURNISSEUR_WINDOW_HPP_
#define YEROTH_CREER_FOURNISSEUR_WINDOW_HPP_

#include "../../../ui_yeroth-erp-creer-fournisseur-window.h"


#include "src/windows/yeroth-erp-window-commons.hpp"


#include <QtWidgets/QMessageBox>


class YerothERPWindows;
class YerothSqlTableModel;


class YerothCreerFournisseurWindow : public YerothWindowsCommons,
									  private Ui_YerothCreerFournisseurWindow
{
    Q_OBJECT

public:

	YEROTH_CLASS_OPERATORS

    YerothCreerFournisseurWindow();

    inline ~YerothCreerFournisseurWindow()
    {
    	delete _logger;
    }

    inline QToolBar * getQMainWindowToolBar()
    {
    	return toolBar_creerFournisseurWindow;
    }

    virtual void definirCaissier();

    virtual void definirManager();

    virtual void definirVendeur();

    virtual void definirGestionaireDesStocks();

    virtual void definirMagasinier();

    virtual void definirPasDeRole();

    virtual void rendreInvisible();

    void rendreVisible(YerothSqlTableModel *stocksTableModel);

public slots:

	inline virtual void apropos()
	{
		YerothWindowsCommons::apropos(this);
	}

	inline virtual void help()
	{
		YerothQMessageBox::information(this,
						 QObject::trUtf8("aide"),
						 QObject::trUtf8("Remplisser le formulaire, et ensuite cliquer "
										 "sur enregistrer !"));
	}

    void deconnecter_utilisateur();

protected slots:

    inline void supprimer_image_fournisseur()
    {
        YerothWindowsCommons::supprimer_image(*label_image_produit);
    }

    inline void selectionner_image_fournisseur()
    {
    	YerothWindowsCommons::selectionner_image(*label_image_produit);
    }

protected:

    virtual void setupShortcuts();

private slots:

	bool creerEnregistrerFournisseur();

private:

	bool supplierAlreadyExist();

	bool check_fields();

    void clear_all_fields();

    void setupLineEdits();

    YerothLogger				*_logger;
};


#endif /* YEROTH_CREER_FOURNISSEUR_WINDOW_HPP_ */
