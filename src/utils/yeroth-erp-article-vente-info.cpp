/*
 * yeroth-erp-article-vente-info.cpp
 *
 *      Author: Xavier NOUMBISSI NOUNDOU, Dipl.-Inf., Ph.D. (ABD)
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
