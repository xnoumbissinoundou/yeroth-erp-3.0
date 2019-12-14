/*
 * yeroth-erp-article-vente-info.cpp
 *
 *      Author: Xavier NOUMBISSI NOUNDOU, Dipl.-Inf., Ph.D. (ABD)
 */

#include "yeroth-erp-article-vente-info.hpp"

#include <QtCore/QDebug>

YerothArticleVenteInfo::YerothArticleVenteInfo()
    :sqlTableModelIndex(-1),
     quantite_en_stock(0.0),
     prix_unitaire(0.0),
     quantite_a_vendre(0.0),
     _montant_tva(0.0),
     remise_prix(0.0),
     remise_pourcentage(0.0)
{

}

YerothArticleVenteInfo::~YerothArticleVenteInfo()
{

}

void YerothArticleVenteInfo::print()
{
    qDebug() << "[ArticleVenteInfo][print]"
             << "\n\treference: " << reference
             << "\n\tdesignation: " << designation
             << "\n\tsqlTableModelIndex: " << sqlTableModelIndex
             << "\n\tqte_en_stock: " << quantite_en_stock
             << "\n\tprix_unitaire: " << prix_unitaire
             << "\n\tquantite_a_vendre: " << quantite_a_vendre
             << "\n\tmontant_tva: " << _montant_tva
             << "\n\tremise_prix: " << remise_prix
             << "\n\tremise_pourcentage: " << remise_pourcentage
             << "\n";
}
