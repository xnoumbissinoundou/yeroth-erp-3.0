/*
 * yeroth-erp-modifier.hpp
 *
 *      Author: DR. XAVIER NOUMBISSI NOUNDOU
 */

#ifndef SRC_WINDOWS_YEROTH_MODIFIER_WINDOW_HPP_
#define SRC_WINDOWS_YEROTH_MODIFIER_WINDOW_HPP_

#include "../../../ui_yeroth-erp-modifier-window.h"


#include "src/windows/yeroth-erp-window-commons.hpp"


#include <QtWidgets/QMessageBox>


class QContextMenuEvent;

class YerothSqlTableModel;

class YerothModifierWindow : public YerothWindowsCommons,
							 private Ui_YerothModifierWindow
{
    Q_OBJECT

public:

	YEROTH_CLASS_OPERATORS

	YerothModifierWindow();

    inline ~YerothModifierWindow()
    {
    	delete _logger;
    }

    inline virtual QToolBar * getQMainWindowToolBar()
    {
    	return toolBar_modifierWindow;
    }

    virtual void definirCaissier();

    virtual void definirManager();

    virtual void definirVendeur();

    virtual void definirGestionaireDesStocks();

    virtual void definirMagasinier();

    virtual void definirPasDeRole();

    virtual void rendreInvisible();

    virtual void rendreVisible(YerothSqlTableModel *stocksTableModel);

public slots:

	void display_prix_vente();

    void display_prix_vente_EN_GROS();

    void handleTVACheckBox(bool clicked);

    void supprimer_ce_stock();

    void supprimer_image_stock();

    inline void selectionner_image_produit()
    {
    	YerothWindowsCommons::selectionner_image(*label_image_produit);
    }

    virtual void deconnecter_utilisateur();

	inline virtual void apropos()
	{
		YerothWindowsCommons::apropos(this);
	}

	inline virtual void help()
	{
		YerothQMessageBox::information(this,
						 QObject::trUtf8("aide"),
						 QObject::trUtf8("Modifier les informations sur le stock (service), "
										 "et ensuite cliquer sur le bouton 'actualiser'!"));
	}

protected:

    virtual void setupShortcuts();

private slots:

	void handle_achat_checkBox(int aState);

	void handle_re_approvisionnement_checkBox(int state);

	void actualiser_stock();

	void edited_prix_vente(const QString &newPrixVente);

	void edited_prix_vente_EN_GROS(const QString &newPrixVente_en_gros);

	void calculate_and_display_ALL_benefit_buying_price_percentage();

	void calculate_and_display_benefit_buying_price_percentage();

	void calculate_and_display_benefit_buying_price_percentage_EN_GROS();

private:

	void handleVisibilityPrixDachat(bool anEditable);

	void ajouter_nouveau_re_approvisionnement_achat(double quantiteEnReApprovisionement,
													int stockId);

	void update_achat_deja_existant(const QSqlRecord &aStockRecord,
									double aPrixUnitaireHT);

    void setupLineEdits();

    void setupLineEditsQCompleters();

    inline void setupDateTimeEdits()
    {
        dateEdit_date_peremption->setStartDate(GET_CURRENT_DATE);
    }

    bool check_fields();

    void clear_all_fields();

    void setStockSpecificWidgetVisible(bool visible);

    void showItem();


    YerothLogger				*_logger;

    int							_currentStockID;

    double						_montantTva;

    double						_montantTva_EN_GROS;

    bool						_tvaCheckBoxPreviousState;

    QString						_currentPrixDachat;

    QString						_lastEditedPrixVente;

    QString						_lastEditedPrixVente_EN_GROS;

    QString						_tvaPercent;
};


#endif /* SRC_WINDOWS_YEROTH_MODIFIER_WINDOW_HPP_ */
