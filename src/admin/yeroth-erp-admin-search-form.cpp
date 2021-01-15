/**
 * yeroth-erp-admin-search-form.cpp
 *      Author: DR. XAVIER NOUMBISSI NOUNDOU
 */

#include "yeroth-erp-admin-search-form.hpp"

#include "src/yeroth-erp-windows.hpp"

#include "src/utils/yeroth-erp-utils.hpp"

#include "src/utils/yeroth-erp-sqltable-model.hpp"

#include "src/utils/yeroth-erp-logger.hpp"

#include <QtCore/QDebug>

#include <QtCore/QRegExp>

#include <QtCore/QStringList>

#include <QtSql/QSqlError>

#include <QtWidgets/QLabel>

#include <QtWidgets/QFormLayout>

#include <QtWidgets/QDesktopWidget>

#include <QtWidgets/QCompleter>

YerothAdminSearchForm::YerothAdminSearchForm(YerothERPWindows * allWindows, QWidget * parent):QDialog(parent), _curSujetAction(SUJET_ACTION_ALERTE), _curSqlTableModel(0), _logger(0),
    _allWindows(allWindows)
{
    setupUi(this);
    _logger = new YerothLogger("YerothAdminSearchForm");
    setFixedSize( width(),  height());
    pushButton_annuler->enable(this, SLOT(reinitialiser()));
}

YerothAdminSearchForm::~YerothAdminSearchForm()
{
    delete _logger;
}

void YerothAdminSearchForm::rendreVisible(int tabWidjetListerIdx)
{
    _logger->log("rendreVisible");
    setupLineEditsQCompleters(tabWidjetListerIdx);
    lineEdit_terme_recherche->setFocus();
    show();
}

void YerothAdminSearchForm::rendreInvisible()
{
    _logger->log("rendreInvisible");
    clear_all_fields();
    close();
}


void YerothAdminSearchForm::setupLineEditsQCompleters(int tabWidjetListerIdx)
{
    switch (tabWidjetListerIdx)
    {
    case SUJET_ACTION_ALERTE:

        lineEdit_terme_recherche->enableForSearch(QObject::trUtf8("nom de l'alerte"));
        lineEdit_terme_recherche->setupMyStaticQCompleter(_allWindows->ALERTES,
        												  YerothDatabaseTableColumn::DESIGNATION_ALERTE,
														  false);
        _curSujetAction = SUJET_ACTION_ALERTE;
        _curSqlTableModel = &_allWindows->getSqlTableModel_alertes();
        break;

    case SUJET_ACTION_BON_DE_COMMANDE:

        _curSujetAction = SUJET_ACTION_BON_DE_COMMANDE;
        break;

    case SUJET_ACTION_CATEGORIE:

        lineEdit_terme_recherche->enableForSearch(QObject::trUtf8("nom de la catégorie"));
        lineEdit_terme_recherche->setupMyStaticQCompleter(_allWindows->CATEGORIES,
        												  YerothDatabaseTableColumn::NOM_CATEGORIE,
														  false);
        _curSujetAction = SUJET_ACTION_CATEGORIE;
        _curSqlTableModel = &_allWindows->getSqlTableModel_categories();
        break;

    case SUJET_ACTION_COMPTE_BANCAIRE:

        lineEdit_terme_recherche->enableForSearch(QObject::trUtf8("référence du compte bancaire"));
        lineEdit_terme_recherche->setupMyStaticQCompleter(_allWindows->COMPTES_BANCAIRES,
        												  YerothDatabaseTableColumn::REFERENCE_DU_COMPTE_BANCAIRE,
														  false);
        _curSujetAction = SUJET_ACTION_COMPTE_BANCAIRE;
        _curSqlTableModel = &_allWindows->getSqlTableModel_comptes_bancaires();
        break;

    case SUJET_ACTION_COMPTE_UTILISATEUR:

        lineEdit_terme_recherche->enableForSearch(QObject::trUtf8("nom de l'utilisateur"));
        lineEdit_terme_recherche->setupMyStaticQCompleter(_allWindows->USERS,
        												  YerothDatabaseTableColumn::NOM_COMPLET,
														  false);
        _curSujetAction = SUJET_ACTION_COMPTE_UTILISATEUR;
        _curSqlTableModel = &_allWindows->getSqlTableModel_users();
        break;

    case SUJET_ACTION_LOCALISATION:

        lineEdit_terme_recherche->enableForSearch(QObject::trUtf8("nom de la localisation"));
        lineEdit_terme_recherche->setupMyStaticQCompleter(_allWindows->LOCALISATIONS,
        												  YerothDatabaseTableColumn::NOM_LOCALISATION,
														  false);
        _curSujetAction = SUJET_ACTION_LOCALISATION;
        _curSqlTableModel = &_allWindows->getSqlTableModel_localisations();
        break;

    case SUJET_ACTION_REMISE:

        lineEdit_terme_recherche->enableForSearch(QObject::trUtf8("désignation de la remise"));
        lineEdit_terme_recherche->setupMyStaticQCompleter(_allWindows->REMISES,
        												  YerothDatabaseTableColumn::DESIGNATION_REMISE,
														  false);
        _curSujetAction = SUJET_ACTION_REMISE;
        _curSqlTableModel = &_allWindows->getSqlTableModel_remises();
        break;

    default:
        break;
    }

    connect(lineEdit_terme_recherche->getMyQCompleter(), SIGNAL(activated(const QString &)), this,
            SLOT(rechercher(const QString &)));
}

void YerothAdminSearchForm::reinitialiser()
{
    _logger->log("reinitialiser");

    lineEdit_terme_recherche->clear();

    if (_curSqlTableModel)
    {
        _curSqlTableModel->resetFilter();
    }

    setCurSqlTableModel(0);
    rendreInvisible();
}

void YerothAdminSearchForm::rechercher(const QString & itemName)
{
    //qDebug() << "++ YerothAdminSearchForm::rechercher | itemName: " << itemName;

    QString searchString(itemName);
    QString filter;

    switch (_curSujetAction)
    {
    case SUJET_ACTION_ALERTE:
        filter = GENERATE_SQL_IS_STMT(YerothDatabaseTableColumn::DESIGNATION_ALERTE, searchString);
        break;
    case SUJET_ACTION_BON_DE_COMMANDE:
        break;
    case SUJET_ACTION_CATEGORIE:
        filter = GENERATE_SQL_IS_STMT(YerothDatabaseTableColumn::NOM_CATEGORIE, searchString);
        break;
    case SUJET_ACTION_COMPTE_BANCAIRE:
        filter = GENERATE_SQL_IS_STMT(YerothDatabaseTableColumn::REFERENCE_DU_COMPTE_BANCAIRE, searchString);
        break;
    case SUJET_ACTION_COMPTE_UTILISATEUR:
        filter = GENERATE_SQL_IS_STMT(YerothDatabaseTableColumn::NOM_COMPLET, searchString);
        break;

    case SUJET_ACTION_LOCALISATION:
        filter = GENERATE_SQL_IS_STMT("nom_localisation", searchString);
        break;
    default:
        break;
    }
    //qDebug() << "++ YerothAdminSearchForm::rechercher, searchString: " << searchString;
    if (0 != _curSqlTableModel)
    {
        _curSqlTableModel->yerothSetFilter_WITH_where_clause(filter);
        //qDebug() << "++ filter: "
        //               << filter << ", _curSqlTableModel: "
        //               << _curSqlTableModel->sqlTableName();
        int curSqlTableModelRows = _curSqlTableModel->easySelect();
        if (curSqlTableModelRows > 0)
        {
            switch (_curSujetAction)
            {
            case SUJET_ACTION_ALERTE:
            	_allWindows->_adminListerWindow->setAlertCurrentlyFiltered(true);
                _allWindows->_adminListerWindow->lister_alerte(_curSqlTableModel);
                break;

            case SUJET_ACTION_BON_DE_COMMANDE:
                break;

            case SUJET_ACTION_CATEGORIE:
            	_allWindows->_adminListerWindow->setCategoryCurrentlyFiltered(true);
                _allWindows->_adminListerWindow->lister_categorie(_curSqlTableModel);
                break;

            case SUJET_ACTION_COMPTE_BANCAIRE:
            	_allWindows->_adminListerWindow->setBankAccountCurrentlyFiltered(true);
                _allWindows->_adminListerWindow->lister_compte_bancaire(_curSqlTableModel);
                break;

            case SUJET_ACTION_COMPTE_UTILISATEUR:
            	_allWindows->_adminListerWindow->setUserCurrentlyFiltered(true);
                _allWindows->_adminListerWindow->lister_utilisateur(_curSqlTableModel);
                break;

            case SUJET_ACTION_LOCALISATION:
            	_allWindows->_adminListerWindow->setSiteCurrentlyFiltered(true);
                _allWindows->_adminListerWindow->lister_localisation(_curSqlTableModel);
                break;
            default:
                break;
            }
        }
    }
    rendreInvisible();
}
