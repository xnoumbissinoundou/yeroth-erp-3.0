/*
 * yeroth-erp-marchandises-table-view.cpp
 *
 *      Author: Dipl.-Inf. XAVIER NOUMBISSI NOUNDOU
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


YerothERPMarchandisesTableView::YerothERPMarchandisesTableView()
:YerothTableViewWITHpaging(),
 _allWindows(YerothUtils::getAllWindows())
{
	_stdItemModel->_curTableView = this;
}


YerothERPMarchandisesTableView::YerothERPMarchandisesTableView(QWidget * parent)
:YerothTableViewWITHpaging(parent),
 _allWindows(YerothUtils::getAllWindows())
{
	_stdItemModel->_curTableView = this;
}


void YerothERPMarchandisesTableView::lister_les_elements_du_tableau(YerothSqlTableModel &tableModel)
{
	_stdItemModel->_curSqlTableModel = &tableModel;

	_allWindows->_marchandisesWindow->_valeurTheoriqueDinventaire = 0.0;

	_allWindows->_marchandisesWindow->_qteTotaleDarticlesEnStock = 0.0;

    emit signal_lister(tableModel);

    bool s = tableModel.select();

    int rows = tableModel.rowCount();

    int columns = tableModel.columnCount();

    _stdItemModel->setRowCount(rows);

    _stdItemModel->setColumnCount(columns);

    //qDebug() << "++  rows: " << rows;

    if (tableModel.isFromQSqlQuery() && rows <= 0)
    {
    	_stdItemModel->yerothPOSClear();

    	//qDebug() << "++  yerothPOSClear";
    }

    QStringList	tableModelRawHeaders;

    YerothUtils::createTableModelHeaders(tableModel,
    									 *_stdItemModel,
										 *_tableModelHeaders,
										 tableModelRawHeaders);

    QString curTableModelRawHdr;

	QString tmpQvString;

	QStandardItem *anItem = 0;

	QVariant qv;

    if(tableModel.isFromQSqlQuery() || s)
    {
    	int querySize = 0;

    	QString designationStr;
    	QString categorieStr;

    	QStringList aStringList;

        QString sqlSearchStockTableQueryStr;
        QSqlQuery sqlSearchStockTableQuery;

        QSqlRecord record;
        QVariant designation;
        QVariant categorie;

        for (int i = 0; i < rows; ++i)
        {
            record = tableModel.record(i);

            designation = record.value(YerothDatabaseTableColumn::DESIGNATION);

            categorie = record.value(YerothDatabaseTableColumn::CATEGORIE);

            for (int k = 0; k < columns; ++k)
            {
            	curTableModelRawHdr = tableModelRawHeaders.at(k);

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

                	if (!YerothUtils::isEqualCaseInsensitive(curTableModelRawHdr, YerothDatabaseTableColumn::ID))
                	{
                		anItem = new YerothQStandardItem(GET_NUM_STRING(qv.toUInt()));
                	}
                	else
                	{
                		anItem = new YerothQStandardItem(QString::number(qv.toUInt()));
                	}

                    _stdItemModel->setItem(i, k, anItem);
                    break;

                case QVariant::Int:

                	if (!YerothUtils::isEqualCaseInsensitive(curTableModelRawHdr, YerothDatabaseTableColumn::ID))
                	{
                		anItem = new YerothQStandardItem(GET_NUM_STRING(qv.toInt()));
                	}
                	else
                	{
                		anItem = new YerothQStandardItem(QString::number(qv.toInt()));
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
                		anItem = new YerothQStandardItem(YerothUtils::YEROTH_TRUNCATE_STRING_ACCORDING_TO_SETTING(tmpQvString));
                	}
                	else
                	{
                		anItem = new YerothQStandardItem(tmpQvString);
                	}

                    _stdItemModel->setItem(i, k, anItem);
                    break;

                case QVariant::Bool:
                    anItem = new YerothQStandardItem(qv.toBool() ? "True" : "False");
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
                	//We query the database once, only at the first column of a row
                	if (0 == k)
                	{
                		sqlSearchStockTableQuery.clear();

                    	designationStr = designation.toString();
                    	categorieStr = categorie.toString();

                		sqlSearchStockTableQueryStr =
                				QString("SELECT * FROM %1 WHERE %2 = '%3' AND %4 = '%5'")
								.arg(YerothUtils::getAllWindows()->STOCKS,
										YerothDatabaseTableColumn::CATEGORIE,
										categorieStr,
										YerothDatabaseTableColumn::DESIGNATION,
										designationStr);

                		querySize = YerothUtils::execQuery(sqlSearchStockTableQuery,
                										   sqlSearchStockTableQueryStr);
                	}

                	if (0 >= querySize)
                	{
                		anItem->setAccessibleText(QString("%1|%2")
                									.arg(categorieStr,
                										 designationStr));

                		anItem->setForeground(YerothUtils::YEROTH_RED_COLOR);
                	}

                	if (YerothUtils::isEqualCaseInsensitive(curTableModelRawHdr, YerothDatabaseTableColumn::QUANTITE_TOTALE))
                	{
                		double qteTotalEnStock =
                				YerothMarchandisesWindow::getQuantiteTotaleEnStock(categorieStr,
                						designationStr);

                		_allWindows->_marchandisesWindow->_qteTotaleDarticlesEnStock += qteTotalEnStock;

                		anItem->setText(GET_DOUBLE_STRING(qteTotalEnStock));
                	}
                	else if (YerothUtils::isEqualCaseInsensitive(curTableModelRawHdr, YerothDatabaseTableColumn::VALEUR_DIVENTAIRE))
                	{
                		double valeurDinventaireEnStock =
                				YerothMarchandisesWindow::getValeurTotaleDinventaireEnStock(categorieStr,
                						designationStr);

                		_allWindows->_marchandisesWindow->_valeurTheoriqueDinventaire += valeurDinventaireEnStock;

                		anItem->setText(GET_DOUBLE_STRING(valeurDinventaireEnStock));
                	}
                } //if (0 != item)
            }
        }
    }

    this->resizeColumnsToContents();
}


void YerothERPMarchandisesTableView::startEditingModeSelection()
{
	setEditTriggers(QAbstractItemView::SelectedClicked |
					QAbstractItemView::DoubleClicked);

	setWriteEnabled(true);

	QObject::connect(_stdItemModel,
					 SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &)),
					 this,
					 SLOT(dataChanged(const QModelIndex &, const QModelIndex &)));
}


void YerothERPMarchandisesTableView::view_setYerothTableViewPreviousPageNumberText(const QString &aPreviousPageNumberText)
{
	_allWindows->_marchandisesWindow->setYerothTableViewPreviousPageNumberText(aPreviousPageNumberText);
}


void YerothERPMarchandisesTableView::view_setYerothTableViewNextPageNumberText(const QString &aNextPageNumberText)
{
	_allWindows->_marchandisesWindow->setYerothTableViewNextPageNumberText(aNextPageNumberText);
}


void YerothERPMarchandisesTableView::dataChanged(const QModelIndex &index,
                                  	  	  	  	 const QModelIndex &bottomRight,
												 const QVector<int> &roles /*= QVector<int>()*/)
{
    if (index != bottomRight)
    {
    	return ;
    }

    if (_writeEnabled)
    {
    	//qDebug() << "YerothTableView::dataChanged(). Updates table " << *_tableName;

    	QString curIDText;

    	if (0 != _stdItemModel->item(index.row(), 0))
    	{
    		curIDText.append(_stdItemModel->item(index.row(), 0)->text());
    	}

    	QString cellTextData;

    	QString designationText;

    	QString columnHeaderText(_tableModelHeaders->at(index.column()));

    	QString REAL_DB_ID_NAME_marchandiseTableColumnProperty(
    			YerothDatabaseTableColumn::_tableColumnToUserViewString.key(columnHeaderText));

    	bool success = false;

    	if (YerothUtils::isEqualCaseInsensitive(YerothDatabaseTableColumn::REFERENCE,
    											REAL_DB_ID_NAME_marchandiseTableColumnProperty))
    	{
    		QSqlQuery mySqlQuery;

        	QString prevReferenceText;

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

    			prevReferenceText = mySqlQuery.value(0).toString();

    			designationText = mySqlQuery.value(1).toString();

    			mySqlQuery.clear();
    		}

//    		QDEBUG_STRINGS_OUTPUT_2(QString::number(mySqlQuerySize), strUpdateMarchandisesTableQuery);

    		success = YerothUtils::execQuery(strUpdateMarchandisesTableQuery);

    		if (success)
    		{
        		QStringList allToUpdateTables;

        		allToUpdateTables << _allWindows->PAIEMENTS
        						  << _allWindows->STOCKS
        						  << _allWindows->STOCKS_SORTIES
    							  << _allWindows->STOCKS_VENDU
    							  << _allWindows->ACHATS;

        		QString strUpdateTableQuery;

        		for (uint i = 0; i < allToUpdateTables.size(); ++i)
        		{
        			strUpdateTableQuery.clear();

            		strUpdateTableQuery.append(QString("UPDATE %1 SET %2 = '%3' WHERE %4 = '%5'")
            										.arg(allToUpdateTables.at(i),
            											 YerothDatabaseTableColumn::REFERENCE,
            											 cellTextData,
    													 YerothDatabaseTableColumn::REFERENCE,
    													 prevReferenceText));

        			success = YerothUtils::execQuery(strUpdateTableQuery,
        							_allWindows->_marchandisesWindow->getLogger()) && success;
        		}
    		}

    		YEROTH_ERP_3_0_COMMIT_DATABASE_TRANSACTION;
    	}

		if (success)
		{
			QString succesMsgBoxTitle(QObject::trUtf8("succès modification (%1) ")
									.arg(columnHeaderText));

	        YerothQMessageBox::information(_allWindows->_marchandisesWindow,
	        							   succesMsgBoxTitle,
	                                       QObject::trUtf8("Succès de la modification de la colone '%1' (%2) "
	                                    		   	   	   "de la marchandise '%3' !")
	        									.arg(columnHeaderText,
	        										 cellTextData,
													 designationText),
										   QMessageBox::Ok);
		}
		else
		{
			QString echecMsgBoxTitle(QObject::trUtf8("échec modification (%1) ")
									.arg(columnHeaderText));

	        YerothQMessageBox::information(_allWindows->_marchandisesWindow,
	        							   echecMsgBoxTitle,
	                                       QObject::trUtf8("Échec de la modification de la colone '%1' (%2) "
	                                    		   	   	   "de la marchandise '%3' !")
	        									.arg(columnHeaderText,
	        										 cellTextData,
													 designationText),
										   QMessageBox::Ok);
		}

    }

    stopEditingModeSelection();
}


void YerothERPMarchandisesTableView::stopEditingModeSelection()
{
	setWriteEnabled(false);

	setEditTriggers(QAbstractItemView::NoEditTriggers);

	QObject::disconnect(_stdItemModel,
			   	   	    SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &)),
						this,
						SLOT(dataChanged(const QModelIndex &, const QModelIndex &)));
}
