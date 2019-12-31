/*
 * yeroth-erp-Entrer un stock-window.cpp
 *
 *      Author: Xavier NOUMBISSI NOUNDOU, Dipl.-Inf., Ph.D. (ABD)
 */

#include "yeroth-erp-entrer-window.hpp"

#include "src/yeroth-erp-windows.hpp"

#include "src/utils/yeroth-erp-database-table-column.hpp"

#include "src/users/yeroth-erp-users.hpp"

#include "src/utils/yeroth-erp-config.hpp"

#include <QtSql/QSqlQuery>

#include <QtSql/QSqlIndex>

#include "src/utils/yeroth-erp-database-table-column.hpp"

#include "src/utils/yeroth-erp-historique-stock.hpp"

const unsigned int YerothEntrerWindow::MAX_STOCKS(7);

const unsigned int YerothEntrerWindow::INDEX_ZERO(0);

const QString YerothEntrerWindow::_WINDOW_TITLE(QString(QObject::trUtf8("%1 - %2")).
        arg(YEROTH_ERP_WINDOW_TITLE, QObject::trUtf8("entrer un stock")));


YerothEntrerWindow::YerothEntrerWindow()
:YerothWindowsCommons(YerothEntrerWindow::_WINDOW_TITLE),
 _logger(new YerothLogger("YerothEntrerWindow")),
 _stocks_id(0),
 _montantTva(0.0),
 _tvaCheckBoxPreviousState(false),
 _createNewCategorie(false),
 _createNewFournisseur(false)
{
    setupUi(this);

    this->mySetupUi(this);

    QMESSAGE_BOX_STYLE_SHEET = QString("QMessageBox {background-color: rgb(%1);}"
                                       "QMessageBox QLabel {color: rgb(%2);}").
                               arg(COLOUR_RGB_STRING_YEROTH_GRAY_78_78_78,
                            	   COLOUR_RGB_STRING_YEROTH_WHITE_255_255_255);

    setupLineEdits();

    setupLineEditsQCompleters();

    setupDateTimeEdits();

	YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMarchandises, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionStocks, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSortir, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVendre, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAnnuler, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEnregisterStock, false);

    pushButton_afficher_stocks->disable(this);
    pushButton_menu_principal->disable(this);
    pushButton_sortir->disable(this);
    pushButton_annuler->disable(this);

    /** Menu actions */
    connect(actionChanger_utilisateur, SIGNAL(triggered()), this, SLOT(changer_utilisateur()));
    connect(actionAppeler_aide, SIGNAL(triggered()), this, SLOT(help()));
    connect(actionDeconnecter_utilisateur, SIGNAL(triggered()), this, SLOT(deconnecter_utilisateur()));
    connect(actionMenu_Principal, SIGNAL(triggered()), this, SLOT(menu()));
    connect(actionMarchandises, SIGNAL(triggered()), this, SLOT(afficherMarchandises()));
    connect(actionStocks, SIGNAL(triggered()), this, SLOT(afficherStocks()));
    connect(actionSortir, SIGNAL(triggered()), this, SLOT(sortir()));
    connect(actionAnnuler, SIGNAL(triggered()), this, SLOT(menu()));
    connect(actionEnregisterStock, SIGNAL(triggered()), this, SLOT(enregistrer_produit()));
    connect(actionVendre, SIGNAL(triggered()), this, SLOT(vendre()));
    connect(actionFermeture, SIGNAL(triggered()), this, SLOT(fermeture()));
    connect(actionA_propos, SIGNAL(triggered()), this, SLOT(apropos()));
    connect(actionAlertes, SIGNAL(triggered()), this, SLOT(alertes()));
    connect(actionInformationEntreprise, SIGNAL(triggered()), this, SLOT(infosEntreprise()));
    connect(actionQui_suis_je, SIGNAL(triggered()), this, SLOT(qui_suis_je()));
    connect(actionAdministration, SIGNAL(triggered()), this, SLOT(administration()));

#ifdef YEROTH_CLIENT
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    connect(lineEdit_quantite_par_lot, SIGNAL(textChanged(const QString &)), this,
            SLOT(display_quantite_total(const QString &)));

    connect(spinBox_lots_entrant, SIGNAL(valueChanged(int)), this, SLOT(display_quantite_total_by_spinbox(int)));

    connect(checkBox_tva, SIGNAL(clicked(bool)), this, SLOT(handleTVACheckBox(bool)));

    connect(lineEdit_prix_vente, SIGNAL(textEdited(const QString &)), this,
            SLOT(edited_prix_vente(const QString &)));

    connect(lineEdit_prix_vente, SIGNAL(editingFinished()), this, SLOT(display_prix_vente()));

    connect(lineEdit_categorie_produit,
    		SIGNAL(textChanged(const QString &)),
			this,
            SLOT(handleCategorieName(const QString &)));

    connect(lineEdit_reference_produit,
    		SIGNAL(editingFinished()),
			this,
            SLOT(product_search_with_codebar()));

    connect(lineEdit_nom_entreprise_fournisseur,
    		SIGNAL(textChanged(const QString &)),
			this,
            SLOT(handleFournisseurName(const QString &)));

    setupShortcuts();
}

void YerothEntrerWindow::deconnecter_utilisateur()
{
    this->clear_all_fields();
    _allWindows->definirPasDeRole();
    _allWindows->_mainWindow->show();
    this->rendreInvisible();
}

void YerothEntrerWindow::setupLineEdits()
{
    lineEdit_quantite_total->setEnabled(false);
    lineEdit_tva->setEnabled(false);
    lineEdit_tva->setText(YerothUtils::getTvaStringWithPercent());
    lineEdit_quantite_total->setValidator(&YerothUtils::DoubleValidator);
    lineEdit_quantite_par_lot->setValidator(&YerothUtils::IntValidator);
    lineEdit_stock_minimum->setValidator(&YerothUtils::DoubleValidator);
    lineEdit_prix_dachat->setValidator(&YerothUtils::DoubleValidator);
    lineEdit_prix_vente->setValidator(&YerothUtils::DoubleValidator);
    lineEdit_reference_produit->setFocus();
}

void YerothEntrerWindow::setupLineEditsQCompleters()
{
    YerothPOSUser *user = _allWindows->getUser();

    lineEdit_reference_produit->setupMyStaticQCompleter(_allWindows->MARCHANDISES,
    												   YerothDatabaseTableColumn::REFERENCE,
													   false,
													   false);

    if (user && user->isManager())
    {
        lineEdit_categorie_produit->setupMyStaticQCompleter(_allWindows->CATEGORIES, YerothDatabaseTableColumn::NOM_CATEGORIE, true);
        lineEdit_nom_entreprise_fournisseur->setupMyStaticQCompleter(_allWindows->FOURNISSEURS, YerothDatabaseTableColumn::NOM_ENTREPRISE,
                true);
    }
    else
    {
        lineEdit_categorie_produit->setupMyStaticQCompleter(_allWindows->CATEGORIES, YerothDatabaseTableColumn::NOM_CATEGORIE);
        lineEdit_nom_entreprise_fournisseur->setupMyStaticQCompleter(_allWindows->FOURNISSEURS, YerothDatabaseTableColumn::NOM_ENTREPRISE);
    }
}


void YerothEntrerWindow::setupShortcuts()
{
    this->setupShortcutActionMessageDaide 	(*actionAppeler_aide);
    this->setupShortcutActionQuiSuisJe		(*actionQui_suis_je);
}


bool YerothEntrerWindow::creerNouveauFournisseur(const QString proposedFournisseurName)
{
	YerothSqlTableModel &fournisseurSqlTableModel = _allWindows->getSqlTableModel_fournisseurs();

	QString fournisseurFilter = QString("nom_entreprise = '%1'")
	            					.arg(proposedFournisseurName);

	fournisseurSqlTableModel.yerothSetFilter(fournisseurFilter);

	int rows = fournisseurSqlTableModel.easySelect();

	if (rows > 0)
	{
		fournisseurSqlTableModel.resetFilter();
		return true;
	}
	else
	{
		fournisseurSqlTableModel.resetFilter();

		QSqlRecord record = fournisseurSqlTableModel.record();

		record.setValue(YerothDatabaseTableColumn::ID, _allWindows->getNextIdSqlTableModel_fournisseurs());
		record.setValue(YerothDatabaseTableColumn::NOM_ENTREPRISE, proposedFournisseurName);

		QString retMsg(QString(QObject::trUtf8("L'entreprise fournisseur '%1"))
							.arg(proposedFournisseurName));

		bool success = fournisseurSqlTableModel.insertNewRecord(record);

		if (!success)
		{
			retMsg.append(QObject::trUtf8("' n'a pas pu être créer !"));

			YerothQMessageBox::warning(this, QObject::trUtf8("échec"), retMsg);

			return false;
		}
		else
		{
			retMsg.append(QObject::trUtf8("' a été créer avec succès !"));

			YerothQMessageBox::information(this, QObject::trUtf8("succès"), retMsg);

			return true;
		}
	}

	return false;
}


bool YerothEntrerWindow::creerNouvelleCategorie(const QString proposedCategorieName)
{
    YerothSqlTableModel &categorieSqlTableModel = _allWindows->getSqlTableModel_categories();

    QString categorieFilter = QString("nom_categorie = '%1'")
    							.arg(proposedCategorieName);

    categorieSqlTableModel.yerothSetFilter(categorieFilter);

    int rows = categorieSqlTableModel.easySelect();

    if (rows > 0)
    {
    	categorieSqlTableModel.resetFilter();
    	return true;
    }
    else
    {
    	categorieSqlTableModel.resetFilter();

    	QSqlRecord record = categorieSqlTableModel.record();

    	record.setValue(YerothDatabaseTableColumn::ID, _allWindows->getNextIdSqlTableModel_categories());
    	record.setValue(YerothDatabaseTableColumn::NOM_CATEGORIE, proposedCategorieName);
    	record.setValue(YerothDatabaseTableColumn::DESCRIPTION_CATEGORIE, "");

    	QString retMsg(QString(QObject::trUtf8("La catégorie '%1"))
    						.arg(proposedCategorieName));

    	bool success = categorieSqlTableModel.insertNewRecord(record);

    	if (!success)
    	{
    		retMsg.append(QObject::trUtf8("' n'a pas pu être créer !"));

    		YerothQMessageBox::warning(this, QObject::trUtf8("échec"), retMsg);

    		return false;
    	}
    	else
    	{
    		retMsg.append(QObject::trUtf8("' a été créer avec succès !"));

    		YerothQMessageBox::information(this, QObject::trUtf8("succès"), retMsg);

    		return true;
    	}
    }

    return false;
}

void YerothEntrerWindow::selectionner_image_produit()
{
    YerothUtils::selectionner_image(this, *label_image_produit);
}

void YerothEntrerWindow::definirPasDeRole()
{
    _logger->log("definirPasDeRole");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionStocks, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSortir, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVendre, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, false);    ;
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEnregisterStock, false);

    pushButton_marchandises->disable(this);
    pushButton_afficher_stocks->disable(this);
    pushButton_sortir->disable(this);
    pushButton_annuler->disable(this);
    pushButton_selectionner_image->setEnabled(false);
    pushButton_enregistrer_produit->setEnabled(false);
}

void YerothEntrerWindow::definirCaissier()
{
    _logger->log("definirCaissier");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMarchandises, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionStocks, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSortir, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVendre, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAnnuler, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEnregisterStock, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);

    pushButton_marchandises->disable(this);
    pushButton_afficher_stocks->disable(this);
    pushButton_menu_principal->disable(this);
    pushButton_sortir->disable(this);
    pushButton_annuler->disable(this);
}

void YerothEntrerWindow::definirManager()
{
    _logger->log("definirManager");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMarchandises, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionStocks, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSortir, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVendre, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);

#ifdef YEROTH_CLIENT
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAnnuler, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEnregisterStock, true);

    pushButton_marchandises->enable(this, SLOT(afficherMarchandises()));
    pushButton_afficher_stocks->enable(this, SLOT(afficherStocks()));
    pushButton_menu_principal->enable(this, SLOT(menu()));
    pushButton_sortir->enable(this, SLOT(sortir()));
    pushButton_annuler->enable(this, SLOT(menu()));
    pushButton_selectionner_image->enable(this, SLOT(selectionner_image_produit()));
    pushButton_enregistrer_produit->enable(this, SLOT(enregistrer_produit()));
}


void YerothEntrerWindow::definirVendeur()
{
    _logger->log("definirVendeur");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMarchandises, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionStocks, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSortir, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVendre, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, false);    ;
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEnregisterStock, false);

    pushButton_marchandises->disable(this);
    pushButton_afficher_stocks->disable(this);
    pushButton_sortir->disable(this);
    pushButton_annuler->disable(this);
    pushButton_selectionner_image->setEnabled(false);
    pushButton_enregistrer_produit->setEnabled(false);
}


void YerothEntrerWindow::definirGestionaireDesStocks()
{
    _logger->log("definirGestionaireDesStocks");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMarchandises, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionStocks, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSortir, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVendre, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);

#ifdef YEROTH_CLIENT
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAnnuler, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEnregisterStock, true);

    pushButton_marchandises->enable(this, SLOT(afficherMarchandises()));
    pushButton_afficher_stocks->enable(this, SLOT(afficherStocks()));
    pushButton_menu_principal->enable(this, SLOT(menu()));
    pushButton_sortir->enable(this, SLOT(sortir()));
    pushButton_annuler->enable(this, SLOT(menu()));
    pushButton_selectionner_image->enable(this, SLOT(selectionner_image_produit()));
    pushButton_enregistrer_produit->enable(this, SLOT(enregistrer_produit()));
}


void YerothEntrerWindow::definirMagasinier()
{
    _logger->log("definirMagasinier");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMarchandises, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionStocks, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSortir, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVendre, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAnnuler, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEnregisterStock, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);

    pushButton_marchandises->enable(this, SLOT(afficherMarchandises()));
    pushButton_afficher_stocks->enable(this, SLOT(afficherStocks()));
    pushButton_menu_principal->enable(this, SLOT(menu()));
    pushButton_sortir->enable(this, SLOT(sortir()));
    pushButton_annuler->enable(this, SLOT(menu()));
    pushButton_selectionner_image->enable(this, SLOT(selectionner_image_produit()));
    pushButton_enregistrer_produit->disable(this);
}


bool YerothEntrerWindow::product_search_with_codebar()
{
    YerothSqlTableModel & productListSqlTableModel = _allWindows->getSqlTableModel_marchandises();

    QString aCodebar(lineEdit_reference_produit->text());

    int codebarRowCount =
        productListSqlTableModel.Is_SearchQSqlTable(YerothDatabaseTableColumn::REFERENCE, aCodebar);

    if (!aCodebar.isEmpty() && 1 == codebarRowCount)
    {
        QString strQuery(QString("select * from %1 where %2 = '%3';")
        					.arg(productListSqlTableModel.sqlTableName(),
								 YerothDatabaseTableColumn::REFERENCE,
								 aCodebar));

        QSqlQuery query;
        query.prepare(strQuery);

        bool success = query.exec();

        if (success && query.last())
        {
        	lineEdit_designation->setText(query.value(YerothDatabaseTableColumn::DESIGNATION).toString());
        	lineEdit_categorie_produit->setText(query.value(YerothDatabaseTableColumn::CATEGORIE).toString());

            return true;
        }
    }

	return false;
}


void YerothEntrerWindow::display_quantite_total(const QString & quantite_par_lot)
{
    double qte_lot = quantite_par_lot.toDouble();
    double qte_total = spinBox_lots_entrant->valueMultiplyBy(qte_lot);
    lineEdit_quantite_total->setText(QString::number(qte_total, 'f', 0));
}

void YerothEntrerWindow::display_quantite_total_by_spinbox(int lots)
{
    double qte_lot = lineEdit_quantite_par_lot->text().toDouble();
    double qte_total = lots * qte_lot;
    lineEdit_quantite_total->setText(QString::number(qte_total, 'f', 0));
}

void YerothEntrerWindow::display_prix_vente()
{
    if (_lastEditedPrixVente != lineEdit_prix_vente->text())
    {
        return;
    }
    if (checkBox_tva->isChecked())
    {
        double prix_vente = lineEdit_prix_vente->text().toDouble();
        _montantTva = prix_vente * YerothERPConfig::tva_value;
        prix_vente = prix_vente + _montantTva;
        lineEdit_prix_vente->setText(QString::number(prix_vente, 'f', 2));
    }
}

void YerothEntrerWindow::handleTVACheckBox(bool clicked)
{
    double prix_vente = lineEdit_prix_vente->text().toDouble();
    if (clicked && checkBox_tva->isChecked())
    {
        if (false == _tvaCheckBoxPreviousState)
        {
            _tvaCheckBoxPreviousState = true;
        }
        else
        {
            _tvaCheckBoxPreviousState = false;
        }
        _montantTva = prix_vente * YerothERPConfig::tva_value;
        prix_vente = prix_vente + _montantTva;
    }
    else
    {
        _montantTva = 0;
        if (true == _tvaCheckBoxPreviousState)
        {
            _tvaCheckBoxPreviousState = false;
            prix_vente = prix_vente / (1 + YerothERPConfig::tva_value);
        }
    }
    lineEdit_prix_vente->setText(QString::number(prix_vente, 'f', 2));
}





void YerothEntrerWindow::handleCategorieName(const QString &text)
{
    //_logger->log("handleCategorieName(QString &)");

	//qDebug() << QString("handleCategorieName()");

    QString proposedCategorieName = lineEdit_categorie_produit->text();

    if (proposedCategorieName.isEmpty())
    {
    	return ;
    }

    _createNewCategorie = true;

    _createNewFournisseur = false;

    if (YerothUtils::isEqualCaseInsensitive(proposedCategorieName,
        								   YerothUtils::NOUVELLE_CATEGORIE))
    {
    	_allWindows->_creerNouvelleCategorieWindow->rendreVisible(_curStocksTableModel);

    	this->rendreInvisibleAvecConservation();
    }

    return ;
}


void YerothEntrerWindow::handleFournisseurName(const QString &)
{
    //_logger->log("handleFournisseurName(const QString &)");

	//qDebug() << QString("handleFournisseurName()");

	QString proposedFournisseurName = lineEdit_nom_entreprise_fournisseur->text();

	if (proposedFournisseurName.isEmpty())
	{
		return ;
	}

    _createNewFournisseur = true;

    _createNewCategorie = false;

    if (YerothUtils::isEqualCaseInsensitive(proposedFournisseurName,
        								   YerothUtils::NOUVEAU_FOURNISSEUR))
    {
    	_allWindows->_creerNouveauFournisseurWindow->rendreVisible(_curStocksTableModel);

    	rendreInvisibleAvecConservation();
    }
}


bool YerothEntrerWindow::check_fields()
{
    bool result;

    bool designation = lineEdit_designation->checkField();

    bool quantite = lineEdit_quantite_par_lot->checkField();

    bool prix_vente = lineEdit_prix_vente->checkField();

    bool categorie_produit = lineEdit_categorie_produit->checkField();

    result = designation && categorie_produit && prix_vente && quantite;

    if (!result)
    {
        if (QMessageBox::Ok ==
            	YerothQMessageBox::warning(this, "enregistrer",
                                           tr("Remplisser tous les champs obligatoires !")))
        {
        }
        else
        {
        }
    }

    return result;
}


void YerothEntrerWindow::clear_all_fields()
{
    lineEdit_reference_produit->clearField();
    lineEdit_designation->clearField();
    lineEdit_categorie_produit->clearField();
    lineEdit_nom_entreprise_fournisseur->clearField();
    spinBox_lots_entrant->setValue(1);
    lineEdit_quantite_par_lot->clearField();
    lineEdit_quantite_total->clear();
    lineEdit_nom_entreprise_fournisseur->clearField();
    lineEdit_stock_minimum->clearField();
    lineEdit_reference_recu_dachat->clearField();
    lineEdit_prix_dachat->clearField();
    lineEdit_prix_vente->clearField();
    lineEdit_localisation_produit->clearField();
    textEdit_description->clear();
    label_image_produit->clear();
    label_image_produit->setAutoFillBackground(false);
    _lastEditedPrixVente.clear();
    _montantTva = 0.0;
    checkBox_tva->setChecked(false);
    _tvaCheckBoxPreviousState = false;
    _createNewCategorie = false;
    _createNewFournisseur = false;
}

void YerothEntrerWindow::rendreInvisible()
{
    this->clear_all_fields();
    dateEdit_date_peremption->reset();
    this->setVisible(false);
}

void YerothEntrerWindow::rendreVisible(YerothSqlTableModel * stocksTableModel, bool aShowItem)
{
    _curStocksTableModel = stocksTableModel;
    if (_createNewCategorie)
    {
        if (!_curCategorieName.isEmpty())
        {
            lineEdit_categorie_produit->setText(_curCategorieName);
            _curCategorieName.clear();
        }
        else
        {
            lineEdit_categorie_produit->clear();
        }
    }
    if (_createNewFournisseur)
    {
        if (!_curFournisseurName.isEmpty())
        {
            lineEdit_nom_entreprise_fournisseur->setText(_curFournisseurName);
            _curFournisseurName.clear();
        }
        else
        {
            lineEdit_nom_entreprise_fournisseur->clear();
        }
    }
    this->setupLineEditsQCompleters();
    lineEdit_tva->setText(YerothUtils::getTvaStringWithPercent());
    if (aShowItem)
    {
        this->clear_all_fields();
        this->showItem();
        lineEdit_reference_produit->clearFocus();
    }
    else
    {
        lineEdit_reference_produit->setFocus();
    }

    this->setVisible(true);
}


bool YerothEntrerWindow::insertStockItemInProductList()
{
    bool success = false;

    YerothSqlTableModel & productListSqlTableModel = _allWindows->getSqlTableModel_marchandises();

    QSqlRecord record = productListSqlTableModel.record();

    record.setValue(YerothDatabaseTableColumn::ID,
                    YerothUtils::getNextIdFromTable(_allWindows->MARCHANDISES));

    record.setValue(YerothDatabaseTableColumn::REFERENCE, lineEdit_reference_produit->text());

    record.setValue(YerothDatabaseTableColumn::DESIGNATION, lineEdit_designation->text());

    record.setValue(YerothDatabaseTableColumn::CATEGORIE, lineEdit_categorie_produit->text());

    record.setValue(YerothDatabaseTableColumn::DESCRIPTION_PRODUIT, textEdit_description->toPlainText());

    record.setValue(YerothDatabaseTableColumn::MONTANT_TVA, _montantTva);

    double prix_vente = lineEdit_prix_vente->text().toDouble();

    record.setValue(YerothDatabaseTableColumn::PRIX_VENTE, prix_vente);

    double prix_unitaire_ht = prix_vente - _montantTva;

    record.setValue(YerothDatabaseTableColumn::PRIX_UNITAIRE, prix_unitaire_ht);
    //qDebug() << "++_tva: " << QString::number(_tva, 'f', 2);
    if (label_image_produit->pixmap())
    {
        QByteArray bytes;
        YerothUtils::savePixmapToByteArray(bytes, *label_image_produit->pixmap(), "JPG");

        record.setValue(YerothDatabaseTableColumn::IMAGE_PRODUIT, bytes);
    }

    success = productListSqlTableModel.insertNewRecord(record);


    QString retMsg(QString(QObject::tr("Le stock '%1'"))
    					.arg(lineEdit_designation->text()));

    if (success)
    {
        retMsg.append(QObject::trUtf8(" a été enregistré dans la liste des articles !"));

        YerothQMessageBox::information(this,
        							   QObject::trUtf8("enregistrement du stock dans la liste des marchandises"),
        							   retMsg);
    }
    else
    {
        retMsg.append(QObject::trUtf8(" n'a pas pu être enregistré dans la liste des produits !"));

        YerothQMessageBox::warning(this,
        						   QObject::trUtf8("échec de l'enregistrement du stock dans la liste des marchandises"),
        						   retMsg);
    }

    return success;
}

bool YerothEntrerWindow::isStockItemInProductList()
{
    YerothSqlTableModel & productListSqlTableModel = _allWindows->getSqlTableModel_marchandises();

    int designationRowCount =
        productListSqlTableModel.Is_SearchQSqlTable(YerothDatabaseTableColumn::DESIGNATION,
                lineEdit_designation->text());

    int categorieRowCount =
        productListSqlTableModel.Is_SearchQSqlTable(YerothDatabaseTableColumn::CATEGORIE,
                lineEdit_categorie_produit->text());

    return (designationRowCount > 0) && (categorieRowCount > 0);
}


bool YerothEntrerWindow::isProfitable()
{
	double prix_dachat = lineEdit_prix_dachat->text().toDouble();
	double prix_vente = lineEdit_prix_vente->text().toDouble();

	double profit = getMargeBeneficiaire(prix_vente,
										 prix_dachat,
										 _montantTva);

//	qDebug() << QString("++ prix_vente: %1, prix_dachat: %2, _montantTva: %3, profit: %4")
//					.arg(QString::number(prix_vente),
//						 QString::number(prix_dachat),
//						 QString::number(_montantTva),
//						 QString::number(profit));

	return (profit >= 0);
}


void YerothEntrerWindow::enregistrer_produit()
{
    _logger->log("enregistrer_produit");

    if (check_fields())
    {
        if (!isProfitable())
        {
            QString warnMsg(QObject::trUtf8("Le prix de vente doit être supérieure ou égal au prix d'achat !"));

            if (QMessageBox::Ok ==
                    YerothQMessageBox::warning(this, QObject::tr("pas profitable"), warnMsg))
            {
            }
            else
            {
            }
        	return ;
        }

        if (dateEdit_date_peremption->date() <= QDate::currentDate())
        {
            QString warnMsg(QObject::trUtf8("La date de péremption n'est pas postdatée !\n\n"
                            				"Continuer avec l'enregistrement des données de l'article ?"));

            if (QMessageBox::Ok ==
                    YerothQMessageBox::question(this,
                    							QObject::tr("enregistrer un stock"),
                                                warnMsg,
												QMessageBox::Cancel,
											    QMessageBox::Ok))
            {
            }
            else
            {
               return ;
            }
        }

YEROTH_ERP_3_0_START_DATABASE_TRANSACTION;

        if (!isStockItemInProductList())
        {
        	insertStockItemInProductList();
        }

    	QString proposedFournisseurName = lineEdit_nom_entreprise_fournisseur->text();

    	if (!proposedFournisseurName.isEmpty())
    	{
    		if (!creerNouveauFournisseur(proposedFournisseurName))
    		{
    			return ;
    		}
    	}

        QString proposedCategorieName = lineEdit_categorie_produit->text();

        if (!creerNouvelleCategorie(proposedCategorieName))
        {
        	return ;
        }

        YerothSqlTableModel & achatSqlTableModel = _allWindows->getSqlTableModel_achats();

        QSqlRecord achatRecord = achatSqlTableModel.record();

        QSqlRecord record = _curStocksTableModel->record();

        int achat_id_to_save = YerothUtils::getNextIdFromTable(_allWindows->ACHATS);

        int stock_id_to_save = YerothUtils::getNextIdFromTable(_allWindows->STOCKS);

        achatRecord.setValue(YerothDatabaseTableColumn::ID, achat_id_to_save);
        achatRecord.setValue(YerothDatabaseTableColumn::STOCKS_ID, stock_id_to_save);
        achatRecord.setValue(YerothDatabaseTableColumn::REFERENCE, lineEdit_reference_produit->text());
        achatRecord.setValue(YerothDatabaseTableColumn::DESIGNATION, lineEdit_designation->text());
        achatRecord.setValue(YerothDatabaseTableColumn::CATEGORIE, proposedCategorieName);
        achatRecord.setValue(YerothDatabaseTableColumn::DESCRIPTION_PRODUIT, textEdit_description->toPlainText());
        achatRecord.setValue(YerothDatabaseTableColumn::LOTS_ENTRANT, spinBox_lots_entrant->value());
        achatRecord.setValue(YerothDatabaseTableColumn::QUANTITE_PAR_LOT, lineEdit_quantite_par_lot->text().toDouble());

        record.setValue(YerothDatabaseTableColumn::ID, stock_id_to_save);
        record.setValue(YerothDatabaseTableColumn::REFERENCE, lineEdit_reference_produit->text());
        record.setValue(YerothDatabaseTableColumn::DESIGNATION, lineEdit_designation->text());
        record.setValue(YerothDatabaseTableColumn::CATEGORIE, proposedCategorieName);
        record.setValue(YerothDatabaseTableColumn::DESCRIPTION_PRODUIT, textEdit_description->toPlainText());
        record.setValue(YerothDatabaseTableColumn::LOTS_ENTRANT, spinBox_lots_entrant->value());
        record.setValue(YerothDatabaseTableColumn::QUANTITE_PAR_LOT, lineEdit_quantite_par_lot->text().toDouble());

        double quantite_total = lineEdit_quantite_total->text().toDouble();

        double stock_minimum = lineEdit_stock_minimum->text().toDouble();
        QString reference_recu_dachat = lineEdit_reference_recu_dachat->text();

        double prix_vente = lineEdit_prix_vente->text().toDouble();

        double prix_dachat = lineEdit_prix_dachat->text().toDouble();

        if (prix_dachat < 0)
        {
        	prix_dachat = 0;
        }

        QString utilisateurCourrantNomComplet;

        YerothPOSUser *aUser = _allWindows->getUser();

        if (0 != aUser)
        {
        	utilisateurCourrantNomComplet.append(aUser->nom_complet());
        }

        achatRecord.setValue(YerothDatabaseTableColumn::ENREGISTREUR_STOCK, utilisateurCourrantNomComplet);
        achatRecord.setValue(YerothDatabaseTableColumn::QUANTITE_TOTAL, quantite_total);
        achatRecord.setValue(YerothDatabaseTableColumn::STOCK_MINIMUM, stock_minimum);
        achatRecord.setValue(YerothDatabaseTableColumn::REFERENCE_RECU_DACHAT, reference_recu_dachat);
        achatRecord.setValue(YerothDatabaseTableColumn::PRIX_DACHAT, prix_dachat);
        achatRecord.setValue(YerothDatabaseTableColumn::PRIX_VENTE, prix_vente);
        //qDebug() << "++_tva: " << QString::number(_tva, 'f', 2);
        achatRecord.setValue(YerothDatabaseTableColumn::MONTANT_TVA, _montantTva);



        record.setValue(YerothDatabaseTableColumn::ENREGISTREUR_STOCK, utilisateurCourrantNomComplet);
        record.setValue(YerothDatabaseTableColumn::QUANTITE_TOTAL, quantite_total);
        record.setValue(YerothDatabaseTableColumn::STOCK_MINIMUM, stock_minimum);
        record.setValue(YerothDatabaseTableColumn::REFERENCE_RECU_DACHAT, reference_recu_dachat);
        record.setValue(YerothDatabaseTableColumn::PRIX_DACHAT, prix_dachat);
        record.setValue(YerothDatabaseTableColumn::PRIX_VENTE, prix_vente);
        //qDebug() << "++_tva: " << QString::number(_tva, 'f', 2);
        record.setValue(YerothDatabaseTableColumn::MONTANT_TVA, _montantTva);

        double prix_unitaire_ht = prix_vente - _montantTva;

        double marge_beneficiaire = getMargeBeneficiaire(prix_vente, prix_dachat, _montantTva);

        achatRecord.setValue(YerothDatabaseTableColumn::MARGE_BENEFICIAIRE, marge_beneficiaire);
        achatRecord.setValue(YerothDatabaseTableColumn::PRIX_UNITAIRE, prix_unitaire_ht);
        achatRecord.setValue(YerothDatabaseTableColumn::NOM_ENTREPRISE_FOURNISSEUR, proposedFournisseurName);
        achatRecord.setValue(YerothDatabaseTableColumn::LOCALISATION, _allWindows->getInfoEntreprise().getLocalisation());
        achatRecord.setValue(YerothDatabaseTableColumn::LOCALISATION_STOCK, lineEdit_localisation_produit->text());
        achatRecord.setValue(YerothDatabaseTableColumn::DATE_ENTREE, GET_CURRENT_DATE);
        achatRecord.setValue(YerothDatabaseTableColumn::DATE_PEREMPTION, dateEdit_date_peremption->date());

        record.setValue(YerothDatabaseTableColumn::PRIX_UNITAIRE, prix_unitaire_ht);
        record.setValue(YerothDatabaseTableColumn::NOM_ENTREPRISE_FOURNISSEUR, proposedFournisseurName);
        record.setValue(YerothDatabaseTableColumn::LOCALISATION, _allWindows->getInfoEntreprise().getLocalisation());
        record.setValue(YerothDatabaseTableColumn::LOCALISATION_STOCK, lineEdit_localisation_produit->text());
        record.setValue(YerothDatabaseTableColumn::DATE_ENTREE, GET_CURRENT_DATE);
        record.setValue(YerothDatabaseTableColumn::DATE_PEREMPTION, dateEdit_date_peremption->date());

        QString historiqueStockInitial(
        		YerothHistoriqueStock::creer_mouvement_stock(ENTREE,
        													stock_id_to_save,
															GET_CURRENT_DATE,
															quantite_total,
															0,
															quantite_total));

        //qDebug() << QString("++ test: %1")
        //				.arg(historiqueStockInitial);

        record.setValue(YerothDatabaseTableColumn::HISTORIQUE_STOCK, historiqueStockInitial);

        if (label_image_produit->pixmap())
        {
            QByteArray bytes;
            YerothUtils::savePixmapToByteArray(bytes, *label_image_produit->pixmap(), "JPG");
            record.setValue(YerothDatabaseTableColumn::IMAGE_PRODUIT, bytes);
        }

        bool achatSuccess = achatSqlTableModel.insertNewRecord(achatRecord);

        bool success = _curStocksTableModel->insertNewRecord(record);

YEROTH_ERP_3_0_COMMIT_DATABASE_TRANSACTION;

        QString achatRetMsg(QString(QObject::tr("L'achat du stock '%1'"))
        						.arg(lineEdit_designation->text()));

        if (achatSuccess)
        {
        	achatRetMsg.append(QObject::trUtf8(" a été enregistré dans la base de données !"));

            YerothQMessageBox::information(this,
            							   QObject::trUtf8("enregistrement du stock avec succès"),
                                           achatRetMsg);
        }
        else
        {
        	achatRetMsg.append(QObject::trUtf8(" n'a pas pu être enregistré dans la base de données !"));

            YerothQMessageBox::warning(this,
            						   QObject::trUtf8("échec de l'enregistrement du stock"),
									   achatRetMsg);
        }

        QString retMsg(QString(QObject::tr("Le stock '%1'"))
        					.arg(lineEdit_designation->text()));

        if (success)
        {
            retMsg.append(QObject::trUtf8(" a été enregistré dans la base de données !"));

            YerothQMessageBox::information(this,
            							   QObject::trUtf8("enregistrement du stock avec succès"),
                                           retMsg);
        }
        else
        {
            retMsg.append(QObject::trUtf8(" n'a pas pu être enregistré dans la base de données !"));

            YerothQMessageBox::warning(this,
            						   QObject::trUtf8("échec de l'enregistrement du stock"),
									   retMsg);
        }

        rendreInvisible();

        _allWindows->_stocksWindow->rendreVisible(_curStocksTableModel);
    }
    else
    {
        //check fields
    }
}


void YerothEntrerWindow::showItem()
{
    QSqlRecord record = _curStocksTableModel->record(_allWindows->getLastSelectedListerRow());

    lineEdit_reference_produit->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::REFERENCE));
    lineEdit_designation->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::DESIGNATION));
    textEdit_description->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::DESCRIPTION_PRODUIT));

    double prix_vente = GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::PRIX_VENTE).toDouble();
    double montant_tva = GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::MONTANT_TVA).toDouble();

    double prix_dachat = GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::PRIX_DACHAT).toDouble();

    lineEdit_prix_dachat->setText(QString::number(prix_dachat));

    double prix_sans_taxes = prix_vente - montant_tva;

    lineEdit_prix_vente->setText(QString::number(prix_sans_taxes));

    if (0 < montant_tva)
    {
        checkBox_tva->setChecked(true);
        handleTVACheckBox(true);
    }
    else
    {
        checkBox_tva->setChecked(false);
        handleTVACheckBox(false);
    }

    lineEdit_prix_vente->setText(QString::number(prix_vente));
    lineEdit_categorie_produit->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::CATEGORIE));
    lineEdit_localisation_produit->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::LOCALISATION_STOCK));

    QVariant img(record.value(YerothDatabaseTableColumn::IMAGE_PRODUIT));

    if (!img.isNull())
    {
        YerothUtils::loadPixmapFromDB(*label_image_produit, img, "JPG");
    }
    else
    {
        label_image_produit->setAutoFillBackground(false);
    }
}
