/*
 * yeroth-erp-print_yeroth_latex_util_pdf.hpp
 *
 *      PR. XAVIER NOUMBISSI NOUNDOU
 */

#ifndef YEROTH_ERP_PRINT_YEROTH_LATEX_UTIL_PDF_HPP_
#define YEROTH_ERP_PRINT_YEROTH_LATEX_UTIL_PDF_HPP_


#include "src/include/yeroth-erp-3-0-software.text-configuration.hpp"

#include "src/windows/yeroth-erp-window-commons.hpp"

#include <QtCore/QSet>
#include <QtCore/QString>
#include <QtCore/QCryptographicHash>


class YerothTableViewPRINT_UTILITIES_TEX_TABLE : public QObject
{
	Q_OBJECT

public:

	YEROTH_CLASS_OPERATORS

	YerothTableViewPRINT_UTILITIES_TEX_TABLE(const QString &output_pdf_file_name_out,
											 YerothWindowsCommons &aYerothWindowTableOutputView,
											 YerothTableView &aYerothTableView);

	void setYerothTableView(YerothTableView *aYerothTableView);

	void RESET_NOMBRE_DE_LIGNES_TABLEAU_STANDARD();

	uint MAX_TABLE_ROW_COUNT_first_page();

	inline double GET_NOMBRE_DE_LIGNES()
	{
		return _MAX_TABLE_ROW_COUNT;
	}

	inline void SET_NOMBRE_DE_LIGNES(uint nbre_de_lignes)
	{
		_MAX_TABLE_ROW_COUNT = nbre_de_lignes;
	}

	inline QString GET_PRINT_POSITION_PAPER_SPECIFICATION()
	{
		return _LATEX_A4_PAPER_SPEC;
	}

	void SET_VERTICAL_PRINT_POSITION();

	void SET_HORIZONTAL_PRINT_POSITION();

	QString print_YEROTH_document_from_TableView(uint pageFROM,
												 uint pageTO,
												 const QString &aLatex_template_document_string,
												 QMap<QString, QString> *documentSpecificElements = 0);

	QString print_YEROTH_document_from_TableView(const QString &aLatex_template_document_string,
												 QMap<QString, QString> *documentSpecificElements = 0);

protected:

	void get_YEROTH_ListingTex_TEMPLATE_DocumentString(QString &output_tex_document_string_in_out,
												  	   const QString &append_string);

	void get_YEROTH_TableViewListingTexDocumentString(const QList<int> &aDBTableColumnsToIgnore_in_out,
													  QString &latexTable_in_out,
													  int fromRowIndex,
													  int toRowIndex,
													  bool lastPage);

public:

	QString					_LATEX_A4_PAPER_SPEC;

	static const double		_STANDARD_INITIAL_MAX_TABLE_ROW_COUNT_A4PORTRAIT;

	static const double		_STANDARD_INITIAL_MAX_TABLE_ROW_COUNT_A4LANDSCAPE;

protected:

	static const QString	_A4PAGE_PORTRAIT_SPECIFICATION;

	static const QString	_A4PAGE_LANDSCAPE_SPECIFICATION;

	double 					_MAX_TABLE_ROW_COUNT;

	QString					_output_pdf_file_name;

	QString					_tex_template_document_string;

	QSet<int>				_dbCENTER_TO_ALIGN_FieldName;

	QSet<int> 				_dbRIGHT_TO_ALIGN_FieldName;

	QSet<int> 				_dbLEFT_TO_ALIGN_FieldName;

	YerothWindowsCommons 	*_yerothWindowTableOutputView;

	YerothTableView 		*_yerothTableView;
};

#endif /* YEROTH_ERP_PRINT_YEROTH_LATEX_UTIL_PDF_HPP_ */
