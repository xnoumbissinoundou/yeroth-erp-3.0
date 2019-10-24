/*
 * yeroth-erp-pointdevente-methode-paiement-comptant-entree-dialog.hpp
 *
 *      Author: Dipl.-Inf. Xavier NOUMBISSI NOUNDOU, Ph.D. (ABD)
 *      Email:  xnoundou7@gmail.com
 */

#ifndef YEROTH_ERP_VENTE_METHODE_PAIEMENT_COMPTANT_ENTREE_DIALOG_HPP_
#define YEROTH_ERP_VENTE_METHODE_PAIEMENT_COMPTANT_ENTREE_DIALOG_HPP_

#include "../../ui_yeroth-erp-pointdevente-methode-paiement-comptant-entree-dialog.h"

#include "yeroth-erp-dialog-commons.hpp"

class QPoint;

class YerothWindowsCommons;
class YerothERPWindows;

class YerothPOSVenteMethodePaiementComptantEntreeDialog : public YerothPOSDialogCommons,
														 public Ui_YerothPOSVenteMethodePaiementComptantEntreeDialog
{
    Q_OBJECT

public:

	YEROTH_CLASS_OPERATORS

	YerothPOSVenteMethodePaiementComptantEntreeDialog(YerothERPWindows *allWindows,
									   	   	   	     YerothWindowsCommons *parent);

    inline ~YerothPOSVenteMethodePaiementComptantEntreeDialog()
    {
    	delete _currentPosition;
    }

    virtual QPoint * getCurrentPosition()
    {
    	return _currentPosition;
    }

public slots:

	inline void clearLineEditValue()
	{
		lineEdit_vente_montant_recu->clear();
	}

	virtual void show();

	virtual void rendreInvisible();

	void valider();

	inline void annuler()
	{
		rendreInvisible();
	}

private:

	static const QString 	DIALOG_WINDOW_TITLE;

	QPoint					*_currentPosition;

};


#endif /* YEROTH_ERP_VENTE_METHODE_PAIEMENT_COMPTANT_ENTREE_DIALOG_HPP_ */
