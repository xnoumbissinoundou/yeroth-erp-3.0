/*
 * yeroth-erp-dbus-server.hpp
 *
 *      Author: PR. XAVIER NOUMBISSI NOUNDOU
 */

#ifndef SRC_DBUS_YEROTH_DBUS_SERVER_HPP_
#define SRC_DBUS_YEROTH_DBUS_SERVER_HPP_

#include "src/utils/yeroth-erp-utils.hpp"

#include <QtCore/QObject>

class YerothUtils;
class YerothERPWindows;

class QDBusInterface;

class YerothDBusServer : public QObject
{
	Q_OBJECT

public:

	 YerothDBusServer(YerothERPWindows *allWindows);

	~YerothDBusServer();

	bool connect_to_dbus_client();

	inline void emit_signal_refresh_app_parameters()
	{
		emit signal_refresh_app_parameters();
	}

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

Q_SIGNALS:

	void signal_refresh_app_parameters();

	void signal_server_started();

private:

	QDBusInterface	*_dbusClientInterface;

	YerothERPWindows 	*_allWindows;
};

#endif /* SRC_DBUS_YEROTH_DBUS_SERVER_HPP_ */
