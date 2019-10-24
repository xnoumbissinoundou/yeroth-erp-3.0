

void YerothAdminModifierWindow::setupEditCategorie()
{
    YerothAdminListerWindow *lw = _allWindows->_adminListerWindow;
    YerothSqlTableModel *categoriesTableModel = lw->getCurSearchSqlTableModel();

    if (0 == categoriesTableModel)
    {
        categoriesTableModel =  &_allWindows->getSqlTableModel_categories();
    }
    else if (false == YerothUtils::isEqualCaseInsensitive(categoriesTableModel->sqlTableName(), _allWindows->CATEGORIES))
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
		YerothUtils::startTransaction();
		modifier_categorie_main();
		YerothUtils::commitTransaction();
	}
}

void YerothAdminModifierWindow::modifier_categorie_main()
{
    YerothAdminListerWindow *lw = _allWindows->_adminListerWindow;
    YerothSqlTableModel *categoriesTableModel = lw->getCurSearchSqlTableModel();

    if (!categoriesTableModel)
    {
        categoriesTableModel =  &_allWindows->getSqlTableModel_categories();
    }
    else if (categoriesTableModel &&
             !YerothUtils::isEqualCaseInsensitive(categoriesTableModel->sqlTableName(),
                     _allWindows->CATEGORIES))
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
        QString stocksQuery("UPDATE ");
        stocksQuery.append(_allWindows->STOCKS).append(" SET ")
        .append("categorie='").append(newNomCategorie)
        .append("'").append(" WHERE categorie='")
        .append(oldNomCategorie).append("'");

        YerothUtils::execQuery(stocksQuery, 0);
    }

    if (successCategorieTable)
    {
        ////Handling of table "stocks_vendu"
        QString stocksVenduQuery("UPDATE ");
        stocksVenduQuery.append(_allWindows->STOCKS_VENDU).append(" SET ")
        .append("categorie='").append(newNomCategorie)
        .append("'").append(" WHERE categorie='")
        .append(oldNomCategorie).append("'");

        YerothUtils::execQuery(stocksVenduQuery, 0);
    }

    if (successCategorieTable)
    {
        ////Handling of table "stocks_sorties"
        QString stocksVenduQuery("UPDATE ");
        stocksVenduQuery.append(_allWindows->STOCKS_SORTIES).append(" SET ")
        .append("categorie='").append(newNomCategorie)
        .append("'").append(" WHERE categorie='")
        .append(oldNomCategorie).append("'");

        YerothUtils::execQuery(stocksVenduQuery, 0);
    }

    if (successCategorieTable)
    {
        ////Handling of table "inventaire_des_stocks"
        QString stocksVenduQuery("UPDATE ");
        stocksVenduQuery.append(_allWindows->MARCHANDISES).append(" SET ")
        .append("categorie='").append(newNomCategorie)
        .append("'").append(" WHERE categorie='")
        .append(oldNomCategorie).append("'");

        YerothUtils::execQuery(stocksVenduQuery, 0);
    }

    QString retMsg("Les données de la catégorie '");
    retMsg.append(lineEdit_modifier_categorie_nom->text())
    .append("'");

    if (successCategorieTable)
    {
        retMsg.append(" ont été modifiées avec succès!");
        YerothQMessageBox::information(this,
                                 QObject::trUtf8("Yeroth-erp-3.0 ~ admin-modifier-catégorie"),
                                 FROM_UTF8_STRING(retMsg));

        _allWindows->_adminListerWindow->rendreVisible(SUJET_ACTION_CATEGORIE);
        this->rendreInvisible();
    }
    else
    {
        qDebug() << "\t[reason for failing] " << categoriesTableModel->lastError();
        retMsg.append(" n'ont pas pu être modifiées !");
        YerothQMessageBox::warning(this,
                             QObject::trUtf8("Yeroth-erp-3.0 ~ admin-modifier-catégorie"),
                             FROM_UTF8_STRING(retMsg));
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
