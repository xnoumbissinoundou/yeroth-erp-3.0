/*
 * yeroth-erp-modifier.hpp
 *
 *      Author: Xavier NOUMBISSI NOUNDOU, Dipl.-Inf., Ph.D. (ABD)
 */

# ifndef SRC_WINDOWS_YEROTH_MODIFIER_WINDOW_HPP_
# define SRC_WINDOWS_YEROTH_MODIFIER_WINDOW_HPP_

# include "../../ui_yeroth-erp-modifier-window.h"

# include <QtWidgets/QMessageBox>
#include "yeroth-erp-window-commons.hpp"

class QContextMenuEvent;

class YerothSqlTableModel;

class YerothModifierWindow : public YerothWindowsCommons, private Ui_YerothModifierWindow
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

    void display_quantite_restante(const QString &quantite_par_lot);

    void display_quantite_restante_by_spinbox(int lots);

    inline void edited_prix_vente(const QString & newPrixVente)
    {
    	_lastEditedPrixVente = newPrixVente;
    }

    void display_prix_vente();

    void handleTVACheckBox(bool clicked);

    void supprimer_ce_stock();

    void supprimer_image_stock();

    inline void selectionner_image_produit()
    {
    	YerothUtils::selectionner_image(this, *label_image_produit);
    }

    void actualiser_article();

    virtual void deconnecter_utilisateur();

	inline virtual void apropos()
	{
		YerothWindowsCommons::apropos(this);
	}

	inline virtual void help()
	{
		YerothQMessageBox::information(this,
						 QObject::trUtf8("aide"),
						 QObject::trUtf8("Modifier les informations sur le stock, "
										 "et ensuite cliquer sur le bouton 'actualiser'!"));
	}

protected:

    virtual void setupShortcuts();

private:

    void setupLineEdits();

    inline void setupDateTimeEdits()
    {
        dateEdit_date_peremption->setStartDate(GET_CURRENT_DATE);
    }

    bool check_fields();

    void clear_all_fields();

    void showItem();

    static const unsigned int 	INDEX_ZERO;

    static const QString 		_WINDOW_TITLE;

    YerothLogger					*_logger;

    double						_montantTva;

    bool						_tvaCheckBoxPreviousState;

    QString						_lastEditedPrixVente;

    QString						_tvaPercent;
};


#endif /* SRC_WINDOWS_YEROTH_MODIFIER_WINDOW_HPP_ */
