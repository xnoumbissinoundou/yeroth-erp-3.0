/*
 * yeroth-erp-marchandises-table-view.cpp
 *
 *      Author: Dipl.-Inf. XAVIER NOUMBISSI NOUNDOU
 */


#include "yeroth-erp-marchandises-table-view.hpp"

#include "src/yeroth-erp-windows.hpp"

#include "src/utils/yeroth-erp-database-table-column.hpp"

#include "src/utils/yeroth-erp-historique-stock.hpp"

#include "src/utils/yeroth-erp-utils.hpp"

#include "src/utils/yeroth-erp-config.hpp"

#include "src/utils/yeroth-erp-sqltable-model.hpp"

#include "src/widgets/yeroth-erp-qstandard-item.hpp"


#include <QtCore/QDebug>

#include <QtCore/QDate>

#include <QtCore/QMap>

#include <QtSql/QSqlQuery>

#include <QtSql/QSqlRecord>

#include <QtSql/QSqlError>


void YerothERPMarchandisesTableView::lister_les_elements_du_tableau(YerothSqlTableModel &tableModel,
																	QString aStockListingStrategy /* = "" */)
{
	_stdItemModel->_curSqlTableModel = &tableModel;

	YEROTH_TABLE_VIEW_ALL_WINDOWS_POINTER->_marchandisesWindow->_valeurTheoriqueDinventaire = 0.0;

	YEROTH_TABLE_VIEW_ALL_WINDOWS_POINTER->_marchandisesWindow->_qteTotaleDarticlesEnStock = 0.0;

    emit signal_lister(tableModel);

    bool s = true;

    if (_needExecSelectStatement_FOR_TABLE_VIEW_PAGING_LISTING)
    {
    	s = tableModel.select();
    }

    int rows = tableModel.rowCount();

    int columns = tableModel.columnCount();

    _stdItemModel->setRowCount(rows);

    _stdItemModel->setColumnCount(columns);

//    qDebug() << "++  rows: " << rows;

    if (tableModel.isFromQSqlQuery() && rows <= 0)
    {
    	_stdItemModel->yerothPOSClear();

    	//qDebug() << "++  yerothPOSClear";
    }

    YerothUtils::createTableModelHeaders(tableModel,
    									 *_stdItemModel,
										 *_tableModelHeaders,
										 _tableModelRawHeaders_IN_OUT);

    QString curTableModelRawHdr;

	QString tmpQvString;

	QStandardItem *anItem = 0;

	QVariant qv;

    if(tableModel.isFromQSqlQuery() || s)
    {
    	int querySize = 0;

    	QString designationStr;
    	QString categorieStr;

    	QStringList aStringList;

        QString sqlSearchStockTableQueryStr;
        QSqlQuery sqlSearchStockTableQuery;

        QSqlRecord record;
        QVariant designation;
        QVariant categorie;

        for (int i = 0; i < rows; ++i)
        {
            record = tableModel.record(i);

            designation = record.value(YerothDatabaseTableColumn::DESIGNATION);

            categorie = record.value(YerothDatabaseTableColumn::CATEGORIE);

            for (int k = 0; k < columns; ++k)
            {
            	curTableModelRawHdr = _tableModelRawHeaders_IN_OUT.at(k);

                qv.setValue(tableModel.record(i).value(k));

                anItem = _stdItemModel->item(i, k);

                if (anItem)
                {
                    delete anItem;
                }

                anItem = new YerothQStandardItem;

                switch (qv.type())
                {
                case QVariant::UInt:

                	if (!YerothUtils::isEqualCaseInsensitive(curTableModelRawHdr, YerothDatabaseTableColumn::ID))
                	{
                		anItem = new YerothQStandardItem(GET_NUM_STRING(qv.toUInt()));
                	}
                	else
                	{
                		anItem = new YerothQStandardItem(QString::number(qv.toUInt()));
                	}

                    _stdItemModel->setItem(i, k, anItem);
                    break;

                case QVariant::Int:

                	if (!YerothUtils::isEqualCaseInsensitive(curTableModelRawHdr, YerothDatabaseTableColumn::ID))
                	{
                		anItem = new YerothQStandardItem(GET_NUM_STRING(qv.toInt()));
                	}
                	else
                	{
                		anItem = new YerothQStandardItem(QString::number(qv.toInt()));
                	}

                    _stdItemModel->setItem(i, k, anItem);
                    break;

                case QVariant::Double:
                    anItem = new YerothQStandardItem(GET_DOUBLE_STRING(qv.toDouble()));
                    _stdItemModel->setItem(i, k, anItem);
                    break;

                case QVariant::ULongLong:
                    anItem = new YerothQStandardItem(GET_NUM_STRING(qv.toULongLong()));
                    _stdItemModel->setItem(i, k, anItem);
                    break;

                case QVariant::LongLong:
                    anItem = new YerothQStandardItem(GET_NUM_STRING(qv.toLongLong()));
                    _stdItemModel->setItem(i, k, anItem);
                    break;

                case QVariant::Char:
                    anItem = new YerothQStandardItem(QString(qv.toChar()));
                    _stdItemModel->setItem(i, k, anItem);
                    break;

                case QVariant::String:
                	tmpQvString.clear();
                	tmpQvString.append(qv.toString());

                	if (!YerothUtils::isEqualCaseInsensitive(curTableModelRawHdr, YerothDatabaseTableColumn::REFERENCE))
                	{
                		anItem = new YerothQStandardItem(tmpQvString, false);
                	}
                	else
                	{
                		anItem = new YerothQStandardItem(tmpQvString);
                	}

                    _stdItemModel->setItem(i, k, anItem);
                    break;

                case QVariant::Bool:
                    anItem = new YerothQStandardItem(qv.toBool() ? BOOLEAN_STRING_TRUE : BOOLEAN_STRING_FALSE);
                    _stdItemModel->setItem(i, k, anItem);
                    break;

                case QVariant::Date:
                    anItem = new YerothQStandardItem(DATE_TO_STRING(qv.toDate()));
                    _stdItemModel->setItem(i, k, anItem);
                    break;

                case QVariant::Time:
                    anItem = new YerothQStandardItem(TIME_TO_STRING(qv.toTime()));
                    _stdItemModel->setItem(i, k, anItem);
                    break;

                default:
					anItem = new YerothQStandardItem(YerothUtils::EMPTY_STRING);
					_stdItemModel->setItem(i, k, anItem);
                    //qDebug() << "YerothTableView::lister(): undecoded QVariant -> " << qv.type();
                    break;
                }

                if (0 != anItem)
                {
                	//We query the database once, only at the first column of a row
                	if (0 == k)
                	{
                		sqlSearchStockTableQuery.clear();

                    	designationStr = designation.toString();
                    	categorieStr = categorie.toString();

                		sqlSearchStockTableQueryStr =
                				QString("SELECT * FROM %1 WHERE %2 = '%3' AND %4 = '%5'")
								.arg(YerothUtils::getAllWindows()->STOCKS,
										YerothDatabaseTableColumn::CATEGORIE,
										categorieStr,
										YerothDatabaseTableColumn::DESIGNATION,
										designationStr);

                		querySize = YerothUtils::execQuery(sqlSearchStockTableQuery,
                										   sqlSearchStockTableQueryStr);
                	}

                	if (0 >= querySize)
                	{
                		anItem->setAccessibleText(QString("%1|%2")
                									.arg(categorieStr,
                										 designationStr));
                	}

                	if (YerothUtils::isEqualCaseInsensitive(curTableModelRawHdr, YerothDatabaseTableColumn::QUANTITE_TOTALE))
                	{
                		double qteTotalEnStock =
                				YerothMarchandisesWindow::getQuantiteTotaleEnStock(categorieStr,
                						designationStr);

                		YEROTH_TABLE_VIEW_ALL_WINDOWS_POINTER->_marchandisesWindow->_qteTotaleDarticlesEnStock += qteTotalEnStock;

                		anItem->setText(GET_DOUBLE_STRING(qteTotalEnStock));
                	}
                	else if (YerothUtils::isEqualCaseInsensitive(curTableModelRawHdr, YerothDatabaseTableColumn::VALEUR_DIVENTAIRE))
                	{
                		double valeurDinventaireEnStock =
                				YerothMarchandisesWindow::getValeurTotaleDinventaireEnStock(categorieStr,
                						designationStr);

                		YEROTH_TABLE_VIEW_ALL_WINDOWS_POINTER->_marchandisesWindow->_valeurTheoriqueDinventaire += valeurDinventaireEnStock;

                		anItem->setText(GET_DOUBLE_STRING(valeurDinventaireEnStock));
                	}
                } //if (0 != item)
            }
        }
    }

    this->resizeColumnsToContents();
}


void YerothERPMarchandisesTableView::startEditingModeSelection()
{
	setEditTriggers(QAbstractItemView::SelectedClicked |
					QAbstractItemView::DoubleClicked);

	setWriteEnabled(true);

	QObject::connect(_stdItemModel,
					 SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &)),
					 this,
					 SLOT(dataChanged(const QModelIndex &, const QModelIndex &)));
}


void YerothERPMarchandisesTableView::dataChanged(const QModelIndex &index,
                                  	  	  	  	 const QModelIndex &bottomRight,
												 const QVector<int> &roles /*= QVector<int>()*/)
{
//	QDEBUG_STRINGS_OUTPUT_2_N("bottomRight.column()", bottomRight.column());

    if (index != bottomRight)
    {
    	return ;
    }

    if (!_writeEnabled)
    {
    	return ;
    }

    if (0 == _stdItemModel->item(index.row(), 0))
    {
    	return ;
    }

    QString curIDText(_stdItemModel->item(index.row(), 0)->text());


//    int curTableViewHeaderModifiedAttemptedColumnIdx = bottomRight.column();

    QString columnHeaderText_VISIBLE(_tableModelHeaders->at(index.column()));

    QString columnHeaderText_NATIVE(_tableModelRawHeaders_IN_OUT.at(index.column()));

    if (!YerothUtils::isEqualCaseInsensitive(columnHeaderText_NATIVE,
    		YerothDatabaseTableColumn::REFERENCE) 					&&

    	!YerothUtils::isEqualCaseInsensitive(columnHeaderText_NATIVE,
    		YerothDatabaseTableColumn::PRIX_DACHAT_PRECEDENT) 		&&

		!YerothUtils::isEqualCaseInsensitive(columnHeaderText_NATIVE,
			YerothDatabaseTableColumn::PRIX_VENTE_PRECEDENT))
    {
    	YerothQMessageBox::information(YEROTH_TABLE_VIEW_ALL_WINDOWS_POINTER->_marchandisesWindow,
    								   QObject::tr("modification non possible"),
									   QObject::trUtf8("modification non possible de la colone '%1' !")
    												.arg(columnHeaderText_VISIBLE));

    	return ;
    }


    QString cellTextData;

    QString designationText;

    QString REAL_DB_ID_NAME_marchandiseTableColumnProperty(
    		YerothDatabaseTableColumn::_tableColumnToUserViewString.key(columnHeaderText_VISIBLE));

    bool success = false;

    QSqlQuery mySqlQuery;

    QString prevValueDataString;

    QVariant qvIndexData = index.data();

    cellTextData = YerothUtils::get_text(qvIndexData);

    QString strSelectPreviousValueQuery(QString("SELECT %1, %2 FROM %3 WHERE %4 = '%5'")
    		.arg(REAL_DB_ID_NAME_marchandiseTableColumnProperty,
    				YerothDatabaseTableColumn::DESIGNATION,
					*_tableName,
					YerothDatabaseTableColumn::ID,
					curIDText));

    QString strUpdateMarchandisesTableQuery(QString("UPDATE %1 SET %2 = '%3' WHERE %4 = '%5'")
    		.arg(*_tableName,
    				REAL_DB_ID_NAME_marchandiseTableColumnProperty,
					cellTextData,
					YerothDatabaseTableColumn::ID,
					curIDText));

    YEROTH_ERP_3_0_START_DATABASE_TRANSACTION;

    int mySqlQuerySize = YerothUtils::execQuery(mySqlQuery, strSelectPreviousValueQuery);

    if (mySqlQuerySize > 0)
    {
    	mySqlQuery.next();

    	prevValueDataString = mySqlQuery.value(0).toString();

    	designationText = mySqlQuery.value(1).toString();

    	mySqlQuery.clear();
    }

    //    		QDEBUG_STRINGS_OUTPUT_2(QString::number(mySqlQuerySize), strUpdateMarchandisesTableQuery);

    success = YerothUtils::execQuery(strUpdateMarchandisesTableQuery);

    if (success &&
    		YerothUtils::isEqualCaseInsensitive(columnHeaderText_NATIVE,
    				YerothDatabaseTableColumn::REFERENCE))
    {
    	QStringList allToUpdateTables;

    	allToUpdateTables << YEROTH_TABLE_VIEW_ALL_WINDOWS_POINTER->PAIEMENTS
    			<< YEROTH_TABLE_VIEW_ALL_WINDOWS_POINTER->STOCKS
				<< YEROTH_TABLE_VIEW_ALL_WINDOWS_POINTER->STOCKS_SORTIES
				<< YEROTH_TABLE_VIEW_ALL_WINDOWS_POINTER->STOCKS_VENDU
				<< YEROTH_TABLE_VIEW_ALL_WINDOWS_POINTER->ACHATS;

    	QString strUpdateTableQuery;

    	for (uint i = 0; i < allToUpdateTables.size(); ++i)
    	{
    		strUpdateTableQuery.clear();

    		strUpdateTableQuery.append(QString("UPDATE %1 SET %2 = '%3' WHERE %4 = '%5'")
    				.arg(allToUpdateTables.at(i),
    						YerothDatabaseTableColumn::REFERENCE,
							cellTextData,
							YerothDatabaseTableColumn::REFERENCE,
							prevValueDataString));

    		success = YerothUtils::execQuery(strUpdateTableQuery,
    				YEROTH_TABLE_VIEW_ALL_WINDOWS_POINTER->_marchandisesWindow->getLogger()) && success;
    	}
    }

    YEROTH_ERP_3_0_COMMIT_DATABASE_TRANSACTION;


    if (success)
    {
    	QString succesMsgBoxTitle(QObject::trUtf8("succès modification (%1) ")
    	.arg(columnHeaderText_NATIVE));

    	YerothQMessageBox::information(YEROTH_TABLE_VIEW_ALL_WINDOWS_POINTER->_marchandisesWindow,
    			succesMsgBoxTitle,
				QObject::trUtf8("Succès de la modification de la colone '%1' (%2) "
						"de la marchandise '%3' !")
    	.arg(columnHeaderText_NATIVE,
    			cellTextData,
				designationText),
				QMessageBox::Ok);
    }
    else
    {
    	QString echecMsgBoxTitle(QObject::trUtf8("échec modification (%1) ")
    	.arg(columnHeaderText_NATIVE));

    	YerothQMessageBox::information(YEROTH_TABLE_VIEW_ALL_WINDOWS_POINTER->_marchandisesWindow,
    			echecMsgBoxTitle,
				QObject::trUtf8("Échec de la modification de la colone '%1' (%2) "
						"de la marchandise '%3' !")
    	.arg(columnHeaderText_NATIVE,
    			cellTextData,
				designationText),
				QMessageBox::Ok);
    }

    stopEditingModeSelection();
}


void YerothERPMarchandisesTableView::stopEditingModeSelection()
{
	setWriteEnabled(false);

	setEditTriggers(QAbstractItemView::NoEditTriggers);

	QObject::disconnect(_stdItemModel,
			   	   	    SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &)),
						this,
						SLOT(dataChanged(const QModelIndex &, const QModelIndex &)));
}
