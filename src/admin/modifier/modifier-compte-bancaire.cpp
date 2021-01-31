

void YerothAdminModifierWindow::setupEditCompteBancaire()
{
    YerothAdminListerWindow *lw = _allWindows->_adminListerWindow;

    YerothSqlTableModel *comptesBancairesTableModel = lw->getCurSearchSqlTableModel();

    if (0 == comptesBancairesTableModel)
    {
        comptesBancairesTableModel =  &_allWindows->getSqlTableModel_comptes_bancaires();
    }
    else if (false == YerothUtils::isEqualCaseInsensitive(comptesBancairesTableModel->sqlTableName(),
    		 YerothDatabase::COMPTES_BANCAIRES))
    {
        comptesBancairesTableModel =  &_allWindows->getSqlTableModel_comptes_bancaires();
    }

    QSqlRecord record = comptesBancairesTableModel->record(lw->lastSelectedItemForModification());

    lineEdit_modifier_compte_bancaire_reference_du_compte_bancaire
		->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::REFERENCE_DU_COMPTE_BANCAIRE));

    lineEdit_modifier_compte_bancaire_intitule_du_compte_bancaire
		->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::INTITULE_DU_COMPTE_BANCAIRE));

    lineEdit_modifier_compte_bancaire_institut_bancaire
		->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::INSTITUT_BANCAIRE));

    textEdit_modifier_compte_bancaire_description_du_compte
		->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::DESCRIPTION_DU_COMPTE_BANCAIRE));
}

void YerothAdminModifierWindow::modifier_compte_bancaire()
{
	if (modifier_compte_bancaire_check_fields())
	{
		YEROTH_ERP_3_0_START_DATABASE_TRANSACTION;
		modifier_compte_bancaire_main();
		YEROTH_ERP_3_0_COMMIT_DATABASE_TRANSACTION;
	}
}

void YerothAdminModifierWindow::modifier_compte_bancaire_main()
{
//    YerothAdminListerWindow *lw = _allWindows->_adminListerWindow;
//
//    YerothSqlTableModel *comptesBancairesTableModel = lw->getCurSearchSqlTableModel();
//
//    if (0 == comptesBancairesTableModel)
//    {
//        comptesBancairesTableModel =  &_allWindows->getSqlTableModel_comptes_bancaires();
//    }
//    else if (0 != comptesBancairesTableModel &&
//             !YerothUtils::isEqualCaseInsensitive(comptesBancairesTableModel->sqlTableName(),
//                     YerothDatabase::COMPTES_BANCAIRES))
//    {
//        comptesBancairesTableModel =  &_allWindows->getSqlTableModel_comptes_bancaires();
//    }
//
//    QSqlRecord record = comptesBancairesTableModel->record(lw->lastSelectedItemForModification());
//
//    QString oldReferenceDuCompteBancaire(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::REFERENCE_DU_COMPTE_BANCAIRE));
//    QString newReferenceDuCompteBancaire(lineEdit_modifier_categorie_nom->text());
//
//    record.setValue(YerothDatabaseTableColumn::NOM_CATEGORIE, newNomCategorie);
//    record.setValue(YerothDatabaseTableColumn::DESCRIPTION_CATEGORIE, textEdit_modifier_categorie_description->toPlainText());
//
//    bool successCategorieTable = comptesBancairesTableModel->updateRecord(lw->lastSelectedItemForModification(), record);
//
//    if (successCategorieTable)
//    {
//        //Handling of table "stocks"
//    	QString stocksQuery(QString("UPDATE %1 SET %2='%3' WHERE %4='%5'")
//    							.arg(YerothDatabase::STOCKS,
//    								 YerothDatabaseTableColumn::CATEGORIE,
//									 newNomCategorie,
//									 YerothDatabaseTableColumn::CATEGORIE,
//									 oldNomCategorie));
//
//        YerothUtils::execQuery(stocksQuery, 0);
//    }
//
//    if (successCategorieTable)
//    {
//        ////Handling of table "stocks_vendu"
//    	QString stocksVenduQuery(QString("UPDATE %1 SET %2='%3' WHERE %4='%5'")
//    							.arg(YerothDatabase::STOCKS_VENDU,
//    								 YerothDatabaseTableColumn::CATEGORIE,
//									 newNomCategorie,
//									 YerothDatabaseTableColumn::CATEGORIE,
//									 oldNomCategorie));
//
//        YerothUtils::execQuery(stocksVenduQuery, 0);
//    }
//
//    if (successCategorieTable)
//    {
//        ////Handling of table "stocks_sorties"
//    	QString stocksVenduQuery(QString("UPDATE %1 SET %2='%3' WHERE %4='%5'")
//    							.arg(YerothDatabase::STOCKS_SORTIES,
//    								 YerothDatabaseTableColumn::CATEGORIE,
//									 newNomCategorie,
//									 YerothDatabaseTableColumn::CATEGORIE,
//									 oldNomCategorie));
//
//        YerothUtils::execQuery(stocksVenduQuery, 0);
//    }
//
//    if (successCategorieTable)
//    {
//        ////Handling of table "inventaire_des_stocks"
//    	QString stocksVenduQuery(QString("UPDATE %1 SET %2='%3' WHERE %4='%5'")
//    							.arg(YerothDatabase::MARCHANDISES,
//    								 YerothDatabaseTableColumn::CATEGORIE,
//									 newNomCategorie,
//									 YerothDatabaseTableColumn::CATEGORIE,
//									 oldNomCategorie));
//
//        YerothUtils::execQuery(stocksVenduQuery, 0);
//    }
//
//    QString retMsg(QObject::trUtf8("Les données de la catégorie '%1'")
//    					.arg(lineEdit_modifier_categorie_nom->text()));
//
//    if (successCategorieTable)
//    {
//        retMsg.append(QObject::trUtf8(" ont été modifiées avec succès !"));
//
//        YerothQMessageBox::information(this,
//                                 QObject::trUtf8("Yeroth-erp-3.0 ~ admin-modifier-catégorie"),
//                                 retMsg);
//
//        _allWindows->_adminListerWindow->rendreVisible(SUJET_ACTION_CATEGORIE);
//         rendreInvisible();
//    }
//    else
//    {
//        qDebug() << "\t[reason for failing] " << comptesBancairesTableModel->lastError();
//        retMsg.append(QObject::trUtf8(" n'ont pas pu être modifiées !"));
//        YerothQMessageBox::warning(this,
//                             QObject::trUtf8("Yeroth-erp-3.0 ~ admin-modifier-catégorie"),
//                             retMsg);
//    }
}

bool YerothAdminModifierWindow::modifier_compte_bancaire_check_fields()
{
    bool reference_du_compte_bancaire = lineEdit_modifier_compte_bancaire_reference_du_compte_bancaire->checkField();
    bool intitule_du_compte_bancaire = lineEdit_modifier_compte_bancaire_intitule_du_compte_bancaire->checkField();
    bool institut_bancaire = lineEdit_modifier_compte_bancaire_institut_bancaire->checkField();

    return reference_du_compte_bancaire &&
    	   intitule_du_compte_bancaire  &&
		   institut_bancaire;
}

void YerothAdminModifierWindow::clear_compte_bancaire_all_fields()
{
	lineEdit_modifier_compte_bancaire_institut_bancaire->clearField();
	lineEdit_modifier_compte_bancaire_intitule_du_compte_bancaire->clearField();
	lineEdit_modifier_compte_bancaire_institut_bancaire->clearField();

	textEdit_modifier_compte_bancaire_description_du_compte->clear();
}
