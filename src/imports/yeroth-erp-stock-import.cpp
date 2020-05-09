/*
 * yeroth-erp-stock-import.hpp
 *
 *      Author: Xavier NOUMBISSI NOUNDOU, Dipl.-Inf., Ph.D. (ABD)
 */

#include "yeroth-erp-stock-import.hpp"


#include "src/utils/yeroth-erp-database-table-column.hpp"

#include "src/widgets/yeroth-erp-qmessage-box.hpp"


#include <QtCore/QDebug>


YerothERPStockImport::YerothERPStockImport(YerothPOSAdminWindowsCommons 	&aCallingWindow,
		 	 	 	 	 	 	 	 	   QStringList 				  		&aCurCsvFileToImportContentWordList,
										   QMap<int, YerothComboBox *> 		&anIndexToSQLTableImportHeader)
:_curCsvFileToImportContentWordList(&aCurCsvFileToImportContentWordList),
 _indexToSQLTableImportHeader(&anIndexToSQLTableImportHeader),
 _callingWindow(&aCallingWindow)
{

}


int YerothERPStockImport::import()
{
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

	YerothComboBox *aMappedComboBox = 0;

	int curCsvFileColumnSize = csvHeader.size();

	/*
	 * check if '2 csv columns' are mapped to
	 * a same database table import-column.
	 */
	for (int i = 0; i < curCsvFileColumnSize; ++i)
	{
		aMappedComboBox = _indexToSQLTableImportHeader->value(i);

		if (0 != aMappedComboBox)
		{
			curSqlTableImportHeaderStr = aMappedComboBox->currentText();

			if (!_allSqlTableImportColumns.contains(curSqlTableImportHeaderStr, Qt::CaseInsensitive))
			{
				_allSqlTableImportColumns.append(curSqlTableImportHeaderStr);
			}
			else
			{
				infoMesg = QString(QObject::trUtf8("La colone '%1' apparait "
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

	QStringList curCsvFileImportRow;

	for (int k = 1; k < curCsvFileLineCount; ++k)
	{
		curCsvFileImportRow = _curCsvFileToImportContentWordList->at(k)
						.split(YerothUtils::SEMI_COLON_STRING_CHAR);

		if (import_csv_entry_row(curCsvFileImportRow))
		{
			++successImportCount;
		}
	}

	return successImportCount;
}

bool YerothERPStockImport::import_csv_entry_row(QStringList &aCsvFileEntryLine)
{
//	qDebug() << "++ aCsvFileEntryLine: " << aCsvFileEntryLine;
//
//
//	qDebug() << QString("++ aCsvFileEntryLine.size(): %1")
//					.arg(QString::number(aCsvFileEntryLine.size()));


	for (int j = 0; j < aCsvFileEntryLine.size(); ++j)
	{

	}

	return false;
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
	if (!_allSqlTableImportColumns.contains(YerothDatabaseTableColumn::DESIGNATION))
	{
		missing_mandatory_item_field_msg(YerothDatabaseTableColumn::DESIGNATION);
		return false;
	}

	if (!_allSqlTableImportColumns.contains(YerothDatabaseTableColumn::CATEGORIE))
	{
		missing_mandatory_item_field_msg(YerothDatabaseTableColumn::CATEGORIE);
		return false;
	}

	if (!_allSqlTableImportColumns.contains(YerothDatabaseTableColumn::QUANTITE_TOTAL))
	{
		missing_mandatory_item_field_msg(YerothDatabaseTableColumn::QUANTITE_TOTAL);
		return false;
	}

	if (!_allSqlTableImportColumns.contains(YerothDatabaseTableColumn::PRIX_UNITAIRE))
	{
		missing_mandatory_item_field_msg(YerothDatabaseTableColumn::PRIX_UNITAIRE);
		return false;
	}

	return true;
}
