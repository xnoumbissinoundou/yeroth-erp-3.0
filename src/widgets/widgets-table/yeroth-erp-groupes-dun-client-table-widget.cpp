/*
 * yeroth-erp-groupes-dun-client-table-widget.cpp
 *
 *      Author: PR. XAVIER NOUMBISSI NOUNDOU
 */


#include "src/widgets/widgets-table/yeroth-erp-groupes-dun-client-table-widget.hpp"


int YerothGroupesDunClientTableWidget::insert_group(const QString &group_db_ID,
													const QString &groupDesignation,
											   	    const QString &groupReference,
													const QString &groupNumber)
{
//	QDEBUG_STRING_OUTPUT_2("YerothGroupesDunClientWindow::insert_group", groupDesignation);

	setEditTriggers(QAbstractItemView::NoEditTriggers);

    setRowCount(_curRow + 1);

    _groupDesignationItem 	= new YerothQTableWidgetItem(groupDesignation);
    _groupReferenceItem   	= new YerothQTableWidgetItem(groupReference);
    _groupNumberItem 		= new YerothQTableWidgetItem(groupNumber);

    _mapListIdxToElement_db_ID.insert(_curRow, group_db_ID);

    unsigned idx = 0;

    //Each call to setItem triggers a call to YerothPointDeVenteWindow::handleQteChange
    setItem(_curRow, idx++, _groupDesignationItem);
    setItem(_curRow, idx++, _groupReferenceItem);
    setItem(_curRow, idx++, _groupNumberItem);

    resizeColumnsToContents();

    int lastCurRow = _curRow;

    if (_curRow >= 0)
    {
    	selectRow(_curRow);
    }

    ++_curRow;

    set_MY_QStandardItemFlags();

    emit addedYerothTableWidget();

    return lastCurRow;
}

