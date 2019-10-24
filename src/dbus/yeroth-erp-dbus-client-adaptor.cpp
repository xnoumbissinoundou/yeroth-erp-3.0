/*
 * yeroth-erp-dbus-client-adaptor.cpp
 *
 *  Created on: June 27, 2017
 *      Author: Dipl.-Inf. Xavier NOUMBISSI NOUNDOU, Ph.D. (ABD)
 *      Email:  xnoundou7@gmail.com
 */

#include "yeroth-erp-dbus-client-adaptor.hpp"


YerothDBusClientAdaptor::YerothDBusClientAdaptor(QObject *parent)
    :QDBusAbstractAdaptor(parent)
{
    setAutoRelaySignals(true);
}

YerothDBusClientAdaptor::~YerothDBusClientAdaptor()
{
}

bool YerothDBusClientAdaptor::slot_refresh_app_parameters()
{
    // handle method call com.magelan.client.slot_refresh_app_parameters
    return QMetaObject::invokeMethod(parent(), "slot_refresh_app_parameters");
}
