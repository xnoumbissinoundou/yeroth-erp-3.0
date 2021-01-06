/*
 * yeroth-erp-creer-groupe-de-clients-window.hpp
 *
 *      Author: DR. XAVIER NOUMBISSI NOUNDOU
 */

#ifndef YEROTH_CREER_GROUPE_DE_CLIENTS_WINDOW_HPP_
#define YEROTH_CREER_GROUPE_DE_CLIENTS_WINDOW_HPP_

#include "../../../ui_yeroth-erp-creer-groupe-de-clients-window.h"

# include <QtWidgets/QMessageBox>
#include "src/windows/yeroth-erp-window-commons.hpp"

class YerothERPWindows;
class YerothSqlTableModel;

class YerothCreerGroupeDeClientsWindow : public YerothWindowsCommons,
									     private Ui_YerothCreerGroupeDeClientsWindow
{
    Q_OBJECT

public:

	YEROTH_CLASS_OPERATORS

    YerothCreerGroupeDeClientsWindow();

    inline ~YerothCreerGroupeDeClientsWindow()
    {
    	delete _logger;
    }

    inline QToolBar * getQMainWindowToolBar()
    {
    	return toolBar_creerGroupeDeClientsWindow;
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

protected:

    virtual void setupShortcuts();

private slots:

	inline void annuler_la_creation_dun_groupe_de_clients()
	{
		groupes_de_clients();
	}

	void supprimerUnMembreDunGroupeDeClients();

	bool creerEnregistrerUnGroupeDeClients();

private:

	bool clientGroupAlreadyExists();

	bool check_fields();

    void clear_all_fields();

    void setupLineEdits();

    void setupLineEditsQCompleters();


    YerothLogger				*_logger;
};


#endif /* YEROTH_CREER_GROUPE_DE_CLIENTS_WINDOW_HPP_ */
