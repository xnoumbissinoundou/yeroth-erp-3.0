/*
 * yeroth-erp-alertes-window.hpp
 *
 *      Author: DR. XAVIER NOUMBISSI NOUNDOU
 */

#ifndef YEROTH_ERP_ALERTES_WINDOW_HPP_
#define YEROTH_ERP_ALERTES_WINDOW_HPP_

#include "../../ui_yeroth-erp-alertes-window.h"


#include <QtCore/QDebug>

#include <QtWidgets/QMessageBox>

#include <QtGui/QContextMenuEvent>


#include "yeroth-erp-window-commons.hpp"


class QStandardItemModel;
class QContextMenuEvent;
class QProcess;

class YerothSqlTableModel;

class YerothAlertesWindow : public YerothWindowsCommons,
						    private Ui_YerothAlertesWindow,
						    public YerothAbstractClassYerothSearchWindow
{
    Q_OBJECT

	enum TabIndex
	{
		TableauDesAlertes 		= 0,
		AfficherAlerteAuDetail	= 1
	};

public:

	YEROTH_CLASS_OPERATORS

    YerothAlertesWindow();

    virtual ~YerothAlertesWindow();

    inline virtual QToolBar * getQMainWindowToolBar()
    {
    	return toolBar_alertesWindow;
    }

    virtual void definirCaissier();

    virtual void definirManager();

    virtual void definirVendeur();

    virtual void definirGestionaireDesStocks();

    virtual void definirMagasinier();

    virtual void definirPasDeRole();

    MACRO_TO_DEFINE_VIEWING_PAGE_NUMBER_FOR_TABLEVIEW(label_alertes_numero_page_derniere,
    												  label_alertes_numero_page_courante)

    virtual void rendreVisible(YerothSqlTableModel *stocksTableModel);

public slots:

	MACRO_TO_DEFINE_VIEWING_POINTERS_PAGE_SLOTS(tableView_alertes)

	inline virtual void apropos()
	{
		YerothWindowsCommons::apropos(this);
	}

	inline virtual void help()
	{
		YerothQMessageBox::information(this,
							 QObject::trUtf8("aide"),
							 QObject::trUtf8("Sélectionner un alertes dans le tableau des alertes, ensuite cliquer "
											 "sur l'opération que vous souhaitez réaliser !"));
	}

	virtual bool imprimer_pdf_document();

	void resetFilter(YerothSqlTableModel *alertesSqlTableModel);

    virtual void lister_les_elements_du_tableau(YerothSqlTableModel &alertesSqlTableModel);

    inline void lister_les_elements_du_tableau()
    {
    	lister_les_elements_du_tableau(*_curAlertesSqlTableModel);
    }

    void afficher_alerte_selectioner(const QString &alerteDesignation);

    bool afficher_au_detail();

    void reinitialiser_recherche();

    void handle_visibilite_autres_elements(bool aBoolValue);

protected slots:

	void handleCurrentTabChanged(int index);

	void handleComboBoxClients_courriers_alertes_resolue_oui_OU_non(const QString &currentText);

protected:

	virtual void reinitialiser_champs_db_visibles();

	void contextMenuEvent(QContextMenuEvent *event);

    virtual void setupShortcuts();

 protected slots:

    virtual void slot_reinitialiser_champs_db_visibles();

    virtual void textChangedSearchLineEditsQCompleters();

	inline virtual void disableImprimer()
	{
		actionAfficherPDF->setVisible(false);
	}

	inline virtual void enableImprimer()
	{
		actionAfficherPDF->setVisible(true);
	}

private slots:

	void afficher_alertes();

    void marquer_resolue();

    void supprimer();

private:

    void setCurrentUser_NOM_UTILISATEUR(const QString &aUserNomUtilisateur);

    void disable_NOM_UTILISATEUR();

    void enable_NOM_UTILISATEUR_ONLY_MANAGER();

    void clear_all_fields();

    void populateComboBoxes();

    void setupLineEdits();


    YerothLogger			*_logger;

    YerothSqlTableModel 	*_curAlertesSqlTableModel;
};

#endif /* YEROTH_ERP_ALERTES_WINDOW_HPP_ */
