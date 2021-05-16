/*
 * yeroth-erp-table-view_WITH_TABWIDGET_AS_DIRECT_PARENT_movable_section.hpp
 *
 * Author: PR. XAVIER NOUMBISSI NOUNDOU
 */

#ifndef SRC_YEROTH_TABLE_VIEW_WITH_TABWIDGET_AS_DIRECT_PARENT_MOVABLE_SECTION_HPP_
#define SRC_YEROTH_TABLE_VIEW_WITH_TABWIDGET_AS_DIRECT_PARENT_MOVABLE_SECTION_HPP_


#include "src/widgets/table-view/yeroth-erp-table-view-with-pagination.hpp"


class YerothERP_TABLE_VIEW_WITH_TABWIDGET_AS_DIRECT_PARENT_MOVABLE_SECTION : public YerothTableViewWITHpagination
{
	Q_OBJECT

public:

	YEROTH_CLASS_OPERATORS

	inline YerothERP_TABLE_VIEW_WITH_TABWIDGET_AS_DIRECT_PARENT_MOVABLE_SECTION()
	:YerothTableViewWITHpagination()
	{
	}

	inline YerothERP_TABLE_VIEW_WITH_TABWIDGET_AS_DIRECT_PARENT_MOVABLE_SECTION(QWidget * parent)
	:YerothTableViewWITHpagination(parent)
	{
	}

	inline virtual ~YerothERP_TABLE_VIEW_WITH_TABWIDGET_AS_DIRECT_PARENT_MOVABLE_SECTION()
	{
	}

protected slots:

	virtual void handle_yeroth_header_view_position_changed(int logicalIndex,
															int oldVisualIndex,
															int newVisualIndex);
};


#endif /* SRC_YEROTH_TABLE_VIEW_WITH_TABWIDGET_AS_DIRECT_PARENT_MOVABLE_SECTION_HPP_ */
