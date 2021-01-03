/*
 * yeroth-erp-stock-detail-window.hpp
 *
 *      Author: DR. XAVIER NOUMBISSI NOUNDOU
 */

# ifndef SRC_WINDOWS_YEROTH_ACHATS_DETAIL_WINDOW_HPP_
# define SRC_WINDOWS_YEROTH_ACHATS_DETAIL_WINDOW_HPP_

# include "../../../ui_yeroth-erp-achats-detail-window.h"

#include "src/utils/yeroth-erp-logger.hpp"

# include <QtWidgets/QMessageBox>
#include "src/windows/yeroth-erp-window-commons.hpp"

class QContextMenuEvent;

class YerothERPWindows;
class YerothSqlTableModel;
class YerothLogger;

class YerothAchatsDetailWindow : public YerothWindowsCommons, private Ui_YerothAchatsDetailWindow
{
    Q_OBJECT

public:

	YEROTH_CLASS_OPERATORS

	YerothAchatsDetailWindow();

	inline ~YerothAchatsDetailWindow()
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

    void rendreVisible(YerothSqlTableModel *achatStocksTableModel,
					   YerothSqlTableModel *stocksTableModel);

public slots:

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

    virtual void setupShortcuts();

private:

    void setupLineEdits();

    void showItem();

    void checkCourrierAlerts();

    int 					_achatLastSelectedRow;

    YerothSqlTableModel 	*_curAchatStocksTableModel;

	YerothLogger			*_logger;
};


#endif /* SRC_WINDOWS_YEROTH_ACHATS_DETAIL_WINDOW_HPP_ */
