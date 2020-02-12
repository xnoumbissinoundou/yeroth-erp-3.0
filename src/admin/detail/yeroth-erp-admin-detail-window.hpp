/*
 * yeroth-erp-admin-detail-window.hpp
 *
 *      Author: Xavier NOUMBISSI NOUNDOU, Dipl.-Inf., Ph.D. (ABD)
 */

#ifndef YEROTH_ADMIN_DETAIL_WINDOW_HPP_
#define YEROTH_ADMIN_DETAIL_WINDOW_HPP_

# include "../../../ui_yeroth-erp-admin-detail-window.h"

# include "src/admin/yeroth-erp-admin-windows-commons.hpp"

# include "src/admin/admin-actions-subjects.hpp"

# include <QtWidgets/QMessageBox>

class YerothERPWindows;
class YerothLogger;

class YerothAdminDetailWindow : public YerothPOSAdminWindowsCommons, private Ui_YerothAdminDetailWindow
{
    Q_OBJECT

public:

	YEROTH_CLASS_OPERATORS

    YerothAdminDetailWindow();

    inline ~YerothAdminDetailWindow()
    {
    	delete _logger;
    }

    inline virtual QToolBar * getQMainWindowToolBar()
    {
    	return toolBar_adminDetailWindow;
    }

    virtual void definirPasDeRole();

    virtual void definirAdministrateur();

    virtual void definirManager();

    void rendreVisibleCompteUtilisateur(int sqlTableRow);

    void rendreVisibleLocalisation(int sqlTableRow);

    void rendreVisibleCategorie(int sqlTableRow);

    void rendreVisibleFournisseur(int sqlTableRow);

    void rendreVisibleRemise(int sqlTableRow);

    void rendreVisibleAlerte(int sqlTableRow);

    virtual void rendreInvisible();

public slots:

    void creer();

    void lister();

    void modifier();

    void supprimer();

    void retourListerCompteUtilisateur();

    void retourListerLocalisation();

    void retourListerCategorie();

    void retourListerFournisseur();

    void retourListerAlerte();

private:

    void enableOtherTabs(enum AdminSujetAction curAction, bool enabled);

    void setupLineEdits();

    void clear_set_edit_comboBoxes();

    void clear_set_edit_utilisateur_comboBoxes();

    void clear_set_edit_alerte_comboBoxes();

    YerothLogger		*_logger;
};

#endif /* YEROTH_ADMIN_DETAIL_WINDOW_HPP_ */
