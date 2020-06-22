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


YerothERPMarchandisesTableView::YerothERPMarchandisesTableView()
:YerothTableView(),
 _allWindows(YerothUtils::getAllWindows())
{
	_stdItemModel->_curTableView = this;
}


YerothERPMarchandisesTableView::YerothERPMarchandisesTableView(QWidget * parent)
:YerothTableView(parent),
 _allWindows(YerothUtils::getAllWindows())
{
	_stdItemModel->_curTableView = this;
}


void YerothERPMarchandisesTableView::lister_les_elements_du_tableau(YerothSqlTableModel &tableModel)
{
	_stdItemModel->_curSqlTableModel = &tableModel;

	_allWindows->_marchandisesWindow->_valeurTheoriqueDinventaire = 0.0;

	_allWindows->_marchandisesWindow->_qteTotaleDarticlesEnStock = 0.0;

    emit signal_lister(tableModel);

    bool s = tableModel.select();

    int rows = tableModel.rowCount();

    int columns = tableModel.columnCount();

    _stdItemModel->setRowCount(rows);

    _stdItemModel->setColumnCount(columns);

    //qDebug() << "++  rows: " << rows;

    if (tableModel.isFromQSqlQuery() && rows <= 0)
    {
    	_stdItemModel->yerothPOSClear();

    	//qDebug() << "++  yerothPOSClear";
    }

    QStringList	tableModelRawHeaders;

    YerothUtils::createTableModelHeaders(tableModel,
    									 *_stdItemModel,
										 *_tableModelHeaders,
										 tableModelRawHeaders);

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
                    anItem = new YerothQStandardItem(GET_NUM_STRING(qv.toUInt()));
                    _stdItemModel->setItem(i, k, anItem);
                    break;

                case QVariant::Int:
                    anItem = new YerothQStandardItem(GET_NUM_STRING(qv.toInt()));
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

                	curTableModelRawHdr = tableModelRawHeaders.at(k);

                	if (!YerothUtils::isEqualCaseInsensitive(curTableModelRawHdr, YerothDatabaseTableColumn::REFERENCE))
                	{
                		anItem = new YerothQStandardItem(YerothUtils::YEROTH_TRUNCATE_STRING_ACCORDING_TO_SETTING(tmpQvString));
                	}
                	else
                	{
                		anItem = new YerothQStandardItem(tmpQvString);
                	}

                    _stdItemModel->setItem(i, k, anItem);
                    break;

                case QVariant::Bool:
                    anItem = new YerothQStandardItem(qv.toBool() ? "True" : "False");
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

                		anItem->setForeground(YerothUtils::YEROTH_RED_COLOR);
                	}

                	curTableModelRawHdr = tableModelRawHeaders.at(k);

                	if (YerothUtils::isEqualCaseInsensitive(curTableModelRawHdr, YerothDatabaseTableColumn::QUANTITE_TOTALE))
                	{
                		double qteTotalEnStock =
                				YerothMarchandisesWindow::getQuantiteTotaleEnStock(categorieStr,
                						designationStr);

                		_allWindows->_marchandisesWindow->_qteTotaleDarticlesEnStock += qteTotalEnStock;

                		anItem->setText(GET_DOUBLE_STRING(qteTotalEnStock));
                	}
                	else if (YerothUtils::isEqualCaseInsensitive(curTableModelRawHdr, YerothDatabaseTableColumn::VALEUR_DIVENTAIRE))
                	{
                		double valeurDinventaireEnStock =
                				YerothMarchandisesWindow::getValeurTotaleDinventaireEnStock(categorieStr,
                						designationStr);

                		_allWindows->_marchandisesWindow->_valeurTheoriqueDinventaire += valeurDinventaireEnStock;

                		anItem->setText(GET_DOUBLE_STRING(valeurDinventaireEnStock));
                	}
                } //if (0 != item)
            }
        }
    }

    this->resizeColumnsToContents();
}

