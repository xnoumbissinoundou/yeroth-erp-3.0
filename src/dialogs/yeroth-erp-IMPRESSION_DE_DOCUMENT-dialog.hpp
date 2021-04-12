/*
 * yeroth-erp-IMPRESSION_DE_DOCUMENT-dialog.hpp
 *
 *      Author: PR. XAVIER NOUMBISSI NOUNDOU
 */

#ifndef YEROTH_ERP_IMPRESSION_DE_DOCUMENT_DIALOG_HPP_
#define YEROTH_ERP_IMPRESSION_DE_DOCUMENT_DIALOG_HPP_

#include "../../ui_yeroth-erp-imprimer-dialog.h"

#include "yeroth-erp-dialog-commons.hpp"


#include "src/utils/yeroth-erp-utils.hpp"


class QPoint;

class YerothERPWindows;

class YerothWindowsCommons;

class YerothTableViewPRINT_UTILITIES_TEX_TABLE;


class YerothIMPRESSION_DE_DOCUMENT_Dialog : public YerothPOSDialogCommons,
											public Ui_YerothIMPRESSION_DE_DOCUMENT_Dialog
{
    Q_OBJECT

public:


	YEROTH_CLASS_OPERATORS

	YerothIMPRESSION_DE_DOCUMENT_Dialog();

    ~YerothIMPRESSION_DE_DOCUMENT_Dialog();

    virtual QPoint * getCurrentPosition()
    {
    	return _currentPosition;
    }


public:

    void setupLineEdits();

    void reset_all_fields();

public slots:

	virtual void show();

	virtual void rendreInvisible();

	void valider();

	inline void annuler()
	{
		rendreInvisible();
	}

public:

	static YerothWindowsCommons						*_current_window_to_print;

	static YerothTableViewPRINT_UTILITIES_TEX_TABLE *_yeroth_CURRENT_DOCUMENT_PRINT_UTILITIES_TEX_TABLE;

protected:

	virtual void closeEvent(QCloseEvent * closeEvent);

private:

	static const QString 	DIALOG_WINDOW_TITLE;

	QPoint					*_currentPosition;
};


#endif /* YEROTH_ERP_IMPRESSION_DE_DOCUMENT_DIALOG_HPP_ */
