/*
 * yeroth-erp-windows-commons.cpp
 *      Author: Xavier NOUMBISSI NOUNDOU, Dipl.-Inf., Ph.D. (ABD)
 */
#include "yeroth-erp-window-commons.hpp"


#include "src/widgets/yeroth-erp-select-db-qcheckbox.hpp"

#include "src/dialogs/yeroth-erp-generic-select-db-field-dialog.hpp"

#include "src/users/yeroth-erp-user-manager.hpp"

#include "src/yeroth-erp-windows.hpp"

#include "src/users/yeroth-erp-users.hpp"


#include <QtSql/QSqlQuery>

#include <QtCore/QtMath>

#include <QtCore/QProcess>

#include <QtCore/QObject>

#include <QtWidgets/QDesktopWidget>

YerothERPWindows *YerothWindowsCommons::_allWindows(0);

QPoint *YerothWindowsCommons::_centerPosition(new QPoint);




YerothWindowsCommons::~YerothWindowsCommons()
{
	if (0 != _selectExportDBQDialog)
	{
		_selectExportDBQDialog->close();

		delete _selectExportDBQDialog;
	}

	_DBFieldNamesToPrintLeftAligned.clear();

	_toSelectDBFieldNameStrToDBColumnIndex.clear();

	_visibleDBFieldColumnStrList.clear();

	_visibleQCheckboxs.clear();

}


void YerothWindowsCommons::mySetupUi(QMainWindow * aWindow)
{
    aWindow->setFixedSize(aWindow->width(), aWindow->height());
    aWindow->move(*_centerPosition);
    aWindow->setWindowTitle(getWindowName());
}

void YerothWindowsCommons::getManuelUtilisateurPDF()
{
    YerothPOSUser *user = _allWindows->getUser();
    if (user)
    {
        QStringList progArguments;
        QProcess aProcess;

        if (user->isAdmin())
        {
            progArguments << YerothERPConfig::FILE_ABSOLUTEPATH_YEROTH_ERP_3_0_MANUEL__DE_LUTILISATEUR_ADMINISTRATEUR;
            aProcess.startDetached(YerothERPConfig::pathToPdfReader, progArguments);
        }
        else if (user->isCaissier())
        {
            progArguments << YerothERPConfig::FILE_ABSOLUTEPATH_YEROTH_ERP_3_0_MANUEL_DE_LUTILISATEUR_CAISSIER;
            aProcess.startDetached(YerothERPConfig::pathToPdfReader, progArguments);
        }
        else if (user->isGestionaireDesStocks())
        {
            progArguments << YerothERPConfig::FILE_ABSOLUTEPATH_YEROTH_ERP_3_0_MANUEL_DE_LUTILISATEUR_GESTIONAIREDESTOCKS;
            aProcess.startDetached(YerothERPConfig::pathToPdfReader, progArguments);
        }
        else if (user->isMagasinier())
        {
            progArguments << YerothERPConfig::FILE_ABSOLUTEPATH_YEROTH_ERP_3_0_MANUEL_DE_LUTILISATEUR_MAGASINIER;
            aProcess.startDetached(YerothERPConfig::pathToPdfReader, progArguments);
        }
        else if (user->isManager())
        {
            progArguments << YerothERPConfig::FILE_ABSOLUTEPATH_YEROTH_ERP_3_0_MANUEL_DE_LUTILISATEUR_MANAGER;
            aProcess.startDetached(YerothERPConfig::pathToPdfReader, progArguments);
        }
    }
}

void YerothWindowsCommons::infosEntreprise()
{
    YerothUtils::infosEntreprise(*this, _allWindows->getInfoEntreprise().toString());
}

void YerothWindowsCommons::achats()
{
    rendreInvisible();
    _allWindows->_achatsWindow->rendreVisible(_curStocksTableModel);
}

void YerothWindowsCommons::afficherStocks()
{
    rendreInvisible();
    _allWindows->_stocksWindow->rendreVisible(_curStocksTableModel);
}

void YerothWindowsCommons::afficherMarchandises()
{
	rendreInvisible();
    _allWindows->_marchandisesWindow->rendreVisible(_curStocksTableModel);
}


void YerothWindowsCommons::alertes()
{
	rendreInvisible();
    _allWindows->_listerAlertesWindow->rendreVisible(_curStocksTableModel);
}


void YerothWindowsCommons::clients()
{
	rendreInvisible();
    _allWindows->_clientWindow->rendreVisible(_curStocksTableModel);
}

void YerothWindowsCommons::sortir()
{
	rendreInvisible();
    _allWindows->_sortirWindow->rendreVisible(_curStocksTableModel);
}

void YerothWindowsCommons::tableaux_de_bords()
{
	rendreInvisible();
    _allWindows->_statistiquesWindow->rendreVisible(_curStocksTableModel);
}

void YerothWindowsCommons::transactions()
{
    _allWindows->_transactionsWindow->rendreVisible(_curStocksTableModel);
    rendreInvisible();
}


void YerothWindowsCommons::creerCompteClient()
{
	rendreInvisible();
    _allWindows->_creerCompteClientWindow->rendreVisible(_curStocksTableModel);
}


void YerothWindowsCommons::entrer()
{
	rendreInvisible();
    _allWindows->_entrerWindow->rendreVisible(_curStocksTableModel);
}

void YerothWindowsCommons::paiements()
{
	rendreInvisible();
    _allWindows->_historiquePaiementsWindow->rendreVisible(_curStocksTableModel);
}

void YerothWindowsCommons::ventes()
{
	rendreInvisible();
    _allWindows->_ventesWindow->rendreVisible(_curStocksTableModel);
}

void YerothWindowsCommons::vendre()
{
	rendreInvisible();
    _allWindows->_pdVenteWindow->rendreVisible(_curStocksTableModel);
}

void YerothWindowsCommons::menu()
{
    rendreInvisible();
    _allWindows->_mainWindow->rendreVisible(_curStocksTableModel);
}


void YerothWindowsCommons::comptabilite()
{
    rendreInvisible();
    _allWindows->_configurationComptabiliteWindow->rendreVisible(_curStocksTableModel);
}


void YerothWindowsCommons::administration()
{
    rendreInvisible();
    _allWindows->_adminWindow->rendreVisible(_curStocksTableModel);
}


void YerothWindowsCommons::setupSelectDBFields(QString aSqlTableName)
{
	_selectExportDBQDialog = new YerothERPGenericSelectDBFieldDialog(_allWindows, this);

	if ( 0!= getQMainWindowToolBar())
	{
		_selectExportDBQDialog->setPalette(getQMainWindowToolBar()->palette());
	}

	_selectExportDBQDialog->setStyleSheet(qMessageBoxStyleSheet());

	QString strShowColumnQuery(QString("SHOW COLUMNS FROM %1")
									.arg(aSqlTableName));

	QSqlQuery query;

	int querySize = YerothUtils::execQuery(query, strShowColumnQuery);

//	qDebug() << QString("++ size: %1")
//					.arg(QString::number(querySize));

	unsigned int columnIdx = -1;

	for( unsigned int k = 0; k < querySize && query.next(); ++k)
	{
		QString fieldName(query.value(0).toString());

		QString type(query.value(1).toString());

		columnIdx = columnIdx + 1;

		if (type.contains("char") ||
			type.contains("time") ||
			type.contains("date") )
		{
			_DBFieldNamesToPrintLeftAligned.append(columnIdx);
		}

		_toSelectDBFieldNameStrToDBColumnIndex.insert(fieldName, columnIdx);
	}
}


void YerothWindowsCommons::tableView_show_or_hide_columns(YerothTableView &tableView_in_out)
{
    QMapIterator<QString, int> it(_toSelectDBFieldNameStrToDBColumnIndex);

    QString fieldColumn;

    while (it.hasNext())
    {
    	it.next();

    	fieldColumn.clear();
    	fieldColumn.append(it.key());

    	if (_visibleDBFieldColumnStrList.contains(fieldColumn))
    	{
    		tableView_in_out.showColumn(it.value());
    	}
    	else
    	{
    		tableView_in_out.hideColumn(it.value());
    	}
    }

    tableView_in_out.selectRow(tableView_in_out.lastSelectedRow());
}


void YerothWindowsCommons::supprimer_image(QLabel &aLabel_image_produit)
{
	aLabel_image_produit.clear();
	aLabel_image_produit.setAutoFillBackground(false);
}


int YerothWindowsCommons::getDialogBox_LONGUEUR(unsigned int n)
{
	static unsigned int delta = 218;

	int result = qCeil(n/8.0);

	return (result * delta);
}


int YerothWindowsCommons::getDialogBox_Yn_coordinate(unsigned int n)
{
	#define N 8

	static unsigned int y[N] = {7, 37, 67, 97, 127, 157, 187, 217};

	unsigned int modyn = (n%N);

	return y[modyn];
}


int YerothWindowsCommons::getDialogBox_Xn_coordinate(unsigned int n)
{
	static unsigned int w = 200;

	static unsigned int X_0 = 7;

	int result = qFloor(n/8);

	return (X_0 + (result * w));
}


void YerothWindowsCommons::selectionner_champs_db_visibles()
{
	unsigned int toSelectDBFieldNameStrSize = _toSelectDBFieldNameStrToDBColumnIndex.size();

	if (_visibleDBFieldColumnStrList.size() >= 0)
	{
		_visibleQCheckboxs.clear();
		_visibleQCheckboxs.resize(_visibleDBFieldColumnStrList.size());
	}

	YerothSelectDBQCheckBox *aQCheckBox = 0;

	for(unsigned int k = 0; k < toSelectDBFieldNameStrSize; ++k)
	{

		aQCheckBox = new YerothSelectDBQCheckBox(_selectExportDBQDialog, &_visibleDBFieldColumnStrList);

		QString dbFieldName(_toSelectDBFieldNameStrToDBColumnIndex.key(k));

		aQCheckBox->setObjectName(dbFieldName);

		aQCheckBox->setGeometry(QRect(getDialogBox_Xn_coordinate(k),
									  getDialogBox_Yn_coordinate(k),
									  200,
									  25));

		aQCheckBox->setText(YerothDatabaseTableColumn::_tableColumnToUserViewString.value(dbFieldName));

		if (_visibleDBFieldColumnStrList.contains(dbFieldName))
		{
			aQCheckBox->setChecked(true);
		}

		connect(aQCheckBox, SIGNAL(clicked(bool)),
				aQCheckBox, SLOT(handle_visible_db_field_checkBox(bool)));

		_visibleQCheckboxs.append(aQCheckBox);
	}

	_selectExportDBQDialog->setWindowTitle(QString(QObject::trUtf8("('%1' colones de base de données)"))
												.arg(QString::number(toSelectDBFieldNameStrSize)));

	_selectExportDBQDialog->setFixedSize(getDialogBox_LONGUEUR(toSelectDBFieldNameStrSize), 255);

	_selectExportDBQDialog->showAsModalDialogWithParent(*this);
}


void YerothWindowsCommons::fill_table_columns_to_ignore(QList<int> &tableColumnsToIgnore_in_out)
{
    QMapIterator<QString, int> it(_toSelectDBFieldNameStrToDBColumnIndex);

    QString curFieldColumn;

    while (it.hasNext())
    {
    	it.next();

    	curFieldColumn = it.key();

    	if (curFieldColumn.isEmpty())
    	{
    		continue;
    	}

    	if (!_visibleDBFieldColumnStrList.contains(curFieldColumn))
    	{
    		tableColumnsToIgnore_in_out.append(it.value());
    	}
    }
}


void YerothWindowsCommons::qui_suis_je()
{
    YerothQMessageBox::information(this, QObject::tr("qui suis je ?"), _allWindows->getUser()->toString());
}


void YerothWindowsCommons::changer_utilisateur()
{
    if (YerothUtils::isEqualCaseInsensitive(YerothDatabase::QSQLITE, YerothERPConfig::_db_type))
    {
        YerothPOSUserManager *u = new YerothPOSUserManager(_allWindows);
        u->set_prenom("Xavier");
        u->set_nom("NOUMBISSI-NOUNDOU");
        u->set_nom_utilisateur("xavierp");
        u->set_localisation("Yaoundé");
        u->set_nom_complet("Xavier NOUMBISSI-NOUNDOU");
        _allWindows->setUser(u);
    }
    else
    {
        _allWindows->_changerUtilisateurDialog->setParent(this, Qt::Popup | Qt::Dialog);
        _allWindows->_changerUtilisateurDialog->show();
        _allWindows->_changerUtilisateurDialog->activateWindow();
    }
}

void YerothWindowsCommons::deconnecter_utilisateur()
{
    _allWindows->definirPasDeRole();
    _allWindows->_mainWindow->show();
    rendreInvisible();
}

void YerothWindowsCommons::rendreVisible(YerothSqlTableModel * stocksTableModel)
{
    _curStocksTableModel = stocksTableModel;
    setVisible(true);
}
