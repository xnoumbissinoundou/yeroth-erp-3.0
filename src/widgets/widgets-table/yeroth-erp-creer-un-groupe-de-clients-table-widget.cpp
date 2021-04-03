/*
 * yeroth-erp-creer-un-groupe-de-clients-table-widget.cpp
 *
 *      Author: PR. XAVIER NOUMBISSI NOUNDOU
 */

#include "yeroth-erp-creer-un-groupe-de-clients-table-widget.hpp"


#include "src/widgets/widgets-table/yeroth-erp-table-widget.hpp"

#include "src/utils/yeroth-erp-logger.hpp"


YerothCreerUnGroupeDeClientsTableWidget::YerothCreerUnGroupeDeClientsTableWidget(QWidget *parent /* = 0 */)
:YerothTableWidget(parent),
 _clientReferenceItem(0),
 _clientNameItem(0)
{
}


unsigned int YerothCreerUnGroupeDeClientsTableWidget::addAClientGroupMember(const QString& aClientGroupMember_db_ID,
																			const QString& aClientGroupMemberReferenceClient,
																   	   	   	const QString& aClientGroupMemberNomDuClient)
{
    setRowCount(_curRow + 1);

    _clientReferenceItem = new YerothQTableWidgetItem(aClientGroupMemberReferenceClient);
    _clientNameItem 	 = new YerothQTableWidgetItem(aClientGroupMemberNomDuClient);


    _mapListIdxToElement_db_ID.insert(_curRow, aClientGroupMember_db_ID);

    unsigned idx = 0;

    //Each call to setItem triggers a call to YerothPointDeVenteWindow::handleQteChange
    setItem(_curRow, idx++, _clientReferenceItem);
    setItem(_curRow, idx++, _clientNameItem);

    set_MY_QStandardItemFlags();

    resizeColumnsToContents();

    int lastCurRow = _curRow;

    if (_curRow >= 0)
    {
    	selectRow(_curRow);
    }

    ++_curRow;

    return lastCurRow;
}
