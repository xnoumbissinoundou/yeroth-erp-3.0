/*
 * yeroth-erp-push-button.cpp
 *
 *      Author: DIPL.-INF. XAVIER NOUMBISSI NOUNDOU
 */

#include "yeroth-erp-push-button.hpp"

#include "src/utils/yeroth-erp-utils.hpp"


void YerothPushButton::enable(const QObject *receiver, const char *f)
{
    QObject::connect(this, SIGNAL(clicked()), receiver, f, Qt::UniqueConnection);
    setEnabled(true);
    setVisible(true);
}

void YerothPushButton::disable(const QObject *receiver, bool buttonVisible /*= false*/)
{
    QObject::disconnect(this, SIGNAL(clicked()), receiver, 0);
    setEnabled(false);
    setVisible(buttonVisible);
}
