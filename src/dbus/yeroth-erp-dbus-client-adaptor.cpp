/*
 * yeroth-erp-dbus-client-adaptor.cpp
 *
 *      Author: Xavier NOUMBISSI NOUNDOU, Dipl.-Inf., Ph.D. (ABD)
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
