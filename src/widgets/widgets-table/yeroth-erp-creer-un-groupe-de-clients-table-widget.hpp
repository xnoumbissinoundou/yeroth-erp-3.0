/*
 * yeroth-erp-creer-un-groupe-de-clients-table-widget.hpp
 *
 *      Author: PR. XAVIER NOUMBISSI NOUNDOU
 */

#ifndef SRC_WIDGETS_YEROTH_CREER_UN_GROUPE_DE_CLIENTS_TABLE_WIDGET_HPP_
#define SRC_WIDGETS_YEROTH_CREER_UN_GROUPE_DE_CLIENTS_TABLE_WIDGET_HPP_

#include "src/include/yeroth-erp-3-0-software.text-configuration.hpp"

#include "yeroth-erp-qtable-widget-item.hpp"

#include "src/utils/yeroth-erp-article-vente-info.hpp"

#include "yeroth-erp-point-de-vente-table-widget.hpp"

#include <QtCore/QMap>

class YerothTableWidget;
class YerothQTableWidgetItem;
class YerothLogger;

class YerothCreerUnGroupeDeClientsTableWidget : public YerothTableWidget
{
	Q_OBJECT

public:

	YEROTH_CLASS_OPERATORS

	YerothCreerUnGroupeDeClientsTableWidget(QWidget *parent = 0);

	inline virtual ~YerothCreerUnGroupeDeClientsTableWidget()
	{
	}

	unsigned int addAClientGroupMember(const QString& aClientGroupMember_db_ID,
									   const QString& aClientGroupMemberReferenceClient,
							   	   	   const QString& aClientGroupMemberNomDuClient);

private:

	YerothQTableWidgetItem 	*_clientReferenceItem;

	YerothQTableWidgetItem 	*_clientNameItem;
};


#endif /* SRC_WIDGETS_YEROTH_CREER_UN_GROUPE_DE_CLIENTS_TABLE_WIDGET_HPP_ */
