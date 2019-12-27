/*
 * yeroth-erp-utils.cpp
 *
 *      Author: Xavier NOUMBISSI NOUNDOU, Dipl.-Inf., Ph.D. (ABD)
 */

#include "yeroth-erp-utils.hpp"

#include "src/widgets/yeroth-erp-qmessage-box.hpp"

#include "src/utils/yeroth-erp-database-table-column.hpp"

#include "yeroth-erp-logger.hpp"

#include "src/windows/yeroth-erp-window-commons.hpp"

#include "src/widgets/yeroth-erp-table-widget.hpp"

#include "src/widgets/yeroth-erp-line-edit.hpp"

#include "src/process/yeroth-erp-process.hpp"

#include "src/utils/yeroth-erp-config.hpp"

#include "src/utils/yeroth-erp-sqltable-model.hpp"

#include "src/admin/yeroth-erp-admin-windows-commons.hpp"

#include "src/yeroth-erp-windows.hpp"


#include <string>

#include <cassert>

#include <QtCore/qmath.h>

#include <QtCore/QDebug>

#include <QtCore/QDate>

#include <QtCore/QBuffer>

#include <QtCore/QProcess>

#include <QtSql/QSqlDriver>

#include <QtSql/QSqlRecord>

#include <QtSql/QSqlError>

#include <QtSql/QSqlQuery>

#include <QtWidgets/QStyle>

#include <QtWidgets/QStyleFactory>

#include <QtWidgets/QStylePlugin>

#include <QtWidgets/QLabel>

#include <QtWidgets/QDialog>

#include <QtWidgets/QFileDialog>

#include <QtWidgets/QMessageBox>

#include <QtWidgets/QApplication>

#include <QtWidgets/QMainWindow>

#include <QtPrintSupport/QPrintDialog>

#include <QtCore/QPluginLoader>

#include <QtCore/QLibraryInfo>

#include <QtCore/QtPlugin>



YerothERPWindows *YerothUtils::_allWindows(0);

QString YerothUtils::_mainWorkingDirectory("");

QString YerothUtils::_logFileName("");

QString YerothUtils::_1a_tex("");

QString YerothUtils::template_fiche_client_tex("");

QString YerothUtils::FR_template_historique_dun_stock_tex("");

QString YerothUtils::EN_template_historique_dun_stock_tex("");

QString YerothUtils::FR_template_journal_des_paiements_tex("");

QString YerothUtils::EN_template_journal_des_paiements_tex("");

QString YerothUtils::FR_template_journal_des_ventes_tex("");

QString YerothUtils::EN_template_journal_des_ventes_tex("");

QString YerothUtils::FR_template_journal_des_transactions_tex("");

QString YerothUtils::EN_template_journal_des_transactions_tex("");

QString YerothUtils::FR_template_sortie_des_stocks_grand_tex("");

QString YerothUtils::EN_template_sortie_des_stocks_grand_tex("");

QString YerothUtils::FR_template_sortie_des_stocks_petit_tex("");

QString YerothUtils::EN_template_sortie_des_stocks_petit_tex("");

QString YerothUtils::FR_template_comptes_clients_tex("");

QString YerothUtils::EN_template_comptes_clients_tex("");

QString YerothUtils::FR_template_marchandises_tex("");

QString YerothUtils::EN_template_marchandises_tex("");

QString YerothUtils::FR_template_lister_achats_tex("");

QString YerothUtils::EN_template_lister_achats_tex("");

QString YerothUtils::FR_template_lister_stock_tex("");

QString YerothUtils::EN_template_lister_stock_tex("");

QString YerothUtils::FR_template_facture_grand_tex("");

QString YerothUtils::EN_template_facture_grand_tex("");

QString YerothUtils::FR_template_facture_petit_tex("");

QString YerothUtils::EN_template_facture_petit_tex("");

QString YerothUtils::FR_pie_chart_tex("");

QString YerothUtils::FR_bar_chart_tex("");

QString YerothUtils::FR_bar_diag_tex("");

QString YerothUtils::EN_pie_chart_tex("");

QString YerothUtils::EN_bar_chart_tex("");

QString YerothUtils::EN_bar_diag_tex("");

const QKeySequence YerothUtils::MESSAGE_DAIDE_QKEYSEQUENCE(QObject::tr(SHORTCUT_USER_TIP));

const QKeySequence YerothUtils::IMPRIMER_QKEYSEQUENCE(QObject::tr(SHORTCUT_PRINT));

const QKeySequence YerothUtils::AFFICHER_LES_STOCKS_TERMINES_QKEYSEQUENCE(QObject::tr(SHORTCUT_AFFICHER_LES_STOCKS_TERMINES));

const QKeySequence YerothUtils::RECHERCHER_QKEYSEQUENCE(QObject::tr(SHORTCUT_SEARCH));

const QKeySequence YerothUtils::REINITIALISER_RECHERCHE_QKEYSEQUENCE(QObject::tr(SHORTCUT_REINITIALIAZE));

const QKeySequence YerothUtils::REINITIALISER_QUI_SUI_JE_QKEYSEQUENCE(QObject::tr(SHORTCUT_WHO_AM_I));

const QKeySequence YerothUtils::LISTER_STOCKS_QKEYSEQUENCE(QObject::tr(SHORTCUT_LIST_STOCKS));

const QString YerothUtils::IMPRIMANTE_PDF("pdf");

const QString YerothUtils::IMPRIMANTE_EPSON_TM_T20ii("epson TM-T20ii");

//This is 3 seconds (3000 milliseconds)
const unsigned int YerothUtils::PRINT_TIMEOUT(TIMEOUT_PRINT);

const QString YerothUtils::STRING_STYLESHEETS_YEROTH_ERP_3_0("QLabel { font-size: 17px; color: rgb(0, 255, 100); }");

const QString YerothUtils::STRING_STYLESHEET_COLOR_YEROTHGREEN("QLabel { color : rgb(0, 255, 100); }");

const QColor YerothUtils::YEROTH_WHITE_COLOR(QColor(COLOUR_RGB_YEROTH_WHITE_255_255_255));

const QColor YerothUtils::YEROTH_BLACK_COLOR(QColor(COLOUR_RGB_YEROTH_BLACK_0_0_0));

const QColor YerothUtils::YEROTH_BLUE_COLOR(QColor(COLOUR_RGB_YEROTH_BLUE_77_93_254));

const QColor YerothUtils::YEROTH_DARK_GREEN_COLOR(QColor(COLOUR_RGB_YEROTH_DARK_GREEN_47_67_67));

const QColor YerothUtils::YEROTH_GRAY_COLOR(QColor(COLOUR_RGB_YEROTH_GRAY_78_78_78));

const QColor YerothUtils::YEROTH_GREEN_COLOR(QColor(COLOUR_RGB_YEROTH_GREEN_0_255_100));

const QColor YerothUtils::YEROTH_YEROTHGREEN_COLOR(QColor(COLOUR_RGB_YEROTH_GREEN_2_160_70));

const QColor YerothUtils::YEROTH_ORANGE_COLOR(QColor(COLOUR_RGB_YEROTH_ORANGE_243_162_0));

const QColor YerothUtils::YEROTH_RED_COLOR(QColor(COLOUR_RGB_YEROTH_RED_178_34_34));

const QColor YerothUtils::YEROTH_INDIGO_COLOR(QColor(COLOUR_RGB_YEROTH_INDIGO_83_0_125));

const QBrush YerothUtils::YEROTH_QBRUSH_GREEN(YEROTH_GREEN_COLOR);

const QBrush YerothUtils::YEROTH_QBRUSH_YEROTHGREEN(YEROTH_YEROTHGREEN_COLOR);

const QBrush YerothUtils::YEROTH_QBRUSH_BLUE(YEROTH_BLUE_COLOR);

const QBrush YerothUtils::YEROTH_QBRUSH_ORANGE(YEROTH_ORANGE_COLOR);

const QBrush YerothUtils::YEROTH_QBRUSH_YEROTHGRAY(YEROTH_GRAY_COLOR);

const QPalette YerothUtils::YEROTH_BLACK_PALETTE(QColor(0, 0, 0));

const QPalette YerothUtils::YEROTH_BLUE_PALETTE(QColor(0, 0, 255));

const QPalette YerothUtils::YEROTH_DARK_GREEN_PALETTE(YEROTH_DARK_GREEN_COLOR);

const QPalette YerothUtils::YEROTH_GRAY_PALETTE(QColor(237, 237, 237));

const QPalette YerothUtils::YEROTH_GREEN_PALETTE(YEROTH_YEROTHGREEN_COLOR);

const QPalette YerothUtils::YEROTH_RED_PALETTE(YEROTH_RED_COLOR);

const QPalette YerothUtils::YEROTH_WHITE_PALETTE(QColor(COLOUR_RGB_YEROTH_WHITE_255_255_255));

const QLocale YerothUtils::frenchLocale(QLocale(QLocale::French, QLocale::France));

const QLocale YerothUtils::englishLocale(QLocale(QLocale::English, QLocale::Zambia));

const QString YerothUtils::TVA_0_STRING(STRING_TVA_0_PERCENT);

const QString YerothUtils::DATE_FORMAT(FORMAT_DATE);

const QString YerothUtils::TIME_FORMAT(FORMAT_TIME);

const QString YerothUtils::DB_DATE_FORMAT(FORMAT_DATE_DATABASE);

# ifdef YEROTH_FRANCAIS_LANGUAGE

const QString YerothUtils::NOUVEAU_CLIENT(QObject::trUtf8(STRING_NEW_CLIENT_FR));

const QString YerothUtils::NOUVEAU_FOURNISSEUR(QObject::trUtf8(STRING_NEW_SUPPLIER_FR));

const QString YerothUtils::NOUVELLE_CATEGORIE(QObject::trUtf8(STRING_NEW_CATEGORY_FR));

const QString YerothUtils::STRING_OUI(QObject::tr(STRING_TEXT_YES_FR));
const QString YerothUtils::STRING_NON(QObject::tr(STRING_TEXT_NO_FR));

const QString YerothUtils::INFERIEUR_OU_EGAL(QObject::trUtf8(STRING_MATH_INFERIOR_OR_EQUAL_FR));
const QString YerothUtils::SUPERIEUR(QObject::trUtf8(STRING_MATH_SUPERIOR_FR));

# else //YEROTH_ENGLISH_LANGUAGE

const QString YerothUtils::NOUVEAU_CLIENT(QObject::trUtf8(STRING_NEW_CLIENT_EN));

const QString YerothUtils::NOUVEAU_FOURNISSEUR(QObject::trUtf8(STRING_NEW_SUPPLIER_EN));

const QString YerothUtils::NOUVELLE_CATEGORIE(QObject::trUtf8(STRING_NEW_CATEGORY_EN));

const QString YerothUtils::STRING_OUI(QObject::tr(STRING_TEXT_YES_EN));
const QString YerothUtils::STRING_NON(QObject::tr(STRING_TEXT_NO_EN));

const QString YerothUtils::INFERIEUR_OU_EGAL(QObject::trUtf8(STRING_MATH_INFERIOR_OR_EQUAL_EN));
const QString YerothUtils::SUPERIEUR(QObject::trUtf8(STRING_MATH_SUPERIOR_EN));

# endif

const QString YerothUtils::MAGELAN_CLIENT		("com.yeroth.client-1");

const QString YerothUtils::MAGELAN_CLIENT_OBJECT ("/");

const QString YerothUtils::MAGELAN_SERVER		("com.yeroth.server");

const QString YerothUtils::MAGELAN_SERVER_OBJECT ("/");

#ifdef YEROTH_SERVER
const QString YerothUtils::APROPOS_MSG(QObject::trUtf8("YEROTH-ERP-3.0 SERVER (22 Septembre 2019)\n\n par \n\n (c) Diplom-Informatiker Xavier NOUMBISSI NOUNDOU, Ph.D. (ABD)."));
const QString YerothUtils::APPLICATION_NAME(QObject::trUtf8("YEROTH-ERP-3.0 SERVER"));
#endif

#ifdef YEROTH_CLIENT
const QString YerothUtils::APROPOS_MSG(QObject::trUtf8("YEROTH-ERP-3.0 CLIENT (22 Septembre 2019)\n\n par \n\n (c) Diplom-Informatiker Xavier NOUMBISSI NOUNDOU, Ph.D. (ABD)."));
const QString YerothUtils::APPLICATION_NAME(QObject::trUtf8("YEROTH-ERP-3.0 CLIENT"));
#endif

#ifdef YEROTH_STANDALONE
const QString YerothUtils::APROPOS_MSG(QObject::trUtf8("YEROTH-ERP-3.0 STANDALONE (22 Septembre 2019)\n\n par \n\n (c) Diplom-Informatiker Xavier NOUMBISSI NOUNDOU, Ph.D. (ABD)."));
const QString YerothUtils::APPLICATION_NAME(QObject::trUtf8("YEROTH-ERP-3.0 STANDALONE"));
#endif

#ifdef YEROTH_ACADEMIC_EVALUATION_VERSION
const QString YerothUtils::APROPOS_MSG(QObject::trUtf8("YEROTH-ERP-3.0 ACADEMIC EVALUATION VERSION (22 Septembre 2019)\n\n par \n\n (c) Diplom-Informatiker Xavier NOUMBISSI NOUNDOU, Ph.D. (ABD)."));
const QString YerothUtils::APPLICATION_NAME(QObject::trUtf8("YEROTH-ERP-3.0 ACADEMIC EVALUATION VERSION"));
#endif

const QString YerothUtils::LOCALHOST(STRING_LOCALHOST);

const QString YerothUtils::LOCALHOST_IP_ADDRESS(STRING_LOCALHOST_IP_ADDRESS);

const unsigned int YerothUtils::FACTURE_GRAND_STRING_MAX_CHARS(12);

const unsigned int YerothUtils::FACTURE_PETIT_NOM_ARTICLE_MAX_CHARS(12);

const unsigned int YerothUtils::STRING_MAX_CHARS(12);

const QChar YerothUtils::SLASH_CHAR('/');

const QRegExp YerothUtils::PasswordRegExp("");

const QRegExp YerothUtils::EMPTY_SPACE_REGEXP("\\s");

const QIntValidator YerothUtils::IntValidator(0, 444444444);
const QDoubleValidator YerothUtils::DoubleValidator(0, 444444444, 2);
const QRegExpValidator YerothUtils::PasswordValidator(PasswordRegExp);


bool YerothUtils::startTransaction()
{
    QSqlDriver *driver = _allWindows->getDatabase().driver();

    if (driver && driver->hasFeature(QSqlDriver::Transactions))
    {
    	bool couldStartDBTransaction =_allWindows->getDatabase().transaction();

    	if (!couldStartDBTransaction)
		{
        	qDebug() << QString("Couldn't start a database transaction !");
        	qDebug() << _allWindows->getDatabase().lastError();
		}

    	return couldStartDBTransaction;
    }

    return false;
}


bool YerothUtils::commitTransaction()
{
    bool couldCommitDBTransaction = _allWindows->getDatabase().commit();

    if (!couldCommitDBTransaction)
    {
    	qDebug() << QString("Couldn't commit a database transaction !");
    	qDebug() << _allWindows->getDatabase().lastError();
    }

    return couldCommitDBTransaction;
}


void YerothUtils::setAllWindows(YerothERPWindows *allWindows)
{
    _allWindows = allWindows;

    YerothPOSDialogCommons::setAllWindows(allWindows);

    YerothWindowsCommons::setAllWindows(allWindows);

    YerothPOSAdminWindowsCommons::setAllWindows(allWindows);
}


void YerothUtils::writeStringToQFilewithUTF8Encoding(QFile &latexContentOutputFile,
												   	QString aLatexFileContentString)
{
    if (latexContentOutputFile.open(QFile::WriteOnly))
    {
    	latexContentOutputFile.write(aLatexFileContentString.toUtf8());
    }

    latexContentOutputFile.close();
}


QString YerothUtils::getUniquePrefixFileInTemporaryFilesDir(QString aPrefixFileName)
{
	return QString("%1/%2.")
				.arg(YerothERPConfig::temporaryFilesDir,
					 FILE_NAME_USERID_CURRENT_TIME(aPrefixFileName));
}


const QString YerothUtils::getCurrentAdminWindowReceiptsFormatAccordingToLanguage(QString currentReceiptFormatValue)
{
    QString resultReceiptFormat(currentReceiptFormatValue);

# ifdef YEROTH_ENGLISH_LANGUAGE

    if (YerothUtils::isEqualCaseInsensitive(currentReceiptFormatValue,
                                           QObject::trUtf8(RECEIPT_FORMAT_SMALL_FR)))
    {
        resultReceiptFormat = QObject::tr(RECEIPT_FORMAT_SMALL_EN);
    }
    else if (YerothUtils::isEqualCaseInsensitive(currentReceiptFormatValue,
             QObject::trUtf8(RECEIPT_FORMAT_LARGE_A4PAPER_FR)))
    {
        resultReceiptFormat = QObject::tr(RECEIPT_FORMAT_LARGE_A4PAPER_EN);
    }

# else

    if (YerothUtils::isEqualCaseInsensitive(currentReceiptFormatValue,
                                           QObject::trUtf8(RECEIPT_FORMAT_SMALL_EN)))
    {
        resultReceiptFormat = QObject::trUtf8(RECEIPT_FORMAT_SMALL_FR);
    }
    else if (YerothUtils::isEqualCaseInsensitive(currentReceiptFormatValue,
             	 	 	 	 	 	 	 	 	QObject::trUtf8(RECEIPT_FORMAT_LARGE_A4PAPER_EN)))
    {
        resultReceiptFormat = QObject::tr(RECEIPT_FORMAT_LARGE_A4PAPER_FR);
    }

#endif

    return resultReceiptFormat;
}


void YerothUtils::saveCurrentAdminWindowFacturesTypeAccordingToLanguage(QString currentFacturesTypeValue)
{
	//qDebug() << QString("YerothUtils::saveCurrentAdminWindowFacturesTypeAccordingToLanguage");

    assert (0 != _allWindows);

    YerothSqlTableModel &configurationsTableModel = _allWindows->getSqlTableModel_configurations();

    QSqlRecord configurationsRecord(configurationsTableModel.record(YerothERPConfig::CONFIG_TYPE_OF_FACTURATION));

    QString currentFacturesTypeValue_FR;

# ifdef YEROTH_ENGLISH_LANGUAGE
    if (YerothUtils::isEqualCaseInsensitive(currentFacturesTypeValue,
                                           QObject::trUtf8(RECEIPT_FORMAT_SMALL_EN)))
    {
        currentFacturesTypeValue_FR = QObject::trUtf8(RECEIPT_FORMAT_SMALL_FR);
    }
    else if (YerothUtils::isEqualCaseInsensitive(currentFacturesTypeValue,
             QObject::trUtf8(RECEIPT_FORMAT_LARGE_A4PAPER_EN)))
    {
        currentFacturesTypeValue_FR = QObject::trUtf8(RECEIPT_FORMAT_LARGE_A4PAPER_FR);
    }

#else

    currentFacturesTypeValue_FR = currentFacturesTypeValue;

# endif

    //qDebug() << QString("++ saveCu... currentFacturesTypeValue_FR: %1")
    //				.arg(currentFacturesTypeValue_FR);

    configurationsRecord.setValue("valeur_configuration", currentFacturesTypeValue_FR);

    bool success = configurationsTableModel.updateRecord(YerothERPConfig::CONFIG_TYPE_OF_FACTURATION, configurationsRecord);

    if (success)
    {
        YerothERPConfig::receiptFormat = currentFacturesTypeValue;
    }
}


bool YerothUtils::executer_fichier_sql(const QString &fileName, YerothLogger *logger)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::critical(0, QObject::tr("Cannot open file"),
                              QObject::tr("Unable to open file establish a database connection.\n"
                                          "This example needs SQLite support. Please read "
                                          "the Qt SQL driver documentation for information how "
                                          "to build it.\n\n"
                                          "Click Cancel to exit."), QMessageBox::Cancel);
        return false;
    }

    static QSqlQuery query;
    QTextStream in(&file);

    query.clear();

    bool success = false;
    while (!in.atEnd())
    {
        QString line = in.readLine();
        success = query.exec(line);

        if (logger)
        {
            logger->log("YerothUtils::executer_fichier_sql",
                        QString("[%1] %2").arg(success ? "True" : "False", line));
        }

        if (!success)
        {
            if (logger)
            {
                logger->log("YerothUtils::executer_fichier_sql",
                            QString("[reason for failing] %1").arg(query.lastError().text()));
            }
        }
    }

    return true;
}

bool YerothUtils::execQuery(QString strQuery, YerothLogger *logger)
{
    //qDebug() << "[YerothUtils][execQuery]";

    QSqlQuery query;
    query.prepare(strQuery);
    bool success = query.exec();

    //qDebug() << "\t[" << success << "]" << query.executedQuery();

    if (logger)
    {
        logger->log("execQuery",
                    QString("[%1] %2").arg(BOOL_TO_STRING(success), query.executedQuery()));
    }

    if (!success)
    {
        if (logger)
        {
            *logger << "\t\t[reason for failing] " << query.lastError().text() << "\n";
        }
        else
        {
            qDebug() << "\t\t[reason for failing] " << query.lastError();
        }
    }

    return success;
}

int YerothUtils::execQuery(QSqlQuery &query, QString strQuery, YerothLogger *logger)
{
    //qDebug() << "[YerothUtils][execQuery]";

    query.prepare(strQuery);
    bool success = query.exec();

    //qDebug() << "\t[" << success << "]" << query.executedQuery();

    if (logger)
    {
        logger->log("execQuery",
                    QString("[%1] %2").arg(BOOL_TO_STRING(success), query.executedQuery()));
    }

    if (!success)
    {
        if (logger)
        {
            *logger << "\t\t[reason for failing] " << query.lastError().text() << "\n";
        }
        else
        {
            qDebug() << "\t\t[reason for failing] " << query.lastError();
        }
        return -1;
    }

    return query.size();
}

int YerothUtils::execQuery(QSqlQuery &query, YerothLogger *logger)
{
    //qDebug() << "[YerothUtils][execQuery]";

    bool success = query.exec();

    //qDebug() << "\t[" << success << "]" << query.executedQuery();

    if (logger)
    {
        logger->log("execQuery",
                    QString("[%1] %2").arg(BOOL_TO_STRING(success), query.executedQuery()));
    }

    if (!success)
    {
        if (logger)
        {
            *logger << "\t\t[reason for failing] " << query.lastError().text() << "\n";
        }
        else
        {
            qDebug() << "\t\t[reason for failing] " << query.lastError();
        }
        return -1;
    }

    return query.size();
}

int YerothUtils::getNextIdFromTable(const QString &tableName)
{
    //qDebug() << "[YerothUtils::getLastIdFromTable()] : next id from table '" << tableName << "'";
    QString strQuery("SELECT id FROM ");
    strQuery.append(tableName);

    QSqlQuery query(strQuery);
    QSqlRecord rec = query.record();

    if (query.last())
    {
        int lastId = query.value(0).toInt();
        ++lastId;
        //qDebug() << "\t next id 1: " << lastId;
        return lastId;
    }

    return 0;
}

int YerothUtils::getNextVentesIdFromTable(const QString &tableName)
{
    //qDebug() << "[YerothUtils::getLastIdFromTable()] : next id from table '" << tableName << "'";
    QString strQuery("SELECT vente_id FROM ");
    strQuery.append(tableName);

    QSqlQuery query(strQuery);
    QSqlRecord rec = query.record();

    if (query.last())
    {
        int lastId = query.value(0).toInt();
        ++lastId;
        //qDebug() << "\t next vente_id 1: " << lastId;
        return lastId;
    }

    return 0;
}

int YerothUtils::getNextSortiesIdFromTable(const QString &tableName)
{
    //qDebug() << "[YerothUtils::getLastIdFromTable()] : next id from table '" << tableName << "'";
    QString strQuery("SELECT sortie_id FROM ");
    strQuery.append(tableName);

    QSqlQuery query(strQuery);
    QSqlRecord rec = query.record();

    if (query.last())
    {
        int lastId = query.value(0).toInt();
        ++lastId;
        //qDebug() << "\t next sortie_id 1: " << lastId;
        return lastId;
    }

    return 0;
}

void YerothUtils::loadPixmapFromDB(QLabel &label_image,
                                  QVariant imageVariant,
                                  const char *format)
{
    QByteArray bytes;
    QBuffer buffer(&bytes);
    buffer.open(QIODevice::WriteOnly);
    QPixmap pixmap;
    pixmap.loadFromData(imageVariant.toByteArray(), format);
    label_image.setAlignment(Qt::AlignCenter);
    label_image.setFrameShape(QFrame::NoFrame);
    label_image.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    label_image.setBackgroundRole(QPalette::Background);
    label_image.setAutoFillBackground(true);
    label_image.setScaledContents(true);
    label_image.setPixmap(pixmap);
}

void YerothUtils::savePixmapToByteArray(QByteArray &bytes,
                                       const QPixmap &pixmap,
                                       const char *format)
{
    QBuffer buffer(&bytes);
    buffer.open(QIODevice::WriteOnly);
    pixmap.save(&buffer, format);
}

void YerothUtils::selectionner_image(QWidget *parent, QLabel &label_image)
{
    QString path;
    path = QFileDialog::getOpenFileName( parent, "Choisir une photo du produit",
                                         QString::null,
										 QObject::trUtf8("Images (*.png *.xpm *.jpg *.jpeg *.gif *.PNG *.XPM *.JPG *.JPEG *.GIF)"));
    QImage image;
    if (image.load(path))
    {
        QPixmap pixMap( QPixmap::fromImage(image) );
        label_image.setAlignment(Qt::AlignCenter);
        label_image.setFrameShape(QFrame::NoFrame);
        label_image.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        label_image.setBackgroundRole(QPalette::Background);
        label_image.setAutoFillBackground(true);
        label_image.setScaledContents(true);
        label_image.setPixmap(pixMap);
        label_image.setEnabled(true);
    }
}

QString YerothUtils::get_text_sql_escaped(const QVariant &qv)
{
    QString retStr;

    switch (qv.type())
    {
    case QVariant::UInt:
        return QString::number(qv.toUInt());

    case QVariant::Int:
        return QString::number(qv.toInt());

    case QVariant::Double:
        return QString::number(qv.toDouble(), 'f', 2);

    case QVariant::ULongLong:
        return QString::number(qv.toULongLong());

    case QVariant::LongLong:
        return QString::number(qv.toLongLong());

    case QVariant::Char:
        retStr = QString(qv.toChar());
        break;

    case QVariant::String:
        retStr = qv.toString();
        break;

    case QVariant::Bool:
        retStr.append(qv.toBool());
        break;

    case QVariant::Date:
        retStr = DATE_TO_STRING(qv.toDate());
        break;

    case QVariant::Time:
        retStr = TIME_TO_STRING(qv.toTime());
        break;

    default:
        retStr = QString(qv.type());
        break;
    }

    return YerothUtils::prepareSqlStr(retStr);
}

QString YerothUtils::get_text(const QVariant &qv)
{
    QString retStr;

    switch (qv.type())
    {
    case QVariant::UInt:
        return QString::number(qv.toUInt());

    case QVariant::Int:
        return QString::number(qv.toInt());

    case QVariant::Double:
        return QString::number(qv.toDouble(), 'f', 2);

    case QVariant::ULongLong:
        return QString::number(qv.toULongLong());

    case QVariant::LongLong:
        return QString::number(qv.toLongLong());

    case QVariant::Char:
        if (!qv.isNull())
        {
            return QString(qv.toChar());
        }
        break;

    case QVariant::String:
        if (!qv.isNull())
        {
            return QString(qv.toString());
        }
        break;

    case QVariant::Bool:
        retStr.append(qv.toBool());
        break;

    case QVariant::Date:
        return  DATE_TO_STRING(qv.toDate());

    case QVariant::Time:
        return TIME_TO_STRING(qv.toTime());

    default:
        break;
    }

    return retStr;
}

void YerothUtils::getCenterPosition(unsigned desktopWidth,
                                   unsigned desktopHeight,
                                   QWidget &aWidget,
                                   QPoint &result)
{
    int x = desktopWidth / 2 - aWidget.width() / 2;
    int y = desktopHeight / 2 - aWidget.height() / 2 - 25;
    result.setX(x);
    result.setY(y);
}

QString YerothUtils::boolToString(bool b)
{
    QString result;
    if (b)
    {
        result = "True";
    }
    else
    {
        result = "False";
    }
    return result;
}

void YerothUtils::createTableModelHeaders(QSqlTableModel &tableModel,
        QStandardItemModel &stdItemModel,
        QStringList &tableModelHeaders)
{
    tableModelHeaders.clear();

    //Nous mettons les noms des colones
    for (unsigned k = 0; k < tableModel.columnCount(); ++k)
    {
        QString strHdr(tableModel.record(0).fieldName(k));

        strHdr = YerothDatabaseTableColumn::_tableColumnToUserViewString.value(strHdr);

        tableModelHeaders.append(strHdr);
        stdItemModel.setHeaderData(k, Qt::Horizontal, strHdr);
    }
}

QString YerothUtils::generateSqlLike(QString sqlTableColumn,
                                    QString searchStr)
{
    QString result;

    searchStr.replace("'", "''");

    result.append(sqlTableColumn)
    .append(" LIKE '%").append(searchStr).append("%' ");

    return result;
}

QString YerothUtils::generateSqlIs(QString sqlTableColumn,
                                  QString searchStr)
{
    QString result;

    searchStr.replace("'", "''");

    result.append(sqlTableColumn)
    .append(" = \'").append(searchStr).append("\'");

    return result;
}

QString YerothUtils::generateSqlIsNotEmpty(QString sqlTableColumn)
{
    QString result;
    result.append(sqlTableColumn)
    .append(" != \'\'");

    return result;
}

void YerothUtils::addFiltre(YerothLineEdit *aLineEdit,
							QString filterName,
							QString &yerothFiltre_in_out)
{
    if (aLineEdit && !aLineEdit->text().isEmpty())
    {
        yerothFiltre_in_out.append(QString("%1: %2")
        								.arg(filterName, aLineEdit->text()))
        .append(", ");
    }
}

void YerothUtils::populateComboBox(QComboBox &aComboBox,
                                  const QString &tableName,
                                  const char *fieldName)
{
    static QStringList curItems;

    aComboBox.clear();

    QString strQuery("select ");
    strQuery.append(fieldName).append(" from ")
    .append(tableName).append(" order by `").append(fieldName).append("` asc");

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

    aComboBox.addItems(curItems);
    curItems.clear();
}


/*
 * This is used when the argument 'aContent' must be the first
 * element of the combo box when modifying an existing set
 * of combo box elements.
 */
void YerothUtils::populateComboBoxMissing(QComboBox &aComboBox,
        const QString &aContent,
        const QString &tableName,
        const char *fieldName)
{
    static QStringList curItems;

    QString strQuery("select ");
    strQuery.append(fieldName).append(" from ")
    .append(tableName).append(" order by `").append(fieldName).append("` asc");

    QSqlQuery query;
    query.prepare(strQuery);
    bool success = query.exec();
    //qDebug() << "[" << success << "]" << query.executedQuery();

    if (success)
    {
        QSqlRecord rec = query.record();

        QString content;
        curItems.append(aContent);

        while (query.next())
        {
            content = query.value(0).toString();

            if (content != aContent)
            {
                curItems.append(content);
            }
        }
    }

    aComboBox.addItems(curItems);

    curItems.clear();
}

void YerothUtils::getColumnListString(QStringList &columnStringList,
                                     const QString &tableName,
                                     const char *fieldName)
{
    columnStringList.clear();

    QString strQuery("select ");
    strQuery.append(fieldName).append(" from ")
    .append(tableName).append(" order by `").append(fieldName).append("` asc");
    //qDebug() << "++ query: " << strQuery;
    QSqlQuery query;
    query.prepare(strQuery);
    bool success = query.exec();
    //qDebug() << "[" << success << "]" << query.executedQuery();

    if (success)
    {
        QSqlRecord rec = query.record();

        while (query.next())
        {
            QString currentItem(query.value(0).toString());

            if (!currentItem.isEmpty() &&
                    !columnStringList.contains(currentItem))
            {
                columnStringList.append(currentItem);
            }
        }
    }
}

void YerothUtils::yerothSetWidgetColor(QWidget *aWidget)
{
    if (!aWidget)
    {
        return;
    }

    /*if (aWidget->isEnabled())
    {
    	aWidget->setPalette(YerothUtils::YEROTH_WHITE_PALETTE);
    }
    else
    {
    	aWidget->setPalette(YerothUtils::YEROTH_BLACK_PALETTE);
    }*/
}

QString YerothUtils::getFileNameWithCurrentTime(const QString fileName)
{
    QString resultFileName(fileName);

    resultFileName.append(QDate::currentDate().toString("-yyyy-MM-dd"));
    resultFileName.append(QTime::currentTime().toString("_HH-mm-ss"));

    //qDebug() << "++ " << resultFileName;

    return resultFileName;
}

QString YerothUtils::getFileNameWithUserIDAndCurrentTime(const QString fileName)
{
    QString resultFileName(fileName);

    YerothPOSUser *aUser = 0;

    if (0 != YerothUtils::_allWindows)
    {
        aUser = YerothUtils::_allWindows->getUser();

        if (0 != aUser)
        {
            resultFileName.append(QString("-%1").arg(aUser->nom_utilisateur()));
        }
    }

    resultFileName = YerothUtils::getFileNameWithCurrentTime(resultFileName);

    return resultFileName;
}

QString YerothUtils::getWindowTitleWithStrategy(QMainWindow *aMainWindow, QString localVisibleStrategy/* = ""*/)
{
    QString aNewTitle(aMainWindow->windowTitle());

    QString currentStockListingStrategy(localVisibleStrategy);

    if (localVisibleStrategy.isEmpty())
    {
        currentStockListingStrategy = YerothERPConfig::salesStrategy;
    }

    if (aNewTitle.contains(YerothERPConfig::STRATEGIE_VENTE_SORTIE_ALL))
    {
        aNewTitle.replace(YerothERPConfig::STRATEGIE_VENTE_SORTIE_ALL, currentStockListingStrategy);
    }
    else if (aNewTitle.contains(YerothERPConfig::STRATEGIE_VENTE_SORTIE_DEF_DEO))
    {
        aNewTitle.replace(YerothERPConfig::STRATEGIE_VENTE_SORTIE_DEF_DEO, currentStockListingStrategy);
    }
    else if (aNewTitle.contains(YerothERPConfig::STRATEGIE_VENTE_SORTIE_FIFO))
    {
        aNewTitle.replace(YerothERPConfig::STRATEGIE_VENTE_SORTIE_FIFO, currentStockListingStrategy);
    }
    else if (aNewTitle.contains(YerothERPConfig::STRATEGIE_VENTE_SORTIE_LIFO))
    {
        aNewTitle.replace(YerothERPConfig::STRATEGIE_VENTE_SORTIE_LIFO, currentStockListingStrategy);
    }
    else
    {
        if (!aNewTitle.contains(YerothERPConfig::salesStrategy))
        {
            aNewTitle.append(" - ").append(currentStockListingStrategy);
        }
    }

    return aNewTitle;
}

QString YerothUtils::appendPDFReceiptFormat(QString aStr)
{
    QString resultStr(aStr);

    if (resultStr.contains(YerothERPConfig::RECEIPT_FORMAT_GRAND_A4PAPER))
    {
        resultStr.replace(YerothERPConfig::RECEIPT_FORMAT_GRAND_A4PAPER, YerothERPConfig::receiptFormat);
    }
    else if (resultStr.contains(YerothERPConfig::RECEIPT_FORMAT_PETIT))
    {
        resultStr.replace(YerothERPConfig::RECEIPT_FORMAT_PETIT, YerothERPConfig::receiptFormat);
    }
    else
    {
        if (!resultStr.contains(YerothERPConfig::receiptFormat))
        {
# ifdef YEROTH_FRANCAIS_LANGUAGE

            resultStr.append("\t[ factures pdf au format: '").append(YerothERPConfig::receiptFormat).append("' ]");

# elif YEROTH_ENGLISH_LANGUAGE

            resultStr.append("\t[ pdf receipt format: '").append(YerothERPConfig::receiptFormat).append("' ]");

# endif
        }
    }

    return resultStr;
}


void YerothUtils::infosEntreprise(YerothPOSAdminWindowsCommons &aYerothPOSAdminQMainWindow,
                                 const QString infos)
{
#ifdef YEROTH_FRANCAIS_LANGUAGE
    YerothQMessageBox::information(&aYerothPOSAdminQMainWindow,
                                  QString("Informations sur l'entreprise"),
                                  infos);
#endif

#ifdef YEROTH_ENGLISH_LANGUAGE
    YerothQMessageBox::information(&aYerothPOSAdminQMainWindow,
                                  QString("Information about the company"),
                                  infos);
#endif
}

void YerothUtils::infosEntreprise(YerothWindowsCommons &aYerothPOSQMainWindow,
                                 const QString infos)
{
#ifdef YEROTH_FRANCAIS_LANGUAGE
    YerothQMessageBox::information(&aYerothPOSQMainWindow,
                                  QString("Informations sur l'entreprise"),
                                  infos);
#endif

#ifdef YEROTH_ENGLISH_LANGUAGE
    YerothQMessageBox::information(&aYerothPOSQMainWindow,
                                  QString("Information about the company"),
                                  infos);
#endif
}

void YerothUtils::getCurrentLocaleDate(QString &date)
{
#ifdef YEROTH_FRANCAIS_LANGUAGE
    date.append(", le ")
    .append(YerothUtils::frenchLocale.toString(GET_CURRENT_DATE));
    //qDebug() << "++ before date: " << date << " (" << date.indexOf(QString::fromUtf8("û")) << ")";
    date = YerothUtils::handleForeignAccents(date);
    //qDebug() << "++ after date: " << date << " (" << date.indexOf(QString::fromUtf8("û")) << ")";
#endif

#ifdef YEROTH_ENGLISH_LANGUAGE
    QString d(YerothUtils::englishLocale.toString(GET_CURRENT_DATE));
    int firstCommaIndex = d.indexOf(",", 0);
    d.replace(firstCommaIndex, 1, " ");
    date.append(", ")
    .append(d);
    date = YerothUtils::handleForeignAccents(date);
#endif
}

bool YerothUtils::slot_connecter_localisation(QWidget &aWidget,
        YerothERPWindows *allWindows,
        const QString localisation)
{
    YerothSqlTableModel &localisationsSqlTableModel =
        allWindows->getSqlTableModel_localisations();

    localisationsSqlTableModel.yerothSetFilter(GENERATE_SQL_IS_STMT("nom_localisation", localisation));

    int rowCount = localisationsSqlTableModel.easySelect();

    if (rowCount <= 0)
    {
        //qDebug() << "++ pas de localisation au nom de: " << localisation;
        return false;
    }

    QSqlRecord localisationsRecord = localisationsSqlTableModel.record(0);

    QString addresseIP(GET_SQL_RECORD_DATA(localisationsRecord, "adresse_ip"));

    QString widgetTitle(aWidget.windowTitle());

    if (addresseIP.isEmpty())
    {
        QString retMsg;
        retMsg.append(QString(QObject::trUtf8("La localisation %1 n'a pas d'adresse IP enregistrée !"))
                      .arg(localisation));

        QString msg(QString(QObject::tr("%1 - pas d'adresse IP !"))
        				.arg(widgetTitle));

        QMessageBox::information(&aWidget,
                                 "Connection par adresse IP",
                                 retMsg);

        return false;
    }
    else
    {
        if ("127.0.0.1" == addresseIP)
        {
            QString retMsg;
            retMsg.append(QString(QObject::trUtf8("La localisation ''%1'' est "
                                  "déjà connectée par l'adresse loopback (127.0.0.1) !"))
                          .arg(localisation));

            QMessageBox::information(&aWidget,
                                     "Connection par adresse IP",
                                     retMsg);

            return false;
        }
    }

    YerothDatabase &database = allWindows->getDatabase();

    database.close();

    database.set_db_ip_address(addresseIP);

    if (database.open())
    {
        YerothERPConfig::_distantSiteConnected = true;

        YerothERPConfig::_connectedSiteIPAddress = addresseIP;

        YerothERPConfig::_connectedSite = localisation;

        allWindows->reinitialiseSqlTableModels();

        QString retMsg(QString(QObject::trUtf8("Connecter à la localisation %1 !"))
        				.arg(localisation));

        QString msg(QString(QObject::trUtf8("%1 - Connection à la localisation !"))
        				.arg(widgetTitle));

        QMessageBox::information(&aWidget, msg, retMsg);

        return true;
    }
    else
    {
        QString retMsg(QString(QObject::trUtf8("Impossible de se connecter à la localisation %1 !"))
                      .arg(localisation));

        QString msg(QString(QObject::trUtf8("%1 - Pas de connection à la localisation !"))
                    .arg(widgetTitle));

        QMessageBox::information(&aWidget, msg, retMsg);

        qDebug() << database.lastError();

        slot_deconnecter_localisation(allWindows);
    }

    return false;
}

bool YerothUtils::slot_deconnecter_localisation(YerothERPWindows *allWindows)
{
    YerothDatabase &database = allWindows->getDatabase();

    database.close();

    YerothERPConfig::_distantSiteConnected = false;

    YerothERPConfig::_connectedSite.clear();

    YerothERPConfig::_connectedSite = allWindows->getInfoEntreprise().getLocalisation();

    database.set_db_ip_address(YerothERPConfig::_db_ip_address);

    if (!database.open())
    {
        QString errMsg(QString(QObject::trUtf8("Impossible de se connecter au serveur '%1' "
        									   "de base de données !\n"
        									   "Contacter l'administrateur de yeroth\n\n"
        									   "Cliquer sur 'Cancel' pour terminer %2"))
        					.arg(database.db_type(),
        						 YerothUtils::APPLICATION_NAME));

        QMessageBox::critical(0, YerothUtils::APPLICATION_NAME, errMsg, QMessageBox::Cancel);

        qDebug() << database.lastError();

        return false;
        //logger.log("main",
        //QString("Impossible de se connecter au serveur MYSQL: %1 !").arg(db.lastError().text()));
    }
    //else
    //{
    allWindows->reinitialiseSqlTableModels();

    return true;
    //}
}

QString YerothUtils::handleForeignAccents(const QString &texText_in)
{
    QString tempText(texText_in);
    tempText = tempText.isEmpty() ? "\"\"" : tempText.replace("_", "\\_").replace(EMPTY_SPACE_REGEXP, "\\ ");

    tempText.replace(QString::fromUtf8("&"), "\\&");
    tempText.replace(QString::fromUtf8("$"), "\\$");

	tempText.replace(QString::fromUtf8("ç"), "\\c{c}");

	tempText.replace(QString::fromUtf8("è"), "\\`e");
    tempText.replace(QString::fromUtf8("é"), "\\'e");
    tempText.replace(QString::fromUtf8("ê"), "\\^e");
    tempText.replace(QString::fromUtf8("à"), "\\`a");
    tempText.replace(QString::fromUtf8("â"), "\\^a");
    tempText.replace(QString::fromUtf8("ô"), "\\^o");
    tempText.replace(QString::fromUtf8("î"), "\\^i");
    tempText.replace(QString::fromUtf8("û"), "\\^u");

    tempText.replace(QString::fromUtf8("È"), "\\`E");
    tempText.replace(QString::fromUtf8("É"), "\\'E");
    tempText.replace(QString::fromUtf8("Ê"), "\\^E");
    tempText.replace(QString::fromUtf8("À"), "\\`A");
    tempText.replace(QString::fromUtf8("Â"), "\\^A");
    tempText.replace(QString::fromUtf8("Ô"), "\\^O");
    tempText.replace(QString::fromUtf8("Î"), "\\^I");
    tempText.replace(QString::fromUtf8("Û"), "\\^U");

    return tempText;
}


void YerothUtils::cleanUpTexTableLastString(QString &texTable_in_out)
{
    int lastPos = texTable_in_out.size() - 1;

    if (YerothUtils::isEqualCaseInsensitive("&", texTable_in_out.at(lastPos)))
    {
        texTable_in_out.remove(lastPos, 1);
    }
}


void YerothUtils::handleComptesClientsTexTableItemText(int texTableColumnCount,
        									   	   	   QString &texTable_in_out,
													   int itemTextColumnPosition,
													   QString &itemText)
{
    QString resultItemText(itemText);

    if (!itemText.isEmpty())
    {
        resultItemText = handleForeignAccents(resultItemText);

        texTable_in_out.append(resultItemText);
    }

    if (itemTextColumnPosition < texTableColumnCount - 1)
    {
        texTable_in_out.append(" &");
    }
    else
    {
        texTable_in_out.append(" \\\\").append("\n");
    }
}


void YerothUtils::handleAchatsTexTableItemText(int texTableColumnCount,
        									   QString &texTable_in_out,
											   int itemTextColumnPosition,
											   QString &itemText)
{
    QString resultItemText(itemText);

    if (!itemText.isEmpty())
    {
        resultItemText = handleForeignAccents(resultItemText);

        texTable_in_out.append(resultItemText);
    }

    if (itemTextColumnPosition < texTableColumnCount - 1)
    {
        texTable_in_out.append(" &");
    }
    else
    {
        texTable_in_out.append(" \\\\").append("\n");
    }
}


void YerothUtils::handleFactureTexTableItemText(int texTableColumnCount,
        QString &texTable_in_out,
        int itemTextColumnPosition,
        QString &itemText)
{
    QString resultItemText(itemText);

    if (!itemText.isEmpty())
    {
        resultItemText = handleForeignAccents(resultItemText);

        texTable_in_out.append(resultItemText);
    }

    if (itemTextColumnPosition < texTableColumnCount - 1)
    {
        texTable_in_out.append(" &");
    }
    else
    {
        texTable_in_out.append(" \\\\").append("\n");
    }
}

void YerothUtils::handleTexTableItemText(int texTableColumnCount,
                                        QString &texTable_in_out,
                                        int itemTextColumnPosition,
                                        QString &itemText)
{
    QString resultItemText(handleForeignAccents(itemText));

    if (!resultItemText.isEmpty())
    {
        texTable_in_out.append(resultItemText);
    }

    if (itemTextColumnPosition < texTableColumnCount - 1)
    {
        texTable_in_out.append(" &");
    }
    else
    {
        texTable_in_out.append(" \\\\").append("\n");
    }
}

void YerothUtils::getFactureTexTableString(QString &texTable_in_out,
        QTableWidget &tableStandardItemModel,
        int 	 quantiteVendue,
        double totalTVA,
        double sommeTotal)
{
    texTable_in_out.append("\\begin{table*}[!htbp]").append("\n")
    .append("\\centering").append("\n")
    .append("\\begin{tabular}")
    .append("{|");

    /**
     * We reduce the header items of the table by 1 element
     * (at position 4: stock quantity field) because the
     * stock quantity field shall not be part of the
     * information printed to the customer.
     */

    int texTableColumnCount = tableStandardItemModel.columnCount();

    //Tex table header
    for (int k = 0; k < texTableColumnCount; ++k)
    {
        if (k == YerothTableWidget::QTE_TOTAL_STOCK)
        {
            continue;
        }

        if (YerothTableWidget::TOTAL_COLUMN == k   	||
                YerothTableWidget::TOTAL_TVA_COLUMN == k )
        {
            texTable_in_out.append("r|");
        }
        else
        {
            texTable_in_out.append("r|");
        }
    }

    texTable_in_out.append("} \\hline").append("\n"); //Table header end

    QTableWidgetItem *item;

    for (int k = 0; k < tableStandardItemModel.columnCount(); ++k)
    {
        if (k == YerothTableWidget::QTE_TOTAL_STOCK)
        {
            continue;
        }

        item = tableStandardItemModel.horizontalHeaderItem(k);
        if (item)
        {
            QString emptyString;
            handleTexTableItemText(tableStandardItemModel.columnCount(),
                                   texTable_in_out,
                                   k,
                                   emptyString);
        }
    }
    for (int k = 0; k < tableStandardItemModel.columnCount(); ++k)
    {
        if (k == YerothTableWidget::QTE_TOTAL_STOCK)
        {
            continue;
        }

        item = tableStandardItemModel.horizontalHeaderItem(k);
        if (item)
        {
            QString itemText(item->text().prepend("\\textbf{").append("}"));
            handleTexTableItemText(tableStandardItemModel.columnCount(),
                                   texTable_in_out,
                                   k,
                                   itemText);
        }
    }
    for (int k = 0; k < tableStandardItemModel.columnCount(); ++k)
    {
        if (k == YerothTableWidget::QTE_TOTAL_STOCK)
        {
            continue;
        }

        item = tableStandardItemModel.horizontalHeaderItem(k);
        if (item)
        {
            QString emptyString;
            handleTexTableItemText(tableStandardItemModel.columnCount(),
                                   texTable_in_out,
                                   k,
                                   emptyString);
        }
    }

    cleanUpTexTableLastString(texTable_in_out);

    texTable_in_out.append("\\hline").append("\n");

    //Tex table body

    for (int j = 0; j < tableStandardItemModel.rowCount(); ++j)
    {
        for (int k = 0; k < tableStandardItemModel.columnCount(); ++k)
        {
            if (k == YerothTableWidget::QTE_TOTAL_STOCK)
            {
                continue;
            }

            item = tableStandardItemModel.item(j, k);
            if (item)
            {
                QString itemText(item->text());

                if (itemText.length() > YerothUtils::FACTURE_GRAND_STRING_MAX_CHARS)
                {
                    itemText.truncate(YerothUtils::FACTURE_GRAND_STRING_MAX_CHARS-1);
                    itemText.append(".");
                    //qDebug() << QString("article name after truncate: %1").arg(articleName);
                }

                handleFactureTexTableItemText(tableStandardItemModel.columnCount(),
                                              texTable_in_out,
                                              k,
                                              itemText);
            }
            else
            {
                if (k < tableStandardItemModel.columnCount() - 1)
                {
                    texTable_in_out.append("\"\"").append(" &");
                }
                else
                {
                    texTable_in_out.append("\"\"").append(" \\hline").append("\n");
                }
            }
        }

        cleanUpTexTableLastString(texTable_in_out);

        texTable_in_out.append("\\hline").append("\n");
    }

    QString quantiteVendueStr(handleForeignAccents(GET_NUM_STRING(quantiteVendue)));
    quantiteVendueStr.prepend("\\textbf{").append("}");

    QString totalTVAStr(handleForeignAccents(GET_DOUBLE_STRING(totalTVA)));
    totalTVAStr.prepend("\\textbf{").append("}");

    QString sommeTotalStr(handleForeignAccents(GET_CURRENCY_STRING_NUM(sommeTotal)));
    sommeTotalStr.prepend("\\textbf{").append("}");

    QString totalStr;

#ifdef YEROTH_FRANCAIS_LANGUAGE
    totalStr.append("TOTAUX");
#endif

#ifdef YEROTH_ENGLISH_LANGUAGE
    totalStr.append("TOTAL");
#endif

    texTable_in_out.append("\\multicolumn{").append(GET_NUM_STRING(tableStandardItemModel.columnCount()-5)).append("}")
    										.append("{|c|}")
											.append(QString("{\\textbf{%1}}").arg(totalStr))
				   .append(" & ")
				   .append(quantiteVendueStr).append(" & ")
				   .append(" & ")
				   .append(totalTVAStr).append(" & ")
				   .append(sommeTotalStr).append("\\\\");

    texTable_in_out.append(" \\hline").append("\n");

    //Removes the empty character "" from Latex output
    texTable_in_out.replace("\"\"", "");

    texTable_in_out.append("\\end{tabular}").append("\n")
    .append("\\end{table*}").append("\n");
}

void YerothUtils::getFactureSmallTexTableString(QString &texTable_in_out,
        									   QTableWidget &tableStandardItemModel,
											   int 	 quantiteVendue,
											   double totalTVA,
											   double sommeTotal,
											   double montantRecuDuCient, /* = 0.0 */
											   double montantARembourserAuClient /* = 0.0 */)
{
    texTable_in_out.append("\\begin{table*}[!htbp]").append("\n")
    .append("\\centering").append("\n")
    .append("\\begin{tabular}")
    .append("{lrr}");

    texTable_in_out.append("\n"); //Table header end

    int tableColumnCount = tableStandardItemModel.columnCount();

    QTableWidgetItem *item = 0;
    for (int k = 0; k < tableColumnCount; ++k)
    {
        if (k == YerothTableWidget::DESIGNATION_COLUMN ||
                k == YerothTableWidget::QTE_COLUMN 		  ||
                k == YerothTableWidget::TOTAL_COLUMN)
        {
            item = tableStandardItemModel.horizontalHeaderItem(k);
            if (item)
            {
                QString itemText(item->text());
                handleTexTableItemText(tableStandardItemModel.columnCount(),
                                       texTable_in_out,
                                       k,
                                       itemText);
            }
        }
    }
    cleanUpTexTableLastString(texTable_in_out);

    texTable_in_out.append("\\hline").append("\n");

    QString articleName;
    int articleNameLength;

    QTableWidgetItem *articleNameItem;
    QTableWidgetItem *articleQteItem;
    QTableWidgetItem *totalTtcItem;

    int tableRowCount = tableStandardItemModel.rowCount();

    for (int k = 0; k < tableRowCount; ++k)
    {
        articleNameItem = tableStandardItemModel.item(k, YerothTableWidget::DESIGNATION_COLUMN);
        articleQteItem = tableStandardItemModel.item(k, YerothTableWidget::QTE_COLUMN);
        totalTtcItem = tableStandardItemModel.item(k, YerothTableWidget::TOTAL_COLUMN);

        if (articleNameItem && articleQteItem && totalTtcItem)
        {
            articleName = articleNameItem->text();
            articleNameLength = articleName.length();

            //qDebug() << QString("article name: %1, articleNameLength: %2")
            //				.arg(articleName, articleNameLength);

            if (articleNameLength > YerothUtils::FACTURE_PETIT_NOM_ARTICLE_MAX_CHARS)
            {
                articleName.truncate(YerothUtils::FACTURE_PETIT_NOM_ARTICLE_MAX_CHARS-1);
                //qDebug() << QString("article name after truncate: %1").arg(articleName);
            }

            texTable_in_out.append(QString("%1. %2 & $x%3$ & %4 \\\\ \n")
                                   .arg(QString::number(k+1),
                                        YerothUtils::handleForeignAccents(articleName),
                                        articleQteItem->text(),
                                        YerothUtils::handleForeignAccents(totalTtcItem->text())));
        }
    }

    //We now add the sum
    QString sommeTotalStr(YerothUtils::handleForeignAccents(GET_CURRENCY_STRING_NUM(sommeTotal)));

    texTable_in_out.append(QString("\\hline\n & $%1$ & %2 \\\\ \n")
                           .arg(QString::number(quantiteVendue), sommeTotalStr));

    texTable_in_out.append(QString(" & & \\\\ \n"));

    QString totalTVAStr(YerothUtils::handleForeignAccents(GET_CURRENCY_STRING_NUM(totalTVA)));

    QString montantRecuDuClientStr(YerothUtils::handleForeignAccents(GET_CURRENCY_STRING_NUM(montantRecuDuCient)));

    QString montantARembourserAuClientStr;

    if (montantARembourserAuClient >= 0)
    {
    	montantARembourserAuClientStr =
    			YerothUtils::handleForeignAccents(GET_CURRENCY_STRING_NUM(montantARembourserAuClient));
    }
    else
    {
    	montantARembourserAuClientStr =
    			YerothUtils::handleForeignAccents(GET_CURRENCY_STRING_NUM(0.0));
    }


#ifdef YEROTH_FRANCAIS_LANGUAGE
    texTable_in_out.append(QString("dont total TVA: & & %1\\\\ \n").arg(totalTVAStr));

    texTable_in_out.append(QString("Montant re\\c{c}u: & & %1\\\\ \n").arg(montantRecuDuClientStr));

    texTable_in_out.append(QString("Montant remi: & & %1\\ \n").arg(montantARembourserAuClientStr));


#endif

#ifdef YEROTH_ENGLISH_LANGUAGE
    texTable_in_out.append(QString("with total taxes: & & %1\\\\ \n").arg(totalTVAStr));

    texTable_in_out.append(QString("Received amount: & & %1\\\\ \n").arg(montantRecuDuClientStr));

    texTable_in_out.append(QString("Amout given back: & & %1\\ \n").arg(montantARembourserAuClientStr));
#endif

    texTable_in_out.append("\\end{tabular}").append("\n")
    .append("\\end{table*}").append("\n");
}


void YerothUtils::getComptesClientsListingTexTableString(QString &texTable_in_out,
        										 	 	 QStandardItemModel &tableStandardItemModel,
														 QList<int> &columnsToIgnore,
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

    texTable_in_out.append("\\begin{table*}[!htbp]").append("\n")
    			   .append("\\centering").append("\n")
				   .append("\\begin{tabular}")
				   .append("{|");

    int texTableColumnCount = tableStandardItemModel.columnCount() + 1;

    //Tex table header
    for (int k = 0; k < texTableColumnCount; ++k)
    {
        if (columnsToIgnore.contains(k))
        {
            continue;
        }

        texTable_in_out.append("r|");
    }

    texTable_in_out.append("} \\hline");//.append("\n");

    /** We add a column named 'id' for numbering the rows
     * in the Tex table. */
    unsigned int id = fromRowIndex + 1;
    texTable_in_out.append("\\textbf{id} & ");

    QStandardItem *item;

    for (int k = 0; k < texTableColumnCount; ++k)
    {
        if (columnsToIgnore.contains(k))
        {
            continue;
        }

        item = tableStandardItemModel.horizontalHeaderItem(k);

        if (item)
        {
            QString itemText(item->text().prepend("\\textbf{").append("}"));
            handleTexTableItemText(tableStandardItemModel.columnCount(),
                                   texTable_in_out,
                                   k,
                                   itemText);
        }
    }
    /** Closing Tex table header */
    cleanUpTexTableLastString(texTable_in_out);

    texTable_in_out.append(" \\hline");

    //qDebug() << QString("++ fromRowIndex: %1, toRowIndex: %2")
    //			.arg(QString::number(fromRowIndex), QString::number(toRowIndex));

    for (int j = fromRowIndex; j < toRowIndex; ++j)
    {
        texTable_in_out.append(QString::number(id));
        texTable_in_out.append(" &");
        ++id;

        for (int k = 0; k < tableStandardItemModel.columnCount(); ++k)
        {
            if (columnsToIgnore.contains(k))
            {
                continue;
            }

            item = tableStandardItemModel.item(j, k);
            if (item)
            {
                QString itemText(item->text());
                handleComptesClientsTexTableItemText(tableStandardItemModel.columnCount(),
                                              texTable_in_out,
                                              k,
                                              itemText);
            }
            else
            {
                if (k < tableStandardItemModel.columnCount() - 1)
                {
                    texTable_in_out.append("\"\"").append(" &");
                }
                else
                {
                    texTable_in_out.append("\"\"").append("\\hline");
                }
            }
        }

        texTable_in_out = texTable_in_out.trimmed();

        cleanUpTexTableLastString(texTable_in_out);

        texTable_in_out.append("\\hline");
    }

    //Removes the empty character "" from Latex output
    texTable_in_out.replace("\"\"", "");

    texTable_in_out.append("\\end{tabular}").append("\n")
    			   .append("\\end{table*}").append("\n");

    //qDebug() << "++ texTable_in_out in getStocksListingTexTableString: " << texTable_in_out;
}


void YerothUtils::getInventoryStocksListingTexTableString(QString &texTable_in_out,
        QStandardItemModel &tableStandardItemModel,
        QList<int> &columnsToIgnore,
        int fromRowIndex,
        int toRowIndex,
        bool lastPage)
{
    texTable_in_out.append("\\begin{table*}[!htbp]").append("\n")
    .append("\\centering").append("\n")
    .append("\\begin{tabular}")
    .append("{|");

    int texTableColumnCount = tableStandardItemModel.columnCount() + 1;

    //Tex table header
    for (int k = 0; k < texTableColumnCount; ++k)
    {
        if (columnsToIgnore.contains(k))
        {
            continue;
        }

        texTable_in_out.append("r|");
    }

    texTable_in_out.append("} \\hline").append("\n");

    /** We add a column named 'id' for numbering the rows
     * in the Tex table. */
    unsigned int id = 1;
    texTable_in_out.append("\\textbf{id} & ");

    QStandardItem *item;

    for (int k = 0; k < texTableColumnCount; ++k)
    {
        if (columnsToIgnore.contains(k))
        {
            continue;
        }

        item = tableStandardItemModel.horizontalHeaderItem(k);
        if (item)
        {
            QString itemText(item->text().prepend("\\textbf{").append("}"));
            handleTexTableItemText(tableStandardItemModel.columnCount(),
                                   texTable_in_out,
                                   k,
                                   itemText);
        }
    }
    /** Closing Tex table header */
    cleanUpTexTableLastString(texTable_in_out);

    texTable_in_out.append("\\hline\n");


    for (int j = 0; j < tableStandardItemModel.rowCount(); ++j)
    {
        texTable_in_out.append(QString::number(id));
        texTable_in_out.append(" &");
        ++id;

        for (int k = 0; k < tableStandardItemModel.columnCount(); ++k)
        {
            if (columnsToIgnore.contains(k))
            {
                continue;
            }

            item = tableStandardItemModel.item(j, k);
            if (item)
            {
                QString itemText(item->text());
                handleFactureTexTableItemText(tableStandardItemModel.columnCount(),
                                              texTable_in_out,
                                              k,
                                              itemText);
            }
            else
            {
                if (k < tableStandardItemModel.columnCount() - 1)
                {
                    texTable_in_out.append("\"\"").append(" &");
                }
                else
                {
                    texTable_in_out.append("\"\"").append("\\hline\n");
                }
            }
        }

        texTable_in_out = texTable_in_out.trimmed();

        cleanUpTexTableLastString(texTable_in_out);

        texTable_in_out.append("\\hline\n");
    }

    //Removes the empty character "" from Latex output
    texTable_in_out.replace("\"\"", "");

    texTable_in_out.append("\\end{tabular}").append("\n")
    .append("\\end{table*}").append("\n");

    //qDebug() << "++ texTable_in_out in getStocksListingTexTableString: " << texTable_in_out;
}


void YerothUtils::getAchatsListingTexTableString(QString &texTable_in_out,
        										 QStandardItemModel &tableStandardItemModel,
												 QList<int> &columnsToIgnore,
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

    texTable_in_out.append("\\begin{table*}[!htbp]").append("\n")
    			   .append("\\centering").append("\n")
				   .append("\\begin{tabular}")
				   .append("{|");

    int texTableColumnCount = tableStandardItemModel.columnCount() + 1;

    //Tex table header
    for (int k = 0; k < texTableColumnCount; ++k)
    {
        if (columnsToIgnore.contains(k))
        {
            continue;
        }

        texTable_in_out.append("r|");
    }

    texTable_in_out.append("} \\hline\n");

    /** We add a column named 'id' for numbering the rows
     * in the Tex table. */
    unsigned int id = fromRowIndex + 1;
    texTable_in_out.append("\\textbf{id} & ");

    QStandardItem *item;

    for (int k = 0; k < texTableColumnCount; ++k)
    {
        if (columnsToIgnore.contains(k))
        {
            continue;
        }

        item = tableStandardItemModel.horizontalHeaderItem(k);

        if (item)
        {
            QString itemText(item->text().prepend("\\textbf{").append("}"));
            handleTexTableItemText(tableStandardItemModel.columnCount(),
                                   texTable_in_out,
                                   k,
                                   itemText);
        }
    }
    /** Closing Tex table header */
    cleanUpTexTableLastString(texTable_in_out);

    texTable_in_out.append(" \\\\ \\hline\n");

    //qDebug() << QString("++ fromRowIndex: %1, toRowIndex: %2")
    //			.arg(QString::number(fromRowIndex), QString::number(toRowIndex));

    for (int j = fromRowIndex; j < toRowIndex; ++j)
    {
        texTable_in_out.append(QString::number(id));
        texTable_in_out.append(" &");
        ++id;

        for (int k = 0; k < tableStandardItemModel.columnCount(); ++k)
        {
            if (columnsToIgnore.contains(k))
            {
                continue;
            }

            item = tableStandardItemModel.item(j, k);
            if (item)
            {
                QString itemText(item->text());
                handleAchatsTexTableItemText(tableStandardItemModel.columnCount(),
                                              texTable_in_out,
                                              k,
                                              itemText);
            }
            else
            {
                if (k < tableStandardItemModel.columnCount() - 1)
                {
                    texTable_in_out.append("\"\"").append(" &");
                }
                else
                {
                    texTable_in_out.append("\"\"").append("\\hline\n");
                }
            }
        }

        texTable_in_out = texTable_in_out.trimmed();

        cleanUpTexTableLastString(texTable_in_out);

        texTable_in_out.append("\\\\ \\hline\n");
    }

    //Removes the empty character "" from Latex output
    texTable_in_out.replace("\"\"", "");

    texTable_in_out.append("\\end{tabular}").append("\n")
    			   .append("\\end{table*}").append("\n");

    //qDebug() << "++ texTable_in_out in getStocksListingTexTableString: " << texTable_in_out;
}


void YerothUtils::getStocksListingTexTableString(QString &texTable_in_out,
        QStandardItemModel &tableStandardItemModel,
        QList<int> &columnsToIgnore,
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

    texTable_in_out.append("\\begin{table*}[!htbp]").append("\n")
    .append("\\centering").append("\n")
    .append("\\begin{tabular}")
    .append("{|");

    int texTableColumnCount = tableStandardItemModel.columnCount() + 1;

    //Tex table header
    for (int k = 0; k < texTableColumnCount; ++k)
    {
        if (columnsToIgnore.contains(k))
        {
            continue;
        }

        texTable_in_out.append("r|");
    }

    texTable_in_out.append("} \\hline").append("\n");

    /** We add a column named 'id' for numbering the rows
     * in the Tex table. */
    unsigned int id = fromRowIndex + 1;
    texTable_in_out.append("\\textbf{id} & ");

    QStandardItem *item;

    for (int k = 0; k < texTableColumnCount; ++k)
    {
        if (columnsToIgnore.contains(k))
        {
            continue;
        }

        item = tableStandardItemModel.horizontalHeaderItem(k);
        if (item)
        {
            QString itemText(item->text().prepend("\\textbf{").append("}"));
            handleTexTableItemText(tableStandardItemModel.columnCount(),
                                   texTable_in_out,
                                   k,
                                   itemText);
        }
    }
    /** Closing Tex table header */
    cleanUpTexTableLastString(texTable_in_out);

    texTable_in_out.append("\\\\ \\hline\n");

    //qDebug() << QString("++ fromRowIndex: %1, toRowIndex: %2")
    //			.arg(QString::number(fromRowIndex), QString::number(toRowIndex));

    for (int j = fromRowIndex; j < toRowIndex; ++j)
    {
        texTable_in_out.append(QString::number(id));
        texTable_in_out.append(" &");
        ++id;

        for (int k = 0; k < tableStandardItemModel.columnCount(); ++k)
        {
            if (columnsToIgnore.contains(k))
            {
                continue;
            }

            item = tableStandardItemModel.item(j, k);
            if (item)
            {
                QString itemText(item->text());
                handleFactureTexTableItemText(tableStandardItemModel.columnCount(),
                                              texTable_in_out,
                                              k,
                                              itemText);
            }
            else
            {
                if (k < tableStandardItemModel.columnCount() - 1)
                {
                    texTable_in_out.append("\"\"").append(" &");
                }
                else
                {
                    texTable_in_out.append("\"\"").append("\\\\ \\hline\n");
                }
            }
        }

        texTable_in_out = texTable_in_out.trimmed();

        cleanUpTexTableLastString(texTable_in_out);

        texTable_in_out.append("\\\\ \\hline\n");
    }

    //Removes the empty character "" from Latex output
    texTable_in_out.replace("\"\"", "");

    texTable_in_out.append("\\end{tabular}").append("\n")
    .append("\\end{table*}").append("\n");

    //qDebug() << "++ texTable_in_out in getStocksListingTexTableString: " << texTable_in_out;
}

void YerothUtils::refreshSalesStrategy(YerothSqlTableModel &curStocksTableModel,
                                      YerothLineEdit 	 *lineEdit_recherche_article,
                                      YerothLineEdit 	 *lineEdit_recherche_article_codebar)
{
    QString curStocksTableModelName = curStocksTableModel.sqlTableName();

    if (YerothUtils::isEqualCaseInsensitive(YerothERPConfig::STRATEGIE_VENTE_SORTIE_FIFO,
                                           YerothERPConfig::salesStrategy))
    {
        lineEdit_recherche_article->setupMyQCompleterFIFO(curStocksTableModelName);

        lineEdit_recherche_article_codebar->setupMyQCompleterCodebarFIFO(curStocksTableModelName);
    }
    else if (YerothUtils::isEqualCaseInsensitive(YerothERPConfig::STRATEGIE_VENTE_SORTIE_LIFO,
             YerothERPConfig::salesStrategy))
    {
        lineEdit_recherche_article->setupMyQCompleterLIFO(curStocksTableModelName);

        lineEdit_recherche_article_codebar->
        setupMyQCompleterCodebarLIFO(curStocksTableModelName);
    }
    else if (YerothUtils::isEqualCaseInsensitive(YerothERPConfig::STRATEGIE_VENTE_SORTIE_DEF_DEO,
             YerothERPConfig::salesStrategy))
    {
        lineEdit_recherche_article->setupMyQCompleterDEF_DEO(curStocksTableModelName);

        lineEdit_recherche_article_codebar->
        setupMyQCompleterCodebarDEF_DEO(curStocksTableModelName);
    }
    else //YerothConfig::STRATEGIE_VENTE_SORTIE_ALL
    {
        lineEdit_recherche_article->setupMyQCompleterALL(curStocksTableModelName);

        lineEdit_recherche_article_codebar->setupMyQCompleterCodebarALL(curStocksTableModelName);
    }
}


void YerothUtils::getLatexStockHistoryFR(QString &texDocumentString_in_out,
									   	 QString &printString)
{
    texDocumentString_in_out.clear();
    texDocumentString_in_out.append(FR_template_historique_dun_stock_tex);
    texDocumentString_in_out.append(printString).append("\n");
    texDocumentString_in_out.append("\\end{document}");
}


void YerothUtils::getLatexStockHistoryEN(QString &texDocumentString_in_out,
        								 QString &printString)
{
    texDocumentString_in_out.clear();
    texDocumentString_in_out.append(EN_template_historique_dun_stock_tex);
    texDocumentString_in_out.append(printString).append("\n");
    texDocumentString_in_out.append("\\end{document}");
}


void YerothUtils::getTexLandscapePaymentsFRDocumentString(QString &texDocumentString_in_out,
														  QString &printString)
{
    texDocumentString_in_out.clear();
    texDocumentString_in_out.append(FR_template_journal_des_paiements_tex);
    texDocumentString_in_out.append(printString).append("\n");
    texDocumentString_in_out.append("\\end{document}");
}

void YerothUtils::getTexLandscapePaymentsENDocumentString(QString &texDocumentString_in_out,
        												  QString &printString)
{
    texDocumentString_in_out.clear();
    texDocumentString_in_out.append(EN_template_journal_des_paiements_tex);
    texDocumentString_in_out.append(printString).append("\n");
    texDocumentString_in_out.append("\\end{document}");
}


void YerothUtils::getTexLandscapeSellingDocumentString(QString &texDocumentString_in_out,
        QString &printString)
{
    texDocumentString_in_out.clear();
    texDocumentString_in_out.append(FR_template_journal_des_ventes_tex);
    texDocumentString_in_out.append(printString).append("\n");
    texDocumentString_in_out.append("\\end{document}");
}

void YerothUtils::getTexLandscapeENDocumentString(QString &texDocumentString_in_out,
        QString &printString)
{
    texDocumentString_in_out.clear();
    texDocumentString_in_out.append(EN_template_journal_des_ventes_tex);
    texDocumentString_in_out.append(printString).append("\n");
    texDocumentString_in_out.append("\\end{document}");
}

void YerothUtils::getTexLandscapeFROutgoingDocumentString(QString &texDocumentString_in_out,
        QString &printString)
{
    texDocumentString_in_out.clear();
    texDocumentString_in_out.append(FR_template_journal_des_transactions_tex);
    texDocumentString_in_out.append(printString).append("\n");
    texDocumentString_in_out.append("\\end{document}");
}

void YerothUtils::getTexLandscapeENOutgoingDocumentString(QString &texDocumentString_in_out,
        QString &printString)
{
    texDocumentString_in_out.clear();
    texDocumentString_in_out.append(EN_template_journal_des_transactions_tex);
    texDocumentString_in_out.append(printString).append("\n");
    texDocumentString_in_out.append("\\end{document}");
}

void YerothUtils::getFactureFRTexDocumentString(QString &texDocumentString_in_out,
        QString &printString)
{
    printString.append("\n \\vspace*{0.2cm}\n")
    .append("Arr\\^et\\'e la pr\\'esente facture au montant total TTC de \\textbf{YEROTHSOMME}.\n");

    texDocumentString_in_out.clear();
    texDocumentString_in_out.append(FR_template_facture_grand_tex);
    texDocumentString_in_out.append(printString).append("\n");
    texDocumentString_in_out.append("\\end{document}");
}

void YerothUtils::getFactureENTexDocumentString(QString &texDocumentString_in_out,
        QString &printString)
{
    printString.append("\n \\vspace*{0.2cm}\n")
    .append("The total amount of this receipt all taxes comprised is of \\textbf{YEROTHSOMME}.\n");

    texDocumentString_in_out.clear();
    texDocumentString_in_out.append(EN_template_facture_grand_tex);
    texDocumentString_in_out.append(printString).append("\n");
    texDocumentString_in_out.append("\\end{document}");
}

void YerothUtils::getFactureSmallFRTexDocumentString(QString &texDocumentString_in_out,
        QString &printString)
{
    texDocumentString_in_out.clear();
    texDocumentString_in_out.append(FR_template_facture_petit_tex);
    texDocumentString_in_out.append(printString).append("\n");
    texDocumentString_in_out.append("\\end{document}");
}

void YerothUtils::getFactureSmallENTexDocumentString(QString &texDocumentString_in_out,
        QString &printString)
{
    texDocumentString_in_out.clear();
    texDocumentString_in_out.append(EN_template_facture_petit_tex);
    texDocumentString_in_out.append(printString).append("\n");
    texDocumentString_in_out.append("\\end{document}");
}


bool YerothUtils::export_csv_file(YerothWindowsCommons &aCallingWindow,
								  YerothTableView &aTableView,
								  QList<int> databaseTableColumnsToIgnore,
								  QString csvFileName,
								  QString strMessage)
{
    QStandardItemModel *tableModel = aTableView.getStandardItemModel();

    if (0 == tableModel)
    {
        return false;
    }

    int tableModelRowCount = tableModel->rowCount();
    int tableModelColumnCount = tableModel->columnCount();

    if (tableModelRowCount    <= 0  ||
        tableModelColumnCount <= 0)
    {
        YerothQMessageBox::information(&aCallingWindow,
                                      QObject::trUtf8("pas de données à exporter au format csv"),
                                      QObject::trUtf8("Il n'y a pas de données à exporter au format csv !"));
        return false;
    }

    QString csvFileContent;

    QStandardItem * anItem = 0;
    QString anItemText;

    for (int k = 0; k < tableModelColumnCount; ++k)
    {
        if (databaseTableColumnsToIgnore.contains(k))
        {
            continue;
        }

        anItem = tableModel->horizontalHeaderItem(k);

        if (0 != anItem)
        {
            anItemText = anItem->text();

            csvFileContent.append( QString("\"%1\", ").arg(anItemText) );
        }
    }

    csvFileContent.remove(csvFileContent.size() - 2, 2)
    .append("\n");

    for (unsigned int j = 0; j < tableModelRowCount; ++j)
    {
        for (unsigned int k = 0; k < tableModelColumnCount; ++k)
        {
            if (databaseTableColumnsToIgnore.contains(k))
            {
                continue;
            }

            anItem = tableModel->item(j, k);

            if (0 != anItem)
            {
                anItemText = anItem->text();

                csvFileContent.append( QString("\"%1\", ").arg(anItemText) );
            }
        }

        csvFileContent.remove(csvFileContent.size() - 2, 2)
        .append("\n");
    }

    QString yerothStocksListingCSVFileName(QString("%1/%2")
    										.arg(YerothERPConfig::temporaryFilesDir,
    											 csvFileName));

    yerothStocksListingCSVFileName = FILE_NAME_USERID_CURRENT_TIME(yerothStocksListingCSVFileName);

#ifdef YEROTH_FRANCAIS_LANGUAGE
    yerothStocksListingCSVFileName = QFileDialog::getSaveFileName(&aCallingWindow,
    															  "Saisir le nom du fichier '.csv'",
																  yerothStocksListingCSVFileName,
																  QString("%1 \"*.csv\" (*.csv)")
																	.arg(strMessage));
#endif

#ifdef YEROTH_ENGLISH_LANGUAGE
    yerothStocksListingCSVFileName = QFileDialog::getSaveFileName(&aCallingWindow,
    															  "Type in '.csv' file name ",
																  yerothStocksListingCSVFileName,
																  QString("%1 \"*.csv\" (*.csv)")
																	.arg(strMessage));
#endif



    yerothStocksListingCSVFileName.append(".csv");

    QFile tmpFile(yerothStocksListingCSVFileName);

    if (tmpFile.open(QFile::WriteOnly))
    {
        tmpFile.write(csvFileContent.toUtf8());
    }

    tmpFile.close();

    return true;
}


QString YerothUtils::prindDocumentFromTableView(YerothWindowsCommons *aWindowCaller,
									   	     YerothTableView &aTableView,
											 QList<int> &tableColumnsToIgnore,
											 void (*functionPointerToGetTableViewLatexString)(QString &texTable_in_out_OUT,
											   	   	   	   	   	   	   	   	   	   	    	  QStandardItemModel &tableStandardItemModel_OUT,
																							  QList<int> &columnsToIgnore_OUT,
																							  int fromRowIndex_OUT,
																							  int toRowIndex_OUT,
																							  bool lastPage_OUT),
									          void (*getLatexTemplateDocumentString)(QString &texDocumentString_in_out_OUT,
											   	   	   	   	   	   	   	   	         QString &printString_in_out_OUT),
									          QString latexFileNamePrefix)
{
    QStandardItemModel *tableModel = aTableView.getStandardItemModel();

    int tableModelRowCount = tableModel->rowCount();

    if (tableModelRowCount <= 0)
    {
        YerothQMessageBox::information(aWindowCaller,
                                       QObject::tr("impression"),
                                       QObject::trUtf8("Il n'y a pas de données à imprimer !"));
        return "";
    }

    int fromRowIndex = 0;

    int toRowIndex = tableModelRowCount;

    QString texTable;

    double MAX_TABLE_ROW_COUNT = 35.0;

    int pageNumber = qCeil(tableModelRowCount / MAX_TABLE_ROW_COUNT);

    YerothPOSUser *aCurrentUser = _allWindows->getUser();

    //qDebug() << QString("number of pages to print: %1").arg(pageNumber);
    //_logger->log("imprimer_document",
    //                  QString("number of pages to print: %1").arg(pageNumber));
    functionPointerToGetTableViewLatexString(texTable, *tableModel, tableColumnsToIgnore, 0,
            								(20 >= tableModelRowCount) ? tableModelRowCount : 20,
            								tableModelRowCount <= 20);

    if (tableModelRowCount >= 20)
    {
        fromRowIndex = 20;

        toRowIndex = (fromRowIndex >= tableModelRowCount) ? fromRowIndex : fromRowIndex + MAX_TABLE_ROW_COUNT;

        int k = 1;

        do
        {
            //qDebug() << QString("## fromRowIndex: %1, toRowIndex: %2")
            //          .arg(QString::number(fromRowIndex), QString::number(toRowIndex));
        	functionPointerToGetTableViewLatexString(texTable, *tableModel, tableColumnsToIgnore,
                    							     (fromRowIndex >= tableModelRowCount) ? tableModelRowCount : fromRowIndex,
													 (toRowIndex >= tableModelRowCount) ? (tableModelRowCount + 1) : toRowIndex,
													 k == pageNumber);
            texTable.append(QString("\\newpage \n"));
            fromRowIndex = toRowIndex;
            toRowIndex =
                (fromRowIndex >= tableModelRowCount) ? (fromRowIndex + 1) : fromRowIndex + MAX_TABLE_ROW_COUNT;
            ++k;
        }

        while (k <= pageNumber && fromRowIndex <= toRowIndex);
    }

    YerothInfoEntreprise & infoEntreprise = YerothUtils::getAllWindows()->getInfoEntreprise();

    QString texDocument;

    getLatexTemplateDocumentString(texDocument, texTable);

    QString factureDate(YerothUtils::handleForeignAccents(infoEntreprise.getVilleTex()));

    YerothUtils::getCurrentLocaleDate(factureDate);

    texDocument.replace("YEROTHPAPERSPEC", "a4paper");

    texDocument.replace("YEROTHENTREPRISE", infoEntreprise.getNomCommercialTex());
    texDocument.replace("YEROTHACTIVITESENTREPRISE", infoEntreprise.getSecteursActivitesTex());
    texDocument.replace("YEROTHBOITEPOSTALE", infoEntreprise.getBoitePostal());
    texDocument.replace("YEROTHVILLE", infoEntreprise.getVilleTex());
    texDocument.replace("YEROTHPAYS", infoEntreprise.getPaysTex());
    texDocument.replace("YEROTHEMAIL", infoEntreprise.getEmailTex());
    texDocument.replace("YEROTHTELEPHONE", infoEntreprise.getTelephone());
    texDocument.replace("YEROTHDATE", factureDate);
    texDocument.replace("YEROTHNOMUTILISATEUR", _allWindows->getUser()->nom_completTex());
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

    return YerothERPProcess::compileLatex(yerothPrefixFileName);
}


void YerothUtils::getLatexCustomerData(QString &texDocumentString_in_out,
									   QString &printString)
{
    texDocumentString_in_out.append(YerothUtils::template_fiche_client_tex);
    texDocumentString_in_out.append(printString).append("\n");
    texDocumentString_in_out.append("\\end{document}");
}


void YerothUtils::getSortieDesStocksFRTexDocumentString(QString &texDocumentString_in_out,
        QString &printString)
{
    texDocumentString_in_out.clear();
    texDocumentString_in_out.append(FR_template_sortie_des_stocks_grand_tex);
    texDocumentString_in_out.append(printString).append("\n");
    texDocumentString_in_out.append("\\end{document}");
}

void YerothUtils::getSortieDesStocksENTexDocumentString(QString &texDocumentString_in_out,
        QString &printString)
{
    texDocumentString_in_out.clear();
    texDocumentString_in_out.append(EN_template_sortie_des_stocks_grand_tex);
    texDocumentString_in_out.append(printString).append("\n");
    texDocumentString_in_out.append("\\end{document}");
}

void YerothUtils::getSortieDesStocksSmallFRTexDocumentString(QString &texDocumentString_in_out,
        QString &printString)
{
    texDocumentString_in_out.clear();
    texDocumentString_in_out.append(FR_template_sortie_des_stocks_petit_tex);
    texDocumentString_in_out.append(printString).append("\n");
    texDocumentString_in_out.append("\\end{document}");
}

void YerothUtils::getSortieDesStocksSmallENTexDocumentString(QString &texDocumentString_in_out,
        QString &printString)
{
    texDocumentString_in_out.clear();
    texDocumentString_in_out.append(EN_template_sortie_des_stocks_petit_tex);
    texDocumentString_in_out.append(printString).append("\n");
    texDocumentString_in_out.append("\\end{document}");
}


void YerothUtils::getComptesClientsFRTexDocumentString(QString &texDocumentString_in_out,
        											   QString &printString)
{
    texDocumentString_in_out.clear();
    texDocumentString_in_out.append(FR_template_comptes_clients_tex);
    texDocumentString_in_out.append(printString).append("\n");
    texDocumentString_in_out.append("\\end{document}");
}


void YerothUtils::getComptesClientsENTexDocumentString(QString &texDocumentString_in_out,
        											   QString &printString)
{
    texDocumentString_in_out.clear();
    texDocumentString_in_out.append(EN_template_comptes_clients_tex);
    texDocumentString_in_out.append(printString).append("\n");
    texDocumentString_in_out.append("\\end{document}");
}


void YerothUtils::getMarchandisesFRTexDocumentString(QString &texDocumentString_in_out,
        QString &printString)
{
    texDocumentString_in_out.clear();
    texDocumentString_in_out.append(FR_template_marchandises_tex);
    texDocumentString_in_out.append(printString).append("\n");
    texDocumentString_in_out.append("\\end{document}");
}


void YerothUtils::getMarchandisesENTexDocumentString(QString &texDocumentString_in_out,
        QString &printString)
{
    texDocumentString_in_out.clear();
    texDocumentString_in_out.append(EN_template_marchandises_tex);
    texDocumentString_in_out.append(printString).append("\n");
    texDocumentString_in_out.append("\\end{document}");
}


void YerothUtils::getAchatsListingFRTexDocumentString(QString &texDocumentString_in_out,
        QString &printString)
{
    texDocumentString_in_out.clear();
    texDocumentString_in_out.append(FR_template_lister_achats_tex);
    texDocumentString_in_out.append(printString).append("\n");
    texDocumentString_in_out.append("\\end{document}");
}

void YerothUtils::getAchatsListingENTexDocumentString(QString &texDocumentString_in_out,
        QString &printString)
{
    texDocumentString_in_out.clear();
    texDocumentString_in_out.append(EN_template_lister_achats_tex);
    texDocumentString_in_out.append(printString).append("\n");
    texDocumentString_in_out.append("\\end{document}");
}


void YerothUtils::getStocksListingFRTexDocumentString(QString &texDocumentString_in_out,
        QString &printString)
{
    texDocumentString_in_out.clear();
    texDocumentString_in_out.append(FR_template_lister_stock_tex);
    texDocumentString_in_out.append(printString).append("\n");
    texDocumentString_in_out.append("\\end{document}");
}

void YerothUtils::getStocksListingENTexDocumentString(QString &texDocumentString_in_out,
        QString &printString)
{
    texDocumentString_in_out.clear();
    texDocumentString_in_out.append(EN_template_lister_stock_tex);
    texDocumentString_in_out.append(printString).append("\n");
    texDocumentString_in_out.append("\\end{document}");
}
