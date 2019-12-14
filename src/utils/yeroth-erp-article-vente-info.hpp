/*
 * yeroth-erp-article-vente-info.hpp
 *
 *      Author: Xavier NOUMBISSI NOUNDOU, Dipl.-Inf., Ph.D. (ABD)
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

	YerothArticleVenteInfo();

	~YerothArticleVenteInfo();

	void print();

	inline double prix_vente()
	{
		return (prix_unitaire + _montant_tva - remise_prix) * quantite_a_vendre;
	}

	inline double montant_tva()
	{
		return _montant_tva * quantite_a_vendre;
	}

	inline QString prixVente()
	{
		return QString::number(this->prix_vente(), 'f', 2);
	}

	inline QString quantiteEnStock()
	{
		return GET_NUM_STRING((int)quantite_en_stock);
	}

	inline QString prixUnitaire()
	{
		return QString::number(prix_unitaire, 'f', 2);
	}

	inline QString quantiteAVendre()
	{
		return QString::number(quantite_a_vendre, 'f', 0);
	}

	inline QString montantTva()
	{
		return QString::number(this->montant_tva(), 'f', 2);
	}

	inline QString remisePrix()
	{
		return QString::number(remise_prix, 'f', 2);
	}

	inline QString remisePourcentage()
	{
		return QString::number(remise_pourcentage, 'f', 2);
	}

	QString reference;
	QString designation;
	QString categorie;

	int 	sqlTableModelIndex;
	double 	quantite_en_stock;
	double 	prix_unitaire;
	double 	quantite_a_vendre;
	double 	_montant_tva;
	double 	remise_prix;
	double 	remise_pourcentage;
};


#endif /* SRC_UTILS_YEROTH_ARTICLE_VENTE_INFO_HPP_ */
