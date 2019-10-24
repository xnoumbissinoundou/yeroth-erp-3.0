/*
 * yeroth-erp-style.hpp
 *
 *  Created on: January 24, 2018
 *      Author: Dipl.-Inf. Xavier NOUMBISSI NOUNDOU, Ph.D. (ABD)
 *      Email:  xnoundou7@gmail.com
 */

#ifndef SRC_YEROTH_ERP_3_0_STYLE_HPP_
#define SRC_YEROTH_ERP_3_0_STYLE_HPP_

#include "src/include/yeroth-erp-3-0-software.text-configuration.hpp"

#include <cstdlib>

class QColor;
class QString;

class YerothPosStyle
{
public:

	YEROTH_CLASS_OPERATORS

	static QString getColorStyleSheetString(const QColor &aColour);

private:

	YerothPosStyle();

	~YerothPosStyle();
};

#endif /* SRC_YEROTH_ERP_3_0_STYLE_HPP_ */
