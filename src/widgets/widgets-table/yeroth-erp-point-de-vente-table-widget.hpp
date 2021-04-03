/*
 * yeroth-erp-point-de-vente-table-widget.hpp
 *
 *      Author: PR. XAVIER NOUMBISSI NOUNDOU
 */

#ifndef SRC_WIDGETS_YEROTH_POINT_DE_VENTE_TABLE_WIDGET_HPP_
#define SRC_WIDGETS_YEROTH_POINT_DE_VENTE_TABLE_WIDGET_HPP_


#include "src/widgets/widgets-table/yeroth-erp-table-widget.hpp"


class YerothPointDeVenteTableWidget : public YerothTableWidget
{
	Q_OBJECT

public:

	YEROTH_CLASS_OPERATORS

	inline YerothPointDeVenteTableWidget(QWidget *parent = 0)
	:YerothTableWidget(parent),
	 _stockReferenceItem(0),
	 _stockNameItem(0),
	 _stockCategorieItem(0),
	 _prixUnitaireItem(0),
	 _montantTvaItem(0),
	 _prixVenteItem(0),
	 _qteItem(0),
	 _qteTotalStockItem(0)
	{
	}

	inline virtual ~YerothPointDeVenteTableWidget()
	{
	}

	inline void setColours_QTE_QStandardItem()
	{
		_qteItem->setForeground(YerothUtils::YEROTH_QBRUSH_ORANGE);
	}

	int addArticleAfter(YerothArticleVenteInfo &articleVenteInfo);

	int addArticle(QString stockID,
	               QString stockReference,
				   QString stockName,
				   QString stockCategorie,
				   QString prix_unitaire,
				   QString montant_tva,
				   QString prix_vente,
				   QString quantite_a_vendre,
				   QString quantite_total_stock);

    static const unsigned int REFERENCE_COLUMN;

    static const unsigned int CATEGORIE_COLUMN;

    static const unsigned int DESIGNATION_COLUMN;

	static const unsigned int TOTAL_TVA_COLUMN;

	static const unsigned int TOTAL_COLUMN;

	static const unsigned int QTE_COLUMN;

	static const unsigned int QTE_TOTAL_STOCK;

private:

	YerothQTableWidgetItem 	*_stockReferenceItem;

	YerothQTableWidgetItem 	*_stockNameItem;

	YerothQTableWidgetItem 	*_stockCategorieItem;

	YerothQTableWidgetItem 	*_prixUnitaireItem;

	YerothQTableWidgetItem 	*_montantTvaItem;

	YerothQTableWidgetItem 	*_prixVenteItem;

	YerothQTableWidgetItem 	*_qteItem;

	YerothQTableWidgetItem 	*_qteTotalStockItem;
};


#endif /* SRC_WIDGETS_YEROTH_POINT_DE_VENTE_TABLE_WIDGET_HPP_ */
