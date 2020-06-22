/*
 * yeroth-erp-stock-import.hpp
 *
 *      Author: Dipl.-Inf. XAVIER NOUMBISSI NOUNDOU
 */

# ifndef YEROTH_STOCK_IMPORT_WINDOW_HPP_
# define YEROTH_STOCK_IMPORT_WINDOW_HPP_

#include "src/imports/yeroth-erp-database-table-import-info.hpp"

#include "src/utils/yeroth-erp-utils.hpp"


enum import_csv_entry_row_return_status
{
    IMPORT_DATA_CSV_STOCK_INSERTION_SUCCEED = 0,
	IMPORT_DATA_CSV_STOCK_INSERTION_FAILED,
	IMPORT_DATA_CSV_STOCK_INCORRECT_COLUMN_VALUE,
	IMPORT_DATA_CSV_STOCK_MANDATORY_COLUMN_VALUE_MISSING,
	IMPORT_DATA_CSV_STOCK_UNDEFINED
};

class YerothERPStockImport : public QObject
{
    Q_OBJECT

public:

	YEROTH_CLASS_OPERATORS

    YerothERPStockImport(QStringList 				  					&aCurCsvFileToImportContentWordList,
						 QMap<int, YerothERPDatabaseTableColumnInfo *> 	&anIndexToDatabaseTableColumnInfo);

    YerothERPStockImport(YerothPOSAdminWindowsCommons 					&aCallingWindow,
    					 QStringList 				  					&aCurCsvFileToImportContentWordList,
						 QMap<int, YerothERPDatabaseTableColumnInfo *> 	&anIndexToDatabaseTableColumnInfo);

    inline virtual ~YerothERPStockImport()
    {
    }

    int import();

    static QString 					_allMissingMandatoryColumnValue;

    static QMap<QString, bool>		*_dbTableColumnToIsNotNULL;

private:

    enum import_csv_entry_row_return_status
		import_csv_entry_row(QStringList &aCsvFileEntryLine);

    void missing_mandatory_item_field_msg(const QString &aMandatoryColumn);

    bool check_mandatory_item_field();

    QStringList 					_allSqlTableImportColumns;

    QStringList 					_allMandatoryTableColumns;

    QStringList 					*_curCsvFileToImportContentWordList;

    QMap<int, YerothERPDatabaseTableColumnInfo *> 	*_csvContentIdxToDatabaseTableColumnInfo;

    YerothPOSAdminWindowsCommons 	*_callingWindow;
};


#endif /* YEROTH_STOCK_IMPORT_WINDOW_HPP_ */
