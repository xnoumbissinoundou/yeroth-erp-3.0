/*
 * yeroth-erp-detail-window.hpp
 *
 *  Created on: Oct 18, 2015
 *      Author: Dipl.-Inf. Xavier NOUMBISSI NOUNDOU, Ph.D. (ABD)
 *      Email:  xnoundou7@gmail.com
 */

# ifndef SRC_WINDOWS_YEROTH_DETAIL_WINDOW_HPP_
# define SRC_WINDOWS_YEROTH_DETAIL_WINDOW_HPP_

# include "../../ui_yeroth-erp-detail-window.h"

#include "src/utils/yeroth-erp-logger.hpp"

# include <QtWidgets/QMessageBox>
#include "yeroth-erp-window-commons.hpp"

class QContextMenuEvent;

class YerothERPWindows;
class YerothSqlTableModel;
class YerothLogger;

class YerothDetailWindow : public YerothWindowsCommons, private Ui_YerothDetailWindow
{
    Q_OBJECT

public:

	YEROTH_CLASS_OPERATORS

	YerothDetailWindow();

	inline ~YerothDetailWindow()
	{
		delete _logger;
	}

	inline QToolBar * getQMainWindowToolBar()
	{
		return toolBar_detailWindow;
	}

    void definirCaissier();

    void definirManager();

    virtual void definirVendeur();

    virtual void definirGestionaireDesStocks();

    void definirMagasinier();

    void definirPasDeRole();

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

    void contextMenuEvent(QContextMenuEvent *event);

    virtual void setupShortcuts();

private:

    void setupLineEdits();

    void showItem();

    void checkCourrierAlerts();

    static const QString 	_WINDOW_TITLE;

    YerothLogger				*_logger;

    static const unsigned int INDEX_ZERO;
};


#endif /* SRC_WINDOWS_YEROTH_DETAIL_WINDOW_HPP_ */
