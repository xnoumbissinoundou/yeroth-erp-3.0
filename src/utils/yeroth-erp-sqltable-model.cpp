/*
 * yeroth-erp-sqltable-model.cpp
 *
  *      Author: Dipl.-Inf. XAVIER NOUMBISSI NOUNDOU
 */

#include "yeroth-erp-sqltable-model.hpp"

#include "src/utils/yeroth-erp-utils.hpp"

#include "src/utils/yeroth-erp-logger.hpp"

#include <QtCore/QDebug>

#include <QtSql/QSqlRecord>

#include <QtSql/QSqlQuery>

#include <QtSql/QSqlError>

#include <QtWidgets/QMainWindow>

#include <QtWidgets/QMessageBox>


YerothSqlTableModel::YerothSqlTableModel()
:_logger(0),
 _fromQSqlQuery(false)
{
    _logger = new YerothLogger("YerothSqlTableModel");

    setEditStrategy(QSqlTableModel::OnFieldChange);
}


YerothSqlTableModel::YerothSqlTableModel(const QSqlQuery &aQSqlQuery)
:_logger(0),
 _fromQSqlQuery(true)
{
    _logger = new YerothLogger("YerothSqlTableModel");

    yerothSetQuery(aQSqlQuery);

    setEditStrategy(QSqlTableModel::OnFieldChange);
}


YerothSqlTableModel::YerothSqlTableModel(const QString &sqlTableName,
									   bool setTableName /* = true */)
:_sqlTableName(sqlTableName),
 _logger(0),
 _fromQSqlQuery(false)
{
    _logger = new YerothLogger("YerothSqlTableModel");

    _logger->log("YerothSqlTableModel",
                 QString("table name: %1, edit strategy: QSqlTableModel::OnFieldChange")
                 .arg(sqlTableName));

    if (setTableName)
    {
        setTable(sqlTableName);
    }

    setEditStrategy(QSqlTableModel::OnFieldChange);
}


int YerothSqlTableModel::Is_SearchQSqlTable(QString tableColumn,
        								    QString searchTerm)
{
    QString queryFilter(GENERATE_SQL_IS_STMT(tableColumn, searchTerm));

    yerothSetFilter(queryFilter);

    int filterRowCount = rowCount();

    _logger->log("Is_SearchQSqlTable",
                 QString("(table: %1, column: %2) | queryFilter (%3) | row count: %4")
                 	 .arg(_sqlTableName,
                 		  tableColumn,
						  queryFilter,
						  QString::number(filterRowCount)));
    resetFilter();

    return filterRowCount;
}


int YerothSqlTableModel::_Is_SearchQSqlTable(enum YerothSqlQueryType,
                                             QString tableColumn,
                                             QStringList &searchTermsList)
{
    QString queryTerm;

    const int size = searchTermsList.size();
    const int max = size - 1;

    for (int k = 0; k < size; ++k)
    {
        if (k < max)
        {
            queryTerm.append(GENERATE_SQL_IS_STMT(tableColumn, searchTermsList.at(k)))
            		 .append(" AND ");
        }
        else
        {
            queryTerm.append(GENERATE_SQL_IS_STMT(tableColumn, searchTermsList.at(k)));
        }
    }

    yerothSetFilter(queryTerm);

    int filterRowCount = rowCount();

    resetFilter();

    return filterRowCount;
}

int YerothSqlTableModel::Like_SearchQSqlTable(QString tableColumn,
        QString searchTerm)
{
    QString queryFilter(GENERATE_SQL_LIKE_STMT(tableColumn, searchTerm));

    yerothSetFilter(queryFilter);

    int filterRowCount = rowCount();

    _logger->log("Like_SearchQSqlTable",
                 QString("(table: %1, column: %2) | queryFilter (%3) | row count: %4")
                 .arg(_sqlTableName)
                 .arg(tableColumn)
                 .arg(queryFilter)
                 .arg(filterRowCount));

    /*qDebug() <<
    		QString("YerothSqlTableModel | Like_SearchQSqlTable (table: %1, column: %2) \n\t\t| queryFilter (%3) | row count: %4")
    		 .arg(_sqlTableName)
    		 .arg(tableColumn)
    		 .arg(queryFilter)
    		 .arg(filterRowCount);*/

    resetFilter();

    return filterRowCount;
}

int YerothSqlTableModel::_Like_SearchQSqlTable(enum YerothSqlQueryType,
                                               QString tableColumn,
                                               QStringList &searchTermsList)
{
    QString queryTerm;

    const int size = searchTermsList.size();
    const int max = size - 1;

    for (int k = 0; k < size; ++k)
    {
        if (k < max)
        {
            queryTerm.append(GENERATE_SQL_LIKE_STMT(tableColumn, searchTermsList.at(k)))
            		 .append(" AND ");
        }
        else
        {
            queryTerm.append(GENERATE_SQL_LIKE_STMT(tableColumn, searchTermsList.at(k)));
        }
    }

    yerothSetFilter(queryTerm);

    int filterRowCount = rowCount();

    resetFilter();

    return filterRowCount;
}

bool YerothSqlTableModel::insertNewRecord(QSqlRecord &record, QMainWindow *parent)
{
    _logger->log("insertNewRecord",
                 QString("Table name: %1").arg(sqlTableName()));

    resetFilter();

    bool success = insertRecord(-1, record);

    //qDebug() << QString("YerothSqlTableModel::insertNewRecord, success: %1")
    //				.arg(BOOL_TO_STRING(success));

    if (!success)
    {
        QString retMsg(QString(QObject::trUtf8("Une erreur (base de données) est survenue pendant l'insertion dans le tableau %1 !\n")
        									.arg(sqlTableName())));

        retMsg.append(QObject::trUtf8("Contactez l'administrateur de Yeroth-erp-3.0 !"));

        if (0 != parent)
        {
            QMessageBox::warning(parent,
                                 QObject::tr("sqltablemodel - insertNewRecord"),
                                 retMsg);
        }

        qDebug() << QString("reason for failing: %1").arg(lastError().text());

        _logger->log("insertNewRecord", QString("reason for failing: %1").arg(lastError().text()));
    }

    return success;
}

bool YerothSqlTableModel::updateRecord(unsigned row, QSqlRecord &record)
{
    _logger->log("updateRecord",
                 QString("Table name: %1").arg(sqlTableName()));

    bool success = setRecord(row, record);

    if (!success)
    {
        _logger->log("updateRecord", QString("updating row %1. reason for failing: %2")
                     .arg(QString::number(row), lastError().text()));
    }
    else
    {
        _logger->log("updateRecord", QString("updated row %1 with success").arg(row));
    }

    return success;
}

void YerothSqlTableModel::resetFilter()
{
    yerothSetFilter(YerothUtils::EMPTY_STRING);
    select();
}

int YerothSqlTableModel::easySelect()
{
    bool success = select();

    if (!success)
    {
        return -1;
    }

    return rowCount();
}


bool YerothSqlTableModel::yerothSetSort(int column, Qt::SortOrder order)
{
    setSort(column, order);
    return select();
}


bool YerothSqlTableModel::yerothSetQuery(QString aSqlQuery)
{
	QSqlQueryModel::setQuery(aSqlQuery);

   	if (lastError().isValid())
   	{
   		qDebug() << "++ YerothSqlTableModel::yerothSetQuery(QString): \n\t"
   				 << aSqlQuery
				 << "\n"
				 << lastError();

   		return false;
   	}

   	return true;
}
