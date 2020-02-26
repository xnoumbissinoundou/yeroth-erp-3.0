/*
 * yeroth-erp-search-form.hpp
 *
 *      Author: Xavier NOUMBISSI NOUNDOU, Dipl.-Inf., Ph.D. (ABD) (Ph.D. (ABD))
 */

#ifndef SRC_YEROTH_SEARCH_FORM_HPP_
#define SRC_YEROTH_SEARCH_FORM_HPP_

#include "../../ui_yeroth-erp-search-form.h"

#include <QtWidgets/QDialog>

class YerothERPWindows;

class YerothSqlTableModel;
class YerothLogger;

class YerothSearchForm : public QDialog, private Ui_YerothSearchForm
{
    Q_OBJECT

public:

	YEROTH_CLASS_OPERATORS

	YerothSearchForm(YerothERPWindows *allWindows,
					 YerothWindowsCommons &aCallingWindow,
					 YerothSqlTableModel *sqlTable,
					 QWidget *parent = 0);

    ~YerothSearchForm();

    inline void setSqlTableModel(YerothSqlTableModel *sqlTable)
    {
    	_sqlTable = sqlTable;
    }

public slots:

	void performSearch();

	void reinitialiser();

	inline const QString getSearchFilter() const
	{
		return *_searchFilter;
	}

    inline void rendreInvisible()
    {
    	this->close();
    }

    void rendreVisible();

    inline void setLastStringRecherche1()
    {
    	_lastStringRecherche1 = lineEdit_recherche_1->text();
    }

    inline void setLastStringRecherche2()
    {
    	_lastStringRecherche2 = lineEdit_recherche_2->text();
    }

    inline void setLastStringRecherche3()
    {
    	_lastStringRecherche3 = lineEdit_recherche_3->text();
    }

private:

    void setupLineEditsQCompleters();

	void clear_all_fields();

	static const QString 	_WINDOW_TITLE;

	YerothLogger				*_logger;

	QString					_lastStringRecherche1;
	QString					_lastStringRecherche2;
	QString					_lastStringRecherche3;
	QString 				*_searchFilter;
	YerothSqlTableModel 		*_sqlTable;
    YerothERPWindows 			*_allWindows;
};


#endif /* SRC_YEROTH_SEARCH_FORM_HPP_ */
