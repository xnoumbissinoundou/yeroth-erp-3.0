
void YerothAdminModifierWindow::setupEditCompteUtilisateur()
{
    YerothAdminListerWindow *lw = _allWindows->_adminListerWindow;
    YerothSqlTableModel *userTableModel = lw->getCurSearchSqlTableModel();

    if (!userTableModel)
    {
        userTableModel =  &_allWindows->getSqlTableModel_users();
    }
    else if (userTableModel &&
             !YerothUtils::isEqualCaseInsensitive(userTableModel->sqlTableName(),
                     _allWindows->USERS))
    {
        userTableModel =  &_allWindows->getSqlTableModel_users();
    }

    QSqlRecord record = userTableModel->record(lw->lastSelectedItemForModification());

    lineEdit_modifier_utilisateur_id->setText(GET_SQL_RECORD_DATA(record, "nom_utilisateur"));
    lineEdit_modifier_utilisateur_mot_passe_1->setText("*****");
    lineEdit_modifier_utilisateur_mot_passe_2->setText("*****");
    lineEdit_modifier_utilisateur_prenom->setText(GET_SQL_RECORD_DATA(record, "prenom"));
    lineEdit_modifier_utilisateur_nom->setText(GET_SQL_RECORD_DATA(record, "nom"));

    QString date_naissance(GET_SQL_RECORD_DATA(record, "date_naissance"));
    dateEdit_modifier_utilisateur_date_naissance->setDate(GET_DATE_FROM_STRING(date_naissance));

    lineEdit_modifier_utilisateur_lieu_naissance->setText(GET_SQL_RECORD_DATA(record, "lieu_naissance"));
    lineEdit_modifier_utilisateur_ville->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::VILLE));
    lineEdit_modifier_utilisateur_province_etat->setText(GET_SQL_RECORD_DATA(record, "province_etat"));
    lineEdit_modifier_utilisateur_pays->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::PAYS));
    lineEdit_modifier_utilisateur_boite_postale->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::BOITE_POSTALE));
    lineEdit_modifier_utilisateur_email->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::EMAIL));
    lineEdit_modifier_utilisateur_numero_telephone_1->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::NUMERO_TELEPHONE_1));
    lineEdit_modifier_utilisateur_numero_telephone_2->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::NUMERO_TELEPHONE_2));

    comboBox_modifier_utilisateur_titre->populateComboBoxMissing(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::TITRE).toInt());

	comboBox_modifier_utilisateur_role->populateComboBoxMissing(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::ROLE).toInt());

    lineEdit_modifier_utilisateur_localisation->
    setText(_allWindows->getInfoEntreprise().getLocalisation());

}

void YerothAdminModifierWindow::modifier_utilisateur()
{
	if (modifier_utilisateur_check_fields())
	{
		YerothUtils::startTransaction();
		modifier_utilisateur_main();
		YerothUtils::commitTransaction();
	}
}

void YerothAdminModifierWindow::modifier_utilisateur_main()
{
    if (modifier_utilisateur_check_fields())
    {
        YerothAdminListerWindow *lw = _allWindows->_adminListerWindow;
        YerothSqlTableModel *userTableModel = lw->getCurSearchSqlTableModel();

        if (!userTableModel)
        {
            userTableModel =  &_allWindows->getSqlTableModel_users();
        }
        else if (userTableModel &&
                 !YerothUtils::isEqualCaseInsensitive(userTableModel->sqlTableName(),
                         _allWindows->USERS))
        {
            userTableModel =  &_allWindows->getSqlTableModel_users();
        }

        QSqlRecord record = userTableModel->record(lw->lastSelectedItemForModification());

        record.setValue("prenom", lineEdit_modifier_utilisateur_prenom->text());
        record.setValue("nom", lineEdit_modifier_utilisateur_nom->text());

        comboBox_modifier_utilisateur_titre->saveCurrentValueToDatabase(YerothDatabaseTableColumn::TITRE, record);

        record.setValue(YerothDatabaseTableColumn::LIEU_NAISSANCE, lineEdit_modifier_utilisateur_lieu_naissance->text());
        record.setValue(YerothDatabaseTableColumn::DATE_NAISSANCE, dateEdit_modifier_utilisateur_date_naissance->date());
        record.setValue(YerothDatabaseTableColumn::VILLE, lineEdit_modifier_utilisateur_ville->text());
        record.setValue(YerothDatabaseTableColumn::PROVINCE_ETAT, lineEdit_modifier_utilisateur_province_etat->text());
        record.setValue(YerothDatabaseTableColumn::PAYS, lineEdit_modifier_utilisateur_pays->text());
        record.setValue(YerothDatabaseTableColumn::BOITE_POSTALE, lineEdit_modifier_utilisateur_boite_postale->text());
        record.setValue(YerothDatabaseTableColumn::EMAIL, lineEdit_modifier_utilisateur_email->text());
        record.setValue(YerothDatabaseTableColumn::NUMERO_TELEPHONE_1, lineEdit_modifier_utilisateur_numero_telephone_1->text());
        record.setValue(YerothDatabaseTableColumn::NUMERO_TELEPHONE_2, lineEdit_modifier_utilisateur_numero_telephone_2->text());

        comboBox_modifier_utilisateur_role->saveCurrentValueToDatabase(YerothDatabaseTableColumn::ROLE, record);

        record.setValue("localisation", lineEdit_modifier_utilisateur_localisation->text());

        QString nom_complet(lineEdit_modifier_utilisateur_prenom->text());
        nom_complet.append(" ").append(lineEdit_modifier_utilisateur_nom->text());

        record.setValue("nom_complet", nom_complet);

        if (_savePassword)
        {
            QString mot_passe_1(lineEdit_modifier_utilisateur_mot_passe_1->text());

            mot_passe_1.append(lineEdit_modifier_utilisateur_id->text().toLower());

            QByteArray md5Hash_mot_passe(MD5_HASH(mot_passe_1));

            //qDebug() << "++ modifier_utilisateur: " << md5Hash_mot_passe;
            record.setValue("mot_passe", md5Hash_mot_passe);
        }

        bool success = userTableModel->updateRecord(lw->lastSelectedItemForModification(), record);

        QString retMsg(QString(QObject::trUtf8("Les données de l'utilisateur '%1'"))
        					.arg(lineEdit_modifier_utilisateur_id->text()));

        if (success)
        {
            retMsg.append(QObject::trUtf8(" ont été modifiées avec succès !"));

            YerothQMessageBox::information(this,
                                     QObject::tr("Yeroth-erp-3.0 ~ admin-modifier-utilisateur"),
                                     retMsg);

            _allWindows->_adminListerWindow->rendreVisible(SUJET_ACTION_COMPTE_UTILISATEUR);
            this->rendreInvisible();
        }
        else
        {
            qDebug() << QString("\t[reason for failing] %1")
            				.arg(userTableModel->lastError().text());

            retMsg.append(QObject::trUtf8(" n'ont pas pu être modifiées !"));

            YerothQMessageBox::warning(this,
                                 	   QObject::tr("Yeroth-erp-3.0 ~ admin-modifier-utilisateur"),
									   retMsg);
        }
    }
}

bool YerothAdminModifierWindow::modifier_utilisateur_check_fields()
{
    QString mot_passe_1(lineEdit_modifier_utilisateur_mot_passe_1->text());
    QString mot_passe_2(lineEdit_modifier_utilisateur_mot_passe_2->text());

    if (mot_passe_1 != "*****" || mot_passe_2 != "*****")
    {
        if (mot_passe_1 != mot_passe_2)
        {
            QString msg(QObject::trUtf8("Les deux mot de passe sont différents !"));

            YerothQMessageBox::warning(this,
            						   QObject::trUtf8("Yeroth-erp-3.0 -  administration ~ modifier ~ utilisateur"),
									   msg,
									   QMessageBox::Ok);

            lineEdit_modifier_utilisateur_mot_passe_1->setPalette(YerothUtils::YEROTH_RED_PALETTE);
            lineEdit_modifier_utilisateur_mot_passe_2->setPalette(YerothUtils::YEROTH_RED_PALETTE);

            _savePassword = false;

            return false;
        }
        else
        {
            _savePassword = true;
        }
    }

    bool nom = lineEdit_modifier_utilisateur_nom->checkField();
    bool prenom = lineEdit_modifier_utilisateur_prenom->checkField();
    bool role = comboBox_modifier_utilisateur_role->checkField();

    return nom 				&&
           prenom  			&&
           role;
}

void YerothAdminModifierWindow::clear_utilisateur_all_fields()
{
    lineEdit_modifier_utilisateur_nom->clearField();
    lineEdit_modifier_utilisateur_prenom->clearField();
    lineEdit_modifier_utilisateur_lieu_naissance->clearField();
    lineEdit_modifier_utilisateur_numero_telephone_1->clearField();
    lineEdit_modifier_utilisateur_numero_telephone_2->clearField();
    lineEdit_modifier_utilisateur_id->clearField();
    lineEdit_modifier_utilisateur_ville->clearField();
    lineEdit_modifier_utilisateur_province_etat->clearField();
    lineEdit_modifier_utilisateur_pays->clearField();
    lineEdit_modifier_utilisateur_boite_postale->clearField();
    lineEdit_modifier_utilisateur_email->clearField();
    lineEdit_modifier_utilisateur_mot_passe_1->clearField();
    lineEdit_modifier_utilisateur_mot_passe_2->clearField();
    lineEdit_modifier_utilisateur_localisation->clearField();

    comboBox_modifier_utilisateur_titre->clear();
    comboBox_modifier_utilisateur_role->clear();
}

