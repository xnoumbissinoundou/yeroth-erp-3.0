void YerothAdminModifierWindow::setupEditFournisseur()
{
    YerothAdminListerWindow *lw = _allWindows->_adminListerWindow;
    YerothSqlTableModel *fournisseursTableModel = lw->getCurSearchSqlTableModel();

    if (!fournisseursTableModel)
    {
        fournisseursTableModel =  &_allWindows->getSqlTableModel_fournisseurs();

        //qDebug() << "++ modifier Fournisseur: " << fournisseursTableModel->easySelect();
    }
    else if (fournisseursTableModel &&
             !YerothUtils::isEqualCaseInsensitive(fournisseursTableModel->sqlTableName(),
                     _allWindows->FOURNISSEURS))
    {
        fournisseursTableModel =  &_allWindows->getSqlTableModel_fournisseurs();
    }

    QSqlRecord record = fournisseursTableModel->record(lw->lastSelectedItemForModification());

    lineEdit_modifier_fournisseur_nom_entreprise->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::NOM_ENTREPRISE));
    lineEdit_modifier_fournisseur_nom_representant->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::NOM_REPRESENTANT));
    lineEdit_modifier_fournisseur_quartier->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::QUARTIER));
    lineEdit_modifier_fournisseur_ville->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::VILLE));
    lineEdit_modifier_fournisseur_pays->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::PAYS));
    lineEdit_modifier_fournisseur_siege_social->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::SIEGE_SOCIAL));
    lineEdit_modifier_fournisseur_boite_postale->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::BOITE_POSTALE));
    lineEdit_modifier_fournisseur_email->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::EMAIL));
    lineEdit_modifier_fournisseur_numero_telephone_1->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::NUMERO_TELEPHONE_1));
    lineEdit_modifier_fournisseur_numero_telephone_2->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::NUMERO_TELEPHONE_2));
    lineEdit_modifier_fournisseur_numero_contribuable->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::NUMERO_CONTRIBUABLE));
    textEdit_modifier_fournisseur_description->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::DESCRIPTION_FOURNISSEUR));
}

void YerothAdminModifierWindow::modifier_fournisseur()
{
    //_logger->log("modifier_fournisseur");
    if (modifier_fournisseur_check_fields())
    {
        YerothAdminListerWindow *lw = _allWindows->_adminListerWindow;
        YerothSqlTableModel *fournisseursTableModel = lw->getCurSearchSqlTableModel();

        if (!fournisseursTableModel)
        {
            fournisseursTableModel =  &_allWindows->getSqlTableModel_fournisseurs();
        }
        else if (fournisseursTableModel &&
                 !YerothUtils::isEqualCaseInsensitive(fournisseursTableModel->sqlTableName(),
                         _allWindows->FOURNISSEURS))
        {
            fournisseursTableModel =  &_allWindows->getSqlTableModel_fournisseurs();
        }

        QSqlRecord record = fournisseursTableModel->record(lw->lastSelectedItemForModification());

        record.setValue(YerothDatabaseTableColumn::NOM_ENTREPRISE, lineEdit_modifier_fournisseur_nom_entreprise->text());
        record.setValue(YerothDatabaseTableColumn::NOM_REPRESENTANT, lineEdit_modifier_fournisseur_nom_representant->text());
        record.setValue(YerothDatabaseTableColumn::QUARTIER, lineEdit_modifier_fournisseur_quartier->text());
        record.setValue(YerothDatabaseTableColumn::VILLE, lineEdit_modifier_fournisseur_ville->text());
        record.setValue(YerothDatabaseTableColumn::PAYS, lineEdit_modifier_fournisseur_pays->text());
        record.setValue(YerothDatabaseTableColumn::BOITE_POSTALE, lineEdit_modifier_fournisseur_boite_postale->text());
        record.setValue(YerothDatabaseTableColumn::EMAIL, lineEdit_modifier_fournisseur_email->text());
        record.setValue(YerothDatabaseTableColumn::SIEGE_SOCIAL, lineEdit_modifier_fournisseur_siege_social->text());
        record.setValue(YerothDatabaseTableColumn::NUMERO_TELEPHONE_1, lineEdit_modifier_fournisseur_numero_telephone_1->text());
        record.setValue(YerothDatabaseTableColumn::NUMERO_TELEPHONE_2, lineEdit_modifier_fournisseur_numero_telephone_2->text());
        record.setValue(YerothDatabaseTableColumn::NUMERO_CONTRIBUABLE, lineEdit_modifier_fournisseur_numero_contribuable->text());
        record.setValue(YerothDatabaseTableColumn::DESCRIPTION_FOURNISSEUR, textEdit_modifier_fournisseur_description->toPlainText());

        bool success = fournisseursTableModel->updateRecord(lw->lastSelectedItemForModification(), record);

        QString retMsg(QString(QObject::trUtf8("Les données du fournisseur '%1'"))
        					.arg(lineEdit_modifier_fournisseur_nom_entreprise->text()));

        if (success)
        {
            retMsg.append(QObject::trUtf8(" ont été modifiées avec succès !"));

            YerothQMessageBox::information(this,
                                     QObject::trUtf8("Yeroth-erp-3.0 ~ admin-modifier-fournisseur"),
                                     retMsg);

            _allWindows->_adminListerWindow->rendreVisible(SUJET_ACTION_FOURNISSEUR);

            this->rendreInvisible();
        }
        else
        {
            qDebug() << "\t[reason for failing] " << fournisseursTableModel->lastError();

            retMsg.append(QObject::trUtf8(" n'ont pas pu être modifiées !"));

            YerothQMessageBox::warning(this,
                                 QObject::tr("Yeroth-erp-3.0 ~ admin-modifier-fournisseur"),
                                 retMsg);
        }
    }
}

bool YerothAdminModifierWindow::modifier_fournisseur_check_fields()
{
    bool nom_entreprise = lineEdit_modifier_fournisseur_nom_entreprise->checkField();

    return nom_entreprise;
}

void YerothAdminModifierWindow::clear_fournisseur_all_fields()
{
    lineEdit_modifier_fournisseur_nom_entreprise->clearField();
    lineEdit_modifier_fournisseur_nom_representant->clearField();
    lineEdit_modifier_fournisseur_quartier->clear();
    lineEdit_modifier_fournisseur_ville->clear();
    lineEdit_modifier_fournisseur_pays->clear();
    lineEdit_modifier_fournisseur_boite_postale->clear();
    lineEdit_modifier_fournisseur_email->clear();
    lineEdit_modifier_fournisseur_siege_social->clear();
    lineEdit_modifier_fournisseur_email->clear();
    lineEdit_modifier_fournisseur_numero_telephone_1->clear();
    lineEdit_modifier_fournisseur_numero_telephone_2->clear();
    lineEdit_modifier_fournisseur_numero_contribuable->clear();
    textEdit_modifier_fournisseur_description->clear();
}
