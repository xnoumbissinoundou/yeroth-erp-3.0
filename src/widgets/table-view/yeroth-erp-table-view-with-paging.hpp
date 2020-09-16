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

	virtual void viewYerothTableViewFirstPage();

	virtual void viewYerothTableViewLastPage();

	virtual void viewYerothTableViewPreviousPage();

	virtual void viewYerothTableViewNextPage();

protected:

	inline virtual void view_lister_les_elements_du_tableau(YerothSqlTableModel &tableModel)
	{
		lister_les_elements_du_tableau(tableModel, false);
	}

	virtual void view_setYerothTableViewLastPageNumberText(const QString &aPreviousPageNumberText) = 0;

	virtual void view_setYerothTableViewCurrentPageNumberText(const QString &aNextPageNumberText) = 0;


	int	_yerothSqlTableModelTotalRowCount;


	int	_yerothTableViewLastPageNumber;

	int	_yerothTableViewCurPageNumber;

	int	_yerothTableViewPageCount;

	int _yerothTableViewPageRowCount;
};


#endif /* SRC_YEROTH_TABLE_VIEW_WITH_PAGING_HPP_ */
