/*
 * yeroth-erp-abstract-class-yerothsearch-window.hpp
 *
 *      Author: Xavier NOUMBISSI NOUNDOU, Dipl.-Inf., Ph.D. (ABD)
 */

#ifndef SRC_UTILS_WINDOWS_YEROTH_ERP_ABSTRACT_CLASS_YEROTHSEARCH_WINDOW_HPP_
#define SRC_UTILS_WINDOWS_YEROTH_ERP_ABSTRACT_CLASS_YEROTHSEARCH_WINDOW_HPP_


#include "src/widgets/yeroth-erp-combo-box.hpp"

#include "src/widgets/yeroth-erp-line-edit.hpp"


#include <QtCore/QObject>

#include <QtCore/QMap>

#include <QtCore/QString>


class YerothComboBox;
class YerothLineEdit;


class YerothAbstractClassYerothSearchWindow
{

public:

	YEROTH_CLASS_OPERATORS

	inline YerothAbstractClassYerothSearchWindow(QString aDBYerothSqlTableName)
	:_currentlyFiltered(false),
	 _searchFilter(YerothUtils::EMPTY_STRING),
	 _dbYerothSqlTableName(aDBYerothSqlTableName),
	 _yerothSqlTableModel(0)
	{
	}

	virtual ~YerothAbstractClassYerothSearchWindow()
	{
	}

protected slots:

	inline virtual void setupLineEditsQCompleters(QObject *aThis)
	{
		setupLineEditsQCompleters(aThis, YerothUtils::EMPTY_STRING);
	}

	virtual void setupLineEditsQCompleters(QObject *aThis,
									   	   QString aConditionStr);

	virtual void textChangedSearchLineEditsQCompleters() = 0;

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

	virtual void resetLineEditsQCompleters(QObject *aThis);

	inline virtual void set_filtrer_font()
	{
	}

    bool								_currentlyFiltered;

	QString								_searchFilter;

	QString								_dbYerothSqlTableName;

	YerothSqlTableModel 				*_yerothSqlTableModel;

	QMap<YerothComboBox **, QString> 	_comboBoxesToANDContentForSearch;

	QMap<YerothLineEdit **, QString> 	_lineEditsToANDContentForSearch;
};


#endif /* SRC_UTILS_WINDOWS_YEROTH_ERP_ABSTRACT_CLASS_YEROTHSEARCH_WINDOW_HPP_ */
