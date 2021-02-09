/*
 * yeroth-erp-print_yeroth_latex_util_pdf.hpp
 *
 *      DR. XAVIER NOUMBISSI NOUNDOU
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

protected:

	QString					_output_pdf_file_name;

	QString					_tex_template_document_string;

	QSet<int>				_dbCENTER_TO_ALIGN_FieldName;

	QSet<int> 				_dbRIGHT_TO_ALIGN_FieldName;

	QSet<int> 				_dbLEFT_TO_ALIGN_FieldName;

	YerothWindowsCommons 	*_yerothWindowTableOutputView;

	YerothTableView 		*_yerothTableView;
};

#endif /* YEROTH_ERP_PRINT_YEROTH_LATEX_UTIL_PDF_HPP_ */
