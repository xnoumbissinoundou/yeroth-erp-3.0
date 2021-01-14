/*
 * yeroth-erp-sqltable-model.cpp
 *
  *      Author: DR. XAVIER NOUMBISSI NOUNDOU
 */

#include "yeroth-erp-sqltable-model.hpp"

#include "src/utils/yeroth-erp-database-table-column.hpp"

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


int YerothSqlTableModel::yeroth_RESET_specify_filter_FROM_SELECT_STATEMENT(const QString &filter)
{
	QString curYerothSelectStatement(yerothSelectStatement());

	curYerothSelectStatement.remove(filter);

//	QDEBUG_STRINGS_OUTPUT_2("RESET filter", curYerothSelectStatement);

	int queryResultSize = yerothSetQueryRowCount(curYerothSelectStatement);

	return queryResultSize;
}


int YerothSqlTableModel::yeroth_specify_filter_FROM_SELECT_STATEMENT(const QString &filter)
{
	QString curYerothSelectStatement(yerothSelectStatement());

	curYerothSelectStatement.append(QString(" %1")
										.arg(filter));

//	QDEBUG_STRINGS_OUTPUT_2("filter", curYerothSelectStatement);

	int queryResultSize = yerothSetQueryRowCount(curYerothSelectStatement);

	return queryResultSize;
}


int YerothSqlTableModel::Is_SearchQSqlTable(QString tableColumn,
        								    QString searchTerm)
{
    QString queryFilter(GENERATE_SQL_IS_STMT(tableColumn, searchTerm));

    yerothSetFilter_WITH_where_clause(queryFilter);

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

    yerothSetFilter_WITH_where_clause(queryTerm);

    int filterRowCount = rowCount();

    resetFilter();

    return filterRowCount;
}

int YerothSqlTableModel::Like_SearchQSqlTable(QString tableColumn,
        QString searchTerm)
{
    QString queryFilter(GENERATE_SQL_LIKE_STMT(tableColumn, searchTerm));

    yerothSetFilter_WITH_where_clause(queryFilter);

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

    yerothSetFilter_WITH_where_clause(queryTerm);

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
        QString retMsg(QObject::trUtf8("Une erreur (base de données) est survenue pendant l'insertion dans le tableau %1 !\n")
        									.arg(sqlTableName()));

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


bool YerothSqlTableModel::updateRecord(QSqlRecord &record)
{
    _logger->log("updateRecord",
                 QString("Table name: %1").arg(sqlTableName()));

    bool success = false;

    QString aCurFieldValue;

    QString dbFieldIDValue;

    QString aCurFieldName;

    QString SQL_UPDATE_STRING_COMMAND = QString("UPDATE %1 SET ")
    										.arg(_sqlTableName);

    for (int k = 0; k < record.count(); ++k)
    {
    	aCurFieldName.clear();

    	aCurFieldName.append(record.fieldName(k));

    	if (!YerothUtils::isEqualCaseInsensitive(YerothDatabaseTableColumn::ID, aCurFieldName))
    	{
    		if (!aCurFieldName.contains("date_"))
    		{
        		aCurFieldValue = YerothUtils::get_text(record.value(aCurFieldName));

            	SQL_UPDATE_STRING_COMMAND.append(QString("%1='%2', ")
            										.arg(aCurFieldName,
            											 aCurFieldValue));
    		}
    		else
    		{
    			aCurFieldValue = DATE_TO_DB_FORMAT_STRING(record.value(aCurFieldName).toDate());

//    			QDEBUG_STRINGS_OUTPUT_2("aCurFieldValue (DATE_TO_DB_FORMAT_STRING)", aCurFieldValue);

    			if (!aCurFieldValue.isEmpty())
    			{
    				SQL_UPDATE_STRING_COMMAND.append(QString("%1='%2', ")
    													.arg(aCurFieldName,
    														 aCurFieldValue));
    			}
    		}
    	}
    	else
    	{
    		dbFieldIDValue = YerothUtils::get_text(record.value(aCurFieldName));
    	}
    }

    int len = SQL_UPDATE_STRING_COMMAND.length();

    SQL_UPDATE_STRING_COMMAND.remove(len - 2, 2);

	SQL_UPDATE_STRING_COMMAND.append(QString(" WHERE %1 = '%2';")
										.arg(YerothDatabaseTableColumn::ID,
											 dbFieldIDValue));

//    QDEBUG_STRINGS_OUTPUT_2("SQL_UPDATE_STRING_COMMAND", SQL_UPDATE_STRING_COMMAND);

    success = YerothUtils::execQuery(SQL_UPDATE_STRING_COMMAND);

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
    yerothSetFilter_WITH_where_clause(YerothUtils::EMPTY_STRING);
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

int YerothSqlTableModel::yerothSetQueryRowCount(const QString &aSqlQuery)
{
	QSqlQueryModel::setQuery(aSqlQuery);

   	if (lastError().isValid())
   	{
   		qDebug() << "++ YerothSqlTableModel::yerothSetQuery(QString): \n\t"
   				 << aSqlQuery
				 << "\n"
				 << lastError();

   		return -1;
   	}

   	int queryResultRowCount = QSqlQueryModel::rowCount();

   	return queryResultRowCount;
}


bool YerothSqlTableModel::yerothSetQuery(const QString &aSqlQuery)
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
