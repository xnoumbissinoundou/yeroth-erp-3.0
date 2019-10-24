/*
 * yeroth-erp-spinbox.hpp
 *
 *  Created on: Dec 1, 2015
 *      Author: Dipl.-Inf. Xavier NOUMBISSI NOUNDOU, Ph.D. (ABD)
 *      Email:  xnoundou7@gmail.com
 */

#ifndef YEROTH_SPINBOX_HPP_
#define YEROTH_SPINBOX_HPP_

#include "src/include/yeroth-erp-3-0-software.text-configuration.hpp"

#include <QtWidgets/QSpinBox>

#include "src/utils/yeroth-erp-utils.hpp"

class YerothUtils;

class YerothSpinBox : public QSpinBox
{
public:

	YEROTH_CLASS_OPERATORS

	inline YerothSpinBox(QWidget *parent = 0)
	:QSpinBox(parent)
	{
		YerothUtils::yerothSetWidgetColor(this);
	}

	inline ~YerothSpinBox()
	{
	}

	bool checkField();

	void clearField();

	void displayValue(double intValue);

	inline double valueMultiplyBy(double aValue)
	{
		return this->value() * aValue;
	}

	void setEnabled(bool enabled);

private:

	QPalette _originalPaletteBeforeMissingInformation;
};


#endif /* YEROTH_SPINBOX_HPP_ */
