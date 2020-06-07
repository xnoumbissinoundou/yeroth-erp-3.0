/*
 * yeroth-erp-stock-import.hpp
 *
 *      Author: Xavier NOUMBISSI NOUNDOU, Dipl.-Inf., Ph.D. (ABD)
 */

#include "yeroth-erp-stock-import.hpp"


#include "src/utils/yeroth-erp-historique-stock.hpp"

#include "src/yeroth-erp-windows.hpp"

#include "src/utils/yeroth-erp-database-table-column.hpp"

#include "src/widgets/yeroth-erp-qmessage-box.hpp"


#include <QtCore/QDebug>


QString 				YerothERPStockImport::_allMissingMandatoryColumnValue;

QMap<QString, bool>		*YerothERPStockImport::_dbTableColumnToIsNotNULL;


YerothERPStockImport::YerothERPStockImport(QStringList 				  						&aCurCsvFileToImportContentWordList,
										   QMap<int, YerothERPDatabaseTableColumnInfo *> 	&aCsvContentIdxToDatabaseTableColumnInfo)
:_curCsvFileToImportContentWordList(&aCurCsvFileToImportContentWordList),
 _csvContentIdxToDatabaseTableColumnInfo(&aCsvContentIdxToDatabaseTableColumnInfo),
 _callingWindow(0)
{
	_allMandatoryTableColumns.append(YerothDatabaseTableColumn::DESIGNATION);

	_allMandatoryTableColumns.append(YerothDatabaseTableColumn::CATEGORIE);

	_allMandatoryTableColumns.append(YerothDatabaseTableColumn::QUANTITE_TOTALE);

	_allMandatoryTableColumns.append(YerothDatabaseTableColumn::PRIX_UNITAIRE);
}


YerothERPStockImport::YerothERPStockImport(YerothPOSAdminWindowsCommons 					&aCallingWindow,
		 	 	 	 	 	 	 	 	   QStringList 				  						&aCurCsvFileToImportContentWordList,
										   QMap<int, YerothERPDatabaseTableColumnInfo *> 	&anIndexToDatabaseTableColumnInfo)
:_curCsvFileToImportContentWordList(&aCurCsvFileToImportContentWordList),
 _csvContentIdxToDatabaseTableColumnInfo(&anIndexToDatabaseTableColumnInfo),
 _callingWindow(&aCallingWindow)
{
	_allMandatoryTableColumns.append(YerothDatabaseTableColumn::DESIGNATION);

	_allMandatoryTableColumns.append(YerothDatabaseTableColumn::CATEGORIE);

	_allMandatoryTableColumns.append(YerothDatabaseTableColumn::QUANTITE_TOTALE);

	_allMandatoryTableColumns.append(YerothDatabaseTableColumn::PRIX_UNITAIRE);
}


int YerothERPStockImport::import()
{
	YerothERPStockImport::_allMissingMandatoryColumnValue.clear();

	QString warnMesg;

	if (_curCsvFileToImportContentWordList->size() <= 1)
	{
		if (0 != _callingWindow)
		{
			warnMesg = QObject::trUtf8("Le fichier CSV n'a pas de données à importer !");

			YerothQMessageBox::warning(_callingWindow,
					QObject::tr("fichier CSV vide"),
					warnMesg);
		}

		return 0;
	}

	QStringList csvHeader = _curCsvFileToImportContentWordList->at(0)
					.split(YerothUtils::CSV_FILE_SEPARATION_SEMI_COLON_STRING_CHAR);

	QString curSqlTableImportHeaderStr;

	YerothERPDatabaseTableColumnInfo *curDatabaseTableColumnInfo = 0;

	int curCsvFileColumnSize = csvHeader.size();

	/*
	 * check if '2 csv columns' are mapped to
	 * a same database table import-column.
	 */

	for (int i = 0; i < curCsvFileColumnSize; ++i)
	{
		curDatabaseTableColumnInfo = _csvContentIdxToDatabaseTableColumnInfo->value(i);

//		qDebug() << "++ curDatabaseTableColumnInfo: " << curDatabaseTableColumnInfo;

		if (0 != curDatabaseTableColumnInfo)
		{
			curSqlTableImportHeaderStr = curDatabaseTableColumnInfo->getColumnName();

//			qDebug() << "++ curSqlTableImportHeaderStr: " << curSqlTableImportHeaderStr;

			if (!_allSqlTableImportColumns.contains(curSqlTableImportHeaderStr, Qt::CaseInsensitive))
			{
				_allSqlTableImportColumns.append(curSqlTableImportHeaderStr);
			}
			else
			{
				if (0 != _callingWindow)
				{
					warnMesg = QString(QObject::trUtf8("La colone '%1' apparaît "
													   "plusieurs fois parmis les "
													   "colones d'importation !"))
									.arg(curSqlTableImportHeaderStr);

					YerothQMessageBox::warning(_callingWindow,
							QObject::tr("colones d'importation multiples"),
							warnMesg);
				}

				return 0;
			}
		}
	}

	if (!check_mandatory_item_field())
	{
		return 0;
	}

	int curCsvFileLineCount = _curCsvFileToImportContentWordList->size() - 1;

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
		curCsvFileImportRow = _curCsvFileToImportContentWordList->at(k)
						.split(YerothUtils::CSV_FILE_SEPARATION_SEMI_COLON_STRING_CHAR);

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

			if (0 != _callingWindow)
			{
				QString warnMesg =
						QString(QObject::tr("Les colones obligatoires "
								"suivantes '%1' sont manquantes !"))
								.arg(YerothERPStockImport::_allMissingMandatoryColumnValue);

				YerothQMessageBox::warning(_callingWindow,
						QObject::tr("colones obligatoires manquantes"),
						warnMesg);
			}
		}
	}

	return successImportCount;
}


enum import_csv_entry_row_return_status
	YerothERPStockImport::import_csv_entry_row(QStringList &aCsvFileEntryLine)
{
	enum import_csv_entry_row_return_status insertionReturnStatus = IMPORT_DATA_CSV_STOCK_INSERTION_FAILED;

	YerothERPWindows *allWindows = YerothUtils::getAllWindows();

	if (0 == allWindows)
	{
		return IMPORT_DATA_CSV_STOCK_INSERTION_FAILED;
	}

	YerothERPDatabaseTableColumnInfo *curDatabaseTableColumnInfo = 0;

	YerothSqlTableModel &curStocksTableModel = allWindows->getSqlTableModel_stocks();

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
												 QString::number(allWindows->getNextIdSqlTableModel_fournisseurs()),
												 curColumnRowEntry);

					if (!YerothUtils::execQuery(queryFournisseurStr))
					{
						QString infoMesg =
								QString(QObject::trUtf8("L'entreprise fournisseur '%1' ne pouvait pas être créée !"))
									.arg(curColumnRowEntry);

						if (0 != _callingWindow)
						{
							YerothQMessageBox::warning(_callingWindow,
									QObject::trUtf8("création d'une entreprise fournisseuse"),
									infoMesg);
						}
						else
						{
							qDebug() << infoMesg;
						}

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
							YerothQMessageBox::warning(_callingWindow,
									QObject::trUtf8("création de catégorie d'articles"),
									infoMesg);
						}
						else
						{
							qDebug() << infoMesg;
						}

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

			if (0 != _callingWindow)
			{
    			YerothQMessageBox::warning(_callingWindow,
    					QObject::trUtf8("aucune référence"),
						infoMesg);
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
    			YerothQMessageBox::warning(_callingWindow, "enregistrer", infoMesg);
			}
			else
			{
				qDebug() << infoMesg;
			}
    	}

        return IMPORT_DATA_CSV_STOCK_INSERTION_FAILED;
    }

    service_stock_already_exist_type serviceStockExists =
    		YerothUtils::isStockItemInProductList(false,
    											  productReference,
												  productCategorie,
												  productName);

    YerothERPServiceStockMarchandiseData aServiceStockData;

    aServiceStockData._categorie = productCategorie;
    aServiceStockData._designation = productName;
    aServiceStockData._reference = productReference;


    if (SERVICE_STOCK_UNDEFINED == serviceStockExists)
    {
    	if (!YerothUtils::insertStockItemInProductList(aServiceStockData))
    	{
    		return IMPORT_DATA_CSV_STOCK_INSERTION_FAILED;
    	}
    }
    else
    {
    	if (SERVICE_REFERENCE_EXISTS == serviceStockExists)
    	{
    		QString infoMesg =
					   QString(QObject::trUtf8("Un service (stock) "
							   	   	   	   	   "avec la référence '%1' existe déjà !"))
							.arg(productReference);

    		if (0 != _callingWindow)
    		{
    			YerothQMessageBox::warning(_callingWindow,
    					QObject::trUtf8("référence"),
    					infoMesg);
    		}
    		else
    		{
    			qDebug() << infoMesg;
    		}

    		return IMPORT_DATA_CSV_STOCK_INSERTION_FAILED;
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
    			YerothQMessageBox::warning(_callingWindow,
    					QObject::trUtf8("désignation"),
    					infoMesg);
    		}
    		else
    		{
    			qDebug() << infoMesg;
    		}

    		return IMPORT_DATA_CSV_STOCK_INSERTION_FAILED;
    	}
    }

	static bool quantite_totale_already_visited = false;

	if (quantite_totale <= 0 && !quantite_totale_already_visited)
	{
		QString infoMesg =
				QString(QObject::trUtf8("La colone '%1' a une valeur (%2) <= '0' !"))
					.arg(YerothDatabaseTableColumn::QUANTITE_TOTALE,
						 QString::number(quantite_totale));

		if (0 != _callingWindow)
		{
			YerothQMessageBox::warning(_callingWindow,
					QObject::tr("valeur incorrecte"),
					infoMesg);
		}
		else
		{
			qDebug() << infoMesg;
		}

		quantite_totale_already_visited = true;

		return IMPORT_DATA_CSV_STOCK_INCORRECT_COLUMN_VALUE;
	}

	if (montant_tva > -1 && prix_unitaire > 0)
	{
		prix_vente = prix_unitaire + montant_tva;
//		qDebug() << QString("++ prix_vente: %1")
//						.arg(QString::number(prix_vente));
		record.setValue(YerothDatabaseTableColumn::PRIX_VENTE, prix_vente);
	}

	record.setValue(YerothDatabaseTableColumn::IS_SERVICE, 0);
	record.setValue(YerothDatabaseTableColumn::LOTS_ENTRANT, 1);
	record.setValue(YerothDatabaseTableColumn::QUANTITE_PAR_LOT, quantite_totale);
	record.setValue(YerothDatabaseTableColumn::LOCALISATION,
			allWindows->getInfoEntreprise().getLocalisation());
	record.setValue(YerothDatabaseTableColumn::DATE_ENTREE, GET_CURRENT_DATE);


	if (0 != _dbTableColumnToIsNotNULL)
	{
		QString aCurSqlTableImportColumn;

		QStringList allSqltableColumns = _dbTableColumnToIsNotNULL->keys();

		allSqltableColumns.removeAll(YerothDatabaseTableColumn::PRIX_VENTE);
		allSqltableColumns.removeAll(YerothDatabaseTableColumn::IS_SERVICE);
		allSqltableColumns.removeAll(YerothDatabaseTableColumn::LOTS_ENTRANT);
		allSqltableColumns.removeAll(YerothDatabaseTableColumn::QUANTITE_PAR_LOT);
		allSqltableColumns.removeAll(YerothDatabaseTableColumn::LOCALISATION);
		allSqltableColumns.removeAll(YerothDatabaseTableColumn::DATE_ENTREE);

	    QString historiqueStockInitial(
	    		YerothHistoriqueStock::creer_mouvement_stock(ENTREE,
	    				stock_id_to_save,
						GET_CURRENT_DATE,
						0.0,
						quantite_totale,
						quantite_totale));

	    //qDebug() << QString("++ test: %1")
	        		//				.arg(historiqueStockInitial);

	    record.setValue(YerothDatabaseTableColumn::HISTORIQUE_STOCK, historiqueStockInitial);

		for (int k = 0; k < allSqltableColumns.size(); ++k)
		{
			aCurSqlTableImportColumn = allSqltableColumns.at(k);

			if (allImportedTableColumns.contains(aCurSqlTableImportColumn))
			{
				continue;
			}

			if (false == _dbTableColumnToIsNotNULL->value(aCurSqlTableImportColumn))
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


void YerothERPStockImport::missing_mandatory_item_field_msg(const QString &aMandatoryColumn)
{
	QString warnMesg =
			QString(QObject::trUtf8("La colone OBLIGATOIRE '%1' est manquante !"))
				.arg(aMandatoryColumn);

	if (0 != _callingWindow)
	{
		YerothQMessageBox::warning(_callingWindow,
				QObject::tr("colone OBLIGATOIRE manquante"),
				warnMesg);
	}
	else
	{
		qDebug() << warnMesg;
	}
}


bool YerothERPStockImport::check_mandatory_item_field()
{
	QString curMandatoryTableColumn;

	for (int j = 0; j < _allMandatoryTableColumns.size(); ++j)
	{
		curMandatoryTableColumn = _allMandatoryTableColumns.at(j);

		if (!_allSqlTableImportColumns.contains(curMandatoryTableColumn))
		{
			missing_mandatory_item_field_msg(curMandatoryTableColumn);

			return false;
		}
	}

	return true;
}
