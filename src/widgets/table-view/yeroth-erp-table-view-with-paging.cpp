/*
 * yeroth-erp-table-view-with-paging.cpp
 *
 *      Author: Dipl.-Inf. XAVIER NOUMBISSI NOUNDOU
 */


#include "yeroth-erp-table-view-with-paging.hpp"

#include "src/yeroth-erp-windows.hpp"

#include "src/utils/yeroth-erp-utils.hpp"


#include <QtSql/QSqlQuery>

#include <QtCore/QtMath>


YerothTableViewWITHpaging::YerothTableViewWITHpaging()
:YerothTableView(),
 _yerothTableViewPreviousPageNumber(1),
 _yerothTableViewNextPageNumber(1),
 _yerothTableViewCurPageNumber(1),
 _yerothTableViewPageCount(1),
 _yerothTableViewPageRowCount(1)
{
}


YerothTableViewWITHpaging::YerothTableViewWITHpaging(QWidget * parent)
:YerothTableView(parent),
 _yerothTableViewPreviousPageNumber(1),
 _yerothTableViewNextPageNumber(1),
 _yerothTableViewCurPageNumber(1),
 _yerothTableViewPageCount(1),
 _yerothTableViewPageRowCount(1)
{
}


void YerothTableViewWITHpaging::queryYerothTableViewCurrentPageContentRow()
{
	YerothSqlTableModel *curYerothSqlTableModel =
			YerothERPWindows::getSqlTableModelFromName(YerothTableView::getSqlTableName());

	if (0 == curYerothSqlTableModel)
	{
		return ;
	}

	_yerothTableViewCurPageNumber = 1;

	int curPageFromRow = 0 ;

	int curPageToRow = curPageFromRow + _yerothTableViewPageRowCount;

	QString curYerothTableViewPageFilter =
			QString("limit %2, %3")
			.arg(QString::number(curPageFromRow),
					QString::number(curPageToRow));

	curYerothSqlTableModel->yerothSetFilter(curYerothTableViewPageFilter);

	int querySize = curYerothSqlTableModel->easySelect();

	if (querySize > 0)
	{
		_yerothTableViewNextPageNumber = _yerothTableViewNextPageNumber + 1;

		if (1 != _yerothTableViewCurPageNumber)
		{
			_yerothTableViewPreviousPageNumber = _yerothTableViewCurPageNumber - 1;
		}
		else
		{
			_yerothTableViewPreviousPageNumber = 1;
		}

//		lineEdit_marchandises_numero_page_precedente
	}
}


void YerothTableViewWITHpaging::setYerothTableViewPageRowCount(uint rowCount)
{
	_yerothTableViewPageRowCount = rowCount;

	double realPageCount = YerothTableView::rowCount() / _yerothTableViewPageRowCount;

	_yerothTableViewPageCount = qCeil(realPageCount);
}


