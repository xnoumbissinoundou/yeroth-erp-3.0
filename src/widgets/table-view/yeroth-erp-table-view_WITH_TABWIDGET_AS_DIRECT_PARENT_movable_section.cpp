/*
 * yeroth-erp-table-view_WITH_TABWIDGET_AS_DIRECT_PARENT_movable_section.cpp
 *
 * Author: PR. XAVIER NOUMBISSI NOUNDOU
 */

#include "yeroth-erp-table-view_WITH_TABWIDGET_AS_DIRECT_PARENT_movable_section.hpp"


#include "src/yeroth-erp-windows.hpp"

#include "src/windows/yeroth-erp-window-commons.hpp"


#include <QtWidgets/QHeaderView>


void YerothERP_TABLE_VIEW_WITH_TABWIDGET_AS_DIRECT_PARENT_MOVABLE_SECTION::
	handle_yeroth_header_view_position_changed(int logicalIndex,
											   int oldVisualIndex,
											   int newVisualIndex)
{
	//	QDEBUG_QSTRINGLIST_OUTPUT("_tableModelRawHeaders_IN_OUT",
	//							   _tableModelRawHeaders_IN_OUT);

	//	QDEBUG_STRING_OUTPUT_1(QString("handle_yeroth_header_view_position_changed, "
	//						   	   	   "logicalIndex: %1,"
	//						   	   	   "oldVisualIndex: %2, "
	//						   	   	   "newVisualIndex: %3")
	//								.arg(QString::number(logicalIndex),
	//									 QString::number(oldVisualIndex),
	//									 QString::number(newVisualIndex)));

	YerothTableView *a_yeroth_table_view = 0;

	if ( 0 != parent() &&
		(0 != parent()->parent()) &&
		(0 != parent()->parent()->parent()) &&
		(0 != parent()->parent()->parent()->parent()))
	{
		QTabWidget* a_q_tab_widget =
				dynamic_cast<QTabWidget *>(parent()->parent()->parent()->parent()->parent());

		if (0 == a_q_tab_widget)
		{
//			QDEBUG_STRING_OUTPUT_1("YEROTH-TAB_WIDGET-TYPE-NOT_OK");
			return ;
		}

		YerothWindowsCommons* a_yeroth_window =
				dynamic_cast<YerothWindowsCommons *>(a_q_tab_widget->parent());

		if (0 == a_yeroth_window)
		{
//			QDEBUG_STRING_OUTPUT_1("YEROTH-WINDOW-TYPE-NOT_OK");
			return ;
		}

		if (0 != _q_header_view)
		{
			QStringList yerothTableView_model_raw_visible_headers(_tableModelRawHeaders_IN_OUT);

			const QStringList *a_visible_DB_columnname_string_List =
					a_yeroth_window->get_visible_DB_column_name_str_list();

			QString pageTableColumnOrder_STRING;

			if (0 != a_visible_DB_columnname_string_List)
			{
				for (uint i = 0; i < yerothTableView_model_raw_visible_headers.size(); ++i)
				{
					const QString &headerColumnData = yerothTableView_model_raw_visible_headers.at(i);

					if (!headerColumnData.isEmpty())
					{
						if (a_visible_DB_columnname_string_List->contains(headerColumnData))
						{
							pageTableColumnOrder_STRING.append(QString("%1:%2;")
									.arg(headerColumnData,
										 QString::number(_q_header_view->visualIndex(i))));
						}
					}
				}
			}

			a_yeroth_window->set_PARAMETER_TABLE_COLUMN_ORDER(pageTableColumnOrder_STRING);

			YerothERPWindows *allWindows = YerothUtils::getAllWindows();

			if (0 != allWindows)
			{
				YerothPOSUser *aUser = allWindows->getUser();

				if (0 != aUser)
				{
					aUser->save_user_personal_PRINTING_PARAMETER_settings();
				}
			}
		}
	}
}
