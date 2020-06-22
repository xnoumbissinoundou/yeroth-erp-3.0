/*
 * yeroth-erp-article-vente-info.hpp
 *
 *      Author: Dipl.-Inf. XAVIER NOUMBISSI NOUNDOU
 */

#ifndef SRC_UTILS_YEROTH_ARTICLE_VENTE_INFO_HPP_
#define SRC_UTILS_YEROTH_ARTICLE_VENTE_INFO_HPP_

#include "src/include/yeroth-erp-3-0-software.text-configuration.hpp"

#include <QtCore/QString>
#include <QtCore/QDebug>

#include "src/utils/yeroth-erp-utils.hpp"

class YerothUtils;

class YerothArticleVenteInfo
{
public:

	YEROTH_CLASS_OPERATORS

	inline YerothArticleVenteInfo()
	:_quantite_en_stock(0.0),
	 _prix_unitaire(0.0),
	 _quantite_a_vendre(0.0),
	 _montant_tva(0.0),
	 _remise_prix(0.0),
	 _remise_pourcentage(0.0)
	{
	}

	inline ~YerothArticleVenteInfo()
	{
	}

	void print();

	inline double prix_vente()
	{
		return (_prix_unitaire + _montant_tva - _remise_prix) * _quantite_a_vendre;
	}

	inline double montant_tva()
	{
		return _montant_tva * _quantite_a_vendre;
	}

	inline QString prixVente()
	{
		return QString::number(this->prix_vente(), 'f', 2);
	}

	inline QString quantiteEnStock()
	{
		return GET_NUM_STRING((int)_quantite_en_stock);
	}

	inline QString prixUnitaire()
	{
		return QString::number(_prix_unitaire, 'f', 2);
	}

	inline QString quantiteAVendre()
	{
		return QString::number(_quantite_a_vendre, 'f', 0);
	}

	inline QString montantTva()
	{
		return QString::number(this->montant_tva(), 'f', 2);
	}

	inline QString remisePrix()
	{
		return QString::number(_remise_prix, 'f', 2);
	}

	inline QString remisePourcentage()
	{
		return QString::number(_remise_pourcentage, 'f', 2);
	}


	double 	_quantite_en_stock;

	double 	_prix_unitaire;

	double 	_quantite_a_vendre;

	double 	_montant_tva;

	double 	_remise_prix;

	double 	_remise_pourcentage;


	QString _stockReference;

	QString _stockName;

	QString _stockCategorie;

	QString _stockID;
};


#endif /* SRC_UTILS_YEROTH_ARTICLE_VENTE_INFO_HPP_ */
