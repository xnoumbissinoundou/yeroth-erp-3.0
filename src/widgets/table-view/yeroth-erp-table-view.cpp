/*
 * yeroth-erp-table-view-model.cpp
 *
 *      Author: Xavier NOUMBISSI NOUNDOU, Dipl.-Inf., Ph.D. (ABD)
 */


#include "yeroth-erp-table-view.hpp"

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


const int YerothTableView::REFERENCE_COLUMN = 1;

const int YerothTableView::DATE_ENTREE_COLUMN = 12;

const int YerothTableView::DATE_PREEMPTION_COLUMN = 13;

const int YerothTableView::DATE_VENTE_COLUMN = 4;

const int YerothTableView::DESIGNATION_COLUMN = 2;

const int YerothTableView::QUANTITE_TOTAL_COLUMN = 8;



YerothTableView::YerothTableView()
    :QTableView()
{
    _writeEnabled = false;
    _lastSelectedRow = 0;
    _tableName = 0;

    _tableModelHeaders = new QStringList;
    _stdItemModel = new YerothPOSQStandardItemModel;

    setModel(_stdItemModel);
    setupSelectionOptions();
    setBackgroundRole(QPalette::Window);
}

YerothTableView::YerothTableView(QWidget * parent)
    :QTableView(parent)
{
    _writeEnabled = false;
    _lastSelectedRow = 0;
    _tableName = 0;

    _tableModelHeaders = new QStringList;
    _stdItemModel = new YerothPOSQStandardItemModel;

    setModel(_stdItemModel);
    setupSelectionOptions();
    setBackgroundRole(QPalette::Window);
}

YerothTableView::~YerothTableView()
{
    delete _stdItemModel;
    delete _tableModelHeaders;
}

void YerothTableView::setupSelectionOptions()
{
    setFrameShadow(QFrame::Plain);
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::SingleSelection);
}

void YerothTableView::selectionChanged (const QItemSelection & selected,
                                       const QItemSelection & deselected)
{
    QModelIndexList selectedIndexes = selected.indexes();
    if (selectedIndexes.size() > 0)
    {
        _lastSelectedRow = selectedIndexes.at(0).row();
        //qDebug() << "selectRow: " << _lastSelectedRowForSell << "\n";
    }

    /*
     * Cet appel de la fonction 'clearFocus' est necessaire pour
     * que les nouveaux elements du tableau soit visible immediatement.
     */
    this->clearFocus();
}

/**
 * TODO: update this code to the new architecture.
 */
void YerothTableView::dataChanged(const QModelIndex &index,
                                  const QModelIndex &bottomRight,
								  const QVector<int> &roles /*= QVector<int>()*/)
{
    if (index != bottomRight) return;
    if (_writeEnabled)
    {
        //qDebug() << "dataChanged(). Updates table " << *_tableName;
        QString msg;
        QString strQuery("UPDATE ");
        strQuery.append(*_tableName)
        .append(" SET ")
        .append(_tableModelHeaders->at(index.column()))
        .append(" = ?")
        .append(" WHERE id = ")
        .append( _stdItemModel->item(index.row(), 0)->text() );

        QSqlQuery query(strQuery);

        QVariant q = index.data();
        switch (q.type())
        {
        case QVariant::UInt:
            msg.clear();
            msg.setNum(q.toUInt());
            break;

        case QVariant::Int:
            msg.clear();
            msg.setNum(q.toInt());
            break;

        case QVariant::Double:
            msg.clear();
            msg.setNum(q.toDouble());
            break;

        case QVariant::ULongLong:
            msg.clear();
            msg.setNum(q.toULongLong());
            break;

        case QVariant::LongLong:
            msg.clear();
            msg.setNum(q.toLongLong());
            break;

        case QVariant::Char:
            msg.append(q.toChar());
            break;

        case QVariant::String:
            msg.append(q.toString());
            break;

        case QVariant::Bool:
            msg.append(q.toBool());
            break;

        default:
            break;
        }

        query.bindValue(0, msg);
        query.exec();
        //qDebug() << "[" << success << "]" << query.executedQuery();
    }
}


void YerothTableView::lister_les_transactions_dun_client(QSqlQuery &sqlClientTransactionsUnionQuery)
{
    QString companyNameHdr(QObject::tr("Entreprise"));
    QString dateHdr(QObject::tr("Date"));
    QString timeHdr(QObject::tr("Heure"));
    QString transactionAmountHdr(QObject::tr("Montant total"));
    QString customerAccountValueAfterHdr(QObject::trUtf8("Compte client (après)"));
    QString paymentModeHdr(QObject::tr("Type de paiement"));
    QString reasonHdr(QObject::trUtf8("Justification"));

    _tableModelHeaders->clear();

    _tableModelHeaders->append(companyNameHdr);
    _tableModelHeaders->append(dateHdr);
    _tableModelHeaders->append(timeHdr);
    _tableModelHeaders->append(transactionAmountHdr);
    _tableModelHeaders->append(customerAccountValueAfterHdr);
    _tableModelHeaders->append(paymentModeHdr);
    _tableModelHeaders->append(reasonHdr);


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

	if (querySize > 0)
	{
		QString stringKeyValue;

		for (int i = 0; i < querySize; ++i)
		{
			if (sqlClientTransactionsUnionQuery.next())
			{
				QVariant qv;

				for (int j = 0; j < _stdItemModel->columnCount(); ++j)
				{
					qv = sqlClientTransactionsUnionQuery.value(j);

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
							aYerothQStandardItem = new YerothQStandardItem("N/A");
						}
						else
						{
							aYerothQStandardItem = new YerothQStandardItem(GET_DOUBLE_STRING(qv.toDouble()));
						}

						_stdItemModel->setItem(i, j, aYerothQStandardItem);
						break;

					case QVariant::String:
						if (5 == j)
						{
							stringKeyValue = qv.toString();

							int keyValue = stringKeyValue.toInt();

							if (0 < keyValue)
							{
								stringKeyValue = YerothUtils::_typedepaiementToUserViewString.value(keyValue);
							}

							aYerothQStandardItem = new YerothQStandardItem(YerothUtils::YEROTH_TRUNCATE_STRING_ACCORDING_TO_SETTING(stringKeyValue));
							_stdItemModel->setItem(i, j, aYerothQStandardItem);
						}
						else
						{
							aYerothQStandardItem = new YerothQStandardItem(YerothUtils::YEROTH_TRUNCATE_STRING_ACCORDING_TO_SETTING(qv.toString()));
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
}


void YerothTableView::lister_lhistorique_du_stock(const QStringList &aMouvementStockList)
{
	//qDebug() << QString("YerothTableView::lister_lhistorique_du_stock(const QStringList &)");

    QString dateHdr(QObject::tr("Date"));
    QString timeHdr(QObject::tr("Heure"));
    QString idHdr(QObject::tr("ID du stock"));
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

    _stdItemModel->setColumnCount(7);

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
				if (QObject::tr("RETOUR VENTE") == curTypeMouvementStock ||
					QObject::tr("ENTRÉE") == curTypeMouvementStock)
				{
					//5 is corresponds to 'RETOUR'.
					strOut = YerothUtils::YEROTH_TRUNCATE_STRING_ACCORDING_TO_SETTING("(+) " + anEntry.at(j));
				}
				else
				{
					strOut = YerothUtils::YEROTH_TRUNCATE_STRING_ACCORDING_TO_SETTING("(-) " + anEntry.at(j));
				}

				anItem = new YerothQStandardItem(strOut);
			}
			else
			{
				strOut = YerothUtils::YEROTH_TRUNCATE_STRING_ACCORDING_TO_SETTING(anEntry.at(j));
				anItem = new YerothQStandardItem(strOut);
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

    YerothUtils::createTableModelHeaders(tableModel, *_stdItemModel, *_tableModelHeaders);

    QString tmpQvString;
    QStandardItem *anItem = 0;
    QVariant qv;

    if(s)
    {
        for (int i = 0; i < rows; ++i)
        {
            for (int k = 0; k < columns; ++k)
            {
                qv.setValue(tableModel.record(i).value(k));

                anItem = _stdItemModel->item(i, k);

                if (anItem)
                {
                    delete anItem;
                }

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
                	tmpQvString.clear();
                	tmpQvString.append(qv.toString());
                	if (YerothTableView::REFERENCE_COLUMN != k)
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
                    //qDebug() << "YerothTableView:::lister(): undecoded QVariant -> " << qv.type();
                    break;
                }
            }
        }

        this->resizeColumnsToContents();
    }
}


void YerothTableView::lister_ALL(YerothSqlTableModel &tableModel,
                                QMap<QString, int> &designationToTableRows_in_out)
{
    emit signal_lister(tableModel);

    QString anALLQueryStr(QString("%1 ORDER BY %2 ASC")
    							.arg(tableModel.yerothSelectStatement(),
									 YerothDatabaseTableColumn::ID));

    bool s = tableModel.yerothSetQuery(anALLQueryStr);

    int rows = tableModel.rowCount();
    int columns = tableModel.columnCount();

    _stdItemModel->setRowCount(rows);
    _stdItemModel->setColumnCount(columns);

    YerothUtils::createTableModelHeaders(tableModel, *_stdItemModel, *_tableModelHeaders);

	QString tmpQvString;
    QStandardItem *anItem = 0;
    QVariant qv;

    QString curDesignation;

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
                    if (YerothTableView::DESIGNATION_COLUMN == k)
                    {
                        curDesignation = qv.toString();
                        if (!designationToTableRows_in_out.contains(curDesignation))
                        {
                            designationToTableRows_in_out.insert(curDesignation, i);
                        }
                    }
                	tmpQvString.clear();
                	tmpQvString.append(qv.toString());
                	if (YerothTableView::REFERENCE_COLUMN != k)
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
                    anItem = new YerothQStandardItem(qv.toBool() ? "True" : "False");
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
                    //qDebug() << "YerothTableView::lister(): undecoded QVariant -> " << qv.type();
                    break;
                }

            }//for columns
        }//for rows

        this->resizeColumnsToContents();
    }// if
}

void YerothTableView::lister_codebar_ALL(YerothSqlTableModel &tableModel,
                                        QMap<QString, int> &codebarToTableRows_in_out)
{
    codebarToTableRows_in_out.clear();

    emit signal_lister(tableModel);

    QString aReferenceALLQueryStr(QString("%1 ORDER BY %2 ASC")
    							.arg(tableModel.yerothSelectStatement(),
									 YerothDatabaseTableColumn::ID));

    bool s = tableModel.yerothSetQuery(aReferenceALLQueryStr);

    int rows = tableModel.rowCount();
    int columns = tableModel.columnCount();

    _stdItemModel->setRowCount(rows);
    _stdItemModel->setColumnCount(columns);

    YerothUtils::createTableModelHeaders(tableModel, *_stdItemModel, *_tableModelHeaders);

    QStandardItem *anItem = 0;
    QVariant qv;

    QString curCodebar;

    if(s)
    {
        for (int i = 0; i < rows; ++i)
        {
            for (int k = 0; k < columns; ++k)
            {
                qv.setValue(tableModel.record(i).value(k));

                anItem = _stdItemModel->item(i, k);

                if (anItem)
                {
                    delete anItem;
                }

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
                    if (YerothTableView::REFERENCE_COLUMN == k)
                    {
                        curCodebar = qv.toString();
                        if (!codebarToTableRows_in_out.contains(curCodebar))
                        {
                            codebarToTableRows_in_out.insert(curCodebar, i);
                            anItem = new YerothQStandardItem(qv.toString());
                            _stdItemModel->setItem(i, k, anItem);
                        }
                    }

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
                    //qDebug() << "YerothTableView::lister(): undecoded QVariant -> " << qv.type();
                    break;
                }
            }
        }

        this->resizeColumnsToContents();
    }
}

void YerothTableView::lister_FIFO(YerothSqlTableModel &tableModel,
                                 QMap<QString, int> &designationToTableRows_in_out)
{
    emit signal_lister(tableModel);

    QString aFIFOQueryStr(QString("%1 ORDER BY %2 ASC, %3 ASC")
    							.arg(tableModel.yerothSelectStatement(),
									 YerothDatabaseTableColumn::DESIGNATION,
									 YerothDatabaseTableColumn::DATE_ENTREE));

    bool s = tableModel.yerothSetQuery(aFIFOQueryStr);

    int rows = tableModel.rowCount();
    int columns = tableModel.columnCount();

    _stdItemModel->setRowCount(rows);
    _stdItemModel->setColumnCount(columns);

    YerothUtils::createTableModelHeaders(tableModel, *_stdItemModel, *_tableModelHeaders);

	QString tmpQvString;
    QStandardItem *anItem = 0;
    QVariant qv;

    QMap<QString, QDate> designationToDateEntree;

    QString prevDesignation;
    QString curDesignation;

    QDate defaultDate;
    defaultDate.setDate(QDate::currentDate().year() + 1000, 12, 31);

    QDate curDate = defaultDate;
    QDate prevDate = defaultDate;

    bool testDateEntreeOK = false;

    if(s)
    {
        QSqlRecord record;
        QVariant date_premption;
        QVariant quantite_total;
        QVariant stock_minimum;

        for (int i = 0; i < rows; ++i)
        {
            record = tableModel.record(i);
            date_premption = record.value(YerothDatabaseTableColumn::DATE_PEREMPTION);
            quantite_total = record.value(YerothDatabaseTableColumn::QUANTITE_TOTAL);
            stock_minimum = record.value(YerothDatabaseTableColumn::STOCK_MINIMUM);

            for (int k = 0; k < columns; ++k)
            {
                qv.setValue(tableModel.record(i).value(k));

                anItem = _stdItemModel->item(i, k);

                if (anItem)
                {
                    delete anItem;
                }

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

                	if (YerothTableView::DESIGNATION_COLUMN == k)
                    {
                        prevDesignation = curDesignation;
                        curDesignation = qv.toString();
                    }

                    tmpQvString.clear();
                	tmpQvString.append(qv.toString());

                	if (YerothTableView::REFERENCE_COLUMN != k)
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
                    anItem = new YerothQStandardItem(qv.toBool() ? "True" : "False");
                    _stdItemModel->setItem(i, k, anItem);
                    break;

                case QVariant::Date:

                    if (YerothTableView::DATE_ENTREE_COLUMN == k)
                    {
                        prevDate = curDate;
                        curDate = qv.toDate();

                        if (curDate < prevDate)
                        {
                            testDateEntreeOK = true;
                            designationToDateEntree.insert(curDesignation, curDate);
                            designationToTableRows_in_out.insert(curDesignation, i);
                            //qDebug() << QString("++ FIFO. 1) curDesignation: %1, curDate: %2")
                            //			.arg(curDesignation, DATE_TO_STRING(designationToDateEntree[curDesignation]));
                        }
                        else
                        {
                            if (!designationToDateEntree.contains(curDesignation))
                            {
                                testDateEntreeOK = true;
                                designationToDateEntree.insert(curDesignation, curDate);
                                designationToTableRows_in_out.insert(curDesignation, i);
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
                    //qDebug() << "YerothTableView::lister(): undecoded QVariant -> " << qv.type();
                    break;
                }

                if (anItem)
                {
                    anItem->setForeground(Qt::white);

                    anItem->setTextAlignment(Qt::AlignCenter);

                    if (testDateEntreeOK &&
                            YerothTableView::DATE_ENTREE_COLUMN == k)
                    {
                        anItem->setForeground(YerothUtils::YEROTH_GREEN_COLOR);
                        testDateEntreeOK = false;
                    }

                    double quantite_minimale = stock_minimum.toDouble() - 1;

                    if (YerothTableView::QUANTITE_TOTAL_COLUMN ==k &&
                            quantite_total.toDouble() <= quantite_minimale)
                    {
                        anItem->setForeground(YerothUtils::YEROTH_RED_COLOR);
                    }

                    if (YerothTableView::DATE_PREEMPTION_COLUMN == k)
                    {
                        if (date_premption.toDate() <= GET_CURRENT_DATE)

                        {
                            anItem->setForeground(YerothUtils::YEROTH_RED_COLOR);
                        }
                    }
                }
            }
        }

        this->resizeColumnsToContents();
    }
}

void YerothTableView::lister_codebar_FIFO(YerothSqlTableModel &tableModel,
        								  QMap<QString, int> &codebarToTableRows_in_out)
{
    emit signal_lister(tableModel);

    QString aReferenceFIFOQueryStr(QString("%1 ORDER BY %2 ASC, %3 ASC")
    							.arg(tableModel.yerothSelectStatement(),
									 YerothDatabaseTableColumn::DESIGNATION,
									 YerothDatabaseTableColumn::DATE_ENTREE));

    bool s = tableModel.yerothSetQuery(aReferenceFIFOQueryStr);

    int rows = tableModel.rowCount();
    int columns = tableModel.columnCount();

    _stdItemModel->setRowCount(rows);
    _stdItemModel->setColumnCount(columns);

    YerothUtils::createTableModelHeaders(tableModel, *_stdItemModel, *_tableModelHeaders);

    QStandardItem *anItem = 0;
    QVariant qv;

    QString prevCodebar;
    QString curCodebar;

    QDate defaultDate;
    defaultDate.setDate(QDate::currentDate().year() + 1000, 12, 31);

    QDate curDate = defaultDate;
    QDate prevDate = defaultDate;

    if(s)
    {
        for (int i = 0; i < rows; ++i)
        {
            for (int k = 0; k < columns; ++k)
            {
                qv.setValue(tableModel.record(i).value(k));

                anItem = _stdItemModel->item(i, k);

                if (anItem)
                {
                    delete anItem;
                }

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
                    if (YerothTableView::REFERENCE_COLUMN == k)
                    {
                        prevCodebar = curCodebar;
                        curCodebar = qv.toString();
                    }
                    anItem = new YerothQStandardItem(qv.toString());
                    _stdItemModel->setItem(i, k, anItem);
                    break;

                case QVariant::Bool:
                    anItem = new YerothQStandardItem(qv.toBool() ? "True" : "False");
                    _stdItemModel->setItem(i, k, anItem);
                    break;

                case QVariant::Date:
                    if (YerothTableView::DATE_ENTREE_COLUMN == k)
                    {
                        prevDate = curDate;
                        curDate = qv.toDate();

                        if (curDate < prevDate)
                        {
                            codebarToTableRows_in_out.insert(curCodebar, i);
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
                    //qDebug() << "YerothTableView::lister(): undecoded QVariant -> " << qv.type();
                    break;
                }
            }
        }

        this->resizeColumnsToContents();
    }
}

QDate YerothTableView::getEarliestDate(QMultiMap<QString, QDate> allElements,
                                      QString aDesignation)
{
    QDate defaultDate;
    defaultDate.setDate(QDate::currentDate().year() + 1000, 12, 31);

    QDate earliestDate = defaultDate;

    if (allElements.count(aDesignation) > 0)
    {
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
    }

    return earliestDate;
}

QDate YerothTableView::getLatestDate(QMultiMap<QString, QDate> allElements,
                                    QString aDesignation)
{
    QDate defaultDate;
    defaultDate.setDate(QDate::currentDate().year() - 7, 12, 31);

    QDate latestDate = defaultDate;

    //qDebug() << "++ check date: " << latestDate;

    //qDebug() << QString("++ YerothTableView::getLatestDate. aDesignation: %1")
    //				.arg(aDesignation);

    //qDebug() << "++ YerothTableView::getLatestDate. allElements: "
    //		 << allElements;

    if (allElements.count(aDesignation) > 0)
    {
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
    }

    return latestDate;
}

void YerothTableView::lister_LIFO(YerothSqlTableModel &tableModel,
                                 QMap<QString, int> &designationToTableRows_in_out)
{
    emit signal_lister(tableModel);

    QString aLIFOStringQuery(QString("%1 ORDER BY %2 ASC, %3 DESC")
    							.arg(tableModel.yerothSelectStatement(),
									 YerothDatabaseTableColumn::DESIGNATION,
									 YerothDatabaseTableColumn::DATE_ENTREE));

    bool s = tableModel.yerothSetQuery(aLIFOStringQuery);

    int rows = tableModel.rowCount();
    int columns = tableModel.columnCount();

    _stdItemModel->setRowCount(rows);
    _stdItemModel->setColumnCount(columns);

    YerothUtils::createTableModelHeaders(tableModel, *_stdItemModel, *_tableModelHeaders);

    QMap<QString, QStandardItem *> designationTopreviousLIFOGreenItems;

    QStandardItem *aPreviousLIFOGreenItem = 0;
    QStandardItem *anItem = 0;
    QVariant qv;

    QMap<QString, QDate> designationToDateEntree;

    QString tmpQvString;
    QString prevDesignation;
    QString curDesignation;

    QDate defaultDate;
    defaultDate.setDate(QDate::currentDate().year() - 7, 12, 31);

    QDate curDate = defaultDate;
    QDate prevDate = defaultDate;

    bool testDateEntreeOK = false;

    if(s)
    {
        QSqlRecord record;

        QVariant date_premption;
        QVariant quantite_total;
        QVariant stock_minimum;

        QMultiMap<QString, QDate> allElements;

        for (int i = 0; i < rows; ++i)
        {
            record = tableModel.record(i);

            date_premption = record.value(YerothDatabaseTableColumn::DATE_PEREMPTION);
            quantite_total = record.value(YerothDatabaseTableColumn::QUANTITE_TOTAL);
            stock_minimum = record.value(YerothDatabaseTableColumn::STOCK_MINIMUM);

            for (int k = 0; k < columns; ++k)
            {
                qv.setValue(tableModel.record(i).value(k));

                anItem = _stdItemModel->item(i, k);

                if (anItem)
                {
                    delete anItem;
                }

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
                    if (YerothTableView::DESIGNATION_COLUMN == k)
                    {
                        prevDesignation = curDesignation;
                        curDesignation = qv.toString();
                    }

                	tmpQvString.clear();
                	tmpQvString.append(qv.toString());
                	if (YerothTableView::REFERENCE_COLUMN != k)
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
                    anItem = new YerothQStandardItem(qv.toBool() ? "True" : "False");
                    _stdItemModel->setItem(i, k, anItem);
                    break;

                case QVariant::Date:

                    if (YerothTableView::DATE_ENTREE_COLUMN == k)
                    {
                        prevDate = curDate;
                        curDate = qv.toDate();

                        if (curDate >= prevDate)
                        {
                            testDateEntreeOK = true;
                            designationToDateEntree.insert(curDesignation, curDate);
                            designationToTableRows_in_out.insert(curDesignation, i);
                            allElements.insert(curDesignation, curDate);
//                            qDebug() << QString("++ LIFO. 1) curDesignation: %1, curDate: %2")
//                            			.arg(curDesignation, DATE_TO_STRING(designationToDateEntree[curDesignation]));
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
                    //qDebug() << "YerothTableView::lister(): undecoded QVariant -> " << qv.type();
                    break;
                }

                if (anItem)
                {
                	anItem->setForeground(Qt::white);

                	if (testDateEntreeOK && YerothTableView::DATE_ENTREE_COLUMN == k)
                	{
                		QDate latestDate = getLatestDate(allElements, curDesignation);

//                		qDebug() << QString("++ 1, LATEST: designation %1, latestDate: %2")
//                            			.arg(curDesignation, DATE_TO_STRING(latestDate));

                		QStandardItem *itemDateOfStockEntry;
                		QString aDesignation;
                		QDate itemEntryDate;

                		for(int h = 0; h < i; ++h)
                		{
                			itemDateOfStockEntry = _stdItemModel->item(h, YerothTableView::DATE_ENTREE_COLUMN);

                			aDesignation = _stdItemModel->item(h, YerothTableView::DESIGNATION_COLUMN)->text();

                			//                                qDebug() << QString("++ designation: %1, itemEntryDate: %2")
                			//                                				.arg(aDesignation, itemDateOfStockEntry->text());

                			if (0 != itemDateOfStockEntry &&
                					YerothUtils::isEqualCaseInsensitive(aDesignation, curDesignation))
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

                		if (latestDate == designationToDateEntree.value(curDesignation))
                		{
//                			qDebug() << QString("++ YES, designation %1, latestDate: %2")
//                                						.arg(curDesignation, DATE_TO_STRING(latestDate));

                			aPreviousLIFOGreenItem = designationTopreviousLIFOGreenItems.value(curDesignation);

                			if (0 != aPreviousLIFOGreenItem)
                			{
                				aPreviousLIFOGreenItem->setForeground(Qt::white);
                			}

                			anItem->setForeground(YerothUtils::YEROTH_GREEN_COLOR);

                			aPreviousLIFOGreenItem = anItem;

                			designationTopreviousLIFOGreenItems.insert(curDesignation, aPreviousLIFOGreenItem);
                		}

                		testDateEntreeOK = false;
                	}

                    double quantite_minimale = stock_minimum.toDouble() - 1;

                    if (YerothTableView::QUANTITE_TOTAL_COLUMN == k 		&&
                            quantite_total.toDouble() <= quantite_minimale)
                    {
                        anItem->setForeground(YerothUtils::YEROTH_RED_COLOR);
                    }
                }
            }
        }
    }

    this->resizeColumnsToContents();
}

void YerothTableView::lister_codebar_LIFO(YerothSqlTableModel &tableModel,
        								  QMap<QString, int> &codebarToTableRows_in_out)
{
    emit signal_lister(tableModel);

    QString aReferenceLIFOStringQuery(QString("%1 ORDER BY %2 ASC, %3 DESC")
    							.arg(tableModel.yerothSelectStatement(),
									 YerothDatabaseTableColumn::DESIGNATION,
									 YerothDatabaseTableColumn::DATE_ENTREE));

    bool s = tableModel.yerothSetQuery(aReferenceLIFOStringQuery);

    int rows = tableModel.rowCount();
    int columns = tableModel.columnCount();

    _stdItemModel->setRowCount(rows);
    _stdItemModel->setColumnCount(columns);

    YerothUtils::createTableModelHeaders(tableModel, *_stdItemModel, *_tableModelHeaders);

    QStandardItem *anItem = 0;
    QVariant qv;

    QString prevReference;
    QString curReference;

    QDate defaultDate;
    defaultDate.setDate(QDate::currentDate().year() - 7, 12, 31);

    QDate curDate = defaultDate;
    QDate prevDate = defaultDate;

    if(s)
    {
        QSqlRecord record;

        QVariant date_premption;
        QVariant quantite_total;
        QVariant stock_minimum;

        for (int i = 0; i < rows; ++i)
        {
            record = tableModel.record(i);

            date_premption = record.value(YerothDatabaseTableColumn::DATE_PEREMPTION);
            quantite_total = record.value(YerothDatabaseTableColumn::QUANTITE_TOTAL);
            stock_minimum = record.value(YerothDatabaseTableColumn::STOCK_MINIMUM);

            for (int k = 0; k < columns; ++k)
            {
                qv.setValue(tableModel.record(i).value(k));

                anItem = _stdItemModel->item(i, k);

                if (anItem)
                {
                    delete anItem;
                }

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
                    if (YerothTableView::REFERENCE_COLUMN == k)
                    {
                        prevReference = curReference;
                        curReference = qv.toString();
                    }

                    anItem = new YerothQStandardItem(qv.toString());
                    _stdItemModel->setItem(i, k, anItem);
                    break;

                case QVariant::Bool:
                    anItem = new YerothQStandardItem(qv.toBool() ? "True" : "False");
                    _stdItemModel->setItem(i, k, anItem);
                    break;

                case QVariant::Date:

                    if (YerothTableView::DATE_ENTREE_COLUMN == k)
                    {
                        prevDate = curDate;
                        curDate = qv.toDate();

                        if (curDate >= prevDate)
                        {
                            codebarToTableRows_in_out.insert(curReference, i);
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
                    //qDebug() << "YerothTableView::lister(): undecoded QVariant -> " << qv.type();
                    break;
                }
            }
        }
    }

    this->resizeColumnsToContents();
}

void YerothTableView::lister_DEF_DEO(YerothSqlTableModel &tableModel,
                                    QMap<QString, int> &designationToTableRows_in_out)
{
    emit signal_lister(tableModel);

    QString aDEFDEOQueryStr(QString("%1 ORDER BY %2 ASC, %3 ASC")
    							.arg(tableModel.yerothSelectStatement(),
									 YerothDatabaseTableColumn::DESIGNATION,
									 YerothDatabaseTableColumn::DATE_PEREMPTION));

    bool s = tableModel.yerothSetQuery(aDEFDEOQueryStr);

    int rows = tableModel.rowCount();
    int columns = tableModel.columnCount();

    _stdItemModel->setRowCount(rows);
    _stdItemModel->setColumnCount(columns);

    YerothUtils::createTableModelHeaders(tableModel, *_stdItemModel, *_tableModelHeaders);

    QStandardItem *anItem = 0;
    //QStandardItem *prevItem = 0;
    QVariant qv;

    QMap<QString, QDate> designationToDatePreemption;
    QMultiMap<QString, QDate> allElements;

    QString tmpQvString;
    QString prevDesignation;
    QString curDesignation;

    QDate defaultDate;
    defaultDate.setDate(QDate::currentDate().year() + 1000, 12, 31);

    QDate curDate = defaultDate;
    QDate prevDate = defaultDate;

    bool testDatePreemptionOK = false;

    if(s)
    {
        QSqlRecord record;
        QVariant date_premption;
        QVariant quantite_total;
        QVariant stock_minimum;
        bool itemIsPreempted = false;

        for (int i = 0; i < rows; ++i)
        {
            record = tableModel.record(i);
            date_premption = record.value(YerothDatabaseTableColumn::DATE_PEREMPTION);
            quantite_total = record.value(YerothDatabaseTableColumn::QUANTITE_TOTAL);
            stock_minimum = record.value(YerothDatabaseTableColumn::STOCK_MINIMUM);
            itemIsPreempted = (date_premption.toDate() < GET_CURRENT_DATE);

            for (int k = 0; k < columns; ++k)
            {
                qv.setValue(tableModel.record(i).value(k));

                anItem = _stdItemModel->item(i, k);

                if (anItem)
                {
                    delete anItem;
                }

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
                    if (YerothTableView::DESIGNATION_COLUMN == k && !itemIsPreempted)
                    {
                        prevDesignation = curDesignation;
                        curDesignation = qv.toString();
                    }
                	tmpQvString.clear();
                	tmpQvString.append(qv.toString());
                	if (YerothTableView::REFERENCE_COLUMN != k)
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
                    anItem = new YerothQStandardItem(qv.toBool() ? "True" : "False");
                    _stdItemModel->setItem(i, k, anItem);
                    break;

                case QVariant::Date:

                    if (YerothTableView::DATE_PREEMPTION_COLUMN == k && !itemIsPreempted)
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
                            if (!designationToDatePreemption.contains(curDesignation))
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
                    //qDebug() << "YerothTableView::lister(): undecoded QVariant -> " << qv.type();
                    break;
                }

                if (anItem)
                {
                    anItem->setForeground(Qt::white);
                    //prevItem = anItem;

                    double quantite_minimale = stock_minimum.toDouble() - 1;

                    if (YerothTableView::QUANTITE_TOTAL_COLUMN == k &&
                            quantite_total.toDouble() <= quantite_minimale)
                    {
                        anItem->setForeground(YerothUtils::YEROTH_RED_COLOR);
                    }
                    else if (YerothTableView::DATE_PREEMPTION_COLUMN == k)
                    {
                        if (itemIsPreempted)
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
                                itemDesignation = _stdItemModel->item(h, YerothTableView::DESIGNATION_COLUMN);
                                if (itemDesignation)
                                {
                                    aDesignation = itemDesignation->text();
                                    aDate = designationToDatePreemption.value(aDesignation);
                                    //qDebug() << "++ itemDesignation: " << aDesignation;

                                    if (YerothUtils::isEqualCaseInsensitive(aDesignation, curDesignation))
                                    {
                                        item = _stdItemModel->item(h, YerothTableView::DATE_PREEMPTION_COLUMN);
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

                            designationToDatePreemption.insert(curDesignation, earliestDate);
                            designationToTableRows_in_out.insert(curDesignation, i);

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

        this->resizeColumnsToContents();
    }
}

void YerothTableView::lister_codebar_DEF_DEO(YerothSqlTableModel &tableModel,
        QMap<QString, int> &codebarToTableRows_in_out)
{
    emit signal_lister(tableModel);

    QString aReferenceDEFDEOQueryStr(QString("%1 ORDER BY %2 ASC, %3 ASC")
    							.arg(tableModel.yerothSelectStatement(),
									 YerothDatabaseTableColumn::DESIGNATION,
									 YerothDatabaseTableColumn::DATE_PEREMPTION));

    bool s = tableModel.yerothSetQuery(aReferenceDEFDEOQueryStr);

    int rows = tableModel.rowCount();
    int columns = tableModel.columnCount();

    _stdItemModel->setRowCount(rows);
    _stdItemModel->setColumnCount(columns);

    YerothUtils::createTableModelHeaders(tableModel, *_stdItemModel, *_tableModelHeaders);

    QStandardItem *anItem = 0;
    QVariant qv;

    QString prevCodebar;
    QString curCodebar;

    QDate defaultDate;
    defaultDate.setDate(QDate::currentDate().year() + 1000, 12, 31);

    QDate curDate = defaultDate;
    QDate prevDate = defaultDate;

    if(s)
    {
        QSqlRecord record;
        QVariant quantite_total;
        QVariant stock_minimum;
        QVariant date_premption;
        bool itemIsPreempted = false;

        for (int i = 0; i < rows; ++i)
        {
            quantite_total = record.value(YerothDatabaseTableColumn::QUANTITE_TOTAL);
            stock_minimum = record.value(YerothDatabaseTableColumn::STOCK_MINIMUM);
            record = tableModel.record(i);
            date_premption = record.value(YerothDatabaseTableColumn::DATE_PEREMPTION);
            itemIsPreempted = (date_premption.toDate() < GET_CURRENT_DATE);

            for (int k = 0; k < columns; ++k)
            {
                qv.setValue(tableModel.record(i).value(k));

                anItem = _stdItemModel->item(i, k);

                if (anItem)
                {
                    delete anItem;
                }

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
                    if (YerothTableView::REFERENCE_COLUMN == k && !itemIsPreempted)
                    {
                        prevCodebar = curCodebar;
                        curCodebar = qv.toString();
                    }

                    anItem = new YerothQStandardItem(qv.toString());
                    _stdItemModel->setItem(i, k, anItem);
                    break;

                case QVariant::Bool:
                    anItem = new YerothQStandardItem(qv.toBool() ? "True" : "False");
                    _stdItemModel->setItem(i, k, anItem);
                    break;

                case QVariant::Date:
                    if (YerothTableView::DATE_PREEMPTION_COLUMN == k && !itemIsPreempted)
                    {
                        prevDate = curDate;
                        curDate = qv.toDate();

                        if (curDate < prevDate)
                        {
                            codebarToTableRows_in_out.insert(curCodebar, i);
                            //qDebug() << "++ 1-OK. curCodebar: "
                            //	 << curCodebar << " date: " << curDate;
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
                    //qDebug() << "YerothTableView::lister(): undecoded QVariant -> " << qv.type();
                    break;
                }
            } //switch-case
        }

        this->resizeColumnsToContents();
    }
}

