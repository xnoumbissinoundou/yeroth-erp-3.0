/*
 * yeroth-erp-service-stock-marchandise-data.hpp
 *
 *      Author: Xavier NOUMBISSI NOUNDOU, Dipl.-Inf., Ph.D. (ABD)
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
	:_isService(false)
	{
	}

	inline ~YerothERPServiceStockMarchandiseData()
	{
	}

	bool 		_isService;

    QString 	_categorie;

    QString 	_reference;

    QString 	_designation;

    QString 	_description;
};

#endif /* SRC_YEROTH_SERVICE_STOCK_MARCHANDISES_DATA_HPP_ */
