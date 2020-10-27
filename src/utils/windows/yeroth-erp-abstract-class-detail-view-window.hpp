/*
 * yeroth-erp-abstract-class-detail-view-window.hpp
 *
 *      Author: DIPL.-INF. XAVIER NOUMBISSI NOUNDOU
 */

#ifndef SRC_UTILS_WINDOWS_YEROTH_ABSTRACT_CLASS_VIEW_DETAIL_WINDOW_HPP_
#define SRC_UTILS_WINDOWS_YEROTH_ABSTRACT_CLASS_VIEW_DETAIL_WINDOW_HPP_


#include "src/windows/yeroth-erp-window-commons.hpp"


#include <QtCore/QString>


class YerothERPAbstractClassDetailViewFOR_WINDOW
{

protected:

	YEROTH_CLASS_OPERATORS

	inline YerothERPAbstractClassDetailViewFOR_WINDOW()
	{
	}

	inline virtual ~YerothERPAbstractClassDetailViewFOR_WINDOW()
	{
		_previousBeforeTHISWINDOWEnteringFilter.clear();
	}

    virtual void showItem_YerothERPAbstractClassDetailViewFOR_WINDOW() = 0;

    inline void resetDetailViewFilter(YerothSqlTableModel *detailView_Yeroth_SQL_TABLE_MODEL)
    {
    	detailView_Yeroth_SQL_TABLE_MODEL->setFilter(getPreviousBEFORETHISWindowsEntering_SQL_TABLE_Filter());
    }

	void setDetailViewFilter(YerothSqlTableModel *detailView_Yeroth_SQL_TABLE_MODEL);

	inline void setPreviousBEFORETHISWindowsEntering_SQL_TABLE_Filter(QString aPreviousBeforeTHISWINDOWEnteringFilter)
	{
		_previousBeforeTHISWINDOWEnteringFilter = aPreviousBeforeTHISWINDOWEnteringFilter;
	}

	inline const QString & getPreviousBEFORETHISWindowsEntering_SQL_TABLE_Filter() const
	{
		return _previousBeforeTHISWINDOWEnteringFilter;
	}

private:

	QString 				_previousBeforeTHISWINDOWEnteringFilter;
};


#endif /* SRC_UTILS_WINDOWS_YEROTH_ABSTRACT_CLASS_VIEW_DETAIL_WINDOW_HPP_ */
