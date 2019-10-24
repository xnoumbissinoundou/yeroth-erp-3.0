/*
 * yeroth-erp-text-edit.cpp
 *
 *  Created on: Oct 30, 2015
 *      Author: Dipl.-Inf. Xavier NOUMBISSI NOUNDOU, Ph.D. (ABD)
 *      Email:  xnoundou7@gmail.com
 */

#include "yeroth-erp-text-edit.hpp"

#include "src/utils/yeroth-erp-utils.hpp"



YerothTextEdit::YerothTextEdit(QWidget * parent /* = 0 */)
:QTextEdit(parent)
{
	_originalPaletteBeforeMissingInformation = palette();

	setYerothPOSTextEditDisplayFormat();
}


YerothTextEdit::YerothTextEdit(const QString &text, QWidget *parent)
:QTextEdit(text, parent)
{
	_originalPaletteBeforeMissingInformation = palette();

	setYerothPOSTextEditDisplayFormat();
}


bool YerothTextEdit::checkField()
{
    if (this->toPlainText().isEmpty())
    {
        setPalette(YerothUtils::YEROTH_RED_PALETTE);

        return false;
    }

    setPalette(_originalPaletteBeforeMissingInformation);

    return true;
}


void YerothTextEdit::setYerothPOSTextEditDisplayFormat()
{
	if (isEnabled())
	{
		setTextColor(YerothUtils::YEROTH_BLACK_COLOR);

		setPalette(YerothUtils::YEROTH_WHITE_PALETTE);
	}
	else
	{
		setTextColor(YerothUtils::YEROTH_WHITE_COLOR);

		setPalette(YerothUtils::YEROTH_BLACK_PALETTE);
	}
}


void YerothTextEdit::setVisible(bool visible)
{
	QTextEdit::setVisible(visible);

	setYerothPOSTextEditDisplayFormat();
}


void YerothTextEdit::clearField()
{
	QTextEdit::clear();

	setPalette(_originalPaletteBeforeMissingInformation);

	setYerothPOSTextEditDisplayFormat();
}


void YerothTextEdit::setEnabled(bool enabled)
{
    QTextEdit::setEnabled(enabled);

    setYerothPOSTextEditDisplayFormat();

    YerothUtils::yerothSetWidgetColor(this);
}
