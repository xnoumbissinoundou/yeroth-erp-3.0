/*
 * yeroth-erp-dbus-client.hpp
 *
 *      Author: PR. XAVIER NOUMBISSI NOUNDOU
 */

#ifndef SRC_DBUS_YEROTH_DBUS_CLIENT_HPP_
#define SRC_DBUS_YEROTH_DBUS_CLIENT_HPP_

#include <QtCore/QObject>

class QDBusInterface;

class YerothERPWindows;

class YerothDBusClient : public QObject
{
	Q_OBJECT

public:

	YerothDBusClient(YerothERPWindows *allWindows);

	~YerothDBusClient();

	bool connect_to_dbus_server();

	inline void *operator new[](size_t size)
	{
		return malloc(size);
	}

	inline void *operator new(size_t size)
	{
		return malloc(size);
	}

	inline void operator delete[](void *ptr)
	{
		free(ptr);
	}

	inline void operator delete[](void *ptr, size_t)
	{
		free(ptr);
	}

	inline void operator delete(void *ptr)
	{
		free(ptr);
	}

	inline void operator delete(void *ptr, size_t)
	{
		free(ptr);
	}

public slots:

	  bool slot_refresh_app_parameters();

private:

	  QDBusInterface	*_dbusServerInterface;

	  YerothERPWindows 		*_allWindows;
};

#endif /* SRC_DBUS_YEROTH_DBUS_CLIENT_HPP_ */
