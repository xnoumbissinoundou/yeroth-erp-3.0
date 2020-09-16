/*
 * yeroth-erp-sqltable-model.hpp
 *
 *      Author: Dipl.-Inf. XAVIER NOUMBISSI NOUNDOU
 */

#ifndef YEROTH_SQLTABLE_MODEL_HPP_
#define YEROTH_SQLTABLE_MODEL_HPP_

#include "src/include/yeroth-erp-3-0-software.text-configuration.hpp"

#include <QtSql/QSqlRelationalTableModel>

class QMainWindow;
class YerothLogger;

enum YerothSqlQueryType
{
	OR  = 0,
	AND = 1
};

class YerothSqlTableModel : public QSqlTableModel
{
public:

	YEROTH_CLASS_OPERATORS

	YerothSqlTableModel();

	YerothSqlTableModel(const QSqlQuery &aQSqlQuery);

	YerothSqlTableModel(const QString &sqlTableName, bool setTableName = true);

	inline ~YerothSqlTableModel()
	{
		delete _logger;
	}

	inline bool isFromQSqlQuery()
	{
		return _fromQSqlQuery;
	}

//	inline virtual void yerothSetFilter(const QString &filter)
//	{
//		QSqlTableModel::setFilter(filter.trimmed());
//	}

	inline virtual void yerothSetFilter_WITH_where_clause(const QString &filter)
	{
		QSqlTableModel::setFilter(filter.trimmed());
	}

	inline void changeEditStrategy(QSqlTableModel::EditStrategy strategy)
	{
		this->setEditStrategy(strategy);
	}

	int Is_SearchQSqlTable(QString tableColumn,
						   QString searchTerm);

	int _Is_SearchQSqlTable(enum YerothSqlQueryType,
							QString tableColumn,
							QStringList &searchTermsList);

	inline int OR_Is_SearchQSqlTable(QString tableColumn, QStringList &searchTermsList)
	{
		return _Is_SearchQSqlTable(OR, tableColumn, searchTermsList);
	}

	inline int AND_Is_SearchQSqlTable(QString tableColumn, QStringList &searchTermsList)
	{
		return _Is_SearchQSqlTable(AND, tableColumn, searchTermsList);
	}

	int Like_SearchQSqlTable(QString tableColumn,
							 QString searchTerm);

	int _Like_SearchQSqlTable(enum YerothSqlQueryType,
							  QString tableColumn,
							  QStringList &searchTermsList);

	inline int OR_Like_SearchQSqlTable(QString tableColumn, QStringList &searchTermsList)
	{
		return _Like_SearchQSqlTable(OR, tableColumn, searchTermsList);
	}

	inline int AND_Like_SearchQSqlTable(QString tableColumn, QStringList &searchTermsList)
	{
		return _Like_SearchQSqlTable(AND, tableColumn, searchTermsList);
	}

	bool insertNewRecord(QSqlRecord &record, QMainWindow *parent = 0);

	bool updateRecord(unsigned row, QSqlRecord &record);

	/*inline bool removeRow(int row)
	{
		return this->removeRow(row);
	}*/

	void resetFilter();

	inline QString sqlTableName()
	{
		return _sqlTableName;
	}

	inline void yerothSetQuery(const QSqlQuery &query)
	{
		QSqlTableModel::setQuery(query);
	}

	int easySelect();

	bool yerothSetSort(int column, Qt::SortOrder order);

	bool yerothSetQuery(QString aSqlQuery);

	inline QString yerothSelectStatement()
	{
		return QSqlTableModel::selectStatement();
	}

private:

    const QString 	_sqlTableName;

    YerothLogger		*_logger;

    bool			_fromQSqlQuery;
};

#endif /* YEROTH_SQLTABLE_MODEL_HPP_ */
