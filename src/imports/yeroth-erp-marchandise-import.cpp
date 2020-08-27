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
	_allMandatoryTableColumns.clear();

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
	_allMandatoryTableColumns.clear();

	_allMandatoryTableColumns.append(YerothDatabaseTableColumn::DESIGNATION);

	_allMandatoryTableColumns.append(YerothDatabaseTableColumn::CATEGORIE);
}


enum import_csv_entry_row_return_status
	YerothERPMarchandiseImport::import_csv_entry_row(bool importerParlant, QStringList &aCsvFileEntryLine)
{

	enum import_csv_entry_row_return_status insertionReturnStatus = IMPORT_DATA_CSV_INSERTION_FAILED;

	YerothERPWindows *allWindows = YerothUtils::getAllWindows();

	if (0 == allWindows)
	{
		return IMPORT_DATA_CSV_INSERTION_FAILED;
	}

	YerothERPDatabaseTableColumnInfo *curDatabaseTableColumnInfo = 0;

	YerothSqlTableModel &curMarchandisesTableModel = allWindows->getSqlTableModel_marchandises();

    QSqlRecord record = curMarchandisesTableModel.record();

    int marchandises_id_to_save = YerothERPWindows::getNextIdSqlTableModel_marchandises();

    int querySize = -1;

	record.setValue(YerothDatabaseTableColumn::ID, marchandises_id_to_save);

	QStringList allImportedTableColumns;

	QString curTableColumnType;
	QString curTableColumnName;
	QString curColumnRowEntry;

	QString productReference;
	QString productName;
	QString productCategorie;

	for (int j = 0; j < aCsvFileEntryLine.size(); ++j)
	{
		curColumnRowEntry = aCsvFileEntryLine.at(j).trimmed();

		if (!curColumnRowEntry.isEmpty())
		{
			if (curColumnRowEntry.at(0) == '"')
			{
				curColumnRowEntry.replace(0, 1, "");
			}

			if (curColumnRowEntry.at(curColumnRowEntry.length() - 1) == '"')
			{
				curColumnRowEntry.replace(curColumnRowEntry.length() - 1, 1, "");
			}
		}

		curDatabaseTableColumnInfo = _csvContentIdxToDatabaseTableColumnInfo->value(j);

		if (0 != curDatabaseTableColumnInfo)
		{
			curTableColumnType = curDatabaseTableColumnInfo->getColumnType();

			curTableColumnName = curDatabaseTableColumnInfo->getColumnName();

//			qDebug() << QString("++ curTableColumnName: %1, curTableColumnType: %2, curColumnRowEntry: %3")
//							.arg(curTableColumnName,
//								 curTableColumnType,
//								 curColumnRowEntry);

			if (YerothUtils::isEqualCaseInsensitive(YerothDatabaseTableColumn::REFERENCE, curTableColumnName))
			{
				productReference = curColumnRowEntry;
			}

			if (_allMandatoryTableColumns.contains(curTableColumnName)     &&
				YerothUtils::isEqualCaseInsensitive(YerothDatabaseTableColumn::DESIGNATION, curTableColumnName))
			{
				productName = curColumnRowEntry;
			}

			if (_allMandatoryTableColumns.contains(curTableColumnName)     &&
				YerothUtils::isEqualCaseInsensitive(YerothDatabaseTableColumn::CATEGORIE, curTableColumnName))
			{
				productCategorie = curColumnRowEntry;

				QString queryCategoryStr(QString("select * from %1 WHERE %2 = '%3'")
						.arg(YerothUtils::getAllWindows()->CATEGORIES,
							 YerothDatabaseTableColumn::NOM_CATEGORIE,
							 curColumnRowEntry));

				querySize = YerothUtils::execQueryRowCount(queryCategoryStr);

				if (querySize <= 0)
				{
					queryCategoryStr = QString("insert into %1 (%2, %3) values ('%4', '%5')")
											.arg(YerothUtils::getAllWindows()->CATEGORIES,
												 YerothDatabaseTableColumn::ID,
												 YerothDatabaseTableColumn::NOM_CATEGORIE,
												 QString::number(allWindows->getNextIdSqlTableModel_categories()),
												 curColumnRowEntry);

					if (!YerothUtils::execQuery(queryCategoryStr))
					{
						QString infoMesg =
								QString(QObject::trUtf8("La catégorie '%1' ne pouvait pas "
										"être créée !"))
										.arg(curColumnRowEntry);

						if (0 != _callingWindow)
						{
							if (importerParlant)
							{
								YerothQMessageBox::warning(_callingWindow,
										QObject::trUtf8("création de catégorie d'articles"),
										infoMesg);
							}
						}
						else
						{
							qDebug() << infoMesg;
						}

						return IMPORT_DATA_CSV_INSERTION_FAILED;
					}
				}
			}


			if (YEROTH_QSTRING_CONTAINS(curTableColumnType, YerothUtils::DATABASE_MYSQL_INT_TYPE_STRING))
			{
				record.setValue(curTableColumnName, curColumnRowEntry.toInt());
			}
			else
			{
				record.setValue(curTableColumnName, curColumnRowEntry);
			}

			allImportedTableColumns.append(curTableColumnName);
		}
	}

	QString aCurProductExistingReference;

    if (!YerothUtils::isReferenceUnique(productReference,
    									productName,
										productCategorie,
										aCurProductExistingReference))
    {
    	if (aCurProductExistingReference.isEmpty())
    	{
			QString infoMesg =
					QString(QObject::trUtf8("Cette marchandise (désignation: '%1' - catégorie: '%2'), "
							"déjà existante dans la liste des marchandises, "
							"n'utilise aucune valeur pour 'référence' !"))
						.arg(productName,
							 productCategorie);

			if (0 != _callingWindow)
			{
				if (importerParlant)
				{
					YerothQMessageBox::warning(_callingWindow,
							QObject::trUtf8("aucune référence"),
							infoMesg);
				}
			}
			else
			{
				qDebug() << infoMesg;
			}
    	}
    	else
    	{
			QString infoMesg =
					QString(QObject::trUtf8("Cette marchandise (désignation: '%1' - catégorie: '%2'), "
							"déjà existante dans la liste des marchandises, "
							"utilise la 'référence (%3)' !"))
						.arg(productName,
							 productCategorie,
							 aCurProductExistingReference);

			if (0 != _callingWindow)
			{
				if (importerParlant)
				{
					YerothQMessageBox::warning(_callingWindow, "enregistrer", infoMesg);
				}
			}
			else
			{
				qDebug() << infoMesg;
			}
    	}

        return IMPORT_DATA_CSV_INSERTION_FAILED;
    }

    service_stock_already_exist_type serviceStockExists =
    		YerothUtils::isStockItemInProductList(false,
    											  productReference,
												  productCategorie,
												  productName);

    if (SERVICE_REFERENCE_EXISTS == serviceStockExists)
    {
    	QString infoMesg =
    			QString(QObject::trUtf8("Un service (stock) "
    					"avec la référence '%1' existe déjà !"))
						.arg(productReference);

    	if (0 != _callingWindow)
    	{
    		if (importerParlant)
    		{
    			YerothQMessageBox::warning(_callingWindow,
    					QObject::trUtf8("référence"),
						infoMesg);
    		}
    	}
    	else
    	{
    		qDebug() << infoMesg;
    	}

    	return IMPORT_DATA_CSV_INSERTION_FAILED;
    }

    if (SERVICE_STOCK_DESIGNATION_AND_DIFFERENT_CATEGORIE_EXIST == serviceStockExists)
    {
    	QString infoMesg =
    			QString(QObject::trUtf8("La désignation '%1', est déjà existante dans la liste des "
    					"marchandises, dans une autre catégorie que '%2' !"))
						.arg(productName,
								productCategorie);

    	if (0 != _callingWindow)
    	{
    		if (importerParlant)
    		{
    			YerothQMessageBox::warning(_callingWindow,
    					QObject::trUtf8("désignation"),
						infoMesg);
    		}
    	}
    	else
    	{
    		qDebug() << infoMesg;
    	}

    	return IMPORT_DATA_CSV_INSERTION_FAILED;
    }


    record.setValue(YerothDatabaseTableColumn::IS_SERVICE, 0);


	if (0 != _dbTableColumnToIsNotNULL)
	{
		QString aCurSqlTableImportColumn;

		QStringList allSqltableColumns = _dbTableColumnToIsNotNULL->keys();

		allSqltableColumns.removeAll(YerothDatabaseTableColumn::IS_SERVICE);

		for (int k = 0; k < allSqltableColumns.size(); ++k)
		{
			aCurSqlTableImportColumn = allSqltableColumns.at(k);

			if (allImportedTableColumns.contains(aCurSqlTableImportColumn))
			{
				continue;
			}

			if (false == _dbTableColumnToIsNotNULL->value(aCurSqlTableImportColumn))
			{
				insertionReturnStatus = IMPORT_DATA_CSV_MANDATORY_COLUMN_VALUE_MISSING;
				/*
				 * This SQL stock table column MUST BE NOT NULL.
				 * So we attribute it a standard value.
				 */
				if (!YEROTH_QSTRING_CONTAINS(_allMissingMandatoryColumnValue, aCurSqlTableImportColumn))
				{
					_allMissingMandatoryColumnValue.append(QString(" , %1")
							.arg(aCurSqlTableImportColumn));
				}
			}
		}

		if (IMPORT_DATA_CSV_MANDATORY_COLUMN_VALUE_MISSING == insertionReturnStatus)
		{
			return IMPORT_DATA_CSV_MANDATORY_COLUMN_VALUE_MISSING;
		}
	}

	bool queryResut = curMarchandisesTableModel.insertNewRecord(record);

	if (queryResut)
	{
		return IMPORT_DATA_CSV_INSERTION_SUCCEED;
	}

	return IMPORT_DATA_CSV_INSERTION_FAILED;
}
