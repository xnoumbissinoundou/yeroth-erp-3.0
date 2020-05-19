/*
 * yeroth-erp-logger.cpp
 *
 *      Author: Xavier NOUMBISSI NOUNDOU, Dipl.-Inf., Ph.D. (ABD)
 */

#include "yeroth-erp-logger.hpp"

#include "src/utils/yeroth-erp-utils.hpp"

#include <QtCore/QDebug>
#include <QtCore/QFile>

//#define YEROTH_DEBUG_LOG


QDebug 	*YerothLogger::_qDebug(0);

QFile 	*YerothLogger::_logFile(0);


YerothLogger::YerothLogger(QString cppClassName, YEROTH_LOG_LEVEL logLevel)
:_logLevel(logLevel),
 _cppClassName(cppClassName)
{
	static bool alreadyInstantiated = false;

	if (!alreadyInstantiated)
	{
		_logFile = new QFile(YerothUtils::getLogFileName());

	    _qDebug = new QDebug(_logFile);

	    _logFile->open(QIODevice::WriteOnly | QIODevice::Append);

	    alreadyInstantiated = true;
	}
}

YerothLogger::~YerothLogger()
{
	static bool alreadyDestroyed = false;

	if (!alreadyDestroyed)
	{
		_logFile->close();

	    delete _qDebug;

	    alreadyDestroyed = true;
	}
}

void YerothLogger::debug(const char *cppMethodName, const char *msg)
{
    *_qDebug << QString("DEBUG | %1 %2 | %3 | %4 |")
             .arg(GET_CURRENT_STRING_DATE,
                  CURRENT_TIME,
                  _cppClassName,
                  cppMethodName).toStdString().c_str()
             << msg << "\n";

#ifdef YEROTH_DEBUG_LOG
    qDebug() << QString("DEBUG | %1 %2 | %3 | %4 |")
             .arg(GET_CURRENT_STRING_DATE,
                  CURRENT_TIME,
                  _cppClassName,
                  cppMethodName).toStdString().c_str()
             << msg;
#endif
}

void YerothLogger::debug(const char *cppMethodName, QString msg)
{
    *_qDebug << QString("DEBUG | %1 %2 | %3 | %4 |")
             .arg(GET_CURRENT_STRING_DATE,
                  CURRENT_TIME,
                  _cppClassName,
                  cppMethodName).toStdString().c_str()
             << msg << "\n";

#ifdef YEROTH_DEBUG_LOG
    qDebug() << QString("DEBUG | %1 %2 | %3 | %4 |")
             .arg(GET_CURRENT_STRING_DATE,
                  CURRENT_TIME,
                  _cppClassName,
                  cppMethodName).toStdString().c_str()
             << msg;
#endif
}

void YerothLogger::log(const char *cppMethodName, const char *msg)
{
    *_qDebug << QString("LOG | %1 %2 | %3 | %4 |")
             .arg(GET_CURRENT_STRING_DATE,
                  CURRENT_TIME,
                  _cppClassName,
                  cppMethodName).toStdString().c_str()
             << msg << "\n";

#ifdef YEROTH_DEBUG_LOG
    qDebug() << QString("LOG | %1 %2 | %3 | %4 |")
             .arg(GET_CURRENT_STRING_DATE,
                  CURRENT_TIME,
                  _cppClassName,
                  cppMethodName).toStdString().c_str()
             << msg;
#endif
}

void YerothLogger::log(const char *cppMethodName, const QString msg)
{
    *_qDebug << QString("LOG | %1 %2 | %3 | %4 |")
             .arg(GET_CURRENT_STRING_DATE,
                  CURRENT_TIME,
                  _cppClassName,
                  cppMethodName).toStdString().c_str()
             << msg << "\n";

#ifdef YEROTH_DEBUG_LOG
    qDebug() << QString("LOG | %1 %2 | %3 | %4 |")
             .arg(GET_CURRENT_STRING_DATE,
                  CURRENT_TIME,
                  _cppClassName,
                  cppMethodName).toStdString().c_str()
             << msg;
#endif
}

YerothLogger &YerothLogger::operator<< (const char *s)
{
    *_qDebug << s;
    return *this;
}

YerothLogger &YerothLogger::operator<< (QString s)
{
    *_qDebug << s;
    return *this;
}

YerothLogger &YerothLogger::operator<< (bool s)
{
    *_qDebug << s;
    return *this;
}

YerothLogger &YerothLogger::operator<< (int s)
{
    *_qDebug << s;
    return *this;
}

YerothLogger &YerothLogger::operator<< (char s)
{
    *_qDebug << s;
    return *this;
}

YerothLogger &YerothLogger::operator<< (QByteArray s)
{
    *_qDebug << s;
    return *this;
}

