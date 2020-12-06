/*
 * yeroth-erp-qstandard-item-model.cpp
 *
 * 		Author: DR. XAVIER NOUMBISSI NOUNDOU
 */


#include "yeroth-erp-qstandard-item-model.hpp"

#include "src/yeroth-erp-windows.hpp"

#include "src/widgets/table-view/yeroth-erp-table-view.hpp"

#include "src/utils/yeroth-erp-utils.hpp"


#include <QtCore/QDebug>


void YerothPOSQStandardItemModel::yerothPOSClear()
{
    int rows = rowCount();
    int columns = columnCount();

    QStandardItem *anItem = 0;

    for (int i = 0; i < rows; ++i)
    {
        for (int k = 0; k < columns; ++k)
        {
            anItem = item(i, k);

            if (0 != anItem)
            {
                delete anItem;
            }
        }
    }

    QStandardItemModel::clear();
}


QModelIndex YerothPOSQStandardItemModel::create_ZERO_ZERO_MODEL_INDEX_FOR_TABLE_VIEWING()
{
	return createIndex(0, 0, (void *)&YerothUtils::EMPTY_STRING);
}


void YerothPOSQStandardItemModel::sort(int column, Qt::SortOrder order /*= Qt::AscendingOrder*/)
{
	if (0 == _curSqlTableModel)
	{
		return ;
	}

	//qDebug() << "++ YerothPOSQStandardItemModel::sort not null";
	_curSqlTableModel->yerothSetSort(column, order);

	if (0 == _curTableView)
	{
		return ;
	}

	if (YerothUtils::instanceOf__YerothTableViewWITHpagination(_curTableView))
	{
		((YerothTableViewWITHpagination*)_curTableView)
							->queryYerothTableViewCurrentPageContentRow(*_curSqlTableModel);
	}
	else
	{
		_curTableView->lister_les_elements_du_tableau(*_curSqlTableModel);
	}
}

