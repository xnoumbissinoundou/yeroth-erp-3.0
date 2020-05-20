/*
 * yeroth-erp-ventes-table-view.cpp
 *
 *      Author: Xavier NOUMBISSI NOUNDOU, Dipl.-Inf., Ph.D. (ABD)
 */


#include "src/widgets/table-view/yeroth-erp-ventes-table-view.hpp"

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


int YerothERPVentesTableView::reference_column_idx;

int YerothERPVentesTableView::typedevente_column_idx;


YerothERPVentesTableView::YerothERPVentesTableView()
:YerothTableView()
{
	_stdItemModel->_curTableView = this;
}

YerothERPVentesTableView::YerothERPVentesTableView(QWidget * parent)
:YerothTableView(parent)
{
	_stdItemModel->_curTableView = this;
}

YerothERPVentesTableView::~YerothERPVentesTableView()
{
}

void YerothERPVentesTableView::lister_les_elements_du_tableau(YerothSqlTableModel &tableModel_in_out,
															  YerothWindowsCommons *aCallingWindows)
{
	_stdItemModel->_curSqlTableModel = &tableModel_in_out;

    emit signal_lister(tableModel_in_out);

    bool s = tableModel_in_out.select();

    int rows = tableModel_in_out.rowCount();
    int columns = tableModel_in_out.columnCount();

    _stdItemModel->setRowCount(rows);
    _stdItemModel->setColumnCount(columns);

    YerothUtils::createTableModelHeaders(tableModel_in_out, *_stdItemModel, *_tableModelHeaders);

    QString tmpQvString;
    QStandardItem *anItem = 0;
    QVariant qv;

    if(s)
    {
        for (int i = 0; i < rows; ++i)
        {
            for (int k = 0; k < columns; ++k)
            {
                qv.setValue(tableModel_in_out.record(i).value(k));

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

					if (YerothERPVentesTableView::typedevente_column_idx != k)
					{
                		tmpQvString = YerothUtils::_typedeventeToUserViewString.value(qv.toInt());
                		anItem = new YerothQStandardItem(tmpQvString);
					}
					else
					{
						anItem = new YerothQStandardItem(GET_NUM_STRING(qv.toInt()));
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

					if (YerothERPVentesTableView::reference_column_idx != k)
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
                    //qDebug() << "YerothTableView::lister(): undecoded QVariant -> " << qv.type();
                    break;
                }

                if (anItem)
                {
                    anItem->setForeground(Qt::white);
                }
            }
        }

        this->resizeColumnsToContents();
    }
}

