/*
 * yeroth-erp-table-view-with-pagination.hpp
 *
 *      Author: Dipl.-Inf. XAVIER NOUMBISSI NOUNDOU
 */

#ifndef SRC_YEROTH_TABLE_VIEW_WITH_PAGING_HPP_
#define SRC_YEROTH_TABLE_VIEW_WITH_PAGING_HPP_


#include "src/utils/yeroth-erp-utils.hpp"

#include "src/utils/yeroth-erp-config.hpp"

#include "src/widgets/table-view/yeroth-erp-table-view.hpp"


class YerothTableViewWITHpagination : public YerothTableView
{
	Q_OBJECT

public:

	YEROTH_CLASS_OPERATORS

	inline YerothTableViewWITHpagination()
	:YerothTableView(),
	 _currentViewWindow(0),
	 _needExecSelectStatement_FOR_TABLE_VIEW_PAGING_LISTING(false),
	 _yerothSqlTableModelTotalRowCount(0),
	 _yerothTableViewLastPageNumber(1),
	 _yerothTableViewCurPageNumber(1),
	 _yerothTableViewPageCount(1),
	 _yerothTableViewPageRowCount(YerothERPConfig::standard_pagination_number)
	{
	}


	inline YerothTableViewWITHpagination(QWidget * parent)
	:YerothTableView(parent),
	 _currentViewWindow(0),
	 _needExecSelectStatement_FOR_TABLE_VIEW_PAGING_LISTING(false),
	 _yerothSqlTableModelTotalRowCount(0),
	 _yerothTableViewLastPageNumber(1),
	 _yerothTableViewCurPageNumber(1),
	 _yerothTableViewPageCount(1),
	 _yerothTableViewPageRowCount(YerothERPConfig::standard_pagination_number)
	{
	}

	inline virtual ~YerothTableViewWITHpagination()
	{
	}

	inline int getYerothSqlTableModelTotalRowCount()
	{
		return _yerothSqlTableModelTotalRowCount;
	}

	inline int getYerothTableViewPageCount()
	{
		return _yerothTableViewPageCount;
	}

	inline int getYerothTableViewPageRowCount()
	{
		return _yerothTableViewPageRowCount;
	}

	inline int getNEEDExecSelectStatement_FOR_TABLE_VIEW_PAGING_LISTING()
	{
		return _needExecSelectStatement_FOR_TABLE_VIEW_PAGING_LISTING;
	}

	inline void setNEEDExecSelectStatement_FOR_TABLE_VIEW_PAGING_LISTING(bool aBooleanValue)
	{
		_needExecSelectStatement_FOR_TABLE_VIEW_PAGING_LISTING = aBooleanValue;
	}

	void calculate_and_set_YerothViewLastPageNumber();

	virtual void setYerothTableViewPageRowCount(uint rowCount);

	virtual void displayYerothTableViewPageContentRowLimit(YerothSqlTableModel &curYerothSqlTableModel,
														   QString aListingStrategy = YerothUtils::EMPTY_STRING);

	virtual void queryYerothTableViewCurrentPageContentRow(QString aListingStrategy = YerothUtils::EMPTY_STRING);

public slots:

	virtual void slot_set_page_view_row_count(const QString &pageTableViewRowCountText);

	virtual void viewYerothTableViewFirstPage();

	virtual void viewYerothTableViewLastPage();

	virtual void viewYerothTableViewPreviousPage();

	virtual void viewYerothTableViewNextPage();

public:

	YerothWindowsCommons *_currentViewWindow;

protected:

	bool _needExecSelectStatement_FOR_TABLE_VIEW_PAGING_LISTING;

	int	_yerothSqlTableModelTotalRowCount;


	int	_yerothTableViewLastPageNumber;

	int	_yerothTableViewCurPageNumber;

	int	_yerothTableViewPageCount;

	int _yerothTableViewPageRowCount;
};


#endif /* SRC_YEROTH_TABLE_VIEW_WITH_PAGING_HPP_ */
