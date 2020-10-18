/*
 * yeroth-erp-abstract-class-yerothsearch-window.hpp
 *
 *      Author: DIPL.-INF. XAVIER NOUMBISSI NOUNDOU
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

	typedef void (YerothAbstractClassYerothSearchWindow::*fptr)(YerothSqlTableModel &);

	inline YerothAbstractClassYerothSearchWindow(QString aDBYerothSqlTableName)
	:_currentlyFiltered(false),
	 _searchFilter(YerothUtils::EMPTY_STRING),
	 _dbYerothSqlTableName(aDBYerothSqlTableName),
	 _yerothSqlTableModel(0),
	 _yeroth_WINDOW_QComboBox_SearchDBFieldColumnString(0),
	 _yeroth_WINDOW_QLineEdit_SearchDBFieldColumnString(0)
	{
	}

	virtual ~YerothAbstractClassYerothSearchWindow();

public slots:

	virtual void viewYerothTableViewFirstPage(YerothSqlTableModel &curYerothSqlTableModel_IN) = 0;

	virtual void viewYerothTableViewLastPage(YerothSqlTableModel &curYerothSqlTableModel_IN) = 0;

	virtual void viewYerothTableViewPreviousPage(YerothSqlTableModel &curYerothSqlTableModel_IN) = 0;

	virtual void viewYerothTableViewNextPage(YerothSqlTableModel &curYerothSqlTableModel_IN) = 0;

	template<class aYerothMainWindowClassType>
	inline fptr Get__viewYerothTableViewFirstPage__FPointer(aYerothMainWindowClassType *aThisRef)
	{
		return (fptr)&aYerothMainWindowClassType::viewYerothTableViewFirstPage;
	}

	template<class aYerothMainWindowClassType>
	inline fptr Get__viewYerothTableViewLastPage__FPointer(aYerothMainWindowClassType *aThisRef)
	{
		return (fptr)&aYerothMainWindowClassType::viewYerothTableViewLastPage;
	}

	template<class aYerothMainWindowClassType>
	inline fptr Get__viewYerothTableViewPreviousPage__FPointer(aYerothMainWindowClassType *aThisRef)
	{
		return (fptr)&aYerothMainWindowClassType::viewYerothTableViewPreviousPage;
	}

	template<class aYerothMainWindowClassType>
	inline fptr Get__viewYerothTableViewNextPage__FPointer(aYerothMainWindowClassType *aThisRef)
	{
		return (fptr)&aYerothMainWindowClassType::viewYerothTableViewNextPage;
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

	/**
	 * for use with _yeroth_WINDOW_QComboBox_SearchDBFieldColumnString,
	 * and _yeroth_WINDOW_QLineEdit_SearchDBFieldColumnString
	 */
	QSet<QString> 						_yeroth_WINDOW_references_dbColumnString;

	YerothComboBox 						*_yeroth_WINDOW_QComboBox_SearchDBFieldColumnString;

	YerothLineEdit 						*_yeroth_WINDOW_QLineEdit_SearchDBFieldColumnString;

	QMap<YerothComboBox **, QString> 	_comboBoxesToANDContentForSearch;

	QMap<YerothLineEdit **, QString> 	_lineEditsToANDContentForSearch;
};


#endif /* SRC_UTILS_WINDOWS_YEROTH_ERP_ABSTRACT_CLASS_YEROTHSEARCH_WINDOW_HPP_ */
