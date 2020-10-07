/*
 * yeroth-erp-pointdevente-liststocks-window.cpp
 *
 *      Author: Dipl.-Inf. XAVIER NOUMBISSI NOUNDOU
 */

#include "yeroth-erp-pointdevente-liststocks-window.hpp"


#include "src/utils/yeroth-erp-sqltable-model.hpp"

#include "src/utils/yeroth-erp-config.hpp"

#include "src/users/yeroth-erp-users.hpp"


#include <QtWidgets/QDesktopWidget>


YerothPointDeVenteListStocksWindow::YerothPointDeVenteListStocksWindow()
:YerothWindowsCommons(),
 _logger(new YerothLogger("YerothVenteListStocksWindow"))
{
    _windowName = QString("%1 - %2")
    				.arg(YEROTH_ERP_WINDOW_TITLE,
    					 QObject::trUtf8("stocks à vendre"));

    setupUi(this);

    mySetupUi(this);

    enableResizing();
}


void YerothPointDeVenteListStocksWindow::listStocks(YerothSqlTableModel & aSqlTableModel)
{
	tableView_PDV_list_stocks->lister_les_elements_du_tableau(aSqlTableModel, YerothERPConfig::salesStrategy);

    if (YerothUtils::isEqualCaseInsensitive(YerothERPConfig::STRATEGIE_VENTE_SORTIE_FEFO,
    										YerothERPConfig::salesStrategy) 				||
    	YerothUtils::isEqualCaseInsensitive(YerothERPConfig::STRATEGIE_VENTE_SORTIE_FIFO,
    										YerothERPConfig::salesStrategy) 				||
		YerothUtils::isEqualCaseInsensitive(YerothERPConfig::STRATEGIE_VENTE_SORTIE_LIFO,
											YerothERPConfig::salesStrategy))
    {
    	tableView_PDV_list_stocks->setSortingEnabled(false);
    }
    else	//YerothConfig::STRATEGIE_VENTE_SORTIE_ALL
    {
    	tableView_PDV_list_stocks->setSortingEnabled(true);
    }

    tableView_PDV_list_stocks->hideColumn(0);
    tableView_PDV_list_stocks->hideColumn(4);
    tableView_PDV_list_stocks->hideColumn(9);
    tableView_PDV_list_stocks->hideColumn(10);
    tableView_PDV_list_stocks->hideColumn(11);
    tableView_PDV_list_stocks->hideColumn(12);
    tableView_PDV_list_stocks->hideColumn(13);
    tableView_PDV_list_stocks->hideColumn(14);
    tableView_PDV_list_stocks->hideColumn(16);
    tableView_PDV_list_stocks->hideColumn(17);
    tableView_PDV_list_stocks->hideColumn(18);
    tableView_PDV_list_stocks->hideColumn(19);
    tableView_PDV_list_stocks->hideColumn(20);
    tableView_PDV_list_stocks->hideColumn(21);
    tableView_PDV_list_stocks->hideColumn(22);
    tableView_PDV_list_stocks->hideColumn(23);

    setWindowTitle(YerothUtils::getWindowTitleWithStrategy(this));

    show();
}
