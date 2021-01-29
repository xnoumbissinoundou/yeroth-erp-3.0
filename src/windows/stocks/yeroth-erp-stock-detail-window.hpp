/*
 * yeroth-erp-stock-detail-window.hpp
 *
 *      Author: DR. XAVIER NOUMBISSI NOUNDOU
 */

#ifndef SRC_WINDOWS_YEROTH_STOCK_DETAIL_WINDOW_HPP_
#define SRC_WINDOWS_YEROTH_STOCK_DETAIL_WINDOW_HPP_

#include "../../../ui_yeroth-erp-stock-detail-window.h"


#include "src/utils/yeroth-erp-logger.hpp"

#include "src/utils/windows/yeroth-erp-abstract-class-detail-view-window.hpp"


#include <QtWidgets/QMessageBox>


class QContextMenuEvent;

class YerothERPWindows;
class YerothSqlTableModel;
class YerothLogger;

class YerothStockDetailWindow : public YerothWindowsCommons,
								private Ui_YerothStockDetailWindow,
								public YerothERPAbstractClassDetailViewFOR_WINDOW
{
    Q_OBJECT

public:

	YEROTH_CLASS_OPERATORS

	YerothStockDetailWindow();

	inline ~YerothStockDetailWindow()
	{
		delete _logger;
	}

	inline QToolBar * getQMainWindowToolBar()
	{
		return toolBar_detailWindow;
	}

	void definirPasDeRole();

    void definirCaissier();

    void definirManager();

    virtual void definirVendeur();

    virtual void definirGestionaireDesStocks();

    void definirMagasinier();

    void rendreInvisible();

    void rendreVisible(YerothSqlTableModel *stocksTableModel = 0);

public slots:

    virtual void modifier_les_articles();

    void supprimer_ce_stock();

	inline virtual void apropos()
	{
		YerothWindowsCommons::apropos(this);
	}

	inline virtual void help()
	{
		YerothQMessageBox::information(this,
						 QObject::trUtf8("aide"),
						 QObject::trUtf8("Vous pouvez retourner à la page précédente en "
										 "cliquant sur 'Retour'!"));
	}

protected:

    virtual void contextMenuEvent(QContextMenuEvent *event);

    virtual void setupShortcuts();

private:

    void setupLineEdits();

    void setStockAchatValuesVisibility(bool aVisibility);

    void setStockSpecificWidgetVisibility(bool aVisibility);

    virtual void showItem_YerothERPAbstractClassDetailViewFOR_WINDOW();

    void checkCourrierAlerts();

    YerothLogger	*_logger;
};


#endif /* SRC_WINDOWS_YEROTH_STOCK_DETAIL_WINDOW_HPP_ */
