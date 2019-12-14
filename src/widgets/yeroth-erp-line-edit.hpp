/*
 * yeroth-erp-line-edit.hpp
 *
 *      Author: Xavier NOUMBISSI NOUNDOU, Dipl.-Inf., Ph.D. (ABD)
 */

#ifndef SRC_WIDGETS_YEROTH_LINE_EDIT_HPP_
#define SRC_WIDGETS_YEROTH_LINE_EDIT_HPP_

#include <QtCore/QStringList>

#include <QtWidgets/QLineEdit>

#include "src/utils/yeroth-erp-utils.hpp"
#include "src/utils/yeroth-erp-sqltable-model.hpp"

#include "src/widgets/table-view/yeroth-erp-table-view.hpp"

class QCompleter;

class YerothUtils;
class YerothSqlTableModel;
class YerothTableView;

class YerothLineEdit : public QLineEdit
{
	Q_OBJECT

public:

	YEROTH_CLASS_OPERATORS

	YerothLineEdit(QWidget *parent = 0);

	virtual ~YerothLineEdit(){}

	inline bool isEmpty()
	{
		return this->text().isEmpty();
	}

	void enableForSearch(QString placeHoderText);

	void disableForSearch();

	bool checkField(const QPalette *aPalette = 0);

	bool checkFieldAndMinimalValue(double min, QPalette *aPalette = 0);

	inline void setRed()
	{
		this->setPalette(YerothUtils::YEROTH_RED_PALETTE);
	}

	void setYerothERPQLineEditDisplayFormat();

	void clearField();

	void setEnabled(bool enabled);

	inline QString text() const
	{
		return QLineEdit::text().trimmed();
	}

	inline QString getLastQCompleterText()
	{
		return _lastQCompleterText;
	}

	void myClear();

	void setupMyQCompleter(QString sqlTableName,
	   	    			   int listColumnIndex,
						   enum Qt::SortOrder sortOrder,
						   bool truncateString = true);

	void setupMyStaticQCompleter(QString sqlTableName,
								 const char *fieldName,
								 bool enableNewCreation = false,
								 bool returnPressedSignalActivated = true);

	void setupMyStaticQCompleter(QString sqlTableName,
								 QString fieldName,
								 bool enableNewCreation = false,
								 bool returnPressedSignalActivated = true);

	void setupMyQCompleterALL(QString sqlTableName);

	void setupMyQCompleterDEF_DEO(QString sqlTableName);

	void setupMyQCompleterFIFO(QString sqlTableName);

	void setupMyQCompleterLIFO(QString sqlTableName);

	void setupMyQCompleterCodebarALL(QString sqlTableName);

	void setupMyQCompleterCodebarDEF_DEO(QString sqlTableName);

	void setupMyQCompleterCodebarFIFO(QString sqlTableName);

	void setupMyQCompleterCodebarLIFO(QString sqlTableName);

	inline QMap<QString, int> &getDesignationToTableRows()
	{
		return _designationToTableRows;
	}

	inline QMap<QString, int> &getCodebarToTableRows()
	{
		return _codebarToTableRows;
	}

	void refreshCodebarCompleterList(QString strategy);

	void refreshCompleterList(QString strategy);

	inline QCompleter *getMyQCompleter()
	{
		return _searchQCompleter;
	}

	inline YerothSqlTableModel *getMySqlTableModel()
	{
		return _sqlTableModel;
	}

	inline YerothTableView *getMyTableView()
	{
		return _sqlTableModelView;
	}

	inline QStringList getCurrentStaticStringList()
	{
		return _currentStaticStringList;
	}

	static const QString	    EMPTY_STRING;

public slots:

	void clearQCompleterText();

	void isInputValid();

private:

	bool 				_forSearch;

	bool				_wasMissingRequiredText;

	QString				_originalPlaceHolderText;

	QPalette 			_originalPaletteBeforeMissingInformation;

	YerothSqlTableModel 	*_sqlTableModel;

	YerothTableView 		*_sqlTableModelView;

	QCompleter			*_searchQCompleter;

	QMap<QString, int>  _designationToTableRows;

	QMap<QString, int> 	_codebarToTableRows;

	QStringList			_currentStaticStringList;

	QString				_lastQCompleterText;
};

#endif /* SRC_WIDGETS_YEROTH_LINE_EDIT_HPP_ */
