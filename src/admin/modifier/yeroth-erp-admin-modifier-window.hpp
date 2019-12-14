/*
 * yeroth-erp-admin-modifier-window.hpp
 *
 *      Author: Xavier NOUMBISSI NOUNDOU, Dipl.-Inf., Ph.D. (ABD)
 */

# ifndef YEROTH_ADMIN_MODIFIER_WINDOW_HPP_
# define YEROTH_ADMIN_MODIFIER_WINDOW_HPP_

# include "../../../ui_yeroth-erp-admin-modifier-window.h"

# include "src/admin/yeroth-erp-admin-windows-commons.hpp"

# include "../admin-actions-subjects.hpp"

# include <QtWidgets/QMessageBox>
# include <QtSql/QSqlRecord>

class QSqlRecord;

class YerothERPWindows;
class YerothLogger;

class YerothAdminModifierWindow : public YerothPOSAdminWindowsCommons, private Ui_YerothAdminModifierWindow
{
    Q_OBJECT

public:

	YEROTH_CLASS_OPERATORS

    YerothAdminModifierWindow();

    inline ~YerothAdminModifierWindow()
    {
    	delete _logger;
    }

    inline virtual QToolBar * getQMainWindowToolBar()
    {
    	return toolBar_adminModifierWindow;
    }

    virtual void definirPasDeRole();

    virtual void definirAdministrateur();

    virtual void definirManager();

    virtual void rendreVisible(enum AdminSujetAction selectedSujetAction);

    virtual void rendreInvisible();

    void setupEditCompteUtilisateur();

    void setupEditLocalisation();

    void setupEditCategorie();

    void setupEditClient();

    void setupEditFournisseur();

    void setupEditAlerte();

    void setupEditBonDecommande();

public slots:

    void creer();

    void lister();

    void supprimer();

	void modifier_utilisateur();

	void modifier_utilisateur_main();

	void modifier_localisation();

	void modifier_categorie();

	void modifier_alerte();

	void modifier_client();

	void modifier_fournisseur();

    void annuler();

	void radioButtons_quantite();

	void radioButtons_periode_temps();

	inline virtual void help()
	{
		YerothQMessageBox::information(this, QObject::trUtf8("Yeroth-erp-3.0 ~ admin-créer"),
								 QObject::trUtf8("Remplissez les informations nécessaire, et "
										 	 	   "ensuite cliquer sur 'Valider'.\n"
										 	 	   "Cliquez sur 'Annuler' si vous souhaiter annuler "
										 	 	   "l'opération !"),
								 QMessageBox::Ok);
	}

private:

	void enableOtherTabs(enum AdminSujetAction curAction, bool enabled);

	void setupLineEdits();

	void modifier_categorie_main();

	void retourListerCompteUtilisateur();

	void retourListerLocalisation();

	void retourListerCategorie();

	void retourListerClient();

	void retourListerFournisseur();

	void retourListerAlerte();

    void clear_utilisateur_all_fields();

    void clear_categorie_all_fields();

    void clear_localisation_all_fields();

    void clear_client_all_fields();

    void clear_fournisseur_all_fields();

    void clear_alerte_all_fields();

    bool modifier_utilisateur_check_fields();

    bool modifier_categorie_check_fields();

    bool modifier_localisation_check_fields();

    bool modifier_client_check_fields();

    bool modifier_fournisseur_check_fields();

    bool modifier_alerte_check_fields(QString previousAlertText);

    YerothLogger			*_logger;
    YerothSqlTableModel 	*_curSearchSqlTableModel;

    bool				_savePassword;
};

#endif /* YEROTH_ADMIN_MODIFIER_WINDOW_HPP_ */
