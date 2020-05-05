/*
 * yeroth-erp-pointdevente-window.hpp
 *
 *      Author: Xavier NOUMBISSI NOUNDOU, Dipl.-Inf., Ph.D. (ABD)
 */

#ifndef YEROTH_VENTE_WINDOW_HPP_
#define YEROTH_VENTE_WINDOW_HPP_


#include "../../ui_yeroth-erp-pointdevente-window.h"


#include "yeroth-erp-window-commons.hpp"

#include "src/utils/credit-card/yeroth-erp-carte-credit-info.hpp"

#include <QtCore/QMap>

#include <QtCore/QDebug>


class YerothSqlTableModel;
class YerothTableView;
class YerothArticleVenteInfo;
class YerothPOSCreditCardInfo;


class YerothPointDeVenteWindow : public YerothWindowsCommons, public Ui_YerothPointDeVenteWindow
{
    Q_OBJECT

public:

	YEROTH_CLASS_OPERATORS

	YerothPointDeVenteWindow();

    virtual ~YerothPointDeVenteWindow();

	inline bool paiement_par_carte_credit_carte_debit()
	{
	    return _paiement_carte_credit_carte_debit;
	}

	inline bool paiement_comptant()
	{
	    return _paiment_comptant;
	}

	inline bool paiment_compte_client()
	{
	    return _paiement_compte_client;
	}

	inline void set_paiement_par_carte_credit_carte_debit(bool aValueCarteCreditCarteDebit)
	{
	    _paiement_carte_credit_carte_debit = aValueCarteCreditCarteDebit;
	}

	inline void set_paiement_comptant(bool aValueComptant)
	{
	    _paiment_comptant = aValueComptant;
	}

	inline void set_paiment_compte_client(bool aValueCompteClient)
	{
	    _paiement_compte_client = aValueCompteClient;
	}

    inline virtual QToolBar * getQMainWindowToolBar()
    {
    	return toolBar_venteWindow;
    }

    void setupRemises(bool aValue);

    void deleteArticleVenteInfos();

    void cleanUpAfterVente();

    virtual void definirCaissier();

    virtual void definirManager();

    virtual void definirVendeur();

    virtual void definirGestionaireDesStocks();

    virtual void definirMagasinier();

    virtual void definirPasDeRole();

    virtual void rendreInvisible();

    void handleTabViews();

    virtual void rendreVisible(YerothSqlTableModel *stocksTableModel);

signals:

	void SELLING();

public slots:

		inline void disableImprimer()
		{
			actionAfficherPDF->setEnabled(false);
		}

		inline void enableImprimer()
		{
			actionAfficherPDF->setEnabled(true);
		}

		QString afficher_facture_pdf(QString referenceRecu = QString(""));

		QString imprimer_facture(QString referenceRecu = QString(""));

		QString imprimer_facture_grand(QString referenceRecuGRAND = QString(""));

		QString imprimer_facture_petit(QString referenceRecuPETIT = QString(""));

		void handleAddedArticle();

		void handleCurrentChanged(int index);

		void annuler();

		void setRechercheLineEditFocus();

		void setRechercheDesignationArticleFocus();

		void setRechercheCodebarArticleFocus();

		void handleClientName(const QString& text);

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

	    void actualiser_tableau_vente();

	    void actualiser_toutes_valeurs();

	    void update_lineedits_and_labels(double total);

	    inline void setMontantComptantRecu(double unMontantRecu)
	    {
	    	_montantRecu = unMontantRecu;
	    }

		void handleMontantRecu();

	    void enlever_article();

	    void afficher_tableau_ventes(const int tableWidgetRow);

	    void afficher_vente_detail(const int tableWidgetRow);

	    inline void afficher_vente_detail(const QModelIndex &modelIndex)
	    {
	    	afficher_vente_detail(modelIndex.row());
	    }

	    inline void afficher_vente_detail(QTableWidgetItem *item)
	    {
	    	afficher_vente_detail(item->row());
	    }

	    inline void afficher_ventes()
	    {
	    	tabWidget_vente->setCurrentIndex(TableauDesVentes);
	    }

	    inline void rendreInvisibleAvecConservation()
	    {
	    	setVisible(false);
	    }

		void lister();

		inline YerothPOSCreditCardInfo  *getCurrentCreditCardInfo()
		{
			return _currentCreditCardInfo;
		}

		void choisir_methode_paiment();

		unsigned int effectuer_check_out_carte_credit_carte_debit();

		void executer_la_vente_comptant();

		unsigned int effectuer_check_out_comptant();

		void updateCompteClient(double nouveau_compte_client);

		void executer_la_vente_compte_client();

		unsigned int effectuer_check_out_compte_client();

		void retourVentes();

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
							 QObject::trUtf8("Choisissez les articles à vendre en utilisant "
											 "leur référence à la première barre de recherche, ou bien "
											 "leur désignation à la deuxième barre de recherche !"));
		}

private slots:

	void setBarcodeAsStandardInput();

	void setStockItemNameAsStandardInput();

	void handleBarCodeScannerCheckBox(int state);

	bool PROCESS_CREDIT_CARD_PAYMENT();

	bool PRE__PROCESS_CREDIT_CARD_PAYMENT();

protected:

    virtual void hideEvent(QHideEvent *event);

    virtual void setupShortcuts();

private:

    enum TabIndex
    {
    	TableauDesVentes = 0,
    	AfficherVenteAuDetail
    };

    void disconnect_all_objects_for_stock_article_item_selection();

	void connect_manual_selection_of_article_item();

	void connect_barcode_reader_selection_of_article_item();

	inline bool isBarCodeReaderSelectionOfArticleItem()
	{
		return checkBox_lecteur_de_code_barres->isChecked();
	}

    void setupLineEdits();

    void setupLineEditsQCompleters();

    inline void setupDateTimeEdits()
    {
    	dateEdit_article_detail_date_peremption->setEnabled(false);
    }

    static const QString 	_WINDOW_TITLE;

    YerothLogger				*_logger;

    bool					_paiement_carte_credit_carte_debit;

    bool 					_paiment_comptant;

    bool					_paiement_compte_client;

    YerothLineEdit			*_currentFocusSearchBar;

    YerothPOSCreditCardInfo  *_currentCreditCardInfo;

    static bool 			_qteChangeCodeBar;

    bool 					_updateItemConversionError;

    int						_typeDeVente;
    QString					_previousPressedQteValue;

    bool					_tvaCheckBoxPreviousState;

    double 					_sommeTotal;
    double 					_remise_somme_total_prix;
    double 					_remise_somme_total_pourcentage;
    double					_tva;
    double					_montantRecu;
    double					_montantARembourser;
    int 					_quantiteAVendre;
    int 				 	_currentTabView;

    QAction 				*actionRechercheArticle;
    QAction 				*actionRechercheArticleCodebar;

    QString					_searchFilter;
	QString					_curClientName;

    QMap<int, YerothArticleVenteInfo *> articleItemToVenteInfo;
};


#endif /* YEROTH_VENTE_WINDOW_HPP_ */
