/*
 * yeroth-erp-groupes-dun-client-table-widget.hpp
 *
 *      Author: DR. XAVIER NOUMBISSI NOUNDOU
 */

#ifndef SRC_WIDGETS_YEROTH_GROUPE_DUN_CLIENT_TABLE_WIDGET_HPP_
#define SRC_WIDGETS_YEROTH_GROUPE_DUN_CLIENT_TABLE_WIDGET_HPP_


#include "src/widgets/yeroth-erp-table-widget.hpp"


class YerothGroupesDunClientTableWidget : public YerothTableWidget
{
	Q_OBJECT

public:

	YEROTH_CLASS_OPERATORS

	YerothGroupesDunClientTableWidget(QWidget *parent = 0);

	inline virtual ~YerothGroupesDunClientTableWidget()
	{
	}

	int insert_group(const QString &group_db_ID,
					 const QString &groupDesignation,
					 const QString &groupReference,
                     const QString &groupNumber);

private:

    YerothQTableWidgetItem 	*_groupDesignationItem;

    YerothQTableWidgetItem 	*_groupReferenceItem;

	YerothQTableWidgetItem 	*_groupNumberItem;

};


#endif /* SRC_WIDGETS_YEROTH_GROUPE_DUN_CLIENT_TABLE_WIDGET_HPP_ */
