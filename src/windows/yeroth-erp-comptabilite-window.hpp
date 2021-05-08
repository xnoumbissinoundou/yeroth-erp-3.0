/*
 * yeroth-erp-comptabilite-window.hpp
 *
 *      Author: PR. XAVIER NOUMBISSI NOUNDOU
 */

#ifndef SRC_YEROTH_COMPTES_BANCAIRES_WINDOW_HPP_
#define SRC_YEROTH_COMPTES_BANCAIRES_WINDOW_HPP_

#include "../../ui_yeroth-erp-comptabilite-window.h"

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

class YerothComptabiliteWindow : public YerothWindowsCommons,
									 private Ui_YerothComptabiliteWindow,
									 public YerothAbstractClassYerothSearchWindow
{
    Q_OBJECT

public:

	YEROTH_CLASS_OPERATORS

    YerothComptabiliteWindow();

	virtual ~YerothComptabiliteWindow();

    inline virtual QToolBar * getQMainWindowToolBar()
    {
    	return toolBar_comptabilite_Window;
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

    MACRO_TO_DEFINE_VIEWING_PAGE_NUMBER_FOR_TABLEVIEW(label_comptabilite_numero_page_derniere,
    												  label_comptabilite_numero_page_courante)

    virtual void rendreVisible(YerothSqlTableModel *stocksTableModel);

public slots:

	MACRO_TO_DEFINE_VIEWING_POINTERS_PAGE_SLOTS(tableView_comptes_doperations_comptables)

	inline virtual void apropos()
	{
		YerothWindowsCommons::apropos(this);
	}

	inline virtual void help()
	{
		YerothQMessageBox::information(this,
						 QObject::trUtf8("aide"),
						 QObject::trUtf8("1) Vous avez accès aux informations sur tous les"
								   	   	 " comptes bancaires enregistrés dans YEROTH-ERP-3.0,"
								   	   	 " ainsi que toutes les transactions financières"
								   	   	 " éffectuées sur ses comptes bancaires !"));
	}

	bool export_csv_file();

	virtual QString get_latex_template_print_pdf_content();

	virtual void deconnecter_utilisateur();

	void afficher_comptes_bancaires(YerothSqlTableModel &aYerothSqlTableModel);

	inline void afficher_comptes_bancaires()
	{
		textChangedSearchLineEditsQCompleters();
	}

protected:

	virtual void reinitialiser_colones_db_visibles();

	virtual void hideEvent(QHideEvent * hideEvent);

    virtual void setupShortcuts();

protected slots:

	virtual void slot_reinitialiser_colones_db_visibles();

	virtual void textChangedSearchLineEditsQCompleters();

private slots:

	void handle_change_tab(int);

	void set_filtrer_font();

	bool creer_le_compte_doperation_comptable();

	void clear_creer_le_compte_doperation_comptable();

	inline void annuler_creer_le_compte_doperation_comptable()
	{
		clear_creer_le_compte_doperation_comptable();
	}

	void refineYerothLineEdits();

private:

	bool creer_COMPTE_DOPERATION_CHECK_FIELDS();

	void updateComboBoxes();

	void populateComboBoxes();

	void setupLineEdits();

	void setupDateTimeEdits();


	QString 				_comptesDoperationsComptablesTransactionFilter;

    YerothLogger			*_logger;

    YerothSqlTableModel 	*_curCompte_DOPERATIONS_COMPTABLES_SqlTableModel;

    QFont 					*_pushButton_comptabilite_filtrer_font;
};

#endif /* SRC_YEROTH_COMPTES_BANCAIRES_WINDOW_HPP_ */
