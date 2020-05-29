/*
 * yeroth-erp-paiements-window.hpp
 *
 *      Author: Xavier NOUMBISSI NOUNDOU, Dipl.-Inf., Ph.D. (ABD)
 */

#ifndef SRC_YEROTH_HISTORIQUE_PAIEMENTS_WINDOW_HPP_
#define SRC_YEROTH_HISTORIQUE_PAIEMENTS_WINDOW_HPP_


# include "../../ui_yeroth-erp-paiements-window.h"


#include "src/utils/yeroth-erp-logger.hpp"

#include "src/windows/yeroth-erp-window-commons.hpp"


#include <QtWidgets/QMessageBox>


class QContextMenuEvent;

class YerothERPWindows;
class YerothSqlTableModel;
class YerothLogger;

class QProcess;

class YerothPaiementsWindow : public YerothWindowsCommons,
							  private Ui_YerothPaiementsWindow,
							  public YerothAbstractClassYerothSearchWindow
{
    Q_OBJECT

	enum TabIndex
	{
		TableauDesPaiements = 0,
		AfficherPaiementAuDetail
	};

public:

    YEROTH_CLASS_OPERATORS

	YerothPaiementsWindow();

	inline ~YerothPaiementsWindow()
	{
		delete _logger;
	}

	inline virtual QToolBar * getQMainWindowToolBar()
	{
		return toolBar_historiquePaiementsWindow;
	}

    virtual void definirCaissier();

    virtual void definirManager();

    virtual void definirVendeur();

    virtual void definirGestionaireDesStocks();

    virtual void definirMagasinier();

    virtual void definirPasDeRole();

    void lister_les_elements_du_tableau(YerothSqlTableModel &historiquePaiementsTableModel);

    inline void lister_les_elements_du_tableau()
    {
    	lister_les_elements_du_tableau(*_curPaiementsTableModel);
    }

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
						 QObject::trUtf8("1) Vous avez accès aux informations sur les paiements"
								   	   	 " en utilisant le nom de l'entreprise. Ceci se fait"
								   	   	 " dans le 1er champ de recherche de cette l'interface"
								   	     " graphique !\n\n"
								   	     "2) Vous pouvez filtrer les résultats de la recherche,"
								   	   	 " sur les informations de paiements, en paramétrant"
								 	 	 " celle-ci avec des dates de 'Début', et de 'Fin'.\n\n"
								   	     " Vous pouvez filtrer les résultats de la recherche,"
								 	 	 " en utilisant les paramètres que l'on retrouve"
								   	     " tout en bas de l'interface graphique"
								 	 	 " (exemple: numéro du bon de paiement, etc.) !"));
	}

	void readProcessData();

	inline void disableImprimer()
	{
		actionAfficherPDF->setVisible(false);
	}

	inline void enableImprimer()
	{
		actionAfficherPDF->setVisible(true);
	}

	bool export_csv_file();

	virtual bool imprimer_pdf_document();

	void handleCurrentChanged(int index);

	void retourPaiements();

    virtual void deconnecter_utilisateur();

    void refineYerothLineEdits();

    void reinitialiser_elements_filtrage();

    void reinitialiser_recherche();

    void afficher_paiements_detail();

    inline void afficher_paiements()
    {
    	textChangedSearchLineEditsQCompleters();
    }

    inline void setLastListerSelectedRow(int row)
    {
    	tableView_paiements->setLastSelectedRow(row);
    }

    void resetFilter(YerothSqlTableModel *stocksVenduTableModel);

protected slots:

   	virtual void slot_reinitialiser_champs_db_visibles();

	void handleComboBoxClients_Typedepaiement_TextChanged(const QString &currentText);

	virtual void textChangedSearchLineEditsQCompleters();

protected:

	virtual void reinitialiser_champs_db_visibles();

    void contextMenuEvent(QContextMenuEvent *event);

    virtual void setupShortcuts();

    virtual void set_filtrer_font();

private slots:

	bool filtrer_paiements();

private:

    void populateComboBoxes();

    void setupLineEdits();

    void clear_all_fields();

    void setupDateTimeEdits();

    static const QString 	_WINDOW_TITLE;

    YerothLogger			*_logger;

    QProcess 				*_aProcess;

    int 				 	_currentTabView;

    QFont 					*_pushButton_paiements_filtrer_font;

    QString					_paiementsDateFilter;

    YerothSqlTableModel 	*_curPaiementsTableModel;
};


#endif /* SRC_YEROTH_HISTORIQUE_PAIEMENTS_WINDOW_HPP_ */
