
void YerothAdminModifierWindow::setupEditLocalisation()
{
    YerothAdminListerWindow *lw = _allWindows->_adminListerWindow;
    YerothSqlTableModel *localisationsTableModel = lw->getCurSearchSqlTableModel();

    if (!localisationsTableModel)
    {
        localisationsTableModel =  &_allWindows->getSqlTableModel_localisations();
    }
    else if (localisationsTableModel &&
             !YerothUtils::isEqualCaseInsensitive(localisationsTableModel->sqlTableName(),
                     _allWindows->LOCALISATIONS))
    {
        localisationsTableModel =  &_allWindows->getSqlTableModel_localisations();
    }

    QSqlRecord record = localisationsTableModel->record(lw->lastSelectedItemForModification());

    lineEdit_modifier_localisation_adresse_ip->setText(GET_SQL_RECORD_DATA(record, "adresse_ip"));
    lineEdit_modifier_localisation_nom->setText(GET_SQL_RECORD_DATA(record, "nom_localisation"));
    lineEdit_modifier_localisation_numero_unique->setText(GET_SQL_RECORD_DATA(record, "numero_unique"));
    lineEdit_modifier_localisation_quartier->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::QUARTIER));
    lineEdit_modifier_localisation_ville->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::VILLE));
    lineEdit_modifier_localisation_province_etat->setText(GET_SQL_RECORD_DATA(record, "province_etat"));
    lineEdit_modifier_localisation_pays->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::PAYS));
    lineEdit_modifier_localisation_boite_postale->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::BOITE_POSTALE));

    QString date_ouverture(GET_SQL_RECORD_DATA(record, "date_ouverture"));
    dateEdit_modifier_localisation_date_ouverture->setDate(GET_DATE_FROM_STRING(date_ouverture));
    dateEdit_modifier_localisation_date_ouverture->setEnabled(false);

    lineEdit_modifier_localisation_email->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::EMAIL));
    lineEdit_modifier_localisation_numero_telephone_1->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::NUMERO_TELEPHONE_1));
    lineEdit_modifier_localisation_numero_telephone_2->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::NUMERO_TELEPHONE_2));

    POPULATE_COMBOBOX_MISSING(comboBox_modifier_localisation_base_donnees,
                              GET_SQL_RECORD_DATA(record, "dbms"),
                              _allWindows->DBMS,
                              "nom_dbms");

    textEdit_modifier_localisation_description_lieu->setText(GET_SQL_RECORD_DATA(record, "description_lieu"));
}

void YerothAdminModifierWindow::modifier_localisation()
{
    //_logger->log("modifier_localisation");
    if (modifier_localisation_check_fields())
    {
        YerothAdminListerWindow *lw = _allWindows->_adminListerWindow;
        YerothSqlTableModel *localisationsTableModel = lw->getCurSearchSqlTableModel();

        if (!localisationsTableModel)
        {
            localisationsTableModel =  &_allWindows->getSqlTableModel_localisations();
        }
        else if (localisationsTableModel &&
                 !YerothUtils::isEqualCaseInsensitive(localisationsTableModel->sqlTableName(),
                         _allWindows->LOCALISATIONS))
        {
            localisationsTableModel =  &_allWindows->getSqlTableModel_localisations();
        }

        QSqlRecord record = localisationsTableModel->record(lw->lastSelectedItemForModification());

        record.setValue("adresse_ip", lineEdit_modifier_localisation_adresse_ip->text());
        record.setValue(YerothDatabaseTableColumn::QUARTIER, lineEdit_modifier_localisation_quartier->text());
        record.setValue(YerothDatabaseTableColumn::VILLE, lineEdit_modifier_localisation_ville->text());
        record.setValue("province_etat", lineEdit_modifier_localisation_province_etat->text());
        record.setValue(YerothDatabaseTableColumn::PAYS, lineEdit_modifier_localisation_pays->text());
        record.setValue(YerothDatabaseTableColumn::BOITE_POSTALE, lineEdit_modifier_localisation_boite_postale->text());
        record.setValue(YerothDatabaseTableColumn::EMAIL, lineEdit_modifier_localisation_email->text());
        record.setValue(YerothDatabaseTableColumn::NUMERO_TELEPHONE_1, lineEdit_modifier_localisation_numero_telephone_1->text());
        record.setValue(YerothDatabaseTableColumn::NUMERO_TELEPHONE_2, lineEdit_modifier_localisation_numero_telephone_2->text());
        record.setValue("dbms", comboBox_modifier_localisation_base_donnees->currentText());
        record.setValue("description_lieu", textEdit_modifier_localisation_description_lieu->toPlainText());
        record.setValue("numero_unique", lineEdit_modifier_localisation_numero_unique->text());

        bool success = localisationsTableModel->updateRecord(lw->lastSelectedItemForModification(), record);

        QString retMsg(QString(QObject::trUtf8("Les données de la localisation '%1'"))
        					.arg(lineEdit_modifier_localisation_nom->text()));

        if (success)
        {
            retMsg.append(QObject::trUtf8(" ont été modifiées avec succès !"));

            YerothQMessageBox::information(this,
                                     QObject::tr("Yeroth-erp-3.0 ~ admin-modifier-localisation"),
                                     retMsg);

            _allWindows->_adminListerWindow->rendreVisible(SUJET_ACTION_LOCALISATION);

            this->rendreInvisible();
        }
        else
        {
            qDebug() << "\t[reason for failing] " << localisationsTableModel->lastError();

            retMsg.append(QObject::trUtf8(" n'ont pas pu être modifiées !"));

            YerothQMessageBox::warning(this,
                                 QObject::tr("Yeroth-erp-3.0 ~ admin-modifier-localisation"),
                                 retMsg);
        }
    }
}

void YerothAdminModifierWindow::clear_localisation_all_fields()
{
    lineEdit_modifier_localisation_adresse_ip->clearField();
    lineEdit_modifier_localisation_nom->clearField();
    lineEdit_modifier_localisation_quartier->clearField();
    lineEdit_modifier_localisation_ville->clearField();
    lineEdit_modifier_localisation_province_etat->clearField();
    lineEdit_modifier_localisation_pays->clearField();
    lineEdit_modifier_localisation_boite_postale->clearField();
    dateEdit_modifier_localisation_date_ouverture->clear();
    lineEdit_modifier_localisation_email->clearField();
    lineEdit_modifier_localisation_numero_telephone_1->clearField();
    lineEdit_modifier_localisation_numero_telephone_2->clearField();
    comboBox_modifier_localisation_base_donnees->clearField();
    textEdit_modifier_localisation_description_lieu->clear();
}

bool YerothAdminModifierWindow::modifier_localisation_check_fields()
{
    bool nom = lineEdit_modifier_localisation_nom->checkField();
    bool quartier = lineEdit_modifier_localisation_quartier->checkField();
    bool ville = lineEdit_modifier_localisation_ville->checkField();

    return nom &&
           quartier  &&
           ville;
}
