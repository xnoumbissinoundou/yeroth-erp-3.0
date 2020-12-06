/*
 * yeroth-erp-logger.hpp
 *
 *      Author: DR. XAVIER NOUMBISSI NOUNDOU
 */

#ifndef SRC_YEROTH_LOGGER_HPP_
#define SRC_YEROTH_LOGGER_HPP_


#include "src/include/yeroth-erp-3-0-software.text-configuration.hpp"


#include <QtCore/QString>


class QDebug;
class QFile;


class YerothPOSUser;
class YerothERPWindows;


class YerothLogger
{
public:

	YEROTH_CLASS_OPERATORS

	typedef enum _YEROTH_LOG_LEVEL
	{
		YEROTH_DEBUG = 0,
		YEROTH_LOG,
		YEROTH_WARN

	} YEROTH_LOG_LEVEL;

	YerothLogger(QString cppClassName, YEROTH_LOG_LEVEL logLevel = YEROTH_LOG);

	~YerothLogger();

	void renewCurrentYEROTHERPUser();

	void debug(const char *cppMethodName, const char *msg = "");

	void debug(const char *cppMethodName, QString msg);

	void log(const char *cppMethodName, const char *msg = "");

	void log(const char *cppMethodName, QString msg);

	YerothLogger &operator<< (const char *s);
	YerothLogger &operator<< (QString s);
	YerothLogger &operator<< (bool s);
	YerothLogger &operator<< (int s);
	YerothLogger &operator<< (char s);
	YerothLogger &operator<< (QByteArray s);

private:

	YEROTH_LOG_LEVEL 		_logLevel;

	QString					_curUserUtilisateur;

	QString					_curUserNomComplet;

	QString 				_cppClassName;

	YerothPOSUser 			*_curYEROTHERPUser;

	static QDebug  			*_qDebug;

	static QFile 			*_logFile;

	static YerothERPWindows *_allWindows;
};


#endif //SRC_YEROTH_LOGGER_HPP_
