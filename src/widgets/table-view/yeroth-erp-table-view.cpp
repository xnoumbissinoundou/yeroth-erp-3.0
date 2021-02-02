/*
 * yeroth-erp-table-view.cpp
 *
 *      Author: DR. XAVIER NOUMBISSI NOUNDOU
 */


#include "yeroth-erp-table-view.hpp"

#include "src/yeroth-erp-windows.hpp"

#include "src/utils/yeroth-erp-database-table-column.hpp"

#include "src/utils/yeroth-erp-historique-stock.hpp"

#include "src/utils/yeroth-erp-utils.hpp"

#include "src/utils/yeroth-erp-config.hpp"

#include "src/utils/yeroth-erp-sqltable-model.hpp"

#include "src/widgets/yeroth-erp-qstandard-item.hpp"

#include "src/windows/yeroth-erp-window-commons.hpp"


#include <QtCore/QDebug>

#include <QtCore/QDate>

#include <QtCore/QMap>

#include <QtSql/QSqlQuery>

#include <QtSql/QSqlRecord>

#include <QtSql/QSqlError>


YerothERPWindows *YerothTableView::YEROTH_TABLE_VIEW_ALL_WINDOWS_POINTER;


YerothTableView::YerothTableView()
:QTableView()
{
    _writeEnabled = false;
    _tableName = 0;

    _tableModelHeaders = new QStringList;
    _stdItemModel = new YerothPOSQStandardItemModel;

    setModel(_stdItemModel);
    setupSelectionOptions();
    setBackgroundRole(QPalette::Window);

    QHeaderView *qHeaderView = horizontalHeader();

    if (0 != qHeaderView)
    {
    	qHeaderView->setSectionsMovable(true);
    }
}

YerothTableView::YerothTableView(QWidget * parent)
:QTableView(parent)
{
    _writeEnabled = false;
    _tableName = 0;

    _tableModelHeaders = new QStringList;
    _stdItemModel = new YerothPOSQStandardItemModel;

    setModel(_stdItemModel);
    setupSelectionOptions();
    setBackgroundRole(QPalette::Window);

    QHeaderView *qHeaderView = horizontalHeader();

    if (0 != qHeaderView)
    {
    	qHeaderView->setSectionsMovable(true);
    }
}

YerothTableView::~YerothTableView()
{
    delete _stdItemModel;
    delete _tableModelHeaders;
}


const QStringList &YerothTableView::get_NOT_VISIBLE_FOR_USER_DB_TABLE_COLUMN_NAME()
{
	return ((YerothWindowsCommons *) parent()->parent())->
				get_NOT_VISIBLE_FOR_USER_DB_TABLE_COLUMN_NAME();
}


void YerothTableView::setupSelectionOptions()
{
    setFrameShadow(QFrame::Plain);
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::ExtendedSelection);
}


void YerothTableView::selectionChanged (const QItemSelection & selected,
                                        const QItemSelection & deselected)
{
    QModelIndexList selectedIndexes = QAbstractItemView::selectedIndexes();

    QString db_ID_in_out;

    if (selectedIndexes.size() > 0)
    {
        _lastSelected_Row__ID = selectedIndexes.at(0).row();
    }

    /*
     * Cet appel de la fonction 'clearFocus' est necessaire pour
     * que les nouveaux elements du tableau soit visible immediatement.
     */
    clearFocus();
}


void YerothTableView::lister_les_elements_du_tableau(YerothSqlTableModel &tableModel)
{
	_stdItemModel->_curSqlTableModel = &tableModel;

	emit signal_lister(tableModel);

	bool s = tableModel.select();

	int rows = tableModel.rowCount();
	int columns = tableModel.columnCount();

	_stdItemModel->setRowCount(rows);
	_stdItemModel->setColumnCount(columns);

	QStringList	tableModelRawHeaders;

    YerothUtils::createTableModelHeaders(tableModel,
    									 *_stdItemModel,
										 *_tableModelHeaders,
										 tableModelRawHeaders);

	QStandardItem *anItem = 0;
	QVariant qv;

	if(s)
	{
		QSqlRecord record;

		for (int i = 0; i < rows; ++i)
		{
			record = tableModel.record(i);

			for (int k = 0; k < columns; ++k)
			{
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
					anItem = new YerothQStandardItem(GET_NUM_STRING(qv.toInt()));
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
					anItem = new YerothQStandardItem(qv.toString());
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
					//qDebug() << "YerothTableView::lister(): undecoded QVariant -> " << qv.type();
					break;
				}

				if (anItem)
				{
					anItem->setForeground(Qt::white);
				}
			}
		}

		 resizeColumnsToContents();
	}
}


void YerothTableView::construire_le_MAPPING_ORIGINAL_db_ID_VERS_db_row_Nr(YerothSqlTableModel &tableModel)
{
	_MAP_ORIGINAL_NON_FILTERED_DB_ID__TO__ORIGINAL_DB_ROW.clear();

    int rows = tableModel.rowCount();

    QString curTableModelRawHdr;

    QVariant qv;

    QSqlRecord record;

    int db_ID_row_column_number = 0;

    for (int i = 0; i < rows; ++i)
    {
    	record = tableModel.record(i);

    	curTableModelRawHdr = tableModel.record(i).fieldName(db_ID_row_column_number);

    	qv.setValue(tableModel.record(i).value(db_ID_row_column_number));

    	switch (qv.type())
    	{
    	case QVariant::Int:

    		if (YerothUtils::isEqualCaseInsensitive(curTableModelRawHdr, YerothDatabaseTableColumn::ID))
    		{
//    			QDEBUG_STRINGS_OUTPUT_2("YerothTableView::construire_le_MAPPING_ORIGINAL_db_ID_VERS_db_row_Nr db_ID <--> db_ROW", QString("%1 <--> %2")
//    					.arg(QString::number(qv.toInt()),
//    							QString::number(i)));

    			_MAP_ORIGINAL_NON_FILTERED_DB_ID__TO__ORIGINAL_DB_ROW.insert(qv.toInt(), i);
    		}

    		break;

    	default:
    		break;
    	}

    }
}


void YerothTableView::lister_les_transactions_dun_fournisseur(QSqlQuery &sqlFournisseurTransactionsUnionQuery)
{
    QString dateHdr(QObject::tr("Date"));
    QString timeHdr(QObject::tr("Heure"));
    QString operationTypeHdr(QObject::tr("Type d'opération"));
    QString transactionAmountHdr(QObject::tr("Montant total"));
    QString customerAccountValueAfterHdr(QObject::trUtf8("Compte fournisseur de services (après)"));
    QString referenceHdr(QObject::trUtf8("Référence service (achat)"));
    QString receiptReferenceHdr(QObject::trUtf8("Référence reçu"));

    _tableModelHeaders->clear();

    _tableModelHeaders->append(dateHdr);
    _tableModelHeaders->append(timeHdr);
    _tableModelHeaders->append(operationTypeHdr);
    _tableModelHeaders->append(transactionAmountHdr);
    _tableModelHeaders->append(customerAccountValueAfterHdr);
    _tableModelHeaders->append(referenceHdr);
    _tableModelHeaders->append(receiptReferenceHdr);


    int querySize = sqlFournisseurTransactionsUnionQuery.size();

	_stdItemModel->yerothPOSClear();

    _stdItemModel->setColumnCount(_tableModelHeaders->size());

    _stdItemModel->setRowCount(querySize);

    //Nous mettons les noms des colones
    for(int k = 0; k < _tableModelHeaders->size(); ++k)
    {
    	_stdItemModel->setHeaderData(k, Qt::Horizontal, _tableModelHeaders->at(k));
    }

    YerothQStandardItem *aYerothQStandardItem = 0;

    if (querySize <= 0)
    {
    	return ;
    }

    int keyValue;
    QString stringKeyValue;

    for (int i = 0; i < querySize; ++i)
    {
    	if (sqlFournisseurTransactionsUnionQuery.next())
    	{
    		QVariant qv;

    		for (int j = 0; j < _stdItemModel->columnCount() ; ++j)
    		{
    			/*
    			 * J'ignore le nom de l'entreprise pour la presentation
    			 * des donnees de transactions de l'entreprise cliente.
    			 * Ce nom d'entreprise apparait deja dans le fichier
    			 * PDF.
    			 *
    			 * J'utilise (j+1), et non (j) comme index !
    			 */

    			qv = sqlFournisseurTransactionsUnionQuery.value(j+1);

    			//					qDebug() << QString("++ j: %1, qv: %2")
    			//									.arg(QString::number(j), qv.toString());

    			switch (qv.type())
    			{
    			case QVariant::Double:
    				/*
    				 * Le montant de la transaction est un 'double'.
    				 * Dans ce cas de figure, utiliser le chaine de
    				 * caractere "N/A" au cas ou il est 'NULL' !
    				 */
    				if (qv.isNull())
    				{
    					aYerothQStandardItem = new YerothQStandardItem("N/A", Qt::AlignRight);
    				}
    				else
    				{
    					aYerothQStandardItem = new YerothQStandardItem(GET_DOUBLE_STRING(qv.toDouble()), Qt::AlignRight);
    				}

    				_stdItemModel->setItem(i, j, aYerothQStandardItem);
    				break;

    			case QVariant::String:

    				if (5 == j ||
    					6 == j)
    				{
        				aYerothQStandardItem = new YerothQStandardItem(qv.toString());
    				}
    				else
    				{
        				aYerothQStandardItem = new YerothQStandardItem(qv.toString(), false);
    				}

    				_stdItemModel->setItem(i, j, aYerothQStandardItem);
    				break;

    			case QVariant::Int:
    				keyValue = qv.toInt();

    				if (2 == j)
    				{
    					if (0 < keyValue)
    					{
    						if (YerothUtils::_typedepaiementToUserViewString.contains(keyValue))
    						{
    							stringKeyValue = YerothUtils::_typedepaiementToUserViewString.value(keyValue);
    						}
    						else if (YerothUtils::_typedeventeToUserViewString.contains(keyValue))
    						{
    							stringKeyValue = YerothUtils::_typedeventeToUserViewString.value(keyValue);
    						}
    					}

    					aYerothQStandardItem = new YerothQStandardItem(stringKeyValue, false);
    					_stdItemModel->setItem(i, j, aYerothQStandardItem);
    				}
    				else
    				{
    					aYerothQStandardItem = new YerothQStandardItem(keyValue, Qt::AlignRight);
    					_stdItemModel->setItem(i, j, aYerothQStandardItem);
    				}

    				break;

    			case QVariant::Date:
    				aYerothQStandardItem = new YerothQStandardItem(DATE_TO_STRING(qv.toDate()));
    				_stdItemModel->setItem(i, j, aYerothQStandardItem);
    				break;

    			case QVariant::Time:
    				aYerothQStandardItem = new YerothQStandardItem(TIME_TO_STRING(qv.toTime()));
    				_stdItemModel->setItem(i, j, aYerothQStandardItem);
    				break;

    			default:
    				//qDebug() << "YerothTableView:::lister(): undecoded QVariant -> " << qv.type();
    				break;
    			}

    			_stdItemModel->setItem(i, j, aYerothQStandardItem);
    		}
    	}
    }
}


void YerothTableView::lister_les_transactions_dun_client(QSqlQuery &sqlClientTransactionsUnionQuery)
{
    QString dateHdr(QObject::tr("Date"));
    QString timeHdr(QObject::tr("Heure"));
    QString operationTypeHdr(QObject::tr("Type d'opération"));
    QString transactionAmountHdr(QObject::tr("Montant total"));
    QString customerAccountValueAfterHdr(QObject::trUtf8("Compte client (après)"));
    QString referenceHdr(QObject::trUtf8("Référence article (service)"));
    QString receiptReferenceHdr(QObject::trUtf8("Référence reçu"));

    _tableModelHeaders->clear();

    _tableModelHeaders->append(dateHdr);
    _tableModelHeaders->append(timeHdr);
    _tableModelHeaders->append(operationTypeHdr);
    _tableModelHeaders->append(transactionAmountHdr);
    _tableModelHeaders->append(customerAccountValueAfterHdr);
    _tableModelHeaders->append(referenceHdr);
    _tableModelHeaders->append(receiptReferenceHdr);


    int querySize = sqlClientTransactionsUnionQuery.size();

	_stdItemModel->yerothPOSClear();

    _stdItemModel->setColumnCount(_tableModelHeaders->size());

    _stdItemModel->setRowCount(querySize);

    //Nous mettons les noms des colones
    for(int k = 0; k < _tableModelHeaders->size(); ++k)
    {
    	_stdItemModel->setHeaderData(k, Qt::Horizontal, _tableModelHeaders->at(k));
    }

    YerothQStandardItem *aYerothQStandardItem = 0;

    if (querySize <= 0)
    {
    	return ;
    }

    int keyValue;
    QString stringKeyValue;

    for (int i = 0; i < querySize; ++i)
    {
    	if (sqlClientTransactionsUnionQuery.next())
    	{
    		QVariant qv;

    		for (int j = 0; j < _stdItemModel->columnCount() ; ++j)
    		{
    			/*
    			 * J'ignore le nom de l'entreprise pour la presentation
    			 * des donnees de transactions de l'entreprise cliente.
    			 * Ce nom d'entreprise apparait deja dans le fichier
    			 * PDF.
    			 *
    			 * J'utilise (j+1), et non (j) comme index !
    			 */

    			qv = sqlClientTransactionsUnionQuery.value(j+1);

    			//					qDebug() << QString("++ j: %1, qv: %2")
    			//									.arg(QString::number(j), qv.toString());

    			switch (qv.type())
    			{
    			case QVariant::Double:
    				/*
    				 * Le montant de la transaction est un 'double'.
    				 * Dans ce cas de figure, utiliser le chaine de
    				 * caractere "N/A" au cas ou il est 'NULL' !
    				 */
    				if (qv.isNull())
    				{
    					aYerothQStandardItem = new YerothQStandardItem("N/A", Qt::AlignRight);
    				}
    				else
    				{
    					aYerothQStandardItem = new YerothQStandardItem(GET_DOUBLE_STRING(qv.toDouble()), Qt::AlignRight);
    				}

    				_stdItemModel->setItem(i, j, aYerothQStandardItem);
    				break;

    			case QVariant::String:

    				if (5 == j ||
    					6 == j)
    				{
        				aYerothQStandardItem = new YerothQStandardItem(qv.toString());
    				}
    				else
    				{
        				aYerothQStandardItem = new YerothQStandardItem(qv.toString(), false);
    				}

    				_stdItemModel->setItem(i, j, aYerothQStandardItem);
    				break;

    			case QVariant::Int:
    				keyValue = qv.toInt();

    				if (2 == j)
    				{
    					if (0 < keyValue)
    					{
    						if (YerothUtils::_typedepaiementToUserViewString.contains(keyValue))
    						{
    							stringKeyValue = YerothUtils::_typedepaiementToUserViewString.value(keyValue);
    						}
    						else if (YerothUtils::_typedeventeToUserViewString.contains(keyValue))
    						{
    							stringKeyValue = YerothUtils::_typedeventeToUserViewString.value(keyValue);
    						}
    					}

    					aYerothQStandardItem = new YerothQStandardItem(stringKeyValue);
    					_stdItemModel->setItem(i, j, aYerothQStandardItem);
    				}
    				else
    				{
    					aYerothQStandardItem = new YerothQStandardItem(keyValue, Qt::AlignRight);
    					_stdItemModel->setItem(i, j, aYerothQStandardItem);
    				}

    				break;

    			case QVariant::Date:
    				aYerothQStandardItem = new YerothQStandardItem(DATE_TO_STRING(qv.toDate()));
    				_stdItemModel->setItem(i, j, aYerothQStandardItem);
    				break;

    			case QVariant::Time:
    				aYerothQStandardItem = new YerothQStandardItem(TIME_TO_STRING(qv.toTime()));
    				_stdItemModel->setItem(i, j, aYerothQStandardItem);
    				break;

    			default:
    				//qDebug() << "YerothTableView:::lister(): undecoded QVariant -> " << qv.type();
    				break;
    			}

    			_stdItemModel->setItem(i, j, aYerothQStandardItem);
    		}
    	}
    }
}


void YerothTableView::lister_lhistorique_du_stock(const QStringList &aMouvementStockList)
{
	//qDebug() << QString("YerothTableView::lister_lhistorique_du_stock(const QStringList &)");

    QString dateHdr(QObject::tr("Date"));
    QString timeHdr(QObject::tr("Heure"));
    QString idHdr(QObject::tr("ID de l'opération"));
    QString operationHdr(QObject::trUtf8("Type d'opération"));
    QString qteInitialeHdr(QObject::trUtf8("Qté initiale en stock"));
    QString qteRetireeHdr(QObject::trUtf8("Qté en mouvement"));
    QString qteRestanteHdr(QObject::trUtf8("Qté restante en stock"));

    _tableModelHeaders->clear();

    _tableModelHeaders->append(dateHdr);
    _tableModelHeaders->append(timeHdr);
    _tableModelHeaders->append(operationHdr);
    _tableModelHeaders->append(idHdr);
    _tableModelHeaders->append(qteInitialeHdr);
    _tableModelHeaders->append(qteRetireeHdr);
    _tableModelHeaders->append(qteRestanteHdr);

	int rowSize = aMouvementStockList.size();

	_stdItemModel->yerothPOSClear();

    _stdItemModel->setColumnCount(_tableModelHeaders->size());

    _stdItemModel->setRowCount(rowSize);

    //Nous mettons les noms des colones
    for(int k = 0; k < _tableModelHeaders->size(); ++k)
    {
    	_stdItemModel->setHeaderData(k, Qt::Horizontal, _tableModelHeaders->at(k));
    }

    QString strOut;

    QStringList anEntry;

    QString curTypeMouvementStock;

	QString unMouvementDeStock;

    YerothQStandardItem *anItem = 0;

	for (int i = 0; i < rowSize; ++i)
	{
		unMouvementDeStock.clear();
		unMouvementDeStock.append(aMouvementStockList.at(i));

		anEntry.clear();
		anEntry.append(unMouvementDeStock.split(YerothHistoriqueStock::SEPARATION_INTERNE));

		for (int j = 0; j < _stdItemModel->columnCount(); ++j)
		{
			//qDebug() << "++ 1: " << anEntry.at(j);

			if (2 == j)
			{
				curTypeMouvementStock = YerothHistoriqueStock::get_type_mouvement_stock_string(anEntry.at(j));

				anItem = new YerothQStandardItem(curTypeMouvementStock);
			}
			else if (5 == j)
			{
				if (QObject::trUtf8("RÉ-APPROVISIONNEMENT") == curTypeMouvementStock ||
					QObject::tr("RETOUR VENTE") == curTypeMouvementStock ||
					QObject::trUtf8("ENTRÉE") == curTypeMouvementStock)
				{
					//5 is corresponds to 'RETOUR'.
					strOut =  "(+) " + anEntry.at(j);
				}
				else
				{
					strOut =  "(-) " + anEntry.at(j);
				}

				anItem = new YerothQStandardItem(strOut, false);
			}
			else
			{
				strOut =  anEntry.at(j);
				anItem = new YerothQStandardItem(strOut, false);
			}

			_stdItemModel->setItem(i, j, anItem);
		}
	}

	resizeColumnsToContents();
}


void YerothTableView::lister(YerothSqlTableModel &tableModel,
							 bool truncateString /* = true*/)
{
	_stdItemModel->_curSqlTableModel = &tableModel;

    emit signal_lister(tableModel);

    bool s = tableModel.select();

    int rows = tableModel.rowCount();
    int columns = tableModel.columnCount();

    _stdItemModel->setRowCount(rows);
    _stdItemModel->setColumnCount(columns);

    QStringList	tableModelRawHeaders;

    YerothUtils::createTableModelHeaders(tableModel,
    									 *_stdItemModel,
										 *_tableModelHeaders,
										 tableModelRawHeaders);
    if (!s)
    {
    	return ;
    }

    QString curTableModelRawHdr;

    QString tmpQvString;
    QStandardItem *anItem = 0;
    QVariant qv;

    for (int i = 0; i < rows; ++i)
    {
    	for (int k = 0; k < columns; ++k)
    	{
    		curTableModelRawHdr = tableModelRawHeaders.at(k);

    		qv.setValue(tableModel.record(i).value(k));

    		anItem = _stdItemModel->item(i, k);

    		if (anItem)
    		{
    			delete anItem;
    		}

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

    			if (! YerothUtils::isEqualCaseInsensitive(curTableModelRawHdr, YerothDatabaseTableColumn::REFERENCE))
    			{
    				if (true == truncateString && tmpQvString.length() > YerothERPConfig::max_string_display_length)
    				{
    					tmpQvString.truncate(YerothERPConfig::max_string_display_length);
    					tmpQvString.append(".");
    				}
    			}

    			anItem = new YerothQStandardItem(tmpQvString);
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
    			//qDebug() << "YerothTableView:::lister(): undecoded QVariant -> " << qv.type();
    			break;
    		}
    	}
    }

    resizeColumnsToContents();
}


void YerothTableView::lister_ALL(YerothSqlTableModel &tableModel,
                                 QMap<QString, QString> &stockNameToStockID_in_out)
{
    emit signal_lister(tableModel);

    QString anALLQueryStr =
    		YerothUtils::getStrategySqlQueryStr(tableModel.yerothSelectStatement(),
    											YerothERPConfig::STRATEGIE_VENTE_SORTIE_ALL);

    bool s = tableModel.yerothSetQuery(anALLQueryStr);

    int rows = tableModel.rowCount();
    int columns = tableModel.columnCount();

    _stdItemModel->setRowCount(rows);
    _stdItemModel->setColumnCount(columns);

    QStringList	tableModelRawHeaders;

    YerothUtils::createTableModelHeaders(tableModel,
    									 *_stdItemModel,
										 *_tableModelHeaders,
										 tableModelRawHeaders);

    if (!s)
    {
    	return ;
    }

    QString curTableModelRawHdr;

	QString tmpQvString;
    QStandardItem *anItem = 0;
    QVariant qv;

    QString curDesignation;

    QSqlRecord record;

    QString curStockID;

    for (int i = 0; i < rows; ++i)
    {
    	record = tableModel.record(i);

    	curStockID = record.value(YerothDatabaseTableColumn::ID).toString();

    	for (int k = 0; k < columns; ++k)
    	{
        	curTableModelRawHdr = tableModelRawHeaders.at(k);

    		qv.setValue(tableModel.record(i).value(k));

    		anItem = _stdItemModel->item(i, k);

    		if (anItem)
    		{
    			delete anItem;
    		}

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
    			if (YerothUtils::isEqualCaseInsensitive(curTableModelRawHdr, YerothDatabaseTableColumn::DESIGNATION))
    			{
    				curDesignation = qv.toString();

    				if (!stockNameToStockID_in_out.contains(curDesignation))
    				{
    					stockNameToStockID_in_out.insert(curDesignation, curStockID);
    				}
    			}

    			tmpQvString.clear();
    			tmpQvString.append(qv.toString());

            	if (!YerothUtils::isEqualCaseInsensitive(curTableModelRawHdr, YerothDatabaseTableColumn::REFERENCE))
    			{
    				if (tmpQvString.length() > YerothERPConfig::max_string_display_length)
    				{
    					tmpQvString.truncate(YerothERPConfig::max_string_display_length);
    					tmpQvString.append(".");
    				}
    			}
    			anItem = new YerothQStandardItem(tmpQvString);
    			_stdItemModel->setItem(i, k, anItem);
    			break;

    		case QVariant::Bool:
    			anItem = new YerothQStandardItem(qv.toBool() ? BOOLEAN_STRING_TRUE : BOOLEAN_STRING_FALSE);
    			_stdItemModel->setItem(i, k, anItem);
    			break;

    		case QVariant::Date:
    			anItem = new YerothQStandardItem(DATE_TO_STRING(qv.toDate()));;
    			_stdItemModel->setItem(i, k, anItem);
    			break;

    		case QVariant::Time:
    			anItem = new YerothQStandardItem(TIME_TO_STRING(qv.toTime()));
    			_stdItemModel->setItem(i, k, anItem);
    			break;

    		default:
    			anItem = new YerothQStandardItem(YerothUtils::EMPTY_STRING);
    			//qDebug() << "YerothTableView::lister_ALL(): undecoded QVariant -> " << qv.type();
    			break;
    		}

    	}//for columns
    }//for rows

    resizeColumnsToContents();

    //    qDebug() << "++ ALL, designationToTableRows_in_out: " << designationToTableRows_in_out;
}


void YerothTableView::lister_FIFO(YerothSqlTableModel &tableModel,
                                  QMap<QString, QString> &stockNameToStockID_in_out)
{
    emit signal_lister(tableModel);

    QString aFIFOQueryStr =
    		YerothUtils::getStrategySqlQueryStr(tableModel.yerothSelectStatement(),
    											YerothERPConfig::STRATEGIE_VENTE_SORTIE_FIFO);

    bool s = tableModel.yerothSetQuery(aFIFOQueryStr);

    int rows = tableModel.rowCount();
    int columns = tableModel.columnCount();

    _stdItemModel->setRowCount(rows);
    _stdItemModel->setColumnCount(columns);

    QStringList	tableModelRawHeaders;

    YerothUtils::createTableModelHeaders(tableModel,
    									 *_stdItemModel,
										 *_tableModelHeaders,
										 tableModelRawHeaders);

    if (!s)
    {
    	return ;
    }

    QString curTableModelRawHdr;

    QStandardItem *anItem = 0;
    QVariant qv;

    QMap<QString, QDate> stockNameToDateEntree;

    QString prevStockName;
    QString curStockName;

    QDate defaultDate;
    defaultDate.setDate(QDate::currentDate().year() + 1000, 12, 31);

    QDate curDate = defaultDate;
    QDate prevDate = defaultDate;

    QString curStockID;

    bool itemHasExpired = false;
    bool testDateEntreeOK = false;

    QSqlRecord record;

    QVariant date_premption;
    QVariant quantite_totale;
    QVariant stock_dalerte;

    for (int i = 0; i < rows; ++i)
    {
    	record = tableModel.record(i);

    	curStockID = record.value(YerothDatabaseTableColumn::ID).toString();

    	quantite_totale = record.value(YerothDatabaseTableColumn::QUANTITE_TOTALE);
    	stock_dalerte = record.value(YerothDatabaseTableColumn::STOCK_DALERTE);

    	itemHasExpired = (date_premption.toDate() < GET_CURRENT_DATE);

    	for (int k = 0; k < columns; ++k)
    	{
        	curTableModelRawHdr = tableModelRawHeaders.at(k);

    		qv.setValue(tableModel.record(i).value(k));

    		anItem = _stdItemModel->item(i, k);

    		if (anItem)
    		{
    			delete anItem;
    		}

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

    			if (YerothUtils::isEqualCaseInsensitive(curTableModelRawHdr, YerothDatabaseTableColumn::DESIGNATION))
    			{
    				prevStockName = curStockName;
    				curStockName = qv.toString();
    			}

            	if (!YerothUtils::isEqualCaseInsensitive(curTableModelRawHdr, YerothDatabaseTableColumn::REFERENCE))
            	{
            		anItem = new YerothQStandardItem(qv.toString(), false);
            	}
            	else
            	{
            		anItem = new YerothQStandardItem(qv.toString());
            	}

    			_stdItemModel->setItem(i, k, anItem);

    			break;

    		case QVariant::Bool:
    			anItem = new YerothQStandardItem(qv.toBool() ? BOOLEAN_STRING_TRUE : BOOLEAN_STRING_FALSE);
    			_stdItemModel->setItem(i, k, anItem);
    			break;

    		case QVariant::Date:

    			if (YerothUtils::isEqualCaseInsensitive(curTableModelRawHdr, YerothDatabaseTableColumn::DATE_ENTREE))
    			{
    				prevDate = curDate;
    				curDate = qv.toDate();

    				if (curDate < prevDate)
    				{
    					testDateEntreeOK = true;
    					stockNameToDateEntree.insert(curStockName, curDate);
    					stockNameToStockID_in_out.insert(curStockName, curStockID);
    					//qDebug() << QString("++ FIFO. 1) curDesignation: %1, curDate: %2")
    					//			.arg(curDesignation, DATE_TO_STRING(designationToDateEntree[curDesignation]));
    				}
    				else
    				{
    					if (!stockNameToDateEntree.contains(curStockName))
    					{
    						testDateEntreeOK = true;
    						stockNameToDateEntree.insert(curStockName, curDate);
    						stockNameToStockID_in_out.insert(curStockName, curStockID);
    						//qDebug() << QString("++ FIFO. 2) curDesignation: %1, curDate: %2")
    						//			.arg(curDesignation, DATE_TO_STRING(designationToDateEntree[curDesignation]));
    					}
    				}
    				//qDebug() << "++ 1. test, curDesignation: " << curDesignation
    				//	 << ", t date: " << designationToDateEntree[curDesignation]
    				// << ", i row: " << i;
    			}

    			anItem = new YerothQStandardItem(DATE_TO_STRING(qv.toDate()));;
    			_stdItemModel->setItem(i, k, anItem);
    			break;

    		case QVariant::Time:
    			anItem = new YerothQStandardItem(TIME_TO_STRING(qv.toTime()));
    			_stdItemModel->setItem(i, k, anItem);
    			break;

    		default:
    			anItem = new YerothQStandardItem(YerothUtils::EMPTY_STRING);
    			//qDebug() << "YerothTableView::lister_FIFO(): undecoded QVariant -> " << qv.type();
    			break;
    		}

    		if (anItem)
    		{
    			anItem->setForeground(Qt::white);

    			anItem->setTextAlignment(Qt::AlignCenter);

    			if (testDateEntreeOK &&
    				YerothUtils::isEqualCaseInsensitive(curTableModelRawHdr, YerothDatabaseTableColumn::DATE_ENTREE))
    			{
    				anItem->setForeground(YerothUtils::YEROTH_GREEN_COLOR);
    				testDateEntreeOK = false;
    			}

    			if (YerothUtils::isEqualCaseInsensitive(curTableModelRawHdr, YerothDatabaseTableColumn::QUANTITE_TOTALE) &&
    					(quantite_totale.toDouble() <= stock_dalerte.toDouble()))
    			{
    				anItem->setForeground(YerothUtils::YEROTH_RED_COLOR);
    			}

    			if (YerothUtils::isEqualCaseInsensitive(curTableModelRawHdr, YerothDatabaseTableColumn::DATE_PEREMPTION))
    			{
    				if (itemHasExpired)
    				{
    					anItem->setForeground(YerothUtils::YEROTH_RED_COLOR);
    				}
    			}
    		}
    	}
    }

    resizeColumnsToContents();

//    qDebug() << "++ FIFO, designationToTableRows_in_out: " << stockNameToStockID_in_out;
}


QDate YerothTableView::getEarliestDate(QMultiMap<QString, QDate> allElements,
                                       QString aDesignation)
{
    QDate defaultDate;
    defaultDate.setDate(QDate::currentDate().year() + 1000, 12, 31);

    if (allElements.count(aDesignation) <= 0)
    {
    	return defaultDate;
    }

    QDate earliestDate = defaultDate;

    //Let's only keep the stock with the earliest
    //(Date of Preemption First) preemption date
    QMap<QString, QDate>::const_iterator itAllReadyUsed = allElements.find(aDesignation);
    QDate curDate;
    while (itAllReadyUsed != allElements.end())
    {
    	curDate = itAllReadyUsed.value();

    	if (curDate < earliestDate)
    	{
    		earliestDate = curDate;
    	}

    	++itAllReadyUsed;
    }

    return earliestDate;
}

QDate YerothTableView::getLatestDate(QMultiMap<QString, QDate> allElements,
                                     QString aDesignation)
{
    QDate defaultDate;
    defaultDate.setDate(QDate::currentDate().year() - 7, 12, 31);

    if (allElements.count(aDesignation) <= 0)
    {
    	return defaultDate;
    }

    QDate latestDate = defaultDate;

    //qDebug() << "++ check date: " << latestDate;

    //qDebug() << QString("++ YerothTableView::getLatestDate. aDesignation: %1")
    //				.arg(aDesignation);

    //qDebug() << "++ YerothTableView::getLatestDate. allElements: "
    //		 << allElements;

    //Let's only keep the stock with the latest date
    //of stock entry
    QMap<QString, QDate>::const_iterator itAllReadyUsed = allElements.find(aDesignation);
    QDate curDate;

    while (itAllReadyUsed != allElements.end())
    {
    	curDate = itAllReadyUsed.value();

    	if (curDate > latestDate)
    	{
    		latestDate = curDate;
    	}

    	++itAllReadyUsed;
    }

    return latestDate;
}

void YerothTableView::lister_LIFO(YerothSqlTableModel &tableModel,
                                  QMap<QString, QString> &stockNameToStockID_in_out)
{
    emit signal_lister(tableModel);

    QString aLIFOStringQuery =
    		YerothUtils::getStrategySqlQueryStr(tableModel.yerothSelectStatement(),
    											YerothERPConfig::STRATEGIE_VENTE_SORTIE_LIFO);

    bool s = tableModel.yerothSetQuery(aLIFOStringQuery);

    int rows = tableModel.rowCount();
    int columns = tableModel.columnCount();

    _stdItemModel->setRowCount(rows);
    _stdItemModel->setColumnCount(columns);

    QStringList	tableModelRawHeaders;

    YerothUtils::createTableModelHeaders(tableModel,
    									 *_stdItemModel,
										 *_tableModelHeaders,
										 tableModelRawHeaders);

    if (!s)
    {
    	return ;
    }

    QString curTableModelRawHdr;

    QMap<QString, QStandardItem *> designationTopreviousLIFOGreenItems;

    QStandardItem *aPreviousLIFOGreenItem = 0;
    QStandardItem *anItem = 0;
    QVariant qv;

    QMap<QString, QDate> designationToDateEntree;

    QString prevStockName;
    QString curStockName;

    QDate defaultDate;
    defaultDate.setDate(QDate::currentDate().year() - 7, 12, 31);

    QDate curDate = defaultDate;
    QDate prevDate = defaultDate;

    bool testDateEntreeOK = false;

    QSqlRecord record;

    QVariant date_premption;
    QVariant quantite_totale;
    QVariant stock_dalerte;

    QString curStockID;

    QMultiMap<QString, QDate> allElements;

    for (int i = 0; i < rows; ++i)
    {
    	record = tableModel.record(i);

    	curStockID = record.value(YerothDatabaseTableColumn::ID).toString();

    	date_premption = record.value(YerothDatabaseTableColumn::DATE_PEREMPTION);
    	quantite_totale = record.value(YerothDatabaseTableColumn::QUANTITE_TOTALE);
    	stock_dalerte = record.value(YerothDatabaseTableColumn::STOCK_DALERTE);

    	for (int k = 0; k < columns; ++k)
    	{
    		curTableModelRawHdr = tableModelRawHeaders.at(k);

    		qv.setValue(tableModel.record(i).value(k));

    		anItem = _stdItemModel->item(i, k);

    		if (anItem)
    		{
    			delete anItem;
    		}

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
    			if (YerothUtils::isEqualCaseInsensitive(curTableModelRawHdr, YerothDatabaseTableColumn::DESIGNATION))
    			{
    				prevStockName = curStockName;
    				curStockName = qv.toString();
    			}

            	if (!YerothUtils::isEqualCaseInsensitive(curTableModelRawHdr, YerothDatabaseTableColumn::REFERENCE))
            	{
            		anItem = new YerothQStandardItem(qv.toString(), false);
            	}
            	else
            	{
            		anItem = new YerothQStandardItem(qv.toString());
            	}

    			_stdItemModel->setItem(i, k, anItem);
    			break;

    		case QVariant::Bool:
    			anItem = new YerothQStandardItem(qv.toBool() ? BOOLEAN_STRING_TRUE : BOOLEAN_STRING_FALSE);
    			_stdItemModel->setItem(i, k, anItem);
    			break;

    		case QVariant::Date:

    			if (YerothUtils::isEqualCaseInsensitive(curTableModelRawHdr, YerothDatabaseTableColumn::DATE_ENTREE))
    			{
    				prevDate = curDate;
    				curDate = qv.toDate();

    				if (curDate >= prevDate)
    				{
    					testDateEntreeOK = true;
    					designationToDateEntree.insert(curStockName, curDate);
    					stockNameToStockID_in_out.insert(curStockName, curStockID);
    					allElements.insert(curStockName, curDate);
    					//                            qDebug() << QString("++ LIFO. 1) curDesignation: %1, curDate: %2")
    					//                            			.arg(curDesignation, DATE_TO_STRING(designationToDateEntree[curDesignation]));
    				}
    				else
    				{
    					if (!designationToDateEntree.contains(curStockName))
    					{
    						testDateEntreeOK = true;
    						designationToDateEntree.insert(curStockName, curDate);
    						stockNameToStockID_in_out.insert(curStockName, curStockID);
    						allElements.insert(curStockName, curDate);
    					}
    				}
    				//qDebug() << "++ 1. test, curDesignation: " << curDesignation
    				//	 << ", t date: " << designationToDateEntree[curDesignation]
    				// << ", i row: " << i;
    			}

    			anItem = new YerothQStandardItem(DATE_TO_STRING(qv.toDate()));;
    			_stdItemModel->setItem(i, k, anItem);
    			break;

    		case QVariant::Time:
    			anItem = new YerothQStandardItem(TIME_TO_STRING(qv.toTime()));
    			_stdItemModel->setItem(i, k, anItem);
    			break;

    		default:
    			anItem = new YerothQStandardItem(YerothUtils::EMPTY_STRING);
    			//qDebug() << "YerothTableView::lister_LIFO(): undecoded QVariant -> " << qv.type();
    			break;
    		}

    		if (0 != anItem)
    		{
    			anItem->setForeground(Qt::white);

    			if (testDateEntreeOK &&
    				YerothUtils::isEqualCaseInsensitive(curTableModelRawHdr, YerothDatabaseTableColumn::DATE_ENTREE))
    			{
    				QDate latestDate = getLatestDate(allElements, curStockName);

    				//                		qDebug() << QString("++ 1, LATEST: designation %1, latestDate: %2")
    				//                            			.arg(curDesignation, DATE_TO_STRING(latestDate));

    				QStandardItem *itemDateOfStockEntry;
    				QString aStockName;
    				QDate itemEntryDate;

    				for(int h = 0; h < i; ++h)
    				{
    					itemDateOfStockEntry = _stdItemModel->item(h,
    							YerothUtils::get_index_of_table_raw_column(tableModelRawHeaders,
    																	   YerothDatabaseTableColumn::DATE_ENTREE));

    					aStockName = _stdItemModel->item(h,
    							YerothUtils::get_index_of_table_raw_column(tableModelRawHeaders,
    							    									   YerothDatabaseTableColumn::DESIGNATION))->text();

    					                                qDebug() << QString("++ aStockName: %1, itemEntryDate: %2")
    					                                				.arg(aStockName, itemDateOfStockEntry->text());

    					if (0 != itemDateOfStockEntry &&
    							YerothUtils::isEqualCaseInsensitive(aStockName, curStockName))
    					{
    						itemEntryDate = GET_DATE_FROM_STRING(itemDateOfStockEntry->text());

    						//                                    qDebug() << QString("++ designation %1, itemEntryDate: %2")
    						//                                    		.arg(aDesignation, itemDateOfStockEntry->text());

    						if (itemEntryDate != latestDate)
    						{
    							itemDateOfStockEntry->setForeground(Qt::white);
    							//                					qDebug() << QString("++ item: %1, itemDateOfStockEntry: %2 <==> white")
    							//                                        		.arg(aDesignation, itemDateOfStockEntry->text());
    						}
    					}
    				}

    				if (latestDate == designationToDateEntree.value(curStockName))
    				{
    					//                			qDebug() << QString("++ YES, designation %1, latestDate: %2")
    					//                                						.arg(curDesignation, DATE_TO_STRING(latestDate));

    					aPreviousLIFOGreenItem = designationTopreviousLIFOGreenItems.value(curStockName);

    					if (0 != aPreviousLIFOGreenItem)
    					{
    						aPreviousLIFOGreenItem->setForeground(Qt::white);
    					}

    					anItem->setForeground(YerothUtils::YEROTH_GREEN_COLOR);

    					aPreviousLIFOGreenItem = anItem;

    					designationTopreviousLIFOGreenItems.insert(curStockName, aPreviousLIFOGreenItem);
    				}

    				testDateEntreeOK = false;
    			}

    			if (YerothUtils::isEqualCaseInsensitive(curTableModelRawHdr, YerothDatabaseTableColumn::DATE_PEREMPTION) &&
    					date_premption.toDate() <= GET_CURRENT_DATE)
    			{
    				anItem->setForeground(YerothUtils::YEROTH_RED_COLOR);
    			}

    			if (YerothUtils::isEqualCaseInsensitive(curTableModelRawHdr, YerothDatabaseTableColumn::QUANTITE_TOTALE) &&
    					quantite_totale.toDouble() <= stock_dalerte.toDouble())
    			{
    				anItem->setForeground(YerothUtils::YEROTH_RED_COLOR);
    			}
    		}
    	}
    }

    resizeColumnsToContents();

    //    qDebug() << "++ LIFO, designationToTableRows_in_out: " << designationToTableRows_in_out;
}


void YerothTableView::lister_FEFO(YerothSqlTableModel &tableModel,
                                  QMap<QString, QString> &stockNameToStockID_in_out)
{
    emit signal_lister(tableModel);

    QString aDEFDEOQueryStr =
    		YerothUtils::getStrategySqlQueryStr(tableModel.yerothSelectStatement(),
    											YerothERPConfig::STRATEGIE_VENTE_SORTIE_FEFO);

    bool s = tableModel.yerothSetQuery(aDEFDEOQueryStr);

    int rows = tableModel.rowCount();
    int columns = tableModel.columnCount();

    _stdItemModel->setRowCount(rows);
    _stdItemModel->setColumnCount(columns);

    QStringList	tableModelRawHeaders;

    YerothUtils::createTableModelHeaders(tableModel,
    									 *_stdItemModel,
										 *_tableModelHeaders,
										 tableModelRawHeaders);

    if (!s)
    {
    	return ;
    }

    QString curTableModelRawHdr;

    QStandardItem *anItem = 0;
    //QStandardItem *prevItem = 0;
    QVariant qv;

    QMap<QString, QDate> stockNameToDatePreemption;
    QMultiMap<QString, QDate> allElements;

    QString prevDesignation;
    QString curDesignation;

    QDate defaultDate;
    defaultDate.setDate(QDate::currentDate().year() + 1000, 12, 31);

    QDate curDate = defaultDate;
    QDate prevDate = defaultDate;

    bool testDatePreemptionOK = false;

    QSqlRecord record;

    QVariant date_premption;
    QVariant quantite_totale;
    QVariant stock_dalerte;

    QString curStockID;

    bool itemHasExpired = false;

    for (int i = 0; i < rows; ++i)
    {
    	record = tableModel.record(i);

    	curStockID = record.value(YerothDatabaseTableColumn::ID).toString();

    	date_premption = record.value(YerothDatabaseTableColumn::DATE_PEREMPTION);
    	quantite_totale = record.value(YerothDatabaseTableColumn::QUANTITE_TOTALE);
    	stock_dalerte = record.value(YerothDatabaseTableColumn::STOCK_DALERTE);

    	itemHasExpired = (date_premption.toDate() < GET_CURRENT_DATE);

    	for (int k = 0; k < columns; ++k)
    	{
    		curTableModelRawHdr = tableModelRawHeaders.at(k);

    		qv.setValue(tableModel.record(i).value(k));

    		anItem = _stdItemModel->item(i, k);

    		if (0 != anItem)
    		{
    			delete anItem;
    		}

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
    			if (YerothUtils::isEqualCaseInsensitive(curTableModelRawHdr, YerothDatabaseTableColumn::DESIGNATION))
    			{
    				prevDesignation = curDesignation;
    				curDesignation = qv.toString();
    			}

            	if (!YerothUtils::isEqualCaseInsensitive(curTableModelRawHdr, YerothDatabaseTableColumn::REFERENCE))
            	{
            		anItem = new YerothQStandardItem(qv.toString(), false);
            	}
            	else
            	{
            		anItem = new YerothQStandardItem(qv.toString());
            	}

    			_stdItemModel->setItem(i, k, anItem);
    			break;

    		case QVariant::Bool:
    			anItem = new YerothQStandardItem(qv.toBool() ? BOOLEAN_STRING_TRUE : BOOLEAN_STRING_FALSE);
    			_stdItemModel->setItem(i, k, anItem);
    			break;

    		case QVariant::Date:

    			if (YerothUtils::isEqualCaseInsensitive(curTableModelRawHdr, YerothDatabaseTableColumn::DATE_PEREMPTION))
    			{
    				prevDate = curDate;
    				curDate = qv.toDate();

    				if (curDate < prevDate)
    				{
    					testDatePreemptionOK = true;
    					allElements.insert(curDesignation, curDate);
    					//qDebug() << "++ 1-OK. curDesignation: "
    					//<< curDesignation << " date: " << curDate;
    				}
    				else
    				{
    					if (!stockNameToDatePreemption.contains(curDesignation))
    					{
    						testDatePreemptionOK = true;
    						allElements.insert(curDesignation, curDate);
    						//qDebug() << "++ 2-OK. curDesignation: "
    								//<< curDesignation << " date: " << curDate;
    					}
    				}
    			}

    			anItem = new YerothQStandardItem(DATE_TO_STRING(qv.toDate()));;
    			_stdItemModel->setItem(i, k, anItem);
    			break;

    		case QVariant::Time:
    			anItem = new YerothQStandardItem(TIME_TO_STRING(qv.toTime()));
    			_stdItemModel->setItem(i, k, anItem);
    			break;

    		default:
    			anItem = new YerothQStandardItem(YerothUtils::EMPTY_STRING);
//    			qDebug() << "YerothTableView::lister_FEFO(): undecoded QVariant -> " << qv.type();
    			break;
    		}

    		if (0 != anItem)
    		{
    			anItem->setForeground(Qt::white);
    			//prevItem = anItem;

    			if (YerothUtils::isEqualCaseInsensitive(curTableModelRawHdr, YerothDatabaseTableColumn::QUANTITE_TOTALE) &&
    					quantite_totale.toDouble() <= stock_dalerte.toDouble())
    			{
    				anItem->setForeground(YerothUtils::YEROTH_RED_COLOR);
    			}
    			else if (YerothUtils::isEqualCaseInsensitive(curTableModelRawHdr, YerothDatabaseTableColumn::DATE_PEREMPTION))
    			{
    				if (itemHasExpired)
    				{
    					anItem->setForeground(YerothUtils::YEROTH_RED_COLOR);
    				}
    				else if (testDatePreemptionOK)
    				{
    					QDate earliestDate = getEarliestDate(allElements, curDesignation);

    					//qDebug() << "\t++ earliestDate: " << earliestDate;
    					QStandardItem *item;
    					QStandardItem *itemDesignation;
    					QString aDesignation;
    					QDate aDate;

    					for(int h = 0; h < i; ++h)
    					{
    						itemDesignation = _stdItemModel->item(h,
    								YerothUtils::get_index_of_table_raw_column(tableModelRawHeaders,
    																		   YerothDatabaseTableColumn::DESIGNATION));

    						if (itemDesignation)
    						{
    							aDesignation = itemDesignation->text();
    							aDate = stockNameToDatePreemption.value(aDesignation);
    							//qDebug() << "++ itemDesignation: " << aDesignation;

    							if (YerothUtils::isEqualCaseInsensitive(aDesignation, curDesignation))
    							{
    								item = _stdItemModel->item(h,
    										YerothUtils::get_index_of_table_raw_column(tableModelRawHeaders,
    																				   YerothDatabaseTableColumn::DATE_PEREMPTION));

    								bool isPreempted = aDate < GET_CURRENT_DATE;
    								if (!isPreempted)
    								{
    									item->setForeground(Qt::white);
    									//qDebug() << QString("\twhite for item %1 at (%2, %3).")
                                            		//					.arg(item->text(),
                                            				//					QString::number(h),
    									//					QString::number(DATE_PREEMPTION_COLUMN));
    								}
    								else
    								{
    									//item is preempted. so leave its colour as it is (red).
    								}
    							}
    						}
    					}

    					stockNameToDatePreemption.insert(curDesignation, earliestDate);
    					stockNameToStockID_in_out.insert(curDesignation, curStockID);

    					//qDebug() << "++ ici: " << designationToDatePreemption;

    					anItem->setForeground(YerothUtils::YEROTH_GREEN_COLOR);
    					//qDebug() << QString("\tgreen for %1 with date %2.")
    					//					.arg(curDesignation,
    					//					DATE_TO_STRING(earliestDate));

    					testDatePreemptionOK = false;
    				}
    			}
    		} // first if
    	} //switch-case
    }

    resizeColumnsToContents();
    //    qDebug() << "++ FEFO, designationToTableRows_in_out: " << designationToTableRows_in_out;
}


void YerothTableView::lister_codebar_ALL(YerothSqlTableModel &tableModel,
                                         QMap<QString, QString> &stockReferenceToStockID_in_out)
{
    stockReferenceToStockID_in_out.clear();

    emit signal_lister(tableModel);

    QString aReferenceALLQueryStr =
    		YerothUtils::getStrategySqlQueryStr(tableModel.yerothSelectStatement(),
    											YerothERPConfig::STRATEGIE_VENTE_SORTIE_ALL);

//    qDebug() << QString("++ aReferenceALLQueryStr: %1")
//    				.arg(aReferenceALLQueryStr);

    bool s = tableModel.yerothSetQuery(aReferenceALLQueryStr);

    int rows = tableModel.rowCount();
    int columns = tableModel.columnCount();

    _stdItemModel->setRowCount(rows);
    _stdItemModel->setColumnCount(columns);

    QStringList	tableModelRawHeaders;

    YerothUtils::createTableModelHeaders(tableModel,
    									 *_stdItemModel,
										 *_tableModelHeaders,
										 tableModelRawHeaders);

    if (!s)
    {
    	return ;
    }

    QString curTableModelRawHdr;

    QStandardItem *anItem = 0;
    QVariant qv;

    QString curStockReference;

    QSqlRecord record;

    QVariant date_premption;

    QString curStockID;

    bool itemHasExpired = false;

    for (int i = 0; i < rows; ++i)
    {
    	record = tableModel.record(i);

    	curStockID = record.value(YerothDatabaseTableColumn::ID).toString();

    	date_premption = record.value(YerothDatabaseTableColumn::DATE_PEREMPTION);

    	itemHasExpired = (date_premption.toDate() < GET_CURRENT_DATE);

    	for (int k = 0; k < columns; ++k)
    	{
    		curTableModelRawHdr = tableModelRawHeaders.at(k);

    		qv.setValue(tableModel.record(i).value(k));

    		anItem = _stdItemModel->item(i, k);

    		if (0 != anItem)
    		{
    			delete anItem;
    		}

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

    			if (YerothUtils::isEqualCaseInsensitive(curTableModelRawHdr, YerothDatabaseTableColumn::REFERENCE) &&
    					!itemHasExpired)
    			{
    				curStockReference = qv.toString();

    				if (!stockReferenceToStockID_in_out.contains(curStockReference))
    				{
    					stockReferenceToStockID_in_out.insert(curStockReference, curStockID);
    					anItem = new YerothQStandardItem(curStockReference);
    					_stdItemModel->setItem(i, k, anItem);
    				}
    			}

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
//    			qDebug() << "YerothTableView::lister_codebar_ALL(): undecoded QVariant -> " << qv.type();
    			break;
    		}
    	}
    }
    //    qDebug() << "++ ALL, codebarToTableRows_in_out: " << codebarToTableRows_in_out;
}


void YerothTableView::lister_codebar_FIFO(YerothSqlTableModel &tableModel,
        								  QMap<QString, QString> &stockReferenceToStockID_in_out)
{
    emit signal_lister(tableModel);

    QString aReferenceFIFOQueryStr =
    		YerothUtils::getStrategySqlQueryStr(tableModel.yerothSelectStatement(),
    											YerothERPConfig::STRATEGIE_VENTE_SORTIE_FIFO);

    bool s = tableModel.yerothSetQuery(aReferenceFIFOQueryStr);

    int rows = tableModel.rowCount();
    int columns = tableModel.columnCount();

    _stdItemModel->setRowCount(rows);
    _stdItemModel->setColumnCount(columns);

    QStringList	tableModelRawHeaders;

    YerothUtils::createTableModelHeaders(tableModel,
    									 *_stdItemModel,
										 *_tableModelHeaders,
										 tableModelRawHeaders);

    if (!s)
    {
    	return ;
    }

    QString curTableModelRawHdr;

    QStandardItem *anItem = 0;
    QVariant qv;

    QString prevStockReference;
    QString curStockReference;

    QDate defaultDate;
    defaultDate.setDate(QDate::currentDate().year() + 1000, 12, 31);

    QDate curDate = defaultDate;
    QDate prevDate = defaultDate;

    QSqlRecord record;

    QVariant date_premption;

    QString curStockID;

    bool itemHasExpired = false;

    for (int i = 0; i < rows; ++i)
    {
    	record = tableModel.record(i);

    	curStockID = record.value(YerothDatabaseTableColumn::ID).toString();

    	date_premption = record.value(YerothDatabaseTableColumn::DATE_PEREMPTION);

    	itemHasExpired = (date_premption.toDate() < GET_CURRENT_DATE);

    	for (int k = 0; k < columns; ++k)
    	{
    		curTableModelRawHdr = tableModelRawHeaders.at(k);

    		qv.setValue(tableModel.record(i).value(k));

    		anItem = _stdItemModel->item(i, k);

    		if (0 != anItem)
    		{
    			delete anItem;
    		}

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
    			if (YerothUtils::isEqualCaseInsensitive(curTableModelRawHdr, YerothDatabaseTableColumn::REFERENCE) &&
    					!itemHasExpired)
    			{
    				prevStockReference = curStockReference;
    				curStockReference = qv.toString();
    			}
    			anItem = new YerothQStandardItem(qv.toString());
    			_stdItemModel->setItem(i, k, anItem);
    			break;

    		case QVariant::Bool:
    			anItem = new YerothQStandardItem(qv.toBool() ? BOOLEAN_STRING_TRUE : BOOLEAN_STRING_FALSE);
    			_stdItemModel->setItem(i, k, anItem);
    			break;

    		case QVariant::Date:
    			if (YerothUtils::isEqualCaseInsensitive(curTableModelRawHdr, YerothDatabaseTableColumn::DATE_ENTREE) &&
    					!itemHasExpired)
    			{
    				prevDate = curDate;
    				curDate = qv.toDate();

    				if (curDate < prevDate)
    				{
    					stockReferenceToStockID_in_out.insert(curStockReference, curStockID);
    				}
    				else
    				{
    					if (!stockReferenceToStockID_in_out.contains(curStockReference))
    					{
    						stockReferenceToStockID_in_out.insert(curStockReference, curStockID);
    					}
    				}
    			}

    			anItem = new YerothQStandardItem(DATE_TO_STRING(qv.toDate()));
    			_stdItemModel->setItem(i, k, anItem);
    			break;

    		case QVariant::Time:
    			anItem = new YerothQStandardItem(TIME_TO_STRING(qv.toTime()));
    			_stdItemModel->setItem(i, k, anItem);
    			break;

    		default:
    			anItem = new YerothQStandardItem(YerothUtils::EMPTY_STRING);
//    			qDebug() << "YerothTableView::lister_codebar_FIFO(): undecoded QVariant -> " << qv.type();
    			break;
    		}
    	}
    }

    //    qDebug() << "++ FIFO, codebarToTableRows_in_out: " << codebarToTableRows_in_out;
}


void YerothTableView::lister_codebar_LIFO(YerothSqlTableModel &tableModel,
        								  QMap<QString, QString> &stockReferenceToStockID_in_out)
{
    emit signal_lister(tableModel);

    QString aReferenceLIFOStringQuery =
    		YerothUtils::getStrategySqlQueryStr(tableModel.yerothSelectStatement(),
    											YerothERPConfig::STRATEGIE_VENTE_SORTIE_LIFO);

    bool s = tableModel.yerothSetQuery(aReferenceLIFOStringQuery);

    int rows = tableModel.rowCount();
    int columns = tableModel.columnCount();

    _stdItemModel->setRowCount(rows);
    _stdItemModel->setColumnCount(columns);

    QStringList	tableModelRawHeaders;

    YerothUtils::createTableModelHeaders(tableModel,
    									 *_stdItemModel,
										 *_tableModelHeaders,
										 tableModelRawHeaders);

    if (!s)
    {
    	return ;
    }

    QString curTableModelRawHdr;

    QStandardItem *anItem = 0;
    QVariant qv;

    QString prevStockReference;
    QString curStockReference;

    QDate defaultDate;
    defaultDate.setDate(QDate::currentDate().year() - 7, 12, 31);

    QDate curDate = defaultDate;
    QDate prevDate = defaultDate;

    QSqlRecord record;

    QVariant date_premption;
    QVariant quantite_totale;
    QVariant stock_dalerte;

    QString curStockID;

    bool itemHasExpired = false;

    for (int i = 0; i < rows; ++i)
    {
    	record = tableModel.record(i);

    	curStockID = record.value(YerothDatabaseTableColumn::ID).toString();

    	date_premption = record.value(YerothDatabaseTableColumn::DATE_PEREMPTION);
    	quantite_totale = record.value(YerothDatabaseTableColumn::QUANTITE_TOTALE);
    	stock_dalerte = record.value(YerothDatabaseTableColumn::STOCK_DALERTE);

    	itemHasExpired = (date_premption.toDate() < GET_CURRENT_DATE);

    	for (int k = 0; k < columns; ++k)
    	{
    		curTableModelRawHdr = tableModelRawHeaders.at(k);

    		qv.setValue(tableModel.record(i).value(k));

    		anItem = _stdItemModel->item(i, k);

    		if (anItem)
    		{
    			delete anItem;
    		}

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
    			if (YerothUtils::isEqualCaseInsensitive(curTableModelRawHdr, YerothDatabaseTableColumn::REFERENCE) &&
    					!itemHasExpired)
    			{
    				prevStockReference = curStockReference;
    				curStockReference = qv.toString();
    			}

    			anItem = new YerothQStandardItem(qv.toString());
    			_stdItemModel->setItem(i, k, anItem);
    			break;

    		case QVariant::Bool:
    			anItem = new YerothQStandardItem(qv.toBool() ? BOOLEAN_STRING_TRUE : BOOLEAN_STRING_FALSE);
    			_stdItemModel->setItem(i, k, anItem);
    			break;

    		case QVariant::Date:

    			if (YerothUtils::isEqualCaseInsensitive(curTableModelRawHdr, YerothDatabaseTableColumn::DATE_ENTREE) &&
    					!itemHasExpired)
    			{
    				prevDate = curDate;
    				curDate = qv.toDate();

    				if (curDate >= prevDate)
    				{
    					stockReferenceToStockID_in_out.insert(curStockReference, curStockID);
    				}
    				else
    				{
    					if (!stockReferenceToStockID_in_out.contains(curStockReference))
    					{
    						stockReferenceToStockID_in_out.insert(curStockReference, curStockID);
    					}
    				}
    			}

    			anItem = new YerothQStandardItem(DATE_TO_STRING(qv.toDate()));;
    			_stdItemModel->setItem(i, k, anItem);
    			break;

    		case QVariant::Time:
    			anItem = new YerothQStandardItem(TIME_TO_STRING(qv.toTime()));
    			_stdItemModel->setItem(i, k, anItem);
    			break;

    		default:
    			anItem = new YerothQStandardItem(YerothUtils::EMPTY_STRING);
//    			qDebug() << "YerothTableView::lister_codebar_LIFO(): undecoded QVariant -> " << qv.type();
    			break;
    		}
    	}
    }
    //    qDebug() << "++ LIFO, codebarToTableRows_in_out: " << codebarToTableRows_in_out;
}


void YerothTableView::lister_codebar_FEFO(YerothSqlTableModel &tableModel,
        									 QMap<QString, QString> &stockReferenceToStockID_in_out)
{
    emit signal_lister(tableModel);

    QString aReferenceDEFDEOQueryStr =
    		YerothUtils::getStrategySqlQueryStr(tableModel.yerothSelectStatement(),
    											YerothERPConfig::STRATEGIE_VENTE_SORTIE_FEFO);

    bool s = tableModel.yerothSetQuery(aReferenceDEFDEOQueryStr);

    int rows = tableModel.rowCount();
    int columns = tableModel.columnCount();

    _stdItemModel->setRowCount(rows);
    _stdItemModel->setColumnCount(columns);

    QStringList	tableModelRawHeaders;

    YerothUtils::createTableModelHeaders(tableModel,
    									 *_stdItemModel,
										 *_tableModelHeaders,
										 tableModelRawHeaders);

    if (!s)
    {
    	return ;
    }

    QString curTableModelRawHdr;

    QStandardItem *anItem = 0;
    QVariant qv;

    QString prevStockReference;
    QString curStockReference;

    QDate defaultDate;
    defaultDate.setDate(QDate::currentDate().year() + 1000, 12, 31);

    QDate curDate = defaultDate;
    QDate prevDate = defaultDate;

    QSqlRecord record;

    QVariant quantite_totale;
    QVariant stock_dalerte;
    QVariant date_premption;

    QString curStockID;

    bool itemIsPreempted = false;

    for (int i = 0; i < rows; ++i)
    {
    	record = tableModel.record(i);

    	curStockID = record.value(YerothDatabaseTableColumn::ID).toString();

    	quantite_totale = record.value(YerothDatabaseTableColumn::QUANTITE_TOTALE);
    	stock_dalerte = record.value(YerothDatabaseTableColumn::STOCK_DALERTE);
    	date_premption = record.value(YerothDatabaseTableColumn::DATE_PEREMPTION);

    	itemIsPreempted = (date_premption.toDate() < GET_CURRENT_DATE);

    	for (int k = 0; k < columns; ++k)
    	{
    		curTableModelRawHdr = tableModelRawHeaders.at(k);

    		qv.setValue(tableModel.record(i).value(k));

    		anItem = _stdItemModel->item(i, k);

    		if (anItem)
    		{
    			delete anItem;
    		}

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
    			if (YerothUtils::isEqualCaseInsensitive(curTableModelRawHdr, YerothDatabaseTableColumn::REFERENCE) &&
    					!itemIsPreempted)
    			{
    				prevStockReference = curStockReference;
    				curStockReference = qv.toString();
    			}

    			anItem = new YerothQStandardItem(qv.toString());
    			_stdItemModel->setItem(i, k, anItem);
    			break;

    		case QVariant::Bool:
    			anItem = new YerothQStandardItem(qv.toBool() ? BOOLEAN_STRING_TRUE : BOOLEAN_STRING_FALSE);
    			_stdItemModel->setItem(i, k, anItem);
    			break;

    		case QVariant::Date:
    			if (YerothUtils::isEqualCaseInsensitive(curTableModelRawHdr, YerothDatabaseTableColumn::DATE_PEREMPTION) &&
    					!itemIsPreempted)
    			{
    				prevDate = curDate;
    				curDate = qv.toDate();

    				if (curDate < prevDate)
    				{
    					stockReferenceToStockID_in_out.insert(curStockReference, curStockID);
    					//qDebug() << "++ 1-OK. curCodebar: "
    							//	 << curCodebar << " date: " << curDate;
    				}
    				else
    				{
    					if (!stockReferenceToStockID_in_out.contains(curStockReference))
    					{
    						stockReferenceToStockID_in_out.insert(curStockReference, curStockID);
    					}
    				}
    			}

    			anItem = new YerothQStandardItem(DATE_TO_STRING(qv.toDate()));
    			_stdItemModel->setItem(i, k, anItem);
    			break;

    		case QVariant::Time:
    			anItem = new YerothQStandardItem(TIME_TO_STRING(qv.toTime()));
    			_stdItemModel->setItem(i, k, anItem);
    			break;

    		default:
    			anItem = new YerothQStandardItem(YerothUtils::EMPTY_STRING);
//    			qDebug() << "YerothTableView::lister_codebar_FEFO(): undecoded QVariant -> " << qv.type();
    			break;
    		}
    	} //switch-case
    }
//    qDebug() << "++ codebarToTableRows_in_out: " << codebarToTableRows_in_out;
}

