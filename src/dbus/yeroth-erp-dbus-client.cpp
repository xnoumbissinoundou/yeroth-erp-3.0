/*
 * yeroth-erp-dbus-client.cpp
 *
 *      Author: DR. XAVIER NOUMBISSI NOUNDOU
 */

#include "yeroth-erp-dbus-client.hpp"
#include "yeroth-erp-dbus-client-adaptor.hpp"

#include "src/utils/yeroth-erp-config.hpp"
#include "src/utils/yeroth-erp-utils.hpp"

#include "src/yeroth-erp-windows.hpp"

#include <QtCore/QDebug>

#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusError>
#include <QtDBus/QDBusInterface>

#include <QtWidgets/QMessageBox>

YerothDBusClient::YerothDBusClient(YerothERPWindows *allWindows)
    :_dbusServerInterface(0),
     _allWindows(allWindows)
{
    this->connect_to_dbus_server();
}

YerothDBusClient::~YerothDBusClient()
{
}

bool YerothDBusClient::connect_to_dbus_server()
{
    //This object is automatically destroyed after the QApplication
    //has terminated.
    new YerothDBusClientAdaptor(this);

    QDBusConnection clientConnection = QDBusConnection::systemBus();

    bool couldRegisterObject = false;

    bool couldRegisterService = false;

    if (!clientConnection.registerObject(YerothUtils::MAGELAN_CLIENT_OBJECT, this))
    {
        qDebug() << QString("++ YerothDBusClient::connect_to_dbus_server() | client | "
                            "could not register '%1' object")
                 .arg(YerothUtils::MAGELAN_CLIENT_OBJECT);

        qDebug() << "++ YerothDBusClient::connect_to_dbus_server() | client | last error: "
                 << clientConnection.lastError();

        couldRegisterObject = false;
    }
    else
    {
        qDebug() << QString("++ YerothDBusClient::connect_to_dbus_server() | client | "
                            "could register '%1' object")
                 .arg(YerothUtils::MAGELAN_CLIENT_OBJECT);

        couldRegisterObject = true;
    }

    if (!clientConnection.registerService(YerothUtils::MAGELAN_CLIENT))
    {
        qDebug() << QString("++ YerothDBusClient::connect_to_dbus_server() | client | "
                            "could not register '%1' service")
                 .arg(YerothUtils::MAGELAN_CLIENT);

        qDebug() << "++ YerothDBusClient::connect_to_dbus_server() | client | last error: "
                 << clientConnection.lastError();

        couldRegisterService = false;
    }
    else
    {
        qDebug() << QString("++ YerothDBusClient::connect_to_dbus_server() | client | "
                            "could register '%1' service")
                 .arg(YerothUtils::MAGELAN_CLIENT);

        couldRegisterService = true;
    }

    if (couldRegisterObject && couldRegisterService)
    {
        _dbusServerInterface =
            new QDBusInterface(YerothUtils::MAGELAN_SERVER,
                               YerothUtils::MAGELAN_SERVER_OBJECT,
                               YerothUtils::MAGELAN_SERVER,
                               clientConnection);

        if (_dbusServerInterface && _dbusServerInterface->isValid())
        {
            bool _dbus_client_refresh_connectedto_server =
                QObject::connect(_dbusServerInterface,
                                 SIGNAL(signal_refresh_app_parameters()),
                                 this,
                                 SLOT(slot_refresh_app_parameters()));

            if (!_dbus_client_refresh_connectedto_server)
            {
                qDebug() << "there is no refresh parameters "
                         "signal connection to the server";
            }
            else
            {
                qDebug() << "++ YerothDBusClient::connect_to_dbus_server(), signal connection, true";

                return true;
            }
        }
    }

    return false;
}

bool YerothDBusClient::slot_refresh_app_parameters()
{
    if (_allWindows)
    {
        YerothSqlTableModel &configurationsTableModel =
            _allWindows->getSqlTableModel_configurations();

        QSqlRecord configurationRecord = configurationsTableModel.record(YerothERPConfig::CONFIG_ALERT_PERIOD_TIME_INTERVAL);
        QString alertPeriodTimeIntervalValue(GET_SQL_RECORD_DATA(configurationRecord, "valeur_configuration"));

        configurationRecord = configurationsTableModel.record(YerothERPConfig::CONFIG_ALERT_QUANTITY_TIME_INTERVAL);
        QString alertQuantityTimeIntervalValue(GET_SQL_RECORD_DATA(configurationRecord, "valeur_configuration"));

        configurationRecord = configurationsTableModel.record(YerothERPConfig::CONFIG_TVA_VALUE);
        QString tvaValue(GET_SQL_RECORD_DATA(configurationRecord, "valeur_configuration"));

        configurationRecord = configurationsTableModel.record(YerothERPConfig::CONFIG_SALES_STRATEGY);
        QString salesStrategyValue(GET_SQL_RECORD_DATA(configurationRecord, "valeur_configuration"));

        YerothERPConfig::alert_period_time_interval = alertPeriodTimeIntervalValue.toUInt();
        YerothERPConfig::alert_quantity_time_interval = alertQuantityTimeIntervalValue.toUInt();
        YerothERPConfig::tva_value = tvaValue.toDouble();
        YerothERPConfig::salesStrategy = salesStrategyValue;

        return true;
    }

    return false;
}
