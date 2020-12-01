/*
 * yeroth-erp-abstract-class-detail-view-window.cpp
 *
 *      Author: DR. XAVIER NOUMBISSI NOUNDOU
 */

#include "yeroth-erp-abstract-class-detail-view-window.hpp"


void YerothERPAbstractClassDetailViewFOR_WINDOW::setDetailViewFilter(YerothSqlTableModel *detailView_Yeroth_SQL_TABLE_MODEL)
{
    setPreviousBEFORETHISWindowsEntering_SQL_TABLE_Filter(detailView_Yeroth_SQL_TABLE_MODEL->filter());

    showItem_YerothERPAbstractClassDetailViewFOR_WINDOW();
}
