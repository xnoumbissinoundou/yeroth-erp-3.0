

void YerothAdminModifierWindow::setupEditDepartementsDeProduits()
{
	YerothAdminListerWindow *lw = _allWindows->_adminListerWindow;

	YerothSqlTableModel *departementsDeProduitsTableModel = lw->getCurSearchSqlTableModel();

	if (0 == departementsDeProduitsTableModel)
	{
		departementsDeProduitsTableModel =  &_allWindows->getSqlTableModel_departements_produits();
	}
	else if (false == YerothUtils::isEqualCaseInsensitive(departementsDeProduitsTableModel->sqlTableName(),
														  _allWindows->DEPARTEMENTS_PRODUITS))
	{
		departementsDeProduitsTableModel =  &_allWindows->getSqlTableModel_departements_produits();
	}

	QSqlRecord record = departementsDeProduitsTableModel->record(lw->lastSelectedItemForModification());

	lineEdit_modifier_departements_de_produits_nom->
		setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::NOM_DEPARTEMENT_PRODUIT));

	textEdit_modifier_departements_de_produits_description->
		setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::DESCRIPTION_DEPARTEMENT_PRODUIT));
}


void YerothAdminModifierWindow::modifier_departements_de_produits()
{
	if (modifier_departements_de_produits_check_fields())
	{
		YEROTH_ERP_3_0_START_DATABASE_TRANSACTION;
		modifier_departements_de_produits_main();
		YEROTH_ERP_3_0_COMMIT_DATABASE_TRANSACTION;
	}
}


void YerothAdminModifierWindow::modifier_departements_de_produits_main()
{
    YerothAdminListerWindow *lw = _allWindows->_adminListerWindow;


    YerothSqlTableModel *departementsDeProduitsTableModel = lw->getCurSearchSqlTableModel();


    if (0 == departementsDeProduitsTableModel)
    {
        departementsDeProduitsTableModel =  &_allWindows->getSqlTableModel_departements_produits();
    }
    else if (!YerothUtils::isEqualCaseInsensitive(departementsDeProduitsTableModel->sqlTableName(),
                     	 	 	 	 	 	 	  _allWindows->DEPARTEMENTS_PRODUITS))
    {
        departementsDeProduitsTableModel =  &_allWindows->getSqlTableModel_departements_produits();
    }


    QSqlRecord record = departementsDeProduitsTableModel->record(lw->lastSelectedItemForModification());


    QString current_nom_departement_produit(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::NOM_DEPARTEMENT_PRODUIT));

    QString new_nom_departement_produit(lineEdit_modifier_departements_de_produits_nom->text());

    record.setValue(YerothDatabaseTableColumn::NOM_DEPARTEMENT_PRODUIT, new_nom_departement_produit);

    record.setValue(YerothDatabaseTableColumn::DESCRIPTION_DEPARTEMENT_PRODUIT,
    				textEdit_modifier_departements_de_produits_description->toPlainText());


    bool success_departements_produits_table =
    		departementsDeProduitsTableModel->
				updateRecord(lw->lastSelectedItemForModification(), record);


    if (success_departements_produits_table)
    {
        //Handling of table "stocks"
    	QString stocksQuery(QString("UPDATE %1 SET %2='%3' WHERE %4='%5'")
    							.arg(_allWindows->CATEGORIES,
    								 YerothDatabaseTableColumn::NOM_DEPARTEMENT_PRODUIT,
									 new_nom_departement_produit,
									 YerothDatabaseTableColumn::NOM_DEPARTEMENT_PRODUIT,
									 current_nom_departement_produit));

        YerothUtils::execQuery(stocksQuery, 0);
    }

    if (success_departements_produits_table)
    {
        //Handling of table "stocks"
    	QString stocksQuery(QString("UPDATE %1 SET %2='%3' WHERE %4='%5'")
    							.arg(_allWindows->STOCKS,
    								 YerothDatabaseTableColumn::NOM_DEPARTEMENT_PRODUIT,
									 new_nom_departement_produit,
									 YerothDatabaseTableColumn::NOM_DEPARTEMENT_PRODUIT,
									 current_nom_departement_produit));

        YerothUtils::execQuery(stocksQuery, 0);
    }

    if (success_departements_produits_table)
    {
        ////Handling of table "stocks_vendu"
    	QString stocksVenduQuery(QString("UPDATE %1 SET %2='%3' WHERE %4='%5'")
    							.arg(_allWindows->STOCKS_VENDU,
    								 YerothDatabaseTableColumn::NOM_DEPARTEMENT_PRODUIT,
									 new_nom_departement_produit,
									 YerothDatabaseTableColumn::NOM_DEPARTEMENT_PRODUIT,
									 current_nom_departement_produit));

        YerothUtils::execQuery(stocksVenduQuery, 0);
    }

    if (success_departements_produits_table)
    {
        ////Handling of table "stocks_sorties"
    	QString stocksVenduQuery(QString("UPDATE %1 SET %2='%3' WHERE %4='%5'")
    							.arg(_allWindows->STOCKS_SORTIES,
    								 YerothDatabaseTableColumn::NOM_DEPARTEMENT_PRODUIT,
									 new_nom_departement_produit,
									 YerothDatabaseTableColumn::NOM_DEPARTEMENT_PRODUIT,
									 current_nom_departement_produit));

        YerothUtils::execQuery(stocksVenduQuery, 0);
    }

    if (success_departements_produits_table)
    {
        ////Handling of table "inventaire_des_stocks"
    	QString stocksVenduQuery(QString("UPDATE %1 SET %2='%3' WHERE %4='%5'")
    							.arg(_allWindows->MARCHANDISES,
    								 YerothDatabaseTableColumn::NOM_DEPARTEMENT_PRODUIT,
									 new_nom_departement_produit,
									 YerothDatabaseTableColumn::NOM_DEPARTEMENT_PRODUIT,
									 current_nom_departement_produit));

        YerothUtils::execQuery(stocksVenduQuery, 0);
    }

    QString retMsg(QObject::trUtf8("Les données du département de produits '%1'")
    					.arg(lineEdit_modifier_departements_de_produits_nom->text()));

    if (success_departements_produits_table)
    {
        retMsg.append(QObject::trUtf8(" ont été modifiées avec succès !"));

        YerothQMessageBox::information(this,
                                 QObject::trUtf8("Yeroth-erp-3.0 ~ admin-modifier-département_de_produits"),
                                 retMsg);

        _allWindows->_adminListerWindow->rendreVisible(SUJET_ACTION_DEPARTEMENTS_DE_PRODUITS);

         rendreInvisible();
    }
    else
    {
        qDebug() << "\t[reason for failing] " << departementsDeProduitsTableModel->lastError();

        retMsg.append(QObject::trUtf8(" n'ont pas pu être modifiées !"));

        YerothQMessageBox::warning(this,
                             QObject::trUtf8("Yeroth-erp-3.0 ~ admin-modifier-département_de_produits"),
                             retMsg);
    }
}


void YerothAdminModifierWindow::clear_departements_de_produits_all_fields()
{
	lineEdit_modifier_departements_de_produits_nom->clearField();
	textEdit_modifier_departements_de_produits_description->clear();
}

