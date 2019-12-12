/*
 * yeroth-erp-sortir-window.hpp
 *
 *  Created on: June 18, 2016
 *      Author: Dipl.-Inf. Xavier NOUMBISSI NOUNDOU, Ph.D. (ABD)
 *      Email:  xnoundou7@gmail.com
 */

# ifndef YEROTH_SORTIR_WINDOW_HPP_
# define YEROTH_SORTIR_WINDOW_HPP_


# include "../../ui_yeroth-erp-sortir-window.h"

# include <QtWidgets/QMessageBox>

# include <QtCore/QMap>

# include <QtCore/QDebug>

# include "src/windows/yeroth-erp-search-form.hpp"
#include "yeroth-erp-window-commons.hpp"

class QProcess;

class YerothSqlTableModel;
class YerothTableView;
class YerothArticleVenteInfo;

class YerothSortirWindow : public YerothWindowsCommons, private Ui_YerothSortirWindow
{
    Q_OBJECT

public:

	YEROTH_CLASS_OPERATORS

	YerothSortirWindow();

    ~YerothSortirWindow();

    inline virtual QToolBar * getQMainWindowToolBar()
    {
    	return toolBar_sortirWindow;
    }

    void deleteArticleVenteInfos();

    virtual void definirCaissier();

    virtual void definirManager();

    virtual void definirVendeur();

    virtual void definirGestionaireDesStocks();

    virtual void definirMagasinier();

    virtual void definirPasDeRole();

    void cleanUpAfterVente();

    virtual void rendreInvisible();

    virtual void rendreVisible(YerothSqlTableModel *stocksTableModel);

public slots:

	inline void disableImprimer()
	{
		actionAfficherPDF->setEnabled(false);
	}

	inline void enableImprimer()
	{
		actionAfficherPDF->setEnabled(true);
	}

	inline void handleClient()
	{
		lineEdit_articles_nom_client->clear();
	}

	inline void handleDestination()
	{
		lineEdit_articles_transfert->clear();
	}

	QString afficher_facture_pdf(int vente_id = -1);

	QString imprimer_facture(int vente_id = -1);

	QString imprimer_facture_grand(int vente_id = -1);

	QString imprimer_facture_petit(int vente_id = -1);

	void handle_info_succursale(QString &factureTexDocument,
								const QString nom_succursale);

	void handle_info_client(QString &factureTexDocument,
							const QString nom_cient);

	void handleAddedArticle();

	void handleCurrentChanged(int index);

	void annuler();

	void setRechercheLineEditFocus();

	void setRechercheDesignationArticleFocus();

	void setRechercheCodebarArticleFocus();

	void activateLineEditRemisePrix(bool toggled);

	void activateLineEditRemisePourcentage(bool toggled);

	void calculate_details_window_remise_prix();

    void calculate_details_window_remise_pourcentage();

    void actualiser_montant_remise();

    void resetCheckboxTVA();

    void handleTVACheckBox(bool clicked);

	inline void handleQtePressed(QTableWidgetItem *itemPressed)
	{
		_previousPressedQteValue = itemPressed->text();
	}

	void handleQteChange(QTableWidgetItem *itemChanged);

	void updateQuantiteAVendre();

	bool article_exist(const QString codeBar,
			 	 	   const QString designation);


    void ajouter_article(const QString &text);

	//This is also used to add articles from yeroth-erp-pointdevente-liststocks-window
    void ajouter_article(const QModelIndex& modelIndex);

    void ajouter_article_codebar(const QString &text);

    void ajouter_article_codebar(const QModelIndex& modelIndex);

    void actualiser_articles(int row, unsigned newItemQte);

    void actualiser_articles_codebar(int row, unsigned newItemQte);

    void actualiser_tableau_sortie();

    void update_lineedits_and_labels();

    void enlever_article();

    void afficher_tableau_sorties(const int tableWidgetRow);

    void afficher_sortie_detail(const int tableWidgetRow);

    inline void afficher_sortie_detail(const QModelIndex &modelIndex)
    {
    	afficher_sortie_detail(modelIndex.row());
    }

    inline void afficher_sortie_detail(QTableWidgetItem *item)
    {
    	afficher_sortie_detail(item->row());
    }

    inline void afficher_sorties()
    {
    	tabWidget_sorties->setCurrentIndex(TableauDesSorties);
    }

    inline void rendreInvisibleAvecConservation()
    {
    	YerothWindowsCommons::rendreInvisible();
    }

	void lister();

	void sortir();

	void retourSorties();

    inline void setCurrentClientName(QString curClientName)
    {
    	_curClientName = curClientName;
    }

    inline QString getCurrentClientName()
    {
    	return _curClientName;
    }

	inline virtual void apropos()
	{
		YerothWindowsCommons::apropos(this);
	}

    inline virtual void help()
    {
		YerothQMessageBox::information(this,
						 QObject::trUtf8("aide"),
						 QObject::trUtf8("Choisissez les articles à sortir en utilisant "
			    						 "leur reference à la première barre de recherche, ou bien "
			    						 "leur désignation à la deuxième barre de recherche !"));
    }

protected:

    virtual void hideEvent(QHideEvent *hideEvent);

    virtual void setupShortcuts();

private:

    enum TabIndex
    {
    	TableauDesSorties = 0,
    	AfficherSortieAuDetail
    };

    bool check_fields();

    void clear_all_fields();

    void setupLineEditsQCompleters();

    void setupLineEdits();

    inline void setupDateTimeEdits()
    {
    	dateEdit_article_detail_date_peremption->setEnabled(false);
    }

    static const QString 	_WINDOW_TITLE;

    YerothLogger				*_logger;

    YerothLineEdit			*_currentFocusSearchBar;

    static bool            _qteChangeCodeBar;

    QProcess 				*_linuxWhich;

    bool 					_updateItemConversionError;
    QString					_previousPressedQteValue;

    bool					_tvaCheckBoxPreviousState;

    double 					_sommeTotal;
    double					_tva;
    int 					_quantiteVendue;
    int 				 	_currentTabView;

    QAction 				*actionRechercheArticle;
    QAction 				*actionRechercheArticleCodebar;

    QString					_searchFilter;
	QString					_curClientName;

    QMap<int, YerothArticleVenteInfo *> articleItemToVenteInfo;
};


#endif /* YEROTH_SORTIR_WINDOW_HPP_ */
