

void YerothAdminModifierWindow::setupEditCategorie()
{
    YerothAdminListerWindow *lw = _allWindows->_adminListerWindow;
    YerothSqlTableModel *categoriesTableModel = lw->getCurSearchSqlTableModel();

    if (0 == categoriesTableModel)
    {
        categoriesTableModel =  &_allWindows->getSqlTableModel_categories();
    }
    else if (false == YerothUtils::isEqualCaseInsensitive(categoriesTableModel->sqlTableName(), YerothDatabase::CATEGORIES))
    {
        categoriesTableModel =  &_allWindows->getSqlTableModel_categories();
    }

    QSqlRecord record = categoriesTableModel->record(lw->lastSelectedItemForModification());

    lineEdit_modifier_categorie_nom->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::NOM_CATEGORIE));
    textEdit_modifier_categorie_description->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::DESCRIPTION_CATEGORIE));
}

void YerothAdminModifierWindow::modifier_categorie()
{
	if (modifier_categorie_check_fields())
	{
		YEROTH_ERP_3_0_START_DATABASE_TRANSACTION;
		modifier_categorie_main();
		YEROTH_ERP_3_0_COMMIT_DATABASE_TRANSACTION;
	}
}

void YerothAdminModifierWindow::modifier_categorie_main()
{
    YerothAdminListerWindow *lw = _allWindows->_adminListerWindow;
    YerothSqlTableModel *categoriesTableModel = lw->getCurSearchSqlTableModel();

    if (0 == categoriesTableModel)
    {
        categoriesTableModel =  &_allWindows->getSqlTableModel_categories();
    }
    else if (!YerothUtils::isEqualCaseInsensitive(categoriesTableModel->sqlTableName(),
                     	 	 	 	 	 	 	  YerothDatabase::CATEGORIES))
    {
        categoriesTableModel =  &_allWindows->getSqlTableModel_categories();
    }

    QSqlRecord record = categoriesTableModel->record(lw->lastSelectedItemForModification());

    QString oldNomCategorie(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::NOM_CATEGORIE));
    QString newNomCategorie(lineEdit_modifier_categorie_nom->text());

    record.setValue(YerothDatabaseTableColumn::NOM_CATEGORIE, newNomCategorie);
    record.setValue(YerothDatabaseTableColumn::DESCRIPTION_CATEGORIE, textEdit_modifier_categorie_description->toPlainText());

    bool successCategorieTable = categoriesTableModel->updateRecord(lw->lastSelectedItemForModification(), record);

    if (successCategorieTable)
    {
        //Handling of table "stocks"
    	QString stocksQuery(QString("UPDATE %1 SET %2='%3' WHERE %4='%5'")
    							.arg(YerothDatabase::STOCKS,
    								 YerothDatabaseTableColumn::CATEGORIE,
									 newNomCategorie,
									 YerothDatabaseTableColumn::CATEGORIE,
									 oldNomCategorie));

        YerothUtils::execQuery(stocksQuery, 0);
    }

    if (successCategorieTable)
    {
        ////Handling of table "stocks_vendu"
    	QString stocksVenduQuery(QString("UPDATE %1 SET %2='%3' WHERE %4='%5'")
    							.arg(YerothDatabase::STOCKS_VENDU,
    								 YerothDatabaseTableColumn::CATEGORIE,
									 newNomCategorie,
									 YerothDatabaseTableColumn::CATEGORIE,
									 oldNomCategorie));

        YerothUtils::execQuery(stocksVenduQuery, 0);
    }

    if (successCategorieTable)
    {
        ////Handling of table "stocks_sorties"
    	QString stocksVenduQuery(QString("UPDATE %1 SET %2='%3' WHERE %4='%5'")
    							.arg(YerothDatabase::STOCKS_SORTIES,
    								 YerothDatabaseTableColumn::CATEGORIE,
									 newNomCategorie,
									 YerothDatabaseTableColumn::CATEGORIE,
									 oldNomCategorie));

        YerothUtils::execQuery(stocksVenduQuery, 0);
    }

    if (successCategorieTable)
    {
        ////Handling of table "inventaire_des_stocks"
    	QString stocksVenduQuery(QString("UPDATE %1 SET %2='%3' WHERE %4='%5'")
    							.arg(YerothDatabase::MARCHANDISES,
    								 YerothDatabaseTableColumn::CATEGORIE,
									 newNomCategorie,
									 YerothDatabaseTableColumn::CATEGORIE,
									 oldNomCategorie));

        YerothUtils::execQuery(stocksVenduQuery, 0);
    }

    QString retMsg(QObject::trUtf8("Les données de la catégorie '%1'")
    					.arg(lineEdit_modifier_categorie_nom->text()));

    if (successCategorieTable)
    {
        retMsg.append(QObject::trUtf8(" ont été modifiées avec succès !"));

        YerothQMessageBox::information(this,
                                 QObject::trUtf8("Yeroth-pgi-3.0 ~ admin-modifier-catégorie"),
                                 retMsg);

        _allWindows->_adminListerWindow->rendreVisible(SUJET_ACTION_CATEGORIE);
         rendreInvisible();
    }
    else
    {
        qDebug() << "\t[reason for failing] " << categoriesTableModel->lastError();

        retMsg.append(QObject::trUtf8(" n'ont pas pu être modifiées !"));

        YerothQMessageBox::warning(this,
                             QObject::trUtf8("Yeroth-pgi-3.0 ~ admin-modifier-catégorie"),
                             retMsg);
    }
}

bool YerothAdminModifierWindow::modifier_categorie_check_fields()
{
    bool nom = lineEdit_modifier_categorie_nom->checkField();
    return nom;
}

void YerothAdminModifierWindow::clear_categorie_all_fields()
{
    lineEdit_modifier_categorie_nom->clearField();
    textEdit_modifier_categorie_description->clear();
}
