
#include "yeroth-erp-pointdevente-methode-paiement-comptant-entree-dialog.hpp"

#include"src/yeroth-erp-windows.hpp"

#include"src/utils/yeroth-erp-utils.hpp"

#include <QtCore/QPoint>

#include <QtWidgets/QDesktopWidget>

#include <QtWidgets/QDialog>

#include <QtWidgets/QMainWindow>


const QString YerothPOSVenteMethodePaiementComptantEntreeDialog::DIALOG_WINDOW_TITLE(QString("%1 - %2").
        																arg(YEROTH_ERP_WINDOW_TITLE,
        																	 QObject::trUtf8("Entrée du Montant de Paiement Comptant")));

YerothPOSVenteMethodePaiementComptantEntreeDialog::YerothPOSVenteMethodePaiementComptantEntreeDialog(YerothERPWindows *allWindows,
																	   	   	   	   	   	   	   	   YerothWindowsCommons *parent)

:YerothPOSDialogCommons(YerothPOSVenteMethodePaiementComptantEntreeDialog::DIALOG_WINDOW_TITLE),
 _currentPosition(new QPoint)
{
	setupUi(this);

    QDesktopWidget &desktopWidget = _allWindows->desktopWidget();

	YerothUtils::getCenterPosition(desktopWidget.width(),
								  desktopWidget.height(),
								  *this,
								  *_currentPosition);

	setWindowTitle(YerothPOSVenteMethodePaiementComptantEntreeDialog::DIALOG_WINDOW_TITLE);

	setFixedSize(width(), height());

	lineEdit_vente_montant_recu->setValidator(&YerothUtils::DoubleValidator);
}

void YerothPOSVenteMethodePaiementComptantEntreeDialog::valider()
{
	_allWindows->_pdVenteWindow->setMontantComptantRecu(lineEdit_vente_montant_recu->text().toDouble());

	if ( -1 != _allWindows->_pdVenteWindow->effectuer_check_out_comptant() )
	{
		rendreInvisible();
	}
	else
	{
		setFocus();
	}
}


void YerothPOSVenteMethodePaiementComptantEntreeDialog::show()
{
    move(*_currentPosition);

    clearLineEditValue();

	lineEdit_vente_montant_recu->setFocus();

	pushButton_annuler->enable(this, SLOT(annuler()));

	pushButton_valider->enable(this, SLOT(valider()));

	YerothPOSDialogCommons::show();
}


void YerothPOSVenteMethodePaiementComptantEntreeDialog::rendreInvisible()
{
	pushButton_annuler->disable(this);

	pushButton_valider->disable(this);

	YerothPOSDialogCommons::rendreInvisible();
}

