/*
 * yeroth-erp-article-vente-info.cpp
 *
 *      Author: DIPL.-INF. XAVIER NOUMBISSI NOUNDOU
 */

#include "yeroth-erp-article-vente-info.hpp"

#include <QtCore/QDebug>


void YerothArticleVenteInfo::print()
{
    qDebug() << QString("[ArticleVenteInfo][print]"
    					"	stockReference: %1"
    					"	stockName: %2"
    					"	stockID: %3"
    					"	_qte_en_stock: %4"
    					"	_prix_unitaire: %5"
    					"	_quantite_a_vendre: %6"
    					"	_montant_tva: %7"
    					"	_remise_prix: %8"
    					"	_remise_pourcentage: %9")
    				.arg(_stockReference,
    					 _stockName,
						 _stockID,
						 QString::number(_quantite_en_stock),
						 QString::number(_prix_unitaire),
						 QString::number(_quantite_a_vendre),
						 QString::number(_montant_tva),
						 QString::number(_remise_prix),
						 QString::number(_remise_pourcentage));
}


double YerothArticleVenteInfo::prix_vente()
{
	double result_prix_vente = 0.0;

	if (!_effectuer_vente_en_gros)
	{
		result_prix_vente =
				(_prix_unitaire + _montant_tva - _remise_prix) * _quantite_a_vendre;
	}
	else
	{
		result_prix_vente =
				(_prix_unitaire_en_gros + _montant_tva_en_gros - _remise_prix) * _quantite_a_vendre;
	}

	return result_prix_vente;
}

