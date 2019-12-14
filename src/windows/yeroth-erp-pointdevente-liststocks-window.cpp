/*

   * yeroth-erp-pointdevente-liststocks-window.cpp

   *


   *      Author: Xavier NOUMBISSI NOUNDOU, Dipl.-Inf., Ph.D. (ABD)


   */
#include "yeroth-erp-pointdevente-liststocks-window.hpp"


#include "src/utils/yeroth-erp-sqltable-model.hpp"

#include "src/utils/yeroth-erp-config.hpp"

#include "src/users/yeroth-erp-users.hpp"


#include <QtWidgets/QDesktopWidget>

const QString YerothPointDeVenteListStocksWindow::_WINDOW_TITLE(QString(QObject::trUtf8("%1 - %2")).
        arg(YEROTH_ERP_WINDOW_TITLE,
            QObject::trUtf8("stocks à vendre")));

YerothPointDeVenteListStocksWindow::YerothPointDeVenteListStocksWindow()
:YerothWindowsCommons(YerothPointDeVenteListStocksWindow::_WINDOW_TITLE),
 _logger(new YerothLogger("YerothVenteListStocksWindow"))
{
    setupUi(this);

    this->mySetupUi(this);
}


void YerothPointDeVenteListStocksWindow::listStocks(YerothSqlTableModel & aSqlTableModel)
{
    if (YerothUtils::isEqualCaseInsensitive(YerothERPConfig::salesStrategy, YerothERPConfig::STRATEGIE_VENTE_SORTIE_ALL))
    {
        tableView_list_stocks->lister_les_elements_du_tableau(aSqlTableModel);
    }
    else if (YerothUtils::
             isEqualCaseInsensitive(YerothERPConfig::salesStrategy, YerothERPConfig::STRATEGIE_VENTE_SORTIE_FIFO))
    {
        QMap < QString, int >designationToTableRows_in_out;
        //qDebug() << "++ YerothVenteListStocksWindow::listStocks tableView_list_stocks->lister_FIFO";
        tableView_list_stocks->lister_FIFO(aSqlTableModel, designationToTableRows_in_out);
        //qDebug() << designationToTableRows_in_out;
    }
    else if (YerothUtils::
             isEqualCaseInsensitive(YerothERPConfig::salesStrategy, YerothERPConfig::STRATEGIE_VENTE_SORTIE_LIFO))
    {
        QMap < QString, int >designationToTableRows_in_out;
        //qDebug() << "++ YerothVenteListStocksWindow::listStocks tableView_list_stocks->lister_LIFO";
        tableView_list_stocks->lister_LIFO(aSqlTableModel, designationToTableRows_in_out);
        //qDebug() << designationToTableRows_in_out;
    }
    else if (YerothUtils::
             isEqualCaseInsensitive(YerothERPConfig::salesStrategy, YerothERPConfig::STRATEGIE_VENTE_SORTIE_DEF_DEO))
    {
        QMap < QString, int >designationToTableRows_in_out;
        tableView_list_stocks->lister_DEF_DEO(aSqlTableModel, designationToTableRows_in_out);
    }

    tableView_list_stocks->hideColumn(4);
    tableView_list_stocks->hideColumn(9);
    tableView_list_stocks->hideColumn(11);
    tableView_list_stocks->hideColumn(12);
    tableView_list_stocks->hideColumn(13);
    tableView_list_stocks->hideColumn(14);
    tableView_list_stocks->hideColumn(15);
    tableView_list_stocks->hideColumn(16);
    tableView_list_stocks->hideColumn(17);
    tableView_list_stocks->hideColumn(18);
    tableView_list_stocks->hideColumn(19);
    tableView_list_stocks->hideColumn(20);

    setWindowTitle(YerothUtils::getWindowTitleWithStrategy(this));

    show();
}
