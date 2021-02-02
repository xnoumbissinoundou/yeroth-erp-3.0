/*
 * yeroth-erp-fournisseurs-window.hpp
 *
 *      Author: DR. XAVIER NOUMBISSI NOUNDOU
 */

#ifndef SRC_WINDOWS_YEROTH_ERP_FOURNISSEURS_WINDOW_HPP_
#define SRC_WINDOWS_YEROTH_ERP_FOURNISSEURS_WINDOW_HPP_

#include "../../../ui_yeroth-erp-fournisseurs-window.h"


#include "src/windows/yeroth-erp-window-commons.hpp"


#include <QtCore/QDebug>

#include <QtWidgets/QMessageBox>

#include <QtGui/QContextMenuEvent>


class QStandardItemModel;
class QContextMenuEvent;
class QProcess;

class YerothSqlTableModel;

class YerothERPFournisseursWindow : public YerothWindowsCommons,
							   	    private Ui_YerothERPFournisseursWindow,
									public YerothAbstractClassYerothSearchWindow
{
    Q_OBJECT

public:

	YEROTH_CLASS_OPERATORS

    YerothERPFournisseursWindow();

    virtual ~YerothERPFournisseursWindow();

    inline virtual QToolBar * getQMainWindowToolBar()
    {
    	return toolBar_fournisseursWindow;
    }

    virtual void definirCaissier();

    virtual void definirManager();

    virtual void definirVendeur();

    virtual void definirGestionaireDesStocks();

    virtual void definirMagasinier();

    virtual void definirPasDeRole();

    MACRO_TO_DEFINE_VIEWING_PAGE_NUMBER_FOR_TABLEVIEW(label_fournisseurs_numero_page_derniere,
    												  label_fournisseurs_numero_page_courante)

    virtual void rendreVisible(YerothSqlTableModel *stocksTableModel);

public slots:

	MACRO_TO_DEFINE_VIEWING_POINTERS_PAGE_SLOTS(tableView_fournisseurs)

	inline virtual void apropos()
	{
		YerothWindowsCommons::apropos(this);
	}

	inline virtual void help()
	{
		YerothQMessageBox::information(this,
							 QObject::trUtf8("aide"),
							 QObject::trUtf8("Sélectionner un fournisseur dans le tableau "
											 "des fournisseurs, ensuite cliquer sur l'opération que vous souhaitez réaliser !"));
	}

	bool export_csv_file();

	virtual bool imprimer_pdf_document();

    virtual void afficherFournisseurs(YerothSqlTableModel &fournisseurSqlTableModel);

    inline void afficherFournisseurs()
    {
    	afficherFournisseurs(*_curFournisseursTableModel);
    }

    void afficher_nom_entreprise_selectioner(const QString &nomEntreprise);

protected:

    virtual void reinitialiser_colones_db_visibles();

    virtual void contextMenuEvent(QContextMenuEvent *event);

    virtual void hideEvent(QHideEvent * hideEvent);

    virtual void setupShortcuts();

    virtual void set_filtrer_font();

protected slots:

    virtual void slot_reinitialiser_colones_db_visibles();

    virtual void textChangedSearchLineEditsQCompleters();

private slots:

	void modifierFournisseur();

	void supprimer_PLUSIEURS_Fournisseurs(YerothSqlTableModel &aFournisseursTableModel);

	void supprimerFournisseur();

	void afficher_au_detail();

	inline virtual void afficher_au_detail(const QModelIndex & modelIndex)
	{
		afficher_au_detail();
	}

	bool filtrer();

    void reinitialiser_elements_filtrage();

    void reinitialiser_recherche();

private:

    void populateFournisseursComboBoxes();

    void setupLineEdits();


    YerothLogger			*_logger;

    QFont 					*_pushButton_filtrer_font;

    YerothSqlTableModel 	*_curFournisseursTableModel;
};

#endif /* SRC_WINDOWS_YEROTH_ERP_FOURNISSEURS_WINDOW_HPP_ */
