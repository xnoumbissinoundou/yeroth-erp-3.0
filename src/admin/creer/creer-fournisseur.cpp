
bool YerothAdminCreateWindow::creer_fournisseur()
{
    if (creer_fournisseur_check_fields())
    {
        QString retMsg("Le founisseur '");

        YerothSqlTableModel &fournisseursTableModel = _allWindows->getSqlTableModel_fournisseurs();

        fournisseursTableModel.yerothSetFilter(QString("LOWER(nom_entreprise) = LOWER('%1')")
                                         .arg(lineEdit_creer_fournisseur_nom_entreprise->text()));

        //qDebug() << "++ filter: " << fournisseursTableModel.filter();

        int fournisseursTableModelRowCount = fournisseursTableModel.easySelect();

        if (fournisseursTableModelRowCount > 0)
        {
            QSqlRecord record = fournisseursTableModel.record(0);
            QString duplicateFournisseur(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::NOM_ENTREPRISE));

            //qDebug() << "++ duplicate fournisseur: " << duplicateFournisseur;

            retMsg.append(duplicateFournisseur).append("' existe déjà !");

            YerothQMessageBox::warning(this,
                                 QObject::trUtf8("Yeroth-erp-3.0 ~ administration ~ créer ~ fournisseur"),
                                 FROM_UTF8_STRING(retMsg));

            fournisseursTableModel.resetFilter();

            return false;
        }

        fournisseursTableModel.resetFilter();

        QSqlRecord record = fournisseursTableModel.record();

        record.setValue(YerothDatabaseTableColumn::ID, _allWindows->getNextIdSqlTableModel_fournisseurs());
        record.setValue(YerothDatabaseTableColumn::NOM_ENTREPRISE, lineEdit_creer_fournisseur_nom_entreprise->text());
        record.setValue(YerothDatabaseTableColumn::NOM_REPRESENTANT, lineEdit_creer_fournisseur_nom_representant->text());
        record.setValue(YerothDatabaseTableColumn::QUARTIER, lineEdit_creer_fournisseur_quartier->text());
        record.setValue(YerothDatabaseTableColumn::VILLE, lineEdit_creer_fournisseur_ville->text());
        record.setValue(YerothDatabaseTableColumn::PAYS, lineEdit_creer_fournisseur_pays->text());
        record.setValue(YerothDatabaseTableColumn::BOITE_POSTALE, lineEdit_creer_fournisseur_boite_postale->text());
        record.setValue(YerothDatabaseTableColumn::EMAIL, lineEdit_creer_fournisseur_email->text());
        record.setValue(YerothDatabaseTableColumn::NUMERO_TELEPHONE_1, lineEdit_creer_fournisseur_numero_telephone_1->text());
        record.setValue(YerothDatabaseTableColumn::NUMERO_TELEPHONE_2, lineEdit_creer_fournisseur_numero_telephone_2->text());
        record.setValue(YerothDatabaseTableColumn::NUMERO_CONTRIBUABLE, lineEdit_creer_fournisseur_numero_contribuable->text());
        record.setValue(YerothDatabaseTableColumn::DESCRIPTION_FOURNISSEUR, textEdit_creer_fournisseur_description->toPlainText());

        retMsg.append(lineEdit_creer_fournisseur_nom_entreprise->text());

        bool success = fournisseursTableModel.insertNewRecord(record);

        if (!success)
        {
            retMsg.append("' n'a pas pu être créer !");
            YerothQMessageBox::warning(this,
                                 QObject::trUtf8("Yeroth-erp-3.0 ~ administration ~ créer ~ fournisseur"),
                                 FROM_UTF8_STRING(retMsg));
            return false;
        }

        retMsg.append("' a été créer avec succès !");
        YerothQMessageBox::information(this,
                                 QObject::trUtf8("Yeroth-erp-3.0 ~ administration ~ créer ~ fournisseur"),
                                 FROM_UTF8_STRING(retMsg));
        clear_fournisseur_all_fields();

        this->rendreInvisible();

        _allWindows->_adminListerWindow->rendreVisible(SUJET_ACTION_FOURNISSEUR);
    }

    return true;
}

bool YerothAdminCreateWindow::creer_fournisseur_check_fields()
{
    bool nom_entreprise = lineEdit_creer_fournisseur_nom_entreprise->checkField();

    return nom_entreprise;
}

void YerothAdminCreateWindow::clear_fournisseur_all_fields()
{
    lineEdit_creer_fournisseur_nom_entreprise->clearField();
    lineEdit_creer_fournisseur_nom_representant->clearField();
    lineEdit_creer_fournisseur_quartier->clear();
    lineEdit_creer_fournisseur_ville->clear();
    lineEdit_creer_fournisseur_pays->clear();
    lineEdit_creer_fournisseur_boite_postale->clear();
    lineEdit_creer_fournisseur_email->clear();
    lineEdit_creer_fournisseur_numero_telephone_1->clear();
    lineEdit_creer_fournisseur_numero_telephone_2->clear();
    lineEdit_creer_fournisseur_numero_contribuable->clear();
    textEdit_creer_fournisseur_description->clear();
}
