/*
 * modifier-client.cpp
 *
 *  Created on: Feb 3, 2016
 *      Author: Dipl.-Inf. Xavier NOUMBISSI NOUNDOU, Ph.D. (ABD)
 *      Email:  xnoundou7@gmail.com
 */

void YerothAdminModifierWindow::setupEditClient()
{
    YerothAdminListerWindow *lw = _allWindows->_adminListerWindow;
    YerothSqlTableModel *clientsTableModel = lw->getCurSearchSqlTableModel();

    if (!clientsTableModel)
    {
        clientsTableModel =  &_allWindows->getSqlTableModel_clients();
    }
    else if (clientsTableModel &&
             !YerothUtils::isEqualCaseInsensitive(clientsTableModel->sqlTableName(),
                     _allWindows->CLIENTS))
    {
        clientsTableModel =  &_allWindows->getSqlTableModel_clients();
    }

    QSqlRecord record = clientsTableModel->record(lw->lastSelectedItemForModification());

    lineEdit_modifier_client_nom_entreprise->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::NOM_ENTREPRISE));
    lineEdit_modifier_client_nom_representant->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::NOM_REPRESENTANT));
    lineEdit_modifier_client_quartier->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::QUARTIER));
    lineEdit_modifier_client_ville->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::VILLE));
    lineEdit_modifier_client_province_etat->setText(GET_SQL_RECORD_DATA(record, "province_etat"));
    lineEdit_modifier_client_pays->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::PAYS));
    lineEdit_modifier_client_boite_postale->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::BOITE_POSTALE));
    lineEdit_modifier_client_siege_social->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::SIEGE_SOCIAL));
    lineEdit_modifier_client_email->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::EMAIL));
    lineEdit_modifier_client_numero_telephone_1->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::NUMERO_TELEPHONE_1));
    lineEdit_modifier_client_numero_telephone_2->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::NUMERO_TELEPHONE_2));
    lineEdit_modifier_client_numero_contribuable->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::NUMERO_CONTRIBUABLE));

    if (0 != YerothUtils::getAllWindows())
    {
    	YerothPOSUser *aUser = YerothUtils::getAllWindows()->getUser();

    	if (0 != aUser && aUser->isManager())
    	{
    		label_admin_modifier_client_dette_maximale_compte_client->setVisible(true);
    		lineEdit_modifier_client_dette_maximale_compte_client->setVisible(true);
    		lineEdit_modifier_client_dette_maximale_compte_client->setEnabled(true);

    	    double dette_maximale_compte_client = GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::DETTE_MAXIMALE_COMPTE_CLIENT).toDouble();

    	    lineEdit_modifier_client_dette_maximale_compte_client->setText(GET_DOUBLE_STRING(dette_maximale_compte_client));
    	}
    	else
    	{
    		label_admin_modifier_client_dette_maximale_compte_client->setVisible(false);
    		lineEdit_modifier_client_dette_maximale_compte_client->setVisible(false);
    		lineEdit_modifier_client_dette_maximale_compte_client->setEnabled(false);
    	}
    }
    else
    {
    	label_admin_modifier_client_dette_maximale_compte_client->setVisible(false);
		lineEdit_modifier_client_dette_maximale_compte_client->setVisible(false);
		lineEdit_modifier_client_dette_maximale_compte_client->setEnabled(false);
    }

    textEdit_modifier_client_description->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::DESCRIPTION_CLIENT));
}

void YerothAdminModifierWindow::modifier_client()
{
    //_logger->log("modifier_client");
    if (modifier_client_check_fields())
    {
        YerothAdminListerWindow *lw = _allWindows->_adminListerWindow;
        YerothSqlTableModel *clientsTableModel = lw->getCurSearchSqlTableModel();

        if (!clientsTableModel)
        {
            clientsTableModel =  &_allWindows->getSqlTableModel_clients();
        }
        else if (clientsTableModel &&
                 !YerothUtils::isEqualCaseInsensitive(clientsTableModel->sqlTableName(),
                         _allWindows->CLIENTS))
        {
            clientsTableModel =  &_allWindows->getSqlTableModel_clients();
        }

        QSqlRecord record = clientsTableModel->record(lw->lastSelectedItemForModification());

        record.setValue(YerothDatabaseTableColumn::NOM_ENTREPRISE, lineEdit_modifier_client_nom_entreprise->text());
        record.setValue(YerothDatabaseTableColumn::NOM_REPRESENTANT, lineEdit_modifier_client_nom_representant->text());
        record.setValue(YerothDatabaseTableColumn::QUARTIER, lineEdit_modifier_client_quartier->text());
        record.setValue(YerothDatabaseTableColumn::VILLE, lineEdit_modifier_client_ville->text());
        record.setValue("province_etat", lineEdit_modifier_client_province_etat->text());
        record.setValue(YerothDatabaseTableColumn::PAYS, lineEdit_modifier_client_pays->text());
        record.setValue(YerothDatabaseTableColumn::SIEGE_SOCIAL, lineEdit_modifier_client_siege_social->text());
        record.setValue(YerothDatabaseTableColumn::BOITE_POSTALE, lineEdit_modifier_client_boite_postale->text());
        record.setValue(YerothDatabaseTableColumn::EMAIL, lineEdit_modifier_client_email->text());
        record.setValue(YerothDatabaseTableColumn::NUMERO_TELEPHONE_1, lineEdit_modifier_client_numero_telephone_1->text());
        record.setValue(YerothDatabaseTableColumn::NUMERO_TELEPHONE_2, lineEdit_modifier_client_numero_telephone_2->text());
        record.setValue(YerothDatabaseTableColumn::NUMERO_CONTRIBUABLE, lineEdit_modifier_client_numero_contribuable->text());

        if (0 != YerothUtils::getAllWindows())
        {
        	YerothPOSUser *aUser = YerothUtils::getAllWindows()->getUser();

        	if (0 != aUser && aUser->isManager())
        	{
        		record.setValue(YerothDatabaseTableColumn::DETTE_MAXIMALE_COMPTE_CLIENT, lineEdit_modifier_client_dette_maximale_compte_client->text());
        	}
        }

        record.setValue(YerothDatabaseTableColumn::DESCRIPTION_CLIENT, textEdit_modifier_client_description->toPlainText());

        bool success = clientsTableModel->updateRecord(lw->lastSelectedItemForModification(), record);

        QString retMsg("Les données du client '");
        retMsg.append(lineEdit_modifier_client_nom_entreprise->text())
        .append("'");

        if (success)
        {
            retMsg.append(" ont été modifiées avec succès!");
            YerothQMessageBox::information(this,
                                     QObject::trUtf8("Yeroth-erp-3.0 ~ admin-modifier-client"),
                                     FROM_UTF8_STRING(retMsg));

            _allWindows->_adminListerWindow->rendreVisible(SUJET_ACTION_CLIENT);
            this->rendreInvisible();
        }
        else
        {
            qDebug() << "\t[reason for failing] " << clientsTableModel->lastError();
            retMsg.append(" n'ont pas pu être modifiées !");
            YerothQMessageBox::warning(this,
                                 QObject::trUtf8("Yeroth-erp-3.0 ~ admin-modifier-client"),
                                 FROM_UTF8_STRING(retMsg));
        }
    }
}

bool YerothAdminModifierWindow::modifier_client_check_fields()
{
    bool nom_entreprise = lineEdit_modifier_client_nom_entreprise->checkField();
    bool nom_representant = lineEdit_modifier_client_nom_representant->checkField();

    return nom_entreprise &&
           nom_representant;
}

void YerothAdminModifierWindow::clear_client_all_fields()
{
    lineEdit_modifier_client_nom_entreprise->clearField();
    lineEdit_modifier_client_nom_representant->clearField();
    lineEdit_modifier_client_quartier->clear();
    lineEdit_modifier_client_ville->clear();
    lineEdit_modifier_client_province_etat->clear();
    lineEdit_modifier_client_pays->clear();
    lineEdit_modifier_client_siege_social->clear();
    lineEdit_modifier_client_boite_postale->clear();
    lineEdit_modifier_client_email->clear();
    lineEdit_modifier_client_numero_telephone_1->clear();
    lineEdit_modifier_client_numero_telephone_2->clear();
    lineEdit_modifier_client_numero_contribuable->clear();
    textEdit_modifier_client_description->clear();
}

