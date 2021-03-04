/*
 * yeroth-erp-creer-un-programme-de-fidelite-clients-window.hpp
 *
 *      Author: DR. XAVIER NOUMBISSI NOUNDOU
 */

#ifndef YEROTH_CREER_UN_PROGRAMME_DE_FIDELITE_CLIENTS_WINDOW_HPP_
#define YEROTH_CREER_UN_PROGRAMME_DE_FIDELITE_CLIENTS_WINDOW_HPP_

#include "../../../../ui_yeroth-erp-creer-un-programme-de-fidelite-clients-window.h"

# include <QtWidgets/QMessageBox>
#include "src/windows/yeroth-erp-window-commons.hpp"

class YerothERPWindows;
class YerothSqlTableModel;

class YerothCreerUnProgrammeDeFideliteClientsWindow : public YerothWindowsCommons,
									     	 	 	  private Ui_YerothCreerUnProgrammeDeFideliteClientsWindow
{
    Q_OBJECT

public:

	YEROTH_CLASS_OPERATORS

    YerothCreerUnProgrammeDeFideliteClientsWindow();

    inline ~YerothCreerUnProgrammeDeFideliteClientsWindow()
    {
    	delete _logger;
    }

    inline virtual QToolBar * getQMainWindowToolBar()
    {
    	return toolBar_YerothCreerProgrammeDeFideliteClientsWindow;
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

    inline virtual void definirMagasinier()
    {
       definirPasDeRole();
    }

    virtual void definirPasDeRole();

    virtual void rendreInvisible();

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
						 QObject::trUtf8("Remplisser le formulaire, et ensuite cliquer "
										 "sur enregistrer !"));
	}

    void deconnecter_utilisateur();

protected:

    virtual void setupShortcuts();

private slots:

	void annuler_la_creation_dun_programme_de_fidelite_de_clients();

	bool creerEnregistrerUnProgrammeDeFideliteClients();

private:

	bool royaltyClientProgramAlreadyExists();

	bool check_fields();

    void clear_all_fields();

    void updatePopulateComboBoxes();

    inline void populateComboBoxes()
    {
    	comboBox_creer_un_programme_de_fidelite_clients_localisation->
    		populateComboBoxRawString(YerothDatabase::LOCALISATIONS,
    								  YerothDatabaseTableColumn::NOM_LOCALISATION);
    }


    int				_last_selected_table_widget_row;

    YerothLogger	*_logger;
};


#endif /* YEROTH_CREER_UN_PROGRAMME_DE_FIDELITE_CLIENTS_WINDOW_HPP_ */
