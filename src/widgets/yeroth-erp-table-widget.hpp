/*
 * yeroth-erp-list-widget.hpp
 *
 *  Created on: Mar 21, 2016
 *      Author: Dipl.-Inf. Xavier NOUMBISSI NOUNDOU, Ph.D. (ABD)
 *      Email:  xnoundou7@gmail.com
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

	~YerothTableWidget();

	inline void setColoursQStandardItem()
	{
		_qteItem->setForeground(YerothUtils::YEROTH_QBRUSH_ORANGE);
	}

	void setQStandardItemFlags(Qt::ItemFlags &flags);

	void myClear();

	int addArticleAfter(YerothArticleVenteInfo &articleVenteInfo);

	int addArticle(int stocksSqlTableModelIndex,
				   QString reference,
				   QString designation,
				   QString categorie,
				   QString prix_unitaire,
				   QString montant_tva,
				   QString prix_vente,
				   QString quantite_a_vendre,
				   QString quantite_total_stock);

	void removeArticle(int tableWidgetRow);

	inline int getSqlTableModelIndex(int aRow)
	{
		return _mapListIdxToItems.value(aRow);
	}

	inline int itemCount()
	{
		return _mapListIdxToItems.size();
	}

    static const unsigned int REFERENCE_COLUMN;

    static const unsigned int CATEGORIE_COLUMN;

    static const unsigned int DESIGNATION_COLUMN;

	static const unsigned int TOTAL_TVA_COLUMN;

	static const unsigned int TOTAL_COLUMN;

	static const unsigned int QTE_COLUMN;

	static const unsigned int QTE_TOTAL_STOCK;

	static const QString QTE_1;

signals:

	void addedYerothTableWidget();

	void emptiedYerothTableWidget();

private:

	int 				_curRow;

	QMap<int, int> 		_mapListIdxToItems;

	YerothQTableWidgetItem 	*_referenceItem;

	YerothQTableWidgetItem 	*_designationItem;

	YerothQTableWidgetItem 	*_categorieItem;

	YerothQTableWidgetItem 	*_prixUnitaireItem;

	YerothQTableWidgetItem 	*_montantTvaItem;

	YerothQTableWidgetItem 	*_prixVenteItem;

	YerothQTableWidgetItem 	*_qteItem;

	YerothQTableWidgetItem 	*_qteTotalStockItem;

	YerothLogger		 	*_logger;
};


#endif /* SRC_WIDGETS_YEROTH_TABLE_WIDGET_HPP_ */
