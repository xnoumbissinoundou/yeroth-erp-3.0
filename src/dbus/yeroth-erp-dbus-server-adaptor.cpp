/*
 * yeroth-erp-dbus-server-adaptor.cpp
 *
 *      Author: Dipl.-Inf. XAVIER NOUMBISSI NOUNDOU
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
