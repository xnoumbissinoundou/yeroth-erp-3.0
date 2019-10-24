/*
 * yeroth-erp-historique-stock.hpp
 *
 *      Author: Dipl.-Inf. Xavier NOUMBISSI NOUNDOU, Ph.D. (ABD)
 */

#include "src/utils/yeroth-erp-historique-stock.hpp"

#include "src/yeroth-erp-windows.hpp"

#include "src/utils/yeroth-erp-utils.hpp"



#include <QtCore/QMap>


const QString YerothHistoriqueStock::SEPARATION_EXTERNE("*");

const QString YerothHistoriqueStock::SEPARATION_INTERNE(";");

QMap<QString, enum TypeMouvementStock> YerothHistoriqueStock::_qstringToTypeMouvement;



YerothHistoriqueStock::YerothHistoriqueStock()
{
	_qstringToTypeMouvement.insert("0", INDEFINI);
	_qstringToTypeMouvement.insert("1", ENTREE);
	_qstringToTypeMouvement.insert("2", VENTE);
	_qstringToTypeMouvement.insert("3", SORTIE);
	_qstringToTypeMouvement.insert("4", TRANSFERT);
}


YerothHistoriqueStock::~YerothHistoriqueStock()
{

}


void YerothHistoriqueStock::listHistoriqueDuStock(const QString historiqueDuStock,
												  const QString stockID,
												  const QString stockReference,
												  const QString stockDesignation)
{
	//qDebug() << QString("YerothHistoriqueStock::listHistoriqueDuStock | %1")
	//				.arg(QString::number(aStockId));

	QStringList aMouvementStockList;

	aMouvementStockList.append(historiqueDuStock.split(YerothHistoriqueStock::SEPARATION_EXTERNE));

	YerothUtils::getAllWindows()->_historiqueDuStockWindow->listHistoriqueDuStock(aMouvementStockList,
																				  stockID,
																				  stockReference,
																				  stockDesignation);
}


QString YerothHistoriqueStock::get_mouvement_stock(QString aMouvementStock_in_out)
{
    QString aMouvementStock;

    QStringList aComplexMouvementStock(
    		aMouvementStock_in_out.split(YerothHistoriqueStock::SEPARATION_INTERNE));

	return aMouvementStock;
}


QString YerothHistoriqueStock::creer_mouvement_stock(enum TypeMouvementStock aTypeMouvementStock,
													int  	mouvementId,
													QDate 	date,
													double 	qte_initiale_en_stock,
													double 	qte_retiree,
													double 	qte_restante)
{
	unsigned int typeMouvement = (unsigned int) aTypeMouvementStock;

	QString result = QString("%1;%2;%3;%4;%5;%6;%7")
						.arg(DATE_TO_STRING(date),
							 CURRENT_TIME,
							 QString::number(typeMouvement),
							 QString::number(mouvementId),
							 GET_DOUBLE_STRING(qte_initiale_en_stock),
							 GET_DOUBLE_STRING(qte_retiree),
							 GET_DOUBLE_STRING(qte_restante));

	return result;
}


QString YerothHistoriqueStock::get_type_mouvement_stock_string(const QString & aStockHistory)
{
	//enum TypeMouvementStock type_mouvement(YerothHistoriqueStock::get_type_mouvement_stock(aStockHistory));
	//qDebug() << "++ qstring: " << aStockHistory;

	if ("1" == aStockHistory)
	{
		return QObject::tr("ENTREE");
	}
	else if ("2" == aStockHistory)
	{
		return QObject::tr("VENTE");
	}
	else if ("3" == aStockHistory)
	{
		return QObject::tr("SORTIE");
	}
	else if ("4" == aStockHistory)
	{
		return QObject::tr("TRANSFERT");
	}
	else
	{
		return QObject::tr("INDEFINI");
	}
}


enum TypeMouvementStock YerothHistoriqueStock::get_type_mouvement_stock(QString aStockHistoryEntry)
{
	 QString type_mouvement = aStockHistoryEntry.split(YerothHistoriqueStock::SEPARATION_INTERNE).at(2);

	 QMap<QString, enum TypeMouvementStock>::const_iterator i = _qstringToTypeMouvement.find(type_mouvement);

	 if (i != _qstringToTypeMouvement.end())
	 {
		 //qDebug() << QString("++ test-2: %1").arg(QString::number((int)i.value()));
		 return i.value();
	 }
	 else
	 {
		 //qDebug() << QString("++ test-3: %1").arg(QString::number(INDEFINI));
		 return INDEFINI;
	 }
}


int YerothHistoriqueStock::get_id_mouvement_stock(QString aStockHistory)
{
	int id = aStockHistory.split(YerothHistoriqueStock::SEPARATION_INTERNE).at(3).toInt();

	return id;
}


QDate YerothHistoriqueStock::get_date_mouvement_stock(QString aStockHistory)
{
	QString date = aStockHistory.split(YerothHistoriqueStock::SEPARATION_INTERNE).first();

	return GET_DATE_FROM_STRING(date);
}

