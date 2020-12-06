/*
 * yeroth-erp-dbus-server.cpp
 *
 *      Author: DR. XAVIER NOUMBISSI NOUNDOU
 */

#include "yeroth-erp-dbus-server.hpp"
#include "yeroth-erp-dbus-server-adaptor.hpp"

#include "src/yeroth-erp-windows.hpp"

#include <QtCore/QDebug>

#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusInterface>

YerothDBusServer::YerothDBusServer(YerothERPWindows *allWindows)
    :_dbusClientInterface(0),
     _allWindows(allWindows)
{
    this->connect_to_dbus_client();
}

YerothDBusServer::~YerothDBusServer()
{
}

bool YerothDBusServer::connect_to_dbus_client()
{
    qDebug() << "++ YerothDBusServer::connect_to_dbus_client()";

    //This object is automatically destroyed after the QApplication
    //has terminated.
    new YerothDBusServerAdaptor(this);

    QDBusConnection serverConnection = QDBusConnection::systemBus();

    if (!serverConnection.registerObject(YerothUtils::MAGELAN_SERVER_OBJECT, this))
    {
        qDebug() << QString("++ YerothDBusServer::connect_to_dbus_client() | server | "
                            "could not register '%1' object")
                 .arg(YerothUtils::MAGELAN_SERVER_OBJECT);

        qDebug() << "++ YerothDBusServer::connect_to_dbus_client() | server | last error: "
                 << serverConnection.lastError();
    }
    else
    {
        qDebug() << QString("++ YerothDBusServer::connect_to_dbus_client() | server | "
                            "could register '%1' object")
                 .arg(YerothUtils::MAGELAN_SERVER_OBJECT);
    }

    if (!serverConnection.registerService(YerothUtils::MAGELAN_SERVER))
    {
        qDebug() << QString("++ YerothDBusServer::connect_to_dbus_client() | server | "
                            "could not register '%1' service")
                 .arg(YerothUtils::MAGELAN_SERVER);

        qDebug() << "++ YerothDBusServer::connect_to_dbus_client() | server | "
                 "last error: " << serverConnection.lastError();
    }
    else
    {
        qDebug() << QString("++ YerothDBusServer::connect_to_dbus_client() | server | "
                            "could register '%1' service")
                 .arg(YerothUtils::MAGELAN_SERVER);
    }

    /*
     * Le code ci-dessous resout le cas ou les applications clientes
     * ont ete demarre avant l'application serveur. Dans ce cas, le serveur
     * se connecte aux clients pendant son demarrage.
     */
    _dbusClientInterface =
        new QDBusInterface(YerothUtils::MAGELAN_CLIENT,
                           YerothUtils::MAGELAN_CLIENT_OBJECT,
                           YerothUtils::MAGELAN_CLIENT,
                           serverConnection);

    if (_dbusClientInterface && _dbusClientInterface->isValid())
    {
        bool _dbus_client_refresh_connectedto_server =
            QObject::connect(this,
                             SIGNAL(signal_refresh_app_parameters()),
                             _dbusClientInterface,
                             SLOT(slot_refresh_app_parameters()));

        if (!_dbus_client_refresh_connectedto_server)
        {
            qDebug() << "there is no refresh parameters "
                     "signal connection to the server";
        }
        else
        {
            qDebug() << "++ YerothDBusServer::connect_to_dbus_client(), signal connection, true";

            return true;
        }
    }

    qDebug() << "++ YerothDBusServer::connect_to_dbus_client(), false";

    return false;
}
