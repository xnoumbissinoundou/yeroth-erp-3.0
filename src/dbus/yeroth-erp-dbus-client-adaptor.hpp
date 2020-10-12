/*
 * yeroth-erp-dbus-client-adaptor.hpp
 *
 *      Author: DIPL.-INF. XAVIER NOUMBISSI NOUNDOU
 */

#ifndef SRC_DBUS_YEROTH_DBUS_CLIENT_ADAPTOR_HPP_
#define SRC_DBUS_YEROTH_DBUS_CLIENT_ADAPTOR_HPP_

#include <QtCore/QObject>

#include <QtDBus/QDBusAbstractAdaptor>

class YerothDBusClientAdaptor : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.yeroth.client")

public:

	YerothDBusClientAdaptor(QObject *parent);

	virtual ~YerothDBusClientAdaptor();

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

public Q_SLOTS:

	bool slot_refresh_app_parameters();
};

#endif /* SRC_DBUS_YEROTH_DBUS_CLIENT_ADAPTOR_HPP_ */
