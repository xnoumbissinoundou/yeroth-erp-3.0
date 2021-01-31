/*
 * yeroth-erp-marchandise-import.hpp
 *
 *      Author: DR. XAVIER NOUMBISSI NOUNDOU
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

	_allMandatoryTableColumns.append(YerothDatabaseTableColumn::NOM_DEPARTEMENT_PRODUIT);

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

	_allMandatoryTableColumns.append(YerothDatabaseTableColumn::NOM_DEPARTEMENT_PRODUIT);

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

	QStringList allImportedTableColumns;

	QString curTableColumnType;
	QString curTableColumnName;
	QString curColumnRowEntry;

	QString productReference;
	QString productName;
	QString productDepartment;
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

//			QDEBUG_STRINGS_OUTPUT_2("curTableColumnName", curTableColumnName);
//
//			QDEBUG_STRINGS_OUTPUT_2("curTableColumnType", curTableColumnType);
//
//			QDEBUG_STRINGS_OUTPUT_2("curColumnRowEntry", curColumnRowEntry);

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
				YerothUtils::isEqualCaseInsensitive(YerothDatabaseTableColumn::NOM_DEPARTEMENT_PRODUIT, curTableColumnName))
			{
				productDepartment = curColumnRowEntry;

				QString queryProductDepartmentStr(QString("select * from %1 WHERE %2 = '%3'")
						.arg(YerothDatabase::DEPARTEMENTS_PRODUITS,
							 YerothDatabaseTableColumn::NOM_DEPARTEMENT_PRODUIT,
							 curColumnRowEntry));

				querySize = YerothUtils::execQueryRowCount(queryProductDepartmentStr);

				if (querySize <= 0)
				{
					queryProductDepartmentStr = QString("insert into %1 (%2, %3) values ('%4', '%5')")
														.arg(YerothDatabase::DEPARTEMENTS_PRODUITS,
																YerothDatabaseTableColumn::ID,
																YerothDatabaseTableColumn::NOM_DEPARTEMENT_PRODUIT,
																QString::number(allWindows->getNextIdSqlTableModel_departements_produits()),
																curColumnRowEntry);

					if (!YerothUtils::execQuery(queryProductDepartmentStr))
					{
						QString infoMesg =
								QObject::trUtf8("Le département '%1' ne pouvait pas être créée !")
									.arg(curColumnRowEntry);

						if (0 != _callingWindow)
						{
							if (importerParlant)
							{
								YerothQMessageBox::warning(_callingWindow,
										QObject::trUtf8("création d'1 département de produits"),
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

			if (_allMandatoryTableColumns.contains(curTableColumnName)     &&
				YerothUtils::isEqualCaseInsensitive(YerothDatabaseTableColumn::CATEGORIE, curTableColumnName))
			{
				productCategorie = curColumnRowEntry;

				QString queryCategoryStr(QString("select * from %1 WHERE %2 = '%3'")
						.arg(YerothDatabase::CATEGORIES,
							 YerothDatabaseTableColumn::NOM_CATEGORIE,
							 curColumnRowEntry));

				querySize = YerothUtils::execQueryRowCount(queryCategoryStr);

				if (querySize <= 0)
				{
					queryCategoryStr = QString("insert into %1 (%2, %3) values ('%4', '%5')")
											.arg(YerothDatabase::CATEGORIES,
												 YerothDatabaseTableColumn::ID,
												 YerothDatabaseTableColumn::NOM_CATEGORIE,
												 QString::number(allWindows->getNextIdSqlTableModel_categories()),
												 curColumnRowEntry);

					if (!YerothUtils::execQuery(queryCategoryStr))
					{
						QString infoMesg =
								QObject::trUtf8("La catégorie '%1' ne pouvait pas "
										"être créée !")
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
				int aCurIntValue = curColumnRowEntry.toInt();

				record.setValue(curTableColumnName, aCurIntValue);

				if (YerothUtils::isEqualCaseInsensitive(YerothDatabaseTableColumn::ID,
						curTableColumnName))
				{
					marchandises_id_to_save = aCurIntValue;
				}
			}
			else if (YEROTH_QSTRING_CONTAINS(curTableColumnType, YerothUtils::DATABASE_MYSQL_DOUBLE_TYPE_STRING))
			{
				record.setValue(curTableColumnName,
						YerothUtils::YEROTH_CONVERT_QSTRING_TO_DOUBLE_LOCALIZED(curColumnRowEntry));
			}
			else if (YEROTH_QSTRING_CONTAINS(curTableColumnType, YerothUtils::DATABASE_MYSQL_DATE_TYPE_STRING))
			{
				record.setValue(curTableColumnName,
						GET_DATE_FROM_STRING(curColumnRowEntry));
			}
			else
			{
				record.setValue(curTableColumnName, curColumnRowEntry);
			}

			allImportedTableColumns.append(curTableColumnName);
		}
	}


	QString infoMesg;

	QString curExistingReferenceDesignationCategory_PRODUCT_in_out;

    enum service_stock_already_exist_type SERVICE_REFERENCE_STOCK_DESIGNATION_EXIST =
    		YerothUtils::IS_STOCK_DESIGNATION_OR_REFERENCE_UNIQUE(productReference,
    															  productCategorie,
    									   	   	   	   	   	   	  productName,
																  curExistingReferenceDesignationCategory_PRODUCT_in_out);

    if (SERVICE_REFERENCE_EXISTS == SERVICE_REFERENCE_STOCK_DESIGNATION_EXIST)
    {
    	if (!curExistingReferenceDesignationCategory_PRODUCT_in_out.isEmpty())
    	{
			infoMesg = QObject::trUtf8("Cette référence ('%1') "
									   "est déjà utilisée par la marchandise '%2' !")
							.arg(productReference,
								 curExistingReferenceDesignationCategory_PRODUCT_in_out);
    	}
    }
    else if (SERVICE_STOCK_CATEGORY_EXIST == SERVICE_REFERENCE_STOCK_DESIGNATION_EXIST)
    {
		QString infoMesg =
				QObject::trUtf8("La marchandise '%1' est déjà dans la catégorie ('%2') !")
					.arg(productCategorie,
						 curExistingReferenceDesignationCategory_PRODUCT_in_out);
    }
    else if (SERVICE_STOCK_DESIGNATION_EXIST == SERVICE_REFERENCE_STOCK_DESIGNATION_EXIST)
    {
		infoMesg = QObject::trUtf8("La marchandise ('%1') utilise déjà la référence '%2' !")
						.arg(productName,
							 curExistingReferenceDesignationCategory_PRODUCT_in_out);
    }

    if (SERVICE_REFERENCE_EXISTS		== SERVICE_REFERENCE_STOCK_DESIGNATION_EXIST ||
    	SERVICE_STOCK_CATEGORY_EXIST 	== SERVICE_REFERENCE_STOCK_DESIGNATION_EXIST ||
    	SERVICE_STOCK_DESIGNATION_EXIST == SERVICE_REFERENCE_STOCK_DESIGNATION_EXIST )
    {
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

    	return IMPORT_DATA_CSV_INSERTION_FAILED;
    }


	record.setValue(YerothDatabaseTableColumn::ID, marchandises_id_to_save);

    record.setValue(YerothDatabaseTableColumn::IS_SERVICE, 0);


	if (0 != _dbTableColumnToIsNotNULL)
	{
		QString aCurSqlTableImportColumn;

		QStringList allSqltableColumns = _dbTableColumnToIsNotNULL->keys();

		allSqltableColumns.removeAll(YerothDatabaseTableColumn::ID);
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
