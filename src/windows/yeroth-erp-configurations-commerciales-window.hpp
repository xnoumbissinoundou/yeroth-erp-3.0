/*
 * yeroth-erp-configurations-commerciales-window.hpp
 *
 *      Author: Xavier NOUMBISSI NOUNDOU, Dipl.-Inf., Ph.D. (ABD)
 */

#ifndef SRC_YEROTH_CONFIGURATIONS_COMMERCIALES_WINDOW_HPP_
#define SRC_YEROTH_CONFIGURATIONS_COMMERCIALES_WINDOW_HPP_

#include "../../ui_yeroth-erp-configurations-commerciales-window.h"

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

class YerothConfigurationsCommercialesWindow : public YerothWindowsCommons, public Ui_YerothConfigurationsCommercialesWindow
{
    Q_OBJECT

public:

	YEROTH_CLASS_OPERATORS

    YerothConfigurationsCommercialesWindow();

	inline ~YerothConfigurationsCommercialesWindow()
	{
	    delete _logger;
	}

    inline virtual QToolBar * getQMainWindowToolBar()
    {
    	return toolBar_configurationsCommercialesWindow;
    }

    virtual void definirCaissier();

    virtual void definirManager();

    virtual void definirVendeur();

    virtual void definirGestionaireDesStocks();

    virtual void definirAdministrateur();

    virtual void definirMagasinier();

    virtual void definirPasDeRole();

public slots:

	inline virtual void apropos()
	{
		YerothWindowsCommons::apropos(this);
	}

	inline virtual void help()
	{
		YerothQMessageBox::information(this,
						 QObject::trUtf8("aide"),
						 QObject::trUtf8("1) Vous avez accès aux informations des ventes"
								   	   	 " en utilisant la référence d'un article. Ceci se fait"
								   	   	 " dans le 1er champ de recherche de cette l'interface"
								   	     " graphique !\n\n"
								   	     "2) Vous pouvez filtrer les résultats de la recherche,"
								   	   	 " sur les informations de ventes, en paramétrant"
								 	 	 " celle-ci avec des dates de 'Début', et de 'Fin'.\n\n"
								   	     " Vous pouvez filtrer les résultats de la recherche,"
								 	 	 " en utilisant les paramètres que l'on retrouve"
								   	     " tout en bas de l'interface graphique"
								 	 	 " (exemple: nom du caissier, désignation de l'article, etc.) !"));
	}

    virtual void deconnecter_utilisateur();

protected:

    virtual void setupShortcuts();

private:

    static const QString 	_WINDOW_TITLE;

    YerothLogger				*_logger;
};

#endif /* SRC_YEROTH_CONFIGURATIONS_COMMERCIALES_WINDOW_HPP_ */
