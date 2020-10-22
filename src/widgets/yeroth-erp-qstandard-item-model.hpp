/*
 * yeroth-erp-qstandard-item-model.hpp
 *
 * 		Author: DIPL.-INF. XAVIER NOUMBISSI NOUNDOU
 */

#ifndef YEROTH_ERP_QSTANDARD_ITEM_MODEL_HPP_
#define YEROTH_ERP_QSTANDARD_ITEM_MODEL_HPP_


#include "src/include/yeroth-erp-3-0-definition-oo-class-operators.hpp"

#include "src/utils/yeroth-erp-sqltable-model.hpp"


#include <QtGui/QStandardItemModel>


class YerothTableView;


class YerothPOSQStandardItemModel : public QStandardItemModel
{

public:

	YEROTH_CLASS_OPERATORS

	inline YerothPOSQStandardItemModel(QObject * parent = 0)
	:QStandardItemModel(parent),
	 _curTableView(0)
	{
		_curSqlTableModel = 0;
	}


	inline YerothPOSQStandardItemModel(int rows,
							   	   	   int columns,
									   QObject *parent = 0)
	:QStandardItemModel(rows, columns, parent),
	 _curTableView(0)
	{
		_curSqlTableModel = 0;
	}

	inline ~YerothPOSQStandardItemModel()
	{
		yerothPOSClear();
	}

	void yerothPOSClear();

	QModelIndex create_ZERO_ZERO_MODEL_INDEX_FOR_TABLE_VIEWING();

	virtual void sort(int column, Qt::SortOrder order = Qt::AscendingOrder);

public:

	YerothTableView		*_curTableView;

	YerothSqlTableModel *_curSqlTableModel;
};


#endif /* YEROTH_ERP_QSTANDARD_ITEM_MODEL_HPP_ */
