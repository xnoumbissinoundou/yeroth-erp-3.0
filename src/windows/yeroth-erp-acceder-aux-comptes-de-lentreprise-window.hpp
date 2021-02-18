/*
 * yeroth-erp-acceder-aux-comptes-de-lentreprise-window.hpp
 *
 *      Author: DR. XAVIER NOUMBISSI NOUNDOU
 */

#ifndef SRC_YEROTH_ACCEDER_AUX_COMPTES_DE_LENTREPRISE_WINDOW_HPP_
#define SRC_YEROTH_ACCEDER_AUX_COMPTES_DE_LENTREPRISE_WINDOW_HPP_

#include "../../ui_yeroth-erp-acceder-aux-comptes-de-lentreprise-window.h"

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

class YerothAccederAuxComptesDeLentrepriseWindow : public YerothWindowsCommons,
												   public Ui_YerothAccederAuxComptesDeLentrepriseWindow
{
    Q_OBJECT

public:

	YEROTH_CLASS_OPERATORS

    YerothAccederAuxComptesDeLentrepriseWindow();

	inline ~YerothAccederAuxComptesDeLentrepriseWindow()
	{
	    delete _logger;
	}

    inline virtual QToolBar * getQMainWindowToolBar()
    {
    	return toolBar_acceder_aux_comptes_de_lentrepriseWindow;
    }

    inline virtual void definirCaissier()
    {
    	definirPasDeRole();
    }

    virtual void definirManager();

    inline virtual void definirVendeur()
    {
    	definirPasDeRole();
    }

    inline virtual void definirGestionaireDesStocks()
    {
    	definirPasDeRole();
    }

    inline virtual void definirAdministrateur()
    {
    	definirPasDeRole();
    }

    inline virtual void definirMagasinier()
    {
    	definirPasDeRole();
    }

    virtual void definirPasDeRole();

    virtual void rendreVisible(YerothSqlTableModel *stocksTableModel);

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

    YerothLogger			*_logger;

    YerothSqlTableModel 	*_curComptesBancairesSqlTableModel;
};

#endif /* SRC_YEROTH_ACCEDER_AUX_COMPTES_DE_LENTREPRISE_WINDOW_HPP_ */
