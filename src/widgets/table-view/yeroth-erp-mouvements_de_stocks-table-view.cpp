/*
 * yeroth-erp-mouvements_de_stocks-table-view.cpp
 *
 *      Author: DR. XAVIER NOUMBISSI NOUNDOU
 */


#include "yeroth-erp-mouvements_de_stocks-table-view.hpp"

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


void YerothERPMouvementsDeStocksTableView::lister_les_elements_du_tableau(YerothSqlTableModel &tableModel)
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

    			if (!YerothUtils::isEqualCaseInsensitive(curTableModelRawHdr, YerothDatabaseTableColumn::ID))
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
    			//qDebug() << "YerothTableView:::lister(): undecoded QVariant -> " << qv.type();
    			break;
    		}
    	}
    }

    static YerothERPWindows *curAllWindows = YerothUtils::getAllWindows();

    YEROTH_SET_LAST_TABLE_VIEW_SELECTED_ROW_ID(curAllWindows, curAllWindows->_mouvementsDeStocksWindow)

    resizeColumnsToContents();
}


void YerothERPMouvementsDeStocksTableView::selectionChanged (const QItemSelection & selected,
                                        		  	  	  	 const QItemSelection & deselected)
{
    static YerothERPWindows *curAllWindows = YerothUtils::getAllWindows();

    QModelIndexList selectedIndexes = selected.indexes();

    QString db_ID_in_out;

    if (selectedIndexes.size() == 1)
    {
    	_MAP_lastSelected_Row__TO__DB_ID.clear();

        _lastSelected_Row__ID = selectedIndexes.at(0).row();
    }
    else if (selectedIndexes.size() > 1)
    {
    	uint last_Row_ID = selectedIndexes.size() - 1;

    	for (uint j = 0; j < selectedIndexes.size(); ++j)
    	{
    		curAllWindows->_mouvementsDeStocksWindow->
    				getQModelIndex_dbID_from_MODEL_INDEX(selectedIndexes.at(j),
    													 db_ID_in_out);

			_MAP_lastSelected_Row__TO__DB_ID
					.insert(QString::number(selectedIndexes.at(j).row()),
			    			db_ID_in_out);

    		if (last_Row_ID == j)
    		{
    			_lastSelected_Row__ID = selectedIndexes.at(j).row();
    		}
    	}
    }

    /*
     * Cet appel de la fonction 'clearFocus' est necessaire pour
     * que les nouveaux elements du tableau soit visible immediatement.
     */
    clearFocus();
}
