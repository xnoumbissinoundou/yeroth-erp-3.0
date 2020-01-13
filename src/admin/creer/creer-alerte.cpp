bool YerothAdminCreateWindow::creer_alerte()
{
    if (creer_alerte_check_fields())
    {
        QString retMsg(QObject::tr("Une alerte au nom de '"));

        YerothSqlTableModel &alertesTableModel = _allWindows->getSqlTableModel_alertes();

        alertesTableModel.yerothSetFilter(QString("LOWER(designation_alerte) = LOWER('%1')")
                                    .arg(lineEdit_creer_alerte_nom->text()));

        //qDebug() << "++ filter: " << alertesTableModel.filter();

        int alertesTableModelRowCount = alertesTableModel.easySelect();

        //qDebug() << "++ result alertesTableModelRowCount : " << alertesTableModelRowCount ;

        if (alertesTableModelRowCount > 0)
        {
            QSqlRecord record = alertesTableModel.record(0);
            QString duplicateAlerte(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::DESIGNATION_ALERTE));

            //qDebug() << "++ duplicate alerte: " << duplicateAlerte;

            retMsg.append(QString(QObject::trUtf8("%1' existe déjà ! "))
            				.arg(duplicateAlerte));

            YerothQMessageBox::warning(this,
                                 QObject::trUtf8("Yeroth-erp-3.0 ~ administration ~ créer ~ alerte"),
                                 retMsg);

            alertesTableModel.resetFilter();

            return false;
        }

        alertesTableModel.resetFilter();

        QSqlRecord record = alertesTableModel.record();

        YerothSqlTableModel &stocksTableModel = _allWindows->getSqlTableModel_stocks();
        stocksTableModel.yerothSetFilter(GENERATE_SQL_IS_STMT(YerothDatabaseTableColumn::DESIGNATION, lineEdit_creer_alerte_designation->text()));
        int stocksTableModelRowCount = stocksTableModel.easySelect();
        if (stocksTableModelRowCount <= 0)
        {
            YerothQMessageBox::warning(this,
                                 QObject::trUtf8("Yeroth-erp-3.0 ~ administration ~ créer ~ alerte"),
                                 QObject::trUtf8("Il n'est pas possible de créer une alerte "
                                                 "sur un stock déjà écoulé !"));

            return false;
        }

        QSqlRecord stocksRecord = stocksTableModel.record(0);

        record.setValue(YerothDatabaseTableColumn::ID, _allWindows->getNextIdSqlTableModel_alertes());
        record.setValue(YerothDatabaseTableColumn::DESIGNATION_ALERTE, lineEdit_creer_alerte_nom->text());
        record.setValue(YerothDatabaseTableColumn::DESTINATAIRE, lineEdit_creer_alerte_destinataire->text());
        record.setValue(YerothDatabaseTableColumn::NOM_COMPLET_DESTINATAIRE, lineEdit_creer_alerte_nom_destinataire->text());
        record.setValue(YerothDatabaseTableColumn::DESIGNATION, lineEdit_creer_alerte_designation->text());
        record.setValue(YerothDatabaseTableColumn::STOCKS_ID, GET_SQL_RECORD_DATA(stocksRecord, YerothDatabaseTableColumn::ID));
        record.setValue(YerothDatabaseTableColumn::ALERTE_RESOLUE, 0);

        QString quantite = lineEdit_creer_alerte_quantite->text();

        if (radioButton_creer_alerte_quantite->isChecked())
        {
            record.setValue(YerothDatabaseTableColumn::QUANTITE, quantite);
            record.setValue(YerothDatabaseTableColumn::CONDITION_ALERTE, comboBox_creer_alerte_condition->currentText());
        }
        else if (radioButton_creer_alerte_periode_temps->isChecked())
        {
            quantite = "-1";
            record.setValue(YerothDatabaseTableColumn::QUANTITE, quantite);
            record.setValue(YerothDatabaseTableColumn::DATE_DEBUT, dateEdit_creer_alerte_date_debut->date());
            record.setValue(YerothDatabaseTableColumn::DATE_FIN, dateEdit_creer_alerte_date_fin->date());
        }

        record.setValue(YerothDatabaseTableColumn::MESSAGE_ALERTE, textEdit_creer_alerte_message->toPlainText());
        record.setValue(YerothDatabaseTableColumn::STATUT_RESOLU, false);
        record.setValue(YerothDatabaseTableColumn::DATE_CREATION, GET_CURRENT_DATE);

        bool success = alertesTableModel.insertNewRecord(record);

        if (!success)
        {
        	retMsg.append(QString(QObject::trUtf8("%1' n'a pas pu être créer !"))
        					.arg(lineEdit_creer_alerte_nom->text()));

            YerothQMessageBox::warning(this,
                                 QObject::trUtf8("Yeroth-erp-3.0 - créer"),
                                 retMsg);
            return false;
        }

        retMsg.append(QString(QObject::trUtf8("%1' a été créer avec succès !"))
    					.arg(lineEdit_creer_alerte_nom->text()));

        YerothQMessageBox::information(this,
        							   QObject::trUtf8("Yeroth-erp-3.0 - créer"),
                                 	   retMsg);

        this->clear_alerte_all_fields();
        this->rendreInvisible();

        stocksTableModel.resetFilter();

        _allWindows->_adminListerWindow->rendreVisible(SUJET_ACTION_ALERTE);

        return true;
    }

    return false;
}

void YerothAdminCreateWindow::populateAlerteComboBoxes()
{
    _logger->log("populateAlerteComboBoxes");

    YerothUtils::populateComboBox(*comboBox_creer_alerte_condition,
                                 _allWindows->CONDITIONS_ALERTES, YerothDatabaseTableColumn::CONDITION_ALERTE);
}


void YerothAdminCreateWindow::creer_alerte_check_fields_entry()
{
    bool alerte_nom = lineEdit_creer_alerte_nom->checkField();
    bool produit = lineEdit_creer_alerte_designation->checkField();
    bool message = textEdit_creer_alerte_message->checkField();
    bool destinataire = lineEdit_creer_alerte_destinataire->checkField();

    if (!radioButton_creer_alerte_quantite->isChecked() &&
            !radioButton_creer_alerte_periode_temps->isChecked())
    {
        comboBox_creer_alerte_condition->checkField();
        lineEdit_creer_alerte_quantite->setPalette(YerothUtils::YEROTH_RED_PALETTE);
        dateEdit_creer_alerte_date_debut->setPalette(YerothUtils::YEROTH_RED_PALETTE);
        dateEdit_creer_alerte_date_fin->setPalette(YerothUtils::YEROTH_RED_PALETTE);
    }
    else
    {
        comboBox_creer_alerte_condition->checkField();
        lineEdit_creer_alerte_quantite->setPalette(YerothUtils::YEROTH_WHITE_PALETTE);
        dateEdit_creer_alerte_date_debut->setPalette(YerothUtils::YEROTH_WHITE_PALETTE);
        dateEdit_creer_alerte_date_fin->setPalette(YerothUtils::YEROTH_WHITE_PALETTE);
    }

    if (radioButton_creer_alerte_quantite->isChecked())
    {
        dateEdit_creer_alerte_date_debut->setPalette(YerothUtils::YEROTH_WHITE_PALETTE);
        dateEdit_creer_alerte_date_fin->setPalette(YerothUtils::YEROTH_WHITE_PALETTE);

        bool quantite = lineEdit_creer_alerte_quantite->checkField();

        bool condition = comboBox_creer_alerte_condition->checkField();
    }
    else if (radioButton_creer_alerte_periode_temps->isChecked())
    {
        comboBox_creer_alerte_condition->setPalette(YerothUtils::YEROTH_WHITE_PALETTE);
        lineEdit_creer_alerte_quantite->setPalette(YerothUtils::YEROTH_WHITE_PALETTE);

        if (dateEdit_creer_alerte_date_debut->date() < QDate::currentDate())
        {
            dateEdit_creer_alerte_date_debut->setPalette(YerothUtils::YEROTH_RED_PALETTE);
            dateEdit_creer_alerte_date_fin->setPalette(YerothUtils::YEROTH_RED_PALETTE);
        }

        bool periode =
            (dateEdit_creer_alerte_date_debut->date() <= dateEdit_creer_alerte_date_fin->date());

        if (!periode)
        {
            dateEdit_creer_alerte_date_debut->setPalette(YerothUtils::YEROTH_RED_PALETTE);
            dateEdit_creer_alerte_date_fin->setPalette(YerothUtils::YEROTH_RED_PALETTE);
        }
    }
}


bool YerothAdminCreateWindow::creer_alerte_check_fields()
{
    bool alerte_nom = lineEdit_creer_alerte_nom->checkField();
    bool produit = lineEdit_creer_alerte_designation->checkField();
    bool message = textEdit_creer_alerte_message->checkField();
    bool destinataire = lineEdit_creer_alerte_destinataire->checkField();

    bool check =  alerte_nom 	&&
                  produit 		&&
                  message 		&&
                  destinataire;

    if (!radioButton_creer_alerte_quantite->isChecked() &&
            !radioButton_creer_alerte_periode_temps->isChecked())
    {
        QString aMsg(QObject::trUtf8("Veuillez choisir comme paramètre de l'alerte "
        							 "soit la 'quantité', soit la 'période de temps' !"));

        YerothQMessageBox::information(this,
                                 QObject::trUtf8("Yeroth-erp-3.0 - créer ~ alerte"),
                                 aMsg);

        comboBox_creer_alerte_condition->checkField();
        lineEdit_creer_alerte_quantite->setPalette(YerothUtils::YEROTH_RED_PALETTE);
        dateEdit_creer_alerte_date_debut->setPalette(YerothUtils::YEROTH_RED_PALETTE);
        dateEdit_creer_alerte_date_fin->setPalette(YerothUtils::YEROTH_RED_PALETTE);

        return false;
    }
    else
    {
        comboBox_creer_alerte_condition->checkField();
        lineEdit_creer_alerte_quantite->setPalette(YerothUtils::YEROTH_WHITE_PALETTE);
        dateEdit_creer_alerte_date_debut->setPalette(YerothUtils::YEROTH_WHITE_PALETTE);
        dateEdit_creer_alerte_date_fin->setPalette(YerothUtils::YEROTH_WHITE_PALETTE);
    }

    if (radioButton_creer_alerte_quantite->isChecked())
    {
        dateEdit_creer_alerte_date_debut->setPalette(YerothUtils::YEROTH_WHITE_PALETTE);
        dateEdit_creer_alerte_date_fin->setPalette(YerothUtils::YEROTH_WHITE_PALETTE);

        bool quantite = lineEdit_creer_alerte_quantite->checkField();
        if (quantite)
        {
            int q = lineEdit_creer_alerte_quantite->text().toInt();
            if (q < 0)
            {
                QString qMsg(QObject::trUtf8("La quantité doit être supérieure à 0 !"));

                YerothQMessageBox::information(this,
                                         QObject::trUtf8("Yeroth-erp-3.0 - créer-alerte"),
                                         qMsg);
                return false;
            }
        }

        bool condition = comboBox_creer_alerte_condition->checkField();

        check = check    &&
                quantite &&
                condition;
    }
    else if (radioButton_creer_alerte_periode_temps->isChecked())
    {
        comboBox_creer_alerte_condition->setPalette(YerothUtils::YEROTH_WHITE_PALETTE);
        lineEdit_creer_alerte_quantite->setPalette(YerothUtils::YEROTH_WHITE_PALETTE);

        if (dateEdit_creer_alerte_date_debut->date() < QDate::currentDate())
        {
            dateEdit_creer_alerte_date_debut->setPalette(YerothUtils::YEROTH_RED_PALETTE);
            dateEdit_creer_alerte_date_fin->setPalette(YerothUtils::YEROTH_RED_PALETTE);

            QString pMsg(QObject::trUtf8("Le paramètre 'début' de l'alerte ne doit pas être daté dans le passé !"));

            YerothQMessageBox::information(this,
                                     QObject::trUtf8("Yeroth-erp-3.0 - créer ~ alerte"),
                                     pMsg);
            return false;
        }

        bool periode =
            (dateEdit_creer_alerte_date_debut->date() <= dateEdit_creer_alerte_date_fin->date());

        if (!periode)
        {
            dateEdit_creer_alerte_date_debut->setPalette(YerothUtils::YEROTH_RED_PALETTE);
            dateEdit_creer_alerte_date_fin->setPalette(YerothUtils::YEROTH_RED_PALETTE);

            QString pMsg(QObject::trUtf8("Le paramètre 'début' de l'alerte doit être daté avant le paramètre 'fin' !"));

            YerothQMessageBox::information(this,
                                     QObject::trUtf8("Yeroth-erp-3.0 - créer ~ alerte"),
                                     pMsg);
            return false;
        }

        check = check   &&
                periode;
    }

    return check;
}

void YerothAdminCreateWindow::clear_alerte_all_fields()
{
    lineEdit_creer_alerte_nom->clearField();
    lineEdit_creer_alerte_quantite->clearField();
    lineEdit_creer_alerte_designation->clearField();
    lineEdit_creer_alerte_destinataire->clearField();
    lineEdit_creer_alerte_nom_destinataire->clear();
    lineEdit_creer_alerte_quantite_en_stock->clear();

    textEdit_creer_alerte_message->clearField();

    comboBox_creer_alerte_condition->clearField();

    dateEdit_creer_alerte_date_debut->reset();
    dateEdit_creer_alerte_date_fin->reset();

    dateEdit_creer_alerte_date_debut->setPalette(YerothUtils::YEROTH_WHITE_PALETTE);
    dateEdit_creer_alerte_date_fin->setPalette(YerothUtils::YEROTH_WHITE_PALETTE);
}

