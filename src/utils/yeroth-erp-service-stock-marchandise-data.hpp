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


class YerothERPServiceData
{

public:

	YEROTH_CLASS_OPERATORS

	inline YerothERPServiceData()
	:_isService(true)
	{
	}

	inline ~YerothERPServiceData()
	{
	}

	bool 		_isService;

	QString 	_nom_departement_produit;

	QString 	_categorie;

    QString 	_reference;

    QString 	_designation;

    QString 	_description;
};


class YerothERPStockMarchandiseData : public YerothERPServiceData
{

public:

	YEROTH_CLASS_OPERATORS

	inline YerothERPStockMarchandiseData()
	:_prix_dachat_precedent("0.0"),
	 _prix_vente_precedent("0.0"),
	 _prix_vente_en_gros_precedent("0.0")
	{
		_isService = false;
	}

	inline ~YerothERPStockMarchandiseData()
	{
	}


	QString		_prix_dachat_precedent;

	QString		_prix_vente_precedent;

	QString		_prix_vente_en_gros_precedent;
};



#endif /* SRC_YEROTH_SERVICE_STOCK_MARCHANDISES_DATA_HPP_ */
