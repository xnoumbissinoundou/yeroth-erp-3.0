/*
 * yeroth-erp-style.cpp
 *
 *  Created on: January 24, 2018
 *      Author: Dipl.-Inf. Xavier NOUMBISSI NOUNDOU, Ph.D. (ABD)
 *      Email:  xnoundou7@gmail.com
 */
#include "src/utils/yeroth-erp-style.hpp"

#include <QtCore/QString>
#include <QtGui/QColor>

QString YerothPosStyle::getColorStyleSheetString(const QColor &aColour)
{
    return QString("QLabel { color : %1; }").arg(aColour.name());
}
