
void YerothAdminModifierWindow::setupEditAlerte()
{
    YerothAdminListerWindow *lw = _allWindows->_adminListerWindow;
    YerothSqlTableModel *alertesTableModel = lw->getCurSearchSqlTableModel();

    if (!alertesTableModel)
    {
        alertesTableModel =  &_allWindows->getSqlTableModel_alertes();
    }
    else if (alertesTableModel &&
             !YerothUtils::isEqualCaseInsensitive(alertesTableModel->sqlTableName(),
                     YerothDatabase::ALERTES))
    {
        alertesTableModel =  &_allWindows->getSqlTableModel_alertes();
    }

    QSqlRecord record = alertesTableModel->record(lw->lastSelectedItemForModification());

    lineEdit_modifier_alerte_designation_alerte->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::DESIGNATION_ALERTE));
    textEdit_modifier_alerte_message->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::MESSAGE_ALERTE));

    QString date_debut(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::DATE_DEBUT));
    QString date_fin(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::DATE_FIN));

    dateEdit_modifier_alerte_date_debut->setDate(GET_DATE_FROM_STRING(date_debut));
    dateEdit_modifier_alerte_date_fin->setDate(GET_DATE_FROM_STRING(date_fin));

    radioButton_modifier_alerte_periode_temps->setEnabled(false);
    radioButton_modifier_alerte_quantite->setEnabled(false);

    if (GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::QUANTITE).toInt() > -1)
    {
        lineEdit_modifier_alerte_quantite->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::QUANTITE));
        radioButton_modifier_alerte_quantite->setChecked(true);
        radioButton_modifier_alerte_periode_temps->setChecked(false);
    }
    else
    {
        lineEdit_modifier_alerte_quantite->setText("-1");
        radioButton_modifier_alerte_periode_temps->setChecked(true);
        radioButton_modifier_alerte_quantite->setChecked(false);
    }

    lineEdit_modifier_alerte_designation_article->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::DESIGNATION));
    lineEdit_modifier_alerte_designation_article->setEnabled(false);

    lineEdit_modifier_alerte_id_destinataire->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::DESTINATAIRE));
    lineEdit_modifier_alerte_id_destinataire->setEnabled(false);

    lineEdit_modifier_alerte_nom_destinataire->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::NOM_COMPLET_DESTINATAIRE));

    comboBox_modifier_alerte_condition->addItem(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::CONDITION_ALERTE));
    comboBox_modifier_alerte_condition->setEnabled(false);
}

void YerothAdminModifierWindow::modifier_alerte()
{
    //_logger->log("modifier_alerte");
    YerothAdminListerWindow *lw = _allWindows->_adminListerWindow;
    YerothSqlTableModel *alertesTableModel = lw->getCurSearchSqlTableModel();

    if (!alertesTableModel)
    {
        alertesTableModel =  &_allWindows->getSqlTableModel_alertes();
    }
    else if (alertesTableModel &&
             !YerothUtils::isEqualCaseInsensitive(alertesTableModel->sqlTableName(),
                     YerothDatabase::ALERTES))
    {
        alertesTableModel =  &_allWindows->getSqlTableModel_alertes();
    }

    QSqlRecord record = alertesTableModel->record(lw->lastSelectedItemForModification());

    if (modifier_alerte_check_fields(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::MESSAGE_ALERTE)))
    {
        record.setValue(YerothDatabaseTableColumn::MESSAGE_ALERTE, textEdit_modifier_alerte_message->toPlainText());

        bool success = alertesTableModel->updateRecord(lw->lastSelectedItemForModification(), record);

        QString retMsg(QObject::trUtf8("Le message de l'alerte '%1' ")
        					.arg(lineEdit_modifier_alerte_designation_alerte->text()));

        if (success)
        {
            retMsg.append(QObject::trUtf8("a été modifiées avec succès !"));

            YerothQMessageBox::information(this,
                                     QObject::trUtf8("Yeroth-erp-3.0 ~ administration ~ modifier ~ alerte"),
                                     retMsg);

            _allWindows->_adminListerWindow->rendreVisible(SUJET_ACTION_ALERTE);
             rendreInvisible();
        }
        else
        {
            qDebug() << "\t[reason for failing] " << alertesTableModel->lastError();

            retMsg.append(QObject::trUtf8(" n'a pas pu être modifiées !"));

            YerothQMessageBox::warning(this,
                                 QObject::trUtf8("Yeroth-erp-3.0 ~ administration ~ modifier ~ alerte"),
                                 retMsg);
        }
    }
}



bool YerothAdminModifierWindow::modifier_alerte_check_fields(QString previousAlertText)
{
    QString currentText(textEdit_modifier_alerte_message->toPlainText());
    bool modified = (previousAlertText != currentText);
    if (!modified)
    {
        YerothQMessageBox::information(this,
                                 QObject::trUtf8("Yeroth-erp-3.0 ~ administration ~ modifier ~ alerte"),
                                 QObject::trUtf8("Le message de l'alerte n'a pas changé!"));
        return false;
    }

    return true;
}

void YerothAdminModifierWindow::clear_alerte_all_fields()
{
    textEdit_modifier_alerte_message->clear();
    lineEdit_modifier_alerte_designation_article->clear();
    comboBox_modifier_alerte_condition->clear();
    lineEdit_modifier_alerte_id_destinataire->clear();
}
