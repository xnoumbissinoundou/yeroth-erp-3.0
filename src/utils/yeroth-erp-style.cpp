/*
 * yeroth-erp-style.cpp
 *
 *      Author: Xavier NOUMBISSI NOUNDOU, Dipl.-Inf., Ph.D. (ABD)
 */
#include "src/utils/yeroth-erp-style.hpp"

#include <QtCore/QString>
#include <QtGui/QColor>

QString YerothPosStyle::getColorStyleSheetString(const QColor &aColour)
{
    return QString("QLabel { color : %1; }").arg(aColour.name());
}
