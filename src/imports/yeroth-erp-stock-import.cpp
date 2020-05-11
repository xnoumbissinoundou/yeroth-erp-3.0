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


YerothERPStockImport::YerothERPStockImport(YerothPOSAdminWindowsCommons 					&aCallingWindow,
		 	 	 	 	 	 	 	 	   QStringList 				  						&aCurCsvFileToImportContentWordList,
										   QMap<int, YerothERPDatabaseTableColumnInfo *> 	&anIndexToDatabaseTableColumnInfo)
:_curCsvFileToImportContentWordList(&aCurCsvFileToImportContentWordList),
 _indexToDatabaseTableColumnInfo(&anIndexToDatabaseTableColumnInfo),
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

	QString infoMesg;

	if (_curCsvFileToImportContentWordList->size() <= 1)
	{
		infoMesg = QObject::trUtf8("Le fichier CSV n'a pas de données à importer !");

		YerothQMessageBox::information(_callingWindow,
									   QObject::tr("fichier CSV vide"),
									   infoMesg);

		return 0;
	}

	QStringList csvHeader = _curCsvFileToImportContentWordList->at(0)
					.split(YerothUtils::SEMI_COLON_STRING_CHAR);

	QString curSqlTableImportHeaderStr;

	YerothERPDatabaseTableColumnInfo *curDatabaseTableColumnInfo = 0;

	int curCsvFileColumnSize = csvHeader.size();

	/*
	 * check if '2 csv columns' are mapped to
	 * a same database table import-column.
	 */

	for (int i = 0; i < curCsvFileColumnSize; ++i)
	{
		curDatabaseTableColumnInfo = _indexToDatabaseTableColumnInfo->value(i);

		if (0 != curDatabaseTableColumnInfo)
		{
			curSqlTableImportHeaderStr = curDatabaseTableColumnInfo->getColumnName();

			if (!_allSqlTableImportColumns.contains(curSqlTableImportHeaderStr, Qt::CaseInsensitive))
			{
				_allSqlTableImportColumns.append(curSqlTableImportHeaderStr);
			}
			else
			{
				infoMesg = QString(QObject::trUtf8("La colone '%1' apparaît "
												   "plusieurs fois parmis les "
												   "colones d'importation !"))
								.arg(curSqlTableImportHeaderStr);

				YerothQMessageBox::information(_callingWindow,
						QObject::tr("colones d'importation multiples"),
						infoMesg);

				return 0;
			}
		}
	}

	if (!check_mandatory_item_field())
	{
		return 0;
	}

	int curCsvFileLineCount = _curCsvFileToImportContentWordList->size() - 1;

	/*
	 * We start at index 1 so to avoid the first line
	 * that represents the header content, and not data !
	 */
	int successImportCount = 0;

	enum import_csv_entry_row_return_status insertionReturnStatusValue = UNDEFINED;

	QStringList curCsvFileImportRow;

	for (int k = 1; k < curCsvFileLineCount; ++k)
	{
		curCsvFileImportRow = _curCsvFileToImportContentWordList->at(k)
						.split(YerothUtils::SEMI_COLON_STRING_CHAR);

		insertionReturnStatusValue = import_csv_entry_row(curCsvFileImportRow);

		if (INSERTION_SUCCEED == insertionReturnStatusValue)
		{
			++successImportCount;
		}
	}

	if (successImportCount != curCsvFileLineCount)
	{
		if (MANDATORY_COLUMN_VALUE_MISSING == insertionReturnStatusValue)
		{
			YerothERPStockImport::_allMissingMandatoryColumnValue.replace(0, 3, YerothUtils::EMPTY_STRING);

			QString warnMesg =
					QString(QObject::tr("Les colones obligatoires "
										"suivantes '%1' sont manquantes !"))
							.arg(YerothERPStockImport::_allMissingMandatoryColumnValue);

			YerothQMessageBox::information(_callingWindow,
					QObject::tr("colones obligatoires manquantes"),
					warnMesg);
		}
	}

	return successImportCount;
}

enum import_csv_entry_row_return_status
	YerothERPStockImport::import_csv_entry_row(QStringList &aCsvFileEntryLine)
{
//	qDebug() << "++ aCsvFileEntryLine: " << aCsvFileEntryLine;
//
//
//	qDebug() << QString("++ aCsvFileEntryLine.size(): %1")
//					.arg(QString::number(aCsvFileEntryLine.size()));

	enum import_csv_entry_row_return_status insertionReturnStatus = INSERTION_FAILED;

	YerothERPWindows *allWindows = YerothUtils::getAllWindows();

	if (0 == allWindows)
	{
		return INSERTION_FAILED;
	}

	YerothERPDatabaseTableColumnInfo *curDatabaseTableColumnInfo = 0;

	YerothSqlTableModel &curStocksTableModel = allWindows->getSqlTableModel_stocks();

    QSqlRecord record = curStocksTableModel.record();

    double montant_tva = -1;
    double prix_unitaire = -1;
    double prix_vente = -1;
    double quantite_totale = -1;

    int stock_id_to_save = YerothERPWindows::getNextIdSqlTableModel_stocks();

    int querySize = -1;

	record.setValue(YerothDatabaseTableColumn::ID, stock_id_to_save);

	QStringList allImportedTableColumns;

	QString curTableColumnType;
	QString curTableColumnName;
	QString curColumnRowEntry;

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

		curDatabaseTableColumnInfo = _indexToDatabaseTableColumnInfo->value(j);

		if (0 != curDatabaseTableColumnInfo)
		{
			curTableColumnType = curDatabaseTableColumnInfo->getColumnType();

			curTableColumnName = curDatabaseTableColumnInfo->getColumnName();

			if (_allMandatoryTableColumns.contains(curTableColumnName)     &&
				YerothUtils::isEqualCaseInsensitive(YerothDatabaseTableColumn::CATEGORIE, curTableColumnName))
			{
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

						YerothQMessageBox::information(_callingWindow,
													   QObject::tr("création de catégorie d'articles"),
													   infoMesg);

						return INSERTION_FAILED;
					}
				}
			}


			if (YEROTH_QSTRING_CONTAINS(curTableColumnType, "int"))
			{
				record.setValue(curTableColumnName, curColumnRowEntry.toInt());
			}
			else if (YEROTH_QSTRING_CONTAINS(curTableColumnType, "double"))
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
			else if (YEROTH_QSTRING_CONTAINS(curTableColumnType, "date"))
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

	static bool quantite_totale_already_visited = false;

	if (quantite_totale <= 0 && !quantite_totale_already_visited)
	{
		QString infoMesg =
				QString(QObject::trUtf8("La colone '%1' a une valeur <= '0' !"))
					.arg(YerothDatabaseTableColumn::QUANTITE_TOTALE);

		YerothQMessageBox::information(_callingWindow,
									   QObject::tr("valeur incorrecte"),
									   infoMesg);

		quantite_totale_already_visited = true;

		return INCORRECT_COLUMN_VALUE;
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

	if (0 != _dbTableColumnToIsNotNULL)
	{
		QString aCurSqlTableImportColumn;

		QStringList allSqltableColumns = _dbTableColumnToIsNotNULL->keys();

		allSqltableColumns.removeAll(YerothDatabaseTableColumn::PRIX_VENTE);
		allSqltableColumns.removeAll(YerothDatabaseTableColumn::IS_SERVICE);
		allSqltableColumns.removeAll(YerothDatabaseTableColumn::LOTS_ENTRANT);
		allSqltableColumns.removeAll(YerothDatabaseTableColumn::QUANTITE_PAR_LOT);

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
				insertionReturnStatus = MANDATORY_COLUMN_VALUE_MISSING;
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

		if (MANDATORY_COLUMN_VALUE_MISSING == insertionReturnStatus)
		{
			return MANDATORY_COLUMN_VALUE_MISSING;
		}
	}

	bool queryResut = curStocksTableModel.insertNewRecord(record);

	if (queryResut)
	{
		return INSERTION_SUCCEED;
	}

	return INSERTION_FAILED;
}


void YerothERPStockImport::missing_mandatory_item_field_msg(const QString &aMandatoryColumn)
{
	QString infoMesg =
			QString(QObject::trUtf8("La colone OBLIGATOIRE '%1' est manquante !"))
				.arg(aMandatoryColumn);

	YerothQMessageBox::information(_callingWindow,
								   QObject::tr("colone OBLIGATOIRE manquante"),
								   infoMesg);
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