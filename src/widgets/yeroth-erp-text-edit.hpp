/*
 * yeroth-erp-text-edit.hpp
 *
 *      Author: PR. XAVIER NOUMBISSI NOUNDOU
 */

#ifndef SRC_WIDGETS_YEROTH_TEXT_EDIT_HPP_
#define SRC_WIDGETS_YEROTH_TEXT_EDIT_HPP_

#include "src/include/yeroth-erp-3-0-software.text-configuration.hpp"

#include "src/utils/yeroth-erp-utils.hpp"

#include <QtWidgets/QTextEdit>

class YerothUtils;

class YerothTextEdit : public QTextEdit
{
public:

	YEROTH_CLASS_OPERATORS

	inline YerothTextEdit(QWidget *parent = 0)
		   :QTextEdit(parent),
			_originalPaletteBeforeMissingInformation(palette())
	{
		setYerothPOSTextEditDisplayFormat();
	}


	inline YerothTextEdit(const QString &text, QWidget *parent)
		   :QTextEdit(text, parent),
		   _originalPaletteBeforeMissingInformation(palette())
	{
		setYerothPOSTextEditDisplayFormat();
	}

	inline ~YerothTextEdit()
	{
	}

	bool checkField();

	void setYerothPOSTextEditDisplayFormat();

	void setVisible(bool visible);

	void clearField();

	void setYerothEnabled(bool enabled);

	QString toPlainTextForLatex() const;

	inline QString toPlainText() const
	{
		return QTextEdit::toPlainText().trimmed();
	}

private:

	QPalette _originalPaletteBeforeMissingInformation;
};


#endif /* SRC_WIDGETS_YEROTH_TEXT_EDIT_HPP_ */
