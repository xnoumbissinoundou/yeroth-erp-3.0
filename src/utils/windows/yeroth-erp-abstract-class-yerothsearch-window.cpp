/*
 * yeroth-erp-abstract-class-yerothsearch-window.cpp
 *
 *      Author: DR. XAVIER NOUMBISSI NOUNDOU
 */

#include "yeroth-erp-abstract-class-yerothsearch-window.hpp"

#include "src/windows/yeroth-erp-window-commons.hpp"

#include "src/utils/yeroth-erp-database-table-column.hpp"


YerothAbstractClassYerothSearchWindow::~YerothAbstractClassYerothSearchWindow()
{
	 _yeroth_WINDOW_QComboBox_SearchDBFieldColumnString = 0;

	 _yeroth_WINDOW_QLineEdit_SearchDBFieldColumnString = 0;
}


void YerothAbstractClassYerothSearchWindow::setupLineEditsQCompleters(QObject *aThis,
																	  QString aConditionStr)
{
    QString correspondingDBFieldKeyValue;

    {
    	QMapIterator <YerothLineEdit **, QString> it(_lineEditsToANDContentForSearch);

    	YerothLineEdit *aYerothLineEdit = 0;

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
								true,
								aConditionStr);
    				}
    				else
    				{
    					aYerothLineEdit->
						setupMyStaticQCompleter(_dbYerothSqlTableName,
								correspondingDBFieldKeyValue,
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

    {
    	YerothComboBox *aYerothComboBox = 0;

    	QMapIterator <YerothComboBox **, QString> it(_comboBoxesToANDContentForSearch);

    	while (it.hasNext())
    	{
    		it.next();

    		aYerothComboBox = *it.key();

    		if (0 != aYerothComboBox)
    		{
    			QObject::connect(aYerothComboBox,
    		    				 SIGNAL(currentTextChanged(const QString &)),
								 aThis,
								 SLOT(textChangedSearchLineEditsQCompleters()));
    		}
    	}
    }

    {
    	if (0 != _yeroth_WINDOW_QComboBox_SearchDBFieldColumnString &&
    		0 != _yeroth_WINDOW_QLineEdit_SearchDBFieldColumnString)
    	{
    		((YerothWindowsCommons *)aThis)
    				->updateYerothLineEditQCompleter(_yeroth_WINDOW_QComboBox_SearchDBFieldColumnString->currentText());

    		QObject::connect(_yeroth_WINDOW_QComboBox_SearchDBFieldColumnString,
    						 SIGNAL(currentTextChanged(const QString &)),
							 aThis,
							 SLOT(updateYerothLineEditQCompleter(const QString &)));

    		QObject::connect(_yeroth_WINDOW_QLineEdit_SearchDBFieldColumnString,
    						 SIGNAL(textChanged(const QString &)),
							 aThis,
							 SLOT(textChangedSearchLineEditsQCompleters()));
    	}
    }
}


void YerothAbstractClassYerothSearchWindow::setCurrentlyFiltered(bool currentlyFiltered)
{
	_currentlyFiltered = currentlyFiltered;

	set_filtrer_font();
}


void YerothAbstractClassYerothSearchWindow::clearLineEditsQCompleters()
{
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

	{
		QMapIterator <YerothComboBox **, QString> it(_comboBoxesToANDContentForSearch);

		YerothComboBox *aYerothComboBox = 0;

		while (it.hasNext())
		{
			it.next();

			aYerothComboBox = *it.key();

			if (0 != aYerothComboBox)
			{
				aYerothComboBox->resetYerothComboBox();
			}
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
