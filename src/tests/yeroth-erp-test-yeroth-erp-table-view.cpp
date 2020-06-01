/*
 * yeroth-erp-test-yeroth-erp-windows.cpp
 *
 *      Author: Dipl.-Inf. Xavier NOUMBISSI NOUNDOU, Ph.D. (ABD)
 */

#include "yeroth-erp-test-yeroth-erp-table-view.hpp"


#include "src/utils/yeroth-erp-historique-stock.hpp"

#include "src/yeroth-erp-windows.hpp"

#include "src/widgets/table-view/yeroth-erp-table-view.hpp"


QString		Test_YerothERPTableView::_allMissingMandatoryColumnValue;


void Test_YerothERPTableView::initTestCase()
{
	QVERIFY(0 != _allWindows);

	_allMandatoryTableColumns.append(YerothDatabaseTableColumn::DESIGNATION);

	_allMandatoryTableColumns.append(YerothDatabaseTableColumn::CATEGORIE);

	_allMandatoryTableColumns.append(YerothDatabaseTableColumn::QUANTITE_TOTALE);

	_allMandatoryTableColumns.append(YerothDatabaseTableColumn::PRIX_UNITAIRE);

	test_TABLE_VIEW_lister_import_test_data();
}


void Test_YerothERPTableView::cleanupTestCase()
{
    QMapIterator<int, YerothERPDatabaseTableColumnInfo *>
    	_it_TEST_csvContentIdxToDatabaseTableColumnInfo(_TEST_csvContentIdxToDatabaseTableColumnInfo);

    YerothERPDatabaseTableColumnInfo *anERPDatabaseTableColumnInfo = 0;

    while(_it_TEST_csvContentIdxToDatabaseTableColumnInfo.hasNext())
    {
    	_it_TEST_csvContentIdxToDatabaseTableColumnInfo.next();

    	anERPDatabaseTableColumnInfo = _it_TEST_csvContentIdxToDatabaseTableColumnInfo.value();

    	if (0 != anERPDatabaseTableColumnInfo)
    	{
    		delete anERPDatabaseTableColumnInfo;
    	}
    }
}


void Test_YerothERPTableView::test_TABLE_VIEW_lister_fefo()
{
	YerothTableView a_test_TableView_object;

	QMap<QString, QString> stockNameToStockID_in_out;

	YerothSqlTableModel &aQSqlStockTableModel = _allWindows->getSqlTableModel_stocks();

	a_test_TableView_object.lister_FEFO(aQSqlStockTableModel, stockNameToStockID_in_out);

	qDebug() << "++ stockNameToStockID_in_out: " << stockNameToStockID_in_out;

	int TEST_VAR_QMAP_STOCK_RESULT_COUNT = stockNameToStockID_in_out.size();

	QVERIFY(TEST_VAR_QMAP_STOCK_RESULT_COUNT == 1);

	int stockID = stockNameToStockID_in_out.value("test_yeroth_1").toInt();

	QVERIFY(stockID == 3);
}


void Test_YerothERPTableView::test_TABLE_VIEW_lister_fifo()
{
}


void Test_YerothERPTableView::test_TABLE_VIEW_lister_lifo()
{

}


void Test_YerothERPTableView::test_TABLE_VIEW_lister_collect_data_result(
		const QList<QDate> &allStocksInsertDate_in_out)
{
}


enum import_csv_entry_row_return_status
	Test_YerothERPTableView::import_csv_entry_row(QStringList &aCsvFileEntryLine)
{
	enum import_csv_entry_row_return_status insertionReturnStatus = IMPORT_DATA_CSV_STOCK_INSERTION_FAILED;

	YerothERPDatabaseTableColumnInfo *curDatabaseTableColumnInfo = 0;

	YerothSqlTableModel &curStocksTableModel = _allWindows->getSqlTableModel_stocks();

    QSqlRecord record = curStocksTableModel.record();

    double montant_tva = -1.0;
    double prix_unitaire = -1.0;
    double prix_vente = -1.0;
    double quantite_totale = -1.0;

    int stock_id_to_save = YerothERPWindows::getNextIdSqlTableModel_stocks();

    int querySize = -1;

	record.setValue(YerothDatabaseTableColumn::ID, stock_id_to_save);

	QStringList allImportedTableColumns;

	QString curTableColumnType;
	QString curTableColumnName;
	QString curColumnRowEntry;

	QString productReference;
	QString productName;
	QString productCategorie;
	QString proposedFournisseur;

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

		curDatabaseTableColumnInfo = _TEST_csvContentIdxToDatabaseTableColumnInfo.value(j);

//		qDebug() << "++ curDatabaseTableColumnInfo: " << curDatabaseTableColumnInfo;

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

			if (YerothUtils::isEqualCaseInsensitive(YerothDatabaseTableColumn::NOM_ENTREPRISE_FOURNISSEUR,
													curTableColumnName))
			{
				proposedFournisseur = curColumnRowEntry;

				QString queryFournisseurStr(QString("select * from %1 WHERE %2 = '%3'")
						.arg(YerothUtils::getAllWindows()->FOURNISSEURS,
							 YerothDatabaseTableColumn::NOM_ENTREPRISE,
							 curColumnRowEntry));

				querySize = YerothUtils::execQueryRowCount(queryFournisseurStr);

				if (querySize <= 0)
				{
					queryFournisseurStr = QString("insert into %1 (%2, %3) values ('%4', '%5')")
											.arg(YerothUtils::getAllWindows()->FOURNISSEURS,
												 YerothDatabaseTableColumn::ID,
												 YerothDatabaseTableColumn::NOM_ENTREPRISE,
												 QString::number(_allWindows->getNextIdSqlTableModel_fournisseurs()),
												 curColumnRowEntry);

					if (!YerothUtils::execQuery(queryFournisseurStr))
					{
						QString infoMesg =
								QString(QObject::trUtf8("L'entreprise fournisseur '%1' ne pouvait pas être créée !"))
									.arg(curColumnRowEntry);

						qDebug() << infoMesg;

						return IMPORT_DATA_CSV_STOCK_INSERTION_FAILED;
					}
				}
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
												 QString::number(_allWindows->getNextIdSqlTableModel_categories()),
												 curColumnRowEntry);

					if (!YerothUtils::execQuery(queryCategoryStr))
					{
						QString infoMesg =
								QString(QObject::trUtf8("La catégorie '%1' ne pouvait pas "
										"être créée !"))
										.arg(curColumnRowEntry);

						qDebug() << infoMesg;

						return IMPORT_DATA_CSV_STOCK_INSERTION_FAILED;
					}
				}
			}

			if (YEROTH_QSTRING_CONTAINS(curTableColumnType, YerothUtils::DATABASE_MYSQL_INT_TYPE_STRING))
			{
				record.setValue(curTableColumnName, curColumnRowEntry.toInt());
			}
			else if (YEROTH_QSTRING_CONTAINS(curTableColumnType, YerothUtils::DATABASE_MYSQL_DOUBLE_TYPE_STRING))
			{
				if (YerothDatabaseTableColumn::QUANTITE_TOTALE == curTableColumnName)
				{
					quantite_totale = YerothUtils::YEROTH_CONVERT_QSTRING_TO_DOUBLE_LOCALIZED(curColumnRowEntry);
					record.setValue(curTableColumnName, quantite_totale);
				}
				else if(YerothDatabaseTableColumn::MONTANT_TVA == curTableColumnName)
				{
					montant_tva = YerothUtils::YEROTH_CONVERT_QSTRING_TO_DOUBLE_LOCALIZED(curColumnRowEntry);
					record.setValue(curTableColumnName, montant_tva);
				}
				else if(YerothDatabaseTableColumn::PRIX_UNITAIRE == curTableColumnName)
				{
					prix_unitaire = YerothUtils::YEROTH_CONVERT_QSTRING_TO_DOUBLE_LOCALIZED(curColumnRowEntry);
					record.setValue(curTableColumnName, prix_unitaire);
				}
				else
				{
					record.setValue(curTableColumnName,
							YerothUtils::YEROTH_CONVERT_QSTRING_TO_DOUBLE_LOCALIZED(curColumnRowEntry));
				}
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

			qDebug() << infoMesg;
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

			qDebug() << infoMesg;
    	}

        return IMPORT_DATA_CSV_STOCK_INSERTION_FAILED;
    }

	if (SERVICE_STOCK_DESIGNATION_AND_DIFFERENT_CATEGORIE_EXIST ==
			YerothUtils::isStockItemInProductList(productCategorie,
												  productName))
	{
		QString infoMesg =
				QString(QObject::trUtf8("La désignation '%1', est déjà existante dans la liste des "
										"marchandises, dans une autre catégorie que '%2' !"))
					.arg(productName,
						 productCategorie);

		qDebug() << infoMesg;

		return IMPORT_DATA_CSV_STOCK_INSERTION_FAILED;
	}

	static bool quantite_totale_already_visited = false;

	if (quantite_totale <= 0 && !quantite_totale_already_visited)
	{
		QString infoMesg =
				QString(QObject::trUtf8("La colone '%1' a une valeur (%2) <= '0' !"))
					.arg(YerothDatabaseTableColumn::QUANTITE_TOTALE,
						 QString::number(quantite_totale));

		qDebug() << infoMesg;

		quantite_totale_already_visited = true;

		return IMPORT_DATA_CSV_STOCK_INCORRECT_COLUMN_VALUE;
	}

	if (montant_tva > -1 && prix_unitaire > 0)
	{
		prix_vente = prix_unitaire + montant_tva;

		record.setValue(YerothDatabaseTableColumn::PRIX_VENTE, prix_vente);
	}

	record.setValue(YerothDatabaseTableColumn::IS_SERVICE, 0);
	record.setValue(YerothDatabaseTableColumn::LOTS_ENTRANT, 1);
	record.setValue(YerothDatabaseTableColumn::QUANTITE_PAR_LOT, quantite_totale);
	record.setValue(YerothDatabaseTableColumn::DATE_ENTREE, GET_CURRENT_DATE);

	if (_dbTableColumnToIsNotNULL.size() > 0)
	{
		QString aCurSqlTableImportColumn;

		QStringList allSqltableColumns = _dbTableColumnToIsNotNULL.keys();

		allSqltableColumns.removeAll(YerothDatabaseTableColumn::PRIX_VENTE);
		allSqltableColumns.removeAll(YerothDatabaseTableColumn::IS_SERVICE);
		allSqltableColumns.removeAll(YerothDatabaseTableColumn::LOTS_ENTRANT);
		allSqltableColumns.removeAll(YerothDatabaseTableColumn::QUANTITE_PAR_LOT);
		allSqltableColumns.removeAll(YerothDatabaseTableColumn::DATE_ENTREE);

	    QString historiqueStockInitial(
	    		YerothHistoriqueStock::creer_mouvement_stock(ENTREE,
	    				stock_id_to_save,
						GET_CURRENT_DATE,
						0.0,
						quantite_totale,
						quantite_totale));

	    record.setValue(YerothDatabaseTableColumn::HISTORIQUE_STOCK, historiqueStockInitial);

		for (int k = 0; k < allSqltableColumns.size(); ++k)
		{
			aCurSqlTableImportColumn = allSqltableColumns.at(k);

			if (allImportedTableColumns.contains(aCurSqlTableImportColumn))
			{
				continue;
			}

			if (false == _dbTableColumnToIsNotNULL.value(aCurSqlTableImportColumn))
			{
				insertionReturnStatus = IMPORT_DATA_CSV_STOCK_MANDATORY_COLUMN_VALUE_MISSING;
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

		if (IMPORT_DATA_CSV_STOCK_MANDATORY_COLUMN_VALUE_MISSING == insertionReturnStatus)
		{
			return IMPORT_DATA_CSV_STOCK_MANDATORY_COLUMN_VALUE_MISSING;
		}
	}

	bool queryResut = curStocksTableModel.insertNewRecord(record);

	if (queryResut)
	{
		return IMPORT_DATA_CSV_STOCK_INSERTION_SUCCEED;
	}

	return IMPORT_DATA_CSV_STOCK_INSERTION_FAILED;
}


void Test_YerothERPTableView::test_TABLE_VIEW_lister_import_test_data()
{
    QString csvFilePath(QString("%1/yeroth-erp-3-0-test_data/%2")
    						.arg(YerothERPConfig::YEROTH_ERP_3_0_HOME_FOLDER,
    							 "yeroth_test_data_stock_1_fefo.csv"));

    if (!csvFilePath.isEmpty())
    {
    	bool importSuccess = YerothUtils::import_csv_file_content(csvFilePath, _curCsvFileToImportContentWordList);

    	QVERIFY(importSuccess == true);
    }


    QString curSqlTableImportHeaderStr;

    YerothERPDatabaseTableColumnInfo *curDatabaseTableColumnInfo = 0;

	QStringList csvHeader = _curCsvFileToImportContentWordList.at(0)
					.split(YerothUtils::CSV_FILE_SEPARATION_SEMI_COLON_STRING_CHAR);

    int curCsvFileColumnSize = csvHeader.size();

	YerothUtils::fillDBTableColumnNameToDBTableColumnType_TEST(_allWindows->STOCKS,
															   _dbTableColumnToType,
															   _dbTableColumnToIsNotNULL);

    YerothERPDatabaseTableColumnInfo *anERPDatabaseTableColumnInfo = 0;

    QString aDBColumnName;

    for (int kTEST = 0; kTEST < curCsvFileColumnSize; ++kTEST)
    {
    	aDBColumnName = csvHeader.at(kTEST).trimmed();

    	anERPDatabaseTableColumnInfo = new YerothERPDatabaseTableColumnInfo(aDBColumnName,
    												_dbTableColumnToType.value(aDBColumnName));

    	_TEST_csvContentIdxToDatabaseTableColumnInfo.insert(kTEST, anERPDatabaseTableColumnInfo);
    }

//	qDebug() << "++ _TEST_csvContentIdxToDatabaseTableColumnInfo: " << _TEST_csvContentIdxToDatabaseTableColumnInfo;

    for (int i = 0; i < curCsvFileColumnSize; ++i)
    {
    	curDatabaseTableColumnInfo = _TEST_csvContentIdxToDatabaseTableColumnInfo.value(i);

    	if (0 != curDatabaseTableColumnInfo)
    	{
    		curSqlTableImportHeaderStr = curDatabaseTableColumnInfo->getColumnName();

    		QVERIFY(false == _allSqlTableImportColumns.contains(curSqlTableImportHeaderStr, Qt::CaseInsensitive));

    		_allSqlTableImportColumns.append(curSqlTableImportHeaderStr);
    	}
    }

//    qDebug() << "++ _allSqlTableImportColumns: " << _allSqlTableImportColumns;

    int curCsvFileLineCount = _curCsvFileToImportContentWordList.size() - 1;

    int successImportCount = 0;

    enum import_csv_entry_row_return_status insertionReturnStatusValue = IMPORT_DATA_CSV_STOCK_UNDEFINED;

    QStringList curCsvFileImportRow;

    YEROTH_ERP_3_0_START_DATABASE_TRANSACTION;

    /*
     * We start at index 1 so to avoid the first line
     * that represents the header content, and not data !
     */
    for (int k = 1; k < curCsvFileLineCount; ++k)
    {
    	curCsvFileImportRow = _curCsvFileToImportContentWordList.at(k)
    								.split(YerothUtils::CSV_FILE_SEPARATION_SEMI_COLON_STRING_CHAR);

//		qDebug() << QString("++ curCsvFileImportRow at k (%1): ")
//						.arg(QString::number(k))
//				 << curDatabaseTableColumnInfo;

    	insertionReturnStatusValue = import_csv_entry_row(curCsvFileImportRow);

    	if (IMPORT_DATA_CSV_STOCK_INSERTION_SUCCEED == insertionReturnStatusValue)
    	{
    		++successImportCount;
    	}
    }

    YEROTH_ERP_3_0_COMMIT_DATABASE_TRANSACTION;

    if (successImportCount != curCsvFileLineCount)
    {
    	if (IMPORT_DATA_CSV_STOCK_MANDATORY_COLUMN_VALUE_MISSING == insertionReturnStatusValue)
    	{
    		YerothERPStockImport::_allMissingMandatoryColumnValue.replace(0, 3, YerothUtils::EMPTY_STRING);
    	}
    }

    QVERIFY(successImportCount > 0);
}

