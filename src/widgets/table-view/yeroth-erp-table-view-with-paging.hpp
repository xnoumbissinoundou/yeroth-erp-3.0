/*
 * yeroth-erp-table-view-with-paging.hpp
 *
 *      Author: Dipl.-Inf. XAVIER NOUMBISSI NOUNDOU
 */

#ifndef SRC_YEROTH_TABLE_VIEW_WITH_PAGING_HPP_
#define SRC_YEROTH_TABLE_VIEW_WITH_PAGING_HPP_

#include "src/widgets/table-view/yeroth-erp-table-view.hpp"


class YerothTableViewWITHpaging : public YerothTableView
{
	Q_OBJECT

public:

	YEROTH_CLASS_OPERATORS

	YerothTableViewWITHpaging();

	YerothTableViewWITHpaging(QWidget * parent);

	inline virtual ~YerothTableViewWITHpaging()
	{
	}


	inline int getYerothTableViewPageCount()
	{
		return _yerothTableViewPageCount;
	}

	inline int getYerothTableViewPageRowCount()
	{
		return _yerothTableViewPageRowCount;
	}

protected:

	virtual void queryYerothTableViewCurrentPageContentRow();

	virtual void setYerothTableViewPageRowCount(uint rowCount);

	int	_yerothTableViewPreviousPageNumber;

	int	_yerothTableViewNextPageNumber;

	int	_yerothTableViewCurPageNumber;

	int	_yerothTableViewPageCount;

	int _yerothTableViewPageRowCount;
};


#endif /* SRC_YEROTH_TABLE_VIEW_WITH_PAGING_HPP_ */
