/*
 * yeroth-erp-table-view-model.cpp
 *
 *      Author: Dipl.-Inf. Xavier NOUMBISSI NOUNDOU, Ph.D. (ABD)
 *      Email:  xnoundou7@gmail.com
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


void YerothTableView::lister_lhistorique_du_stock(const QStringList &aMouvementStockList)
{
	//qDebug() << QString("YerothTableView::lister_lhistorique_du_stock(const QStringList &)");

    QString dateHdr(QObject::tr("Date"));
    QString timeHdr(QObject::tr("Temps"));
    QString idHdr(QObject::tr("ID du stock"));
    QString operationHdr(QObject::trUtf8("Type d'opération"));
    QString qteInitialeHdr(QObject::trUtf8("Qté initiale en stock"));
    QString qteRetireeHdr(QObject::trUtf8("Qté retirée"));
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

    QStringList anEntry;

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

			if (j == 2)
			{
				anItem = new YerothQStandardItem(
						YerothHistoriqueStock::get_type_mouvement_stock_string(anEntry.at(j)));
			}
			else
			{
				anItem = new YerothQStandardItem(anEntry.at(j));
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
                		if (true == truncateString && tmpQvString.length() > YerothUtils::STRING_MAX_CHARS)
                		{
                			tmpQvString.truncate(YerothUtils::STRING_MAX_CHARS);
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
    tableModel.yerothSetSort(YerothTableView::DESIGNATION_COLUMN, Qt::AscendingOrder);

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
                		if (tmpQvString.length() > YerothUtils::STRING_MAX_CHARS)
                		{
                			tmpQvString.truncate(YerothUtils::STRING_MAX_CHARS);
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

    tableModel.yerothSetSort(DESIGNATION_COLUMN, Qt::AscendingOrder);

    emit signal_lister(tableModel);

    bool s = tableModel.select();

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
    tableModel.yerothSetSort(DESIGNATION_COLUMN, Qt::AscendingOrder);

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
            date_premption = record.value("date_peremption");
            quantite_total = record.value("quantite_total");
            stock_minimum = record.value("stock_minimum");

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
                		if (tmpQvString.length() > YerothUtils::STRING_MAX_CHARS)
                		{
                			tmpQvString.truncate(YerothUtils::STRING_MAX_CHARS);
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
    tableModel.yerothSetSort(YerothTableView::DESIGNATION_COLUMN, Qt::AscendingOrder);

    emit signal_lister(tableModel);

    bool s = tableModel.select();

    int rows = tableModel.rowCount();
    int columns = tableModel.columnCount();

    _stdItemModel->setRowCount(rows);
    _stdItemModel->setColumnCount(columns);

    YerothUtils::createTableModelHeaders(tableModel, *_stdItemModel, *_tableModelHeaders);

    QStandardItem *anItem = 0;
    QVariant qv;

    QMap<QString, QDate> codebarToDateEntree;

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

                        if (prevDate >= curDate)
                        {
                            codebarToDateEntree.insert(curCodebar, curDate);
                            codebarToTableRows_in_out.insert(curCodebar, i);
                        }
                        else
                        {
                            if (!codebarToDateEntree.contains(curCodebar))
                            {
                                codebarToDateEntree.insert(curCodebar, prevDate);
                                codebarToTableRows_in_out.insert(curCodebar, i);
                            }
                        }

                        //qDebug() << "++ 2. test, curCodebar: " << curCodebar
                        //	 << ", reference: " << curCodebar
                        // << ", t date: " << codebarToDateEntree[curCodebar]
                        //<< ", i row: " << i;
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
    tableModel.yerothSetSort(YerothTableView::DESIGNATION_COLUMN, Qt::AscendingOrder);

    emit signal_lister(tableModel);

    bool s = tableModel.select();

    int rows = tableModel.rowCount();
    int columns = tableModel.columnCount();

    _stdItemModel->setRowCount(rows);
    _stdItemModel->setColumnCount(columns);

    YerothUtils::createTableModelHeaders(tableModel, *_stdItemModel, *_tableModelHeaders);

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

        bool itemIsPreempted = false;

        for (int i = 0; i < rows; ++i)
        {
            record = tableModel.record(i);
            date_premption = record.value("date_peremption");
            quantite_total = record.value("quantite_total");
            stock_minimum = record.value("stock_minimum");
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
                    if (YerothTableView::DESIGNATION_COLUMN == k)
                    {
                        prevDesignation = curDesignation;
                        curDesignation = qv.toString();
                    }

                	tmpQvString.clear();
                	tmpQvString.append(qv.toString());
                	if (YerothTableView::REFERENCE_COLUMN != k)
                	{
                		if (tmpQvString.length() > YerothUtils::STRING_MAX_CHARS)
                		{
                			tmpQvString.truncate(YerothUtils::STRING_MAX_CHARS);
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

                        if (curDate > prevDate)
                        {
                            testDateEntreeOK = true;
                            designationToDateEntree.insert(curDesignation, curDate);
                            designationToTableRows_in_out.insert(curDesignation, i);
                            allElements.insert(curDesignation, curDate);
                            //qDebug() << QString("++ LIFO. 1) curDesignation: %1, curDate: %2")
                            //			.arg(curDesignation, DATE_TO_STRING(designationToDateEntree[curDesignation]));
                        }
                        else
                        {
                            if (!designationToDateEntree.contains(curDesignation))
                            {
                                testDateEntreeOK = true;
                                designationToDateEntree.insert(curDesignation, curDate);
                                designationToTableRows_in_out.insert(curDesignation, i);
                                allElements.insert(curDesignation, curDate);
                                //qDebug() << QString("++ LIFO. 2) curDesignation: %1, curDate: %2")
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

                    if (testDateEntreeOK && YerothTableView::DATE_ENTREE_COLUMN == k)
                    {
                        if (itemIsPreempted)
                        {
                            anItem->setForeground(YerothUtils::YEROTH_RED_COLOR);
                        }
                        else
                        {
                            QDate latestDate = getLatestDate(allElements, curDesignation);

                            //qDebug() << QString("++ 1, LATEST: designation %1, latestDate: %2")
                            //							.arg(curDesignation, DATE_TO_STRING(latestDate));

                            QStandardItem *itemDateOfStockEntry;
                            QString aDesignation;
                            QDate itemEntryDate;

                            for(int h = 0; h < i; ++h)
                            {
                                itemDateOfStockEntry = _stdItemModel->item(h, YerothTableView::DATE_ENTREE_COLUMN);

                                aDesignation = _stdItemModel->item(h, YerothTableView::DESIGNATION_COLUMN)->text();

                                //qDebug() << QString("++ designation: %1, itemEntryDate: %2")
                                //				.arg(aDesignation, itemDateOfStockEntry->text());

                                if (itemDateOfStockEntry &&
                                        YerothUtils::isEqualCaseInsensitive(aDesignation, curDesignation))
                                {
                                    itemEntryDate = GET_DATE_FROM_STRING(itemDateOfStockEntry->text());

                                    //qDebug() << QString("++ designation %1, itemEntryDate: %2")
                                    //		.arg(aDesignation, itemDateOfStockEntry->text());

                                    if (itemEntryDate != latestDate)
                                    {
                                        itemDateOfStockEntry->setForeground(Qt::white);
                                        //qDebug() << QString("++ item: %1, itemDateOfStockEntry: %2 <==> white")
                                        //.arg(aDesignation, itemDateOfStockEntry->text());
                                    }
                                }
                            }
                            if (latestDate == designationToDateEntree.value(curDesignation))
                            {
                                //qDebug() << QString("++ YES, designation %1, date: %2")
                                //				.arg(curDesignation, DATE_TO_STRING(latestDate));
                                anItem->setForeground(YerothUtils::YEROTH_GREEN_COLOR);
                            }
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
    tableModel.yerothSetSort(YerothTableView::DESIGNATION_COLUMN, Qt::AscendingOrder);

    emit signal_lister(tableModel);

    bool s = tableModel.select();

    int rows = tableModel.rowCount();
    int columns = tableModel.columnCount();

    _stdItemModel->setRowCount(rows);
    _stdItemModel->setColumnCount(columns);

    YerothUtils::createTableModelHeaders(tableModel, *_stdItemModel, *_tableModelHeaders);

    QStandardItem *anItem = 0;
    QVariant qv;

    QMap<QString, QDate> codebarToDateEntree;

    bool testDateEntreeOK = false;

    QString prevCodebar;
    QString curCodebar;

    QDate defaultDate;
    defaultDate.setDate(QDate::currentDate().year() - 7, 12, 31);

    QDate curDate = defaultDate;
    QDate prevDate = defaultDate;

    if(s)
    {
        bool itemIsPreempted = false;
        QMultiMap<QString, QDate> allElements;

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
                    if (REFERENCE_COLUMN == k)
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

                        if (curDate > prevDate)
                        {
                            testDateEntreeOK = true;
                            codebarToDateEntree.insert(curCodebar, curDate);
                            codebarToTableRows_in_out.insert(curCodebar, i);
                            allElements.insert(curCodebar, curDate);
                            //qDebug() << QString("++ LIFO. barcode. 1) curCodebar: %1, curDate: %2")
                            //		.arg(curDesignation, DATE_TO_STRING(designationToDateEntree[curCodebar]));
                        }
                        else
                        {
                            if (!codebarToDateEntree.contains(curCodebar))
                            {
                                testDateEntreeOK = true;
                                codebarToDateEntree.insert(curCodebar, curDate);
                                codebarToTableRows_in_out.insert(curCodebar, i);
                                allElements.insert(curCodebar, curDate);
                                //qDebug() << QString("++ LIFO. barcode. 2) curCodebar: %1, curDate: %2")
                                //		.arg(curDesignation, DATE_TO_STRING(designationToDateEntree[curCodebar]));
                            }
                        }

                        //qDebug() << "++ 2. test, curCodebar: " << curCodebar
                        //	 << ", reference: " << curCodebar
                        // << ", t date: " << codebarToDateEntree[curCodebar]
                        //<< ", i row: " << i;
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
                if (anItem)
                {
                    anItem->setForeground(Qt::white);

                    if (testDateEntreeOK && YerothTableView::DATE_ENTREE_COLUMN == k)
                    {
                        if (itemIsPreempted)
                        {
                            anItem->setForeground(YerothUtils::YEROTH_RED_COLOR);
                        }
                        else
                        {
                            QDate latestDate = getLatestDate(allElements, curCodebar);

                            //qDebug() << QString("++ 1, LATEST: designation %1, latestDate: %2")
                            //							.arg(curDesignation, DATE_TO_STRING(latestDate));

                            QStandardItem *itemDateOfStockEntry;
                            QString aCodebar;
                            QDate itemEntryDate;

                            for(int h = 0; h < i; ++h)
                            {
                                itemDateOfStockEntry = _stdItemModel->item(h, YerothTableView::DATE_ENTREE_COLUMN);

                                aCodebar = _stdItemModel->item(h, YerothTableView::REFERENCE_COLUMN)->text();

                                //qDebug() << QString("++ reference: %1, itemEntryDate: %2")
                                //				.arg(aCodebar, itemDateOfStockEntry->text());

                                if (itemDateOfStockEntry &&
                                        YerothUtils::isEqualCaseInsensitive(aCodebar, curCodebar))
                                {
                                    itemEntryDate = GET_DATE_FROM_STRING(itemDateOfStockEntry->text());

                                    //qDebug() << QString("++ designation %1, itemEntryDate: %2")
                                    //				.arg(aCodebar, itemDateOfStockEntry->text());

                                    if (itemEntryDate != latestDate)
                                    {
                                        itemDateOfStockEntry->setForeground(Qt::white);
                                        //qDebug() << QString("++ item: %1, itemDateOfStockEntry: %2 <==> white")
                                        //				.arg(aCodebar, itemDateOfStockEntry->text());
                                    }
                                }
                            }
                            if (latestDate == codebarToDateEntree.value(curCodebar))
                            {
                                //qDebug() << QString("++ YES, designation %1, date: %2")
                                //				.arg(curCodebar, DATE_TO_STRING(latestDate));
                                anItem->setForeground(YerothUtils::YEROTH_GREEN_COLOR);
                            }
                        }
                        testDateEntreeOK = false;
                    }
                }
            }

            this->resizeColumnsToContents();
        }
    }
}

void YerothTableView::lister_DEF_DEO(YerothSqlTableModel &tableModel,
                                    QMap<QString, int> &designationToTableRows_in_out)
{
    tableModel.yerothSetSort(YerothTableView::DESIGNATION_COLUMN, Qt::AscendingOrder);

    emit signal_lister(tableModel);

    bool s = tableModel.select();

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
            date_premption = record.value("date_peremption");
            quantite_total = record.value("quantite_total");
            stock_minimum = record.value("stock_minimum");
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
                		if (tmpQvString.length() > YerothUtils::STRING_MAX_CHARS)
                		{
                			tmpQvString.truncate(YerothUtils::STRING_MAX_CHARS);
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
    tableModel.yerothSetSort(YerothTableView::REFERENCE_COLUMN, Qt::AscendingOrder);

    emit signal_lister(tableModel);

    bool s = tableModel.select();

    int rows = tableModel.rowCount();
    int columns = tableModel.columnCount();

    _stdItemModel->setRowCount(rows);
    _stdItemModel->setColumnCount(columns);

    YerothUtils::createTableModelHeaders(tableModel, *_stdItemModel, *_tableModelHeaders);

    QStandardItem *anItem = 0;
    QVariant qv;

    QMap<QString, QDate> codebarToDatePreemption;
    QMultiMap<QString, QDate> allElements;

    QString prevCodebar;
    QString curCodebar;

    QDate defaultDate;
    defaultDate.setDate(QDate::currentDate().year() + 1000, 12, 31);

    QDate curDate = defaultDate;
    QDate prevDate = defaultDate;

    bool testDatePreemptionOK = false;

    if(s)
    {
        QSqlRecord record;
        QVariant quantite_total;
        QVariant stock_minimum;
        QVariant date_premption;
        bool itemIsPreempted = false;

        for (int i = 0; i < rows; ++i)
        {
            quantite_total = record.value("quantite_total");
            stock_minimum = record.value("stock_minimum");
            record = tableModel.record(i);
            date_premption = record.value("date_peremption");
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
                            testDatePreemptionOK = true;
                            allElements.insert(curCodebar, curDate);
                            //qDebug() << "++ 1-OK. curCodebar: "
                            //	 << curCodebar << " date: " << curDate;
                        }
                        else
                        {
                            if (!codebarToDatePreemption.contains(curCodebar))
                            {
                                testDatePreemptionOK = true;
                                allElements.insert(curCodebar, curDate);
                                //qDebug() << "++ 2-OK. curCodebar: "
                                //	 << curCodebar << " date: " << curDate;
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
                            QDate earliestDate = getEarliestDate(allElements, curCodebar);

                            //qDebug() << "\t++ earliestDate: " << earliestDate;
                            //QStandardItem *item;
                            QStandardItem *itemCodebar;
                            QString aCodebar;
                            QDate aDate;

                            for(int h = 0; h < i; ++h)
                            {
                                itemCodebar = _stdItemModel->item(h, YerothTableView::DESIGNATION_COLUMN);
                                if (itemCodebar)
                                {
                                    aCodebar = itemCodebar->text();
                                    aDate = codebarToDatePreemption.value(aCodebar);
                                    //qDebug() << "++ itemCodebar: " << aCodebar;

                                    if (YerothUtils::isEqualCaseInsensitive(aCodebar, curCodebar))
                                    {
                                        //item = _stdItemModel->item(h, DATE_PREEMPTION_COLUMN);
                                        bool isPreempted = aDate < GET_CURRENT_DATE;
                                        if (!isPreempted)
                                        {
                                            //Do nothing
                                        }
                                        else
                                        {
                                            //item is preempted. so leave colour as it is (red).
                                        }
                                    }
                                }
                            }

                            codebarToDatePreemption.insert(curCodebar, earliestDate);
                            codebarToTableRows_in_out.insert(curCodebar, i);

                            //qDebug() << "++ ici: " << codebarToDatePreemption;

                            anItem->setForeground(YerothUtils::YEROTH_GREEN_COLOR);
                            //qDebug() << QString("\tgreen for %1 with date %2.")
                            //					.arg(curCodebar,
                            //					DATE_TO_STRING(earliestDate));

                            testDatePreemptionOK = false;
                        }
                    }//first else
                } // first if
            } //switch-case
        }

        this->resizeColumnsToContents();
    }
}

