/*
 * yeroth-erp-fournisseurs-table-view.cpp
 *
 *      Author: Dipl.-Inf. XAVIER NOUMBISSI NOUNDOU
 */


#include "yeroth-erp-fournisseurs-table-view.hpp"

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


YerothERPFournisseursTableView::YerothERPFournisseursTableView()
:YerothTableViewWITHpagination()
{
	 _needExecSelectStatement_FOR_TABLE_VIEW_PAGING_LISTING = true;

	_stdItemModel->_curTableView = this;
}


YerothERPFournisseursTableView::YerothERPFournisseursTableView(QWidget * parent)
:YerothTableViewWITHpagination(parent)
{
	 _needExecSelectStatement_FOR_TABLE_VIEW_PAGING_LISTING = true;

	_stdItemModel->_curTableView = this;
}


void YerothERPFournisseursTableView::lister_les_elements_du_tableau(YerothSqlTableModel &tableModel,
																	QString aStockListingStrategy /* = "" */)
{
	_stdItemModel->_curSqlTableModel = &tableModel;

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

	QStringList	tableModelRawHeaders;

    YerothUtils::createTableModelHeaders(tableModel,
    									 *_stdItemModel,
										 *_tableModelHeaders,
										 tableModelRawHeaders);

    if (!s)
    {
    	return ;
    }

    QString curTableModelRawHdr;

	QString tmpQvString;

	QStandardItem *anItem = 0;

	QVariant qv;

	QSqlRecord record;

	for (int i = 0; i < rows; ++i)
	{
		record = tableModel.record(i);

		for (int k = 0; k < columns; ++k)
		{
			curTableModelRawHdr = tableModelRawHeaders.at(k);

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

				if (!YerothUtils::isEqualCaseInsensitive(curTableModelRawHdr, YerothDatabaseTableColumn::REFERENCE_FOURNISSEUR))
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

			if (anItem)
			{
				anItem->setForeground(Qt::white);
			}
		}
	}

	resizeColumnsToContents();
}
