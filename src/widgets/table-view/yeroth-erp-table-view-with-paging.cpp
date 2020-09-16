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
 _yerothSqlTableModelTotalRowCount(0),
 _yerothTableViewCurPageFromRowNumber(0),
 _yerothTableViewLastPageNumber(1),
 _yerothTableViewPreviousPageNumber(1),
 _yerothTableViewNextPageNumber(1),
 _yerothTableViewCurPageNumber(1),
 _yerothTableViewPageCount(1),
 _yerothTableViewPageRowCount(12)
{
}


YerothTableViewWITHpaging::YerothTableViewWITHpaging(QWidget * parent)
:YerothTableView(parent),
 _yerothSqlTableModelTotalRowCount(0),
 _yerothTableViewCurPageFromRowNumber(0),
 _yerothTableViewLastPageNumber(1),
 _yerothTableViewPreviousPageNumber(1),
 _yerothTableViewNextPageNumber(1),
 _yerothTableViewCurPageNumber(1),
 _yerothTableViewPageCount(1),
 _yerothTableViewPageRowCount(12)
{
}


void YerothTableViewWITHpaging::setYerothTableViewPageRowCount(uint rowCount)
{
	_yerothTableViewPageRowCount = rowCount;

	double realPageCount = YerothTableView::rowCount() / _yerothTableViewPageRowCount;

	_yerothTableViewPageCount = qCeil(realPageCount);

	_yerothTableViewPreviousPageNumber = 1;
	_yerothTableViewCurPageNumber = 1;
	_yerothTableViewNextPageNumber = _yerothTableViewCurPageNumber + 1;
	_yerothTableViewCurPageFromRowNumber = 0;

	queryYerothTableViewCurrentPageContentRow();
}


void YerothTableViewWITHpaging::displayYerothTableViewPageContentRowLimit(YerothSqlTableModel &curYerothSqlTableModel)
{
	if (1 == _yerothTableViewCurPageNumber)
	{
//		curYerothSqlTableModel.resetFilter();
	}

	QString selectTotalSqlTableRowCountStr(QString("select COUNT(*) from %1")
												.arg(YerothTableView::getSqlTableName()));

	QSqlQuery query;

	int querySize = YerothUtils::execQuery(query, selectTotalSqlTableRowCountStr);

	if (querySize > 0 && query.next())
	{
		_yerothSqlTableModelTotalRowCount = query.value(0).toInt();
	}

	if (_yerothTableViewCurPageFromRowNumber > _yerothSqlTableModelTotalRowCount)
	{
		return ;
	}

	QString curYerothTableViewPageFilter =
			QString("limit %2, %3")
				.arg(QString::number(_yerothTableViewCurPageFromRowNumber),
					 QString::number(_yerothTableViewPageRowCount));

//	QDEBUG_STRINGS_OUTPUT_2("yerothTableViewFilter", curYerothTableViewPageFilter);

	querySize = curYerothSqlTableModel.yeroth_specify_filter_FROM_SELECT_STATEMENT(curYerothTableViewPageFilter);

	view_lister_les_elements_du_tableau(curYerothSqlTableModel);

//	QDEBUG_STRINGS_OUTPUT_2("yerothSelectStatement", curYerothSqlTableModel.yerothSelectStatement());

//	QDEBUG_STRINGS_OUTPUT_2("querySize", QString::number(querySize));

	if (querySize > 0)
	{
		_yerothTableViewNextPageNumber = _yerothTableViewCurPageNumber + 1;

		if (1 != _yerothTableViewCurPageNumber)
		{
			_yerothTableViewPreviousPageNumber = _yerothTableViewCurPageNumber - 1;
		}
		else
		{
			_yerothTableViewPreviousPageNumber = 1;
		}

		double tmp = _yerothSqlTableModelTotalRowCount / (double) _yerothTableViewPageRowCount;

//		QDEBUG_STRINGS_OUTPUT_2("_yerothTableViewPageRowCount", QString::number(_yerothTableViewPageRowCount));

//		QDEBUG_STRINGS_OUTPUT_2("_yerothSqlTableModelTotalRowCount", QString::number(_yerothSqlTableModelTotalRowCount));

		_yerothTableViewLastPageNumber = qCeil(tmp);

//		QDEBUG_STRINGS_OUTPUT_2("_yerothTableViewLastPageNumber", QString::number(_yerothTableViewLastPageNumber));

		if (_yerothTableViewPreviousPageNumber >= 1 &&
			_yerothTableViewCurPageNumber != 1)
		{
			view_setYerothTableViewPreviousPageNumberText(QString::number(_yerothTableViewPreviousPageNumber));
		}
		else
		{
			view_setYerothTableViewPreviousPageNumberText("-");
		}

		if (_yerothTableViewCurPageFromRowNumber <= _yerothSqlTableModelTotalRowCount &&
			_yerothTableViewLastPageNumber != _yerothTableViewCurPageNumber)
		{
			view_setYerothTableViewNextPageNumberText(QString::number(_yerothTableViewNextPageNumber));
		}
		else
		{
			view_setYerothTableViewNextPageNumberText("-");
		}
	}
}


void YerothTableViewWITHpaging::queryYerothTableViewCurrentPageContentRow()
{
	YerothSqlTableModel *curYerothSqlTableModel =
			YerothERPWindows::getSqlTableModelFromName(YerothTableView::getSqlTableName());

	if (0 == curYerothSqlTableModel)
	{
		return ;
	}

	displayYerothTableViewPageContentRowLimit(*curYerothSqlTableModel);
}


void YerothTableViewWITHpaging::viewYerothTableViewPreviousPage()
{
	YerothSqlTableModel *curYerothSqlTableModel =
			YerothERPWindows::getSqlTableModelFromName(YerothTableView::getSqlTableName());

	if (0 == curYerothSqlTableModel || (1 == _yerothTableViewCurPageNumber))
	{
		return ;
	}

	int potentialPreviousPage = _yerothTableViewCurPageNumber - 2;

	if (potentialPreviousPage >= 1)
	{
		_yerothTableViewPreviousPageNumber 		= potentialPreviousPage;
		_yerothTableViewCurPageNumber 			= _yerothTableViewPreviousPageNumber + 1;
		_yerothTableViewCurPageFromRowNumber 	= _yerothTableViewPreviousPageNumber * _yerothTableViewPageRowCount;
	}
	else
	{
		view_setYerothTableViewPreviousPageNumberText("-");

		_yerothTableViewPreviousPageNumber = 1;
		_yerothTableViewCurPageNumber = 1;
		_yerothTableViewCurPageFromRowNumber = 0;
	}

	_yerothTableViewNextPageNumber = _yerothTableViewCurPageNumber + 1;

//	QDEBUG_STRINGS_OUTPUT_2("_yerothTableViewPreviousPageNumber", QString::number(_yerothTableViewPreviousPageNumber));
//	QDEBUG_STRINGS_OUTPUT_2("_yerothTableViewCurPageNumber", QString::number(_yerothTableViewCurPageNumber));
//	QDEBUG_STRINGS_OUTPUT_2("_yerothTableViewNextPageNumber", QString::number(_yerothTableViewNextPageNumber));
//	QDEBUG_STRINGS_OUTPUT_2("_yerothTableViewCurPageFromRowNumber", QString::number(_yerothTableViewCurPageFromRowNumber));

	displayYerothTableViewPageContentRowLimit(*curYerothSqlTableModel);
}


void YerothTableViewWITHpaging::viewYerothTableViewNextPage()
{
	YerothSqlTableModel *curYerothSqlTableModel =
			YerothERPWindows::getSqlTableModelFromName(YerothTableView::getSqlTableName());

	if (0 == curYerothSqlTableModel)
	{
		return ;
	}

	_yerothTableViewPreviousPageNumber 		= _yerothTableViewCurPageNumber;

	_yerothTableViewCurPageNumber 			= _yerothTableViewPreviousPageNumber + 1;

	_yerothTableViewNextPageNumber 			= _yerothTableViewCurPageNumber + 1;

	_yerothTableViewCurPageFromRowNumber 	= _yerothTableViewPreviousPageNumber * _yerothTableViewPageRowCount;

	if (_yerothTableViewCurPageFromRowNumber <= _yerothSqlTableModelTotalRowCount)
	{
		displayYerothTableViewPageContentRowLimit(*curYerothSqlTableModel);
	}
	else
	{
		view_setYerothTableViewNextPageNumberText("-");

		_yerothTableViewNextPageNumber 		= _yerothTableViewCurPageNumber;

		_yerothTableViewCurPageNumber 		= _yerothTableViewPreviousPageNumber;

		_yerothTableViewPreviousPageNumber 	= _yerothTableViewCurPageNumber - 1;
	}
}

