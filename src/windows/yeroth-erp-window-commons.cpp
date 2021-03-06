/*
 * yeroth-erp-windows-commons.cpp
 *
 *      Author: PR. XAVIER NOUMBISSI NOUNDOU
 */
#include "yeroth-erp-window-commons.hpp"


#include "src/process/yeroth-erp-process.hpp"

#include "src/utils/print_latex_pdf/yeroth-erp-print_yeroth_latex_util_pdf.hpp"

#include "src/widgets/yeroth-erp-push-button.hpp"

#include "src/widgets/yeroth-erp-select-db-qcheckbox.hpp"

#include "src/dialogs/yeroth-erp-generic-select-db-field-dialog.hpp"

#include "src/users/yeroth-erp-user-manager.hpp"

#include "src/yeroth-erp-windows.hpp"

#include "src/users/yeroth-erp-users.hpp"


#include <QtSql/QSqlQuery>

#include <QtCore/QtMath>

#include <QtCore/QProcess>

#include <QtCore/QObject>

#include <QtWidgets/QDesktopWidget>


YerothERPWindows *YerothWindowsCommons::_allWindows(0);


QString YerothWindowsCommons::_yerothTableView_FROM_WINDOWS_COMMONS_LAST_SELECTED_ROW__ID;

QPoint *YerothWindowsCommons::_centerPosition(new QPoint);

const uint	YerothWindowsCommons::CHECKBOX_YEROTH_FOR_DB_TABLE_COLUMN_FILTERING_WIDTH_SIZE = 282;


YerothWindowsCommons::~YerothWindowsCommons()
{
	if (0 != _selectExportDBQDialog)
	{
		_selectExportDBQDialog->close();

		delete _selectExportDBQDialog;
	}

	if (0 != _yeroth_PRINT_UTILITIES_TEX_TABLE)
	{
		delete _yeroth_PRINT_UTILITIES_TEX_TABLE;
	}

	_varchar_dbtable_column_name_list.clear();

	_DBFieldNamesToPrintLeftAligned.clear();

	_dbtablecolumnNameToDBColumnIndex.clear();

	_visibleDBColumnNameStrList.clear();

	_visibleQCheckboxs.clear();

	YEROTH_DELETE_FREE_POINTER_NOW(_WRITE_READ_YEROTH_SEMAPHORE_APPLY_USER_SETTING_FILE_PROPERTIES);
}


void YerothWindowsCommons::yeroth_hide_columns()
{
	if (0 == _yerothTableView_FROM_WINDOWS_COMMONS)
	{
		return ;
	}

	unsigned int size = _NOT_VISIBLE_FOR_USER_DB_TABLE_COLUMN_NAME.size();

	for (unsigned int j = 0; j < size; ++j)
	{
		_yerothTableView_FROM_WINDOWS_COMMONS->
			hideColumn(_dbtablecolumnNameToDBColumnIndex
					.value(_NOT_VISIBLE_FOR_USER_DB_TABLE_COLUMN_NAME.at(j)));
	}
}


void YerothWindowsCommons::PRINT_OUTPUT_PRINTING_DEBUG_PARAMETERS()
{
	qDebug() << QString("_pageFROM: %1 - _pageTO: %2, | "
						"_print_table_row_count: %3 | "
						"_a4paper_printing_position: %4")
					.arg(QString::number(_page_from),
						 QString::number(_page_to),
						 QString::number(_print_table_row_count),
						 _a4paper_printing_position);
}


int YerothWindowsCommons::get_INT_last_selected_row_number()
{
	if (0 == _yerothTableView_FROM_WINDOWS_COMMONS)
	{
		return -1;
	}

	return _yerothTableView_FROM_WINDOWS_COMMONS->
			_map_dbID_TO_yeroth_table_view_ROW_NUMBER.
				value(getLastListerSelectedRow__ID(), -1);
}


void YerothWindowsCommons::YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(QAction *anAction,
												   	   	   	   	  bool aBooleanValue)
{
	_MAP_actions_to_enable_on_positive_tableview_ROW_COUNT__TO__AUTHORIZED_FOR_CURRENT_USER
		.insert(anAction, aBooleanValue);

	anAction->setVisible(aBooleanValue);
}


void YerothWindowsCommons::
	setYerothTableView_FROM_WINDOWS_COMMONS(const QList<YerothTableView *> &aYerothTableView_FROM_WINDOWS_COMMONS_QLIST)
{
	if (aYerothTableView_FROM_WINDOWS_COMMONS_QLIST.size() <= 0)
	{
		return ;
	}

	YerothTableView * curTableView = 0;

	curTableView = aYerothTableView_FROM_WINDOWS_COMMONS_QLIST.at(0);

	setYerothTableView_FROM_WINDOWS_COMMONS(curTableView);


	for (int k = 1; k < aYerothTableView_FROM_WINDOWS_COMMONS_QLIST.size(); ++k)
	{
		curTableView = aYerothTableView_FROM_WINDOWS_COMMONS_QLIST.at(k);

		if (0 != curTableView)
		{
			connect(curTableView,
					SIGNAL(clicked(const QModelIndex &)),
					this,
					SLOT(setLast_YEROTH_TABLE_VIEW_SelectedRow__db_ID(const QModelIndex &)));

		    connect(curTableView,
		    		SIGNAL(pressed(const QModelIndex &)),
		    		this,
		            SLOT(setLast_YEROTH_TABLE_VIEW_SelectedRow__db_ID(const QModelIndex &)));
		}
	}
}


void YerothWindowsCommons::
	setYerothTableView_FROM_WINDOWS_COMMONS(YerothTableView *aYerothTableView_FROM_WINDOWS_COMMONS,
											bool a_setLast_YEROTH_TABLE_VIEW_SelectedRow__db_ID /* = true */)
{
	if (0 == aYerothTableView_FROM_WINDOWS_COMMONS)
	{
		return ;
	}

	_yerothTableView_FROM_WINDOWS_COMMONS = aYerothTableView_FROM_WINDOWS_COMMONS;

	if (a_setLast_YEROTH_TABLE_VIEW_SelectedRow__db_ID)
	{
		connect(_yerothTableView_FROM_WINDOWS_COMMONS,
				SIGNAL(clicked(const QModelIndex &)),
				this,
				SLOT(setLast_YEROTH_TABLE_VIEW_SelectedRow__db_ID(const QModelIndex &)));

		connect(_yerothTableView_FROM_WINDOWS_COMMONS,
				SIGNAL(pressed(const QModelIndex &)),
				this,
				SLOT(setLast_YEROTH_TABLE_VIEW_SelectedRow__db_ID(const QModelIndex &)));
	}

    if (0 != _yeroth_PRINT_UTILITIES_TEX_TABLE)
    {
    	_yeroth_PRINT_UTILITIES_TEX_TABLE->setYerothTableView(_yerothTableView_FROM_WINDOWS_COMMONS);
    }
}


void YerothWindowsCommons::YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED_AUTOMATIC_CONSTRUCTOR_ONLY()
{
	QAction *current_qaction = 0;

	for (uint i = 0; i < _list_actions_to_enable_on_positive_tableview_ROW_COUNT.size(); ++i)
	{
		current_qaction =
				_list_actions_to_enable_on_positive_tableview_ROW_COUNT.at(i);

		if (0 != current_qaction)
		{
			YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(current_qaction,false);
		}
	}
}


void YerothWindowsCommons::APPLY_USER_LOCAL_SETTINGS_PARAMETERS_TABLE_COLUMN_ORDER_from_settings_parameters(const QString &pageTableColumnOrder_STRING)
{
	_CURRENTLY_APPLYING_USER_FILE_SETTING_TABLE_COLUMN_ORDER = true;

	set_PARAMETER_TABLE_COLUMN_ORDER(pageTableColumnOrder_STRING);

	QStringList print_table_column_order = pageTableColumnOrder_STRING.split(";");

	if (print_table_column_order.size() > 0)
	{
		YerothTableView *a_yeroth_table_view = GET_YEROTH_TABLE_VIEW();

		if (0 != a_yeroth_table_view)
		{
			QHeaderView *header_view = a_yeroth_table_view->horizontalHeader();

			if (0 != header_view)
			{
				_visibleDBColumnNameStrList.clear();

				int a_logical_index = 0;

				QStringList a_tmp_list;

				int saved_VISUAL_INDEX;

				QString headerColumnData;

				for (uint k = 0; k < print_table_column_order.size(); ++k)
				{
					if (!print_table_column_order.at(k).isEmpty())
					{
						a_tmp_list = print_table_column_order.at(k).split(":");

//						QDEBUG_QSTRINGLIST_OUTPUT("a_tmp_list", a_tmp_list);

						headerColumnData = a_tmp_list.at(0);

						_visibleDBColumnNameStrList.append(headerColumnData);

						saved_VISUAL_INDEX = a_tmp_list.at(1).toInt();

						a_logical_index = _dbtablecolumnNameToDBColumnIndex.value(headerColumnData);

//						QDEBUG_STRING_OUTPUT_1(QString("a_logical_index: %1, headerColumnData: %2, "
//													   "current visualIndex: %3, "
//													   "saved_VISUAL_INDEX: %4")
//													.arg(QString::number(a_logical_index),
//														 headerColumnData,
//														 QString::number(header_view->visualIndex(a_logical_index)),
//														 QString::number(saved_VISUAL_INDEX)));

						header_view->moveSection(header_view->visualIndex(a_logical_index),
								saved_VISUAL_INDEX);
					}
				}
			}
		}
	}

	_CURRENTLY_APPLYING_USER_FILE_SETTING_TABLE_COLUMN_ORDER = false;
}


void YerothWindowsCommons::APPLY_USER_LOCAL_SETTINGS_PARAMETERS()
{
	_WRITE_READ_YEROTH_SEMAPHORE_APPLY_USER_SETTING_FILE_PROPERTIES->acquire(1);

	YerothERPWindows *allWindows = YerothUtils::getAllWindows();

	if (0 != allWindows)
	{
		YerothPOSUser *aUser = allWindows->getUser();

		if (0 != aUser)
		{
			allWindows->_impressionDeDocumentDialog->_current_window_to_print = this;

			aUser->read_user_personal_PRINTING_PARAMETER_settings(this);

			QString yeroth_qt_THIS_object_name = YEROTH_QT_OBJECT_NAME(this);

			QString pageFrom_STRING = aUser->get_PRINTING_PARAMETER_VALUE_page_from(yeroth_qt_THIS_object_name);

			QString pageTo_STRING = aUser->get_PRINTING_PARAMETER_VALUE_page_to(yeroth_qt_THIS_object_name);

			QString pageTablePrintRowCount_STRING = aUser->get_PRINTING_PARAMETER_VALUE_print_table_row_count(yeroth_qt_THIS_object_name);

			QString pageTableRowCount_STRING = aUser->get_PRINTING_PARAMETER_VALUE_USERSQL_table_row_count(yeroth_qt_THIS_object_name);

			QString pageTableColumnOrder_STRING = aUser->get_PARAMETER_VALUE_table_column_order(yeroth_qt_THIS_object_name);

			QString pagePagePrinting_STRING = aUser->get_PRINTING_PARAMETER_VALUE_printing_position(yeroth_qt_THIS_object_name);

			if (!pageFrom_STRING.isEmpty())
			{
				_page_from = pageFrom_STRING.toInt();
			}

			if (!pageTo_STRING.isEmpty())
			{
				_page_to = pageTo_STRING.toInt();
			}

			if (!pageTableRowCount_STRING.isEmpty())
			{
				_USERSQL_table_row_count = pageTableRowCount_STRING.toUInt();

				if (0 != _QLINEEDIT_nombre_de_lignes_par_page)
				{
					_QLINEEDIT_nombre_de_lignes_par_page->setText(pageTableRowCount_STRING);
				}
			}

			if (!pageTablePrintRowCount_STRING.isEmpty())
			{
				_print_table_row_count = pageTablePrintRowCount_STRING.toUInt();
			}

			if (!pageTableColumnOrder_STRING.isEmpty())
			{
				APPLY_USER_LOCAL_SETTINGS_PARAMETERS_TABLE_COLUMN_ORDER_from_settings_parameters(pageTableColumnOrder_STRING);
			}

			if (!pagePagePrinting_STRING.isEmpty())
			{
				_a4paper_printing_position = pagePagePrinting_STRING;
			}

			tableView_show_or_hide_columns(*_yerothTableView_FROM_WINDOWS_COMMONS);
		}
	}

	_WRITE_READ_YEROTH_SEMAPHORE_APPLY_USER_SETTING_FILE_PROPERTIES->release(1);
}


void YerothWindowsCommons::setLeftAligned_FOR_YEROTH_PDF_LATEX_PRINTING(const QString &dbtableColumnString)
{
	int columnIndexStocksID = _dbtablecolumnNameToDBColumnIndex.value(dbtableColumnString);

	_DBFieldNamesToPrintLeftAligned.insert(columnIndexStocksID);

	_DBFieldNamesToPrintCenterAligned.remove(columnIndexStocksID);

	_DBFieldNamesToPrintRightAligned.remove(columnIndexStocksID);
}


void YerothWindowsCommons::setLeftAligned_FOR_YEROTH_PDF_LATEX_PRINTING(int dbtableColumnIndex)
{
	_DBFieldNamesToPrintLeftAligned.insert(dbtableColumnIndex);

	_DBFieldNamesToPrintCenterAligned.remove(dbtableColumnIndex);

	_DBFieldNamesToPrintRightAligned.remove(dbtableColumnIndex);
}


void YerothWindowsCommons::setCenterAligned_FOR_YEROTH_PDF_LATEX_PRINTING(const QString &dbtableColumnString)
{
	int columnIndexStocksID = _dbtablecolumnNameToDBColumnIndex.value(dbtableColumnString);

	_DBFieldNamesToPrintCenterAligned.insert(columnIndexStocksID);

	_DBFieldNamesToPrintLeftAligned.remove(columnIndexStocksID);

	_DBFieldNamesToPrintRightAligned.remove(columnIndexStocksID);
}


void YerothWindowsCommons::setCenterAligned_FOR_YEROTH_PDF_LATEX_PRINTING(int dbtableColumnIndex)
{
	_DBFieldNamesToPrintCenterAligned.insert(dbtableColumnIndex);

	_DBFieldNamesToPrintLeftAligned.remove(dbtableColumnIndex);

	_DBFieldNamesToPrintRightAligned.remove(dbtableColumnIndex);
}


void YerothWindowsCommons::setRightAligned_FOR_YEROTH_PDF_LATEX_PRINTING(const QString &dbtableColumnString)
{
	int columnIndexStocksID = _dbtablecolumnNameToDBColumnIndex.value(dbtableColumnString);

	_DBFieldNamesToPrintRightAligned.insert(columnIndexStocksID);

	_DBFieldNamesToPrintLeftAligned.remove(columnIndexStocksID);

	_DBFieldNamesToPrintCenterAligned.remove(columnIndexStocksID);
}


void YerothWindowsCommons::setRightAligned_FOR_YEROTH_PDF_LATEX_PRINTING(int dbtableColumnIndex)
{
	_DBFieldNamesToPrintRightAligned.insert(dbtableColumnIndex);

	_DBFieldNamesToPrintLeftAligned.remove(dbtableColumnIndex);

	_DBFieldNamesToPrintCenterAligned.remove(dbtableColumnIndex);
}


void YerothWindowsCommons::on_print_actions_visibility_SET(bool isVisible)
{
	QAction *a_current_action = 0;

	QSetIterator<QAction *> a_set_iterator(_ACTIONS_TO_DISABLE_WHEN_NOT_PRINT_ACTION);

	while(a_set_iterator.hasNext())
	{
		a_current_action = a_set_iterator.next();

		if (0 != a_current_action)
		{
			a_current_action->setVisible(isVisible);
		}
	}
}


void YerothWindowsCommons::enableResizing()
{
    setMaximumSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);

    setMinimumSize(width(), height());
}


void YerothWindowsCommons::mySetupUi(QMainWindow * aWindow)
{
    aWindow->setFixedSize(aWindow->width(), aWindow->height());
    aWindow->move(*_centerPosition);
    aWindow->setWindowTitle(getWindowName());
}


bool YerothWindowsCommons::SQL_UPDATE_YEROTH_TABLE_VIEW_LAST_SELECTED_ROW(QSqlRecord &resultRecord_IN)
{
	if (0 == _yerothTableView_FROM_WINDOWS_COMMONS)
	{
//		QDEBUG_STRING_OUTPUT_2("YerothWindowsCommons::SQL_UPDATE_YEROTH_TABLE_VIEW_LAST_SELECTED_ROW",
//				QString("There is no SQL TABLE VIEW associated !"));

		return false;
	}

	int lastSelectRecord_ROW_NUMBER = get_INT_last_selected_row_number();

	YerothSqlTableModel *yerothTableViewSQL_TABLE_MODEL =
			YerothERPWindows::getSqlTableModelFromName(_yerothTableView_FROM_WINDOWS_COMMONS->getSqlTableName());

	if (0 == yerothTableViewSQL_TABLE_MODEL)
	{
//		QDEBUG_STRING_OUTPUT_2("YerothWindowsCommons::SQL_UPDATE_YEROTH_TABLE_VIEW_LAST_SELECTED_ROW",
//				QString("There is no SQL TABLE MODEL for table view: %1")
//					.arg(_yerothTableView_FROM_WINDOWS_COMMONS->getSqlTableName()));

		return false;
	}

//	QDEBUG_STRINGS_OUTPUT_1(QString("row number: %1, db ID: %2")
//								.arg(QString::number(lastSelectRecord_ROW_NUMBER),
//									 GET_SQL_RECORD_DATA(resultRecord_IN,
//											 	 	 	 YerothDatabaseTableColumn::ID)));
	QString db_ID =
			_yerothTableView_FROM_WINDOWS_COMMONS->
				_MAP_lastSelected_Row__TO__DB_ID.
					value(QString::number(lastSelectRecord_ROW_NUMBER));

	int dbtableOriginalRowID =
			_yerothTableView_FROM_WINDOWS_COMMONS->
				_MAP_ORIGINAL_NON_FILTERED_DB_ID__TO__ORIGINAL_DB_ROW.
					value(db_ID.toInt());

//	QDEBUG_STRING_OUTPUT_2("db_ID <--> db_ROW", QString("%1 <--> %2")
//									.arg(db_ID,
//										 QString::number(dbtableOriginalRowID)));

	bool success = yerothTableViewSQL_TABLE_MODEL->
			updateRecord(dbtableOriginalRowID, resultRecord_IN);

//	QDEBUG_STRING_OUTPUT_2(QString("update of SQL TABLE VIEW: %1")
//								.arg(_yerothTableView_FROM_WINDOWS_COMMONS->getSqlTableName()),
//							BOOL_TO_STRING(success));

	return success;
}


bool YerothWindowsCommons::SQL_QUERY_YEROTH_TABLE_VIEW_LAST_SELECTED_ROW(QSqlRecord &resultRecord_IN_OUT)
{
	if (0 == _yerothTableView_FROM_WINDOWS_COMMONS)
	{
		return false;
	}

	QString QUERY_YEROTH_TABLE_VIEW_LAST_SELECTED_ROW_QUERY_STRING(QString("select * from %1 where %2 = '%3'")
																		.arg(_yerothTableView_FROM_WINDOWS_COMMONS->getSqlTableName(),
																			 YerothDatabaseTableColumn::ID,
																			 YerothWindowsCommons::get_last_lister_selected_row_ID()));

//	QDEBUG_STRING_OUTPUT_2("QUERY_YEROTH_TABLE_VIEW_LAST_SELECTED_ROW_QUERY_STRING",
//			QUERY_YEROTH_TABLE_VIEW_LAST_SELECTED_ROW_QUERY_STRING);

	QSqlQuery query;

	bool success = YerothUtils::execQuery(query, QUERY_YEROTH_TABLE_VIEW_LAST_SELECTED_ROW_QUERY_STRING);

	if (success && query.next())
	{
		resultRecord_IN_OUT = query.record();

//		QDEBUG_STRING_OUTPUT_2("ID", GET_SQL_RECORD_DATA(resultRecord_IN_OUT, YerothDatabaseTableColumn::ID));

		return true;
	}

	return false;
}


bool YerothWindowsCommons::SQL_DELETE_YEROTH_TABLE_VIEW_LAST_SELECTED_ROW()
{
	if (0 == _yerothTableView_FROM_WINDOWS_COMMONS)
	{
		return false;
	}

	QString REMOVE_YEROTH_TABLE_VIEW_LAST_SELECTED_ROW_QUERY_STRING(QString("DELETE FROM %1 WHERE %2 = '%3'")
																		.arg(_yerothTableView_FROM_WINDOWS_COMMONS->getSqlTableName(),
																			 YerothDatabaseTableColumn::ID,
																			 YerothWindowsCommons::get_last_lister_selected_row_ID()));

	return YerothUtils::execQuery(REMOVE_YEROTH_TABLE_VIEW_LAST_SELECTED_ROW_QUERY_STRING);
}


void YerothWindowsCommons::set_PARAMETER_TABLE_COLUMN_ORDER()
{
	QHeaderView *a_current_print_table_header_view =
			_yerothTableView_FROM_WINDOWS_COMMONS->horizontalHeader();

	if (0 == a_current_print_table_header_view)
	{
		return ;
	}

	_table_COLUMN_ORDER.clear();


	QStringList yerothTableView_model_raw_visible_headers =
			_yerothTableView_FROM_WINDOWS_COMMONS->getTableModelRawHeaders();

//	QDEBUG_QSTRINGLIST_OUTPUT("_yerothTableView_model_raw_visible_headers",
//							  _yerothTableView_model_raw_visible_headers);

	QString headerColumnData;

	for (uint i = 0; i < yerothTableView_model_raw_visible_headers.size(); ++i)
	{
		headerColumnData = yerothTableView_model_raw_visible_headers.at(i);

		if (!headerColumnData.isEmpty())
		{
//			QDEBUG_STRING_OUTPUT_2("headerColumnData", headerColumnData);

			if (_visibleDBColumnNameStrList.contains(headerColumnData))
			{
				_table_COLUMN_ORDER.append(QString("%1:%2;")
						.arg(headerColumnData,
							 QString::number(a_current_print_table_header_view->visualIndex(i))));
			}
		}
	}

//	QDEBUG_STRING_OUTPUT_2("headerColumnData", headerColumnData);
}


void YerothWindowsCommons::handle_some_actions_tools_enabled()
{
	if (0 == _yerothTableView_FROM_WINDOWS_COMMONS)
	{
		return ;
	}

	bool enable_action = false;

	// enabling QAction on positive YEROTHTABLEVIEW rowcount
	{
		QAction *anAction = 0;

		QMapIterator<QAction *, bool>
			j(_MAP_actions_to_enable_on_positive_tableview_ROW_COUNT__TO__AUTHORIZED_FOR_CURRENT_USER);

		while (j.hasNext())
		{
			j.next();

			anAction = j.key();

			if (0 != anAction)
			{
				if (_list_actions_to_enable_on_positive_tableview_ROW_COUNT.contains(anAction))
				{
					enable_action = (_yerothTableView_FROM_WINDOWS_COMMONS->rowCount() > 0);
				}
				else
				{
					enable_action = true;
				}

				anAction->setVisible( (j.value() && enable_action) );
			}
		}
	}

	// enabling YerothPushButton on positive YEROTHTABLEVIEW rowcount
	{
		enable_action = (_yerothTableView_FROM_WINDOWS_COMMONS->rowCount() > 0);

		int list_size = _list_yeroth_pushbutton_to_enable_on_positive_tableview_ROW_COUNT.size();

		YerothPushButton *aYerothPushButton = 0;

		for (unsigned int j = 0; j < list_size; ++j)
		{
			aYerothPushButton = _list_yeroth_pushbutton_to_enable_on_positive_tableview_ROW_COUNT.at(j);

			if (0 != aYerothPushButton)
			{
				aYerothPushButton->
					setVisible( (aYerothPushButton->isAuthorizedForCurrentUser() && enable_action) );
			}
		}
	}
}


void YerothWindowsCommons::getManuelUtilisateurPDF()
{
    YerothPOSUser *user = _allWindows->getUser();
    if (user)
    {
        QStringList progArguments;
        QProcess aProcess;

        if (user->isManager())
        {
            progArguments << YerothERPConfig::FILE_ABSOLUTEPATH_YEROTH_ERP_3_0_MANUEL_DE_LUTILISATEUR_MANAGER;
            aProcess.startDetached(YerothERPConfig::pathToPdfReader, progArguments);
        }
    }
}


void YerothWindowsCommons::infosEntreprise()
{
    YerothUtils::infosEntreprise(*this, _allWindows->getInfoEntreprise().toString());
}


void YerothWindowsCommons::charges_financieres()
{
    rendreInvisible();
    _allWindows->_charges_financieresWindow->rendreVisible(_curStocksTableModel);
}


void YerothWindowsCommons::achats_de_stocks()
{
    rendreInvisible();
    _allWindows->_achatsWindow->rendreVisible(_curStocksTableModel);
}


void YerothWindowsCommons::stocks()
{
    rendreInvisible();
    _allWindows->_stocksWindow->rendreVisible(_curStocksTableModel);
}


void YerothWindowsCommons::afficherMarchandises()
{
	rendreInvisible();
    _allWindows->_marchandisesWindow->rendreVisible(_curStocksTableModel);
}


void YerothWindowsCommons::alertes()
{
	rendreInvisible();
    _allWindows->_listerAlertesWindow->rendreVisible(_curStocksTableModel);
}


void YerothWindowsCommons::fournisseurs()
{
	rendreInvisible();
    _allWindows->_fournisseursWindow->rendreVisible(_curStocksTableModel);
}


void YerothWindowsCommons::groupes_de_clients()
{
	rendreInvisible();
    _allWindows->_groupesDeClientsWindow->rendreVisible(_curStocksTableModel);
}


void YerothWindowsCommons::programmes_de_fidelite_clients()
{
	rendreInvisible();
    _allWindows->_programmesDeFideliteClientsWindow->rendreVisible(_curStocksTableModel);
}


void YerothWindowsCommons::creer_un_programme_de_fidelite_clients()
{
	rendreInvisible();
    _allWindows->_creerUnProgrammeDeFideliteClientsWindow->rendreVisible(_curStocksTableModel);
}


void YerothWindowsCommons::clients()
{
	rendreInvisible();
    _allWindows->_clientsWindow->rendreVisible(_curStocksTableModel);
}


void YerothWindowsCommons::sortir()
{
	rendreInvisible();
    _allWindows->_sortirWindow->rendreVisible(_curStocksTableModel);
}


void YerothWindowsCommons::tableaux_de_bords()
{
	rendreInvisible();
    _allWindows->_statistiquesWindow->rendreVisible(_curStocksTableModel);
}


void YerothWindowsCommons::mouvements_de_stocks()
{
    _allWindows->_mouvementsDeStocksWindow->rendreVisible(_curStocksTableModel);
    rendreInvisible();
}


void YerothWindowsCommons::creerFournisseur()
{
	rendreInvisible();
    _allWindows->_creerFournisseurWindow->rendreVisible(_curStocksTableModel);
}


void YerothWindowsCommons::creerCompteClient()
{
	rendreInvisible();
    _allWindows->_creerCompteClientWindow->rendreVisible(_curStocksTableModel);
}


void YerothWindowsCommons::creerUnGroupeDeClients()
{
	rendreInvisible();
    _allWindows->_creerGroupeDeClientsWindow->rendreVisible(_curStocksTableModel);
}


void YerothWindowsCommons::entrer()
{
	rendreInvisible();
    _allWindows->_entrerWindow->rendreVisible(_curStocksTableModel);
}


void YerothWindowsCommons::paiements()
{
	rendreInvisible();
    _allWindows->_paiementsWindow->rendreVisible(_curStocksTableModel);
}


void YerothWindowsCommons::ventes()
{
	rendreInvisible();
    _allWindows->_ventesWindow->rendreVisible(_curStocksTableModel);
}


void YerothWindowsCommons::vendre()
{
	rendreInvisible();
    _allWindows->_pdVenteWindow->rendreVisible(_curStocksTableModel);
}


void YerothWindowsCommons::menu()
{
    rendreInvisible();
    _allWindows->_mainWindow->rendreVisible(_curStocksTableModel);
}


void YerothWindowsCommons::comptabilite()
{
    rendreInvisible();
    _allWindows->_comptabiliteWindow->rendreVisible(_curStocksTableModel);
}


void YerothWindowsCommons::administration()
{
    rendreInvisible();
    _allWindows->_adminWindow->rendreVisible(_curStocksTableModel);
}


void YerothWindowsCommons::setup_select_configure_dbcolumn(const QString &aSqlTableName)
{
	_selectExportDBQDialog = new YerothERPGenericSelectDBFieldDialog(_allWindows, this);

	if ( 0 != getQMainWindowToolBar())
	{
		_selectExportDBQDialog->setPalette(getQMainWindowToolBar()->palette());
	}

	_selectExportDBQDialog->setStyleSheet(qMessageBoxStyleSheet());

	QString strShowColumnQuery(QString("SHOW COLUMNS FROM %1")
									.arg(aSqlTableName));

	QSqlQuery query;

	int querySize = YerothUtils::execQuery(query, strShowColumnQuery);

	unsigned int columnIdx = -1;

	for(int k = 0; k < querySize && query.next(); ++k)
	{
		QString fieldName(query.value(0).toString());

		QString type(query.value(1).toString());

		columnIdx = columnIdx + 1;

		if (type.contains(YerothUtils::DATABASE_MYSQL_VARCHAR_TYPE_STRING) 				||
			YerothUtils::isEqualCaseInsensitive(fieldName, YerothDatabaseTableColumn::ID))
		{
			_varchar_dbtable_column_name_list.insert(fieldName);
		}

		if (type.contains(YerothUtils::DATABASE_MYSQL_CHAR_TYPE_STRING) ||
			type.contains(YerothUtils::DATABASE_MYSQL_TIME_TYPE_STRING) ||
			type.contains(YerothUtils::DATABASE_MYSQL_DATE_TYPE_STRING) )
		{
			_DBFieldNamesToPrintLeftAligned.insert(columnIdx);
		}
		else if (type.contains(YerothUtils::DATABASE_MYSQL_DOUBLE_TYPE_STRING) ||
				 type.contains(YerothUtils::DATABASE_MYSQL_INT_TYPE_STRING))
		{
			_DBFieldNamesToPrintRightAligned.insert(columnIdx);
		}

		_dbtablecolumnNameToDBColumnIndex.insert(fieldName, columnIdx);
	}
}


void YerothWindowsCommons::updateYerothLineEditQCompleter(const QString &currentDBColumnString)
{
	QString correspondingDBFieldKeyValue =
			YEROTH_USER_VIEW_STRING_TO_DATABASE_TABLE_COLUMN(currentDBColumnString);

	if (0 == _yeroth_QComboBox_SearchDBFieldColumnString ||
		0 == _yeroth_QLineEdit_SearchDBFieldColumnString ||
		correspondingDBFieldKeyValue.isEmpty())
	{
		return ;
	}

	if (!correspondingDBFieldKeyValue.isEmpty())
	{
		if (!_yeroth_references_dbColumnString.contains(correspondingDBFieldKeyValue))
		{
			_yeroth_QLineEdit_SearchDBFieldColumnString
				->setupMyStaticQCompleter(_dbYerothSqlTableName_WINDOWS_TABLE_VIEW_FOR_SEARCH,
										  correspondingDBFieldKeyValue,
										  true);
		}
		else
		{
			_yeroth_QLineEdit_SearchDBFieldColumnString
				->setupMyStaticQCompleter(_dbYerothSqlTableName_WINDOWS_TABLE_VIEW_FOR_SEARCH,
										  correspondingDBFieldKeyValue,
										  false);
		}
	}
}


void YerothWindowsCommons::setup_print()
{
	if (!_is_pdf_printing_initialized)
	{
		INITIALIZE_PDF_PRINTING_AT_ONCE();

		_is_pdf_printing_initialized = true;
	}

	if (0 != getQMainWindowToolBar())
	{
		_allWindows->_impressionDeDocumentDialog->
			groupBox_IMPRESSION_DE_DOCUMENT->setPalette(getQMainWindowToolBar()->palette());

		_allWindows->_impressionDeDocumentDialog->
			radioButton_IMPRESSION_VERTICAL->setPalette(getQMainWindowToolBar()->palette());

		_allWindows->_impressionDeDocumentDialog->
			radioButton_IMPRESSION_HORIZONTAL->setPalette(getQMainWindowToolBar()->palette());
	}

	_allWindows->_impressionDeDocumentDialog->_current_window_to_print = this;

//	QDEBUG_STRING_OUTPUT_2("setup_print, setting setup_print to: ", YEROTH_QT_OBJECT_NAME(this));

	_allWindows->_impressionDeDocumentDialog->showAsModalDialogWithParent(*this);
}


void YerothWindowsCommons::print_PDF_PREVIOUSLY_SETUP()
{
	if (0 == _yeroth_PRINT_UTILITIES_TEX_TABLE)
	{
		INITIALIZE_PDF_PRINTING_AT_ONCE();
//		return ;
	}

	if (0 != _yeroth_PRINT_UTILITIES_TEX_TABLE)
	{
		_yeroth_PRINT_UTILITIES_TEX_TABLE->SET_NOMBRE_DE_LIGNES(_print_table_row_count);
		_yeroth_PRINT_UTILITIES_TEX_TABLE->_LATEX_A4_PAPER_SPEC = _a4paper_printing_position;

		if (-1 == _page_from ||
			-1 == _page_to)
		{
			imprimer_pdf_document_WITH_A_YEROTH_PROGRESS_BAR();
			return ;
		}
		else
		{
			if (_page_from >
				_page_to)
			{
				YerothQMessageBox::warning(this,
						QObject::trUtf8("IMPRESSION, NUMÉRO DE PAGES"),
						QObject::trUtf8("La numéro de la page de départ doit "
								"être inférieur au numéro de la page terminale"));
				return ;
			}

//			QDEBUG_STRING_OUTPUT_1("YerothWindowsCommons::print_PDF_PREVIOUSLY_SETUP");

			imprimer_pdf_document_WITH_A_YEROTH_PROGRESS_BAR(_page_from, _page_to);
			return ;
		}
	}
}


void YerothWindowsCommons::setYerothLineEditQCompleterSearchFilter(QString &aYerothLineEditQCompleterSearchFilter_IN_OUT)
{
	if (0 == _yeroth_QComboBox_SearchDBFieldColumnString ||
		0 == _yeroth_QLineEdit_SearchDBFieldColumnString)
	{
		return ;
	}

	QString correspondingDBFieldKeyValue;

	QString aTableColumnFieldContentForANDSearch;

	correspondingDBFieldKeyValue
		.append(YEROTH_USER_VIEW_STRING_TO_DATABASE_TABLE_COLUMN(
				_yeroth_QComboBox_SearchDBFieldColumnString->currentText()));

	aTableColumnFieldContentForANDSearch
		.append(_yeroth_QLineEdit_SearchDBFieldColumnString->text());

	if (!correspondingDBFieldKeyValue.isEmpty() 	  &&
		!aTableColumnFieldContentForANDSearch.isEmpty()	)
	{
		if (!aYerothLineEditQCompleterSearchFilter_IN_OUT.isEmpty())
		{
			aYerothLineEditQCompleterSearchFilter_IN_OUT.append(" AND ");
		}

		if (aTableColumnFieldContentForANDSearch.contains(YerothUtils::YerothUtils::PERCENT_CHAR))
		{
			aYerothLineEditQCompleterSearchFilter_IN_OUT.append(
					GENERATE_SQL_LIKE_STMT_AS_IS(correspondingDBFieldKeyValue,
										   	     aTableColumnFieldContentForANDSearch));
		}
		else
		{
			aYerothLineEditQCompleterSearchFilter_IN_OUT.append(
					GENERATE_SQL_IS_STMT(correspondingDBFieldKeyValue,
										 aTableColumnFieldContentForANDSearch));
		}
	}
}


void YerothWindowsCommons::tableView_show_or_hide_columns(YerothTableView &tableView_in_out)
{
	const QStringList &NOT_VISIBLE_FOR_USER_DB_TABLE_COLUMN_NAME =
			get_NOT_VISIBLE_FOR_USER_DB_TABLE_COLUMN_NAME();

	const QStringList &tableView_in_out_RAW_HEADERS =
			tableView_in_out.getTableModelRawHeaders();

    QMapIterator<QString, int> it(_dbtablecolumnNameToDBColumnIndex);

    QString db_table_column_name;

    while (it.hasNext())
    {
    	it.next();

    	db_table_column_name.clear();

    	db_table_column_name.append(it.key());

    	if (!NOT_VISIBLE_FOR_USER_DB_TABLE_COLUMN_NAME.contains(db_table_column_name) &&
    		tableView_in_out_RAW_HEADERS.contains(db_table_column_name)			  	  &&
			_visibleDBColumnNameStrList.contains(db_table_column_name))
    	{
    		tableView_in_out.showColumn(it.value());
    	}
    	else
    	{
    		tableView_in_out.hideColumn(it.value());
    	}
    }

    tableView_in_out.resizeColumnsToContents();

    handle_some_actions_tools_enabled();
}


int YerothWindowsCommons::getDialogBox_LONGUEUR(unsigned int n)
{
	static unsigned int delta = 300;

	int result = qCeil(n/8.0);

	return (result * delta);
}


int YerothWindowsCommons::getDialogBox_Yn_coordinate(unsigned int n)
{
	#define N 8

	static unsigned int y[N] = {7, 37, 67, 97, 127, 157, 187, 217};

	unsigned int modyn = (n%N);

	return y[modyn];
}


int YerothWindowsCommons::getDialogBox_Xn_coordinate(unsigned int n)
{
	static unsigned int w = YerothWindowsCommons::CHECKBOX_YEROTH_FOR_DB_TABLE_COLUMN_FILTERING_WIDTH_SIZE;

	static unsigned int X_0 = 7;

	int result = qFloor(n/8);

	return (X_0 + (result * w));
}


void YerothWindowsCommons::selectionner_champs_db_visibles()
{
	if (_visibleDBColumnNameStrList.size() >= 0)
	{
		_visibleQCheckboxs.clear();
		_visibleQCheckboxs.resize(_visibleDBColumnNameStrList.size());
	}


	QString aDBColumnName;

	QString aDBColumnName_VIEW_STRING;

	for(unsigned k = 0; k < _dbtablecolumnNameToDBColumnIndex.size(); ++k)
	{
		aDBColumnName = _dbtablecolumnNameToDBColumnIndex.key(k);

		if (_NOT_VISIBLE_FOR_USER_DB_TABLE_COLUMN_NAME.contains(aDBColumnName))
		{
			continue ;
		}

		aDBColumnName_VIEW_STRING = YEROTH_DATABASE_TABLE_COLUMN_TO_USER_VIEW_STRING(aDBColumnName);

		if (!aDBColumnName_VIEW_STRING.isEmpty())
		{
			_db_VIEW_STRING_ALPHABETIC_ORDER_To_dbtablecolumnName.insert(aDBColumnName_VIEW_STRING,
												   	   	   	   	     	 aDBColumnName);
		}
	}


	QStringList all_dbfieldColumnName_IN_ALPHABETIC_ORDER(
			_db_VIEW_STRING_ALPHABETIC_ORDER_To_dbtablecolumnName.keys());

	all_dbfieldColumnName_IN_ALPHABETIC_ORDER.sort();


	YerothSelectDBQCheckBox *aQCheckBox = 0;

	for(unsigned k = 0; k < all_dbfieldColumnName_IN_ALPHABETIC_ORDER.size(); ++k)
	{
		aQCheckBox = new YerothSelectDBQCheckBox(_selectExportDBQDialog,
												 &_visibleDBColumnNameStrList);


		aDBColumnName_VIEW_STRING = all_dbfieldColumnName_IN_ALPHABETIC_ORDER.at(k);

		aDBColumnName = _db_VIEW_STRING_ALPHABETIC_ORDER_To_dbtablecolumnName.value(aDBColumnName_VIEW_STRING);

		aQCheckBox->setObjectName(aDBColumnName);

		aQCheckBox->setGeometry(QRect(getDialogBox_Xn_coordinate(k),
									  getDialogBox_Yn_coordinate(k),
									  YerothWindowsCommons::CHECKBOX_YEROTH_FOR_DB_TABLE_COLUMN_FILTERING_WIDTH_SIZE,
									  25));

		aQCheckBox->setText(aDBColumnName_VIEW_STRING);

		if (_visibleDBColumnNameStrList.contains(aDBColumnName))
		{
			aQCheckBox->setChecked(true);
		}

		connect(aQCheckBox,
				SIGNAL(clicked(bool)),
				aQCheckBox,
				SLOT(handle_visible_db_field_checkBox(bool)));

		_visibleQCheckboxs.append(aQCheckBox);
	}

	_selectExportDBQDialog->setWindowTitle(QObject::trUtf8("%1 - sélectionner les colones "
														   "visibles ('%2' colones)")
												.arg(YEROTH_ERP_WINDOW_TITLE,
													 QString::number(all_dbfieldColumnName_IN_ALPHABETIC_ORDER.size())));

	_selectExportDBQDialog->setFixedSize(getDialogBox_LONGUEUR(all_dbfieldColumnName_IN_ALPHABETIC_ORDER.size()), 255);

	_selectExportDBQDialog->showAsModalDialogWithParent(*this);
}


void YerothWindowsCommons::qui_suis_je()
{
    YerothQMessageBox::information(this, QObject::tr("qui suis je ?"), _allWindows->getUser()->toString());
}


void YerothWindowsCommons::INITIALIZE_PDF_PRINTING_AT_ONCE()
{
	if (0 == _yerothTableView_FROM_WINDOWS_COMMONS)
	{
		return ;
	}

	if (_first_time_imprimer_pdf_document_call && 0 != _yerothTableView_FROM_WINDOWS_COMMONS)
	{
		if (_output_print_pdf_latexFileNamePrefix.isEmpty())
		{
			_output_print_pdf_latexFileNamePrefix.append(_windowName);
		}

		_yeroth_PRINT_UTILITIES_TEX_TABLE =
				new YerothTableViewPRINT_UTILITIES_TEX_TABLE(_output_print_pdf_latexFileNamePrefix,
						*this,
						*_yerothTableView_FROM_WINDOWS_COMMONS);
		_first_time_imprimer_pdf_document_call = false;
	}

	if (0 != _yeroth_PRINT_UTILITIES_TEX_TABLE &&
			0 != _allWindows->_impressionDeDocumentDialog)
	{
		_allWindows->_impressionDeDocumentDialog->_yeroth_CURRENT_DOCUMENT_PRINT_UTILITIES_TEX_TABLE =
				_yeroth_PRINT_UTILITIES_TEX_TABLE;
	}

	return ;
}


bool YerothWindowsCommons::imprimer_pdf_document_WITH_PAGES_SPECIFICATION(int *pageFROM,
		   	   	   	   	   	   	   	   	   	     	 	 	 	 	 	  int *pageTO)
{
	if (0 == pageFROM || 0 == pageTO)
	{
		return false;
	}

	if (_latex_template_print_pdf_content.isEmpty())
	{
//		QDEBUG_STRING_OUTPUT_1("YEROTH *");

		_latex_template_print_pdf_content = get_latex_template_print_pdf_content();
	}


	if (_latex_template_print_pdf_content.isEmpty() ||
		0 == _yerothTableView_FROM_WINDOWS_COMMONS)
	{
		return false;
	}

//	QDEBUG_STRING_OUTPUT_1("YEROTH ***");

	static bool first_time_call = true;

	if (_first_time_imprimer_pdf_document_call && 0 != _yerothTableView_FROM_WINDOWS_COMMONS)
	{
		if (_output_print_pdf_latexFileNamePrefix.isEmpty())
		{
			_output_print_pdf_latexFileNamePrefix.append(_windowName);
		}

		_yeroth_PRINT_UTILITIES_TEX_TABLE =
				new YerothTableViewPRINT_UTILITIES_TEX_TABLE(_output_print_pdf_latexFileNamePrefix,
															 *this,
															 *_yerothTableView_FROM_WINDOWS_COMMONS);
		_first_time_imprimer_pdf_document_call = false;
	}

	if (0 != _yeroth_PRINT_UTILITIES_TEX_TABLE &&
		0 != _allWindows->_impressionDeDocumentDialog)
	{
			_allWindows->_impressionDeDocumentDialog->_yeroth_CURRENT_DOCUMENT_PRINT_UTILITIES_TEX_TABLE =
					_yeroth_PRINT_UTILITIES_TEX_TABLE;
	}

//	QDEBUG_STRING_OUTPUT_1("YEROTH *****");

	if (first_time_call)
	{
		first_time_call = false;
		return imprimer_pdf_document_WITH_PAGES_SPECIFICATION(pageFROM, pageTO);
	}


	QString pdfOutputFileName;

//	QDEBUG_STRING_OUTPUT_1("YEROTH YEROTH YEROTH");

	if (-1 == *pageFROM ||
		-1 == *pageTO)
	{
		pdfOutputFileName =
					_yeroth_PRINT_UTILITIES_TEX_TABLE->
						print_YEROTH_document_from_TableView(_latex_template_print_pdf_content,
														     &_documentSpecificElements_FOR_PDF_LATEX_PRINTING);
	}
	else
	{
		pdfOutputFileName =
					_yeroth_PRINT_UTILITIES_TEX_TABLE->
						print_YEROTH_document_from_TableView(*pageFROM,
															 *pageTO,
															 _latex_template_print_pdf_content,
														     &_documentSpecificElements_FOR_PDF_LATEX_PRINTING);
	}


	if (pdfOutputFileName.isEmpty())
	{
		return false;
	}

	YerothERPProcess::startPdfViewerProcess(pdfOutputFileName);

    return true;
}


bool YerothWindowsCommons::imprimer_pdf_document()
{
	int pageFROM = -1;

	int pageTO = -1;

	get_latex_template_print_pdf_content();

	return imprimer_pdf_document_WITH_PAGES_SPECIFICATION(&pageFROM, &pageTO);
}


void YerothWindowsCommons::changer_utilisateur()
{
    _allWindows->_changerUtilisateurDialog->setParent(this, Qt::Popup | Qt::Dialog);
    _allWindows->_changerUtilisateurDialog->show();
    _allWindows->_changerUtilisateurDialog->activateWindow();
}


void YerothWindowsCommons::deconnecter_utilisateur()
{
	YerothPOSUser *aUser = _allWindows->getUser();

	if (0 != aUser)
	{
		aUser->cleanup_user_personal_PRINTING_PARAMETER_settings();
	}

	// This enables user settings privacy.
	slot_reinitialiser_colones_db_visibles();

    _allWindows->definirPasDeRole();
    _allWindows->_mainWindow->show();
    rendreInvisible();
}


void YerothWindowsCommons::rendreVisible(YerothSqlTableModel * stocksTableModel)
{
    _curStocksTableModel = stocksTableModel;
    setVisible(true);
}


void YerothWindowsCommons::fill_table_columns_to_ignore(QList<int> &tableColumnsToIgnore_in_out)
{
    QMapIterator<QString, int> it(_dbtablecolumnNameToDBColumnIndex);

    QString curFieldColumn;

    while (it.hasNext())
    {
    	it.next();

    	curFieldColumn = it.key();

    	if (curFieldColumn.isEmpty())
    	{
    		continue;
    	}

    	if (!_visibleDBColumnNameStrList.contains(curFieldColumn))
    	{
    		tableColumnsToIgnore_in_out.append(it.value());
    	}
    }
}


void YerothWindowsCommons::getQModelIndex_db_VALUE_from_MODEL_INDEX(const QString &dbTableColumnName,
											  	  	  	  	  	    const QModelIndex &modelIndex,
																	QString &db_VALUE_in_out)
{
	db_VALUE_in_out.clear();

	int yerothTableViewColumnIndex_db_VALUE =
			_dbtablecolumnNameToDBColumnIndex.value(dbTableColumnName);

	int selected_row_nr = modelIndex.row();

	QModelIndex id_row_qmodelindex_sibling =
			modelIndex.sibling(selected_row_nr, yerothTableViewColumnIndex_db_VALUE);

	db_VALUE_in_out.append(YerothUtils::get_text(id_row_qmodelindex_sibling.data()));
}


void YerothWindowsCommons::getQModelIndex_dbID_from_MODEL_INDEX(const QModelIndex &modelIndex,
																QString &db_ID_in_out)
{
	db_ID_in_out.clear();

	int yerothTableViewColumnIndex_dbID =
			_dbtablecolumnNameToDBColumnIndex.value(YerothDatabaseTableColumn::ID);

	int selected_row_nr = modelIndex.row();

	QModelIndex id_row_qmodelindex_sibling =
			modelIndex.sibling(selected_row_nr, yerothTableViewColumnIndex_dbID);

	db_ID_in_out.append(YerothUtils::get_text(id_row_qmodelindex_sibling.data()));
}


void YerothWindowsCommons::setLast_YEROTH_TABLE_VIEW_SelectedRow__db_ID()
{
	if (0 == _yerothTableView_FROM_WINDOWS_COMMONS)
	{
		return ;
	}

	YerothPOSQStandardItemModel *model = _yerothTableView_FROM_WINDOWS_COMMONS->getStandardItemModel();

	if (0 == model 				   ||
		0 == model->_curSqlTableModel)
	{
		return ;
	}

//	qDebug() << "++ model->_curSqlTableModel: " << model->_curSqlTableModel->sqlTableName();

	if (_yerothTableView_FROM_WINDOWS_COMMONS->rowCount() > 0)
	{
		QModelIndex aYerothTableViewIndex = model->index(0, 0);

		setLast_YEROTH_TABLE_VIEW_SelectedRow__db_ID(aYerothTableViewIndex);

		_yerothTableView_FROM_WINDOWS_COMMONS->setCurrentIndex(aYerothTableViewIndex);
	}
	else
	{
		QModelIndex aYerothTableViewIndex = model->create_ZERO_ZERO_MODEL_INDEX_FOR_TABLE_VIEWING();

		setLast_YEROTH_TABLE_VIEW_SelectedRow__db_ID(aYerothTableViewIndex);
	}
}


void YerothWindowsCommons::setLast_YEROTH_TABLE_VIEW_SelectedRow__db_ID(const QModelIndex &modelIndex)
{
	if (0 == _yerothTableView_FROM_WINDOWS_COMMONS 			||
		_yerothTableView_FROM_WINDOWS_COMMONS->rowCount() <= 0)
	{
		return ;
	}

	QString db_ID;

	getQModelIndex_dbID_from_MODEL_INDEX(modelIndex, db_ID);

//	QDEBUG_STRING_OUTPUT_2("db_ID - 1", db_ID);

	if (!db_ID.isEmpty() 		&&
		modelIndex.row() >= 0 	&&
		modelIndex.column() >= 0)
	{
//		qDebug() << "++ modelIndex:" << modelIndex;
		_yerothTableView_FROM_WINDOWS_COMMONS->setCurrentIndex(modelIndex);
	}
	else
	{
		YerothPOSQStandardItemModel *model = _yerothTableView_FROM_WINDOWS_COMMONS->getStandardItemModel();

		if (0 != model)
		{
			QModelIndex aYerothTableViewIndex_2 = model->index(0, 0);

			getQModelIndex_dbID_from_MODEL_INDEX(aYerothTableViewIndex_2, db_ID);

//			QDEBUG_STRING_OUTPUT_2("db_ID - 2", db_ID);
//			qDebug() << "++ aYerothTableViewIndex_2:" << aYerothTableViewIndex_2;

			_yerothTableView_FROM_WINDOWS_COMMONS->setCurrentIndex(aYerothTableViewIndex_2);
		}
		else
		{
			QDEBUG_STRING_OUTPUT_1("couldn't create and set a default QMODELINDEX !");
		}
	}

	_yerothTableView_FROM_WINDOWS_COMMONS->setLastSelectedRow__ID(db_ID);

	_yerothTableView_FROM_WINDOWS_COMMONS->selectRow(modelIndex.row());

	_yerothTableView_FROM_WINDOWS_COMMONS_LAST_SELECTED_ROW__ID = db_ID;
}


void YerothWindowsCommons::imprimer_pdf_document_WITH_A_YEROTH_PROGRESS_BAR()
{
	if (0 != _yeroth_PRINT_UTILITIES_TEX_TABLE)
	{
		_yeroth_PRINT_UTILITIES_TEX_TABLE->
			RESET_NOMBRE_DE_LIGNES_TABLEAU_STANDARD();
	}

	YerothProgressBar(this)(this,
							&YerothWindowsCommons::imprimer_pdf_document);
}

