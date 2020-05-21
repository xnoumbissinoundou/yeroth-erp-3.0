/*
 * yeroth-erp-supplier-table-view.cpp
 *
 *      Author: Xavier NOUMBISSI NOUNDOU, Dipl.-Inf., Ph.D. (ABD)
 */


#include "yeroth-erp-supplier-table-view.hpp"

#include "src/yeroth-erp-windows.hpp"

#include "src/utils/yeroth-erp-database-table-column.hpp"

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


void YerothERPSupplierTableView::lister_les_elements_du_tableau(YerothSqlTableModel &tableModel)
{
	_stdItemModel->_curSqlTableModel = &tableModel;

	emit signal_lister(tableModel);

	bool s = tableModel.select();

	int rows = tableModel.rowCount();
	int columns = tableModel.columnCount();

	_stdItemModel->setRowCount(rows);
	_stdItemModel->setColumnCount(columns);

	QStringList	tableModelRawHeaders;

    YerothUtils::createTableModelHeaders(tableModel,
    									 *_stdItemModel,
										 *_tableModelHeaders,
										 tableModelRawHeaders);

	QStandardItem *anItem = 0;
	QVariant qv;

	if(s)
	{
		QSqlRecord record;

		for (int i = 0; i < rows; ++i)
		{
			record = tableModel.record(i);

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
					anItem = new YerothQStandardItem(qv.toString());
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

				if (anItem)
				{
					anItem->setForeground(Qt::white);
				}
			}
		}

		this->resizeColumnsToContents();
	}
}
