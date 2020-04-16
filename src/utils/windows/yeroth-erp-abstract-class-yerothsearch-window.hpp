/*
 * yeroth-erp-abstract-class-yerothsearch-window.hpp
 *
 *      Author: Xavier NOUMBISSI NOUNDOU, Dipl.-Inf., Ph.D. (ABD)
 */

#ifndef SRC_UTILS_WINDOWS_YEROTH_ERP_ABSTRACT_CLASS_YEROTHSEARCH_WINDOW_HPP_
#define SRC_UTILS_WINDOWS_YEROTH_ERP_ABSTRACT_CLASS_YEROTHSEARCH_WINDOW_HPP_


#include "src/widgets/yeroth-erp-line-edit.hpp"


#include <QtCore/QObject>

#include <QtCore/QMap>

#include <QtCore/QString>


class YerothLineEdit;


class YerothAbstractClassYerothSearchWindow
{

public:

	YEROTH_CLASS_OPERATORS

	inline YerothAbstractClassYerothSearchWindow(QString aDBYerothSqlTableName)
	:_currentlyFiltered(false),
	 _yerothSqlTableModel(0),
	 _dbYerothSqlTableName(aDBYerothSqlTableName)
	{
	}

	virtual ~YerothAbstractClassYerothSearchWindow()
	{
	}

protected:

	inline virtual void setYerothSqlTableModel(YerothSqlTableModel *aYerothSqlTableModel)
	{
		_yerothSqlTableModel = aYerothSqlTableModel;
	}

	inline virtual void clearSearchFilter()
	{
		_searchFilter.clear();
	}

    inline virtual bool isCurrentlyFiltered()
    {
    	return _currentlyFiltered;
    }

	virtual void setCurrentlyFiltered(bool currentlyFiltered);

	virtual void clearLineEditsQCompleters();

	virtual void setupLineEditsQCompleters(QObject *aThis);

	virtual void resetLineEditsQCompleters(QObject *aThis);

	inline virtual void set_filtrer_font()
	{
	}

	virtual void textChangedSearchLineEditsQCompleters() = 0;


    bool								_currentlyFiltered;

	QString								_searchFilter;

	QString								_dbYerothSqlTableName;

	YerothSqlTableModel 				*_yerothSqlTableModel;

	QMap<YerothLineEdit **, QString> 	_lineEditsToANDContentForSearch;
};


#endif /* SRC_UTILS_WINDOWS_YEROTH_ERP_ABSTRACT_CLASS_YEROTHSEARCH_WINDOW_HPP_ */
