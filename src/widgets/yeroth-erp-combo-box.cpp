/*
 * yeroth-erp-combo-box.cpp
 *
 *      Author: Xavier NOUMBISSI NOUNDOU, Dipl.-Inf., Ph.D. (ABD)
 */

#include "yeroth-erp-combo-box.hpp"

#include "src/widgets/yeroth-erp-line-edit.hpp"

YerothComboBox::YerothComboBox(QWidget *parent)
:QComboBox(parent)
{
	_originalPaletteBeforeMissingInformation = palette();

    setLineEdit(new QLineEdit);

    lineEdit()->setReadOnly(true);

    lineEdit()->setAlignment(Qt::AlignHCenter | Qt::AlignCenter);
}

bool YerothComboBox::checkField()
{
    if (currentText().isEmpty())
    {
        setPalette(YerothUtils::YEROTH_RED_PALETTE);

        return false;
    }

    setPalette(_originalPaletteBeforeMissingInformation);

    return true;
}

void YerothComboBox::clearField()
{
    QComboBox::clear();

    setPalette(_originalPaletteBeforeMissingInformation);
}

void YerothComboBox::setYerothEnabled(bool enabled)
{
    QComboBox::setEnabled(enabled);

    YerothUtils::yerothSetWidgetColor(this);
}
