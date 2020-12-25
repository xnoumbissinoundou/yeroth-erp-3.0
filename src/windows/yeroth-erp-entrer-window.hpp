/*
 * yeroth-erp-entrer-window.hpp
 *
 *      Author: DR. XAVIER NOUMBISSI NOUNDOU
 */

# ifndef YEROTH_ENTRER_WINDOW_HPP_
# define YEROTH_ENTRER_WINDOW_HPP_


# include "../../ui_yeroth-erp-entrer-window.h"


# include <QtWidgets/QMessageBox>


#include "src/windows/yeroth-erp-window-commons.hpp"


class YerothERPWindows;
class YerothSqlTableModel;
class ServiceClientInfo;


class YerothEntrerWindow : public YerothWindowsCommons,
						   private Ui_YerothEntrerWindow
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

    inline void rendreInvisibleAvecConservation()
    {
    	setVisible(false);
    }

    inline virtual void modifier()
    {
    	afficherStocks();
    }

    void deconnecter_utilisateur();

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

    void enregistrer_achat_au_fournisseur();

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

	void handle_enregistrer();

	bool product_search_with_designation();

	bool product_search_with_codebar();

	void display_quantite_total(const QString &quantite_par_lot);

	void display_quantite_total_by_spinbox(double lots);

	void calculate_and_display_ALL_benefit_buying_price_percentage();

	void calculate_and_display_benefit_buying_price_percentage();

	void calculate_and_display_benefit_buying_price_percentage_EN_GROS();

	void edited_prix_vente(const QString & newPrixVente);

	void edited_prix_vente_en_gros(const QString & newPrixVente);

	void display_service_montant_total_vente();

	void display_prix_vente();

	void display_prix_vente_en_gros();

	void setStockSpecificWidgetVisible(bool visible);

	void handle_checkBox_service_achat(bool checked);

	void handle_checkBox_service_vente(bool checked);

	void handleTVACheckBox(int state);

	void handleCategorieName(const QString &text);

	bool check_fields_mandatory_buying();

private:

	inline double get_prix_vente_en_gros()
	{
		return !lineEdit_prix_vente_en_gros->isEmpty() ?
				lineEdit_prix_vente_en_gros->text().toDouble() :
				lineEdit_prix_vente->text().toDouble();
	}

    bool insertStockItemInProductList();

    void showItem();

    bool check_fields_service_achat();

    bool check_fields_service_vente();

    bool check_fields(bool withClearAllServiceMandatoryFields = false);

    void clear_all_fields();

    void setupLineEdits();

    void setupLineEditsQCompleters__FOR_STOCK_INVENTORY();

    void setupLineEditsQCompleters();

    inline void setupDateTimeEdits()
    {
        dateEdit_date_peremption->setStartDate(GET_CURRENT_DATE);
    }


    YerothLogger				*_logger;

    ServiceClientInfo		*_currentServiceInfo;

    unsigned 				_stocks_id;

    double					_montantTva;

    double					_montantTva_en_gros;

    bool					_tvaCheckBoxPreviousState;

    bool					_createNewCategorie;

    bool					_createNewFournisseur;

    QString 				_curCategorieName;

    QString 				_curFournisseurName;

    QString					_lastEditedPrixVente;

    QString					_lastEditedPrixVente_en_gros;
};


#endif /* YEROTH_ENTRER_WINDOW_HPP_ */
