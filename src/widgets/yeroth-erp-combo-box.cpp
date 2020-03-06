/*
 * yeroth-erp-combo-box.cpp
 *
 *      Author: Xavier NOUMBISSI NOUNDOU, Dipl.-Inf., Ph.D. (ABD)
 */

#include "yeroth-erp-combo-box.hpp"


#include "src/widgets/yeroth-erp-line-edit.hpp"


#include <QtCore/QDebug>

#include <QtSql/QSqlQuery>


YerothComboBox::YerothComboBox(QWidget *parent)
:QComboBox(parent),
 _populateRawString(true),
 _pointerToUserViewStringMAP(0)
{
	_originalPaletteBeforeMissingInformation = palette();

    setLineEdit(new QLineEdit);

    lineEdit()->setReadOnly(true);

    lineEdit()->setAlignment(Qt::AlignHCenter | Qt::AlignCenter);
}


void YerothComboBox::setupPopulateNOTRawString(QString aDBTableViewStringName,
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


/*
 * This is used when the argument 'aContent' must be the first
 * element of the combo box when modifying an existing set
 * of combo box elements.
 */
bool YerothComboBox::populateComboBoxRawString(QString aDBTableViewStringName,
		   	   	   	   	   	   	   	   	   	   QString aDBFieldColumn)
{
	if (!isPopulateRaw())
	{
		return false;
	}

    static QStringList curItems;

    clear();

    QString strQuery(QString("select %1 from %2")
    					.arg(aDBFieldColumn, aDBTableViewStringName));

    //qDebug() << "++ query: " << strQuery;
    QSqlQuery query;
    query.prepare(strQuery);
    bool success = query.exec();
    //qDebug() << "[" << success << "]" << query.executedQuery();

    if (success)
    {
        QSqlRecord rec = query.record();

        curItems.append("");

        while (query.next())
        {
            QString currentItem(query.value(0).toString());

            if (!curItems.contains(currentItem))
            {
                curItems.append(currentItem);
            }
        }
    }

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

    if (success)
    {
        QSqlRecord rec = query.record();

        QString aContent(aContentSTRINGValue);

        curItems.append(aContent);

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

    addItems(curItems);

    curItems.clear();

    return true;
}


/*
 * This is used when the argument 'aContent' must be the first
 * element of the combo box when modifying an existing set
 * of combo box elements.
 */
bool YerothComboBox::populateComboBoxWithViewStringActivated()
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

        curItems.append("");

        while (query.next())
        {
            QString currentItem(_pointerToUserViewStringMAP->value(query.value(0).toInt()));

            if (!curItems.contains(currentItem))
            {
                curItems.append(currentItem);
            }
        }
    }

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

    if (success)
    {
        QSqlRecord rec = query.record();

        QString aContent(_pointerToUserViewStringMAP->value(aContentINTValue));

        curItems.append(aContent);

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

    addItems(curItems);

    curItems.clear();

    return true;
}


