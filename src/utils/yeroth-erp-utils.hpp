/*
 * yeroth-erp-utils.hpp
 *
 *      Author: Dipl.-Inf. XAVIER NOUMBISSI NOUNDOU
 */

#ifndef SRC_YEROTH_UTILS_HPP_
#define SRC_YEROTH_UTILS_HPP_

#include "src/include/yeroth-erp-3-0-software.text-configuration.hpp"

#include "src/widgets/table-view/yeroth-erp-table-view.hpp"

#include "src/utils/yeroth-erp-service-stock-marchandise-data.hpp"

#include "src/utils/yeroth-erp-config.hpp"

#include "src/widgets/yeroth-erp-combo-box.hpp"

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
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QStyleFactory>
#include <QtWidgets/QStyle>

#include <QtSql/QSqlTableModel>

class YerothLogger;

class YerothDatabase;
class YerothComboBox;
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


enum service_stock_already_exist_type
{
	SERVICE_REFERENCE_EXISTS 									= 0,
	SERVICE_STOCK_DESIGNATION_AND_DIFFERENT_CATEGORIE_EXIST 	= 1,
	SERVICE_STOCK_DESIGNATION_AND_SAME_CATEGORIE_EXIST 			= 2,
	SERVICE_STOCK_UNDEFINED										= 3
};


class YerothUtils : public QObject
{
	Q_OBJECT

public:

	YEROTH_CLASS_OPERATORS

	YerothUtils();

	inline ~YerothUtils(){}

	static bool creerNouvelleCategorie(const QString 		&proposedCategorieName,
									   YerothWindowsCommons *_callingWindow = 0);

	static
	bool isReferenceUnique(const QString &aStockServiceReference,
						   const QString &aStockServiceDesignation,
						   const QString &aStockServiceNomCategorie,
						   QString &curExistingReference_in_out);

    static
    enum service_stock_already_exist_type
		isStockItemInProductList(const QString &productCategorie,
								 const QString &productName);

    static
    enum service_stock_already_exist_type
    	isStockItemInProductList(bool isService,
    							 const QString &productReference,
    							 const QString &productCategorie,
    							 const QString &productName);

    static bool insertStockItemInProductList(const YerothERPServiceStockMarchandiseData &aServiceStockData,
    										 YerothWindowsCommons 		 	 *_callingWindow = 0);

	static QString YEROTH_TRUNCATE_STRING_ACCORDING_TO_SETTING(const QString &aString_IN);

    inline static double getMargeBeneficiaire(double prix_vente,
    								   	   	  double prix_dachat,
											  double montantTva)
    {
    	return (prix_vente - QString::number(montantTva, 'f', 2).toDouble() - prix_dachat);
    }

    inline static bool isProfitable(double prix_vente,
	   	   	  				 	    double prix_dachat,
								    double montantTva)
    {
    	return YerothUtils::getMargeBeneficiaire(prix_vente, prix_dachat, montantTva) >= 0;
    }

	static bool startTransaction();

	static bool commitTransaction();

	static void setAllWindows(YerothERPWindows *allWindows);

	inline static YerothERPWindows *getAllWindows()
	{
		return _allWindows;
	}

	static YerothDatabase *getDatabase();

	inline static QString getYerothAlertDaemonExecutableFullpath()
	{
		return QString("%1/bin/%2")
		  	  	.arg(YerothERPConfig::YEROTH_ERP_ALERT_3_0_HOME_FOLDER,
		  	  		 YerothERPConfig::YEROTH_ALERT_DAEMON_ID);
	}

	static void writeStringToQFilewithUTF8Encoding(QFile &latexContentOutputFile,
												   QString aLatexFileContentString);

	static QString getUniquePrefixFileInTemporaryFilesDir(QString fileName);

	static const QString getCurrentAdminWindowReceiptsFormatAccordingToLanguage(const QString &currentFacturesTypeValue);

	static void saveCurrentAdminWindowFacturesTypeAccordingToLanguage(const QString &currentFacturesTypeValue);

	static bool executer_fichier_sql(const QString &fileName, YerothLogger *logger = 0);

	inline static QString prepareSqlStr(const QString &aStr)
	{
		return QString(aStr).replace("'", "''");
	}

	static double YEROTH_CONVERT_QSTRING_TO_DOUBLE_LOCALIZED(const QString &aDoubleQString);

	static int execQueryRowCount(const QString &strQuery, YerothLogger *logger = 0);

	static bool execQuery(const QString &strQuery, YerothLogger *logger = 0);

	static int execQuery(QSqlQuery &query, const QString &strQuery, YerothLogger *logger = 0);

	static int execQuery(QSqlQuery &query, YerothLogger *logger = 0);

	//static bool imprimer_pdf_document(QWidget *widget);

	inline static QString get_latex_bold_text(const QString &text)
	{
		return QString("\\textbf{%1}")
					.arg(text);
	}

	static QString get_text_sql_escaped(const QVariant &qv);

	static QString get_text(const QVariant &qv);

	static void selectionner_image(QWidget *parent, QLabel &label_image);

	static void loadPixmapFromDB(QLabel 	&label_image,
								 QVariant 	imageVariant,
								 const char *format);

	static void savePixmapToByteArray(QByteArray 	&buffer,
									  const QPixmap &pixmap,
									  const char 	*format);

	static void savePixmapToFile(const QString &aFileFullPath,
	                             const QPixmap &pixmap,
	                             const char *format);

	static void getCenterPosition(unsigned 		desktopWidth,
			   	   	   	   	   	  unsigned 		desktopHeight,
								  const QWidget &aWidget,
								  QPoint 		&result);

	static void addFiltre(YerothLineEdit *aLineEdit,
						  QString filterName,
						  QString &yerothFiltre_in_out);

	static int getComboBoxDatabaseQueryValue(const QString &comboBoxStringValue,
								  	  	     QMap<int, QString> *toViewStringMAP);

	static int getComboBoxDatabaseQueryValue(const QString &comboBoxStringValue,
								  	  	  	 QMap<int, QString> &toViewStringMAP);

	inline static QByteArray md5Hash(QString data)
	{
		return QCryptographicHash::hash(data.toLatin1(), QCryptographicHash::Md5);
	}

	static void createTableModelHeaders(QSqlTableModel 		&tableModel,
			   	   	   	   	   	   	    QStandardItemModel 	&stdItemModel,
			   	   	   	   	   	   	    QStringList			&tableModelHeaders,
										QStringList			&tableModelRawHeaders_IN_OUT);

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

	inline static QString GET_REFERENCE_RECU_SORTIE(QString fixedNumber)
	{
		return  YerothUtils::GET_REFERENCE_RECU_SUFFIX(YerothUtils::PREFIX_RECU_SORTIE, fixedNumber);
	}

	inline static QString GET_REFERENCE_RECU_VENDU(QString fixedNumber)
	{
		return  YerothUtils::GET_REFERENCE_RECU_SUFFIX(YerothUtils::PREFIX_RECU_VENDU, fixedNumber);
	}

	inline static QString GET_REFERENCE_RECU_PAIEMENT_CLIENT(QString fixedNumber)
	{
		return  YerothUtils::GET_REFERENCE_RECU_SUFFIX(YerothUtils::PREFIX_RECU_PAIEMENT_CLIENT, fixedNumber);
	}

	static QString GET_REFERENCE_RECU_SUFFIX(const QString &prefix,
											 const QString &fixedNumber);

	static int STOCK_PURCHASE_RECORDS_QUANTITY(int stockId);

	inline static int STOCK_PURCHASE_RECORDS_QUANTITY(const QString &stockId)
	{
		return YerothUtils::STOCK_PURCHASE_RECORDS_QUANTITY(stockId.toInt());
	}

	static void getColumnListString(QStringList   &columnStringList,
									const QString &tableName,
									const QString &fieldName,
									QString conditionStr = YerothUtils::EMPTY_STRING);

	static QString colorLatexTextInput(const QString &colorSpec,
			 	 	 	 	 	 	   const QString &latexString_in);

	static QString LATEX_IN_OUT_handleForeignAccents(const QString &texText_in_out);

	static void cleanUpTexTableLastString(QString &texTable_in_out);

	static void handleFactureTexTableItemText(int texTableColumnCount,
										      QString &texTable_IN_OUT,
											  int itemTextColumnPosition,
											  const QString &itemText);

	static void handleTexTableItemText(int texTableColumnCount,
									   QString &texTable_IN_OUT,
									   int itemTextColumnPosition,
									   const QString &itemText);

	static void getFactureTexTableString(QString &texTable_IN_OUT,
			  	  	  	  	  	  	  	 QTableWidget &tableStandardItemModel,
										 int 	quantiteVendue,
										 double totalTVA,
										 double sommeTotal);

	static void getFactureSmallTexTableString(QString &texTable_IN_OUT,
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
								const QString &csvFileName,
								const QString &strMessage);

    inline static void getLatexFinancialAccountingReportData(QString &texDocumentString_in_out)
    {
        texDocumentString_in_out.append(YerothUtils::template_bilan_comptable_tex);
    }

    static bool import_csv_file_content(QString aCsvFileFullPath,
    									QStringList &wordList_IN_OUT);

    static void getLatexCustomerData(QString &texDocumentString_in_out);

	static void getSortieDesStocksFRTexDocumentString(QString &texDocumentString_in_out,
								  	 	 	 	 	   QString &printString);

	static void getSortieDesStocksENTexDocumentString(QString &texDocumentString_in_out,
								  	 	 	 	 	   QString &printString);

	static void getSortieDesStocksSmallFRTexDocumentString(QString &texDocumentString_in_out,
												   	       QString &printString);

	static void getSortieDesStocksSmallENTexDocumentString(QString &texDocumentString_in_out,
												   	       QString &printString);

	static void getFactureFRTexDocumentString(QString &texDocumentString_in_out,
											  QString &printString);

	static void getFactureENTexDocumentString(QString &texDocumentString_in_out,
											  QString &printString);

	static void getFactureSmallFRTexDocumentString(QString &texDocumentString_in_out,
												   QString &printString);

	static void getFactureSmallENTexDocumentString(QString &texDocumentString_in_out,
												   QString &printString);

	static QString getFileNameWithCurrentTime(const QString &fileName);

	static QString getFileNameWithUserIDAndCurrentTime(const QString &fileName);

	static QString getWindowTitleWithStrategy(QMainWindow *aMainWindow,
											  QString localVisibleStrategy = "");

	static void fillDBTableColumnNameToDBTableColumnType_TEST(const QString &aDBTableName,
															  QMap<QString, QString> 	&aDBbTableColumnToType_IN_OUT,
															  QMap<QString, bool> 	&aDBTableColumnToIsNotNULL_IN_OUT);

	static QString appendPDFReceiptFormat(QString aStr);

	static QString getStrategySqlQueryStr(QString aSelectStmt,
										  QString aStrategy);

	inline static void YEROTH_PRINT_TO_STDOUT(const QString &aQStringOutput)
	{
		QTextStream(stdout) << aQStringOutput << endl;
	}

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
								const QString &infos);

	static void infosEntreprise(YerothWindowsCommons &aYerothPOSQMainWindow,
								const QString &infos);

	static void getCurrentSimplifiedDate(QString &date_IN_OUT);

	static void getCurrentLocaleDate(QString &date_IN_OUT);

	inline static bool isEqualCaseSensitive(const QString &str1, const QString &str2)
	{
		return (0 == str1.compare(str2, Qt::CaseSensitive));
	}

	inline static bool isEqualCaseInsensitive(const QString &str1, const QString &str2)
	{
		return (0 == str1.compare(str2, Qt::CaseInsensitive));
	}

	inline static void setMainWorkingDirectory(const QString &workingDirectory)
	{
		_mainWorkingDirectory = workingDirectory;
	}

	inline static QString getMainWorkingDirectory()
	{
		return _mainWorkingDirectory;
	}

	inline static void setLogFileName(const QString &logFileName)
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



	static QString YEROTH_ERP_3_0_SERVER_PARAMETERS_DISPLAY;


	static const QString UTILISATEUR_NON_EXISTANT;


	static const QString DATABASE_MYSQL_CHAR_TYPE_STRING;

	static const QString DATABASE_MYSQL_DATE_TYPE_STRING;

	static const QString DATABASE_MYSQL_DOUBLE_TYPE_STRING;

	static const QString DATABASE_MYSQL_INT_TYPE_STRING;

	static const QString DATABASE_MYSQL_TIME_TYPE_STRING;


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

	static const QString YEROTH_ERP_3_0_LAST_BUILD_ID;

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

	static const QString MYSQL_TRUE_LITERAL;

	static const QString MYSQL_FALSE_LITERAL;

	static const QString CSV_FILE_SEPARATION_SEMI_COLON_STRING_CHAR;

	static const QString EMPTY_STRING;

	static const QChar SLASH_CHAR;

	static const QIntValidator IntValidator;

	static const QDoubleValidator DoubleValidator;

	static const QRegExp PasswordRegExp;

	static const QRegExpValidator PasswordValidator;

	static const QRegExp LINE_ENDING_STRING_REGEXP;

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


    enum STRATEGIE_COMBOBOX_INDEX
	{
    	STRATEGIE_ALL_COMBOBOX_INDEX 		= 0,
		STRATEGIE_FEFO_COMBOBOX_INDEX 		= 1,
		STRATEGIE_FIFO_COMBOBOX_INDEX 		= 2,
		STRATEGIE_LIFO_COMBOBOX_INDEX 		= 3
	};

	enum TITRE
	{
		TITRE_DR = 100,
		TITRE_ME = 120,
		TITRE_MLLE = 140,
		TITRE_MME = 160,
		TITRE_MR = 180,
		TITRE_PR = 200,
		TITRE_PROF = 220,
		TITRE_INDEFINI = 240
	};


	enum USER_ROLE
	{
		ROLE_ADMINISTRATEUR = 25,
		ROLE_MANAGER = 35,
		ROLE_GESTIONNAIREDESTOCK = 45,
		ROLE_MAGASINIER = 55,
		ROLE_CAISSIER = 65,
		ROLE_VENDEUR = 75,
		ROLE_INDEFINI = 85
	};


	enum TYPEDEVENTE
	{
		VENTE_COMPTANT 			= 350,
		VENTE_COMPTE_CLIENT		= 360,
		VENTE_INDEFINI 			= 370
	};


	enum TYPEDEPAIEMENT
	{
		VERSEMENT_COMPTANT 			= 10,
		VERSEMENT_CHEQUE 			= 20,
		VERSEMENT_TELEPHONE 		= 30,
		VERSEMENT_BANCAIRE 			= 40,
		VERSEMENT_VIREMENT_BANCAIRE = 50,
		VERSEMENT_ACHAT_ANNULE 		= 60,
		VERSEMENT_INDEFINI 			= 70
	};


	static QMap<int, QString> _strategieindexToUserViewString;


	static QMap<int, QString> _titreToUserViewString;

	static QMap<int, QString> _roleToUserViewString;

	static QMap<int, QString> _typedeventeToUserViewString;

	static QMap<int, QString> _typedepaiementToUserViewString;


	static const QString PREFIX_RECU_PAIEMENT_CLIENT;

	static const QString PREFIX_RECU_VENDU;

	static const QString PREFIX_RECU_SORTIE;

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
											const QString &localisation);

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



#define YEROTH_DATABASE_TABLE_COLUMN_INDEX(W, C) W.getDBTableFieldColumnIndex(C)


#define YEROTH_QSTRING_CONTAINS(S, V) S.contains(V, Qt::CaseInsensitive)

#define EXPR_IS_POSITIV(X)	(0 <= X)

#define INT_TO_STRING(B) ((0 != B) ? "OUI" : "NON")

#define BOOL_TO_STRING(B) (B ? "True" : "False")

//Get the value of an QSqlRecord when given as value R
#define GET_SQL_RECORD_DATA(R,V) YerothUtils::get_text(R.value(V))

//Get the MD5 hash of value X
#define MD5_HASH(X) YerothUtils::md5Hash(X)

#define GENERATE_SQL_LIKE_STMT(COLUMN, SEARCH_STR) \
	YerothUtils::generateSqlLike(COLUMN, SEARCH_STR)

#define GENERATE_SQL_IS_STMT(COLUMN, SEARCH_STR) \
	YerothUtils::generateSqlIs(COLUMN, SEARCH_STR)

#define GENERATE_SQL_IS_NOT_EMPTY(COLUMN) \
	YerothUtils::generateSqlIsNotEmpty(COLUMN)

#define GENERATE_SQL_IS_EMPTY(COLUMN) \
	YerothUtils::generateSqlIsEmpty(COLUMN)


#define YEROTH_ERP_3_0_START_DATABASE_TRANSACTION YerothUtils::startTransaction()

#define YEROTH_ERP_3_0_COMMIT_DATABASE_TRANSACTION YerothUtils::commitTransaction()

#define POURCENTAGE_YEROTH_GET_VALUE(X, TOTAL) ( (0 == TOTAL) ? 0 : ((X / TOTAL) * 100.0) )

#define GET_CURRENT_DATE QDate::currentDate()

#define GET_CURRENT_STRING_DATE QDate::currentDate().toString(YerothUtils::DATE_FORMAT)

#define CURRENT_TIME QTime::currentTime().toString(YerothUtils::TIME_FORMAT)

#define FILE_NAME_USERID_CURRENT_TIME(F) YerothUtils::getFileNameWithUserIDAndCurrentTime(F)

#define DATE_TO_STRING(D) D.toString(YerothUtils::DATE_FORMAT)

#define GET_DATE_FROM_STRING(D) QDate::fromString(D, YerothUtils::DATE_FORMAT)

#define TIME_TO_STRING(D) D.toString(YerothUtils::TIME_FORMAT)

#define DATE_TO_DB_FORMAT_STRING(D) D.toString(YerothUtils::DB_DATE_FORMAT)

#define SET_DATE_TO_STRING(D) D.toString(YerothUtils::DATE_FORMAT)


#ifdef YEROTH_FRANCAIS_LANGUAGE

	#define DOUBLE_FROM_LOCALIZED_STRING(NUM) YerothUtils::frenchLocale.toDouble(NUM)

	#define GET_NUM_STRING(NUM) YerothUtils::frenchLocale.toString(NUM)

	#define GET_DOUBLE_STRING(NUM) YerothUtils::frenchLocale.toString(NUM, 'f', 2)
	#define GET_DOUBLE_STRING_P(NUM, P) YerothUtils::frenchLocale.toString(NUM, 'f', P)

	#define GET_CURRENCY_STRING_NUM(NUM) YerothUtils::frenchLocale.toString(NUM, 'f', 2).append(" ").append(YerothERPConfig::currency)
	#define GET_CURRENCY_STRING_NUM_P(NUM, P) YerothUtils::frenchLocale.toString(NUM, 'f', P).append(" ").append(YerothERPConfig::currency)

	#define GET_MONTH_NAME_LOCALIZED(M) YerothUtils::frenchLocale.monthName(M)

#endif

#ifdef YEROTH_ENGLISH_LANGUAGE

	#define DOUBLE_FROM_LOCALIZED_STRING(NUM) YerothUtils::englishLocale.toDouble(NUM)

	#define GET_NUM_STRING(NUM) YerothUtils::englishLocale.toString(NUM)

	#define GET_DOUBLE_STRING(NUM) YerothUtils::englishLocale.toString(NUM, 'f', 2)
	#define GET_DOUBLE_STRING_P(NUM, P) YerothUtils::englishLocale.toString(NUM, 'f', P)

	#define GET_CURRENCY_STRING_NUM(NUM) YerothUtils::englishLocale.toString(NUM, 'f', 2).append(" ").append(YerothERPConfig::currency)
	#define GET_CURRENCY_STRING_NUM_P(NUM, P) YerothUtils::englishLocale.toString(NUM, 'f', P).append(" ").append(YerothERPConfig::currency)

	#define GET_MONTH_NAME_LOCALIZED(M) YerothUtils::englishLocale.monthName(M)

#endif

#define GET_CURRENCY_STRING_NUM_FOR_LATEX(NUM) YerothUtils::LATEX_IN_OUT_handleForeignAccents(GET_CURRENCY_STRING_NUM(NUM))

#endif /* SRC_YEROTH_UTILS_HPP_ */
