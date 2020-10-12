/*
 * yeroth-erp-creer-nouveau-client-window.hpp
 *
 *      Author: DIPL.-INF. XAVIER NOUMBISSI NOUNDOU
 */

#ifndef SRC_YEROTH_CREER_NOUVEAU_CLIENT_WINDOW_HPP_
#define SRC_YEROTH_CREER_NOUVEAU_CLIENT_WINDOW_HPP_

# include "../../ui_yeroth-erp-creer-nouveau-client-window.h"

# include <QtWidgets/QMessageBox>

# include <QtCore/QDebug>

#include "src/utils/yeroth-erp-logger.hpp"

#include "yeroth-erp-window-commons.hpp"

class YerothERPWindows;
class QStandardItemModel;
class YerothUtils;
class YerothLogger;
class YerothSqlTableModel;

class YerothCreerNouveauClientWindow : public YerothWindowsCommons, private Ui_YerothCreerNouveauClientWindow
{
    Q_OBJECT

public:

	YEROTH_CLASS_OPERATORS

    YerothCreerNouveauClientWindow();

    inline ~YerothCreerNouveauClientWindow()
    {
    	delete _logger;
    }

    inline QToolBar * getQMainWindowToolBar()
    {
    	return toolBar_creerNouveauClientWindow;
    }

    virtual void definirCaissier();

    virtual void definirManager();

    virtual void definirVendeur();

    virtual void definirGestionaireDesStocks();

    virtual void definirMagasinier();

    virtual void definirPasDeRole();

    virtual void rendreVisible(YerothSqlTableModel *stocksTableModel);

    virtual void rendreInvisible();

public slots:

	inline virtual void apropos()
	{
		YerothWindowsCommons::apropos(this);
	}

	inline virtual void help()
	{
		YerothQMessageBox::information(this,
							 QObject::trUtf8("aide"),
							 QObject::trUtf8("Sélectionner un produit, et ensuite cliquer "
											 "sur l'opération que vous souhaitez réaliser!"));
	}

	virtual void alertes();

	virtual void ventes();

    virtual void menu();

    virtual void administration();

    void annuler();

    void valider();

protected:

    bool creer_client();

    bool customerAccountExist();

    bool creer_client_check_fields();

    void clear_client_all_fields();

    virtual void setupShortcuts();

private:

    YerothLogger				*_logger;
};

#endif /* SRC_YEROTH_CREER_NOUVEAU_CLIENT_WINDOW_HPP_ */
