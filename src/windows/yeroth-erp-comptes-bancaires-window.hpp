/*
 * yeroth-erp-comptes-bancaires-window.hpp
 *
 *      Author: DR. XAVIER NOUMBISSI NOUNDOU
 */

#ifndef SRC_YEROTH_COMPTES_BANCAIRES_WINDOW_HPP_
#define SRC_YEROTH_COMPTES_BANCAIRES_WINDOW_HPP_

#include "../../ui_yeroth-erp-comptes-bancaires-window.h"

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

class YerothComptesBancairesWindow : public YerothWindowsCommons,
									 private Ui_YerothComptesBancairesWindow,
									 public YerothAbstractClassYerothSearchWindow
{
    Q_OBJECT

public:

	YEROTH_CLASS_OPERATORS

    YerothComptesBancairesWindow();

	virtual ~YerothComptesBancairesWindow();

    inline virtual QToolBar * getQMainWindowToolBar()
    {
    	return toolBar_comptes_bancaires_Window;
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

    MACRO_TO_DEFINE_VIEWING_PAGE_NUMBER_FOR_TABLEVIEW(label_comptes_bancaires_numero_page_derniere,
    												  label_comptes_bancaires_numero_page_courante)

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

	virtual bool imprimer_pdf_document();

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

	void set_filtrer_font();

	void refineYerothLineEdits();

private:

	void setupLineEdits();

	void setupDateTimeEdits();


	QString 				_comptesBancairesDateDebutTransactionFilter;

    YerothLogger			*_logger;

    YerothSqlTableModel 	*_curComptesBancairesSqlTableModel;

    QFont 					*_pushButton_comptes_bancaires_filtrer_font;
};

#endif /* SRC_YEROTH_COMPTES_BANCAIRES_WINDOW_HPP_ */
