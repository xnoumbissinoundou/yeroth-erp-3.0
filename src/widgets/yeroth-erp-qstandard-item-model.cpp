/*
 * yeroth-erp-qstandard-item-model.cpp
 *
 * 		Author: DIPL.-INF. XAVIER NOUMBISSI NOUNDOU
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


void YerothPOSQStandardItemModel::sort(int column, Qt::SortOrder order /*= Qt::AscendingOrder*/)
{
	if (0 != _curSqlTableModel)
	{
		//qDebug() << "++ YerothPOSQStandardItemModel::sort not null";
		_curSqlTableModel->yerothSetSort(column, order);

		if (0 != _curTableView)
		{
			_curTableView->lister_les_elements_du_tableau(*_curSqlTableModel);
		}
	}
}
