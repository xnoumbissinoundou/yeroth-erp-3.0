/*
 * yeroth-erp-stocks-table-view.cpp
 *
 *      Author: Xavier NOUMBISSI NOUNDOU, Dipl.-Inf., Ph.D. (ABD)
 */


#include "yeroth-erp-stocks-table-view.hpp"

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


YerothERPStocksTableView::YerothERPStocksTableView()
:YerothTableView()
{
	_stdItemModel->_curTableView = this;
}

YerothERPStocksTableView::YerothERPStocksTableView(QWidget * parent)
:YerothTableView(parent)
{
	_stdItemModel->_curTableView = this;
}

YerothERPStocksTableView::~YerothERPStocksTableView()
{
}

void YerothERPStocksTableView::lister_les_elements_du_tableau(YerothSqlTableModel &tableModel,
															  YerothWindowsCommons *aCallingWindows)
{
	_stdItemModel->_curSqlTableModel = &tableModel;

    emit signal_lister(tableModel);

    bool s = tableModel.select();

    int rows = tableModel.rowCount();
    int columns = tableModel.columnCount();

    _stdItemModel->setRowCount(rows);
    _stdItemModel->setColumnCount(columns);

    YerothUtils::createTableModelHeaders(tableModel, *_stdItemModel, *_tableModelHeaders);

	QString tmpQvString;
    QStandardItem *anItem = 0;
    QVariant qv;

    if(s)
    {
        QSqlRecord record;
        QVariant date_premption;
        QVariant quantite_totale;
        QVariant stock_dalerte;

        for (int i = 0; i < rows; ++i)
        {
            record = tableModel.record(i);
            date_premption = record.value(YerothDatabaseTableColumn::DATE_PEREMPTION);
            quantite_totale = record.value(YerothDatabaseTableColumn::QUANTITE_TOTALE);
            stock_dalerte = record.value(YerothDatabaseTableColumn::STOCK_DALERTE);

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
                    //quantite_totale (c'est la quantite restante en stock)

					if (0 != aCallingWindows)
					{
						if (YEROTH_DATABASE_TABLE_COLUMN_INDEX((*aCallingWindows), YerothDatabaseTableColumn::QUANTITE_TOTALE) == k)
						{
							anItem = new YerothQStandardItem(GET_NUM_STRING(qv.toUInt()));
						}
						else
						{
							anItem = new YerothQStandardItem(GET_DOUBLE_STRING(qv.toDouble()));
						}
					}

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

					if (0 != aCallingWindows)
					{
						if (YEROTH_DATABASE_TABLE_COLUMN_INDEX((*aCallingWindows), YerothDatabaseTableColumn::REFERENCE) != k 			 ||
							YEROTH_DATABASE_TABLE_COLUMN_INDEX((*aCallingWindows), YerothDatabaseTableColumn::REFERENCE_RECU_DACHAT) != k	)
						{
							YerothUtils::YEROTH_TRUNCATE_STRING_ACCORDING_TO_SETTING(tmpQvString);
						}
					}

                    anItem = new YerothQStandardItem(tmpQvString);
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
                    //qDebug() << "YerothERPStocksTableView::lister(): undecoded QVariant -> " << qv.type();
                    break;
                }

                if (anItem)
                {
                    anItem->setForeground(Qt::white);

					if (0 != aCallingWindows)
					{
						if (YEROTH_DATABASE_TABLE_COLUMN_INDEX((*aCallingWindows), YerothDatabaseTableColumn::QUANTITE_TOTALE) == k &&
								quantite_totale.toDouble() <= stock_dalerte.toDouble())
						{
							anItem->setForeground(YerothUtils::YEROTH_RED_COLOR);
						}

						if (YEROTH_DATABASE_TABLE_COLUMN_INDEX((*aCallingWindows), YerothDatabaseTableColumn::DATE_PEREMPTION) == k &&
								date_premption.toDate() <= GET_CURRENT_DATE)
						{
							anItem->setForeground(YerothUtils::YEROTH_RED_COLOR);
						}
					}
                }
            }
        }

        this->resizeColumnsToContents();
    }
}

