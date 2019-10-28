
bool YerothAdminCreateWindow::creer_client()
{
    if (creer_client_check_fields())
    {
        QString retMsg(QObject::tr("Le client '"));

        YerothSqlTableModel &clientsTableModel = _allWindows->getSqlTableModel_clients();

        clientsTableModel.yerothSetFilter(QString("LOWER(nom_entreprise) = LOWER('%1')")
                                    .arg(lineEdit_creer_client_nom_entreprise->text()));

        //qDebug() << "++ filter: " << clientsTableModel.filter();

        int clientsTableModelRowCount = clientsTableModel.easySelect();

        if (clientsTableModelRowCount > 0)
        {
            QSqlRecord record = clientsTableModel.record(0);
            QString duplicateClient(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::NOM_ENTREPRISE));

            //qDebug() << "++ duplicate client: " << duplicateClient;

            retMsg.append(QString(QObject::trUtf8("%1' existe déjà !")
            				.arg(duplicateClient)));

            YerothQMessageBox::warning(this,
                                 QObject::trUtf8("Yeroth-erp-3.0 ~ administration ~ créer ~ client"),
                                 retMsg);

            clientsTableModel.resetFilter();

            return false;
        }

        clientsTableModel.resetFilter();

        QSqlRecord record = clientsTableModel.record();

        record.setValue(YerothDatabaseTableColumn::ID, _allWindows->getNextIdSqlTableModel_clients());
        record.setValue(YerothDatabaseTableColumn::NOM_ENTREPRISE, lineEdit_creer_client_nom_entreprise->text());
        record.setValue(YerothDatabaseTableColumn::NOM_REPRESENTANT, lineEdit_creer_client_nom_representant->text());
        record.setValue(YerothDatabaseTableColumn::QUARTIER, lineEdit_creer_client_quartier->text());
        record.setValue(YerothDatabaseTableColumn::VILLE, lineEdit_creer_client_ville->text());
        record.setValue("province_etat", lineEdit_creer_client_province_etat->text());
        record.setValue(YerothDatabaseTableColumn::PAYS, lineEdit_creer_client_pays->text());
        record.setValue(YerothDatabaseTableColumn::BOITE_POSTALE, lineEdit_creer_client_boite_postale->text());
        record.setValue(YerothDatabaseTableColumn::SIEGE_SOCIAL, lineEdit_creer_client_siege_social->text());
        record.setValue(YerothDatabaseTableColumn::EMAIL, lineEdit_creer_client_email->text());
        record.setValue(YerothDatabaseTableColumn::NUMERO_TELEPHONE_1, lineEdit_creer_client_numero_telephone_1->text());
        record.setValue(YerothDatabaseTableColumn::NUMERO_TELEPHONE_2, lineEdit_creer_client_numero_telephone_2->text());
        record.setValue(YerothDatabaseTableColumn::NUMERO_CONTRIBUABLE, lineEdit_creer_client_numero_contribuable->text());

        YerothPOSUser *aUser = YerothUtils::getAllWindows()->getUser();

        if (0 != YerothUtils::getAllWindows())
        {
        	if (0 != aUser && aUser->isManager())
        	{
        		record.setValue(YerothDatabaseTableColumn::DETTE_MAXIMALE_COMPTE_CLIENT,
        						lineEdit_creer_client_dette_maximale_compte_client->text().toDouble());
        	}
        	else
        	{
        		record.setValue(YerothDatabaseTableColumn::DETTE_MAXIMALE_COMPTE_CLIENT, 0.0);
        	}
        }
        else
        {
        	record.setValue(YerothDatabaseTableColumn::DETTE_MAXIMALE_COMPTE_CLIENT, 0.0);
        }

        record.setValue(YerothDatabaseTableColumn::DESCRIPTION_CLIENT, textEdit_creer_client_description->toPlainText());

        retMsg.append(lineEdit_creer_client_nom_entreprise->text());

        bool success = clientsTableModel.insertNewRecord(record);

        if (!success)
        {
            retMsg.append(QObject::trUtf8("' n'a pas pu être créer !"));

            YerothQMessageBox::warning(this,
                                 QObject::trUtf8("Yeroth-erp-3.0 ~ administration ~ créer ~ client"),
                                 retMsg);
            return false;
        }

        retMsg.append(QObject::trUtf8("' a été créer avec succès !"));

        YerothQMessageBox::information(this,
                                 QObject::trUtf8("Yeroth-erp-3.0 ~ administration ~ créer ~ client"),
                                 retMsg);

        clear_client_all_fields();

        this->rendreInvisible();

        _allWindows->_adminListerWindow->rendreVisible(SUJET_ACTION_CLIENT);
    }

    return true;
}

bool YerothAdminCreateWindow::creer_client_check_fields()
{
    bool nom_entreprise = lineEdit_creer_client_nom_entreprise->checkField();

    YerothSqlTableModel &clientsTableModel = _allWindows->getSqlTableModel_clients();

    QString nom_entreprise_filter("nom_entreprise = '");

    nom_entreprise_filter.append(lineEdit_creer_client_nom_entreprise->text())
    .append("'");

    clientsTableModel.yerothSetFilter(nom_entreprise_filter);

    int clientsTableModelRowCount = clientsTableModel.rowCount();

    if (clientsTableModelRowCount > 0)
    {
        clientsTableModel.resetFilter();

        QString retMsg(QString(QObject::trUtf8("L'entreprise nommée '%1' est déjà "
        									   "existante dans la base de données !"))
        					.arg(lineEdit_creer_client_nom_entreprise->text()));

        YerothQMessageBox::warning(this,
                             QObject::trUtf8("Yeroth-erp-3.0 ~ administration ~ créer ~ client"),
                             retMsg);
        return false;
    }

    clientsTableModel.resetFilter();

    bool nom_representant = lineEdit_creer_client_nom_representant->checkField();

    return nom_entreprise &&
           nom_representant;
}

void YerothAdminCreateWindow::clear_client_all_fields()
{
    lineEdit_creer_client_nom_entreprise->clearField();
    lineEdit_creer_client_nom_representant->clearField();
    lineEdit_creer_client_quartier->clear();
    lineEdit_creer_client_ville->clear();
    lineEdit_creer_client_province_etat->clear();
    lineEdit_creer_client_pays->clear();
    lineEdit_creer_client_boite_postale->clear();
    lineEdit_creer_client_siege_social->clear();
    lineEdit_creer_client_email->clear();
    lineEdit_creer_client_numero_telephone_1->clear();
    lineEdit_creer_client_numero_telephone_2->clear();
    lineEdit_creer_client_numero_contribuable->clear();
    lineEdit_creer_client_dette_maximale_compte_client->clear();
    textEdit_creer_client_description->clear();
}
