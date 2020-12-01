/*
 * yeroth-erp-database-table-import-info.hpp
 *
 *      Author: DR. XAVIER NOUMBISSI NOUNDOU
 */

# ifndef YEROTH_ERP_DATABASE_TABLE_INFO_HPP_
# define YEROTH_ERP_DATABASE_TABLE_INFO_HPP_


#include "src/utils/yeroth-erp-utils.hpp"


#include <QtCore/QDebug>


class YerothERPDatabaseTableColumnInfo
{
public:

	inline YerothERPDatabaseTableColumnInfo(const QString				& aColumnName,
									 	 	const QString 				& aColumnType)
	:_tableColumnName(aColumnName),
	 _tableColumntype(aColumnType)
	{
	}

	inline void toString()
	{
		qDebug() << QString("++ %1 has type %2")
						.arg(_tableColumnName,
							 _tableColumntype);
	}

	inline const QString& getColumnName() const
	{
		return _tableColumnName;
	}

	inline const QString& getColumnType() const
	{
		return _tableColumntype;
	}

private:

	QString 	_tableColumnName;

	QString 	_tableColumntype;
};


#endif /* YEROTH_ERP_DATABASE_TABLE_INFO_HPP_ */
