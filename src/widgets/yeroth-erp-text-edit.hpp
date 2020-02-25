/*
 * yeroth-erp-text-edit.hpp
 *
 *      Author: Xavier NOUMBISSI NOUNDOU, Dipl.-Inf., Ph.D. (ABD)
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

	YerothTextEdit(QWidget * parent = 0);

	YerothTextEdit(const QString &text, QWidget *parent);

	inline ~YerothTextEdit()
	{
	}

	bool checkField();

	void setYerothPOSTextEditDisplayFormat();

	void setVisible(bool visible);

	void clearField();

	void setEnabled(bool enabled);

	inline QString toPlainTextForLatex() const
	{
		return YerothUtils::handleForeignAccents(QTextEdit::toPlainText().trimmed());
	}

	inline QString toPlainText() const
	{
		return QTextEdit::toPlainText().trimmed();
	}

private:

	QPalette _originalPaletteBeforeMissingInformation;
};


#endif /* SRC_WIDGETS_YEROTH_TEXT_EDIT_HPP_ */
