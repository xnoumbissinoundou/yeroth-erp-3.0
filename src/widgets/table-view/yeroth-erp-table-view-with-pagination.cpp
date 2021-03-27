/*
 * yeroth-erp-table-view-with-pagination.cpp
 *
 *      Author: DR. XAVIER NOUMBISSI NOUNDOU
 */


#include "yeroth-erp-table-view-with-pagination.hpp"

#include "src/yeroth-erp-windows.hpp"

#include "src/utils/yeroth-erp-utils.hpp"


#include <QtSql/QSqlQuery>

#include <QtCore/QtMath>


void YerothTableViewWITHpagination::setYerothTableViewPageRowCount(uint rowCount)
{
	_yerothTableViewPageRowCount = rowCount;

	double realPageCount = YerothTableView::rowCount() / _yerothTableViewPageRowCount;

	_yerothTableViewPageCount = qCeil(realPageCount);

	_yerothTableViewCurPageNumber = 0;

	if (0 != _curYerothSqlTableModel)
	{
		viewYerothTableViewNextPage(*_curYerothSqlTableModel);
	}
}


void YerothTableViewWITHpagination::calculate_and_set_YerothViewLastPageNumber(YerothSqlTableModel &curYerothSqlTableModel_IN)
{
	_curYerothSqlTableModel = &curYerothSqlTableModel_IN;

	_yerothSqlTableModelTotalRowCount = curYerothSqlTableModel_IN.rowCount();

	double tmp = _yerothSqlTableModelTotalRowCount / (double) _yerothTableViewPageRowCount;

	_yerothTableViewLastPageNumber = qCeil(tmp);

	if (_yerothTableViewLastPageNumber <= 0)
	{
		_yerothTableViewLastPageNumber = 1;
	}

	if (1 == _yerothTableViewLastPageNumber)
	{
		_yerothTableViewCurPageNumber = 1;
	}

	if (0 != _currentViewWindow)
	{
		_currentViewWindow->setYerothTableViewLastPageNumberText(QString::number(_yerothTableViewLastPageNumber));
	}
}


void YerothTableViewWITHpagination::displayYerothTableViewPageContentRowLimit(YerothSqlTableModel &curYerothSqlTableModel_IN,
																			  QString aListingStrategy /* = YerothUtils::EMPTY_STRING*/)
{
	_curYerothSqlTableModel = &curYerothSqlTableModel_IN;

	uint previousPageNumber = 0;

	if (_yerothTableViewCurPageNumber > 1)
	{
		previousPageNumber = _yerothTableViewCurPageNumber - 1;
	}

	uint curPageFromRowNumber = previousPageNumber * _yerothTableViewPageRowCount;

	calculate_and_set_YerothViewLastPageNumber(curYerothSqlTableModel_IN);

	if (curPageFromRowNumber > _yerothSqlTableModelTotalRowCount)
	{
		return ;
	}

	QString aCurYerothTableViewPageFilter = QString("limit %1, %2")
												.arg(QString::number(curPageFromRowNumber),
													 QString::number(_yerothTableViewPageRowCount));

	int querySize = 0;

	if (!YerothUtils::isEqualCaseInsensitive(curYerothSqlTableModel_IN.sqlTableName(),
											 YerothDatabase::MARCHANDISES))
	{
		querySize = curYerothSqlTableModel_IN.yeroth_specify_filter_FROM_SELECT_STATEMENT(aCurYerothTableViewPageFilter);
	}
	else if (YerothUtils::isEqualCaseInsensitive(curYerothSqlTableModel_IN.sqlTableName(),
											 YerothDatabase::MARCHANDISES))
	{
		if (!_allWindows->_marchandisesWindow->IS__CURRENTLY__CHECKING__NON__EMPTY__STOCKS())
		{
			querySize = curYerothSqlTableModel_IN.yeroth_specify_filter_FROM_SELECT_STATEMENT(aCurYerothTableViewPageFilter);
		}
	}


	if (!YerothUtils::isEqualCaseInsensitive(curYerothSqlTableModel_IN.sqlTableName(),
											 YerothDatabase::STOCKS))
	{
		lister_les_elements_du_tableau(curYerothSqlTableModel_IN);
	}
	else
	{
		lister_les_elements_du_tableau(curYerothSqlTableModel_IN,
									   aListingStrategy,
									   aCurYerothTableViewPageFilter);
	}

	curYerothSqlTableModel_IN.yeroth_RESET_specify_filter_FROM_SELECT_STATEMENT(aCurYerothTableViewPageFilter);

	construire_le_MAPPING_ORIGINAL_db_ID_VERS_db_row_Nr(curYerothSqlTableModel_IN);

	if (0 == _currentViewWindow)
	{
		return ;
	}

	if (querySize > 0)
	{
		if (curPageFromRowNumber <= _yerothSqlTableModelTotalRowCount)
		{
			if (_yerothTableViewCurPageNumber != _yerothTableViewLastPageNumber)
			{
				_currentViewWindow->setYerothTableViewCurrentPageNumberText(QString::number(_yerothTableViewCurPageNumber));
			}
			else
			{
				_currentViewWindow->setYerothTableViewCurrentPageNumberText(QString::number(_yerothTableViewLastPageNumber));
			}
		}
	}
	else
	{
		_yerothTableViewCurPageNumber = 1;

		_currentViewWindow->setYerothTableViewCurrentPageNumberText(QString::number(_yerothTableViewCurPageNumber));
	}
}


void YerothTableViewWITHpagination::queryYerothTableViewCurrentPageContentRow(YerothSqlTableModel &curYerothSqlTableModel_IN,
																			  QString aListingStrategy /* = YerothUtils::EMPTY_STRING */)
{
	_curYerothSqlTableModel = &curYerothSqlTableModel_IN;

	displayYerothTableViewPageContentRowLimit(curYerothSqlTableModel_IN,
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


void YerothTableViewWITHpagination::viewYerothTableViewFirstPage(YerothSqlTableModel &curYerothSqlTableModel_IN)
{
	_curYerothSqlTableModel = &curYerothSqlTableModel_IN;

	_yerothTableViewCurPageNumber = 1;

	queryYerothTableViewCurrentPageContentRow(curYerothSqlTableModel_IN);
}


void YerothTableViewWITHpagination::viewYerothTableViewLastPage(YerothSqlTableModel &curYerothSqlTableModel_IN)
{
	_curYerothSqlTableModel = &curYerothSqlTableModel_IN;

	_yerothTableViewCurPageNumber = _yerothTableViewLastPageNumber;

	queryYerothTableViewCurrentPageContentRow(curYerothSqlTableModel_IN);
}


void YerothTableViewWITHpagination::viewYerothTableViewPreviousPage(YerothSqlTableModel &curYerothSqlTableModel_IN)
{
	_curYerothSqlTableModel = &curYerothSqlTableModel_IN;

	if (_yerothTableViewCurPageNumber > 1)
	{
		_yerothTableViewCurPageNumber = _yerothTableViewCurPageNumber - 1;
	}
	else
	{
		_yerothTableViewCurPageNumber = 1;
	}

	queryYerothTableViewCurrentPageContentRow(curYerothSqlTableModel_IN);
}


void YerothTableViewWITHpagination::viewYerothTableViewNextPage(YerothSqlTableModel &curYerothSqlTableModel_IN)
{
	_curYerothSqlTableModel = &curYerothSqlTableModel_IN;

	if (_yerothTableViewCurPageNumber < _yerothTableViewLastPageNumber)
	{
		_yerothTableViewCurPageNumber = _yerothTableViewCurPageNumber + 1;
	}

	queryYerothTableViewCurrentPageContentRow(curYerothSqlTableModel_IN);
}

