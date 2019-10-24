/*
 * yeroth-erp-alertes-window.hpp
 *
 *  Created on: Jan 1, 2016
 *      Author: Dipl.-Inf. Xavier NOUMBISSI NOUNDOU, Ph.D. (ABD)
 *      Email:  xnoundou7@gmail.com
 */

# ifndef SRC_YEROTH_ALERTES_WINDOW_HPP_
# define SRC_YEROTH_ALERTES_WINDOW_HPP_

# include "../../ui_yeroth-erp-alertes-window.h"

# include <QtWidgets/QMessageBox>
#include "yeroth-erp-window-commons.hpp"

class YerothLogger;
class YerothERPWindows;
class YerothSqlTableModel;

class YerothAlertesWindow : public YerothWindowsCommons, private Ui_YerothAlertesWindow
{
    Q_OBJECT

	enum TabIndex
	{
		TableauDesAlertes = 0,
		AfficherAlerteAuDetail
	};

public:

    YEROTH_CLASS_OPERATORS

    YerothAlertesWindow();

    virtual ~YerothAlertesWindow();

    inline virtual QToolBar * getQMainWindowToolBar()
    {
    	return toolBar_alertesWindow;
    }

    virtual void definirCaissier();

    virtual void definirManager();

    virtual void definirVendeur();

    virtual void definirGestionaireDesStocks();

    virtual void definirMagasinier();

    virtual void definirPasDeRole();

    void rendreVisible(YerothSqlTableModel *stocksTableModel);

    void rendreInvisible();

public slots:

	inline virtual void apropos()
	{
		YerothWindowsCommons::apropos(this);
	}

	inline virtual void help()
	{
		YerothQMessageBox::information(this,
						 QObject::trUtf8("aide"),
						 QObject::trUtf8("1) Sélectionez l'alerte que vous souhaitez résoudre!\n"
								   	   	 "2) Résolvez l'alerte en exécutant le message d'alerte!\n"
								   	   	 "3) Cliquez sur le bouton 'marquer résolue' lorsque "
								       	 "vous avez terminer!"));
	}

    void handleCurrentChanged(int index);

    void retourAlertes();

    void afficher();

    void lister_alertes();

    void afficher_au_detail();

    void marquer_resolue();

    void lister_les_alertes();

    void supprimer();

    virtual void deconnecter_utilisateur();

    inline int getLastListerSelectedRow()
    {
    	return tableView_lister_alertes->lastSelectedRow();
    }

    inline void setLastListerSelectedRow(int row)
    {
    	tableView_lister_alertes->setLastSelectedRow(row);
    }

protected:

    virtual void setupShortcuts();

private:

    void setupLineEdits();

    void clear_all_fields();

    static const QString 	_WINDOW_TITLE;

    YerothLogger 			*_logger;

    int 					_currentTabView;
};


#endif /* SRC_YEROTH_ALERTES_WINDOW_HPP_ */
