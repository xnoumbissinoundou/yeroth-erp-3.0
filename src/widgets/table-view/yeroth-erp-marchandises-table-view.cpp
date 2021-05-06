/*
 * yeroth-erp-marchandises-table-view.cpp
 *
 *      Author: PR. XAVIER NOUMBISSI NOUNDOU
 */


#include "yeroth-erp-marchandises-table-view.hpp"

#include "src/yeroth-erp-windows.hpp"

#include "src/utils/yeroth-erp-database-table-column.hpp"

#include "src/utils/yeroth-erp-historique-stock.hpp"

#include "src/utils/yeroth-erp-utils.hpp"

#include "src/utils/yeroth-erp-config.hpp"

#include "src/utils/yeroth-erp-sqltable-model.hpp"

#include "src/widgets/yeroth-erp-qstandard-item.hpp"


#include <QtCore/QDebug>

#include <QtCore/QDate>

#include <QtCore/QMap>

#include <QtSql/QSqlQuery>

#include <QtSql/QSqlRecord>

#include <QtSql/QSqlError>


void YerothERPMarchandisesTableView::lister_les_elements_du_tableau(YerothSqlTableModel &tableModel)
{
	_stdItemModel->_curSqlTableModel = &tableModel;

	YEROTH_TABLE_VIEW_ALL_WINDOWS_POINTER->_marchandisesWindow->_qteTotaleDarticlesEnStock = 0.0;

    emit signal_lister(tableModel);

    bool s = true;

    if (_needExecSelectStatement_FOR_TABLE_VIEW_PAGING_LISTING)
    {
    	s = tableModel.select();
    }

    int rows = tableModel.rowCount();
    int columns = tableModel.columnCount();

    _stdItemModel->setRowCount(rows);
    _stdItemModel->setColumnCount(columns);

//    qDebug() << "++  rows: " << rows;

    if (tableModel.isFromQSqlQuery() && rows <= 0)
    {
    	_stdItemModel->yerothPOSClear();

    	//qDebug() << "++  yerothPOSClear";
    }

    YerothUtils::createTableModelHeaders(tableModel,
    									 *_stdItemModel,
										 _tableModelHeaders,
										 _tableModelRawHeaders_IN_OUT);

    _stdItemModel->setColumnCount(_tableModelRawHeaders_IN_OUT.size());

    if (!s || (0 == _tableModelHeaders.size()) || (0 == _tableModelRawHeaders_IN_OUT.size()))
    {
    	return ;
    }

    QString curTableModelRawHdr;

    QString tmpQvString;

    QStandardItem *anItem = 0;

    QVariant qv;

    int querySize = 0;

    QString nom_departement_produit_str;
    QString designation_str;
    QString categorie_str;

    YerothQStringList aStringList;

    QString sqlSearchStockTableQueryStr;
    QSqlQuery sqlSearchStockTableQuery;

    QSqlRecord record;

    QVariant nom_department_produit;
    QVariant designation;
    QVariant categorie;

    for (int i = 0; i < rows; ++i)
    {
    	record = tableModel.record(i);

    	nom_department_produit = record.value(YerothDatabaseTableColumn::NOM_DEPARTEMENT_PRODUIT);

    	designation = record.value(YerothDatabaseTableColumn::DESIGNATION);

    	categorie = record.value(YerothDatabaseTableColumn::CATEGORIE);

    	for (int k = 0; k < columns; ++k)
    	{
    		curTableModelRawHdr = _tableModelRawHeaders_IN_OUT.at(k);

    		qv.setValue(tableModel.record(i).value(k));

    		anItem = _stdItemModel->item(i, k);

    		if (anItem)
    		{
    			delete anItem;
    		}

    		anItem = new YerothQStandardItem;

    		switch (qv.type())
    		{
    		case QVariant::UInt:
    			anItem = new YerothQStandardItem(GET_NUM_STRING(qv.toUInt()));
    			_stdItemModel->setItem(i, k, anItem);
    			break;

    		case QVariant::Int:

    			if (!YerothUtils::isEqualCaseInsensitive(curTableModelRawHdr, YerothDatabaseTableColumn::ID))
    			{
    				anItem = new YerothQStandardItem(GET_NUM_STRING(qv.toInt()));
    			}
    			else
    			{
    				YEROTH_SAVE_ID_TO_ROW_NUMBER_FOR_YEROTH_TABLE_VIEW(tmpQvString, qv.toInt(), i)
                			anItem = new YerothQStandardItem(tmpQvString);
    			}

    			_stdItemModel->setItem(i, k, anItem);
    			break;

    		case QVariant::Double:
    			anItem = new YerothQStandardItem(GET_DOUBLE_STRING(qv.toDouble()));
    			_stdItemModel->setItem(i, k, anItem);
    			break;

    		case QVariant::ULongLong:
    			anItem = new YerothQStandardItem(GET_NUM_STRING(qv.toULongLong()));
    			_stdItemModel->setItem(i, k, anItem);
    			break;

    		case QVariant::LongLong:
    			anItem = new YerothQStandardItem(GET_NUM_STRING(qv.toLongLong()));
    			_stdItemModel->setItem(i, k, anItem);
    			break;

    		case QVariant::Char:
    			anItem = new YerothQStandardItem(QString(qv.toChar()));
    			_stdItemModel->setItem(i, k, anItem);
    			break;

    		case QVariant::String:
    			tmpQvString.clear();
    			tmpQvString.append(qv.toString());

    			if (!YerothUtils::isEqualCaseInsensitive(curTableModelRawHdr, YerothDatabaseTableColumn::REFERENCE))
    			{
    				anItem = new YerothQStandardItem(tmpQvString, false);
    			}
    			else
    			{
    				anItem = new YerothQStandardItem(tmpQvString);
    			}

    			_stdItemModel->setItem(i, k, anItem);
    			break;

    		case QVariant::Bool:
    			anItem = new YerothQStandardItem(qv.toBool() ? BOOLEAN_STRING_TRUE : BOOLEAN_STRING_FALSE);
    			_stdItemModel->setItem(i, k, anItem);
    			break;

    		case QVariant::Date:
    			anItem = new YerothQStandardItem(DATE_TO_STRING(qv.toDate()));
    			_stdItemModel->setItem(i, k, anItem);
    			break;

    		case QVariant::Time:
    			anItem = new YerothQStandardItem(TIME_TO_STRING(qv.toTime()));
    			_stdItemModel->setItem(i, k, anItem);
    			break;

    		default:
    			anItem = new YerothQStandardItem(YerothUtils::EMPTY_STRING);
    			_stdItemModel->setItem(i, k, anItem);
    			//qDebug() << "YerothTableView::lister(): undecoded QVariant -> " << qv.type();
    			break;
    		}

    		if (0 != anItem)
    		{
    			sqlSearchStockTableQuery.clear();

    			nom_departement_produit_str = nom_department_produit.toString();
    			designation_str = designation.toString();
    			categorie_str = categorie.toString();

    			sqlSearchStockTableQueryStr =
    					QString("SELECT * FROM %1 WHERE %2='%3' AND %4='%5' AND %6='%7'")
						.arg(YerothDatabase::STOCKS,
							 YerothDatabaseTableColumn::NOM_DEPARTEMENT_PRODUIT,
							 nom_departement_produit_str,
							 YerothDatabaseTableColumn::CATEGORIE,
							 categorie_str,
							 YerothDatabaseTableColumn::DESIGNATION,
							 designation_str);

    			//					QDEBUG_STRING_OUTPUT_2("sqlSearchStockTableQueryStr", sqlSearchStockTableQueryStr);

    			querySize = YerothUtils::execQuery(sqlSearchStockTableQuery,
    					sqlSearchStockTableQueryStr);

    			//					QDEBUG_STRING_OUTPUT_2_N("querySize", querySize);

    			/**
    			 * WE USE THIS INFORMATION ABOUT PRODUCT NAME AND
    			 * CATEGORY TO FILTER EMPTY MERCHANDISE OUT !
    			 *
    			 * !!! DO NOT MODIFY IT !!!
    			 */
    			if (querySize <= 0)
    			{
    				anItem->setForeground(YerothUtils::YEROTH_WHITE_COLOR_FOR_EMPTY_MERCHANDISE_FILTERING);

    				anItem->setAccessibleText(QString("%1|%2|%3")
    						.arg(nom_departement_produit_str,
    								categorie_str,
									designation_str));
    			}
    			else
    			{
    				anItem->setForeground(YerothUtils::YEROTH_BLUE_COLOR);
    			}
    		} //if (0 != item)
    	}
    }


    QItemSelectionModel *a_selection_model = selectionModel();

    if (0 != a_selection_model)
    {
    	selectionChanged(a_selection_model->selection(), a_selection_model->selection());
    }


    static YerothERPWindows *curAllWindows = YerothUtils::getAllWindows();

    YEROTH_SET_LAST_TABLE_VIEW_SELECTED_ROW_ID(curAllWindows, curAllWindows->_marchandisesWindow)

    resizeColumnsToContents();

    curAllWindows->_marchandisesWindow->handle_some_actions_tools_enabled();
}


void YerothERPMarchandisesTableView::startEditingModeSelection()
{
	int lastSelectRecord_ROW_NUMBER =
			YEROTH_TABLE_VIEW_ALL_WINDOWS_POINTER->_marchandisesWindow->
			get_INT_last_selected_row_number();

	QStandardItem *anItem = _stdItemModel->item(lastSelectRecord_ROW_NUMBER,
			 YEROTH_TABLE_VIEW_ALL_WINDOWS_POINTER->_marchandisesWindow
			 	 ->getDBTableFieldColumnIndex(YerothDatabaseTableColumn::REFERENCE));

	anItem->setForeground(YerothUtils::YEROTH_BLACK_COLOR);
	anItem->setBackground(Qt::white);


	anItem = _stdItemModel->item(lastSelectRecord_ROW_NUMBER,
			 YEROTH_TABLE_VIEW_ALL_WINDOWS_POINTER->_marchandisesWindow
			 	 ->getDBTableFieldColumnIndex(YerothDatabaseTableColumn::PRIX_DACHAT_PRECEDENT));

	anItem->setForeground(YerothUtils::YEROTH_BLACK_COLOR);
	anItem->setBackground(Qt::white);


	anItem = _stdItemModel->item(lastSelectRecord_ROW_NUMBER,
			 YEROTH_TABLE_VIEW_ALL_WINDOWS_POINTER->_marchandisesWindow
			 	 ->getDBTableFieldColumnIndex(YerothDatabaseTableColumn::PRIX_VENTE_PRECEDENT));

	anItem->setForeground(YerothUtils::YEROTH_BLACK_COLOR);
	anItem->setBackground(Qt::white);

	setEditTriggers(QAbstractItemView::SelectedClicked |
					QAbstractItemView::DoubleClicked);

	setWriteEnabled(true);

	QObject::connect(_stdItemModel,
					 SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &)),
					 this,
					 SLOT(dataChanged(const QModelIndex &, const QModelIndex &)));
}


void YerothERPMarchandisesTableView::dataChanged(const QModelIndex &index,
                                  	  	  	  	 const QModelIndex &bottomRight,
												 const QVector<int> &roles /*= QVector<int>()*/)
{
//	QDEBUG_STRING_OUTPUT_2_N("bottomRight.column()", bottomRight.column());

    if (index != bottomRight)
    {
    	return ;
    }

    if (!_writeEnabled)
    {
    	return ;
    }

    if (0 == _stdItemModel->item(index.row(), 0))
    {
    	return ;
    }

    QString curIDText(_stdItemModel->item(index.row(), 0)->text());

    QString columnHeaderText_VISIBLE(_tableModelHeaders.at(index.column()));

    QString columnHeaderText_NATIVE(_tableModelRawHeaders_IN_OUT.at(index.column()));

    if (!YerothUtils::isEqualCaseInsensitive(columnHeaderText_NATIVE,
    		YerothDatabaseTableColumn::REFERENCE) 					&&

    	!YerothUtils::isEqualCaseInsensitive(columnHeaderText_NATIVE,
    		YerothDatabaseTableColumn::PRIX_DACHAT_PRECEDENT) 		&&

		!YerothUtils::isEqualCaseInsensitive(columnHeaderText_NATIVE,
			YerothDatabaseTableColumn::PRIX_VENTE_PRECEDENT))
    {
    	YerothQMessageBox::information(YEROTH_TABLE_VIEW_ALL_WINDOWS_POINTER->_marchandisesWindow,
    								   QObject::tr("modification non possible"),
									   QObject::trUtf8("modification non possible de la colone '%1' !")
    												.arg(columnHeaderText_VISIBLE));

    	return ;
    }


    QString cellTextData;

    QString designationText;

    QString REAL_DB_ID_NAME_marchandiseTableColumnProperty(
    		YerothDatabaseTableColumn::_tableColumnToUserViewString.key(columnHeaderText_VISIBLE));

    bool success = false;

    QSqlQuery mySqlQuery;

    QString prevValueDataString;

    QVariant qvIndexData = index.data();

    cellTextData = YerothUtils::get_text(qvIndexData);

    QString strSelectPreviousValueQuery(QString("SELECT %1, %2 FROM %3 WHERE %4 = '%5'")
    		.arg(REAL_DB_ID_NAME_marchandiseTableColumnProperty,
    				YerothDatabaseTableColumn::DESIGNATION,
					*_tableName,
					YerothDatabaseTableColumn::ID,
					curIDText));

    QString strUpdateMarchandisesTableQuery(QString("UPDATE %1 SET %2 = '%3' WHERE %4 = '%5'")
    		.arg(*_tableName,
    				REAL_DB_ID_NAME_marchandiseTableColumnProperty,
					cellTextData,
					YerothDatabaseTableColumn::ID,
					curIDText));

    YEROTH_ERP_3_0_START_DATABASE_TRANSACTION;

    int mySqlQuerySize = YerothUtils::execQuery(mySqlQuery, strSelectPreviousValueQuery);

    if (mySqlQuerySize > 0)
    {
    	mySqlQuery.next();

    	prevValueDataString = mySqlQuery.value(0).toString();

    	designationText = mySqlQuery.value(1).toString();

    	mySqlQuery.clear();
    }

    //    		QDEBUG_STRING_OUTPUT_2(QString::number(mySqlQuerySize), strUpdateMarchandisesTableQuery);

    success = YerothUtils::execQuery(strUpdateMarchandisesTableQuery);

    if (success &&
    		YerothUtils::isEqualCaseInsensitive(columnHeaderText_NATIVE,
    				YerothDatabaseTableColumn::REFERENCE))
    {
    	YerothQStringList allToUpdateTables;

    	allToUpdateTables << YerothDatabase::PAIEMENTS
    			<< YerothDatabase::STOCKS
				<< YerothDatabase::STOCKS_SORTIES
				<< YerothDatabase::STOCKS_VENDU
				<< YerothDatabase::ACHATS;

    	QString strUpdateTableQuery;

    	for (uint i = 0; i < allToUpdateTables.size(); ++i)
    	{
    		strUpdateTableQuery.clear();

    		strUpdateTableQuery.append(QString("UPDATE %1 SET %2 = '%3' WHERE %4 = '%5'")
    				.arg(allToUpdateTables.at(i),
    						YerothDatabaseTableColumn::REFERENCE,
							cellTextData,
							YerothDatabaseTableColumn::REFERENCE,
							prevValueDataString));

    		success = YerothUtils::execQuery(strUpdateTableQuery,
    				YEROTH_TABLE_VIEW_ALL_WINDOWS_POINTER->_marchandisesWindow->getLogger()) && success;
    	}
    }

    YEROTH_ERP_3_0_COMMIT_DATABASE_TRANSACTION;


    if (success)
    {
    	QString succesMsgBoxTitle(QObject::trUtf8("succès modification (%1) ")
    										 .arg(columnHeaderText_VISIBLE));

    	YerothQMessageBox::information(YEROTH_TABLE_VIEW_ALL_WINDOWS_POINTER->_marchandisesWindow,
    			succesMsgBoxTitle,
				QObject::trUtf8("Succès de la modification de la colone '%1 (%2)' "
						"de la marchandise '%3' !")
    	.arg(columnHeaderText_VISIBLE,
    			cellTextData,
				designationText),
				QMessageBox::Ok);
    }
    else
    {
    	QString echecMsgBoxTitle(QObject::trUtf8("échec modification (%1) ")
    										.arg(columnHeaderText_VISIBLE));

    	YerothQMessageBox::information(YEROTH_TABLE_VIEW_ALL_WINDOWS_POINTER->_marchandisesWindow,
    			echecMsgBoxTitle,
				QObject::trUtf8("Échec de la modification de la colone '%1 (%2)' "
						"de la marchandise '%3' !")
    	.arg(columnHeaderText_VISIBLE,
    			cellTextData,
				designationText),
				QMessageBox::Ok);
    }

    stopEditingModeSelection();
}


void YerothERPMarchandisesTableView::selectionChanged (const QItemSelection & selected,
                                        		  	   const QItemSelection & deselected)
{
    static YerothERPWindows *curAllWindows = YerothUtils::getAllWindows();

    YerothMarchandisesWindow *marchandises_window =
    		curAllWindows->_marchandisesWindow;

    _MAP_lastSelected_Row__TO__DB_ID.clear();

    QModelIndexList selectedIndexes = QAbstractItemView::selectedIndexes();


    QString cur_marchandise_departement_produit;

    QString cur_marchandise_categorie;

    QString cur_marchandise_designation;


    double cur_marchandises_quantite_totale = 0.0;

    QString db_ID_in_out;

    if (selectedIndexes.size() > 0)
    {
    	uint last_Row_ID = selectedIndexes.size() - 1;

    	for (uint j = 0; j < selectedIndexes.size(); ++j)
    	{
    		marchandises_window->
				getQModelIndex_db_VALUE_from_MODEL_INDEX(YerothDatabaseTableColumn::NOM_DEPARTEMENT_PRODUIT,
														 selectedIndexes.at(j),
														 cur_marchandise_departement_produit);

    		marchandises_window->
				getQModelIndex_db_VALUE_from_MODEL_INDEX(YerothDatabaseTableColumn::CATEGORIE,
														 selectedIndexes.at(j),
														 cur_marchandise_categorie);

    		marchandises_window->
				getQModelIndex_db_VALUE_from_MODEL_INDEX(YerothDatabaseTableColumn::DESIGNATION,
														 selectedIndexes.at(j),
														 cur_marchandise_designation);

    		cur_marchandises_quantite_totale =
    				YerothUtils::GET_QUANTITE_TOTALE_EN_STOCK(cur_marchandise_departement_produit,
    														  cur_marchandise_categorie,
															  cur_marchandise_designation);

    		marchandises_window->
				set_quantite_totale_label_string(cur_marchandises_quantite_totale);


    		marchandises_window->
				getQModelIndex_dbID_from_MODEL_INDEX(selectedIndexes.at(j),
    												 db_ID_in_out);

			_MAP_lastSelected_Row__TO__DB_ID
					.insert(QString::number(selectedIndexes.at(j).row()),
			    			db_ID_in_out);

    		if (last_Row_ID == j)
    		{
    			_lastSelected_Row__ID = selectedIndexes.at(j).row();
    		}
    	}
    }

    /*
     * Cet appel de la fonction 'clearFocus' est necessaire pour
     * que les nouveaux elements du tableau soit visible immediatement.
     */
    clearFocus();
}


void YerothERPMarchandisesTableView::stopEditingModeSelection()
{
	if (!_writeEnabled)
	{
		return ;
	}

	setWriteEnabled(false);

	setEditTriggers(QAbstractItemView::NoEditTriggers);

	QObject::disconnect(_stdItemModel,
			   	   	    SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &)),
						this,
						SLOT(dataChanged(const QModelIndex &, const QModelIndex &)));

	int lastSelectRecord_ROW_NUMBER =
			YEROTH_TABLE_VIEW_ALL_WINDOWS_POINTER->_marchandisesWindow->
			get_INT_last_selected_row_number();

	QStandardItem *anItem = _stdItemModel->item(lastSelectRecord_ROW_NUMBER,
			 YEROTH_TABLE_VIEW_ALL_WINDOWS_POINTER->_marchandisesWindow
			 	 ->getDBTableFieldColumnIndex(YerothDatabaseTableColumn::REFERENCE));

	anItem->setForeground(Qt::white);
	anItem->setBackground(YerothUtils::YEROTH_BLACK_COLOR);


	anItem = _stdItemModel->item(lastSelectRecord_ROW_NUMBER,
			 YEROTH_TABLE_VIEW_ALL_WINDOWS_POINTER->_marchandisesWindow
			 	 ->getDBTableFieldColumnIndex(YerothDatabaseTableColumn::PRIX_DACHAT_PRECEDENT));

	anItem->setForeground(Qt::white);
	anItem->setBackground(YerothUtils::YEROTH_BLACK_COLOR);


	anItem = _stdItemModel->item(lastSelectRecord_ROW_NUMBER,
			 YEROTH_TABLE_VIEW_ALL_WINDOWS_POINTER->_marchandisesWindow
			 	 ->getDBTableFieldColumnIndex(YerothDatabaseTableColumn::PRIX_VENTE_PRECEDENT));

	anItem->setForeground(Qt::white);
	anItem->setBackground(YerothUtils::YEROTH_BLACK_COLOR);
}

