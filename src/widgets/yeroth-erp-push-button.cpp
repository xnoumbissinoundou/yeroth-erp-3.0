/*
 * yeroth-erp-push-button.cpp
 *
 *  Created on: Oct 27, 2015
 *      Author: Dipl.-Inf. Xavier NOUMBISSI NOUNDOU, Ph.D. (ABD)
 *      Email:  xnoundou7@gmail.com
 */

#include "yeroth-erp-push-button.hpp"

#include "src/utils/yeroth-erp-utils.hpp"


void YerothPushButton::enable(const QObject *receiver, const char *f)
{
    QObject::connect(this, SIGNAL(clicked()), receiver, f);
    setEnabled(true);
    setVisible(true);
}

void YerothPushButton::disable(const QObject *receiver, bool buttonVisible /*= false*/)
{
    QObject::disconnect(this, SIGNAL(clicked()), receiver, 0);
    setEnabled(false);
    setVisible(buttonVisible);
}
