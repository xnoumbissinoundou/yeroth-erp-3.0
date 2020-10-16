/*
 * yeroth-erp-table-view-with-pagination.cpp
 *
 *      Author: DIPL.-INF. XAVIER NOUMBISSI NOUNDOU
 */


#include "yeroth-erp-table-view-with-pagination.hpp"

#include "src/yeroth-erp-windows.hpp"

#include "src/utils/yeroth-erp-utils.hpp"


#include <QtSql/QSqlQuery>

#include <QtCore/QtMath>


void YerothTableViewWITHpagination::calculate_and_set_YerothViewLastPageNumber()
{
	QString selectTotalSqlTableRowCountStr(QString("select COUNT(*) from %1")
												.arg(YerothTableView::getSqlTableName()));

	QSqlQuery query;

	int querySize = YerothUtils::execQuery(query, selectTotalSqlTableRowCountStr);

	if (querySize > 0 && query.next())
	{
		_yerothSqlTableModelTotalRowCount = query.value(0).toInt();
	}

	double tmp = _yerothSqlTableModelTotalRowCount / (double) _yerothTableViewPageRowCount;

	_yerothTableViewLastPageNumber = qCeil(tmp);

	if (_yerothTableViewLastPageNumber <= 0)
	{
		_yerothTableViewLastPageNumber = 1;
	}

	if (0 != _currentViewWindow)
	{
		_currentViewWindow->
			setYerothTableViewLastPageNumberText(QString::number(_yerothTableViewLastPageNumber));
	}
}


void YerothTableViewWITHpagination::setYerothTableViewPageRowCount(uint rowCount)
{
	_yerothTableViewPageRowCount = rowCount;

	double realPageCount = YerothTableView::rowCount() / _yerothTableViewPageRowCount;

	_yerothTableViewPageCount = qCeil(realPageCount);

	_yerothTableViewCurPageNumber = 0;

	viewYerothTableViewNextPage();
}


void YerothTableViewWITHpagination::displayYerothTableViewPageContentRowLimit(YerothSqlTableModel &curYerothSqlTableModel,
																			  QString aListingStrategy /* = YerothUtils::EMPTY_STRING*/)
{
	uint previousPageNumber = 0;

	if (_yerothTableViewCurPageNumber > 1)
	{
		previousPageNumber = _yerothTableViewCurPageNumber - 1;
	}

	uint curPageFromRowNumber = previousPageNumber * _yerothTableViewPageRowCount;

	calculate_and_set_YerothViewLastPageNumber();

	if (curPageFromRowNumber > _yerothSqlTableModelTotalRowCount)
	{
		return ;
	}

	QString aCurYerothTableViewPageFilter = QString("limit %2, %3")
												.arg(QString::number(curPageFromRowNumber),
													 QString::number(_yerothTableViewPageRowCount));

	int querySize  =
			curYerothSqlTableModel.yeroth_specify_filter_FROM_SELECT_STATEMENT(aCurYerothTableViewPageFilter);

	if (!YerothUtils::isEqualCaseInsensitive(curYerothSqlTableModel.sqlTableName(),
											 _allWindows->STOCKS))
	{
		lister_les_elements_du_tableau(curYerothSqlTableModel);
	}
	else
	{
		lister_les_elements_du_tableau(curYerothSqlTableModel,
									   aListingStrategy,
									   aCurYerothTableViewPageFilter);
	}

	if (querySize > 0)
	{
		if (curPageFromRowNumber 			<= _yerothSqlTableModelTotalRowCount &&
			_yerothTableViewCurPageNumber   !=_yerothTableViewLastPageNumber)
		{
			if (0 != _currentViewWindow)
			{
				_currentViewWindow->setYerothTableViewCurrentPageNumberText(QString::number(_yerothTableViewCurPageNumber));
			}
		}
		else
		{
			if (0 != _currentViewWindow)
			{
				_currentViewWindow->setYerothTableViewCurrentPageNumberText(QString::number(_yerothTableViewLastPageNumber));
			}
		}

		calculate_and_set_YerothViewLastPageNumber();
	}
	else
	{
		if (0 != _currentViewWindow)
		{
			_currentViewWindow->
				setYerothTableViewCurrentPageNumberText(QString::number(_yerothTableViewLastPageNumber));
		}

		calculate_and_set_YerothViewLastPageNumber();
	}
}


void YerothTableViewWITHpagination::queryYerothTableViewCurrentPageContentRow(QString aListingStrategy /* = YerothUtils::EMPTY_STRING */)
{
	YerothSqlTableModel *curYerothSqlTableModel =
			YerothERPWindows::getSqlTableModelFromName(YerothTableView::getSqlTableName());

	if (0 == curYerothSqlTableModel)
	{
		return ;
	}

	displayYerothTableViewPageContentRowLimit(*curYerothSqlTableModel,
											  aListingStrategy);
}


void YerothTableViewWITHpagination::slot_set_page_view_row_count(const QString &pageTableViewRowCountText)
{
	if (pageTableViewRowCountText.isEmpty())
	{
		setYerothTableViewPageRowCount(1);
		return ;
	}

	int pageTableViewRowCount = pageTableViewRowCountText.toInt();

	if (0 != pageTableViewRowCount)
	{
		setYerothTableViewPageRowCount(pageTableViewRowCount);
	}
}


void YerothTableViewWITHpagination::viewYerothTableViewFirstPage()
{
	_yerothTableViewCurPageNumber = 1;

	queryYerothTableViewCurrentPageContentRow();
}


void YerothTableViewWITHpagination::viewYerothTableViewLastPage()
{
	calculate_and_set_YerothViewLastPageNumber();

	_yerothTableViewCurPageNumber = _yerothTableViewLastPageNumber;

	queryYerothTableViewCurrentPageContentRow();
}


void YerothTableViewWITHpagination::viewYerothTableViewPreviousPage()
{
	if (_yerothTableViewCurPageNumber > 1)
	{
		_yerothTableViewCurPageNumber = _yerothTableViewCurPageNumber - 1;
	}
	else
	{
		_yerothTableViewCurPageNumber = 1;
	}

	queryYerothTableViewCurrentPageContentRow();
}


void YerothTableViewWITHpagination::viewYerothTableViewNextPage()
{
	if (_yerothTableViewCurPageNumber < _yerothTableViewLastPageNumber)
	{
		_yerothTableViewCurPageNumber = _yerothTableViewCurPageNumber + 1;
	}

	queryYerothTableViewCurrentPageContentRow();
}

