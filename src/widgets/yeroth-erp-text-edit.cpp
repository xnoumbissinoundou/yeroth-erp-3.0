/*
 * yeroth-erp-text-edit.cpp
 *
 *      Author: PR. XAVIER NOUMBISSI NOUNDOU
 */

#include "yeroth-erp-text-edit.hpp"

#include "src/utils/yeroth-erp-utils.hpp"


bool YerothTextEdit::checkField()
{
    if ( toPlainText().isEmpty())
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


void YerothTextEdit::setYerothEnabled(bool enabled)
{
    QTextEdit::setEnabled(enabled);

    setYerothPOSTextEditDisplayFormat();

    YerothUtils::yerothSetWidgetColor(this);
}
