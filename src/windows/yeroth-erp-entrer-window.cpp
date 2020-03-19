/*
 * yeroth-erp-entrer-window.cpp
 *
 *      Author: Xavier NOUMBISSI NOUNDOU, Dipl.-Inf., Ph.D. (ABD)
 */

#include "yeroth-erp-entrer-window.hpp"

#include "src/yeroth-erp-windows.hpp"

#include "src/utils/yeroth-erp-database-table-column.hpp"

#include "src/users/yeroth-erp-users.hpp"

#include "src/utils/yeroth-erp-config.hpp"

#include "src/utils/yeroth-erp-database-table-column.hpp"

#include "src/utils/yeroth-erp-historique-stock.hpp"


#include <QtCore/QtMath>

#include <QtSql/QSqlQuery>

#include <QtSql/QSqlIndex>



const unsigned int YerothEntrerWindow::MAX_STOCKS(7);

const unsigned int YerothEntrerWindow::INDEX_ZERO(0);

const QString YerothEntrerWindow::_WINDOW_TITLE(QString(QObject::trUtf8("%1 - %2")).
        arg(YEROTH_ERP_WINDOW_TITLE, QObject::trUtf8("entrer un stock (service)")));


YerothEntrerWindow::YerothEntrerWindow()
:YerothWindowsCommons(YerothEntrerWindow::_WINDOW_TITLE),
 _logger(new YerothLogger("YerothEntrerWindow")),
 _currentServiceInfo(0),
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

    checkBox_achat->setChecked(false);

    setupLineEdits();

    setupLineEditsQCompleters();

    setupDateTimeEdits();

	YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionStocks, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSortir, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAnnuler, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEnregisterStock, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerImage, false);

    pushButton_supprimer_limage_du_stock->disable(this);
    pushButton_afficher_stocks->disable(this);
    pushButton_menu_principal->disable(this);
    pushButton_sortir->disable(this);
    pushButton_annuler->disable(this);

    /** Menu actions */
    connect(actionChanger_utilisateur, SIGNAL(triggered()), this, SLOT(changer_utilisateur()));
    connect(actionAppeler_aide, SIGNAL(triggered()), this, SLOT(help()));
    connect(actionDeconnecter_utilisateur, SIGNAL(triggered()), this, SLOT(deconnecter_utilisateur()));
    connect(actionMenu_Principal, SIGNAL(triggered()), this, SLOT(menu()));
    connect(actionStocks, SIGNAL(triggered()), this, SLOT(afficherStocks()));
    connect(actionSortir, SIGNAL(triggered()), this, SLOT(sortir()));
    connect(actionAnnuler, SIGNAL(triggered()), this, SLOT(menu()));
    connect(actionSupprimerImage, SIGNAL(triggered()), this, SLOT(supprimer_image_stock()));
    connect(actionEnregisterStock, SIGNAL(triggered()), this, SLOT(enregistrer_produit()));
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

    connect(doubleSpinBox_lots_entrant, SIGNAL(valueChanged(double)),
    		this, SLOT(display_quantite_total_by_spinbox(double)));

    connect(checkBox_service, SIGNAL(clicked(bool)), this, SLOT(handleServiceCheckBox(bool)));

    connect(checkBox_achat, SIGNAL(stateChanged(int)), this, SLOT(handleAchatCheckBox(int)));

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

	if (checkBox_service->isChecked())
	{
    	QString aConditionStr(YerothUtils::generateSqlIs(YerothDatabaseTableColumn::IS_SERVICE,
    						  YerothUtils::MYSQL_TRUE_LITERAL));

		lineEdit_designation->setupMyStaticQCompleter(_allWindows->MARCHANDISES,
													  YerothDatabaseTableColumn::DESIGNATION,
													  false,
													  true,
													  aConditionStr);


	    label_fournisseur->setText(QObject::tr("client"));

        lineEdit_nom_entreprise_fournisseur->setupMyStaticQCompleter(_allWindows->CLIENTS,
        															 YerothDatabaseTableColumn::NOM_ENTREPRISE,
																	 true);
	}
	else
	{
		lineEdit_designation->clear();

		label_fournisseur->setText(QObject::tr("fournisseur"));

        lineEdit_nom_entreprise_fournisseur->setupMyStaticQCompleter(_allWindows->FOURNISSEURS,
        															 YerothDatabaseTableColumn::NOM_ENTREPRISE,
																	 true);

    	QString aConditionStr(YerothUtils::generateSqlIs(YerothDatabaseTableColumn::IS_SERVICE,
    						  YerothUtils::MYSQL_FALSE_LITERAL));

    	lineEdit_reference_produit->setupMyStaticQCompleter(_allWindows->MARCHANDISES,
    														YerothDatabaseTableColumn::REFERENCE,
    														false,
    														true,
    														aConditionStr);
	}

	lineEdit_categorie_produit->setupMyStaticQCompleter(_allWindows->CATEGORIES,
														YerothDatabaseTableColumn::NOM_CATEGORIE,
														true);
}


void YerothEntrerWindow::setupShortcuts()
{
    this->setupShortcutActionMessageDaide 	(*actionAppeler_aide);
    this->setupShortcutActionQuiSuisJe		(*actionQui_suis_je);
}


bool YerothEntrerWindow::creerNouveauClient(const QString proposedCustomerName)
{
	YerothSqlTableModel &customerSqlTableModel = _allWindows->getSqlTableModel_clients();

	QString customerTableFilter = QString("%1 = '%2'")
	            					.arg(YerothDatabaseTableColumn::NOM_ENTREPRISE,
	            						 proposedCustomerName);

	customerSqlTableModel.yerothSetFilter(customerTableFilter);

	int rows = customerSqlTableModel.easySelect();

	if (rows > 0)
	{
		customerSqlTableModel.resetFilter();
		return true;
	}
	else
	{
		customerSqlTableModel.resetFilter();

		QSqlRecord record = customerSqlTableModel.record();

		record.setValue(YerothDatabaseTableColumn::ID, _allWindows->getNextIdSqlTableModel_clients());
		record.setValue(YerothDatabaseTableColumn::NOM_ENTREPRISE, proposedCustomerName);
		record.setValue(YerothDatabaseTableColumn::DETTE_MAXIMALE_COMPTE_CLIENT, 0.0);
		record.setValue(YerothDatabaseTableColumn::COMPTE_CLIENT, 0.0);

		QString retMsg(QString(QObject::trUtf8("L'entreprise cliente '%1"))
							.arg(proposedCustomerName));

		bool success = customerSqlTableModel.insertNewRecord(record);

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


bool YerothEntrerWindow::creerNouveauFournisseur(const QString proposedFournisseurName)
{
	YerothSqlTableModel &fournisseurSqlTableModel = _allWindows->getSqlTableModel_fournisseurs();

	QString fournisseurFilter = QString("%1 = '%2'")
	            					.arg(YerothDatabaseTableColumn::NOM_ENTREPRISE,
	            						 proposedFournisseurName);

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

    QString categorieFilter = QString("%1 = '%2'")
    							.arg(YerothDatabaseTableColumn::NOM_CATEGORIE,
    								 proposedCategorieName);

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


void YerothEntrerWindow::supprimer_image_stock()
{
    label_image_produit->clear();
    label_image_produit->setAutoFillBackground(false);
}


void YerothEntrerWindow::definirPasDeRole()
{
    _logger->log("definirPasDeRole");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionStocks, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSortir, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, false);    ;
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEnregisterStock, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerImage, false);

    pushButton_supprimer_limage_du_stock->disable(this);
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
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionStocks, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSortir, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAnnuler, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEnregisterStock, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerImage, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);

    pushButton_supprimer_limage_du_stock->disable(this);
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
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionStocks, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSortir, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);

#ifdef YEROTH_CLIENT
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAnnuler, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEnregisterStock, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerImage, true);

    pushButton_supprimer_limage_du_stock->enable(this, SLOT(supprimer_image_stock()));
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
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionStocks, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSortir, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, false);    ;
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEnregisterStock, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerImage, false);

    pushButton_supprimer_limage_du_stock->disable(this);
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
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionStocks, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSortir, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);

#ifdef YEROTH_CLIENT
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAnnuler, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEnregisterStock, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerImage, true);

    pushButton_supprimer_limage_du_stock->enable(this, SLOT(supprimer_image_stock()));
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
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionStocks, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSortir, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAnnuler, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEnregisterStock, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerImage, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);

    pushButton_supprimer_limage_du_stock->disable(this);
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
    double qte_total = doubleSpinBox_lots_entrant->valueMultiplyBy(qte_lot);
    lineEdit_quantite_total->setText(QString::number(qte_total, 'f', 2));
}

void YerothEntrerWindow::display_quantite_total_by_spinbox(double lots)
{
    double qte_lot = lineEdit_quantite_par_lot->text().toDouble();
    double qte_total = lots * qte_lot;
    lineEdit_quantite_total->setText(QString::number(qte_total, 'f', 2));
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


void YerothEntrerWindow::setStockSpecificWidgetVisible(bool visible)
{
	if (visible)
	{
		lineEdit_quantite_total->setFixedWidth(104);
	}
	else
	{
		lineEdit_quantite_total->setFixedWidth(205);
	}

	label_reference_recu_dachat->setVisible(visible);
	lineEdit_reference_recu_dachat->setVisible(visible);

	label_stock_minimum->setVisible(visible);
	lineEdit_stock_minimum->setVisible(visible);

	label_localisation_du_stock->setVisible(visible);
	lineEdit_localisation_produit->setVisible(visible);

	dateEdit_date_peremption->setVisible(visible);
	label_date_peremption->setVisible(visible);

	label_prix_dachat->setVisible(visible);
	lineEdit_prix_dachat->setVisible(visible);

	checkBox_achat->setVisible(visible);
}


void YerothEntrerWindow::handleServiceCheckBox(bool clicked)
{
	if (clicked && checkBox_service->isChecked())
	{
	    check_fields_service();

	    setStockSpecificWidgetVisible(false);

    	QString aConditionStr(YerothUtils::generateSqlIs(YerothDatabaseTableColumn::IS_SERVICE,
    						  YerothUtils::MYSQL_TRUE_LITERAL));

		lineEdit_designation->setupMyStaticQCompleter(_allWindows->MARCHANDISES,
													  YerothDatabaseTableColumn::DESIGNATION,
													  false,
													  true,
													  aConditionStr);

	    label_fournisseur->setText(QObject::tr("client"));

        lineEdit_nom_entreprise_fournisseur->setupMyStaticQCompleter(_allWindows->CLIENTS,
        															 YerothDatabaseTableColumn::NOM_ENTREPRISE,
																	 true);
	}
	else
	{
		lineEdit_designation->clearQCompleter();

		check_fields(true);

		setStockSpecificWidgetVisible(true);

		label_fournisseur->setText(QObject::tr("fournisseur"));

        lineEdit_nom_entreprise_fournisseur->setupMyStaticQCompleter(_allWindows->FOURNISSEURS,
        															 YerothDatabaseTableColumn::NOM_ENTREPRISE,
																	 true);
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


bool YerothEntrerWindow::check_fields_service()
{
	bool designation = lineEdit_designation->checkField();

	bool client_fournisseur = lineEdit_nom_entreprise_fournisseur->checkField();

	bool categorie_produit = lineEdit_categorie_produit->checkField();

	bool reference = lineEdit_reference_produit->checkField();

    bool quantite = lineEdit_quantite_par_lot->checkField();

    bool prix_vente = lineEdit_prix_vente->checkField();

    bool result = designation 		 &&
    			  client_fournisseur &&
    			  categorie_produit  &&
				  reference 		 &&
				  prix_vente 		 &&
				  quantite;

    return result;
}


bool YerothEntrerWindow::check_fields(bool withClearAllServiceMandatoryFields /*  = false */)
{
	if (withClearAllServiceMandatoryFields)
	{
		lineEdit_nom_entreprise_fournisseur->clearField();

		lineEdit_reference_produit->clearField();
	}

	bool prix_dachat = check_fields_mandatory_buying();

    bool designation = lineEdit_designation->checkField();

    bool quantite = lineEdit_quantite_par_lot->checkField();

    bool prix_vente = lineEdit_prix_vente->checkField();

    bool categorie_produit = lineEdit_categorie_produit->checkField();

    bool result = prix_dachat		&&
    			  designation       &&
    			  categorie_produit &&
				  prix_vente 		&&
				  quantite;

    return result;
}


void YerothEntrerWindow::clear_all_fields()
{
    lineEdit_reference_produit->clearField();
    lineEdit_designation->clearField();
    lineEdit_categorie_produit->clearField();
    lineEdit_nom_entreprise_fournisseur->clearField();
    doubleSpinBox_lots_entrant->setValue(1.0);
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
    checkBox_achat->setChecked(true);
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

    setupLineEditsQCompleters();

    lineEdit_tva->setText(YerothUtils::getTvaStringWithPercent());


    if (aShowItem)
    {
    	checkBox_service->setChecked(false);
    	checkBox_service->clicked();
    }

    if (checkBox_service->isChecked())
    {
    	check_fields_service();
    }
    else
    {
        if (aShowItem)
        {
            clear_all_fields();

            check_fields(true);

            showItem();

            lineEdit_reference_produit->clearFocus();
        }
        else
        {
        	check_fields(true);

            lineEdit_reference_produit->setFocus();
        }
    }

    setVisible(true);
}


bool YerothEntrerWindow::check_fields_mandatory_buying()
{
	bool prix_dachat = true;

	if (checkBox_achat->isChecked())
	{
		prix_dachat = lineEdit_prix_dachat->checkField();
	}
	else
	{
		lineEdit_prix_dachat->clearField();
	}

    return prix_dachat;
}


bool YerothEntrerWindow::insertStockItemInProductList()
{
    bool success = false;

    YerothSqlTableModel & productListSqlTableModel = _allWindows->getSqlTableModel_marchandises();

    QSqlRecord record = productListSqlTableModel.record();

    record.setValue(YerothDatabaseTableColumn::ID,
                    YerothUtils::getNextIdFromTable(_allWindows->MARCHANDISES));

    if (checkBox_service->isChecked())
    {
    	record.setValue(YerothDatabaseTableColumn::IS_SERVICE, YerothUtils::MYSQL_TRUE_LITERAL);
    }
    else
    {
    	record.setValue(YerothDatabaseTableColumn::IS_SERVICE, YerothUtils::MYSQL_FALSE_LITERAL);
    }

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
	bool result = false;

    YerothSqlTableModel & productListSqlTableModel = _allWindows->getSqlTableModel_marchandises();

    if (checkBox_service->isChecked())
    {
        int referenceRowCount =
            productListSqlTableModel.Is_SearchQSqlTable(YerothDatabaseTableColumn::REFERENCE,
            		lineEdit_reference_produit->text());

    	return (referenceRowCount > 0);
    }

    int designationRowCount =
        productListSqlTableModel.Is_SearchQSqlTable(YerothDatabaseTableColumn::DESIGNATION,
                lineEdit_designation->text());

    int categorieRowCount =
        productListSqlTableModel.Is_SearchQSqlTable(YerothDatabaseTableColumn::CATEGORIE,
                lineEdit_categorie_produit->text());

    result = (designationRowCount > 0) && (categorieRowCount > 0);

    return result;
}


bool YerothEntrerWindow::handle_stocks_vendu_table(int stockID,
							   	   	   	   	   	   ServiceClientInfo &aServiceInfo,
												   double montant_total_vente,
												   double nouveau_compte_client)
{
	QString clientName = lineEdit_nom_entreprise_fournisseur->text();

    int stocksVenduID = _allWindows->getNextIdSqlTableModel_stocks_vendu();

    QString referenceRecuVenduCompteClient(YerothUtils::GET_REFERENCE_RECU_VENDU(QString::number(stocksVenduID)));

	YerothSqlTableModel &stocksTableModel = _allWindows->getSqlTableModel_stocks();

	QString stocksTableFilter = QString("%1 = '%2'")
	            					.arg(YerothDatabaseTableColumn::ID,
	            					     stockID);

	stocksTableModel.yerothSetFilter(stocksTableFilter);

	int rows = stocksTableModel.easySelect();

	if (rows > 0)
	{
		QSqlRecord stockRecord = stocksTableModel.record(0);

        QString quantiteQueryStr(QString("SELECT %1 FROM %2 WHERE id = '%3'")
        							.arg(YerothDatabaseTableColumn::QUANTITE_TOTAL,
        								 _allWindows->STOCKS,
										 QString::number(stockID)));
        QSqlQuery quantiteQuery;

        double quantite_actuelle = 0.0;

        int querySize = YerothUtils::execQuery(quantiteQuery, quantiteQueryStr, _logger);

        if (querySize > 0 && quantiteQuery.next())
        {
            quantite_actuelle = quantiteQuery.value(0).toDouble();
        }

        QString historiqueStock =
        		GET_SQL_RECORD_DATA(stockRecord, YerothDatabaseTableColumn::HISTORIQUE_STOCK);

        YerothSqlTableModel & stocksVenduTableModel = _allWindows->getSqlTableModel_stocks_vendu();

        QSqlRecord record = stocksVenduTableModel.record();

        QString typeDeVente = QObject::tr("achat-compte-client");

        record.setValue(YerothDatabaseTableColumn::TYPE_DE_VENTE, typeDeVente);

        record.setValue(YerothDatabaseTableColumn::ID, stocksVenduID);

        record.setValue(YerothDatabaseTableColumn::REFERENCE, aServiceInfo.reference);

        record.setValue(YerothDatabaseTableColumn::DESIGNATION, aServiceInfo.designation);

        record.setValue(YerothDatabaseTableColumn::CATEGORIE, aServiceInfo.nom_categorie);

        record.setValue(YerothDatabaseTableColumn::IS_SERVICE, YerothUtils::MYSQL_TRUE_LITERAL);

        record.setValue(YerothDatabaseTableColumn::REMISE_PRIX, 0.0);

        record.setValue(YerothDatabaseTableColumn::REMISE_POURCENTAGE, 0.0);

        record.setValue(YerothDatabaseTableColumn::DATE_PEREMPTION, GET_CURRENT_DATE);

        record.setValue(YerothDatabaseTableColumn::REFERENCE_RECU_VENDU, referenceRecuVenduCompteClient);

        double quantite_total_vendue =
        		GET_SQL_RECORD_DATA(stockRecord, YerothDatabaseTableColumn::QUANTITE_TOTAL).toDouble();

        record.setValue(YerothDatabaseTableColumn::QUANTITE_VENDUE, quantite_total_vendue);

        record.setValue(YerothDatabaseTableColumn::PRIX_UNITAIRE,
        			GET_SQL_RECORD_DATA(stockRecord, YerothDatabaseTableColumn::PRIX_UNITAIRE).toDouble());

        record.setValue(YerothDatabaseTableColumn::MONTANT_TOTAL_VENTE, montant_total_vente);

        record.setValue(YerothDatabaseTableColumn::MONTANT_TVA,
        		GET_SQL_RECORD_DATA(stockRecord, YerothDatabaseTableColumn::MONTANT_TVA).toDouble());

        record.setValue(YerothDatabaseTableColumn::LOCALISATION,
        		GET_SQL_RECORD_DATA(stockRecord, YerothDatabaseTableColumn::LOCALISATION));

        record.setValue(YerothDatabaseTableColumn::NOM_ENTREPRISE_CLIENT,
                        GET_SQL_RECORD_DATA(stockRecord, YerothDatabaseTableColumn::NOM_ENTREPRISE_CLIENT));

        YerothPOSUser *user = _allWindows->getUser();

        record.setValue(YerothDatabaseTableColumn::NOM_CAISSIER, user->nom_complet());
        record.setValue(YerothDatabaseTableColumn::NOM_UTILISATEUR_CAISSIER, user->nom_utilisateur());
        record.setValue(YerothDatabaseTableColumn::DATE_VENTE, GET_CURRENT_DATE);
        record.setValue(YerothDatabaseTableColumn::HEURE_VENTE, CURRENT_TIME);

        record.setValue(YerothDatabaseTableColumn::STOCKS_ID, stockID);

        record.setValue(YerothDatabaseTableColumn::MONTANT_RECU, montant_total_vente);

        double montant_a_rembourser = montant_total_vente;

        if (nouveau_compte_client >= 0.0)
        {
        	montant_a_rembourser = 0.0;
        }
        else if (qFabs(nouveau_compte_client) >= montant_total_vente)
        {
        	montant_a_rembourser = montant_total_vente;
        }
        else
        {
        	montant_a_rembourser = qFabs(nouveau_compte_client);
        }

        record.setValue(YerothDatabaseTableColumn::MONTANT_A_REMBOURSER, montant_a_rembourser);

        double nouvelle_quantite = quantite_actuelle - quantite_total_vendue;

        QString historiqueStockVendu(
        		YerothHistoriqueStock::creer_mouvement_stock
        									(VENTE,
        									 stocksVenduID,
											 GET_CURRENT_DATE,
											 quantite_actuelle,
											 quantite_total_vendue,
											 nouvelle_quantite));

        historiqueStock.append(QString("%1%2")
        							.arg(YerothHistoriqueStock::SEPARATION_EXTERNE,
        								 historiqueStockVendu));

        record.setValue(YerothDatabaseTableColumn::COMPTE_CLIENT, aServiceInfo.nouveau_compte_client);
        record.setValue(YerothDatabaseTableColumn::CLIENTS_ID, aServiceInfo.ID_client);
        record.setValue(YerothDatabaseTableColumn::NOM_ENTREPRISE_CLIENT, aServiceInfo.nom_entreprise_client);

        bool success1 = stocksVenduTableModel.insertNewRecord(record, this);

        if (success1)
        {
        	QString removeRowQuery(QString("DELETE FROM %1 WHERE %2 = '%3'")
        								.arg(_allWindows->STOCKS,
        									 YerothDatabaseTableColumn::ID,
											 QString::number(stockID)));

        	YerothUtils::execQuery(removeRowQuery);

    		if (0 == montant_a_rembourser)
    		{
    			//handle marchandise table
    			removeRowQuery = QString("DELETE FROM %1 WHERE %2 = '%3'")
    								.arg(_allWindows->MARCHANDISES,
    									 YerothDatabaseTableColumn::REFERENCE,
										 aServiceInfo.reference);

    			YerothUtils::execQuery(removeRowQuery);
    		}

        	return true;
        }
	}

	return false;
}


bool YerothEntrerWindow::handle_clients_table(int stockID, double montant_total_vente)
{
	ServiceClientInfo aServiceClientInfo;

	QString clientName = lineEdit_nom_entreprise_fournisseur->text();

	YerothSqlTableModel &clientsTableModel = _allWindows->getSqlTableModel_clients();

	QString clientsTableFilter = QString("%1 = '%2'")
	            					.arg(YerothDatabaseTableColumn::NOM_ENTREPRISE,
	            						 clientName);

	clientsTableModel.yerothSetFilter(clientsTableFilter);

	int rows = clientsTableModel.easySelect();

	if (rows > 0)
	{
		QSqlRecord clientsRecord = clientsTableModel.record(0);

        double compteClient =
        	GET_SQL_RECORD_DATA(clientsRecord, YerothDatabaseTableColumn::COMPTE_CLIENT).toDouble();

        double nouveau_compte_client = compteClient - montant_total_vente;

		double detteMaximale =
			GET_SQL_RECORD_DATA(clientsRecord, YerothDatabaseTableColumn::DETTE_MAXIMALE_COMPTE_CLIENT).toDouble();

        if (nouveau_compte_client >= 0.0)
        {
        	detteMaximale = 0.0;
        }
        else if (qFabs(nouveau_compte_client) >= montant_total_vente)
        {
        	detteMaximale = detteMaximale + montant_total_vente;
        }
        else
        {
        	detteMaximale = qFabs(nouveau_compte_client);
        }

		clientsRecord.setValue(YerothDatabaseTableColumn::COMPTE_CLIENT, nouveau_compte_client);
		clientsRecord.setValue(YerothDatabaseTableColumn::DETTE_MAXIMALE_COMPTE_CLIENT, detteMaximale);

		bool success_clients = clientsTableModel.updateRecord(0, clientsRecord);

    	aServiceClientInfo.ID_client = GET_SQL_RECORD_DATA(clientsRecord, YerothDatabaseTableColumn::ID).toInt();

    	aServiceClientInfo.reference = lineEdit_reference_produit->text();
    	aServiceClientInfo.designation = lineEdit_designation->text();
    	aServiceClientInfo.nom_categorie = lineEdit_categorie_produit->text();
    	aServiceClientInfo.nom_entreprise_client = clientName;
    	aServiceClientInfo.nouveau_compte_client = nouveau_compte_client ;

		clientsTableModel.resetFilter();

    	//handle 'stocks_vendu' table
    	bool success_stocksVendu = handle_stocks_vendu_table(stockID,
    							  	  	  	  	  	  	  	 aServiceClientInfo,
															 montant_total_vente,
															 nouveau_compte_client);

		return success_stocksVendu && true;
	}

	return false;
}


void YerothEntrerWindow::enregistrer_produit()
{
    _logger->log("enregistrer_produit");

    bool result_check_field = false;

    if (checkBox_service->isChecked())
    {
    	result_check_field = check_fields_service();
    }
    else
    {
    	result_check_field = check_fields();
    }

    if (!result_check_field)
    {
        if (QMessageBox::Ok ==
            	YerothQMessageBox::warning(this, "enregistrer",
                                           tr("Remplisser tous les champs obligatoires !")))
        {
        }
        else
        {
        }

        return ;
    }

    if (!YerothUtils::isProfitable(lineEdit_prix_vente->text().toDouble(),
    							   lineEdit_prix_dachat->text().toDouble(),
								   _montantTva))
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

    if (!checkBox_service->isChecked() &&
    	dateEdit_date_peremption->date() <= QDate::currentDate())
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
    else
    {
    	if (checkBox_service->isChecked())
    	{
    		YerothQMessageBox::information(this,
    						 QObject::trUtf8("aide"),
    						 QString(QObject::trUtf8("Un service avec la référence '%1' existe déjà !")
    						 	 .arg(lineEdit_reference_produit->text())));
    		return;
    	}
    }


    QString proposed_Fournisseur_Client_Name = lineEdit_nom_entreprise_fournisseur->text();

    if (checkBox_service->isChecked())
    {
        if (!proposed_Fournisseur_Client_Name.isEmpty())
        {
        	if (!creerNouveauClient(proposed_Fournisseur_Client_Name))
        	{
        		return ;
        	}
        }
    }
    else
    {
        if (!proposed_Fournisseur_Client_Name.isEmpty())
        {
        	if (!creerNouveauFournisseur(proposed_Fournisseur_Client_Name))
        	{
        		return ;
        	}
        }
    }

    QString proposedCategorieName = lineEdit_categorie_produit->text();

    if (!creerNouvelleCategorie(proposedCategorieName))
    {
    	return ;
    }

    YerothSqlTableModel & achatSqlTableModel = _allWindows->getSqlTableModel_achats();

    QSqlRecord achatRecord;

    QSqlRecord record = _curStocksTableModel->record();

    int stock_id_to_save = _allWindows->getNextIdSqlTableModel_stocks();

    if (!checkBox_service->isChecked() && hasBuying())
    {
    	achatRecord = achatSqlTableModel.record();

    	int achat_id_to_save = YerothUtils::getNextIdFromTable(_allWindows->ACHATS);

    	achatRecord.setValue(YerothDatabaseTableColumn::ID, achat_id_to_save);
    	achatRecord.setValue(YerothDatabaseTableColumn::STOCKS_ID, stock_id_to_save);
    	achatRecord.setValue(YerothDatabaseTableColumn::REFERENCE, lineEdit_reference_produit->text());
    	achatRecord.setValue(YerothDatabaseTableColumn::DESIGNATION, lineEdit_designation->text());
    	achatRecord.setValue(YerothDatabaseTableColumn::CATEGORIE, proposedCategorieName);
    	achatRecord.setValue(YerothDatabaseTableColumn::DESCRIPTION_PRODUIT, textEdit_description->toPlainText());
    	achatRecord.setValue(YerothDatabaseTableColumn::LOTS_ENTRANT, doubleSpinBox_lots_entrant->value());
    	achatRecord.setValue(YerothDatabaseTableColumn::QUANTITE_PAR_LOT, lineEdit_quantite_par_lot->text().toDouble());
    }

    record.setValue(YerothDatabaseTableColumn::ID, stock_id_to_save);
    record.setValue(YerothDatabaseTableColumn::REFERENCE, lineEdit_reference_produit->text());
    record.setValue(YerothDatabaseTableColumn::DESIGNATION, lineEdit_designation->text());
    record.setValue(YerothDatabaseTableColumn::CATEGORIE, proposedCategorieName);
    record.setValue(YerothDatabaseTableColumn::DESCRIPTION_PRODUIT, textEdit_description->toPlainText());
    record.setValue(YerothDatabaseTableColumn::LOTS_ENTRANT, doubleSpinBox_lots_entrant->value());
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

    double montant_total_vente = quantite_total * prix_vente;

    QString utilisateurCourrantNomComplet;

    YerothPOSUser *aUser = _allWindows->getUser();

    if (0 != aUser)
    {
    	utilisateurCourrantNomComplet.append(aUser->nom_complet());
    }

    if (!checkBox_service->isChecked() && hasBuying())
    {
    	achatRecord.setValue(YerothDatabaseTableColumn::ENREGISTREUR_STOCK, utilisateurCourrantNomComplet);
    	achatRecord.setValue(YerothDatabaseTableColumn::QUANTITE_TOTAL, quantite_total);
    	achatRecord.setValue(YerothDatabaseTableColumn::STOCK_MINIMUM, stock_minimum);
    	achatRecord.setValue(YerothDatabaseTableColumn::REFERENCE_RECU_DACHAT, reference_recu_dachat);
    	achatRecord.setValue(YerothDatabaseTableColumn::PRIX_DACHAT, prix_dachat);
    	achatRecord.setValue(YerothDatabaseTableColumn::PRIX_VENTE, prix_vente);
    	//qDebug() << "++_tva: " << QString::number(_tva, 'f', 2);
    	achatRecord.setValue(YerothDatabaseTableColumn::MONTANT_TVA, _montantTva);
    }

    if (!checkBox_service->isChecked())
    {
    	record.setValue(YerothDatabaseTableColumn::REFERENCE_RECU_DACHAT, reference_recu_dachat);
    	record.setValue(YerothDatabaseTableColumn::STOCK_MINIMUM, stock_minimum);
    	record.setValue(YerothDatabaseTableColumn::PRIX_DACHAT, prix_dachat);
    }

    record.setValue(YerothDatabaseTableColumn::ENREGISTREUR_STOCK, utilisateurCourrantNomComplet);
    record.setValue(YerothDatabaseTableColumn::QUANTITE_TOTAL, quantite_total);
    record.setValue(YerothDatabaseTableColumn::PRIX_VENTE, prix_vente);
    //qDebug() << "++_tva: " << QString::number(_tva, 'f', 2);
    record.setValue(YerothDatabaseTableColumn::MONTANT_TVA, _montantTva);

    double prix_unitaire_ht = prix_vente - _montantTva;

    double marge_beneficiaire = YerothUtils::getMargeBeneficiaire(prix_vente, prix_dachat, _montantTva);

    if (!checkBox_service->isChecked() && hasBuying())
    {
    	achatRecord.setValue(YerothDatabaseTableColumn::MARGE_BENEFICIAIRE, marge_beneficiaire);
    	achatRecord.setValue(YerothDatabaseTableColumn::PRIX_UNITAIRE, prix_unitaire_ht);
    	achatRecord.setValue(YerothDatabaseTableColumn::NOM_ENTREPRISE_FOURNISSEUR, proposed_Fournisseur_Client_Name);
    	achatRecord.setValue(YerothDatabaseTableColumn::LOCALISATION, _allWindows->getInfoEntreprise().getLocalisation());
    	achatRecord.setValue(YerothDatabaseTableColumn::LOCALISATION_STOCK, lineEdit_localisation_produit->text());
    	achatRecord.setValue(YerothDatabaseTableColumn::DATE_ENTREE, GET_CURRENT_DATE);
    	achatRecord.setValue(YerothDatabaseTableColumn::DATE_PEREMPTION, dateEdit_date_peremption->date());
    }

    if (!checkBox_service->isChecked())
    {
    	record.setValue(YerothDatabaseTableColumn::IS_SERVICE, YerothUtils::MYSQL_FALSE_LITERAL);
    	record.setValue(YerothDatabaseTableColumn::LOCALISATION_STOCK, lineEdit_localisation_produit->text());
    	record.setValue(YerothDatabaseTableColumn::DATE_PEREMPTION, dateEdit_date_peremption->date());
    	record.setValue(YerothDatabaseTableColumn::NOM_ENTREPRISE_FOURNISSEUR, proposed_Fournisseur_Client_Name);
    }
    else
    {
    	record.setValue(YerothDatabaseTableColumn::IS_SERVICE, YerothUtils::MYSQL_TRUE_LITERAL);
    	record.setValue(YerothDatabaseTableColumn::NOM_ENTREPRISE_CLIENT, proposed_Fournisseur_Client_Name);
    }

    record.setValue(YerothDatabaseTableColumn::PRIX_UNITAIRE, prix_unitaire_ht);
    record.setValue(YerothDatabaseTableColumn::LOCALISATION, _allWindows->getInfoEntreprise().getLocalisation());
    record.setValue(YerothDatabaseTableColumn::DATE_ENTREE, GET_CURRENT_DATE);

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

    bool achatSuccess = false;

    if (!checkBox_service->isChecked() && hasBuying())
    {
    	achatSuccess = achatSqlTableModel.insertNewRecord(achatRecord);
    }

    bool success = _curStocksTableModel->insertNewRecord(record);

    QString achatRetMsg(QString(QObject::tr("L'achat du stock '%1'"))
    		.arg(lineEdit_designation->text()));


    if (!checkBox_service->isChecked() && hasBuying())
    {
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
    }

    QString retMsg;

    if (checkBox_service->isChecked())
    {
        retMsg.append(QString(QObject::tr("Le service '%1'"))
        				.arg(lineEdit_reference_produit->text()));
    }
    else
    {
        retMsg.append(QString(QObject::tr("Le stock '%1'"))
        				.arg(lineEdit_designation->text()));
    }

    if (success)
    {
    	//handle 'clients' table
    	handle_clients_table(stock_id_to_save, montant_total_vente);

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

    YEROTH_ERP_3_0_COMMIT_DATABASE_TRANSACTION;

    rendreInvisible();

    if (checkBox_service->isChecked())
    {
    	_allWindows->_marchandisesWindow->setCheckBoxServices(true);
    	_allWindows->_marchandisesWindow->rendreVisible(_curStocksTableModel);
    }
    else
    {
    	_allWindows->_stocksWindow->rendreVisible(_curStocksTableModel);
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

    QString recordID = GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::ID);

    QString strAchatsQuery(QString("SELECT %1 FROM %2 WHERE %3 = '%4'")
    							.arg(YerothDatabaseTableColumn::ID,
    								 _allWindows->ACHATS,
    								 YerothDatabaseTableColumn::STOCKS_ID,
									 recordID));

    QSqlQuery query;

    int achatQuerySize = YerothUtils::execQuery(query, strAchatsQuery, _logger);

//    qDebug() << QString("++ strAchatsQuery: %1, querySize: %2")
//    				.arg(strAchatsQuery,
//    					 QString::number(achatQuerySize));

    if (1 == achatQuerySize)
    {
    	checkBox_achat->setChecked(true);
    }
    else
    {
    	checkBox_achat->setChecked(false);
    }
}
