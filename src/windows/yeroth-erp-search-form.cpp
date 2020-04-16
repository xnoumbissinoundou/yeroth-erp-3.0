/*
 * yeroth-erp-search-form.cpp
 *
 *      Author: Xavier NOUMBISSI NOUNDOU, Dipl.-Inf., Ph.D. (ABD)
 */

#include "yeroth-erp-search-form.hpp"

#include "src/yeroth-erp-windows.hpp"

#include "src/utils/yeroth-erp-utils.hpp"

#include "src/utils/yeroth-erp-sqltable-model.hpp"

#include "src/utils/yeroth-erp-logger.hpp"

#include <QtWidgets/QLabel>

#include <QtWidgets/QFormLayout>

#include <QtWidgets/QDesktopWidget>

#include <QtWidgets/QCompleter>

#include <QtSql/QSqlError>

#include <QtCore/QDebug>

#include <QtCore/QRegExp>

#include <QtCore/QStringList>


class YerothUtils;
const QString YerothSearchForm::_WINDOW_TITLE(QString(QObject::trUtf8("%1 - %2")).
        arg(YEROTH_ERP_WINDOW_TITLE, QObject::trUtf8("rechercher")));


YerothSearchForm::YerothSearchForm(YerothERPWindows * allWindows,
								   YerothWindowsCommons &aCallingWindow,
								   YerothSqlTableModel * sqlTable,
								   QWidget * parent)
:QDialog(parent),
 _lastStringRecherche1(""),
 _lastStringRecherche2(""),
 _lastStringRecherche3(""),
 _searchFilter(0),
 _sqlTable(sqlTable),
 _allWindows(allWindows)
{
    setupUi(this);

    _logger = new YerothLogger("YerothSearchForm");

    if (0 != aCallingWindow.getQMainWindowToolBar())
    {
    	setPalette(aCallingWindow.getQMainWindowToolBar()->palette());
    }

    setFixedSize(this->width(), this->height());

    _searchFilter = new QString;

    setupLineEditsQCompleters();

    pushButton_ok->enable(this, SLOT(performSearch()));

    pushButton_cancel->enable(this, SLOT(rendreInvisible()));

    pushButton_reset->enable(this, SLOT(reinitialiser()));

    connect(lineEdit_recherche_1->getMyQCompleter(), SIGNAL(activated(const QModelIndex &)), this,
            SLOT(setLastStringRecherche1()));

    connect(lineEdit_recherche_2->getMyQCompleter(), SIGNAL(activated(const QModelIndex &)), this,
            SLOT(setLastStringRecherche2()));

    connect(lineEdit_recherche_3->getMyQCompleter(), SIGNAL(activated(const QModelIndex &)), this,
            SLOT(setLastStringRecherche3()));
}

YerothSearchForm::~YerothSearchForm()
{
    delete _searchFilter;
    delete _logger;
}

void YerothSearchForm::setupLineEditsQCompleters()
{
    if (YerothUtils::isEqualCaseInsensitive(_allWindows->STOCKS, _sqlTable->sqlTableName()) ||
    	YerothUtils::isEqualCaseInsensitive(_allWindows->ACHATS, _sqlTable->sqlTableName())	)
    {
        label_recherche_1->setText(QObject::trUtf8("Catégorie du stock:"));
        label_recherche_2->setText(QObject::trUtf8("Désignation du stock:"));
        label_recherche_3->setText(QObject::trUtf8("Fournisseur:"));
        lineEdit_recherche_1->setupMyStaticQCompleter(_allWindows->STOCKS, YerothDatabaseTableColumn::CATEGORIE, false);
        lineEdit_recherche_2->setupMyStaticQCompleter(_allWindows->STOCKS, YerothDatabaseTableColumn::DESIGNATION, false);
        lineEdit_recherche_3->setupMyStaticQCompleter(_allWindows->STOCKS, YerothDatabaseTableColumn::NOM_ENTREPRISE_FOURNISSEUR, false);
    }
    else if (YerothUtils::isEqualCaseInsensitive(_allWindows->CLIENTS, _sqlTable->sqlTableName()))
    {
        label_recherche_1->setText(QObject::trUtf8("Référence client:"));
        label_recherche_2->setText(QObject::trUtf8("Nom de l'entreprise:"));
        label_recherche_3->setText(QObject::trUtf8("Nom du représentant:"));
        lineEdit_recherche_1->setupMyStaticQCompleter(_allWindows->CLIENTS, YerothDatabaseTableColumn::REFERENCE_CLIENT, false);
        lineEdit_recherche_2->setupMyStaticQCompleter(_allWindows->CLIENTS, YerothDatabaseTableColumn::NOM_ENTREPRISE, false);
        lineEdit_recherche_3->setupMyStaticQCompleter(_allWindows->CLIENTS, YerothDatabaseTableColumn::NOM_REPRESENTANT, false);
    }
    else if (YerothUtils::isEqualCaseInsensitive(_allWindows->MARCHANDISES, _sqlTable->sqlTableName()))
    {
        label_recherche_1->setText(QObject::trUtf8("Catégorie du stock:"));
        label_recherche_2->setText(QObject::trUtf8("Désignation du stock:"));
        label_recherche_3->setText(QObject::trUtf8("Gestionaire de stocks:"));
        lineEdit_recherche_1->setupMyStaticQCompleter(_allWindows->MARCHANDISES, YerothDatabaseTableColumn::CATEGORIE, false);
        lineEdit_recherche_2->setupMyStaticQCompleter(_allWindows->MARCHANDISES, YerothDatabaseTableColumn::DESIGNATION, false);
        lineEdit_recherche_3->setupMyStaticQCompleter(_allWindows->MARCHANDISES, YerothDatabaseTableColumn::STOCK_MANAGER, false);
    }
}

void YerothSearchForm::rendreVisible()
{
    _logger->log("rendreVisible");

    this->setupLineEditsQCompleters();

    if (!_lastStringRecherche1.isEmpty())
    {
        lineEdit_recherche_1->setText(_lastStringRecherche1);
    }
    if (!_lastStringRecherche2.isEmpty())
    {
        lineEdit_recherche_2->setText(_lastStringRecherche2);
    }
    if (!_lastStringRecherche3.isEmpty())
    {
        lineEdit_recherche_3->setText(_lastStringRecherche3);
    }

    this->show();
}

void YerothSearchForm::clear_all_fields()
{
    _searchFilter->clear();
    lineEdit_terme_recherche->clear();
    lineEdit_recherche_1->clear();
    lineEdit_recherche_2->clear();
    lineEdit_recherche_3->clear();
}

void YerothSearchForm::reinitialiser()
{
    _logger->log("reinitialiser");

    _searchFilter->clear();

    lineEdit_terme_recherche->clear();

    lineEdit_recherche_1->clear();
    lineEdit_recherche_2->clear();
    lineEdit_recherche_3->clear();

    _lastStringRecherche1.clear();
    _lastStringRecherche2.clear();
    _lastStringRecherche3.clear();

    _sqlTable->resetFilter();

    this->setupLineEditsQCompleters();

    pushButton_ok->click();
}

void YerothSearchForm::performSearch()
{
    _logger->log("performSearch");

    if (0 == _sqlTable)
    {
        QMessageBox::warning(this,
        					 QObject::trUtf8("rechercher"),
							 QObject::trUtf8("Recherche impossible. SVP contactez \"YEROTH\"."));
        return;
    }

    _sqlTable->resetFilter();

    _searchFilter->clear();

    QString searchTerm(lineEdit_terme_recherche->text());

    if (!searchTerm.isEmpty())
    {
        QStringList searchTermList = searchTerm.split(QRegExp("\\s+"));

        QString partSearchTerm;

        int lastIdx = searchTermList.size() - 1;

        for (int k = 0; k < searchTermList.size(); ++k)
        {
            partSearchTerm = searchTermList.at(k);
            //qDebug() << "++ searchTermList: " << partSearchTerm;
            if (YerothUtils::isEqualCaseInsensitive(_allWindows->STOCKS, _sqlTable->sqlTableName()) ||
            	YerothUtils::isEqualCaseInsensitive(_allWindows->ACHATS, _sqlTable->sqlTableName())   )
            {
                _searchFilter->append(QString("(%1 OR %2 OR %3)")
                						.arg(GENERATE_SQL_LIKE_STMT(YerothDatabaseTableColumn::DESIGNATION, partSearchTerm),
                							 GENERATE_SQL_LIKE_STMT(YerothDatabaseTableColumn::CATEGORIE, partSearchTerm),
											 GENERATE_SQL_LIKE_STMT(YerothDatabaseTableColumn::DESCRIPTION_PRODUIT, partSearchTerm)));
            }
            else if (YerothUtils::isEqualCaseInsensitive(_allWindows->CLIENTS, _sqlTable->sqlTableName()))
            {
                _searchFilter->append(QString("(%1 OR %2 OR %3 OR %4 OR %5 OR %6 OR %7 OR %8)")
                						.arg(GENERATE_SQL_LIKE_STMT(YerothDatabaseTableColumn::NOM_ENTREPRISE, partSearchTerm),
                							 GENERATE_SQL_LIKE_STMT(YerothDatabaseTableColumn::NOM_REPRESENTANT, partSearchTerm),
											 GENERATE_SQL_LIKE_STMT(YerothDatabaseTableColumn::REFERENCE_CLIENT, partSearchTerm),
                							 GENERATE_SQL_LIKE_STMT(YerothDatabaseTableColumn::DESCRIPTION_CLIENT, partSearchTerm),
                							 GENERATE_SQL_LIKE_STMT(YerothDatabaseTableColumn::QUARTIER, partSearchTerm),
											 GENERATE_SQL_LIKE_STMT(YerothDatabaseTableColumn::VILLE, partSearchTerm),
                							 GENERATE_SQL_LIKE_STMT(YerothDatabaseTableColumn::PROVINCE_ETAT, partSearchTerm),
											 GENERATE_SQL_LIKE_STMT(YerothDatabaseTableColumn::SIEGE_SOCIAL, partSearchTerm)));
            }
            else if (YerothUtils::isEqualCaseInsensitive(_allWindows->MARCHANDISES, _sqlTable->sqlTableName()))
            {
                _searchFilter->append(QString("(%1 OR %2 OR %3 OR %4)")
                						.arg(GENERATE_SQL_LIKE_STMT(YerothDatabaseTableColumn::DESIGNATION, partSearchTerm),
                							 GENERATE_SQL_LIKE_STMT(YerothDatabaseTableColumn::CATEGORIE, partSearchTerm),
											 GENERATE_SQL_LIKE_STMT(YerothDatabaseTableColumn::DESCRIPTION_PRODUIT, partSearchTerm),
											 GENERATE_SQL_LIKE_STMT(YerothDatabaseTableColumn::STOCK_MANAGER, partSearchTerm)));
            }

            if (k != lastIdx)
            {
                _searchFilter->append(" AND ");
            }
        }
    }

    QString categorie(lineEdit_recherche_1->text());

    if (!categorie.isEmpty())
    {
        if (!_searchFilter->isEmpty())
        {
            _searchFilter->append(" AND ");
        }

        if (!YerothUtils::isEqualCaseInsensitive(_allWindows->CLIENTS, _sqlTable->sqlTableName()))
        {
        	_searchFilter->append(GENERATE_SQL_IS_STMT(YerothDatabaseTableColumn::CATEGORIE, categorie));
        }
        else
        {
        	_searchFilter->append(GENERATE_SQL_IS_STMT(YerothDatabaseTableColumn::REFERENCE_CLIENT, categorie));
        }
    }

    QString designation(lineEdit_recherche_2->text());

    if (!designation.isEmpty())
    {
        if (!_searchFilter->isEmpty())
        {
            _searchFilter->append(" AND ");
        }

        if (!YerothUtils::isEqualCaseInsensitive(_allWindows->CLIENTS, _sqlTable->sqlTableName()))
        {
        	_searchFilter->append(GENERATE_SQL_IS_STMT(YerothDatabaseTableColumn::DESIGNATION, designation));
        }
        else
        {
        	_searchFilter->append(GENERATE_SQL_IS_STMT(YerothDatabaseTableColumn::NOM_ENTREPRISE, designation));
        }
    }

    if (YerothUtils::isEqualCaseInsensitive(_allWindows->STOCKS, _sqlTable->sqlTableName()) ||
    	YerothUtils::isEqualCaseInsensitive(_allWindows->ACHATS, _sqlTable->sqlTableName())   )
    {
        QString fournisseur(lineEdit_recherche_3->text());

        if (!fournisseur.isEmpty())
        {
            if (!_searchFilter->isEmpty())
            {
                _searchFilter->append(" AND ");
            }
            _searchFilter->append(GENERATE_SQL_IS_STMT(YerothDatabaseTableColumn::NOM_ENTREPRISE_FOURNISSEUR, fournisseur));
        }
    }
    else if (YerothUtils::isEqualCaseInsensitive(_allWindows->CLIENTS, _sqlTable->sqlTableName()))
    {
        QString nom_representant(lineEdit_recherche_3->text());
        if (!nom_representant.isEmpty())
        {
            if (!_searchFilter->isEmpty())
            {
                _searchFilter->append(" AND ");
            }
            _searchFilter->append(GENERATE_SQL_IS_STMT(YerothDatabaseTableColumn::NOM_REPRESENTANT, nom_representant));
        }
    }
    else if (YerothUtils::isEqualCaseInsensitive(_allWindows->MARCHANDISES, _sqlTable->sqlTableName()))
    {
        QString nom_recepteur(lineEdit_recherche_3->text());
        if (!nom_recepteur.isEmpty())
        {
            if (!_searchFilter->isEmpty())
            {
                _searchFilter->append(" AND ");
            }
            _searchFilter->append(GENERATE_SQL_IS_STMT(YerothDatabaseTableColumn::STOCK_MANAGER, nom_recepteur));
        }
    }

    _sqlTable->yerothSetFilter(*_searchFilter);

    _logger->log("performSearch", QString("search filter: %1").arg(*_searchFilter));

    if (_sqlTable->select())
    {
        if (YerothUtils::isEqualCaseInsensitive(_allWindows->ACHATS, _sqlTable->sqlTableName()))
        {
            _allWindows->_achatsWindow->setLastListerSelectedRow(0);
            _allWindows->_achatsWindow->setSearchFormSqlTableModel(_sqlTable);
            _allWindows->_achatsWindow->afficherAchats(*_sqlTable);
        }
        else if (YerothUtils::isEqualCaseInsensitive(_allWindows->CLIENTS, _sqlTable->sqlTableName()))
        {
            _allWindows->_clientWindow->setLastListerSelectedRow(0);
            _allWindows->_clientWindow->setSearchFormSqlTableModel(_sqlTable);
            _allWindows->_clientWindow->afficherClients(*_sqlTable);
        }
        else if (YerothUtils::isEqualCaseInsensitive(_allWindows->MARCHANDISES, _sqlTable->sqlTableName()))
        {
            _allWindows->_marchandisesWindow->setLastListerSelectedRow(0);
            _allWindows->_marchandisesWindow->setSearchFormSqlTableModel(_sqlTable);
            _allWindows->_marchandisesWindow->afficherMarchandises();
        }
    }
    else
    {
        _logger->log("performSearch", QString("reason for failing: %1").arg(_sqlTable->lastError().text()));
    }

    rendreInvisible();
}
