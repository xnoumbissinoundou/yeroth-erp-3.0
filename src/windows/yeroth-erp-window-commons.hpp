/*
 * yeroth-erp-windows-commons.hpp
 *
 *      Author: Xavier NOUMBISSI NOUNDOU, Dipl.-Inf., Ph.D. (ABD)
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
	 _yerothTableView_FROM_WINDOWS_COMMONS(0),
	 _curStocksTableModel(0),
	 _first_time_imprimer_pdf_document_call(true),
	 QMESSAGE_BOX_STYLE_SHEET(QString("QMessageBox {background-color: rgb(%1);}")
								.arg(COLOUR_RGB_STRING_YEROTH_ORANGE_243_162_0)),
	 _windowName(YerothUtils::EMPTY_STRING)
	{
	}

	virtual ~YerothWindowsCommons();

	void setYerothTableView_FROM_WINDOWS_COMMONS(YerothTableView *aYerothTableView_FROM_WINDOWS_COMMONS);

	inline virtual void YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(QAction *anAction,
													   	   	  bool aBooleanValue)
	{
		anAction->setVisible(aBooleanValue);
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
        return _dbtablefieldNameToDBColumnIndex.value(aDBTableColumnName);
    }

    virtual void rendreVisible(YerothSqlTableModel *stocksTableModel);

	inline virtual void rendreInvisible()
	{
		this->setVisible(false);
	}

	virtual void fill_table_columns_to_ignore(QList<int> &tableColumnsToIgnore_in_out);

	inline static YerothERPWindows *getAllWindows()
	{
		return _allWindows;
	}

public slots:

	virtual void getManuelUtilisateurPDF();

	virtual void infosEntreprise();

	virtual void achats();

	virtual void afficherStocks();

	virtual void afficherMarchandises();

	inline virtual void modifier_les_articles()
	{
		this->afficherStocks();
	}

	virtual void alertes();

	virtual void clients();

	inline virtual void courrier()
	{
		this->alertes();
	}

	virtual void sortir();

	virtual void tableaux_de_bords();

	virtual void transactions();

	virtual void creerCompteClient();

	virtual void entrer();

	virtual void paiements();

	virtual void ventes();

	virtual void vendre();

	virtual void menu();

	virtual void comptabilite();

	virtual void administration();

	virtual void setupSelectDBFields(QString aSqlTableName);

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

protected slots:

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

	void mySetupUi(QMainWindow *aWindow);

    virtual void setupShortcuts(){}

    inline void setupShortcutActionMessageDaide(QAction &anAction) const
    {
    	anAction.setShortcut(YerothUtils::MESSAGE_DAIDE_QKEYSEQUENCE);
    }

    inline void setupShortcutActionAfficherPDF(QAction &anAction) const
    {
    	anAction.setShortcut(YerothUtils::IMPRIMER_QKEYSEQUENCE);
    }

    inline void setupShortcutActionQuiSuisJe(QAction &anAction) const
    {
    	anAction.setShortcut(YerothUtils::REINITIALISER_QUI_SUI_JE_QKEYSEQUENCE);
    }


    QMap<QString, int>					_dbtablefieldNameToDBColumnIndex;

    QStringList							_visibleDBFieldColumnStrList;

    QSet<int>							_DBFieldNamesToPrintRightAligned;

    QSet<int>							_DBFieldNamesToPrintLeftAligned;

    QVector<YerothSelectDBQCheckBox *> 	_visibleQCheckboxs;

    YerothERPGenericSelectDBFieldDialog  *_selectExportDBQDialog;


    static YerothERPWindows				 *_allWindows;

    YerothTableViewPRINT_UTILITIES_TEX_TABLE *_yeroth_PRINT_UTILITIES_TEX_TABLE;

    QString								_latex_template_print_pdf_content;

    QString 							_output_print_pdf_latexFileNamePrefix;

    YerothTableView 					 *_yerothTableView_FROM_WINDOWS_COMMONS;

    YerothSqlTableModel 				*_curStocksTableModel;

    bool 								_first_time_imprimer_pdf_document_call;

    QString 							QMESSAGE_BOX_STYLE_SHEET;

    QString 							_windowName;

private:

    static QPoint			*_centerPosition;
};

#endif /* SRC_YEROTH_ERP_WINDOWS_COMMONS_HPP_ */
