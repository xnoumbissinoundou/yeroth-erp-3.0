/*
 * yeroth-erp-table-widget.hpp
 *
 *      Author: DR. XAVIER NOUMBISSI NOUNDOU
 */

#ifndef SRC_WIDGETS_YEROTH_TABLE_WIDGET_HPP_
#define SRC_WIDGETS_YEROTH_TABLE_WIDGET_HPP_

#include "src/include/yeroth-erp-3-0-software.text-configuration.hpp"

#include "yeroth-erp-qtable-widget-item.hpp"

#include "src/utils/yeroth-erp-article-vente-info.hpp"

#include <QtWidgets/QTableWidget>

#include <QtCore/QMap>

class YerothQTableWidgetItem;
class YerothLogger;

class YerothTableWidget : public QTableWidget
{
	Q_OBJECT

public:

	YEROTH_CLASS_OPERATORS

	YerothTableWidget(QWidget *parent = 0);

	inline virtual ~YerothTableWidget()
	{
	    delete _logger;
	}

	virtual void setQStandardItemFlags(Qt::ItemFlags &flags);

	virtual void myClear();

	virtual void removeArticle(int tableWidgetRow);

	inline virtual QString getStockID(int aRow)
	{
		return _mapListIdxToElement_db_ID.value(aRow);
	}

	inline virtual int itemCount()
	{
		return _mapListIdxToElement_db_ID.size();
	}

	static const QString QTE_1;

signals:

	void addedYerothTableWidget();

	void emptiedYerothTableWidget();

protected:

	int 					_curRow;

	QMap<int, QString> 		_mapListIdxToElement_db_ID;

	YerothLogger		 	*_logger;
};


#endif /* SRC_WIDGETS_YEROTH_TABLE_WIDGET_HPP_ */
