/*
 * yeroth-erp-stock-import.hpp
 *
 *      Author: Xavier NOUMBISSI NOUNDOU, Dipl.-Inf., Ph.D. (ABD)
 */

# ifndef YEROTH_STOCK_IMPORT_WINDOW_HPP_
# define YEROTH_STOCK_IMPORT_WINDOW_HPP_


#include "src/utils/yeroth-erp-utils.hpp"


class YerothERPStockImport : public QObject
{
    Q_OBJECT

public:

	YEROTH_CLASS_OPERATORS

    YerothERPStockImport(YerothPOSAdminWindowsCommons 	&aCallingWindow,
    					 QStringList 				  	&aCurCsvFileToImportContentWordList,
    					 QMap<int, YerothComboBox *> 	&anIndexToSQLTableImportHeader);

    inline ~YerothERPStockImport()
    {
    }

    int import();

private:

    bool import_csv_entry_row(QStringList &aCsvFileEntryLine);

    void missing_mandatory_item_field_msg(const QString &aMandatoryColumn);

    bool check_mandatory_item_field();


    QStringList 					_allSqlTableImportColumns;

    QStringList 					*_curCsvFileToImportContentWordList;

    QMap<int, YerothComboBox *> 	*_indexToSQLTableImportHeader;

    YerothPOSAdminWindowsCommons 	*_callingWindow;
};


#endif /* YEROTH_STOCK_IMPORT_WINDOW_HPP_ */
