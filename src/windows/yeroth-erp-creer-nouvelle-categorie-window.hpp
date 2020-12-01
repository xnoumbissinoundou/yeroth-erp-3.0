/*
 * yeroth-erp-creer-nouvelle-categorie-window.hpp
 *
 *      Author: DR. XAVIER NOUMBISSI NOUNDOU
 */

#ifndef SRC_YEROTH_NOUVELLE_CATEGORIE_WINDOW_HPP_
#define SRC_YEROTH_NOUVELLE_CATEGORIE_WINDOW_HPP_

# include "../../ui_yeroth-erp-creer-nouvelle-categorie-window.h"

# include <QtWidgets/QMessageBox>
# include <QtCore/QDebug>
#include "yeroth-erp-window-commons.hpp"

class YerothERPWindows;
class QStandardItemModel;
class YerothUtils;
class YerothLogger;
class YerothSqlTableModel;

class YerothCreerNouvelleCategorieWindow : public YerothWindowsCommons,
										   private Ui_YerothCreerNouvelleCategorieWindow
{
    Q_OBJECT

public:

	YEROTH_CLASS_OPERATORS

	YerothCreerNouvelleCategorieWindow();

	inline ~YerothCreerNouvelleCategorieWindow()
	{
		delete _logger;
	}

	inline virtual QToolBar * getQMainWindowToolBar()
	{
		return toolBar_createNouvelleCategorieWindow;
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

	virtual void alertes();

	virtual void ventes();

    virtual void menu();

    virtual void administration();

    void annuler();

    void valider();

    inline virtual void apropos()
	{
		YerothWindowsCommons::apropos(this);
	}

    inline virtual void help()
    {
		YerothQMessageBox::information(this,
						 QObject::trUtf8("aide"),
						 QObject::trUtf8("Entrer le nom de la nouvelle catégorie et valider, "
			    						 "ou bien annuler l'opération en cliquant sur le boutton annuler!"));
    }

protected:

    bool creer_categorie();

    bool creer_categorie_check_fields();

    void clear_categorie_all_fields();

    virtual void setupShortcuts();

private:

    YerothLogger				*_logger;
};

#endif /* SRC_YEROTH_NOUVELLE_CATEGORIE_WINDOW_HPP_ */
