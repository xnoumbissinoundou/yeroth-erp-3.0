/*
 * yeroth-erp-utils.hpp
 *
 *      Author: Xavier NOUMBISSI NOUNDOU, Dipl.-Inf., Ph.D. (ABD)
 */

#ifndef SRC_YEROTH_UTILS_HPP_
# define SRC_YEROTH_UTILS_HPP_

#include "src/include/yeroth-erp-3-0-software.text-configuration.hpp"

#include "src/widgets/table-view/yeroth-erp-table-view.hpp"

#include "src/utils/yeroth-erp-config.hpp"

#include <QtCore/QRegExp>
#include <QtCore/QCryptographicHash>
#include <QtCore/QLocale>
#include <QtCore/QDate>
#include <QtCore/QDir>

#include <QtGui/QColor>
#include <QtGui/QPalette>
#include <QtGui/QStandardItemModel>
#include <QtGui/QIntValidator>
#include <QtGui/QDoubleValidator>

#include <QtWidgets/QToolBar>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QStyleFactory>
#include <QtWidgets/QStyle>

#include <QtSql/QSqlTableModel>

class YerothLogger;

class YerothQMessageBox;
class QSqlTableModel;
class QStandardItemModel;
class QTableWidget;
class QString;
class QWidget;
class QMainWindow;
class QLabel;
class QComboBox;
class QRegExp;
class QLocale;
class QProcess;

class YerothPOSAdminWindowsCommons;
class YerothWindowsCommons;
class YerothERPWindows;
class YerothLineEdit;
class YerothSqlTableModel;

class QPluginLoader;
class QtPlugin;

class YerothUtils : public QObject
{
	Q_OBJECT

public:

	YEROTH_CLASS_OPERATORS

	inline YerothUtils(){}

	inline ~YerothUtils(){}

	static QString YEROTH_TRUNCATE_STRING_ACCORDING_TO_SETTING(QString aString_IN);

	static bool startTransaction();

	static bool commitTransaction();

	static void setAllWindows(YerothERPWindows *allWindows);

	inline static YerothERPWindows *getAllWindows()
	{
		return _allWindows;
	}

	inline static QString getYerothAlertDaemonExecutableFullpath()
	{
		return QString("%1/bin/%2")
		  	  	.arg(YerothERPConfig::YEROTH_ERP_ALERT_3_0_HOME_FOLDER,
		  	  		 YerothERPConfig::YEROTH_ALERT_DAEMON_ID);
	}

	static void writeStringToQFilewithUTF8Encoding(QFile &latexContentOutputFile,
												   QString aLatexFileContentString);

	static QString getUniquePrefixFileInTemporaryFilesDir(QString fileName);

	static const QString getCurrentAdminWindowReceiptsFormatAccordingToLanguage(QString currentFacturesTypeValue);

	static void saveCurrentAdminWindowFacturesTypeAccordingToLanguage(QString currentFacturesTypeValue);

	static bool executer_fichier_sql(const QString &fileName, YerothLogger *logger = 0);

	inline static QString prepareSqlStr(QString aStr)
	{
		return aStr.replace("'", "''");
	}

	static bool execQuery(QString strQuery, YerothLogger *logger = 0);

	static int execQuery(QSqlQuery &query, QString strQuery, YerothLogger *logger = 0);

	static int execQuery(QSqlQuery &query, YerothLogger *logger = 0);

	//static bool imprimer_document(QWidget *widget);

	inline static QString get_latex_bold_text(QString text)
	{
		return QString("\\textbf{%1}")
					.arg(text);
	}

	static QString get_text_sql_escaped(const QVariant &qv);

	static QString get_text(const QVariant &qv);

	static int getNextIdFromTable(const QString &tableName);

	static int getNextVentesIdFromTable(const QString &tableName);

	static int getNextSortiesIdFromTable(const QString &tableName);

	static void selectionner_image(QWidget *parent, QLabel &label_image);

	static void loadPixmapFromDB(QLabel 	&label_image,
								 QVariant 	imageVariant,
								 const char *format);

	static void savePixmapToByteArray(QByteArray 	&buffer,
									  const QPixmap &pixmap,
									  const char 	*format);

	static void getCenterPosition(unsigned 		desktopWidth,
			   	   	   	   	   	  unsigned 		desktopHeight,
								  QWidget 		&aWidget,
								  QPoint 		&result);

	static QString boolToString(bool b);

	static void addFiltre(YerothLineEdit *aLineEdit,
						  QString filterName,
						  QString &yerothFiltre_in_out);

	static void populateComboBox(QComboBox 		&aComboBox,
								 const QString 	&tableName,
								 const char 	*fieldName);

	inline static void populateComboBox(QComboBox 	&aComboBox,
								 	 	const char 	*tableName,
								 	 	const char 	*fieldName)
	{
		populateComboBox(aComboBox, QString(tableName), fieldName);
	}


	inline static void populateComboBox(QComboBox &aComboBox,
	                                    const QString &tableName,
										const QString	&fieldName)
	{
		populateComboBox(aComboBox,
		                 tableName,
		                 fieldName.toStdString().c_str());
	}


	static void populateComboBoxMissing(QComboBox 		&aComboBox,
										const QString 	&aContent,
								 	 	const QString 	&tableName,
								 	 	const char 		*fieldName);

	inline static void populateComboBoxMissing(QComboBox 	&aComboBox,
											   const char 	*aContent,
								 	 		   const char 	*tableName,
								 	 		   const char 	*fieldName)
	{
		populateComboBoxMissing(aComboBox, QString(aContent),
							    QString(tableName), fieldName);
	}

	inline static QByteArray md5Hash(QString data)
	{
		return QCryptographicHash::hash(data.toLatin1(), QCryptographicHash::Md5);
	}

	static void createTableModelHeaders(QSqlTableModel 		&tableModel,
			   	   	   	   	   	   	    QStandardItemModel 	&stdItemModel,
			   	   	   	   	   	   	    QStringList			&tableModelHeaders);

	inline static QString toUtf8String(QString &str)
	{
		return QString::fromUtf8(str.toStdString().c_str());
	}

	static QString generateSqlLike(QString sqlTableColumn, QString searchStr);

	inline static QString generateSqlLike(const char *sqlTableColumn, QString searchStr)
	{
		return YerothUtils::generateSqlLike(QString(sqlTableColumn), searchStr);
	}

	static QString generateSqlIs(QString sqlTableColumn, QString searchStr);

	inline static QString generateSqlIs(const char *sqlTableColumn, QString searchStr)
	{
		return YerothUtils::generateSqlIs(QString(sqlTableColumn), searchStr);
	}

	static QString generateSqlIsNotEmpty(QString sqlTableColumn);

	inline static QString generateSqlIsNotEmpty(const char *sqlTableColumn)
	{
		return YerothUtils::generateSqlIsNotEmpty(QString(sqlTableColumn));
	}

	static QString generateSqlIsEmpty(QString sqlTableColumn);

	inline static QString generateSqlIsEmpty(const char *sqlTableColumn)
	{
		return YerothUtils::generateSqlIsEmpty(QString(sqlTableColumn));
	}

	static void getColumnListString(QStringList 	&columnStringList,
									const QString 	&tableName,
									const char 		*fieldName);

	static QString colorLatexTextInput(const QString colorSpec,
			 	 	 	 	 	 	   const QString latexString_in);

	static QString LATEX_IN_OUT_handleForeignAccents(const QString &texText_in_out);

	static void cleanUpTexTableLastString(QString &texTable_in_out);

	static void handleComptesClientsTexTableItemText(int texTableColumnCount,
										     	 	 QString &texTable_in_out,
													 int itemTextColumnPosition,
													 QString &itemText);

	static void handleAchatsTexTableItemText(int texTableColumnCount,
										     QString &texTable_in_out,
											 int itemTextColumnPosition,
											 QString &itemText);

	static void handleFactureTexTableItemText(int texTableColumnCount,
										      QString &texTable_in_out,
											  int itemTextColumnPosition,
											  QString &itemText);

	static void handleTexTableItemText(int texTableColumnCount,
									   QString &texTable_in_out,
									   int itemTextColumnPosition,
									   QString &itemText);

	static void getFactureTexTableString(QString &texTable_in_out,
			  	  	  	  	  	  	  	 QTableWidget &tableStandardItemModel,
										 int 	quantiteVendue,
										 double totalTVA,
										 double sommeTotal);

	static void getFactureSmallTexTableString(QString &texTable_in_out,
			  	  	  	  	  	  	  	 	  QTableWidget &tableStandardItemModel,
											  int 	quantiteVendue,
											  double totalTVA,
											  double sommeTotal,
											  double montantRecuDuCient = 0.0,
											  double montantARembourserAuClient = 0.0);

	/**
	 * This method is in common used by classes YerothPointDeVenteWindow and
	 * YerothSortirWindow.
	 */
    static void refreshSalesStrategy(YerothSqlTableModel &curStocksTableModel,
    								 YerothLineEdit 		*lineEdit_recherche_article,
    								 YerothLineEdit 		*lineEdit_recherche_article_codebar);

    static bool export_csv_file(YerothWindowsCommons &aCallingWindow,
    							YerothTableView &aTableView,
    							QList<int> databaseTableColumnsToIgnore,
								QString csvFileName,
								QString strMessage);

    static QString prindDocumentFromTableView(YerothWindowsCommons *aWindowCaller,
    									   YerothTableView &aTableView,
    									   QList<int> &tableColumnsToIgnore,
										   void (*functionPointerToGetTableViewLatexString)(QString &texTable_in_out_OUT,
												   	   	   	   	   	   	   	   	   	   	    QStandardItemModel &tableStandardItemModel_OUT,
																							QList<int> &dbFieldNameOfTypeString_OUT,
																							QList<int> &columnsToIgnore_OUT,
																							int fromRowIndex_OUT,
																							int toRowIndex_OUT,
																							bool lastPage_OUT),
										   void (*getLatexTemplateDocumentString)(QString &texDocumentString_in_out_OUT,
												   	   	   	   	   	   	   	   	  QString &printString_in_out_OUT),
										   QString latexFileNamePrefix,
										   QMap<QString, QString> *documentSpecificElements = 0);

    inline static void getLatexFinancialAccountingReportData(QString &texDocumentString_in_out)
    {
        texDocumentString_in_out.append(YerothUtils::template_bilan_comptable_tex);
    }

    static void getLatexCustomerData(QString &texDocumentString_in_out);

	static void getSortieDesStocksFRTexDocumentString(QString &texDocumentString_in_out,
								  	 	 	 	 	   QString &printString);

	static void getSortieDesStocksENTexDocumentString(QString &texDocumentString_in_out,
								  	 	 	 	 	   QString &printString);

	static void getSortieDesStocksSmallFRTexDocumentString(QString &texDocumentString_in_out,
												   	       QString &printString);

	static void getSortieDesStocksSmallENTexDocumentString(QString &texDocumentString_in_out,
												   	       QString &printString);

	static void getLatexStockHistory(QString &texDocumentString_in_out,
									 QString &printString);

	static void getTexLandscapePaymentsDocumentString(QString &texDocumentString_in_out,
													  QString &printString);

	static void getTexLandscapeSellingDocumentString(QString &texDocumentString_in_out,
								  	 	 	  	  	 QString &printString);

	static void getTexLandscapeFROutgoingDocumentString(QString &texDocumentString_in_out,
													  QString &printString);

	static void getTexLandscapeENOutgoingDocumentString(QString &texDocumentString_in_out,
													  QString &printString);

	static void getFactureFRTexDocumentString(QString &texDocumentString_in_out,
											  QString &printString);

	static void getFactureENTexDocumentString(QString &texDocumentString_in_out,
											  QString &printString);

	static void getFactureSmallFRTexDocumentString(QString &texDocumentString_in_out,
												   QString &printString);

	static void getFactureSmallENTexDocumentString(QString &texDocumentString_in_out,
												   QString &printString);

	static void getComptesClientsTexDocumentString(QString &texDocumentString_in_out,
			   	   	   	   	   	   	   	   	   	   	 QString &printString);

	static QString getFileNameWithCurrentTime(const QString fileName);

	static QString getFileNameWithUserIDAndCurrentTime(const QString fileName);

	static QString getWindowTitleWithStrategy(QMainWindow *aMainWindow,
											  QString localVisibleStrategy = "");

	static QString appendPDFReceiptFormat(QString aStr);

	inline static QString getTvaString()
	{
		return QString::number((YerothERPConfig::tva_value * 100), 'f', 2);
	}

	inline static const QString getTvaStringWithPercent()
	{
	    return QString("%1 %").arg(YerothUtils::getTvaString());
	}

	inline static const QString getTVALabelStringWithPercent()
	{
	    return QString(QObject::tr("TVA (%1)")).arg(YerothUtils::getTvaStringWithPercent());
	}

	static void infosEntreprise(YerothPOSAdminWindowsCommons &aYerothPOSAdminQMainWindow,
								const QString infos);

	static void infosEntreprise(YerothWindowsCommons &aYerothPOSQMainWindow,
								const QString infos);

	static void getCurrentLocaleDate(QString &date);

	inline static bool isEqualCaseSensitive(const QString &str1, const QString &str2)
	{
		return (0 == str1.compare(str2, Qt::CaseSensitive));
	}

	inline static bool isEqualCaseInsensitive(const QString &str1, const QString &str2)
	{
		return (0 == str1.compare(str2, Qt::CaseInsensitive));
	}

	inline static void setMainWorkingDirectory(QString workingDirectory)
	{
		_mainWorkingDirectory = workingDirectory;
	}

	inline static QString getMainWorkingDirectory()
	{
		return _mainWorkingDirectory;
	}

	inline static void setLogFileName(QString logFileName)
	{
		_logFileName = logFileName;
	}

	inline static QString getLogFileName()
	{
		return _logFileName;
	}

	static bool checkIfCustomerAccountAlreadyExist_NOMENTREPRISE(YerothWindowsCommons &aCallingWindow,
																 YerothSqlTableModel &aClientTableModel,
																 YerothLineEdit &aYerothLineEdit_nom_entreprise,
																 int aCurrentClientDetailDBID = YerothUtils::CURRENT_CLIENT_DB_ID_UNDEFINED);

	static bool checkIfCustomerAccountAlreadyExist_REFERENCE_REGISTRE_DU_COMMERCE(YerothWindowsCommons &aCallingWindow,
																				  YerothSqlTableModel &aClientTableModel,
																				  YerothLineEdit &aYerothLineEdit_reference_registre_du_commerce,
																				  int aCurrentClientDetailDBID = YerothUtils::CURRENT_CLIENT_DB_ID_UNDEFINED);

	static bool checkIfCustomerAccountAlreadyExist_REFERENCECLIENT(YerothWindowsCommons &aCallingWindow,
																   YerothSqlTableModel &aClientTableModel,
																   YerothLineEdit &aYerothLineEdit_reference_client,
																   int aCurrentClientDetailDBID = YerothUtils::CURRENT_CLIENT_DB_ID_UNDEFINED);

	static void yerothSetWidgetColor(QWidget *aWidget);

	static const QString YEROTH_QT_STYLE_CDE;

	static const unsigned int CURRENT_CLIENT_DB_ID_UNDEFINED;

	static const QString IMPRIMANTE_PDF;

	static const QString IMPRIMANTE_EPSON_TM_T20ii;

    static const unsigned int PRINT_TIMEOUT;

    static const QString STRING_STYLESHEETS_YEROTH_ERP_3_0;

	static const QString STRING_STYLESHEET_COLOR_YEROTHGREEN;

	static const QColor YEROTH_WHITE_COLOR;

	static const QColor YEROTH_BLACK_COLOR;

	static const QColor YEROTH_BLUE_COLOR;

	static const QColor YEROTH_DARK_GREEN_COLOR;

	static const QColor YEROTH_GRAY_COLOR;

	static const QColor YEROTH_GREEN_COLOR;

	static const QColor YEROTH_YEROTHGREEN_COLOR;

	static const QColor YEROTH_INDIGO_COLOR;

	static const QColor YEROTH_ORANGE_COLOR;

	static const QColor YEROTH_RED_COLOR;

	static const QBrush YEROTH_QBRUSH_GREEN;

	static const QBrush YEROTH_QBRUSH_YEROTHGREEN;

	static const QBrush YEROTH_QBRUSH_BLUE;

	static const QBrush YEROTH_QBRUSH_ORANGE;

	static const QBrush YEROTH_QBRUSH_YEROTHGRAY;

	static const QPalette YEROTH_BLACK_PALETTE;

	static const QPalette YEROTH_BLUE_PALETTE;

	static const QPalette YEROTH_DARK_GREEN_PALETTE;

	static const QPalette YEROTH_GRAY_PALETTE;

	static const QPalette YEROTH_GREEN_PALETTE;

	static const QPalette YEROTH_RED_PALETTE;

	static const QPalette YEROTH_WHITE_PALETTE;

	static const QString TVA_0_STRING;

	static const QString DATE_FORMAT;

	static const QString TIME_FORMAT;

	static const QString DB_DATE_FORMAT;

	static const QString NOUVEAU_CLIENT;

	static const QString NOUVEAU_FOURNISSEUR;

	static const QString NOUVELLE_CATEGORIE;

	static const QString MAGELAN_CLIENT;

	static const QString MAGELAN_CLIENT_OBJECT;

	static const QString MAGELAN_SERVER;

	static const QString MAGELAN_SERVER_OBJECT;

	static const QString APROPOS_MSG;

	static const QString APPLICATION_NAME;

	static const QString LOCALHOST;

	static const QString LOCALHOST_IP_ADDRESS;

	static const QString STRING_OUI;

	static const QString STRING_NON;

	static const unsigned int FACTURE_GRAND_STRING_MAX_CHARS;

	static const unsigned int FACTURE_PETIT_NOM_ARTICLE_MAX_CHARS;

	static const QString EGAL;

	static const QString INFERIEUR_OU_EGAL;

	static const QString SUPERIEUR;

	static const QChar SLASH_CHAR;

	static const QIntValidator IntValidator;

	static const QDoubleValidator DoubleValidator;

	static const QRegExp PasswordRegExp;

	static const QRegExpValidator PasswordValidator;

	static const QRegExp EMPTY_SPACE_REGEXP;

	static const QLocale frenchLocale;

	static const QLocale englishLocale;

	static QString _1a_tex;

	static QString template_bilan_comptable_tex;

	static QString template_fiche_client_tex;

	static QString template_historique_dun_stock_tex;

	static QString template_journal_des_paiements_tex;

	static QString template_journal_des_ventes_tex;

	static QString FR_template_journal_des_transactions_tex;

	static QString EN_template_journal_des_transactions_tex;

	static QString FR_template_sortie_des_stocks_grand_tex;

	static QString EN_template_sortie_des_stocks_grand_tex;

	static QString FR_template_sortie_des_stocks_petit_tex;

	static QString EN_template_sortie_des_stocks_petit_tex;

	static QString template_transactions_dun_client_tex;

	static QString template_comptes_clients_tex;

	static QString template_marchandises_tex;

	static QString template_lister_achats_tex;

	static QString template_lister_stock_tex;

	static QString FR_template_facture_grand_tex;

	static QString EN_template_facture_grand_tex;

	static QString FR_template_facture_petit_tex;

	static QString EN_template_facture_petit_tex;

	static QString FR_pie_chart_tex;

	static QString FR_bar_chart_tex;

	static QString FR_bar_diag_tex;

	static QString EN_pie_chart_tex;

	static QString EN_bar_chart_tex;

	static QString EN_bar_diag_tex;

	static const QKeySequence MESSAGE_DAIDE_QKEYSEQUENCE;

	static const QKeySequence IMPRIMER_QKEYSEQUENCE;

	static const QKeySequence AFFICHER_LES_STOCKS_TERMINES_QKEYSEQUENCE;

	static const QKeySequence RECHERCHER_QKEYSEQUENCE;

	static const QKeySequence REINITIALISER_RECHERCHE_QKEYSEQUENCE;

	static const QKeySequence REINITIALISER_QUI_SUI_JE_QKEYSEQUENCE;

	static const QKeySequence LISTER_STOCKS_QKEYSEQUENCE;

public slots:

	static bool slot_connecter_localisation(QWidget &aWidget,
			 	 	 	 	 	 	 	 	YerothERPWindows *allWindows,
											const QString localisation);

	static bool slot_deconnecter_localisation(YerothERPWindows *allWindows);

private:

	static YerothERPWindows 	*_allWindows;
	static QString 			_mainWorkingDirectory;
	static QString 			_logFileName;
};

/**
 * Macros to facilitate the use of YerothUtils static methods
 */

#define YEROTH_QMESSAGE_BOX_QUELQUE_RESULTAT_FILTRE(THIS, NOMBRE, DIALOG_BOX_TITLE) \
{ QString msg(QObject::trUtf8("Il y'a eu")); \
msg.append(QString(QObject::trUtf8(" %1 résultat(s) à votre filtre !")).arg(NOMBRE)); \
YerothQMessageBox::information(this, QObject::trUtf8(DIALOG_BOX_TITLE), msg); }


#define YEROTH_QMESSAGE_BOX_AUCUN_RESULTAT_FILTRE(THIS, DIALOG_BOX_TITLE) \
		{ QString msg(QObject::trUtf8("Il ny'a eu aucun résultat à votre filtre !")); \
		YerothQMessageBox::information(THIS, QObject::trUtf8(DIALOG_BOX_TITLE), msg); }


# define EXPR_IS_POSITIV(X)	(0 <= X)

# define BOOL_TO_STRING(B) (B ? "True" : "False")

//Get the value of an QSqlRecord when given as value R
# define GET_SQL_RECORD_DATA(R,V) YerothUtils::get_text(R.value(V))

//Get the MD5 hash of value X
# define MD5_HASH(X) YerothUtils::md5Hash(X)

# define GENERATE_SQL_LIKE_STMT(COLUMN, SEARCH_STR) \
	YerothUtils::generateSqlLike(COLUMN, SEARCH_STR)

# define GENERATE_SQL_IS_STMT(COLUMN, SEARCH_STR) \
	YerothUtils::generateSqlIs(COLUMN, SEARCH_STR)

# define GENERATE_SQL_IS_NOT_EMPTY(COLUMN) \
	YerothUtils::generateSqlIsNotEmpty(COLUMN)

# define GENERATE_SQL_IS_EMPTY(COLUMN) \
	YerothUtils::generateSqlIsEmpty(COLUMN)

#define YEROTH_ERP_3_0_START_DATABASE_TRANSACTION YerothUtils::startTransaction()

#define YEROTH_ERP_3_0_COMMIT_DATABASE_TRANSACTION YerothUtils::commitTransaction()

#define GET_POURCENTAGE(X, TOTAL) ((X / TOTAL) * 100.0)

# define GET_CURRENT_DATE QDate::currentDate()

# define GET_CURRENT_STRING_DATE QDate::currentDate().toString(YerothUtils::DATE_FORMAT)

# define CURRENT_TIME QTime::currentTime().toString(YerothUtils::TIME_FORMAT)

# define FILE_NAME_USERID_CURRENT_TIME(F) YerothUtils::getFileNameWithUserIDAndCurrentTime(F)

# define DATE_TO_STRING(D) D.toString(YerothUtils::DATE_FORMAT)

# define GET_DATE_FROM_STRING(D) QDate::fromString(D, YerothUtils::DATE_FORMAT)

# define TIME_TO_STRING(D) D.toString(YerothUtils::TIME_FORMAT)

# define DATE_TO_DB_FORMAT_STRING(D) D.toString(YerothUtils::DB_DATE_FORMAT)

# define SET_DATE_TO_STRING(D) D.toString(YerothUtils::DATE_FORMAT)

# define POPULATE_COMBOBOX(C, T, F)  YerothUtils::populateComboBox(*C, T, F)

# define POPULATE_COMBOBOX_MISSING(C, ACONTENT, TABLENAME, FIELD)  YerothUtils::populateComboBoxMissing(*C, ACONTENT, TABLENAME, FIELD)

# ifdef YEROTH_FRANCAIS_LANGUAGE
	# define GET_NUM_STRING(NUM) YerothUtils::frenchLocale.toString(NUM)

	# define GET_DOUBLE_STRING(NUM) YerothUtils::frenchLocale.toString(NUM, 'f', 2)
	# define GET_DOUBLE_STRING_P(NUM, P) YerothUtils::frenchLocale.toString(NUM, 'f', P)

	# define GET_CURRENCY_STRING_NUM(NUM) YerothUtils::frenchLocale.toString(NUM, 'f', 2).append(" ").append(YerothERPConfig::currency)
	# define GET_CURRENCY_STRING_NUM_P(NUM, P) YerothUtils::frenchLocale.toString(NUM, 'f', P).append(" ").append(YerothERPConfig::currency)

	# define GET_MONTH_NAME_LOCALIZED(M) YerothUtils::frenchLocale.monthName(M)
# endif

# ifdef YEROTH_ENGLISH_LANGUAGE
	# define GET_NUM_STRING(NUM) YerothUtils::englishLocale.toString(NUM)

	# define GET_DOUBLE_STRING(NUM) YerothUtils::englishLocale.toString(NUM, 'f', 2)
	# define GET_DOUBLE_STRING_P(NUM, P) YerothUtils::englishLocale.toString(NUM, 'f', P)

	# define GET_CURRENCY_STRING_NUM(NUM) YerothUtils::englishLocale.toString(NUM, 'f', 2).append(" ").append(YerothERPConfig::currency)
	# define GET_CURRENCY_STRING_NUM_P(NUM, P) YerothUtils::englishLocale.toString(NUM, 'f', P).append(" ").append(YerothERPConfig::currency)

	# define GET_MONTH_NAME_LOCALIZED(M) YerothUtils::englishLocale.monthName(M)
# endif

# define GET_CURRENCY_STRING_NUM_FOR_LATEX(NUM) YerothUtils::LATEX_IN_OUT_handleForeignAccents(GET_CURRENCY_STRING_NUM(NUM))

# endif /* SRC_YEROTH_UTILS_HPP_ */
