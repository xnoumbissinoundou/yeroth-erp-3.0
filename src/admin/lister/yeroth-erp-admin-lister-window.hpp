/**
 * yeroth-erp-admin-lister-window.hpp
 *
 *      Author: DR. XAVIER NOUMBISSI NOUNDOU
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

	void lister_compte_bancaire(YerothSqlTableModel *aSqlTableModel = 0);

	void lister_alerte(YerothSqlTableModel *aSqlTableModel = 0);

	void lister_remise(YerothSqlTableModel *aSqlTableModel = 0);

	void afficher_au_detail();

    void afficher_detail_utilisateur();

    void afficher_detail_localisation();

    void afficher_detail_categorie();

    void afficher_detail_compte_bancaire();

    void afficher_detail_remise();

    void afficher_detail_alerte();

    void supprimer();

    void supprimer_utilisateur();

    void supprimer_localisation();

    void supprimer_categorie();

    void supprimer_compte_bancaire();

    void supprimer_remise();

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

	inline bool isBankAccountCurrentlyFiltered() const
	{
		return _bankAccountCurrentlyFiltered;
	}

	inline bool isSiteCurrentlyFiltered() const
	{
		return _siteCurrentlyFiltered;
	}

	inline bool isUserCurrentlyFiltered() const
	{
		return _userCurrentlyFiltered;
	}

	inline bool isDiscountCurrentlyFiltered() const
	{
		return _discountCurrentlyFiltered;
	}

	inline void setDiscountCurrentlyFiltered(bool discountCurrentlyFiltered)
	{
		_discountCurrentlyFiltered = discountCurrentlyFiltered;
	}

	inline void setAlertCurrentlyFiltered(bool alertCurrentlyFiltered)
	{
		_alertCurrentlyFiltered = alertCurrentlyFiltered;
	}

	inline void setCategoryCurrentlyFiltered(bool categoryCurrentlyFiltered)
	{
		_categoryCurrentlyFiltered = categoryCurrentlyFiltered;
	}

	inline void setBankAccountCurrentlyFiltered(bool bankAccountCurrentlyFiltered)
	{
		_bankAccountCurrentlyFiltered = bankAccountCurrentlyFiltered;
	}

	inline void setSiteCurrentlyFiltered(bool siteCurrentlyFiltered)
	{
		_siteCurrentlyFiltered = siteCurrentlyFiltered;
	}

	inline void setUserCurrentlyFiltered(bool userCurrentlyFiltered)
	{
		_userCurrentlyFiltered = userCurrentlyFiltered;
	}

private:

    bool 					_alertCurrentlyFiltered;

    bool 					_categoryCurrentlyFiltered;

    bool 					_bankAccountCurrentlyFiltered;

    bool 					_userCurrentlyFiltered;

    bool 					_siteCurrentlyFiltered;

    bool 					_discountCurrentlyFiltered;

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
