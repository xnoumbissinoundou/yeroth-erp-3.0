/*
 * yeroth-erp-logger.cpp
 *
 *      Author: Dipl.-Inf. XAVIER NOUMBISSI NOUNDOU
 */

#include "yeroth-erp-logger.hpp"

#include "src/yeroth-erp-windows.hpp"

#include "src/utils/yeroth-erp-utils.hpp"

#include "src/users/yeroth-erp-users.hpp"


#include <QtCore/QDebug>
#include <QtCore/QFile>


QDebug 	*YerothLogger::_qDebug(0);

QFile 	*YerothLogger::_logFile(0);

YerothERPWindows 	*YerothLogger::_allWindows(0);


YerothLogger::YerothLogger(QString cppClassName, YEROTH_LOG_LEVEL logLevel)
:_logLevel(logLevel),
 _cppClassName(cppClassName),
 _curYEROTHERPUser(0)
{
	static bool alreadyInstantiated = false;

	if (!alreadyInstantiated)
	{
		_logFile = new QFile(YerothUtils::getLogFileName());

	    _qDebug = new QDebug(_logFile);

	    _allWindows = YerothUtils::getAllWindows();

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


void YerothLogger::renewCurrentYEROTHERPUser()
{
	_curUserUtilisateur.clear();

	_curUserNomComplet.clear();

	_curUserUtilisateur.append(YerothUtils::UTILISATEUR_NON_EXISTANT);

	_curUserNomComplet.append(YerothUtils::UTILISATEUR_NON_EXISTANT);

	if (0 == _allWindows)
	{
		_allWindows = YerothUtils::getAllWindows();
	}

	if (0 != _allWindows && !_allWindows->CURRENTLY_CHANGING_USER)
	{
		_curYEROTHERPUser = _allWindows->getUser();
	}

	if (0 != _curYEROTHERPUser && !_allWindows->CURRENTLY_CHANGING_USER)
	{
		_curUserUtilisateur = _curYEROTHERPUser->nom_utilisateur();

		_curUserNomComplet = _curYEROTHERPUser->nom_complet();
	}
}


void YerothLogger::debug(const char *cppMethodName, const char *msg)
{
	renewCurrentYEROTHERPUser();

    *_qDebug << QString("DEBUG | %1 \% %2 | %3 %4 | %5 | %6 |")
             .arg(_curUserNomComplet,
            	  _curUserUtilisateur,
            	  GET_CURRENT_STRING_DATE,
                  CURRENT_TIME,
                  _cppClassName,
                  cppMethodName).toStdString().c_str()
             << msg << "\n";

#ifdef YEROTH_DEBUG_LOG
    qDebug() << QString("DEBUG | %1 \% %2 | %3 %4 | %5 | %6 |")
             .arg(_curUserNomComplet,
               	  _curUserUtilisateur,
            	  GET_CURRENT_STRING_DATE,
                  CURRENT_TIME,
                  _cppClassName,
                  cppMethodName).toStdString().c_str()
             << msg;
#endif
}


void YerothLogger::debug(const char *cppMethodName, QString msg)
{
	renewCurrentYEROTHERPUser();

    *_qDebug << QString("DEBUG | %1 \% %2 | %3 %4 | %5 | %6 |")
             .arg(_curUserNomComplet,
               	  _curUserUtilisateur,
            	  GET_CURRENT_STRING_DATE,
                  CURRENT_TIME,
                  _cppClassName,
                  cppMethodName).toStdString().c_str()
             << msg << "\n";

#ifdef YEROTH_DEBUG_LOG
    qDebug() << QString("DEBUG | %1 \% %2 | %3 %4 | %5 | %6 |")
             .arg(_curUserNomComplet,
               	  _curUserUtilisateur,
            	  GET_CURRENT_STRING_DATE,
                  CURRENT_TIME,
                  _cppClassName,
                  cppMethodName).toStdString().c_str()
             << msg;
#endif
}


void YerothLogger::log(const char *cppMethodName, const char *msg)
{
	renewCurrentYEROTHERPUser();

    *_qDebug << QString("LOG | %1 \% %2 | %3 %4 | %5 | %6 |")
             .arg(_curUserNomComplet,
               	  _curUserUtilisateur,
            	  GET_CURRENT_STRING_DATE,
                  CURRENT_TIME,
                  _cppClassName,
                  cppMethodName).toStdString().c_str()
             << msg << "\n";

#ifdef YEROTH_DEBUG_LOG
    qDebug() << QString("LOG | %1 \% %2 | %3 %4 | %5 | %6 |")
             .arg(_curUserNomComplet,
               	  _curUserUtilisateur,
            	  GET_CURRENT_STRING_DATE,
                  CURRENT_TIME,
                  _cppClassName,
                  cppMethodName).toStdString().c_str()
             << msg;
#endif
}


void YerothLogger::log(const char *cppMethodName, const QString msg)
{
	renewCurrentYEROTHERPUser();

    *_qDebug << QString("LOG | %1 \% %2 | %3 %4 | %5 | %6 |")
             .arg(_curUserNomComplet,
               	  _curUserUtilisateur,
            	  GET_CURRENT_STRING_DATE,
                  CURRENT_TIME,
                  _cppClassName,
                  cppMethodName).toStdString().c_str()
             << msg << "\n";

#ifdef YEROTH_DEBUG_LOG
    qDebug() << QString("LOG | %1 \% %2 | %3 %4 | %5 | %6 |")
             .arg(_curUserNomComplet,
               	  _curUserUtilisateur,
            	  GET_CURRENT_STRING_DATE,
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

