/*
 * yeroth-erp-creer-nouveau-fournisseur-window.hpp
 *
 *      Author: Dipl.-Inf. XAVIER NOUMBISSI NOUNDOU
 */

# ifndef SRC_YEROTH_NOUVEAU_FOURNISSEUR_WINDOW_HPP_
# define SRC_YEROTH_NOUVEAU_FOURNISSEUR_WINDOW_HPP_

# include "../../ui_yeroth-erp-creer-nouveau-fournisseur-window.h"

# include <QtWidgets/QMessageBox>
# include <QtCore/QDebug>
#include "yeroth-erp-window-commons.hpp"

class YerothERPWindows;
class QStandardItemModel;
class YerothUtils;
class YerothLogger;
class YerothSqlTableModel;

class YerothCreerNouveauFournisseurWindow : public YerothWindowsCommons,
											private Ui_YerothCreerNouveauFournisseurWindow
{
    Q_OBJECT

public:

	YEROTH_CLASS_OPERATORS

    YerothCreerNouveauFournisseurWindow();

    inline ~YerothCreerNouveauFournisseurWindow(){}

    inline QToolBar * getQMainWindowToolBar()
    {
    	return toolBar_createNouveauFournisseurWindow;
    }

    virtual void definirCaissier();

    virtual void definirManager();

    virtual void definirVendeur();

    virtual void definirGestionaireDesStocks();

    virtual void definirMagasinier();

    virtual void definirPasDeRole();

    virtual void rendreVisible(YerothSqlTableModel *stocksTableModel);

    virtual void rendreInvisible();

public slots:

	inline virtual void apropos()
	{
		YerothWindowsCommons::apropos(this);
	}

	inline virtual void help()
	{
		YerothQMessageBox::information(this,
						 QObject::trUtf8("aide"),
						 QObject::trUtf8("Entrer le nom du nouveau fournisseur et valider, "
										 "ou bien annuler l'opération en cliquant sur le boutton annuler!"));
	}

	virtual void alertes();

	virtual void ventes();

    virtual void menu();

    virtual void administration();

    void annuler();

    void valider();

protected:

    bool creer_fournisseur();

    bool creer_fournisseur_check_fields();

    void clear_fournisseur_all_fields();

    virtual void setupShortcuts();

private:

    YerothLogger				*_logger;
};

#endif /* SRC_YEROTH_NOUVEAU_FOURNISSEUR_WINDOW_HPP_ */
