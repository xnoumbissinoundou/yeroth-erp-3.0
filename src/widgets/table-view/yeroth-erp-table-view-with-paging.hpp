/*
 * yeroth-erp-table-view-with-paging.hpp
 *
 *      Author: Dipl.-Inf. XAVIER NOUMBISSI NOUNDOU
 */

#ifndef SRC_YEROTH_TABLE_VIEW_WITH_PAGING_HPP_
#define SRC_YEROTH_TABLE_VIEW_WITH_PAGING_HPP_


#include "src/utils/yeroth-erp-config.hpp"

#include "src/widgets/table-view/yeroth-erp-table-view.hpp"


class YerothTableViewWITHpaging : public YerothTableView
{
	Q_OBJECT

public:

	YEROTH_CLASS_OPERATORS

	inline YerothTableViewWITHpaging()
	:YerothTableView(),
	 _currentViewWindow(0),
	 _yerothSqlTableModelTotalRowCount(0),
	 _yerothTableViewLastPageNumber(1),
	 _yerothTableViewCurPageNumber(1),
	 _yerothTableViewPageCount(1),
	 _yerothTableViewPageRowCount(YerothERPConfig::standard_pagination_number)
	{
	}


	inline YerothTableViewWITHpaging(QWidget * parent)
	:YerothTableView(parent),
	 _currentViewWindow(0),
	 _yerothSqlTableModelTotalRowCount(0),
	 _yerothTableViewLastPageNumber(1),
	 _yerothTableViewCurPageNumber(1),
	 _yerothTableViewPageCount(1),
	 _yerothTableViewPageRowCount(YerothERPConfig::standard_pagination_number)
	{
	}

	inline virtual ~YerothTableViewWITHpaging()
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

	void calculate_and_set_YerothViewLastPageNumber();

	virtual void setYerothTableViewPageRowCount(uint rowCount);

	virtual void displayYerothTableViewPageContentRowLimit(YerothSqlTableModel &curYerothSqlTableModel);

	virtual void queryYerothTableViewCurrentPageContentRow();

public slots:

	virtual void slot_set_page_view_row_count(const QString &pageTableViewRowCountText);

	virtual void viewYerothTableViewFirstPage();

	virtual void viewYerothTableViewLastPage();

	virtual void viewYerothTableViewPreviousPage();

	virtual void viewYerothTableViewNextPage();

public:

	YerothWindowsCommons *_currentViewWindow;

protected:

	inline virtual void view_lister_les_elements_du_tableau(YerothSqlTableModel &tableModel)
	{
		lister_les_elements_du_tableau(tableModel, false);
	}


	int	_yerothSqlTableModelTotalRowCount;


	int	_yerothTableViewLastPageNumber;

	int	_yerothTableViewCurPageNumber;

	int	_yerothTableViewPageCount;

	int _yerothTableViewPageRowCount;
};


#endif /* SRC_YEROTH_TABLE_VIEW_WITH_PAGING_HPP_ */
