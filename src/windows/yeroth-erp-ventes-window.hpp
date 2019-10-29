/*
 * yeroth-erp-ventes-window.hpp
 *
 *  Created on: Dec 29, 2015
 *      Author: Dipl.-Inf. Xavier NOUMBISSI NOUNDOU, Ph.D. (ABD)
 *      Email:  xnoundou7@gmail.com
 */

#ifndef SRC_YEROTH_CAISSE_WINDOW_HPP_
#define SRC_YEROTH_CAISSE_WINDOW_HPP_


# include "../../ui_yeroth-erp-ventes-window.h"

# include <QtWidgets/QMessageBox>

# include "src/utils/yeroth-erp-logger.hpp"

# include "src/windows/yeroth-erp-search-form.hpp"
#include "yeroth-erp-window-commons.hpp"

class QContextMenuEvent;

class YerothERPWindows;
class YerothSqlTableModel;
class YerothLogger;

class QProcess;

class YerothVentesWindow : public YerothWindowsCommons, private Ui_YerothVentesWindow
{
    Q_OBJECT

	enum TabIndex
	{
		TableauDesVentes = 0,
		AfficherVenteAuDetail
	};

public:

    YEROTH_CLASS_OPERATORS

	YerothVentesWindow();

	inline ~YerothVentesWindow()
	{
		delete _logger;
	}

    inline bool isCurrentlyFiltered()
    {
    	return _currentlyFiltered;
    }

	inline virtual QToolBar * getQMainWindowToolBar()
	{
		return toolBar_caisseWindow;
	}

    virtual void definirCaissier();

    virtual void definirManager();

    virtual void definirVendeur();

    virtual void definirGestionaireDesStocks();

    virtual void definirMagasinier();

    virtual void definirPasDeRole();

    void lister_les_elements_du_tableau(YerothSqlTableModel &stocksVenduTableModel);

    inline void lister_les_elements_du_tableau()
    {
    	lister_les_elements_du_tableau(*_curStocksVenduTableModel);
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
						 QObject::trUtf8("1) Vous avez accès aux informations des ventes"
								   	   	 " en utilisant la référence d'un article. Ceci se fait"
								   	   	 " dans le 1er champ de recherche de cette l'interface"
								   	     " graphique!\n\n"
								   	     "2) Vous pouvez filtrer les résultats de la recherche,"
								   	   	 " sur les informations de ventes, en paramétrant"
								 	 	 " celle-ci avec des dates de 'Début', et de 'Fin'.\n\n"
								   	     " Vous pouvez filtrer les résultats de la recherche,"
								 	 	 " en utilisant les paramètres que l'on retrouve"
								   	     " tout en bas de l'interface graphique"
								 	 	 " (example: nom du caissier, désignation de l'article, etc.)!"));
	}

	void readProcessData();

	/*
	 * This is only use when printing sales information into the
	 * 'journal de ventes / journal of sales'.
	 * Printing sales information into the visible table is done
	 * by the function 'lister_les_elements_du_tableau'
	 */
	void getJournalDesVentesTexTableString(QString &texTable_in_out,
			  	  	  	   	   	   	   	   QStandardItemModel &tableStandardItemModel,
										   QList<int> &columnsToIgnore,
										   int fromRowIndex,
										   int toRowIndex,
										   bool lastPage);

	inline void disableImprimer()
	{
		actionAfficherPDF->setVisible(false);
	}

	inline void enableImprimer()
	{
		actionAfficherPDF->setVisible(true);
	}

	bool export_csv_file();

	virtual bool imprimer_document();

	void handleCurrentChanged(int index);

	void retourVentes();

    virtual void deconnecter_utilisateur();

    void rechercher(bool clearVentesRecherche = false);

    inline void venteRecherche()
    {
    	this->rechercher(false);
    }

    void reinitialiser_elements_filtrage();

    void reinitialiser_recherche();

    void afficher_vente_detail();

    inline void afficher_ventes()
    {
    	tabWidget_ventes->setCurrentIndex(TableauDesVentes);
    }

    inline void setLastListerSelectedRow(int row)
    {
    	tableView_ventes->setLastSelectedRow(row);
    }

    void resetFilter(YerothSqlTableModel *stocksVenduTableModel);

protected:

    void contextMenuEvent(QContextMenuEvent *event);

    virtual void setupShortcuts();

private slots:

	bool filtrer_ventes();

private:

	void set_filtrer_font();

	void setCurrentlyFiltered(bool currentlyFiltered);

    void populateComboBoxes();

    void setupLineEdits();

    void setupLineEditsQCompleters();

    void clear_all_fields();

    void setupDateTimeEdits();

    inline void setFilter()
    {
        _curStocksVenduTableModel->yerothSetFilter(_searchFilter);
    }

    static const QString 	_WINDOW_TITLE;

    YerothLogger				*_logger;

    QProcess 				*_aProcess;

    int 				 	_currentTabView;

    bool					_currentlyFiltered;

    QFont 					*_pushButton_ventes_filtrer_font;

    QString					_searchFilter;

    YerothSqlTableModel 	*_curStocksVenduTableModel;
};


#endif /* SRC_YEROTH_CAISSE_WINDOW_HPP_ */
