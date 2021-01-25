/*
 * yeroth-erp-windows-commons.cpp
 *      Author: DR. XAVIER NOUMBISSI NOUNDOU
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
	setYerothTableView_FROM_WINDOWS_COMMONS(YerothTableView *aYerothTableView_FROM_WINDOWS_COMMONS)
{
	if (0 == aYerothTableView_FROM_WINDOWS_COMMONS)
	{
		return ;
	}

	_yerothTableView_FROM_WINDOWS_COMMONS = aYerothTableView_FROM_WINDOWS_COMMONS;

    connect(_yerothTableView_FROM_WINDOWS_COMMONS,
    		SIGNAL(clicked(const QModelIndex &)),
    		this,
            SLOT(setLast_YEROTH_TABLE_VIEW_SelectedRow__db_ID(const QModelIndex &)));

    connect(_yerothTableView_FROM_WINDOWS_COMMONS,
    		SIGNAL(pressed(const QModelIndex &)),
    		this,
            SLOT(setLast_YEROTH_TABLE_VIEW_SelectedRow__db_ID(const QModelIndex &)));

    if (0 != _yeroth_PRINT_UTILITIES_TEX_TABLE)
    {
    	_yeroth_PRINT_UTILITIES_TEX_TABLE->setYerothTableView(_yerothTableView_FROM_WINDOWS_COMMONS);
    }
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
//		QDEBUG_STRINGS_OUTPUT_2("YerothWindowsCommons::SQL_UPDATE_YEROTH_TABLE_VIEW_LAST_SELECTED_ROW",
//				QString("There is no SQL TABLE VIEW associated !"));

		return false;
	}

	int lastSelectRecord_ROW_NUMBER = get_INT_last_selected_row_number();

	YerothSqlTableModel *yerothTableViewSQL_TABLE_MODEL =
			YerothERPWindows::getSqlTableModelFromName(_yerothTableView_FROM_WINDOWS_COMMONS->getSqlTableName());

	if (0 == yerothTableViewSQL_TABLE_MODEL)
	{
//		QDEBUG_STRINGS_OUTPUT_2("YerothWindowsCommons::SQL_UPDATE_YEROTH_TABLE_VIEW_LAST_SELECTED_ROW",
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

//	QDEBUG_STRINGS_OUTPUT_2("db_ID <--> db_ROW", QString("%1 <--> %2")
//									.arg(db_ID,
//										 QString::number(dbtableOriginalRowID)));

	bool success = yerothTableViewSQL_TABLE_MODEL->
			updateRecord(dbtableOriginalRowID, resultRecord_IN);

//	QDEBUG_STRINGS_OUTPUT_2(QString("update of SQL TABLE VIEW: %1")
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

//	QDEBUG_STRINGS_OUTPUT_2("QUERY_YEROTH_TABLE_VIEW_LAST_SELECTED_ROW_QUERY_STRING",
//			QUERY_YEROTH_TABLE_VIEW_LAST_SELECTED_ROW_QUERY_STRING);

	QSqlQuery query;

	bool success = YerothUtils::execQuery(query, QUERY_YEROTH_TABLE_VIEW_LAST_SELECTED_ROW_QUERY_STRING);

	if (success && query.next())
	{
		resultRecord_IN_OUT = query.record();

//		QDEBUG_STRINGS_OUTPUT_2("ID", GET_SQL_RECORD_DATA(resultRecord_IN_OUT, YerothDatabaseTableColumn::ID));

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

int YerothWindowsCommons::get_INT_LastListerSelectedRow__ID()
{
	QString lastListerSelectedRow__ID = getLastListerSelectedRow__ID();

	return (lastListerSelectedRow__ID.isEmpty()) ? -1 : lastListerSelectedRow__ID.toInt();
}


const QString & YerothWindowsCommons::getLastListerSelectedRow__ID()
{
	if (0 == _yerothTableView_FROM_WINDOWS_COMMONS)
	{
		return YerothUtils::EMPTY_STRING;
	}

	return _yerothTableView_FROM_WINDOWS_COMMONS->lastSelectedRow__ID();
}


void YerothWindowsCommons::getManuelUtilisateurPDF()
{
    YerothPOSUser *user = _allWindows->getUser();
    if (user)
    {
        QStringList progArguments;
        QProcess aProcess;

        if (user->isAdmin())
        {
            progArguments << YerothERPConfig::FILE_ABSOLUTEPATH_YEROTH_ERP_3_0_MANUEL__DE_LUTILISATEUR_ADMINISTRATEUR;
            aProcess.startDetached(YerothERPConfig::pathToPdfReader, progArguments);
        }
        else if (user->isCaissier())
        {
            progArguments << YerothERPConfig::FILE_ABSOLUTEPATH_YEROTH_ERP_3_0_MANUEL_DE_LUTILISATEUR_CAISSIER;
            aProcess.startDetached(YerothERPConfig::pathToPdfReader, progArguments);
        }
        else if (user->isGestionaireDesStocks())
        {
            progArguments << YerothERPConfig::FILE_ABSOLUTEPATH_YEROTH_ERP_3_0_MANUEL_DE_LUTILISATEUR_GESTIONAIREDESTOCKS;
            aProcess.startDetached(YerothERPConfig::pathToPdfReader, progArguments);
        }
        else if (user->isMagasinier())
        {
            progArguments << YerothERPConfig::FILE_ABSOLUTEPATH_YEROTH_ERP_3_0_MANUEL_DE_LUTILISATEUR_MAGASINIER;
            aProcess.startDetached(YerothERPConfig::pathToPdfReader, progArguments);
        }
        else if (user->isManager())
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


void YerothWindowsCommons::achats_aux_fournisseurs()
{
    rendreInvisible();
    _allWindows->_achats_aux_fournisseursWindow->rendreVisible(_curStocksTableModel);
}


void YerothWindowsCommons::achats_de_stocks()
{
    rendreInvisible();
    _allWindows->_achatsWindow->rendreVisible(_curStocksTableModel);
}


void YerothWindowsCommons::afficherStocks()
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
    _allWindows->_historiquePaiementsWindow->rendreVisible(_curStocksTableModel);
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
    _allWindows->_configurationComptabiliteWindow->rendreVisible(_curStocksTableModel);
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

		if (type.contains(YerothUtils::DATABASE_MYSQL_VARCHAR_TYPE_STRING))
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


void YerothWindowsCommons::resetTableViewHorizontalHeader_DEFAULT_ORDERING()
{
	if (0 == _yerothTableView_FROM_WINDOWS_COMMONS)
	{
		return ;
	}

	//TODO: (ACHEVER L'ALGORITHME).
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


void YerothWindowsCommons::handle_some_actions_tools_enabled()
{
	if (0 == _yerothTableView_FROM_WINDOWS_COMMONS)
	{
		return ;
	}

	unsigned int a_list_size =
			_list_actions_to_enable_on_positive_tableview_ROW_COUNT.size();

	bool enable_action = (_yerothTableView_FROM_WINDOWS_COMMONS->rowCount() > 0);

	// enabling QAction on positive YEROTHTABLEVIEW rowcount
	{
		QAction *anAction = 0;

		for (unsigned int j = 0; j < a_list_size; ++j)
		{
			anAction = _list_actions_to_enable_on_positive_tableview_ROW_COUNT.at(j);

			if (0 != anAction)
			{
				anAction->setVisible(enable_action);
			}
		}
	}

	// enabling YerothPushButton on positive YEROTHTABLEVIEW rowcount
	{
		a_list_size = _list_yeroth_pushbutton_to_enable_on_positive_tableview_ROW_COUNT.size();

		YerothPushButton *aYerothPushButton = 0;

		for (unsigned int j = 0; j < a_list_size; ++j)
		{
			aYerothPushButton = _list_yeroth_pushbutton_to_enable_on_positive_tableview_ROW_COUNT.at(j);

			if (0 != aYerothPushButton)
			{
				aYerothPushButton->setVisible(enable_action);
			}
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

		aYerothLineEditQCompleterSearchFilter_IN_OUT.append(GENERATE_SQL_IS_STMT(correspondingDBFieldKeyValue,
				aTableColumnFieldContentForANDSearch));
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


void YerothWindowsCommons::supprimer_image(QLabel &aLabel_image_produit)
{
	aLabel_image_produit.clear();
	aLabel_image_produit.setAutoFillBackground(false);
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


bool YerothWindowsCommons::imprimer_pdf_document()
{
	if (_latex_template_print_pdf_content.isEmpty() ||
		0 == _yerothTableView_FROM_WINDOWS_COMMONS)
	{
		return false;
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


	QString pdfOutputFileName =
			_yeroth_PRINT_UTILITIES_TEX_TABLE->
				print_YEROTH_document_from_TableView(_latex_template_print_pdf_content,
												     &_documentSpecificElements_FOR_PDF_LATEX_PRINTING);

	if (pdfOutputFileName.isEmpty())
	{
		return false;
	}

	YerothERPProcess::startPdfViewerProcess(pdfOutputFileName);

    return true;
}


void YerothWindowsCommons::changer_utilisateur()
{
    if (YerothUtils::isEqualCaseInsensitive(YerothDatabase::QSQLITE, YerothERPConfig::_db_type))
    {
        YerothPOSUserManager *u = new YerothPOSUserManager(_allWindows);
        u->set_prenom("Xavier");
        u->set_nom("NOUMBISSI-NOUNDOU");
        u->set_nom_utilisateur("xavierp");
        u->set_localisation("Yaoundé");
        u->set_nom_complet("Xavier NOUMBISSI-NOUNDOU");
        _allWindows->setUser(u);
    }
    else
    {
        _allWindows->_changerUtilisateurDialog->setParent(this, Qt::Popup | Qt::Dialog);
        _allWindows->_changerUtilisateurDialog->show();
        _allWindows->_changerUtilisateurDialog->activateWindow();
    }
}


void YerothWindowsCommons::deconnecter_utilisateur()
{
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

//	QDEBUG_STRINGS_OUTPUT_2("db_ID - 1", db_ID);

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

//			QDEBUG_STRINGS_OUTPUT_2("db_ID - 2", db_ID);
//			qDebug() << "++ aYerothTableViewIndex_2:" << aYerothTableViewIndex_2;

			_yerothTableView_FROM_WINDOWS_COMMONS->setCurrentIndex(aYerothTableViewIndex_2);
		}
		else
		{
			QDEBUG_STRINGS_OUTPUT_1("couldn't create and set a default QMODELINDEX !");
		}
	}

	_yerothTableView_FROM_WINDOWS_COMMONS->setLastSelectedRow__ID(db_ID);

	_yerothTableView_FROM_WINDOWS_COMMONS->selectRow(modelIndex.row());

	_yerothTableView_FROM_WINDOWS_COMMONS_LAST_SELECTED_ROW__ID = db_ID;
}

