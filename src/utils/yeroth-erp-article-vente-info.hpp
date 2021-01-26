/*
 * yeroth-erp-article-vente-info.hpp
 *
 *      Author: DR. XAVIER NOUMBISSI NOUNDOU
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
	:_effectuer_vente_en_gros(false),
	 _quantite_en_stock(0.0),
	 _prix_unitaire(0.0),
	 _prix_unitaire_en_gros(0.0),
	 _quantite_a_vendre(0.0),
	 _montant_tva(0.0),
	 _montant_tva_en_gros(0.0),
	 _remise_prix(0.0),
	 _remise_pourcentage(0.0)
	{
	}

	inline ~YerothArticleVenteInfo()
	{
	}

	void print();

	inline QString get_stocksID()
	{
		return _stockID;
	}

	double prix_vente();

	inline double prix_unitaire()
	{
		return _effectuer_vente_en_gros ? _prix_unitaire_en_gros : _prix_unitaire;
	}

	inline double montant_tva()
	{
		return _effectuer_vente_en_gros ?
				_montant_tva_en_gros * _quantite_a_vendre :
				_montant_tva * _quantite_a_vendre;
	}

	inline QString prixVente()
	{
		return QString::number(prix_vente(), 'f', 2);
	}

	inline QString quantiteEnStock()
	{
		return GET_NUM_STRING((int)_quantite_en_stock);
	}

	inline QString prixUnitaire()
	{
		return _effectuer_vente_en_gros ?
				QString::number(_prix_unitaire_en_gros, 'f', 2) :
				QString::number(_prix_unitaire, 'f', 2);
	}

	inline QString quantiteAVendre()
	{
		return QString::number(_quantite_a_vendre, 'f', 0);
	}

	inline QString montantTva()
	{
		return QString::number(montant_tva(), 'f', 2);
	}

	inline double remise_prix()
	{
		return _remise_prix;
	}

	inline double remise_pourcentage()
	{
		return _remise_pourcentage;
	}

	inline QString remisePrix()
	{
		return QString::number(_remise_prix, 'f', 2);
	}

	inline QString remisePourcentage()
	{
		return QString::number(_remise_pourcentage, 'f', 2);
	}

	inline void setRemisePrix(double remise_prix)
	{
		_remise_prix = remise_prix;
	}

	inline void setRemisePourcentage(double remise_pourcentage)
	{
		_remise_pourcentage = remise_pourcentage;
	}


	bool 	_effectuer_vente_en_gros;

	double 	_quantite_en_stock;

	double 	_quantite_a_vendre;

	double 	_montant_tva;

	double 	_montant_tva_en_gros;

	double 	_prix_unitaire;

	double 	_prix_unitaire_en_gros;


	QString _stockReference;

	QString _stockName;

	QString _stockNomDepartementProduits;

	QString _stockCategorie;

	QString _stockID;


private:

	double 	_remise_prix;

	double 	_remise_pourcentage;

};


#endif /* SRC_UTILS_YEROTH_ARTICLE_VENTE_INFO_HPP_ */
