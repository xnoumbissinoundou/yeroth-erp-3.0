/*
 * yeroth-erp-print_yeroth_latex_util_pdf.cpp
 *
 *      DR. XAVIER NOUMBISSI NOUNDOU
 */

#include "yeroth-erp-print_yeroth_latex_util_pdf.hpp"


#include "src/process/yeroth-erp-process.hpp"

#include "src/utils/yeroth-erp-info-entreprise.hpp"

#include "src/yeroth-erp-windows.hpp"


#include <QtCore/qmath.h>


YerothTableViewPRINT_UTILITIES_TEX_TABLE::
	YerothTableViewPRINT_UTILITIES_TEX_TABLE(const QString &output_pdf_file_name_out,
											 YerothWindowsCommons &aYerothWindowTableOutputView,
											 YerothTableView &aYerothTableView)
:_output_pdf_file_name(output_pdf_file_name_out),
 _yerothWindowTableOutputView(&aYerothWindowTableOutputView),
 _yerothTableView(&aYerothTableView)
{
	_dbCENTER_TO_ALIGN_FieldName = _yerothWindowTableOutputView->getDBFieldNamesToPrintCenterAligned();

	_dbRIGHT_TO_ALIGN_FieldName = _yerothWindowTableOutputView->getDBFieldNamesToPrintRightAligned();

	_dbLEFT_TO_ALIGN_FieldName = _yerothWindowTableOutputView->getDBFieldNamesToPrintLeftAligned();
}


void YerothTableViewPRINT_UTILITIES_TEX_TABLE::setYerothTableView(YerothTableView *aYerothTableView)
{
	if (0 == aYerothTableView)
	{
		return ;
	}

	_yerothTableView = aYerothTableView;
}


QString YerothTableViewPRINT_UTILITIES_TEX_TABLE::
	print_YEROTH_document_from_TableView(const QString &aLatex_template_document_string,
										 QMap<QString, QString> *documentSpecificElements /* = 0 */)
{
	_tex_template_document_string.clear();
	_tex_template_document_string.append(aLatex_template_document_string);

    QString latexFileNamePrefix(_output_pdf_file_name);

    QList<int> dbTableColumnsToIgnore_in_out;

    _yerothWindowTableOutputView->fill_table_columns_to_ignore(dbTableColumnsToIgnore_in_out);

    QString pdfBuyingsFileName;

#ifdef YEROTH_FRANCAIS_LANGUAGE
    latexFileNamePrefix.append("_french");
#endif

#ifdef YEROTH_ENGLISH_LANGUAGE
    latexFileNamePrefix.append("_english");
#endif

    QStandardItemModel *tableModel = _yerothTableView->getStandardItemModel();

    int tableModelRowCount = tableModel->rowCount();

    if (tableModelRowCount <= 0  ||
    	tableModel->columnCount() <= dbTableColumnsToIgnore_in_out.count())
    {
        YerothQMessageBox::information(_yerothWindowTableOutputView,
                                       QObject::tr("impression"),
                                       QObject::trUtf8("Il n'y a pas de données à imprimer !"));
        return YerothUtils::EMPTY_STRING;
    }

    int fromRowIndex = 0;

    int toRowIndex = tableModelRowCount;

    QString latexTable_in_out;

    double MAX_TABLE_ROW_COUNT = 35.0;

    int pageNumber = qCeil(tableModelRowCount / MAX_TABLE_ROW_COUNT);

    //qDebug() << QString("number of pages to print: %1").arg(pageNumber);
    //_logger->log("imprimer_pdf_document",
    //                  QString("number of pages to print: %1").arg(pageNumber));
	get_YEROTH_TableViewListingTexDocumentString(dbTableColumnsToIgnore_in_out,
												 latexTable_in_out,
												 0,
												 (20 >= tableModelRowCount) ? tableModelRowCount : 20,
												 tableModelRowCount <= 20);

	int currentProgressBarCount = abs(((2.0 / pageNumber) * 50) - 4);

	emit _yerothWindowTableOutputView->SIGNAL_INCREMENT_PROGRESS_BAR(currentProgressBarCount);

    if (tableModelRowCount >= 20)
    {
        fromRowIndex = 20;

        toRowIndex = (fromRowIndex >= tableModelRowCount) ? fromRowIndex : fromRowIndex + MAX_TABLE_ROW_COUNT;

        int k = 1;

        do
        {
            //qDebug() << QString("## fromRowIndex: %1, toRowIndex: %2")
            //          .arg(QString::number(fromRowIndex), QString::number(toRowIndex));
        	get_YEROTH_TableViewListingTexDocumentString(dbTableColumnsToIgnore_in_out,
        												 latexTable_in_out,
														 (fromRowIndex >= tableModelRowCount) ? tableModelRowCount : fromRowIndex,
														 (toRowIndex >= tableModelRowCount) ? (tableModelRowCount + 1) : toRowIndex,
        												 k == pageNumber);

        	latexTable_in_out.append(QString("\\newpage \n"));

            fromRowIndex = toRowIndex;

            toRowIndex =
                (fromRowIndex >= tableModelRowCount) ? (fromRowIndex + 1) : fromRowIndex + MAX_TABLE_ROW_COUNT;

            currentProgressBarCount += abs( (((k+1) / pageNumber) * 100) - 4 );

            currentProgressBarCount = (currentProgressBarCount > 98) ? 97 : currentProgressBarCount;

            emit _yerothWindowTableOutputView->SIGNAL_INCREMENT_PROGRESS_BAR(currentProgressBarCount);

            ++k;
        }

        while (k <= pageNumber && fromRowIndex <= toRowIndex);
    }

    YerothInfoEntreprise & infoEntreprise = YerothUtils::getAllWindows()->getInfoEntreprise();

    QString texDocument;

    get_YEROTH_ListingTex_TEMPLATE_DocumentString(texDocument, latexTable_in_out);

    QString factureDate(YerothUtils::LATEX_IN_OUT_handleForeignAccents(infoEntreprise.getVille_LATEX()));

    YerothUtils::getCurrentLocaleDate(factureDate);


    if ( 0 != documentSpecificElements )
    {
    	QMapIterator<QString, QString> itDocumentSpecificElements(*documentSpecificElements);

    	while (itDocumentSpecificElements.hasNext())
    	{
    		itDocumentSpecificElements.next();
    		texDocument.replace(itDocumentSpecificElements.key(), itDocumentSpecificElements.value());
    	}
    }


    texDocument.replace("YEROTHPAPERSPEC", "a4paper");

    texDocument.replace("YEROTHENTREPRISE", infoEntreprise.getNomCommercial_LATEX());
    texDocument.replace("YEROTHACTIVITESENTREPRISE", infoEntreprise.getSecteursActivitesTex());
    texDocument.replace("YEROTHBOITEPOSTALE", infoEntreprise.getBoitePostal());
    texDocument.replace("YEROTHVILLE", infoEntreprise.getVille_LATEX());
    texDocument.replace("YEROTHPAYS", infoEntreprise.getPaysTex());
    texDocument.replace("YEROTHEMAIL", infoEntreprise.getEmail_LATEX());
    texDocument.replace("YEROTHTELEPHONE", infoEntreprise.getTelephone());
    texDocument.replace("YEROTHDATE", factureDate);

    texDocument.replace("YEROTHNOMUTILISATEUR", QString("%1 %2")
    												.arg(YerothUtils::getAllWindows()->getUser()->titre(),
    													 YerothUtils::getAllWindows()->getUser()->nom_completTex()));

    texDocument.replace("YEROTHHEUREDIMPRESSION", CURRENT_TIME);
    texDocument.replace("YEROTHCOMPTEBANCAIRENR", infoEntreprise.getNumeroCompteBancaire());
    texDocument.replace("YEROTHCONTRIBUABLENR", infoEntreprise.getNumeroDeContribuable());
    texDocument.replace("YEROTHAGENCECOMPTEBANCAIRE", infoEntreprise.getAgenceCompteBancaireTex());

    QString yerothPrefixFileName;

    yerothPrefixFileName.append(YerothUtils::getUniquePrefixFileInTemporaryFilesDir(latexFileNamePrefix));

	//qDebug() << "++\n" << texDocument;

    QFile tmpLatexFile(QString("%1tex")
    					.arg(yerothPrefixFileName));

    YerothUtils::writeStringToQFilewithUTF8Encoding(tmpLatexFile, texDocument);

    emit _yerothWindowTableOutputView->SIGNAL_INCREMENT_PROGRESS_BAR(94);

    return YerothERPProcess::compileLatex(yerothPrefixFileName);
}


void YerothTableViewPRINT_UTILITIES_TEX_TABLE::
	get_YEROTH_ListingTex_TEMPLATE_DocumentString(QString &output_tex_document_string_in_out,
												  const QString &append_string)
{
    output_tex_document_string_in_out.clear();
    output_tex_document_string_in_out.append(_tex_template_document_string);
    output_tex_document_string_in_out.append(append_string).append("\n");
    output_tex_document_string_in_out.append("\\end{document}");
}


void YerothTableViewPRINT_UTILITIES_TEX_TABLE::
	get_YEROTH_TableViewListingTexDocumentString(const QList<int> &aDBTableColumnsToIgnore_in_out,
												 QString &latexTable_in_out,
												 int fromRowIndex,
												 int toRowIndex,
												 bool lastPage)
{
    if (lastPage && toRowIndex > 20)
    {
        toRowIndex -= 1;
    }

    if (fromRowIndex == toRowIndex)
    {
        return ;
    }


    bool USE_RESIZE_BOX_FOR_COLUMN_PRINTING = false;

    static bool first_time_execution = true;

    if (first_time_execution &&
    	0 == fromRowIndex)
    {
    	uint table_column_count = _yerothTableView->horizontalHeader()->count();
    	uint table_column_to_ignore_count = aDBTableColumnsToIgnore_in_out.size();

    	uint table_column_visible_columns_count = table_column_count - table_column_to_ignore_count;

    	if (table_column_visible_columns_count >= 7)
    	{
    		USE_RESIZE_BOX_FOR_COLUMN_PRINTING = true;
    	}
    	else
    	{
    		USE_RESIZE_BOX_FOR_COLUMN_PRINTING = false;
    	}

    	first_time_execution = false;
    }


    if (lastPage)
    {
    	first_time_execution = true;
    }


    if (USE_RESIZE_BOX_FOR_COLUMN_PRINTING)
    {
    	latexTable_in_out.append("\\begin{table*}[!htbp]").append("\n")
    	    			 .append("\\centering").append("\n")
    					 .append("\\resizebox{\\textwidth}{!}{\\centering").append("\n")
    					 .append("\\begin{tabular}")
    					 .append("{|");
    }
    else
    {
    	latexTable_in_out.append("\\begin{table*}[!htbp]").append("\n")
    	    			 .append("\\centering").append("\n")
    					 .append("\\begin{tabular}")
    					 .append("{|");
    }


    QStandardItemModel &tableStandardItemModel =
    		*(static_cast<QStandardItemModel *> (_yerothTableView->model()));

    int texTableColumnCount = tableStandardItemModel.columnCount() + 1;

    latexTable_in_out.append("c|");

    QList<int> headerViewModelIndex;

    int realK_pos = 0;

    //Tex table header
    for (int k = 0; k < _yerothTableView->horizontalHeader()->count(); ++k)
    {
    	realK_pos = _yerothTableView->horizontalHeader()->logicalIndex(k);

        if (aDBTableColumnsToIgnore_in_out.contains(realK_pos))
        {
            continue;
        }

        headerViewModelIndex.append(realK_pos);

        if (_dbLEFT_TO_ALIGN_FieldName.contains(realK_pos))
        {
        	latexTable_in_out.append("l|");
        }
        else if (_dbRIGHT_TO_ALIGN_FieldName.contains(realK_pos))
        {
        	latexTable_in_out.append("r|");
        }
        else if (_dbCENTER_TO_ALIGN_FieldName.contains(realK_pos))
        {
        	latexTable_in_out.append("c|");
        }
        else
        {
        	latexTable_in_out.append("r|");
        }
    }

    latexTable_in_out.append("} \\hline\n");

    /** We add a column named 'id' for numbering the rows
     * in the Tex table. */
    unsigned int id = fromRowIndex + 1;

    latexTable_in_out.append("\\textbf{n\\textsuperscript{o}} & ");

    QStandardItem *item;

    for (int k = 0; k < headerViewModelIndex.count(); ++k)
    {
    	realK_pos = headerViewModelIndex.at(k);

        item = tableStandardItemModel.horizontalHeaderItem(realK_pos);

        if (0 != item)
        {
        	QString itemText(item->text());

        	itemText.prepend("\\textbf{").append("}");

        	itemText.prepend("\\multicolumn{1}{c|}{").append("}");

            YerothUtils::handleTexTableItemText(texTableColumnCount,
                                   latexTable_in_out,
                                   realK_pos,
                                   itemText);
        }
    }

    /** Closing Tex table header */
    YerothUtils::cleanUpTexTableLastString(latexTable_in_out);

    latexTable_in_out.append(" \\\\ \\hline\n");

    //qDebug() << QString("++ fromRowIndex: %1, toRowIndex: %2")
    //			.arg(QString::number(fromRowIndex), QString::number(toRowIndex));

    for (int j = fromRowIndex; j < toRowIndex; ++j)
    {
        latexTable_in_out.append(QString::number(id));
        latexTable_in_out.append(" &");
        ++id;

        realK_pos = 0;

        for (int k = 0; k < headerViewModelIndex.count(); ++k)
        {
        	realK_pos = headerViewModelIndex.at(k);

            item = tableStandardItemModel.item(j, realK_pos);

            if (item)
            {
                QString itemText(item->text());

            	if (YerothUtils::YEROTH_RED_COLOR == item->foreground().color())
    			{
            		itemText.prepend("\\textcolor{yerothColorRed}{").append("}");
    			}

                YerothUtils::handleTexTableItemText(texTableColumnCount,
                                              	  	latexTable_in_out,
													realK_pos,
													itemText);

//                qDebug() << QString("++ itemText: %1")
//                				.arg(itemText);
            }
            else
            {
                if (realK_pos < tableStandardItemModel.columnCount() - 1)
                {
                    latexTable_in_out.append("\"\"").append(" &");
                }
                else
                {
                    latexTable_in_out.append("\"\"").append("\\hline\n");
                }
            }
        }

        latexTable_in_out = latexTable_in_out.trimmed();

        YerothUtils::cleanUpTexTableLastString(latexTable_in_out);

        latexTable_in_out.append("\\\\ \\hline\n");
    }

    //Removes the empty character "" from Latex output
    latexTable_in_out.replace("\"\"", YerothUtils::EMPTY_STRING);

    if (USE_RESIZE_BOX_FOR_COLUMN_PRINTING)
    {
    	latexTable_in_out.append("\\end{tabular}}").append("\n")
    	    			 .append("\\end{table*}").append("\n");
    }
    else
    {
    	latexTable_in_out.append("\\end{tabular}").append("\n")
    	    			 .append("\\end{table*}").append("\n");
    }


//    qDebug() << "++ latexTable_in_out in get_YEROTH_TableViewListingTexDocumentString: " << latexTable_in_out;
}

