/*
 * yeroth-erp-historique-stock.hpp
 *
 *      Author: DIPL.-INF. XAVIER NOUMBISSI NOUNDOU
 */

#ifndef SRC_UTILS_YEROTH_HISTORIQUE_STOCK_HPP_
#define SRC_UTILS_YEROTH_HISTORIQUE_STOCK_HPP_

#include "src/include/yeroth-erp-3-0-software.text-configuration.hpp"

#include <QtCore/QString>
#include <QtCore/QDebug>


class YerothUtils;


enum TypeMouvementStock
{
	MOUVEMENT_DE_STOCK_INDEFINI 			= 	0,
	MOUVEMENT_DE_STOCK_ENTREE	 			= 	1,
	MOUVEMENT_DE_STOCK_VENTE    			= 	2,
	MOUVEMENT_DE_STOCK_SORTIE	  			= 	3,
	MOUVEMENT_DE_STOCK_TRANSFERT 			= 	4,
	MOUVEMENT_DE_STOCK_RETOUR_VENTE			= 	5,
	MOUVEMENT_DE_STOCK_RE_APPROVISIONNEMENT	= 	6
};

class YerothHistoriqueStock
{
public:

	YEROTH_CLASS_OPERATORS

private:

	YerothHistoriqueStock();

	~YerothHistoriqueStock();

public:


	static void listHistoriqueDuStock(const QString historiqueDuStock,
									  const QString stockID,
									  const QString stockReference,
									  const QString stockDesignation);


	static QString get_mouvement_stock(QString aMouvementStock_in_out);

	//static void get_mouvements_stocks(QStringList &aMouvementStockList, int aStockId);


	static QString creer_mouvement_stock(enum  	TypeMouvementStock aTypeMouvementStock,
										 int   	operationId,
										 QDate 	date,
										 double qte_initiale_en_stock,
										 double qte_en_mouvement,
										 double qte_restante);

	static QString get_type_mouvement_stock_string(const QString &aStockHistory);

	static enum TypeMouvementStock get_type_mouvement_stock(QString mouvementStock);

	static int get_id_mouvement_stock(QString mouvementStock);

	static QDate get_date_mouvement_stock(QString mouvementStock);

	static const QString SEPARATION_EXTERNE;

	static const QString SEPARATION_INTERNE;

private:

	static QMap<QString, enum TypeMouvementStock> _qstringToTypeMouvement;
};


#endif /* SRC_UTILS_YEROTH_HISTORIQUE_STOCK_HPP_ */
