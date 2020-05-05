/*
 * yeroth-erp-entrer-window.hpp
 *
 *      Author: Xavier NOUMBISSI NOUNDOU, Dipl.-Inf., Ph.D. (ABD)
 */

# ifndef YEROTH_ENTRER_WINDOW_HPP_
# define YEROTH_ENTRER_WINDOW_HPP_


# include "../../ui_yeroth-erp-entrer-window.h"


# include <QtWidgets/QMessageBox>


#include "yeroth-erp-window-commons.hpp"


class YerothERPWindows;
class YerothSqlTableModel;
class ServiceClientInfo;


enum service_stock_already_exist_type
{
	SERVICE_REFERENCE_EXISTS 						= 0,
	SERVICE_STOCK_DESIGNATION_AND_CATEGORIE_EXIST 	= 1,
	SERVICE_STOCK_UNDEFINED							= 2
};


class YerothEntrerWindow : public YerothWindowsCommons, private Ui_YerothEntrerWindow
{
    Q_OBJECT

public:

	class ServiceClientInfo
	{
	public:
    	int ID_client;
    	double nouveau_compte_client;

    	QString reference;
    	QString designation;
    	QString nom_categorie;
    	QString nom_entreprise_client;
	};

	YEROTH_CLASS_OPERATORS

    YerothEntrerWindow();

    inline ~YerothEntrerWindow()
    {
    	delete _logger;
    }

    inline QToolBar * getQMainWindowToolBar()
    {
    	return toolBar_entrerWindow;
    }

    virtual void definirCaissier();

    virtual void definirManager();

    virtual void definirVendeur();

    virtual void definirGestionaireDesStocks();

    virtual void definirMagasinier();

    virtual void definirPasDeRole();

    virtual void rendreInvisible();

    void rendreVisible(YerothSqlTableModel *stocksTableModel,
					   bool aShowItem = false);

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

	bool product_search_with_codebar();

    void display_quantite_total(const QString &quantite_par_lot);

    void display_quantite_total_by_spinbox(double lots);

    void calculate_and_display_benefit_buying_price_percentage();

    void edited_prix_vente(const QString & newPrixVente);

    void display_service_montant_total_vente();

    void display_prix_vente();

    void setStockSpecificWidgetVisible(bool visible);

    void handle_service_checkBox(int state);

    void handleTVACheckBox(int state);

    void handleCategorieName(const QString &text);

    inline void rendreInvisibleAvecConservation()
    {
    	this->setVisible(false);
    }

    inline virtual void modifier()
    {
    	this->afficherStocks();
    }

    void deconnecter_utilisateur();

    bool creerNouveauClient(const QString proposedCustomerName);

    bool creerNouveauFournisseur(const QString proposedFourisseurName);

    bool creerNouvelleCategorie(const QString proposedCategorieName);

    bool handle_stocks_vendu_table(int stockID,
    							   ServiceClientInfo &aServiceInfo,
								   double montant_total_vente,
								   double nouveau_compte_client);

    bool handle_clients_table(int stockID, double montant_total_vente);

    inline bool hasBuying()
    {
    	return checkBox_achat->isChecked() &&
    		   !lineEdit_prix_dachat->text().isEmpty();
    }

    bool isReferenceUnique(QString aStockServiceReference,
			   	   	   	   QString aStockServiceDesignation,
						   QString aStockServiceNomCategorie,
						   QString &curExistingReference_in_out);

    void enregistrer_produit();

	inline void setCurrentCategorieName(QString curCategorieName)
    {
    	_curCategorieName = curCategorieName;
    }

    inline QString getCurrentCategorieName()
    {
    	return _curCategorieName;
    }

	inline void setCurrentFournisseurName(QString curCategorieName)
    {
    	_curFournisseurName = curCategorieName;
    }

    inline QString setCurrentFournisseurName()
    {
    	return _curFournisseurName;
    }

    inline QString getCurrentFournisseurName()
    {
    	return _curFournisseurName;
    }

    void handle_achat_checkBox(int aState);

protected slots:

    inline void supprimer_image_stock()
    {
    	YerothWindowsCommons::supprimer_image(*label_image_produit);
    }

    inline void selectionner_image_produit()
    {
    	YerothWindowsCommons::selectionner_image(*label_image_produit);
    }

protected:

    virtual void setupShortcuts();

private slots:

	bool check_fields_mandatory_buying();

private:

    bool insertStockItemInProductList();

    enum service_stock_already_exist_type isStockItemInProductList();

    void showItem();

    bool check_fields_service();

    bool check_fields(bool withClearAllServiceMandatoryFields = false);

    void clear_all_fields();

    void setupLineEdits();

    void setupLineEditsQCompleters();

    inline void setupDateTimeEdits()
    {
        dateEdit_date_peremption->setStartDate(GET_CURRENT_DATE);
    }

    static const unsigned int MAX_STOCKS;

    static const unsigned int INDEX_ZERO;

    static const QString 	_WINDOW_TITLE;

    YerothLogger				*_logger;

    ServiceClientInfo		*_currentServiceInfo;

    unsigned 				_stocks_id;

    double					_montantTva;

    bool					_tvaCheckBoxPreviousState;

    bool					_createNewCategorie;

    bool					_createNewFournisseur;

    QString 				_curCategorieName;

    QString 				_curFournisseurName;

    QString					_lastEditedPrixVente;
};


#endif /* YEROTH_ENTRER_WINDOW_HPP_ */
