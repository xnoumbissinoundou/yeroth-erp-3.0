/*
 * yeroth-erp-table-widget.hpp
 *
 *      Author: PR. XAVIER NOUMBISSI NOUNDOU
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

	inline virtual QString get_DB_ELEMENT_db_ID(int aRow)
	{
		return _mapListIdxToElement_db_ID.value(aRow);
	}

	inline const QMap<int, QString> &get_mapListIdxToElement_db_ID()
	{
		return _mapListIdxToElement_db_ID;
	}

	inline virtual int itemCount()
	{
		return _mapListIdxToElement_db_ID.size();
	}

	virtual void setQStandardItemFlags(Qt::ItemFlags &flags);

	static const QString QTE_1;

public slots:

	virtual void yerothClearTableWidgetContent();

	virtual void removeArticle(int tableWidgetRow);

protected:

	inline virtual void set_MY_QStandardItemFlags()
	{
		setQStandardItemFlags(_myQStandardItemFlags);
	}

signals:

	void addedYerothTableWidget();

	void emptiedYerothTableWidget();

protected:

	int 					_curRow;

	Qt::ItemFlags 			_myQStandardItemFlags;

	QMap<int, QString> 		_mapListIdxToElement_db_ID;

	YerothLogger		 	*_logger;
};


#endif /* SRC_WIDGETS_YEROTH_TABLE_WIDGET_HPP_ */
