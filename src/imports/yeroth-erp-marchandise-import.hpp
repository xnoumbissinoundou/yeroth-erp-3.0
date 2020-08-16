/*
 * yeroth-erp-stock-marchandise.hpp
 *
 *      Author: Dipl.-Inf. XAVIER NOUMBISSI NOUNDOU
 */

# ifndef YEROTH_MARCHANDISE_IMPORT_WINDOW_HPP_
# define YEROTH_MARCHANDISE_IMPORT_WINDOW_HPP_


#include "src/imports/yeroth-erp-stock-import.hpp"


class YerothERPMarchandiseImport : public YerothERPStockImport
{
    Q_OBJECT

public:

	YEROTH_CLASS_OPERATORS

	YerothERPMarchandiseImport(QStringList 									 &aCurCsvFileToImportContentWordList,
							   QMap<int, YerothERPDatabaseTableColumnInfo *> &aCsvContentIdxToDatabaseTableColumnInfo);


	YerothERPMarchandiseImport(YerothPOSAdminWindowsCommons 				 &aCallingWindow,
			 	 	 	 	   QStringList 				    				 &aCurCsvFileToImportContentWordList,
							   QMap<int, YerothERPDatabaseTableColumnInfo *> &anIndexToDatabaseTableColumnInfo);

    inline virtual ~YerothERPMarchandiseImport()
    {
    }

    virtual int import();
};


#endif /* YEROTH_MARCHANDISE_IMPORT_WINDOW_HPP_ */
