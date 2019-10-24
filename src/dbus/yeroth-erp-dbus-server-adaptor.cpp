/*
 * yeroth-erp-dbus-server-adaptor.cpp
 *
 *  Created on: July 2, 2017
 *      Author: Dipl.-Inf. Xavier NOUMBISSI NOUNDOU, Ph.D. (ABD)
 *      Email:  xnoundou7@gmail.com
 */

#include "yeroth-erp-dbus-server-adaptor.hpp"

YerothDBusServerAdaptor::YerothDBusServerAdaptor(QObject *parent)
    :QDBusAbstractAdaptor(parent)
{
    setAutoRelaySignals(true);
}

YerothDBusServerAdaptor::~YerothDBusServerAdaptor()
{
}
