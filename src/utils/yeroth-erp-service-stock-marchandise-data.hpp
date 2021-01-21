/*
 * yeroth-erp-service-stock-marchandise-data.hpp
 *
 *      Author: DR. XAVIER NOUMBISSI NOUNDOU
 */

#ifndef SRC_YEROTH_SERVICE_STOCK_MARCHANDISES_DATA_HPP_
#define SRC_YEROTH_SERVICE_STOCK_MARCHANDISES_DATA_HPP_

#include "src/include/yeroth-erp-3-0-definition-oo-class-operators.hpp"

#include "src/utils/yeroth-erp-utils.hpp"


#include <QtCore/QByteArray>
#include <QtCore/QString>


class YerothERPServiceStockMarchandiseData
{

public:

	YEROTH_CLASS_OPERATORS

	inline YerothERPServiceStockMarchandiseData()
	:_isService(false),
     _prix_dachat_precedent("0.0"),
	 _prix_vente_precedent("0.0"),
	 _prix_vente_en_gros_precedent("0.0")
	{
	}

	inline ~YerothERPServiceStockMarchandiseData()
	{
	}


	bool 		_isService;

	QString		_prix_dachat_precedent;

	QString		_prix_vente_precedent;

	QString		_prix_vente_en_gros_precedent;

	QString 	_nom_departement_produit;

	QString 	_categorie;

    QString 	_reference;

    QString 	_designation;

    QString 	_description;
};

#endif /* SRC_YEROTH_SERVICE_STOCK_MARCHANDISES_DATA_HPP_ */
