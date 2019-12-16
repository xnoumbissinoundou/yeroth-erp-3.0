/*
 * yeroth-erp-admin-window.hpp
 *
 *      Author: Xavier NOUMBISSI NOUNDOU, Dipl.-Inf., Ph.D. (ABD)
 */

# ifndef SRC_ADMIN_YEROTH_ADMIN_WINDOW_HPP_
# define SRC_ADMIN_YEROTH_ADMIN_WINDOW_HPP_

# include "../../ui_yeroth-erp-admin-window.h"
# include "src/admin/yeroth-erp-admin-windows-commons.hpp"
# include "../yeroth-erp-windows.hpp"

# include "admin-actions-subjects.hpp"

# include <QtWidgets/QMessageBox>


class QStyle;


class YerothSqlTableModel;
class YerothLogger;
class YerothDatabase;

class YerothAdminWindow : public YerothPOSAdminWindowsCommons, private Ui_YerothAdminWindow
{
    Q_OBJECT

public:

	YEROTH_CLASS_OPERATORS

	enum YerothAdminWindowTabPositionsTypes
	{
		CONNECTER_LOCALISATION = 0,
		MAINTENANCE			   = 1,
		OPERATIONS			   = 2,
		PARAMETRES_APPLICATION = 3,
		SYSTEME_DALERTES	   = 4,
		DONNEES_ENTREPRISE 	   = 5
	};


    YerothAdminWindow();

    ~YerothAdminWindow();

    inline virtual QToolBar * getQMainWindowToolBar()
    {
    	return toolBar_adminWindow;
    }

    void setupValidators();

    virtual void definirPasDeRole();

    virtual void definirAdministrateur();

    virtual void definirManager();

    virtual void rendreVisible(YerothSqlTableModel *stocksTableModel = 0);

public slots:

	void enableAllOperationsTabPushButtons();

	void disableAllOperationsTabPushButtons();

	void setCurrentAdminWindowTitle(enum YerothAdminWindowTabPositionsTypes tabPosition);

	void handleTabChanged(int);

    void action_creer();

    void action_lister();

    void action_modifier();

    void action_supprimer();

    void action_exporter();

    void action_importer();

    void action_effacer();

    void gerer_choix_action();

    void choose_path_pdfReader();

    void choose_path_thermalPrinterDeviceFile();

    void choose_path_latex_system_root_folder();

    void choose_repertoire_fichiers_temporaires();

    void read_configuration();

    void handleThermalPrinterConfiguration(const QString &addedPrinterValue);

    void read_entreprise_info_database_table(bool initalizationValues = true);

    void read_alert_system_init_configuration();

    void read_app_parameters_init_configuration();

    void reinitialiser_alert_system_configuration();

    void reinitialiser_app_parameters_configuration();

    void enregistrer_entreprise_info_database_table();

    void enregistrer_alert_system_configuration();

    void enregistrer_app_parameters_configuration();

    void set_colour_status_of_alert_daemon_process_buttons();

    void start_alert_daemon_process();

    void stop_alert_daemon_process();

    void set_localisation_adresse_ip_text();

    void connecter_localisation_db();

    void deconnecter_localisation_db();

	virtual void deconnecter_utilisateur();

private slots:

	void handleCheckboxActiverRegistreCaisse(int state);

	void choix_registre_de_caisse(const QString &);

private:

    void creer(enum AdminSujetAction selectedSujetAction);

    void lister(enum AdminSujetAction selectedSujetAction);

    void modifier(enum AdminSujetAction selectedSujetAction);

    inline void maintenance_exporter(enum AdminSujetAction selectedSujetAction){}

    inline void maintenance_importer(enum AdminSujetAction selectedSujetAction){}

    inline void maintenance_effacer(enum AdminSujetAction selectedSujetAction){}

	static const QString CREER;
	static const QString LISTER;
	static const QString MODIFIER;
	static const QString SUPPRIMER;
	static const QString EXPORTER;
	static const QString IMPORTER;
	static const QString EFFACER;

	static const QString COMPTE_UTILISATEUR;
	static const QString LOCALISATION;
	static const QString CATEGORIE;
	static const QString CLIENT;
	static const QString FOURNISSEUR;
	static const QString ALERTE;
	static const QString REMISE;
	static const QString BON_DE_COMMANDE;
	static const QString DB;
	static const QString TABLEAU;
	static const QString DONNEES;

	QString _currentWindowTitle;
	QString _yerothAdminWindowTitleStart;

    enum AdminAction 		_curAdminAction;
    enum AdminSujetAction 	_curAdminSujetAction;

    enum AdminAction 		_curAdminMaintainAction;
    enum AdminSujetAction 	_curAdminSujetsMaintainAction;

    YerothLogger							*_logger;

    QMap<QString, AdminAction> 			*_actionsToConst;

    QMap<QString, AdminSujetAction> 	*_sujetActionsToConst;

    QString 							_localisation_adresse_ip;
};


#endif /* SRC_ADMIN_YEROTH_ADMIN_WINDOW_HPP_ */
