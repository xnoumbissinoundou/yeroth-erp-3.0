
bool YerothAdminCreateWindow::creer_categorie()
{
    if (creer_categorie_check_fields())
    {
        QString retMsg("La catégorie '");

        YerothSqlTableModel &categoriesTableModel = _allWindows->getSqlTableModel_categories();

        categoriesTableModel.yerothSetFilter(QString("LOWER(nom_categorie) = LOWER('%1')")
                                       .arg(lineEdit_creer_categorie_nom->text()));

        //qDebug() << "++ filter: " << categoriesTableModel.filter();

        int categoriesTableModelRowCount = categoriesTableModel.easySelect();

        //qDebug() << "++ result categoriesTableModelRowCount : " << categoriesTableModelRowCount ;

        if (categoriesTableModelRowCount > 0)
        {
            QSqlRecord record = categoriesTableModel.record(0);
            QString duplicateCategorie(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::NOM_CATEGORIE));

            //qDebug() << "++ duplicate categorie: " << duplicateCategorie;

            retMsg.append(duplicateCategorie).append("' existe déjà !");

            YerothQMessageBox::warning(this,
                                 QObject::trUtf8("Yeroth-erp-3.0 ~ administration ~ créer ~ catégorie"),
                                 FROM_UTF8_STRING(retMsg));

            categoriesTableModel.resetFilter();

            return false;
        }

        categoriesTableModel.resetFilter();

        QSqlRecord record = categoriesTableModel.record();
        record.setValue(YerothDatabaseTableColumn::ID, _allWindows->getNextIdSqlTableModel_categories());
        record.setValue(YerothDatabaseTableColumn::NOM_CATEGORIE, lineEdit_creer_categorie_nom->text());
        record.setValue(YerothDatabaseTableColumn::DESCRIPTION_CATEGORIE, textEdit_creer_categorie_description->toPlainText());

        bool success = categoriesTableModel.insertNewRecord(record);

        if (!success)
        {
            retMsg.append(lineEdit_creer_categorie_nom->text())
            .append("' n'a pas pu être créer !");
            YerothQMessageBox::warning(this,
                                 QObject::trUtf8("Yeroth-erp-3.0 ~ admin-créer-catégorie"),
                                 FROM_UTF8_STRING(retMsg));
            return false;
        }

        retMsg.append(lineEdit_creer_categorie_nom->text())
        .append("' a été créer avec succès !");
        YerothQMessageBox::information(this,
                                 QObject::trUtf8("Yeroth-erp-3.0 ~ admin-créer-catégorie"),
                                 FROM_UTF8_STRING(retMsg));

        clear_categorie_all_fields();
        this->rendreInvisible();

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
