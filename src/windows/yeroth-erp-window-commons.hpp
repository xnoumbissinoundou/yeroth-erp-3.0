/*
 * yeroth-erp-windows-commons.hpp
 *
 *      Author: DR. XAVIER NOUMBISSI NOUNDOU
 */

#ifndef SRC_YEROTH_ERP_WINDOWS_COMMONS_HPP_
#define SRC_YEROTH_ERP_WINDOWS_COMMONS_HPP_

#include "src/include/yeroth-erp-3-0-software.text-configuration.hpp"

#include "src/utils/yeroth-erp-logger.hpp"

#include "src/widgets/yeroth-erp-qmessage-box.hpp"

#include "src/utils/yeroth-erp-utils.hpp"

#include "src/utils/windows/yeroth-erp-abstract-class-yerothsearch-window.hpp"


#include <QtCore/QSet>

#include <QtWidgets/QAction>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMessageBox>


class YerothTableViewPRINT_UTILITIES_TEX_TABLE;
class YerothSelectDBQCheckBox;
class YerothERPGenericSelectDBFieldDialog;
class YerothERPWindows;


class YerothWindowsCommons : public QMainWindow
{
    Q_OBJECT

public:

	YEROTH_CLASS_OPERATORS

	inline YerothWindowsCommons(const QString &anOutput_print_pdf_latexFileNamePrefix = "")
	:_selectExportDBQDialog(0),
	 _yeroth_PRINT_UTILITIES_TEX_TABLE(0),
	 _output_print_pdf_latexFileNamePrefix(anOutput_print_pdf_latexFileNamePrefix),
	 _yeroth_QComboBox_SearchDBFieldColumnString(0),
	 _yeroth_QLineEdit_SearchDBFieldColumnString(0),
	 _curStocksTableModel(0),
	 _pagination_nombre_de_ligne_IntValidator(0),
	 _yerothTableView_FROM_WINDOWS_COMMONS(0),
	 _first_time_imprimer_pdf_document_call(true),
	 QMESSAGE_BOX_STYLE_SHEET(QString("QMessageBox {background-color: rgb(%1);}")
								.arg(COLOUR_RGB_STRING_YEROTH_ORANGE_243_162_0)),
	 _windowName(YerothUtils::EMPTY_STRING)
	{
	}

	virtual ~YerothWindowsCommons();

	int get_INT_last_selected_row_number();

	void setYerothTableView_FROM_WINDOWS_COMMONS(const QList<YerothTableView *> &aYerothTableView_FROM_WINDOWS_COMMONS_QLIST);

	void setYerothTableView_FROM_WINDOWS_COMMONS(YerothTableView *aYerothTableView_FROM_WINDOWS_COMMONS);

	inline virtual void YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(QAction *anAction,
													   	   	  bool aBooleanValue)
	{
		anAction->setVisible(aBooleanValue);
	}

	inline QSet<int> &getDBFieldNamesToPrintCenterAligned()
	{
		return _DBFieldNamesToPrintCenterAligned;
	}

	inline QSet<int> &getDBFieldNamesToPrintRightAligned()
	{
		return _DBFieldNamesToPrintRightAligned;
	}

	inline QSet<int> &getDBFieldNamesToPrintLeftAligned()
	{
		return _DBFieldNamesToPrintLeftAligned;
	}

	inline virtual QToolBar * getQMainWindowToolBar()
	{
		return 0;
	}

	inline QString getWindowName()
	{
		return _windowName;
	}

	inline virtual QString qMessageBoxStyleSheet()
	{
		return QMESSAGE_BOX_STYLE_SHEET;
	}

	inline static QPoint * getCenterPosition()
	{
		return _centerPosition;
	}

	inline static void setCenterPosition(QPoint *desktopCenterPosition)
	{
		_centerPosition = desktopCenterPosition;
	}

	inline static void setAllWindows(YerothERPWindows *allWindows)
	{
		_allWindows = allWindows;
	}

	inline YerothSqlTableModel * getCurStocksTableModel()
	{
		return _curStocksTableModel;
	}

    virtual void definirCaissier(){}

    virtual void definirManager(){}

    virtual void definirGestionaireDesStocks(){}

    virtual void definirMagasinier(){}

    virtual void definirPasDeRole(){}

    inline int getDBTableFieldColumnIndex(const QString &aDBTableColumnName)
    {
        return _dbtablecolumnNameToDBColumnIndex.value(aDBTableColumnName);
    }

    virtual void rendreVisible(YerothSqlTableModel *stocksTableModel);

	inline virtual void rendreInvisible()
	{
		setVisible(false);
	}

	virtual void fill_table_columns_to_ignore(QList<int> &tableColumnsToIgnore_in_out);

	inline static YerothERPWindows *getAllWindows()
	{
		return _allWindows;
	}

	bool SQL_UPDATE_YEROTH_TABLE_VIEW_LAST_SELECTED_ROW(QSqlRecord &resultRecord_IN);

	bool SQL_QUERY_YEROTH_TABLE_VIEW_LAST_SELECTED_ROW(QSqlRecord &resultRecord_IN_OUT);

	bool SQL_DELETE_YEROTH_TABLE_VIEW_LAST_SELECTED_ROW();

	inline static const QString & get_last_lister_selected_row_ID()
	{
		return _yerothTableView_FROM_WINDOWS_COMMONS_LAST_SELECTED_ROW__ID;
	}

	int get_INT_LastListerSelectedRow__ID();

    const QString & getLastListerSelectedRow__ID();

public slots:

	virtual void getManuelUtilisateurPDF();

	virtual void infosEntreprise();

	virtual void achats_aux_fournisseurs();

	virtual void achats_de_stocks();

	virtual void afficherStocks();

	virtual void afficherMarchandises();

	inline virtual void modifier_les_articles()
	{
		this->afficherStocks();
	}

	virtual void alertes();

	virtual void fournisseurs();

	virtual void groupes_de_clients();

	virtual void creer_un_programme_de_fidelite_clients();

	virtual void clients();

	inline virtual void courrier()
	{
		this->alertes();
	}

	virtual void sortir();

	virtual void tableaux_de_bords();

	virtual void mouvements_de_stocks();

	virtual void creerFournisseur();

	virtual void creerCompteClient();

	virtual void creerUnGroupeDeClients();

	virtual void entrer();

	virtual void paiements();

	virtual void ventes();

	virtual void vendre();

	virtual void menu();

	virtual void comptabilite();

	virtual void administration();

	virtual void setup_select_configure_dbcolumn(const QString &aSqlTableName);

	virtual void qui_suis_je();

	virtual bool imprimer_pdf_document(QMap<QString, QString> *documentSpecificElements = 0);

    virtual void changer_utilisateur();

    virtual void deconnecter_utilisateur();

    inline virtual void apropos(YerothWindowsCommons *aCallingWindow)
    {
    	YerothQMessageBox::about(aCallingWindow,
    					   YerothUtils::APPLICATION_NAME,
						   YerothUtils::APROPOS_MSG);
    }

    inline void fermeture()
    {
    	this->close();
    }

    inline virtual void help(){}

    void resetTableViewHorizontalHeader_DEFAULT_ORDERING();

	inline virtual void setYerothTableViewLastPageNumberText(const QString &aLastPageNumberText)
	{
	}

	inline virtual void setYerothTableViewCurrentPageNumberText(const QString &aNextPageNumberText)
	{
	}

	virtual void updateYerothLineEditQCompleter(const QString &currentDBColumnString);

	inline virtual const QStringList &get_NOT_VISIBLE_FOR_USER_DB_TABLE_COLUMN_NAME()
	{
		return _NOT_VISIBLE_FOR_USER_DB_TABLE_COLUMN_NAME;
	}

	void getQModelIndex_dbID_from_MODEL_INDEX(const QModelIndex &modelIndex, QString &db_ID_in_out);

	virtual void setLast_YEROTH_TABLE_VIEW_SelectedRow__db_ID();

	virtual void setLast_YEROTH_TABLE_VIEW_SelectedRow__db_ID(const QModelIndex &modelIndex);

protected slots:

	void handleSOMEToolsEnabled();

	inline virtual void disableExporterAuFormatCsv()
	{
	}

	inline virtual void enableExporterAuFormatCsv()
	{
	}

	inline virtual void disableImprimer()
	{
	}

	inline virtual void enableImprimer()
	{
	}

	virtual void setYerothLineEditQCompleterSearchFilter(QString &aYerothLineEditQCompleterSearchFilter_IN_OUT);

	virtual void tableView_show_or_hide_columns(YerothTableView &tableView_in_out);

	void supprimer_image(QLabel &aLabel_image_produit);

	inline void selectionner_image(QLabel &aLabel_image_produit)
	{
		YerothUtils::selectionner_image(this, aLabel_image_produit);
	}

	/**
	 * Ce 'slot' est suppose reinitialiser les chanps
	 * du tableau de la base de donnees et, enfin,
	 * actualiser la nouvelle vue du tableau.
	 *
	 * Exemple d'implementation standard dans la classe
	 * 'YerothStocksWindow'.
	 */
	inline virtual void slot_reinitialiser_champs_db_visibles()
	{
		reinitialiser_champs_db_visibles();
	}

	static int getDialogBox_LONGUEUR(unsigned int n);

	static int getDialogBox_Yn_coordinate(unsigned int n);

	static int getDialogBox_Xn_coordinate(unsigned int n);

	virtual void selectionner_champs_db_visibles();

protected:

	inline virtual void reinitialiser_champs_db_visibles()
	{
	}

	void setLeftAligned_FOR_YEROTH_PDF_LATEX_PRINTING(const QString &dbtableColumnString);

    void setLeftAligned_FOR_YEROTH_PDF_LATEX_PRINTING(int dbtableColumnIndex);

    void setCenterAligned_FOR_YEROTH_PDF_LATEX_PRINTING(const QString &dbtableColumnString);

    void setCenterAligned_FOR_YEROTH_PDF_LATEX_PRINTING(int dbtableColumnIndex);

    void setRightAligned_FOR_YEROTH_PDF_LATEX_PRINTING(const QString &dbtableColumnString);

    void setRightAligned_FOR_YEROTH_PDF_LATEX_PRINTING(int dbtableColumnIndex);

	void enableResizing();

	void mySetupUi(QMainWindow *aWindow);

    virtual void setupShortcuts(){}

    inline void setupShortcutActionMessageDaide(QAction &anAction) const
    {
    	anAction.setShortcut(YerothUtils::MESSAGE_DAIDE_QKEYSEQUENCE);
    }

    inline void setupShortcutActionExporterAuFormatCsv(QAction &anAction) const
    {
    	anAction.setShortcut(YerothUtils::EXPORTER_AU_FORMAT_CSV_QKEYSEQUENCE);
    }

    inline void setupShortcutActionAfficherPDF(QAction &anAction) const
    {
    	anAction.setShortcut(YerothUtils::IMPRIMER_QKEYSEQUENCE);
    }

    inline void setupShortcutActionQuiSuisJe(QAction &anAction) const
    {
    	anAction.setShortcut(YerothUtils::REINITIALISER_QUI_SUI_JE_QKEYSEQUENCE);
    }


    /**
     * Ces membres (les 3 suivants) de la classe sont utilises afin d'executer
     * la 'recherche' automatique.
     *
     * Ces membres (les 3 suivants) sont assignes des valeurs
     * dans le constructeur de la classe descendante de "QMainWindow",
     * et de "YerothAbstractClassYerothSearchWindow".
     */
	QSet<QString> 						_yeroth_references_dbColumnString;

	YerothComboBox 						*_yeroth_QComboBox_SearchDBFieldColumnString;

	YerothLineEdit 						*_yeroth_QLineEdit_SearchDBFieldColumnString;


	QString								_dbYerothSqlTableName_WINDOWS_TABLE_VIEW_FOR_SEARCH;

	QMap<QString, QString>				_db_VIEW_STRING_ALPHABETIC_ORDER_To_dbtablecolumnName;

	QMap<QString, int>					_dbtablecolumnNameToDBColumnIndex;

    QSet<QString>						_varchar_dbtable_column_name_list;


    QStringList							_NOT_VISIBLE_FOR_USER_DB_TABLE_COLUMN_NAME;

    QStringList							_visibleDBColumnNameStrList;

    QSet<int>							_DBFieldNamesToPrintCenterAligned;

    QSet<int>							_DBFieldNamesToPrintRightAligned;

    QSet<int>							_DBFieldNamesToPrintLeftAligned;

    QVector<YerothSelectDBQCheckBox *> 	_visibleQCheckboxs;

    YerothERPGenericSelectDBFieldDialog  *_selectExportDBQDialog;


    static YerothERPWindows				 *_allWindows;

    YerothTableViewPRINT_UTILITIES_TEX_TABLE *_yeroth_PRINT_UTILITIES_TEX_TABLE;

    QString								_latex_template_print_pdf_content;

    QString 							_output_print_pdf_latexFileNamePrefix;

    YerothSqlTableModel 				*_curStocksTableModel;

    QIntValidator 						*_pagination_nombre_de_ligne_IntValidator;

    bool 								_first_time_imprimer_pdf_document_call;

    QString 							QMESSAGE_BOX_STYLE_SHEET;

    QString 							_windowName;

    YerothTableView 					*_yerothTableView_FROM_WINDOWS_COMMONS;

private:

    static QString 						_yerothTableView_FROM_WINDOWS_COMMONS_LAST_SELECTED_ROW__ID;

    static QPoint						*_centerPosition;
};




#define MACRO_TO_DISABLE_PAGE_FIRST_NEXT_PREVIOUS_LAST_PUSH_BUTTONS		\
	    pushButton_page_premiere->disable(this);						\
		pushButton_page_derniere->disable(this);						\
	    pushButton_page_precedente->disable(this);						\
		pushButton_page_suivante->disable(this);


#define MACRO_TO_DEFINE_VIEWING_POINTERS_PAGE_SLOTS(X)	\
		inline virtual void viewYerothTableViewFirstPage(YerothSqlTableModel &curYerothSqlTableModel_IN) \
		{																								 \
			X->viewYerothTableViewFirstPage(curYerothSqlTableModel_IN);									 \
		}																								 \
		inline virtual void viewYerothTableViewLastPage(YerothSqlTableModel &curYerothSqlTableModel_IN)  \
		{																								 \
			X->viewYerothTableViewLastPage(curYerothSqlTableModel_IN);									 \
		}																								 \
		inline virtual void viewYerothTableViewPreviousPage(YerothSqlTableModel &curYerothSqlTableModel_IN) \
		{																								 \
			X->viewYerothTableViewPreviousPage(curYerothSqlTableModel_IN);			 					 \
		}																								 \
		inline virtual void viewYerothTableViewNextPage(YerothSqlTableModel &curYerothSqlTableModel_IN)	 \
		{																								 \
			X->viewYerothTableViewNextPage(curYerothSqlTableModel_IN);									 \
		}


#define MACRO_TO_ENABLE_PAGE_FIRST_NEXT_PREVIOUS_LAST_PUSH_BUTTONS(X, Y)					\
	    pushButton_page_premiere->enable_WITH_SQL_TABLE_MODEL_AS_CALL_ARGUMENT((YerothAbstractClassYerothSearchWindow*)X, ((YerothAbstractClassYerothSearchWindow*)X)->Get__viewYerothTableViewFirstPage__FPointer(X), &Y);	\
		pushButton_page_derniere->enable_WITH_SQL_TABLE_MODEL_AS_CALL_ARGUMENT((YerothAbstractClassYerothSearchWindow*)X, ((YerothAbstractClassYerothSearchWindow*)X)->Get__viewYerothTableViewLastPage__FPointer(X), &Y);	\
	    pushButton_page_precedente->enable_WITH_SQL_TABLE_MODEL_AS_CALL_ARGUMENT((YerothAbstractClassYerothSearchWindow*)X, ((YerothAbstractClassYerothSearchWindow*)X)->Get__viewYerothTableViewPreviousPage__FPointer(X), &Y);\
		pushButton_page_suivante->enable_WITH_SQL_TABLE_MODEL_AS_CALL_ARGUMENT((YerothAbstractClassYerothSearchWindow*)X, ((YerothAbstractClassYerothSearchWindow*)X)->Get__viewYerothTableViewNextPage__FPointer(X), &Y);


#define MACRO_TO_DEFINE_VIEWING_PAGE_NUMBER_FOR_TABLEVIEW(X, Y) 				\
	inline virtual void setYerothTableViewLastPageNumberText(const QString &aLastPageNumberText) \
	{																							 \
		X->setText(aLastPageNumberText);						 								 \
	}																							 \
																								 \
	inline virtual void setYerothTableViewCurrentPageNumberText(const QString &aNextPageNumberText) \
	{																								\
		Y->setText(aNextPageNumberText);															\
	}


#define MACRO_TO_DEFINE_CURRENT_VIEW_WINDOW_FOR_TABLE_PAGINATION(X) \
		X->_currentViewWindow = this;


#define MACRO_TO_DELETE_PAGINATION_INTEGER_VALIDATOR 		\
	{														\
		if (0 != _pagination_nombre_de_ligne_IntValidator) 	\
		{													\
			delete _pagination_nombre_de_ligne_IntValidator;\
		}													\
	}


#define MACRO_TO_BIND_PAGING_WITH_QLINEEDIT(X, Y) 				\
	{ 															\
			if (0 == _pagination_nombre_de_ligne_IntValidator) 	\
			{					 								\
				_pagination_nombre_de_ligne_IntValidator = 		\
					new QIntValidator(1, YerothUtils::YEROTH_PAGINATION_INT_VALIDATOR_MAXIMAL_VALUE); 	\
				X->setValidator(_pagination_nombre_de_ligne_IntValidator); 					\
			} 																				\
																				\
			X->setText(QString::number(Y->getYerothTableViewPageRowCount())); 	\
																				\
			connect(X,															\
					SIGNAL(textEdited(const QString &)),	  					\
					Y,						  									\
					SLOT(slot_set_page_view_row_count(const QString &))); 		\
	}


#endif /* SRC_YEROTH_ERP_WINDOWS_COMMONS_HPP_ */
