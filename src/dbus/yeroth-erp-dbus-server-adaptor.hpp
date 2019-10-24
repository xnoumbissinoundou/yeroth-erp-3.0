/*
 * yeroth-erp-dbus-server-adaptor.hpp
 *
 *  Created on: June 2, 2017
 *      Author: Dipl.-Inf. Xavier NOUMBISSI NOUNDOU, Ph.D. (ABD)
 *      Email:  xnoundou7@gmail.com
 */

#ifndef SRC_DBUS_YEROTH_DBUS_SERVER_ADAPTOR_HPP_
#define SRC_DBUS_YEROTH_DBUS_SERVER_ADAPTOR_HPP_

#include <QtCore/QObject>

#include <QtDBus/QDBusAbstractAdaptor>

class YerothDBusServerAdaptor : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.yeroth.server")

public:

	YerothDBusServerAdaptor(QObject *parent);

	virtual ~YerothDBusServerAdaptor();

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
};

#endif /* SRC_DBUS_YEROTH_DBUS_SERVER_ADAPTOR_HPP_ */
