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

#include <QtCore/QProcess>

#include <QtCore/QObject>

#include <QtWidgets/QDesktopWidget>

YerothERPWindows *YerothWindowsCommons::_allWindows(0);

QPoint *YerothWindowsCommons::_centerPosition(new QPoint);

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
    this->rendreInvisible();
    _allWindows->_achatsWindow->rendreVisible(_curStocksTableModel);
}

void YerothWindowsCommons::afficherStocks()
{
    this->rendreInvisible();
    _allWindows->_stocksWindow->rendreVisible(_curStocksTableModel);
}

void YerothWindowsCommons::afficherMarchandises()
{
	this->rendreInvisible();
    _allWindows->_marchandisesWindow->rendreVisible(_curStocksTableModel);
}


void YerothWindowsCommons::alertes()
{
	this->rendreInvisible();
    _allWindows->_listerAlertesWindow->rendreVisible(_curStocksTableModel);
}


void YerothWindowsCommons::clients()
{
	this->rendreInvisible();
    _allWindows->_clientWindow->rendreVisible(_curStocksTableModel);
}

void YerothWindowsCommons::sortir()
{
	this->rendreInvisible();
    _allWindows->_sortirWindow->rendreVisible(_curStocksTableModel);
}

void YerothWindowsCommons::tableaux_de_bords()
{
	this->rendreInvisible();
    _allWindows->_statistiquesWindow->rendreVisible(_curStocksTableModel);
}

void YerothWindowsCommons::transactions()
{
    _allWindows->_transactionsWindow->rendreVisible(_curStocksTableModel);
    this->rendreInvisible();
}


void YerothWindowsCommons::creerCompteClient()
{
	this->rendreInvisible();
    _allWindows->_creerCompteClientWindow->rendreVisible(_curStocksTableModel);
}


void YerothWindowsCommons::entrer()
{
	this->rendreInvisible();
    _allWindows->_entrerWindow->rendreVisible(_curStocksTableModel);
}

void YerothWindowsCommons::paiements()
{
	this->rendreInvisible();
    _allWindows->_historiquePaiementsWindow->rendreVisible(_curStocksTableModel);
}

void YerothWindowsCommons::ventes()
{
	this->rendreInvisible();
    _allWindows->_ventesWindow->rendreVisible(_curStocksTableModel);
}

void YerothWindowsCommons::vendre()
{
	this->rendreInvisible();
    _allWindows->_pdVenteWindow->rendreVisible(_curStocksTableModel);
}

void YerothWindowsCommons::menu()
{
    this->rendreInvisible();
    _allWindows->_mainWindow->rendreVisible(_curStocksTableModel);
}


void YerothWindowsCommons::comptabilite()
{
    this->rendreInvisible();
    _allWindows->_configurationComptabiliteWindow->rendreVisible(_curStocksTableModel);
}


void YerothWindowsCommons::administration()
{
    this->rendreInvisible();
    _allWindows->_adminWindow->rendreVisible(_curStocksTableModel);
}


void YerothWindowsCommons::selectionner_champs_db_visibles()
{
	unsigned int toSelectDBFieldNameStrSize = _toSelectDBFieldNameStrToDBColumnIndex.size();

	if (_visibleDBFieldColumnStrList.size() >= 0)
	{
		_visibleQCheckboxs.clear();
		_visibleQCheckboxs.resize(_visibleDBFieldColumnStrList.size());
	}

	unsigned int dialogBoxHeight = (toSelectDBFieldNameStrSize * 30);

	unsigned int checkBox_X = 7;
	unsigned int checkBox_Y = 3;

	YerothSelectDBQCheckBox *aQCheckBox = 0;

	for(unsigned int k = 0; k < toSelectDBFieldNameStrSize; ++k)
	{
		if (k > 0)
		{
			checkBox_Y = checkBox_Y + 30;
		}

		aQCheckBox = new YerothSelectDBQCheckBox(_selectExportDBQDialog, &_visibleDBFieldColumnStrList);

		QString dbFieldName(_toSelectDBFieldNameStrToDBColumnIndex.key(k));

		aQCheckBox->setObjectName(dbFieldName);

		aQCheckBox->setGeometry(QRect(checkBox_X, checkBox_Y, 200, 25));

		aQCheckBox->setText(YerothDatabaseTableColumn::_tableColumnToUserViewString.value(dbFieldName));

		if (_visibleDBFieldColumnStrList.contains(dbFieldName))
		{
			aQCheckBox->setChecked(true);
		}

		connect(aQCheckBox, SIGNAL(clicked(bool)),
				aQCheckBox, SLOT(handle_visible_db_field_checkBox(bool)));

		_visibleQCheckboxs.append(aQCheckBox);
	}

	_selectExportDBQDialog->setFixedSize(205, dialogBoxHeight);

	_selectExportDBQDialog->showAsModalDialogWithParent(*_allWindows->_stocksWindow);
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

	unsigned int vectorSize = 0;

	for( unsigned int k = 0; k < querySize && query.next(); ++k)
	{
		QString type(query.value(1).toString());

		columnIdx = columnIdx + 1;

		if (type.contains("int(") 		||
			type.contains("double") 	||
			type.contains("date") 		||
			type.contains("blob") 		||
			type.contains("varchar("))
		{
			QString fieldName(query.value(0).toString());

			if (!fieldName.isEmpty())
			{
				_toSelectDBFieldNameStrToDBColumnIndex.insert(fieldName, columnIdx);
//				qDebug() << "++ str: " <<  query.value(0).toString();
			}
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
    this->rendreInvisible();
}

void YerothWindowsCommons::rendreVisible(YerothSqlTableModel * stocksTableModel)
{
    _curStocksTableModel = stocksTableModel;
    this->setVisible(true);
}
