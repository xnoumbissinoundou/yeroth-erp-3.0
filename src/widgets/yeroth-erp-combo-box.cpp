/*
 * yeroth-erp-combo-box.cpp
 *
 *      Author: PR. XAVIER NOUMBISSI NOUNDOU
 */

#include "yeroth-erp-combo-box.hpp"


#include "src/widgets/yeroth-erp-line-edit.hpp"


#include <QtCore/QDebug>

#include <QtSql/QSqlQuery>


YerothComboBox::YerothComboBox(QWidget *parent)
:QComboBox(parent),
 _populateRawString(true),
 _pointerToUserViewStringMAP(0),
 _originalPaletteBeforeMissingInformation(palette())
{
    setLineEdit(new QLineEdit);

    lineEdit()->setReadOnly(true);

    lineEdit()->setAlignment(Qt::AlignHCenter | Qt::AlignCenter);
}


void YerothComboBox::setupPopulateNORawString(QString aDBTableViewStringName,
		   	   	   	   	   	   	   	   	   	   QString aDBFieldColumn,
											   QMap<int, QString> *pointerToUserViewStringMAP)
{
	_dbTableViewStringName = aDBTableViewStringName;

	_dbFieldColumn = aDBFieldColumn;

	_pointerToUserViewStringMAP = pointerToUserViewStringMAP;

	setPopulateRaw(false);
}


bool YerothComboBox::checkField()
{
    if (currentText().isEmpty())
    {
        setPalette(YerothUtils::YEROTH_RED_PALETTE);

        return false;
    }

    setPalette(_originalPaletteBeforeMissingInformation);

    return true;
}


void YerothComboBox::clearField()
{
    QComboBox::clear();

    setPalette(_originalPaletteBeforeMissingInformation);
}


void YerothComboBox::setYerothEnabled(bool enabled)
{
    QComboBox::setEnabled(enabled);

    YerothUtils::yerothSetWidgetColor(this);
}


void YerothComboBox::saveCurrentValueToDatabase(const QString &aDBFieldColumn,
												QSqlRecord &aQSqlRecordToSAVE)
{
	int resultDatabaseQuery = YerothUtils::getComboBoxDatabaseQueryValue(currentText(),
																		 _pointerToUserViewStringMAP);

	assert(-1 != resultDatabaseQuery);

	aQSqlRecordToSAVE.setValue(aDBFieldColumn, resultDatabaseQuery);

//	qDebug() << QString("++ _dbFieldColumn: %1, valeur: %2")
//								.arg(aDBFieldColumn, QString::number(resultDatabaseQuery));
}


void YerothComboBox::addItems_AS_INITIALIZATION(const QString &current_configured_item,
											    const QString &first_item_to_add,
											    const QStringList &items_to_add_as_CHOICE)
{
    clear();

//    QDEBUG_STRING_OUTPUT_2("first_item_to_add", first_item_to_add);

    addItem(first_item_to_add);

    QString a_matching_item;

    QString a_current_item;

    for (unsigned int k = 0; k < items_to_add_as_CHOICE.size(); ++k)
    {
    	a_current_item = items_to_add_as_CHOICE.at(k);

        if (YerothUtils::isEqualCaseInsensitive(a_current_item, current_configured_item))
        {
        	a_matching_item = a_current_item;
        	break;
        }
    }

//    QDEBUG_STRING_OUTPUT_2("a_matching_item", a_matching_item);

//    QDEBUG_QSTRINGLIST_OUTPUT("items_to_add_as_CHOICE", items_to_add_as_CHOICE);

    QStringList items_to_add_as_CHOICE_without_ALLREADY_ADDED_ITEM(items_to_add_as_CHOICE);

    items_to_add_as_CHOICE_without_ALLREADY_ADDED_ITEM.removeAll(first_item_to_add);

//    QDEBUG_QSTRINGLIST_OUTPUT("items_to_add_as_CHOICE_without_ALLREADY_ADDED_ITEM", items_to_add_as_CHOICE_without_ALLREADY_ADDED_ITEM);

    addItems(items_to_add_as_CHOICE_without_ALLREADY_ADDED_ITEM);
}


void YerothComboBox::addItem_AFTER_POPULATE(const QStringList &texts)
{
	QSet<QString> cur_items;

	for( uint i = 0; i < count(); ++i)
	{
		cur_items.insert(itemText(i));
	}

	for( uint j = 0; j < texts.size(); ++j)
	{
		cur_items.insert(texts.at(j));
	}

	QStringList result_items_sorted = cur_items.toList();

	result_items_sorted.sort(Qt::CaseSensitive);

	clear();

	QComboBox::addItems(result_items_sorted);
}


void YerothComboBox::addItems(const QStringList &texts)
{
	QStringList new_texts(texts);

	new_texts.sort();

	QComboBox::addItems(new_texts);
}


void YerothComboBox::addItems(const QStringList &texts,
							  const QMap<QString, QString> &tableColumnToUserViewString)
{
	QStringList new_texts;

	for (uint i = 0; i < texts.size(); ++i)
	{
		new_texts.append(tableColumnToUserViewString.value(texts.at(i)));
	}

	new_texts.sort();

	QComboBox::addItems(new_texts);
}


bool YerothComboBox::populateComboBoxRawString(QString aDBTableViewStringName,
		   	   	   	   	   	   	   	   	   	   QString aDBFieldColumn,
											   QString aConditionStr /* = YerothUtils::EMPTY_STRING */)
{
	if (!isPopulateRaw())
	{
		return false;
	}

    static QStringList curItems;

    clear();

    QString strQuery;

    if (aConditionStr.isEmpty())
    {
    	strQuery = QString("select %1 from %2")
    	    		 .arg(aDBFieldColumn,
    	    			  aDBTableViewStringName);
    }
    else
    {
    	strQuery = QString("select %1 from %2 where %3")
    	    		 .arg(aDBFieldColumn,
    	    			  aDBTableViewStringName,
						  aConditionStr);
    }


    //qDebug() << "++ query: " << strQuery;
    QSqlQuery query;
    query.prepare(strQuery);
    bool success = query.exec();
    //qDebug() << "[" << success << "]" << query.executedQuery();

    if (success)
    {
        QSqlRecord rec = query.record();

        curItems.append(YerothUtils::EMPTY_STRING);

        while (query.next())
        {
            QString currentItem(query.value(0).toString());

            if (!curItems.contains(currentItem))
            {
                curItems.append(currentItem);
            }
        }
    }

    clear();

    curItems.sort();

    addItems(curItems);

    curItems.clear();

    return true;
}


/*
 * This is used when the argument 'aContent' must be the first
 * element of the combo box when modifying an existing set
 * of combo box elements.
 */
bool YerothComboBox::populateComboBoxMissingRawString(const QString aDBFieldColumn,
													  const QString aDBTableViewString,
													  const QString aContentSTRINGValue)
{
	if (!isPopulateRaw())
	{
		return false;
	}

    static QStringList curItems;

    QString strQuery(QString("select %1 from %2")
    					.arg(aDBFieldColumn, aDBTableViewString));

    QSqlQuery query;
    query.prepare(strQuery);
    bool success = query.exec();
    //qDebug() << "[" << success << "]" << query.executedQuery();

    QString aContent(aContentSTRINGValue);

    if (success)
    {
        QSqlRecord rec = query.record();

        QString content;

        while (query.next())
        {
            content = query.value(0).toString();

            if (content != aContent)
            {
                curItems.append(content);
            }
        }
    }

    clear();

    curItems.sort();

    curItems.prepend(aContent);

    addItems(curItems);

    curItems.clear();

    return true;
}


bool YerothComboBox::populateComboBox()
{
	if (isPopulateRaw())
	{
		return false;
	}

    static QStringList curItems;

    clear();

    QString strQuery(QString("select %1 from %2")
    					.arg(_dbFieldColumn, _dbTableViewStringName));

    //qDebug() << "++ query: " << strQuery;
    QSqlQuery query;
    query.prepare(strQuery);
    bool success = query.exec();
    //qDebug() << "[" << success << "]" << query.executedQuery();

    if (success)
    {
        QSqlRecord rec = query.record();

        curItems.append(YerothUtils::EMPTY_STRING);

        while (query.next())
        {
            QString currentItem(_pointerToUserViewStringMAP->value(query.value(0).toInt()));

            if (!curItems.contains(currentItem))
            {
                curItems.append(currentItem);
            }
        }
    }

    clear();

    curItems.sort();

    addItems(curItems);

    curItems.clear();

	return true;
}


/*
 * This is used when the argument 'aContent' must be the first
 * element of the combo box when modifying an existing set
 * of combo box elements.
 */
bool YerothComboBox::populateComboBoxMissing(const int aContentINTValue)
{
	if (isPopulateRaw())
	{
		return false;
	}

    static QStringList curItems;

    QString strQuery(QString("select %1 from %2")
    					.arg(_dbFieldColumn, _dbTableViewStringName));

    QSqlQuery query;
    query.prepare(strQuery);
    bool success = query.exec();
    //qDebug() << "[" << success << "]" << query.executedQuery();

    QString aContent(_pointerToUserViewStringMAP->value(aContentINTValue));

    if (success)
    {
        QSqlRecord rec = query.record();

        QString content;

        while (query.next())
        {
            content = _pointerToUserViewStringMAP->value(query.value(0).toInt());

            if (content != aContent)
            {
                curItems.append(content);
            }
        }
    }

    clear();

    curItems.sort();

    curItems.prepend(aContent);

    addItems(curItems);

    curItems.clear();

    return true;
}


/*
 * This is used when the argument 'aContent' must not be
 * present in the combo box.
 */
bool YerothComboBox::populateComboBoxWithout(const QList<int> &content_to_delete_from_user_view_int_values_IN_OUT)
{
	if (isPopulateRaw())
	{
		return false;
	}

    static QStringList curItems;

    QString strQuery(QString("select %1 from %2")
    					.arg(_dbFieldColumn, _dbTableViewStringName));

    QSqlQuery query;
    query.prepare(strQuery);
    bool success = query.exec();
    //qDebug() << "[" << success << "]" << query.executedQuery();

    if (success)
    {
    	QStringList content_to_delete_from_user_view_string_values;

    	for (uint i = 0; i < content_to_delete_from_user_view_int_values_IN_OUT.size(); ++i)
    	{
    		content_to_delete_from_user_view_string_values
				.append(_pointerToUserViewStringMAP->value(content_to_delete_from_user_view_int_values_IN_OUT.at(i)));
    	}

        QSqlRecord rec = query.record();

        curItems.append(YerothUtils::EMPTY_STRING);

        QString content;

        while (query.next())
        {
            content = _pointerToUserViewStringMAP->value(query.value(0).toInt());

            if (!content_to_delete_from_user_view_string_values.contains(content))
            {
                curItems.append(content);
            }
        }
    }

    clear();

    curItems.sort();

    addItems(curItems);

    curItems.clear();

    return true;
}


/*
 * This is used when the argument 'aContent' must not be
 * present in the combo box.
 */
bool YerothComboBox::populateComboBoxWithout(const int aContentINTValue)
{
	if (isPopulateRaw())
	{
		return false;
	}

    static QStringList curItems;

    QString strQuery(QString("select %1 from %2")
    					.arg(_dbFieldColumn, _dbTableViewStringName));

    QSqlQuery query;
    query.prepare(strQuery);
    bool success = query.exec();
    //qDebug() << "[" << success << "]" << query.executedQuery();

    if (success)
    {
        QSqlRecord rec = query.record();

        QString aContent(_pointerToUserViewStringMAP->value(aContentINTValue));

        curItems.append(YerothUtils::EMPTY_STRING);

        QString content;

        while (query.next())
        {
            content = _pointerToUserViewStringMAP->value(query.value(0).toInt());

            if (content != aContent)
            {
                curItems.append(content);
            }
        }
    }

    clear();

    curItems.sort();

    addItems(curItems);

    curItems.clear();

    return true;
}
