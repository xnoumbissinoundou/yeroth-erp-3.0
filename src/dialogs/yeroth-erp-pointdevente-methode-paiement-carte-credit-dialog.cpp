/*
 * yeroth-erp-pointdevente-methode-paiement-carte-credit-dialog.cpp
 *
 *      Author: DIPL.-INF. XAVIER NOUMBISSI NOUNDOU
 */

#include "yeroth-erp-pointdevente-methode-paiement-carte-credit-dialog.hpp"

#include"src/yeroth-erp-windows.hpp"

#include"src/utils/yeroth-erp-utils.hpp"

#include "src/widgets/yeroth-erp-qmessage-box.hpp"

#include "src/utils/credit-card/yeroth-erp-carte-credit-info.hpp"

#include <QtCore/QPoint>

#include <QtWidgets/QDesktopWidget>

#include <QtWidgets/QDialog>

#include <QtWidgets/QMainWindow>


const QString YerothPOSVenteMethodePaiementCarteCreditDialog::DIALOG_WINDOW_TITLE(QString("%1 - %2").
        																arg(YEROTH_ERP_WINDOW_TITLE,
        																	 QObject::trUtf8("PAIEMENT PAR CARTE DE CRÉDIT")));

YerothPOSVenteMethodePaiementCarteCreditDialog::YerothPOSVenteMethodePaiementCarteCreditDialog(YerothWindowsCommons *parent)

:YerothPOSDialogCommons(YerothPOSVenteMethodePaiementCarteCreditDialog::DIALOG_WINDOW_TITLE),
 _currentPosition(new QPoint),
 _creditCardInfo(0)
{
	setupUi(this);

    QDesktopWidget &desktopWidget = _allWindows->desktopWidget();

	YerothUtils::getCenterPosition(desktopWidget.width(),
								  desktopWidget.height(),
								  *this,
								  *_currentPosition);

	setWindowTitle(YerothPOSVenteMethodePaiementCarteCreditDialog::DIALOG_WINDOW_TITLE);

	setFixedSize(width(), height());

	pushButton_annuler->disable(this);

	pushButton_valider->disable(this);

	setupLineEdits();
}


YerothPOSVenteMethodePaiementCarteCreditDialog::~YerothPOSVenteMethodePaiementCarteCreditDialog()
{
	reset_all_fields();

	delete _currentPosition;
}


void YerothPOSVenteMethodePaiementCarteCreditDialog::setCreditCardIssuingCompany(QString anIssuingCompany)
{
	if ( 0 != _creditCardInfo)
	{
		_creditCardInfo->_issuingCompany = anIssuingCompany;
	}
}


void YerothPOSVenteMethodePaiementCarteCreditDialog::setCreditCardNameOnCard(QString aNameOnCard)
{
	if ( 0 != _creditCardInfo)
	{
		_creditCardInfo->_nameOnCard = aNameOnCard;
	}
}


void YerothPOSVenteMethodePaiementCarteCreditDialog::setCreditCardPostalCode(QString aPostalCode)
{
	if ( 0 != _creditCardInfo)
	{
		_creditCardInfo->_postalCode = aPostalCode;
	}
}


void YerothPOSVenteMethodePaiementCarteCreditDialog::setCreditCardNumber(QString aCardNumber)
{
	if ( 0 != _creditCardInfo)
	{
		_creditCardInfo->_cardNumber = aCardNumber;
	}
}


void YerothPOSVenteMethodePaiementCarteCreditDialog::setCreditCardExpiryDateYY(QString anExpiryDate_YY)
{
	if ( 0 != _creditCardInfo)
	{
		_creditCardInfo->_expiryDate_YY = anExpiryDate_YY;
	}
}


void YerothPOSVenteMethodePaiementCarteCreditDialog::setCreditCardExpiryDateMM(QString anExpiryDate_MM)
{
	if ( 0 != _creditCardInfo)
	{
		_creditCardInfo->_expiryDate_MM = anExpiryDate_MM;
	}
}


void YerothPOSVenteMethodePaiementCarteCreditDialog::setCreditCardCVCode(QString aCVCode)
{
	if ( 0 != _creditCardInfo)
	{
		_creditCardInfo->_cvCode = aCVCode;
	}
}


QString YerothPOSVenteMethodePaiementCarteCreditDialog::getCreditCardIssuingCompany()
{
	if(0 != _creditCardInfo)
	{
		return _creditCardInfo->_issuingCompany;
	}

	return "";
}


QString YerothPOSVenteMethodePaiementCarteCreditDialog::getCreditCardNameOnCard()
{
	if(0 != _creditCardInfo)
	{
		return _creditCardInfo->_nameOnCard;
	}

	return "";
}


QString YerothPOSVenteMethodePaiementCarteCreditDialog::getCreditCardPostalCode()
{
	if(0 != _creditCardInfo)
	{
		return _creditCardInfo->_postalCode;
	}

	return "";
}


QString YerothPOSVenteMethodePaiementCarteCreditDialog::getCreditCardNumber()
{
	if(0 != _creditCardInfo)
	{
		return _creditCardInfo->_cardNumber;
	}

	return "";
}


QString YerothPOSVenteMethodePaiementCarteCreditDialog::getCreditCardExpiryDateYY()
{
	if(0 != _creditCardInfo)
	{
		return _creditCardInfo->_expiryDate_YY;
	}

	return "";
}


QString YerothPOSVenteMethodePaiementCarteCreditDialog::getCreditCardExpiryDateMM()
{
	if(0 != _creditCardInfo)
	{
		return _creditCardInfo->_expiryDate_MM;
	}

	return "";
}


QString YerothPOSVenteMethodePaiementCarteCreditDialog::getCreditCardCVCode()
{
	if(0 != _creditCardInfo)
	{
		return _creditCardInfo->_cvCode;
	}

	return "";
}


void YerothPOSVenteMethodePaiementCarteCreditDialog::setupLineEdits()
{
	comboBox_carte_credit_societe->populateComboBoxRawString(_allWindows->CREDIT_CARD_ISSUING_COMPANY,
    													     QString("credit_card_issuing_company"));

	lineEdit_carte_credit_numero->				setInputMask("9999 - 9999 - 9999 - 9999");
	lineEdit_carte_credit_expiration_annee->	setInputMask("99");
	lineEdit_carte_credit_expiration_mois->		setInputMask("99");
	lineEdit_carte_credit_ccv->					setInputMask("999");
}


void YerothPOSVenteMethodePaiementCarteCreditDialog::reset_all_fields()
{
	comboBox_carte_credit_societe->clearField();

	lineEdit_carte_credit_numero->clearField();
	lineEdit_carte_credit_expiration_annee->clearField();
	lineEdit_carte_credit_expiration_mois->clearField();
	lineEdit_carte_credit_nom->clearField();
	lineEdit_carte_credit_ccv->clearField();
}


void YerothPOSVenteMethodePaiementCarteCreditDialog::show()
{
	move(*_currentPosition);

	pushButton_annuler->enable(this, SLOT(annuler()));

	pushButton_valider->enable(this, SLOT(valider()));

	YerothPOSDialogCommons::show();
}


void YerothPOSVenteMethodePaiementCarteCreditDialog::rendreInvisible()
{
	pushButton_annuler->disable(this);

	pushButton_valider->disable(this);

	YerothPOSDialogCommons::rendreInvisible();
}


bool YerothPOSVenteMethodePaiementCarteCreditDialog::checkAllMandatoryCreditCardFields()
{
	bool cIssuingCompany 	= comboBox_carte_credit_societe->checkField();
	bool cCardNumber 	 	= lineEdit_carte_credit_numero->checkField();
	bool cExpiryDate_YY		= lineEdit_carte_credit_expiration_annee->checkField();
	bool cExpiryDate_MM		= lineEdit_carte_credit_expiration_mois->checkField();
	bool cNameOnCard		= lineEdit_carte_credit_nom->checkField();
	bool cCvCode			= lineEdit_carte_credit_ccv->checkField();


	return cIssuingCompany 	&&
			cCardNumber		&&
			cExpiryDate_YY	&&
			cExpiryDate_MM	&&
			cNameOnCard		&&
			cCvCode;
}


void YerothPOSVenteMethodePaiementCarteCreditDialog::valider()
{
	if ( 0 != _creditCardInfo 				 &&
		 checkAllMandatoryCreditCardFields()    )
	{
		_creditCardInfo->_issuingCompany 	= comboBox_carte_credit_societe->currentText();
		_creditCardInfo->_cardNumber 		= lineEdit_carte_credit_numero->text();
		_creditCardInfo->_expiryDate_YY		= lineEdit_carte_credit_expiration_annee->text();
		_creditCardInfo->_expiryDate_MM		= lineEdit_carte_credit_expiration_mois->text();
		_creditCardInfo->_nameOnCard		= lineEdit_carte_credit_nom->text();
		_creditCardInfo->_cvCode			= lineEdit_carte_credit_ccv->text();

		emit_SIGNAL_CREDIT_CARD_INFO_ALL_PROCESSED();
	}
	else
	{
        YerothQMessageBox::information(_allWindows->_pdVenteWindow,
        							  QObject::trUtf8("remplir tous les champs de texte"),
                                      QObject::trUtf8("S'il vous plait, remplir tous les champs de texte !"),
									  QMessageBox::Ok);
	}
}


