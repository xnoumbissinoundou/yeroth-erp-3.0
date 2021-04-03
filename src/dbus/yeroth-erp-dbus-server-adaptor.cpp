/*
 * yeroth-erp-dbus-server-adaptor.cpp
 *
 *      Author: PR. XAVIER NOUMBISSI NOUNDOU
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
