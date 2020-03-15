/*
 * yeroth-erp-spinbox.cpp
 *
 *      Author: Xavier NOUMBISSI NOUNDOU, Dipl.-Inf., Ph.D. (ABD)
 */


#include "yeroth-erp-spinbox.hpp"


void YerothSpinBox::setEnabled(bool enabled)
{
	_originalPaletteBeforeMissingInformation = palette();

	QDoubleSpinBox::setEnabled(enabled);
}

bool YerothSpinBox::checkField()
{
    if (0 == value())
    {
        setPalette(YerothUtils::YEROTH_RED_PALETTE);

        return false;
    }

    setPalette(_originalPaletteBeforeMissingInformation);

    return true;
}

void YerothSpinBox::clearField()
{
    clear();

    setPalette(_originalPaletteBeforeMissingInformation);
}

void YerothSpinBox::displayValue(double intValue)
{
    setValue(intValue);
    setEnabled(false);
}
