/*
 * yeroth-erp-ventes-table-view.cpp
 *
 *      Author: DR. XAVIER NOUMBISSI NOUNDOU
 */


#include "src/widgets/table-view/yeroth-erp-ventes-table-view.hpp"

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


void YerothERPVentesTableView::lister_les_elements_du_tableau(YerothSqlTableModel &tableModel)
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

    YerothPOSUser *aUser = 0;

    if (0 != _allWindows)
    {
    	aUser = _allWindows->getUser();
    }


    QString curTableModelRawHdr;

    QString tmpQvString;

    QStandardItem *anItem = 0;

    QVariant qv;

    for (int i = 0; i < rows; ++i)
    {
    	for (int k = 0; k < columns; ++k)
    	{
    		curTableModelRawHdr = tableModelRawHeaders.at(k);

    		qv.setValue(tableModel.record(i).value(k));

    		anItem = _stdItemModel->item(i, k);

    		if (anItem)
    		{
    			delete anItem;
    		}

    		switch (qv.type())
    		{
    		case QVariant::UInt:

    			if (!YerothUtils::isEqualCaseInsensitive(curTableModelRawHdr, YerothDatabaseTableColumn::ID))
    			{
    				anItem = new YerothQStandardItem(GET_NUM_STRING(qv.toUInt()));
    			}
    			else
    			{
    				YEROTH_SAVE_ID_TO_ROW_NUMBER_FOR_YEROTH_TABLE_VIEW(tmpQvString, qv.toUInt(), i)
    				anItem = new YerothQStandardItem(tmpQvString);
    			}

    			_stdItemModel->setItem(i, k, anItem);
    			break;

    		case QVariant::Int:

    			if (YerothUtils::isEqualCaseInsensitive(curTableModelRawHdr, YerothDatabaseTableColumn::TYPE_DE_VENTE))
    			{
    				tmpQvString = YerothUtils::_typedeventeToUserViewString.value(qv.toInt());
    				anItem = new YerothQStandardItem(tmpQvString, false);
    			}
    			else if (!YerothUtils::isEqualCaseInsensitive(curTableModelRawHdr, YerothDatabaseTableColumn::ID))
    			{
    				anItem = new YerothQStandardItem(GET_NUM_STRING(qv.toInt()));
    			}
    			else
    			{
    				YEROTH_SAVE_ID_TO_ROW_NUMBER_FOR_YEROTH_TABLE_VIEW(tmpQvString, qv.toInt(), i)
    				anItem = new YerothQStandardItem(tmpQvString);
    			}

    			_stdItemModel->setItem(i, k, anItem);
    			break;

    		case QVariant::Double:

    			if (YerothUtils::isEqualCaseInsensitive(curTableModelRawHdr, YerothDatabaseTableColumn::MARGE_BENEFICIAIRE))
    			{
    				anItem = new YerothQStandardItem(GET_DOUBLE_STRING(0.0));

    				if (0 != aUser && aUser->isManager())
    				{
    					anItem = new YerothQStandardItem(GET_DOUBLE_STRING(qv.toDouble()));
    				}
    			}
    			else
    			{
        			anItem = new YerothQStandardItem(GET_DOUBLE_STRING(qv.toDouble()));
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

    		if (anItem)
    		{
    			anItem->setForeground(Qt::white);
    		}
    	}
    }

    static YerothERPWindows *curAllWindows = YerothUtils::getAllWindows();

    YEROTH_SET_LAST_TABLE_VIEW_SELECTED_ROW_ID(curAllWindows, curAllWindows->_ventesWindow)

    resizeColumnsToContents();
}

