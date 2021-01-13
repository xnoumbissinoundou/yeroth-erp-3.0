/*
 * yeroth-erp-creer-compte-client-window.hpp
 *
 *      Author: DR. XAVIER NOUMBISSI NOUNDOU
 */

# ifndef YEROTH_CREER_COMPTE_CLIENT_WINDOW_HPP_
# define YEROTH_CREER_COMPTE_CLIENT_WINDOW_HPP_

# include "../../../../ui_yeroth-erp-creer-compte-client-window.h"

# include <QtWidgets/QMessageBox>
#include "src/windows/yeroth-erp-window-commons.hpp"

class YerothERPWindows;
class YerothSqlTableModel;

class YerothCreerCompteClientWindow : public YerothWindowsCommons,
									  private Ui_YerothCreerCompteClientWindow
{
    Q_OBJECT

public:

	YEROTH_CLASS_OPERATORS

    YerothCreerCompteClientWindow();

    inline ~YerothCreerCompteClientWindow()
    {
    	delete _logger;
    }

    inline QToolBar * getQMainWindowToolBar()
    {
    	return toolBar_creerCompteClientWindow;
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

    inline void supprimer_image_compte_client()
    {
        YerothWindowsCommons::supprimer_image(*label_image_produit);
    }

    inline void selectionner_image_compte_client()
    {
    	YerothWindowsCommons::selectionner_image(*label_image_produit);
    }

protected:

    virtual void setupShortcuts();

private slots:

	bool creerEnregistrerCompteClient();

private:

	bool customerAccountAlreadyExist();

	bool check_fields();

    void clear_all_fields();

    void setupLineEdits();

    YerothLogger				*_logger;
};


#endif /* YEROTH_CREER_COMPTE_CLIENT_WINDOW_HPP_ */
