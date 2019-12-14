/**
 * yeroth-erp-admin-lister-window.hpp
 *
 *      Author: Xavier NOUMBISSI NOUNDOU, Dipl.-Inf., Ph.D. (ABD)
 */

# ifndef YEROTH_ADMIN_LISTER_WINDOW_HPP_
# define YEROTH_ADMIN_LISTER_WINDOW_HPP_

# include "../../../ui_yeroth-erp-admin-lister-window.h"

# include "src/admin/yeroth-erp-admin-windows-commons.hpp"

# include "src/admin/admin-actions-subjects.hpp"

# include <QtWidgets/QMessageBox>

class QStandardItemModel;

class YerothERPWindows;
class YerothUtils;
class YerothLogger;
class YerothAdminSearchForm;

class YerothAdminListerWindow : public YerothPOSAdminWindowsCommons, private Ui_YerothAdminListerWindow
{
    Q_OBJECT

public:

	YEROTH_CLASS_OPERATORS

    YerothAdminListerWindow();

    ~YerothAdminListerWindow();

    inline virtual QToolBar * getQMainWindowToolBar()
    {
    	return toolBar_adminListerWindow;
    }

    virtual void definirPasDeRole();

    virtual void definirAdministrateur();

    virtual void definirManager();

    void self_reset_view(int currentIndex);

    void rendreVisible(unsigned selectedSujetAction);

    inline void setCurSearchSqlTableModel(YerothSqlTableModel *aSqlTableModel)
	{
    	_curSearchSqlTableModel = aSqlTableModel;
	}

    inline YerothSqlTableModel *getCurSearchSqlTableModel()
	{
    	return _curSearchSqlTableModel;
	}

public slots:

    void handleCurrentChanged(int index);

    void creer();

    void modifier();

	void lister_utilisateur(YerothSqlTableModel *aSqlTableModel = 0);

	void lister_localisation(YerothSqlTableModel *aSqlTableModel = 0);

	void lister_categorie(YerothSqlTableModel *aSqlTableModel = 0);

	void lister_client(YerothSqlTableModel *aSqlTableModel = 0);

	void lister_fournisseur(YerothSqlTableModel *aSqlTableModel = 0);

	void lister_alerte(YerothSqlTableModel *aSqlTableModel = 0);

	void afficher_au_detail();

    void afficher_detail_utilisateur();

    void afficher_detail_localisation();

    void afficher_detail_categorie();

    void afficher_detail_client();

    void afficher_detail_fournisseur();

    void afficher_detail_alerte();

    void supprimer();

    void supprimer_utilisateur();

    void supprimer_localisation();

    void supprimer_categorie();

    void supprimer_client();

    void supprimer_fournisseur();

    void supprimer_alerte();

    inline void annuler()
    {
    	YerothPOSAdminWindowsCommons::menu();
    }

    void rechercher();

    void reinitialiser();

    void set_admin_rechercher_font();

    inline int lastSelectedItemForModification()
    {
    	return _lastItemSelectedForModification;
    }

    void handleItemModification(const QModelIndex &index);

	inline bool isAlertCurrentlyFiltered() const
	{
		return _alertCurrentlyFiltered;
	}

	inline bool isCategoryCurrentlyFiltered() const
	{
		return _categoryCurrentlyFiltered;
	}

	inline bool isCustomerCurrentlyFiltered() const
	{
		return _customerCurrentlyFiltered;
	}

	inline bool isSiteCurrentlyFiltered() const
	{
		return _siteCurrentlyFiltered;
	}

	inline bool isSupplierCurrentlyFiltered() const
	{
		return _supplierCurrentlyFiltered;
	}

	inline bool isUserCurrentlyFiltered() const
	{
		return _userCurrentlyFiltered;
	}

	void setAlertCurrentlyFiltered(bool alertCurrentlyFiltered);

	void setCategoryCurrentlyFiltered(bool categoryCurrentlyFiltered);

	void setCustomerCurrentlyFiltered(bool customerCurrentlyFiltered);

	void setSiteCurrentlyFiltered(bool siteCurrentlyFiltered);

	void setSupplierCurrentlyFiltered(bool supplierCurrentlyFiltered);

	void setUserCurrentlyFiltered(bool userCurrentlyFiltered);

private:

    bool 					_alertCurrentlyFiltered;

    bool 					_categoryCurrentlyFiltered;

    bool 					_customerCurrentlyFiltered;

    bool 					_userCurrentlyFiltered;

    bool 					_supplierCurrentlyFiltered;

    bool 					_siteCurrentlyFiltered;

    QFont					*_pushButton_admin_rechercher_font;

    YerothLogger			*_logger;

    YerothAdminSearchForm 	*_adminSearchForm;

    YerothSqlTableModel		*_curSearchSqlTableModel;

    int 					_lastItemSelectedForModification;
};


#define MACRO_SET_ADMIN_RECHERCHER_FONT(X) 						\
{																\
	if (true == X)												\
	{															\
		_pushButton_admin_rechercher_font->setUnderline(true);	\
	}															\
	else														\
	{															\
		_pushButton_admin_rechercher_font->setUnderline(false);	\
	}															\
}

#endif /* YEROTH_ADMIN_LISTER_WINDOW_HPP_ */
