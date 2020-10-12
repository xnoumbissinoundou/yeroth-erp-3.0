/*
 * yeroth-erp-modifier-fournisseur.hpp
 *
 *      Author: DIPL.-INF. XAVIER NOUMBISSI NOUNDOU
 */

# ifndef SRC_WINDOWS_YEROTH_MODIFIER_FOURNISSEUR_WINDOW_HPP_
# define SRC_WINDOWS_YEROTH_MODIFIER_FOURNISSEUR_WINDOW_HPP_

# include "../../ui_yeroth-erp-modifier-fournisseur-window.h"

# include <QtWidgets/QMessageBox>
#include "yeroth-erp-window-commons.hpp"

class QContextMenuEvent;

class YerothSqlTableModel;

class YerothModifierFournisseurWindow : public YerothWindowsCommons,
										private Ui_YerothModifierFournisseurWindow
{
    Q_OBJECT

public:

	YEROTH_CLASS_OPERATORS

	YerothModifierFournisseurWindow();

    inline ~YerothModifierFournisseurWindow()
    {
    	delete _logger;
    }

    inline virtual QToolBar * getQMainWindowToolBar()
    {
    	return toolBar_modifierFournisseurWindow;
    }

    virtual void definirCaissier();

    virtual void definirManager();

    virtual void definirVendeur();

    virtual void definirGestionaireDesStocks();

    virtual void definirMagasinier();

    virtual void definirPasDeRole();

    virtual void rendreInvisible();

    void rendreVisible(int lastSelectedRow,
    				   YerothSqlTableModel *fournisseurTableModel,
					   YerothSqlTableModel *stocksTableModel);

public slots:

    virtual void deconnecter_utilisateur();

	inline virtual void apropos()
	{
		YerothWindowsCommons::apropos(this);
	}

	inline virtual void help()
	{
		YerothQMessageBox::information(this,
						 QObject::tr("aide"),
						 QObject::tr("Modifier les informations sur le compte fournisseur, "
								     "et ensuite cliquer sur le bouton 'actualiser'!"));
	}

protected:

    void contextMenuEvent(QContextMenuEvent *event);

    virtual void setupShortcuts();

private slots:

	inline void selectionner_image_produit()
	{
		YerothWindowsCommons::selectionner_image(*label_image_produit);
	}

	void supprimer_image_fournisseur();

	bool customerAccountAlreadyExist();

	void actualiserFournisseur();

    void supprimerFournisseur();

private:

    void setupLineEdits();

    bool modifier_fournisseur_check_fields();

    void clear_all_fields();

    void showFournisseurDetail();


    YerothLogger				*_logger;

    YerothSqlTableModel 		*_curFournisseurTableModel;

    int 						_fournisseurLastSelectedRow;

    int							_curFournisseurDetailDBID;
};


#endif /* SRC_WINDOWS_YEROTH_MODIFIER_FOURNISSEUR_WINDOW_HPP_ */
