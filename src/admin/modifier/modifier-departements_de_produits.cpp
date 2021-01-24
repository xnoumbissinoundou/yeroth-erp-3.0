

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
}

void YerothAdminModifierWindow::clear_departements_de_produits_all_fields()
{
	lineEdit_modifier_departements_de_produits_nom->clearField();
	textEdit_modifier_departements_de_produits_description->clear();
}
