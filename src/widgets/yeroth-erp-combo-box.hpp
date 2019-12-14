/*
 * yeroth-erp-combo-box.hpp
 *
 *      Author: Xavier NOUMBISSI NOUNDOU, Dipl.-Inf., Ph.D. (ABD)
 */

#ifndef YEROTH_COMBO_BOX_HPP_
#define YEROTH_COMBO_BOX_HPP_

#include <QtWidgets/QComboBox>

#include "src/utils/yeroth-erp-utils.hpp"

class YerothUtils;

class YerothComboBox : public QComboBox
{

public:

	YEROTH_CLASS_OPERATORS

	YerothComboBox(QWidget *parent);

	inline ~YerothComboBox(){}

	bool checkField();

	void clearField();

	inline QString currentText() const
	{
		return QComboBox::currentText().trimmed();
	}

	void setYerothEnabled(bool enabled);

private:

	QPalette _originalPaletteBeforeMissingInformation;
};


#endif /* YEROTH_COMBO_BOX_HPP_ */
