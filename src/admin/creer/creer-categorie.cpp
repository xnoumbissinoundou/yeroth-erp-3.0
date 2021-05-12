
bool YerothAdminCreateWindow::creer_categorie()
{
    if (creer_categorie_check_fields())
    {
        QString retMsg(QObject::trUtf8("La catégorie '"));

        YerothSqlTableModel &categoriesTableModel = _allWindows->getSqlTableModel_categories();

        categoriesTableModel.yerothSetFilter_WITH_where_clause(QString("LOWER(nom_categorie) = LOWER('%1')")
                                       .arg(lineEdit_creer_categorie_nom->text()));

        //qDebug() << "++ filter: " << categoriesTableModel.filter();

        int categoriesTableModelRowCount = categoriesTableModel.easySelect();

        //qDebug() << "++ result categoriesTableModelRowCount : " << categoriesTableModelRowCount ;

        if (categoriesTableModelRowCount > 0)
        {
            QSqlRecord record = categoriesTableModel.record(0);
            QString duplicateCategorie(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::NOM_CATEGORIE));

            //qDebug() << "++ duplicate categorie: " << duplicateCategorie;

            retMsg.append(QObject::trUtf8("%1' existe déjà ! ")
            				.arg(duplicateCategorie));

            YerothQMessageBox::warning(this,
                                 QObject::trUtf8("Yeroth-pgi-3.0 ~ administration ~ créer ~ catégorie"),
                                 retMsg);

            categoriesTableModel.resetFilter();

            return false;
        }

        categoriesTableModel.resetFilter();

        QSqlRecord record = categoriesTableModel.record();
        record.setValue(YerothDatabaseTableColumn::ID, YerothERPWindows::getNextIdSqlTableModel_categories());
        record.setValue(YerothDatabaseTableColumn::NOM_DEPARTEMENT_PRODUIT, lineEdit_creer_categorie_nom_departement_produit->text());
        record.setValue(YerothDatabaseTableColumn::NOM_CATEGORIE, lineEdit_creer_categorie_nom->text());
        record.setValue(YerothDatabaseTableColumn::DESCRIPTION_CATEGORIE, textEdit_creer_categorie_description->toPlainText());

        bool success = categoriesTableModel.insertNewRecord(record);

        if (!success)
        {
            retMsg.append(QObject::trUtf8("%1' n'a pas pu être créer ! ")
            				.arg(lineEdit_creer_categorie_nom->text()));

            YerothQMessageBox::warning(this,
                                 QObject::trUtf8("Yeroth-pgi-3.0 ~ admin-créer-catégorie"),
                                 retMsg);
            return false;
        }

        retMsg.append(QObject::trUtf8("%1' a été créer avec succès ! ")
        				.arg(lineEdit_creer_categorie_nom->text()));

        YerothQMessageBox::information(this,
                                 QObject::trUtf8("Yeroth-pgi-3.0 ~ admin-créer-catégorie"),
                                 retMsg);

        clear_categorie_all_fields();
         rendreInvisible();

        _allWindows->_adminListerWindow->rendreVisible(SUJET_ACTION_CATEGORIE);

        return true;
    }

    return false;
}

bool YerothAdminCreateWindow::creer_categorie_check_fields()
{
    bool nom = lineEdit_creer_categorie_nom->checkField();
    return nom;
}

void YerothAdminCreateWindow::clear_categorie_all_fields()
{
    lineEdit_creer_categorie_nom->clearField();
    textEdit_creer_categorie_description->clear();
}
