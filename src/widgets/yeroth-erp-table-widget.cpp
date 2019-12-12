/*
 * yeroth-erp-list-widget.cpp
 *
 *  Created on: Mar 21, 2016
 *      Author: Dipl.-Inf. Xavier NOUMBISSI NOUNDOU, Ph.D. (ABD)
 *      Email:  xnoundou7@gmail.com
 */

#include "yeroth-erp-table-widget.hpp"

#include "src/utils/yeroth-erp-logger.hpp"

#include "src/widgets/yeroth-erp-line-edit.hpp"

#include <QtCore/QDebug>

const unsigned int YerothTableWidget::REFERENCE_COLUMN(0);

const unsigned int YerothTableWidget::DESIGNATION_COLUMN(1);

const unsigned int YerothTableWidget::CATEGORIE_COLUMN(2);

const unsigned int YerothTableWidget::TOTAL_TVA_COLUMN(6);

const unsigned int YerothTableWidget::TOTAL_COLUMN(7);

const unsigned int YerothTableWidget::QTE_COLUMN(3);

const unsigned int YerothTableWidget::QTE_TOTAL_STOCK(4);

const QString YerothTableWidget::QTE_1("1");

YerothTableWidget::YerothTableWidget(QWidget *parent)
    :QTableWidget(parent),
     _referenceItem(0),
     _designationItem(0),
     _categorieItem(0),
     _prixUnitaireItem(0),
     _montantTvaItem(0),
     _prixVenteItem(0),
     _qteItem(0),
     _qteTotalStockItem(0),
     _logger(0)
{
    _curRow = 0;

    _logger = new YerothLogger("YerothTableWidget");

    _logger->log("YerothTableWidget");

    this->setFrameShadow(QFrame::Plain);
    this->setEditTriggers(QAbstractItemView::CurrentChanged);
    this->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->setSelectionMode(QAbstractItemView::SingleSelection);
}

YerothTableWidget::~YerothTableWidget()
{
    delete _logger;
}

void YerothTableWidget::setQStandardItemFlags(Qt::ItemFlags &flags)
{
    _referenceItem->setFlags			(flags);
    _designationItem->setFlags		(flags);
    _categorieItem->setFlags		(flags);
    _prixUnitaireItem->setFlags		(flags);
    _montantTvaItem->setFlags		(flags);
    _prixVenteItem->setFlags		(flags);
    _qteTotalStockItem->setFlags	(flags);
}

void YerothTableWidget::myClear()
{
    QTableWidget::clearContents();

    _mapListIdxToItems.clear();

    _curRow = 0;

    this->setRowCount(0);
}

int YerothTableWidget::addArticleAfter(YerothArticleVenteInfo &articleVenteInfo)
{
    _logger->log("addArticleAfter(YerothArticleVenteInfo &)");

    this->setRowCount(_curRow + 1);

    double prix_unitaire = articleVenteInfo.prix_unitaire;

    if (articleVenteInfo.remise_prix > 0)
    {
        prix_unitaire = articleVenteInfo.prix_unitaire - articleVenteInfo.remise_prix;
    }

    _referenceItem 		= new YerothQTableWidgetItem(articleVenteInfo.reference);
    _designationItem 	= new YerothQTableWidgetItem(articleVenteInfo.designation);
    _categorieItem 		= new YerothQTableWidgetItem(articleVenteInfo.categorie);
    _prixUnitaireItem 	= new YerothQTableWidgetItem(QString::number(prix_unitaire, 'f', 2));
    _montantTvaItem 	= new YerothQTableWidgetItem(articleVenteInfo.montantTva());
    _prixVenteItem 		= new YerothQTableWidgetItem(GET_CURRENCY_STRING_NUM(articleVenteInfo.prix_vente()));
    _qteItem 			= new YerothQTableWidgetItem(articleVenteInfo.quantiteAVendre());
    _qteTotalStockItem 	= new YerothQTableWidgetItem(articleVenteInfo.quantiteEnStock());

    Qt::ItemFlags qStandardItemFlags = Qt::ItemIsSelectable | Qt::ItemIsEnabled;

    setQStandardItemFlags(qStandardItemFlags);

    _mapListIdxToItems.insert(_curRow, articleVenteInfo.sqlTableModelIndex);

    unsigned idx = 0;

    //Each call to setItem triggers a call to YerothPointDeVenteWindow::handleQteChange
    this->setItem(_curRow, idx++, _referenceItem);
    this->setItem(_curRow, idx++, _designationItem);
    this->setItem(_curRow, idx++, _categorieItem);
    this->setItem(_curRow, idx++, _qteItem);
    this->setItem(_curRow, idx++, _qteTotalStockItem);
    this->setItem(_curRow, idx++, _prixUnitaireItem);
    this->setItem(_curRow, idx++, _montantTvaItem);
    this->setItem(_curRow, idx,   _prixVenteItem);

    setColoursQStandardItem();

    this->resizeColumnsToContents();

    int lastCurRow = _curRow;

    ++_curRow;

    emit addedYerothTableWidget();

    return lastCurRow;
}

int YerothTableWidget::addArticle(int stocksSqlTableModelIndex,
                                 QString reference,
                                 QString designation,
                                 QString categorie,
                                 QString prix_unitaire,
                                 QString montant_tva,
                                 QString prix_vente,
                                 QString quantite_a_vendre,
                                 QString quantite_total_stock)
{
    _logger->log("addArticle");

    this->setRowCount(_curRow + 1);

    _referenceItem 		= new YerothQTableWidgetItem(reference);
    _designationItem 	= new YerothQTableWidgetItem(designation);
    _categorieItem 		= new YerothQTableWidgetItem(categorie);
    _prixUnitaireItem 	= new YerothQTableWidgetItem(prix_unitaire);
    _montantTvaItem 	= new YerothQTableWidgetItem(montant_tva);
    _prixVenteItem 		= new YerothQTableWidgetItem(prix_vente);
    _qteItem 			= new YerothQTableWidgetItem(quantite_a_vendre);
    _qteTotalStockItem 	= new YerothQTableWidgetItem(quantite_total_stock);

    Qt::ItemFlags qStandardItemFlags = Qt::ItemIsSelectable | Qt::ItemIsEnabled;

    setQStandardItemFlags(qStandardItemFlags);

    _mapListIdxToItems.insert(_curRow, stocksSqlTableModelIndex);

    unsigned idx = 0;

    //Each call to setItem triggers a call to YerothPointDeVenteWindow::handleQteChange
    this->setItem(_curRow, idx++, _referenceItem);
    this->setItem(_curRow, idx++, _designationItem);
    this->setItem(_curRow, idx++, _categorieItem);
    this->setItem(_curRow, idx++, _qteItem);
    this->setItem(_curRow, idx++, _qteTotalStockItem);
    this->setItem(_curRow, idx++, _prixUnitaireItem);
    this->setItem(_curRow, idx++, _montantTvaItem);
    this->setItem(_curRow, idx,   _prixVenteItem);

    setColoursQStandardItem();

    this->resizeColumnsToContents();

    int lastCurRow = _curRow;

    ++_curRow;

    emit addedYerothTableWidget();

    return lastCurRow;
}

void YerothTableWidget::removeArticle(int tableWidgetRow)
{
    _logger->log("removeArticle(int)");

    this->removeRow(tableWidgetRow);

    if (0 == this->rowCount())
    {
        emit emptiedYerothTableWidget();
    }

    _curRow = _curRow - 1;

    QMap<int, int> newMapListIdxToItems;

    QMapIterator<int, int> i(_mapListIdxToItems);

    int k = 0;
    while (i.hasNext())
    {
        i.next();

        if (k != tableWidgetRow)
        {
            newMapListIdxToItems.insert(k, i.value());

            _logger->debug("enlever_article",
                           QString("key: %1, value: %2").arg(QString::number(k), QString::number(i.value())));
        }

        ++k;
    }

    _mapListIdxToItems.clear();
    _mapListIdxToItems = newMapListIdxToItems;
}
