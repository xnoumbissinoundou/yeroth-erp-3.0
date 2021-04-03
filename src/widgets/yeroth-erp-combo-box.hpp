/*
 * yeroth-erp-combo-box.hpp
 *
 *      Author: PR. XAVIER NOUMBISSI NOUNDOU
 */

#ifndef YEROTH_COMBO_BOX_HPP_
#define YEROTH_COMBO_BOX_HPP_


#include "src/utils/yeroth-erp-utils.hpp"


#include <QtWidgets/QComboBox>

#include <QtSql/QSqlRecord>


class QSqlRecord;


class YerothComboBox : public QComboBox
{

public:

	YEROTH_CLASS_OPERATORS

	YerothComboBox(QWidget *parent);

	inline ~YerothComboBox(){}

	void setupPopulateNORawString(QString aDBTableViewStringName,
								   QString aDBFieldColumn,
								   QMap<int, QString> *pointerToUserViewStringMAP);

	inline const QString& getDbFieldColumn() const
	{
		return _dbFieldColumn;
	}

	inline const QString& getDbTableViewStringName() const
	{
		return _dbTableViewStringName;
	}

	inline QMap<int, QString> *getPointerToUserViewStringMap()
	{
		return _pointerToUserViewStringMAP;
	}

	inline bool isPopulateRaw()
	{
		return _populateRawString;
	}

	bool checkField();

	void clearField();

	inline void resetYerothComboBox()
	{
		 setCurrentIndex(0);
	}

	inline virtual bool contains(const QString &a_text)
	{
		return (-1 != findText(a_text));
	}

	inline QString currentText() const
	{
		return QComboBox::currentText().trimmed();
	}

	inline bool isEmpty()
	{
		return currentText().isEmpty();
	}

	void setYerothEnabled(bool enabled);

	inline void saveRawCurrentValueToDatabase(const QString &aDBFieldColumn,
											  QSqlRecord &aQSqlRecordToSAVE)
	{
		aQSqlRecordToSAVE.setValue(aDBFieldColumn, currentText());
	}

	void saveCurrentValueToDatabase(const QString &aDBFieldColumn,
									QSqlRecord &aQSqlRecordToSAVE);

	inline void setDBField(QString aDBFieldColumn)
	{
		_dbFieldColumn = aDBFieldColumn;
	}

	inline void setPointerToUserViewStringMAP(QMap<int, QString> *pointerToUserViewStringMAP)
	{
		_pointerToUserViewStringMAP = pointerToUserViewStringMAP;
	}

	inline void yeroth_remove_item(const QString &text)
	{
		removeItem(findText(text));
	}

	virtual void addItem_AFTER_POPULATE(const QStringList &texts);

	virtual void addItems(const QStringList &texts);

	bool populateComboBoxRawString(QString aDBTableViewStringName,
								   QString aDBFieldColumn,
								   QString aConditionStr = "");

	bool populateComboBoxMissingRawString(const QString aDBFieldColumn,
										  const QString aDBTableViewString,
										  const QString aContentSTRINGValue);

	bool populateComboBox();

	bool populateComboBoxMissing(const int aContentINTValue);

	bool populateComboBoxWithout(const QList<int> &content_to_delete_from_user_view_int_values_IN_OUT);

	bool populateComboBoxWithout(const int aContentINTValue);


private:

	inline void setPopulateRaw(bool populateRawString)
	{
		_populateRawString = populateRawString;
	}

	bool _populateRawString;

	QString _dbTableViewStringName;

	QString _dbFieldColumn;

	QMap<int, QString> *_pointerToUserViewStringMAP;

	QPalette _originalPaletteBeforeMissingInformation;
};


#endif /* YEROTH_COMBO_BOX_HPP_ */
