/*
 * yeroth-erp-IMPRESSION_DE_DOCUMENT-dialog.cpp
 *
 *      Author: PR. XAVIER NOUMBISSI NOUNDOU
 */

#include "yeroth-erp-IMPRESSION_DE_DOCUMENT-dialog.hpp"

#include "src/yeroth-erp-windows.hpp"

#include "src/utils/print_latex_pdf/yeroth-erp-print_yeroth_latex_util_pdf.hpp"


YerothWindowsCommons *YerothIMPRESSION_DE_DOCUMENT_Dialog::_current_window_to_print;

YerothTableViewPRINT_UTILITIES_TEX_TABLE *YerothIMPRESSION_DE_DOCUMENT_Dialog::_yeroth_CURRENT_DOCUMENT_PRINT_UTILITIES_TEX_TABLE;


const QString YerothIMPRESSION_DE_DOCUMENT_Dialog::DIALOG_WINDOW_TITLE(QString("%1 - %2").
        																arg(YEROTH_ERP_WINDOW_TITLE,
        																	 QObject::trUtf8("IMPRESSION DE DOCUMENT")));




YerothIMPRESSION_DE_DOCUMENT_Dialog::YerothIMPRESSION_DE_DOCUMENT_Dialog()
:YerothPOSDialogCommons(YerothIMPRESSION_DE_DOCUMENT_Dialog::DIALOG_WINDOW_TITLE),
 _currentPosition(new QPoint)
{
    setupUi(this);

    QDesktopWidget &desktopWidget = _allWindows->desktopWidget();

	YerothUtils::getCenterPosition(desktopWidget.width(),
								  desktopWidget.height(),
								  *this,
								  *_currentPosition);

    setupLineEdits();

    setFixedSize(width(), height());

    connect(pushButton_annuler, SIGNAL(clicked()), this, SLOT(annuler()));
    connect(pushButton_valider, SIGNAL(clicked()), this, SLOT(valider()));
}


YerothIMPRESSION_DE_DOCUMENT_Dialog::~YerothIMPRESSION_DE_DOCUMENT_Dialog()
{
	delete _currentPosition;
}


void YerothIMPRESSION_DE_DOCUMENT_Dialog::setupLineEdits()
{

}

void YerothIMPRESSION_DE_DOCUMENT_Dialog::reset_all_fields()
{

}


void YerothIMPRESSION_DE_DOCUMENT_Dialog::show()
{
	move(*_currentPosition);

	YerothPOSDialogCommons::show();
}


void YerothIMPRESSION_DE_DOCUMENT_Dialog::closeEvent(QCloseEvent * closeEvent)
{
    YerothPOSDialogCommons::closeEvent(closeEvent);
}


void YerothIMPRESSION_DE_DOCUMENT_Dialog::rendreInvisible()
{
	setVisible(false);
}


void YerothIMPRESSION_DE_DOCUMENT_Dialog::valider()
{
	if (0 != _yeroth_CURRENT_DOCUMENT_PRINT_UTILITIES_TEX_TABLE)
	{
		if (radioButton_IMPRESSION_VERTICAL->isChecked())
		{
			_yeroth_CURRENT_DOCUMENT_PRINT_UTILITIES_TEX_TABLE->SET_VERTICAL_PRINT_POSITION();
		}
		else if (radioButton_IMPRESSION_HORIZONTAL->isChecked())
		{
			_yeroth_CURRENT_DOCUMENT_PRINT_UTILITIES_TEX_TABLE->SET_HORIZONTAL_PRINT_POSITION();
		}
	}

	rendreInvisible();

	if (0 != _current_window_to_print)
	{
		_current_window_to_print->imprimer_pdf_document_WITH_A_YEROTH_PROGRESS_BAR();
	}
}

