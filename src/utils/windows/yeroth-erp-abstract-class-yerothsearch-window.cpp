/*
 * yeroth-erp-abstract-class-yerothsearch-window.cpp
 *
 *      Author: Xavier NOUMBISSI NOUNDOU, Dipl.-Inf., Ph.D. (ABD)
 */

#include "yeroth-erp-abstract-class-yerothsearch-window.hpp"

#include "src/utils/yeroth-erp-database-table-column.hpp"



void YerothAbstractClassYerothSearchWindow::setCurrentlyFiltered(bool currentlyFiltered)
{
	_currentlyFiltered = currentlyFiltered;

	set_filtrer_font();
}


void YerothAbstractClassYerothSearchWindow::clearLineEditsQCompleters()
{
	QMapIterator <YerothLineEdit **, QString> it(_lineEditsToANDContentForSearch);

    YerothLineEdit *aYerothLineEdit = 0;

    while (it.hasNext())
    {
        it.next();

        aYerothLineEdit = *it.key();

    	if (0 != aYerothLineEdit)
    	{
    		aYerothLineEdit->clear();
    	}
    }
}


void YerothAbstractClassYerothSearchWindow::setupLineEditsQCompleters(QObject *aThis,
																	  QString aConditionStr)
{
	QMapIterator <YerothLineEdit **, QString> it(_lineEditsToANDContentForSearch);

    YerothLineEdit *aYerothLineEdit = 0;

    QString correspondingDBFieldKeyValue;

    while (it.hasNext())
    {
        it.next();

        aYerothLineEdit = *it.key();

        correspondingDBFieldKeyValue = it.value();

    	if (0 != aYerothLineEdit)
    	{
    		if (!correspondingDBFieldKeyValue.isEmpty())
    		{
    			if (!YerothUtils::isEqualCaseInsensitive(correspondingDBFieldKeyValue,
    													 YerothDatabaseTableColumn::REFERENCE))
    			{
        			aYerothLineEdit->
    					setupMyStaticQCompleter(_dbYerothSqlTableName,
    											correspondingDBFieldKeyValue,
												false,
												true,
												aConditionStr);
    			}
    			else
    			{
        			aYerothLineEdit->
    					setupMyStaticQCompleter(_dbYerothSqlTableName,
    											correspondingDBFieldKeyValue,
												false,
												false);
    			}
    		}

    		QObject::connect(aYerothLineEdit,
    						 SIGNAL(textChanged(const QString &)),
							 aThis,
							 SLOT(textChangedSearchLineEditsQCompleters()));
    	}
    }
}


void YerothAbstractClassYerothSearchWindow::resetLineEditsQCompleters(QObject *aThis)
{
	_searchFilter.clear();

	if (0 != _yerothSqlTableModel)
	{
		_yerothSqlTableModel->resetFilter();
	}

    clearLineEditsQCompleters();

    setupLineEditsQCompleters(aThis);
}
