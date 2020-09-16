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
 _yerothTableViewLastPageNumber(1),
 _yerothTableViewPreviousPageNumber(1),
 _yerothTableViewNextPageNumber(1),
 _yerothTableViewCurPageNumber(1),
 _yerothTableViewPageCount(1),
 _yerothTableViewPageRowCount(1)
{
}


YerothTableViewWITHpaging::YerothTableViewWITHpaging(QWidget * parent)
:YerothTableView(parent),
 _yerothTableViewLastPageNumber(1),
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
		QDEBUG_STRINGS_OUTPUT_1("curYerothSqlTableModel equals 0");

		return ;
	}

	QDEBUG_STRINGS_OUTPUT_1("curYerothSqlTableModel don't equals 0");

	_yerothTableViewCurPageNumber = 1;

	int curPageFromRow = 0 ;

	int curPageToRow = curPageFromRow + _yerothTableViewPageRowCount;

	curYerothSqlTableModel->resetFilter();

	QDEBUG_STRINGS_OUTPUT_2("yerothSelectStatement (1)", curYerothSqlTableModel->yerothSelectStatement());

	int curYerothSqlTableModelTotalRowCount = curYerothSqlTableModel->rowCount();

	QString curYerothTableViewPageFilter =
			QString("limit %2, %3")
				.arg(QString::number(curPageFromRow),
					 QString::number(curPageToRow));

	QDEBUG_STRINGS_OUTPUT_2("yerothTableViewFilter", curYerothTableViewPageFilter);

	curYerothSqlTableModel->yerothSetFilter(curYerothTableViewPageFilter);

	int querySize = curYerothSqlTableModel->easySelect();

	QDEBUG_STRINGS_OUTPUT_2("yerothSelectStatement (2)", curYerothSqlTableModel->yerothSelectStatement());

	QDEBUG_STRINGS_OUTPUT_2("querySize", QString::number(querySize));

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

		double tmp = curYerothSqlTableModelTotalRowCount / _yerothTableViewPageRowCount;

		_yerothTableViewLastPageNumber = qCeil(tmp);

		view_setYerothTableViewPreviousPageNumberText(QString::number(_yerothTableViewPreviousPageNumber));

		view_setYerothTableViewNextPageNumberText(QString::number(_yerothTableViewNextPageNumber));
	}
}


void YerothTableViewWITHpaging::setYerothTableViewPageRowCount(uint rowCount)
{
	_yerothTableViewPageRowCount = rowCount;

	double realPageCount = YerothTableView::rowCount() / _yerothTableViewPageRowCount;

	_yerothTableViewPageCount = qCeil(realPageCount);

	queryYerothTableViewCurrentPageContentRow();
}


