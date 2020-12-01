/*
 * yeroth-erp-sortir-window.hpp
 *
 *      Author: DR. XAVIER NOUMBISSI NOUNDOU
 */

# ifndef YEROTH_SORTIR_WINDOW_HPP_
# define YEROTH_SORTIR_WINDOW_HPP_


# include "../../ui_yeroth-erp-sortir-window.h"


# include <QtWidgets/QMessageBox>

# include <QtCore/QMap>

# include <QtCore/QDebug>


#include "yeroth-erp-window-commons.hpp"


class QProcess;

class YerothSqlTableModel;
class YerothTableView;
class YerothArticleVenteInfo;

class YerothSortirWindow : public YerothWindowsCommons,
						   private Ui_YerothSortirWindow
{
    Q_OBJECT

public:

	YEROTH_CLASS_OPERATORS

	YerothSortirWindow();

    virtual ~YerothSortirWindow();

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

    void handleTabViews();

    virtual void rendreVisible(YerothSqlTableModel *stocksTableModel);

public slots:

	QString afficher_facture_pdf(QString referenceRecuSORTIE = QString(""));

	QString imprimer_facture(QString referenceRecuSORTIE = QString(""));

	QString imprimer_facture_grand(QString referenceRecuSortieGRAND = QString(""));

	QString imprimer_facture_petit(QString referenceRecuSortiePETIT = QString(""));

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

    void ajouter_article_codebar(const QString &text);

    void actualiser_articles(int row, unsigned newItemQte);

    void actualiser_articles_codebar(int row, unsigned newItemQte);

    void actualiser_tableau_sortie();

    inline void update_lineedits_and_labels()
    {
        lineEdit_articles_quantite_a_transferer->setText(GET_NUM_STRING(_quantiteVendue));
    }

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

protected slots:

	inline virtual void disableImprimer()
	{
		actionAfficherPDF->setVisible(false);
	}

	inline virtual void enableImprimer()
	{
		actionAfficherPDF->setVisible(true);
	}

protected:

    virtual void hideEvent(QHideEvent *hideEvent);

    virtual void setupShortcuts();

private slots:

	void setBarcodeAsStandardInput();

	void setStockItemNameAsStandardInput();

	void updateLineEditQCompleterInput();

	void handleBasculerLecteurDeCodebarres();

	void handleRefreshSaleStrategy();

private:

    enum TabIndex
    {
    	TableauDesSorties = 0,
    	AfficherSortieAuDetail
    };

	void disconnect_all_objects_for_stock_article_item_selection();

	void connect_manual_selection_of_article_item();

	void connect_barcode_reader_selection_of_article_item();

	bool check_fields();

	inline void check_fields_entry()
	{
	    lineEdit_articles_nom_recepteur->checkField();
	}

    void clear_all_fields();

    void setupLineEditsQCompleters();

    void setupLineEdits();

    inline void setupDateTimeEdits()
    {
    	dateEdit_article_detail_date_peremption->setEnabled(false);
    }


    YerothLogger				*_logger;

    YerothLineEdit			*_currentFocusSearchBar;

    static bool            _qteChangeCodeBar;

    bool					_barcodeReaderActivated;

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
