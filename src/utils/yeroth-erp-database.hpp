/*
 * yeroth-erp-database.hpp
 *
 *      Author: Dipl.-Inf. Xavier NOUMBISSI NOUNDOU, Ph.D. (ABD)
 */

#ifndef SRC_UTILS_YEROTH_DATABASE_HPP_
#define SRC_UTILS_YEROTH_DATABASE_HPP_

#include "src/include/yeroth-erp-3-0-software.text-configuration.hpp"

#include <QtCore/QString>

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>

#include "src/utils/yeroth-erp-config.hpp"

class QSqlDatabase;
class QSqlError;

class YerothDatabase
{
public:

	YEROTH_CLASS_OPERATORS

	YerothDatabase();

	inline ~ YerothDatabase()
	{
	}

	QString toString() const;

	inline QSqlDatabase &getDatabase()
	{
		return _database;
	}

	inline bool open()
	{
		return _database.open(_db_user_name, _db_user_pwd);
	}

	inline void close()
	{
		_database.close();
	}

	inline bool transaction()
	{
		return _database.transaction();
	}

	inline QSqlDriver *driver() const
	{
		return _database.driver();
	}

	inline bool commit()
	{
		return _database.commit();
	}

	inline QSqlError lastError() const
	{
		return (_database.isValid()) ? _database.lastError() : QSqlError("");
	}

	inline void set_db_type(QString db_type)
	{
		_db_type =  db_type;
	}

	void set_db_name(QString db_name);

	void set_db_ip_address(QString db_ip_address);

	void set_db_user_name(QString db_user_name);

	void set_db_user_pwd(QString db_user_pwd);

	void set_db_connection_options(QString db_connection_options);

	inline QString db_type() const
	{
		return _db_type;
	}

	inline QString db_name() const
	{
		return _db_name;
	}

	inline QString db_ip_address() const
	{
		return _db_ip_address;
	}

	inline QString db_user_name() const
	{
		return _db_user_name;
	}

	inline QString db_user_pwd() const
	{
		return _db_user_pwd;
	}

	inline QString db_connection_options() const
	{
		return _db_connection_options;
	}

private:

	QSqlDatabase _database;

	static QString _db_type;
	static QString _db_name;
	static QString _db_ip_address;
	static QString _db_user_name;
	static QString _db_user_pwd;
	static QString _db_connection_options;

public:

	static const QString MYSQL;

	static const QString QSQLITE;
};

#endif /* SRC_UTILS_YEROTH_DATABASE_HPP_ */
