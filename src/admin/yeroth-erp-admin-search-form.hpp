/*
 * yeroth-erp-admin-search-form.hpp
 *
 *      Author: Xavier NOUMBISSI NOUNDOU, Dipl.-Inf., Ph.D. (ABD)
 */

#ifndef SRC_YEROTH_ADMIN_SEARCH_FORM_HPP_
#define SRC_YEROTH_ADMIN_SEARCH_FORM_HPP_

#include "../../ui_yeroth-erp-admin-search-form.h"

#include <QtWidgets/QDialog>

#include "src/admin/admin-actions-subjects.hpp"

class YerothERPWindows;

class YerothSqlTableModel;
class YerothLogger;

class YerothAdminSearchForm : public QDialog, private Ui_YerothAdminSearchForm
{
    Q_OBJECT

public:

	YEROTH_CLASS_OPERATORS

	YerothAdminSearchForm(YerothERPWindows *allWindows,
						 QWidget *parent);

    ~YerothAdminSearchForm();

public slots:

	inline void setCurSqlTableModel(YerothSqlTableModel *aCurSqlTableModel)
	{
		_curSqlTableModel = aCurSqlTableModel;
	}

	inline YerothSqlTableModel *getCurSqlTableModel()
	{
		return _curSqlTableModel;
	}

	void reinitialiser();

	void rechercher(const QString &itemName);

    void rendreVisible(int tabWidjetListerIdx);

    void rendreInvisible();

protected:

    void hideEvent(QHideEvent * hideEvent);

private:

    inline void clear_all_fields()
    {
    	lineEdit_terme_recherche->clear();
    }

    void setupLineEditsQCompleters(int tabWidjetListerIdx);


    enum AdminSujetAction	_curSujetAction;
    YerothSqlTableModel  	*_curSqlTableModel;

    YerothLogger				*_logger;
    YerothERPWindows 			*_allWindows;
};


#endif /* SRC_YEROTH_ADMIN_SEARCH_FORM_HPP_ */
