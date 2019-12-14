/*
 * yeroth-erp-spinbox.cpp
 *
 *      Author: Xavier NOUMBISSI NOUNDOU, Dipl.-Inf., Ph.D. (ABD)
 */

#include "yeroth-erp-spinbox.hpp"

void YerothSpinBox::setEnabled(bool enabled)
{
	_originalPaletteBeforeMissingInformation = palette();

    QSpinBox::setEnabled(enabled);
}

bool YerothSpinBox::checkField()
{
    if (0 == this->value())
    {
        setPalette(YerothUtils::YEROTH_RED_PALETTE);

        return false;
    }

    setPalette(_originalPaletteBeforeMissingInformation);

    return true;
}

void YerothSpinBox::clearField()
{
    this->clear();

    setPalette(_originalPaletteBeforeMissingInformation);
}

void YerothSpinBox::displayValue(double intValue)
{
    this->setValue(intValue);
    this->setEnabled(false);
}
