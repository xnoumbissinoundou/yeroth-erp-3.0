
void YerothAdminCreateWindow::populateRemiseComboBoxes()
{
    _logger->log("populateRemiseComboBoxes");
}


void YerothAdminCreateWindow::creer_remise_check_fields_entry()
{
    lineEdit_creer_remise_nom->checkField();
    lineEdit_creer_remise_designation_article->checkField();
}


bool YerothAdminCreateWindow::creer_remise_check_fields()
{
    bool remise_nom = lineEdit_creer_remise_nom->checkField();
    bool produit = lineEdit_creer_remise_designation_article->checkField();

    bool check =  remise_nom 	&&
    			  produit 		;


    if (dateEdit_creer_remise_date_debut->date() < GET_CURRENT_DATE)
    {
    	dateEdit_creer_remise_date_debut->setPalette(YerothUtils::YEROTH_RED_PALETTE);
    	dateEdit_creer_remise_date_fin->setPalette(YerothUtils::YEROTH_RED_PALETTE);

    	QString pMsg(QObject::trUtf8("Le paramètre 'début' de la remise ne doit pas être dans le passé !"));

    	YerothQMessageBox::information(this,
    								   QObject::trUtf8("Yeroth-erp-3.0 - créer ~ remise"),
									   pMsg);
    	return false;
    }

    bool periode =
    		(dateEdit_creer_remise_date_debut->date() <= dateEdit_creer_remise_date_fin->date());

    if (!periode)
    {
    	dateEdit_creer_remise_date_debut->setPalette(YerothUtils::YEROTH_RED_PALETTE);
    	dateEdit_creer_remise_date_fin->setPalette(YerothUtils::YEROTH_RED_PALETTE);

    	QString pMsg(QObject::trUtf8("Le paramètre 'début' de la remise doit être avant le paramètre 'fin' !"));

    	YerothQMessageBox::information(this,
    								   QObject::trUtf8("Yeroth-erp-3.0 - créer ~ remise"),
									   pMsg);
    	return false;
    }

    return check;
}


bool YerothAdminCreateWindow::creer_remise()
{
    if (creer_remise_check_fields())
    {
        QString retMsg(QObject::trUtf8("Une remise dénomée '"));

        YerothSqlTableModel &remiseTableModel = _allWindows->getSqlTableModel_remises();

        remiseTableModel.yerothSetFilter_WITH_where_clause(QString("LOWER(%1) = LOWER('%2')")
                                    .arg(YerothDatabaseTableColumn::DESIGNATION_REMISE,
                                    	 lineEdit_creer_remise_nom->text()));

        //qDebug() << "++ filter: " << remiseTableModel.filter();

        int remisesTableModelRowCount = remiseTableModel.easySelect();

        //qDebug() << "++ result remisesTableModelRowCount : " << remisesTableModelRowCount ;

        if (remisesTableModelRowCount > 0)
        {
            QSqlRecord record = remiseTableModel.record(0);

            QString duplicateRemise(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::DESIGNATION_REMISE));

            //qDebug() << "++ duplicate remise: " << duplicateAlerte;

            retMsg.append(QString(QObject::trUtf8("%1' existe déjà ! "))
            				.arg(duplicateRemise));

            YerothQMessageBox::warning(this,
                                 QObject::trUtf8("Yeroth-erp-3.0 ~ administration ~ créer ~ remise"),
                                 retMsg);

            remiseTableModel.resetFilter();

            return false;
        }

        remiseTableModel.resetFilter();

        QSqlRecord record = remiseTableModel.record();

        YerothSqlTableModel &stocksTableModel = _allWindows->getSqlTableModel_stocks();

        stocksTableModel.yerothSetFilter_WITH_where_clause(GENERATE_SQL_IS_STMT(YerothDatabaseTableColumn::DESIGNATION, lineEdit_creer_remise_designation_article->text()));

        int stocksTableModelRowCount = stocksTableModel.easySelect();
        if (stocksTableModelRowCount <= 0)
        {
            YerothQMessageBox::warning(this,
                                 QObject::trUtf8("Yeroth-erp-3.0 ~ administration ~ créer ~ remise"),
                                 QObject::trUtf8("Il n'est pas possible de créer une remise "
                                                 "sur un stock déjà écoulé !"));

            return false;
        }

        QSqlRecord stocksRecord = stocksTableModel.record(0);

        record.setValue(YerothDatabaseTableColumn::DESIGNATION_REMISE, lineEdit_creer_remise_nom->text());
        record.setValue(YerothDatabaseTableColumn::DESIGNATION, lineEdit_creer_remise_designation_article->text());

    	YerothPOSUser *currentUser = _allWindows->getUser();

    	if (0 != currentUser)
    	{
    		record.setValue(YerothDatabaseTableColumn::NOM_COMPLET_CREATEUR_REMISE_ALERTE, currentUser->nom_complet());
    	}

    	record.setValue(YerothDatabaseTableColumn::STOCKS_ID, GET_SQL_RECORD_DATA(stocksRecord, YerothDatabaseTableColumn::ID));
    	record.setValue(YerothDatabaseTableColumn::REMISE_RESOLUE, 0);

    	record.setValue(YerothDatabaseTableColumn::REMISE_POURCENTAGE, spinBox_remise_pourcentage->value());

    	record.setValue(YerothDatabaseTableColumn::DATE_DEBUT, dateEdit_creer_remise_date_debut->date());
    	record.setValue(YerothDatabaseTableColumn::DATE_FIN, dateEdit_creer_remise_date_fin->date());

        record.setValue(YerothDatabaseTableColumn::REMISE_NOTES, textEdit_creer_remise_message->toPlainText());
        record.setValue(YerothDatabaseTableColumn::DATE_CREATION, GET_CURRENT_DATE);

        bool success = remiseTableModel.insertNewRecord(record);

        if (!success)
        {
        	retMsg.append(QString(QObject::trUtf8("%1' n'a pas pu être créer !"))
        					.arg(lineEdit_creer_remise_nom->text()));

            YerothQMessageBox::warning(this,
                                 QObject::trUtf8("Yeroth-erp-3.0 - créer"),
                                 retMsg);
            return false;
        }

        retMsg.append(QString(QObject::trUtf8("%1' a été créer avec succès !"))
    					.arg(lineEdit_creer_remise_nom->text()));

        YerothQMessageBox::information(this,
        							   QObject::trUtf8("Yeroth-erp-3.0 - créer"),
                                 	   retMsg);

        this->clear_remise_all_fields();
        this->rendreInvisible();

        stocksTableModel.resetFilter();

        _allWindows->_adminListerWindow->rendreVisible(SUJET_ACTION_REMISE);

        return true;
    }

    return false;
}


void YerothAdminCreateWindow::clear_remise_all_fields()
{
    lineEdit_creer_remise_nom->clearField();
    lineEdit_creer_remise_designation_article->clearField();

    lineEdit_creer_remise_quantite_en_stock->clear();
    lineEdit_creer_remise_date_peremption->clear();

    textEdit_creer_remise_message->clearField();

    dateEdit_creer_remise_date_debut->reset();
    dateEdit_creer_remise_date_fin->reset();

    dateEdit_creer_remise_date_debut->setPalette(YerothUtils::YEROTH_WHITE_PALETTE);
    dateEdit_creer_remise_date_fin->setPalette(YerothUtils::YEROTH_WHITE_PALETTE);
}

