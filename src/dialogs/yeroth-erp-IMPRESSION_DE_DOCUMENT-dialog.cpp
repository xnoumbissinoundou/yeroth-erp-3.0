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

	radioButton_IMPRESSION_VERTICAL->setChecked(true);

    QDesktopWidget &desktopWidget = _allWindows->desktopWidget();

	YerothUtils::getCenterPosition(desktopWidget.width(),
								  desktopWidget.height(),
								  *this,
								  *_currentPosition);

    setupLineEdits();

    setFixedSize(width(), height());

    connect(radioButton_IMPRESSION_VERTICAL,
    		SIGNAL(toggled(bool)),
			this,
			SLOT(SET_NOMBRE_DE_LIGNES_PAR_DEFAUT_after_RADIOBUTTON_TOGGLE()));

    connect(radioButton_IMPRESSION_HORIZONTAL,
    		SIGNAL(toggled(bool)),
			this,
			SLOT(SET_NOMBRE_DE_LIGNES_PAR_DEFAUT_after_RADIOBUTTON_TOGGLE()));


    connect(lineEdit_pageFROM,
    		SIGNAL(textEdited(const QString &)),
			this,
			SLOT(enable_MAX_TABLE_ROW_COUNT()));

    connect(lineEdit_pageTO,
    		SIGNAL(textEdited(const QString &)),
			this,
			SLOT(enable_MAX_TABLE_ROW_COUNT()));


    connect(pushButton_annuler,
    		SIGNAL(clicked()),
			this,
			SLOT(annuler()));

    connect(pushButton_valider,
    		SIGNAL(clicked()),
			this,
			SLOT(valider()));
}


void YerothIMPRESSION_DE_DOCUMENT_Dialog::setupLineEdits()
{
	lineEdit_IMPRESSION_LIGNES->setPalette(YerothUtils::YEROTH_BLACK_PALETTE);
	lineEdit_IMPRESSION_LIGNES->setYerothEnabled(false);

	lineEdit_pageFROM->setValidator(&YerothUtils::IntValidator);
	lineEdit_pageTO->setValidator(&YerothUtils::IntValidator);
}


void YerothIMPRESSION_DE_DOCUMENT_Dialog::show()
{
	SET_NOMBRE_DE_LIGNES_PAR_DEFAUT();

	move(*_currentPosition);

	YerothPOSDialogCommons::show();
}


void YerothIMPRESSION_DE_DOCUMENT_Dialog::SET_NOMBRE_DE_LIGNES_PAR_DEFAUT_after_RADIOBUTTON_TOGGLE()
{
	if (lineEdit_IMPRESSION_LIGNES->isReadOnly())
	{
		if (radioButton_IMPRESSION_VERTICAL->isChecked())
		{
			lineEdit_IMPRESSION_LIGNES->
				setText(QString::number(YerothTableViewPRINT_UTILITIES_TEX_TABLE::
							_STANDARD_INITIAL_MAX_TABLE_ROW_COUNT_A4PORTRAIT));
		}
		else if (radioButton_IMPRESSION_HORIZONTAL->isChecked())
		{
			lineEdit_IMPRESSION_LIGNES->
				setText(QString::number(YerothTableViewPRINT_UTILITIES_TEX_TABLE::
							_STANDARD_INITIAL_MAX_TABLE_ROW_COUNT_A4LANDSCAPE));
		}
	}
	else
	{
		SET_NOMBRE_DE_LIGNES_PAR_DEFAUT();
	}
}


void YerothIMPRESSION_DE_DOCUMENT_Dialog::SET_NOMBRE_DE_LIGNES_PAR_DEFAUT()
{
	if (lineEdit_IMPRESSION_LIGNES->isReadOnly())
	{
		if (radioButton_IMPRESSION_VERTICAL->isChecked())
		{
			lineEdit_IMPRESSION_LIGNES->
				setText(QString::number(YerothTableViewPRINT_UTILITIES_TEX_TABLE::
							_STANDARD_INITIAL_MAX_TABLE_ROW_COUNT_A4PORTRAIT));
		}
		else if (radioButton_IMPRESSION_HORIZONTAL->isChecked())
		{
			lineEdit_IMPRESSION_LIGNES->
				setText(QString::number(YerothTableViewPRINT_UTILITIES_TEX_TABLE::
							_STANDARD_INITIAL_MAX_TABLE_ROW_COUNT_A4LANDSCAPE));
		}
	}
	else
	{
		if (0 != _yeroth_CURRENT_DOCUMENT_PRINT_UTILITIES_TEX_TABLE)
		{
			lineEdit_IMPRESSION_LIGNES->
				setText(QString::number(_yeroth_CURRENT_DOCUMENT_PRINT_UTILITIES_TEX_TABLE->GET_NOMBRE_DE_LIGNES()));
		}
		else
		{
			if (radioButton_IMPRESSION_VERTICAL->isChecked())
			{
				lineEdit_IMPRESSION_LIGNES->
					setText(QString::number(YerothTableViewPRINT_UTILITIES_TEX_TABLE::
								_STANDARD_INITIAL_MAX_TABLE_ROW_COUNT_A4PORTRAIT));
			}
			else if (radioButton_IMPRESSION_HORIZONTAL->isChecked())
			{
				lineEdit_IMPRESSION_LIGNES->
					setText(QString::number(YerothTableViewPRINT_UTILITIES_TEX_TABLE::
								_STANDARD_INITIAL_MAX_TABLE_ROW_COUNT_A4LANDSCAPE));
			}
		}
	}
}


void YerothIMPRESSION_DE_DOCUMENT_Dialog::enable_MAX_TABLE_ROW_COUNT()
{
	QString string_pageFROM = lineEdit_pageFROM->text();

	QString string_pageTO = lineEdit_pageTO->text();

	uint pageFROM = string_pageFROM.toUInt();

	uint pageTO = string_pageTO.toUInt();

	if (!string_pageFROM.isEmpty() &&
		!string_pageTO.isEmpty())
	{
		if (pageFROM >
			pageTO)
		{
			YerothQMessageBox::warning(_current_window_to_print,
					QObject::trUtf8("IMPRESSION, NUMÉRO DE PAGES"),
					QObject::trUtf8("La numéro de la page de départ doit "
									"être inférieur au numéro de la page terminale"));
			return ;
		}

		lineEdit_IMPRESSION_LIGNES->setPalette(YerothUtils::YEROTH_WHITE_PALETTE);
		lineEdit_IMPRESSION_LIGNES->setYerothEnabled(true);
	}
	else
	{
		SET_NOMBRE_DE_LIGNES_PAR_DEFAUT();

		lineEdit_IMPRESSION_LIGNES->setPalette(YerothUtils::YEROTH_BLACK_PALETTE);
		lineEdit_IMPRESSION_LIGNES->setYerothEnabled(false);
	}
}


void YerothIMPRESSION_DE_DOCUMENT_Dialog::valider()
{
	if (0 == _yeroth_CURRENT_DOCUMENT_PRINT_UTILITIES_TEX_TABLE)
	{
		if (0 != _current_window_to_print)
		{
			((YerothWindowsCommons *)_current_window_to_print)->imprimer_pdf_document();
		}
	}

	if (0 != _yeroth_CURRENT_DOCUMENT_PRINT_UTILITIES_TEX_TABLE)
	{
		QString nbre_de_lignes_string = lineEdit_IMPRESSION_LIGNES->text();

		if (radioButton_IMPRESSION_VERTICAL->isChecked())
		{
			_yeroth_CURRENT_DOCUMENT_PRINT_UTILITIES_TEX_TABLE->SET_VERTICAL_PRINT_POSITION();
		}
		else if (radioButton_IMPRESSION_HORIZONTAL->isChecked())
		{
			_yeroth_CURRENT_DOCUMENT_PRINT_UTILITIES_TEX_TABLE->SET_HORIZONTAL_PRINT_POSITION();
		}

		if (!lineEdit_IMPRESSION_LIGNES->isReadOnly())
		{
			_yeroth_CURRENT_DOCUMENT_PRINT_UTILITIES_TEX_TABLE->
				SET_NOMBRE_DE_LIGNES(nbre_de_lignes_string.toUInt());
		}
	}

	rendreInvisible();

	if (0 != _current_window_to_print)
	{
		QString string_pageFROM = lineEdit_pageFROM->text();

		QString string_pageTO = lineEdit_pageTO->text();


		if (string_pageFROM.isEmpty() &&
			string_pageTO.isEmpty())
		{
			_current_window_to_print->_page_from = -1;
			_current_window_to_print->_page_to = -1;

			_current_window_to_print->imprimer_pdf_document_WITH_A_YEROTH_PROGRESS_BAR();
			SET_NOMBRE_DE_LIGNES_PAR_DEFAUT();

			_current_window_to_print->_print_table_row_count =
					lineEdit_IMPRESSION_LIGNES->text().toUInt();
		}
		else
		{
			uint pageFROM = string_pageFROM.toUInt();
			uint pageTO = string_pageTO.toUInt();

			if (pageFROM >
				pageTO)
			{
				YerothQMessageBox::warning(_current_window_to_print,
						QObject::trUtf8("IMPRESSION, NUMÉRO DE PAGES"),
						QObject::trUtf8("La numéro de la page de départ doit "
										"être inférieur au numéro de la page terminale"));

				show();

				return ;
			}

			_current_window_to_print->_page_from = pageFROM;
			_current_window_to_print->_page_to = pageTO;

			_current_window_to_print->_print_table_row_count =
					lineEdit_IMPRESSION_LIGNES->text().toUInt();

			_current_window_to_print->imprimer_pdf_document_WITH_A_YEROTH_PROGRESS_BAR(pageFROM,
																					   pageTO);
		}
	}
}

