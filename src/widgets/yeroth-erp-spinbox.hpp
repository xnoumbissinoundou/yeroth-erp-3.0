/*
 * yeroth-erp-spinbox.hpp
 *
 *      Author: PR. XAVIER NOUMBISSI NOUNDOU
 */

#ifndef YEROTH_SPINBOX_HPP_
#define YEROTH_SPINBOX_HPP_

#include "src/include/yeroth-erp-3-0-software.text-configuration.hpp"


#include <QtWidgets/QDoubleSpinBox>


#include "src/utils/yeroth-erp-utils.hpp"

class YerothUtils;

class YerothSpinBox : public QDoubleSpinBox
{
public:

	YEROTH_CLASS_OPERATORS

	inline YerothSpinBox(QWidget *parent = 0)
	:QDoubleSpinBox(parent)
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
		return  value() * aValue;
	}

	void setEnabled(bool enabled);

private:

	QPalette _originalPaletteBeforeMissingInformation;
};


#endif /* YEROTH_SPINBOX_HPP_ */
