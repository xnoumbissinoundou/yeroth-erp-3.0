/*
 * yeroth-erp-marchandise-import.hpp
 *
 *      Author: Dipl.-Inf. XAVIER NOUMBISSI NOUNDOU
 */

#include "yeroth-erp-marchandise-import.hpp"


#include "src/utils/yeroth-erp-historique-stock.hpp"

#include "src/yeroth-erp-windows.hpp"

#include "src/utils/yeroth-erp-database-table-column.hpp"

#include "src/widgets/yeroth-erp-qmessage-box.hpp"


#include <QtCore/QDebug>


YerothERPMarchandiseImport::YerothERPMarchandiseImport(QStringList &aCurCsvFileToImportContentWordList,
						  	  	  	  	  	  	  	   QMap<int, YerothERPDatabaseTableColumnInfo *> &aCsvContentIdxToDatabaseTableColumnInfo)
:YerothERPStockImport(aCurCsvFileToImportContentWordList,
					  aCsvContentIdxToDatabaseTableColumnInfo)
{
	_allMandatoryTableColumns.append(YerothDatabaseTableColumn::DESIGNATION);

	_allMandatoryTableColumns.append(YerothDatabaseTableColumn::CATEGORIE);
}


YerothERPMarchandiseImport::YerothERPMarchandiseImport(YerothPOSAdminWindowsCommons &aCallingWindow,
													   QStringList 				    &aCurCsvFileToImportContentWordList,
													   QMap<int, YerothERPDatabaseTableColumnInfo *> &anIndexToDatabaseTableColumnInfo)
:YerothERPStockImport(aCallingWindow,
					  aCurCsvFileToImportContentWordList,
					  anIndexToDatabaseTableColumnInfo)
{
	_allMandatoryTableColumns.append(YerothDatabaseTableColumn::DESIGNATION);

	_allMandatoryTableColumns.append(YerothDatabaseTableColumn::CATEGORIE);
}


int YerothERPMarchandiseImport::import()
{

	return 0;
}
