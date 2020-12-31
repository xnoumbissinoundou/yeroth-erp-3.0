/*
 * yeroth-erp-groupes-dun-client-table-widget.cpp
 *
 *      Author: DR. XAVIER NOUMBISSI NOUNDOU
 */


#include "src/widgets/yeroth-erp-groupes-dun-client-table-widget.hpp"


YerothGroupesDunClientTableWidget::YerothGroupesDunClientTableWidget(QWidget *parent  /* = 0 */)
:YerothTableWidget(parent),
 _groupDesignationItem(0),
 _groupReferenceItem(0),
 _groupNumberItem(0)
{
}


int YerothGroupesDunClientTableWidget::insert_group(const QString &group_db_ID,
													const QString &groupDesignation,
											   	    const QString &groupReference,
													const QString &groupNumber)
{
//	QDEBUG_STRINGS_OUTPUT_2("YerothGroupesDunClientWindow::insert_group", groupDesignation);

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

    ++_curRow;

    Qt::ItemFlags qStandardItemFlags = Qt::ItemIsSelectable | Qt::ItemIsEnabled;

    setQStandardItemFlags(qStandardItemFlags);

    emit addedYerothTableWidget();

    return lastCurRow;
}
