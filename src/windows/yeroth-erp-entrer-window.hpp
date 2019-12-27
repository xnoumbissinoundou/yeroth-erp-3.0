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

class YerothEntrerWindow : public YerothWindowsCommons, private Ui_YerothEntrerWindow
{
    Q_OBJECT

public:

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
										 "sur enregistrer!"));
	}

	bool product_search_with_codebar();

    void display_quantite_total(const QString &quantite_par_lot);

    void display_quantite_total_by_spinbox(int lots);

    inline void edited_prix_vente(const QString & newPrixVente)
    {
    	_lastEditedPrixVente = newPrixVente;
    }

    void display_prix_vente();

    void handleTVACheckBox(bool);

    void handleCategorieName(const QString &text);

    void handleFournisseurName(const QString &text);

    inline void rendreInvisibleAvecConservation()
    {
    	this->setVisible(false);
    }

    inline virtual void modifier()
    {
    	this->afficherStocks();
    }

    void deconnecter_utilisateur();

    void selectionner_image_produit();

    bool creerNouveauFournisseur(const QString proposedFourisseurName);

    bool creerNouvelleCategorie(const QString proposedCategorieName);

    inline double getMargeBeneficiaire(double prix_vente,
    								   double prix_dachat,
									   double montantTva)
    {
    	return (prix_vente - QString::number(montantTva, 'f', 2).toDouble() - prix_dachat);
    }

    bool isProfitable();

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

protected:

    virtual void setupShortcuts();

private:

    bool insertStockItemInProductList();

    bool isStockItemInProductList();

    void showItem();

    bool check_fields();

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
