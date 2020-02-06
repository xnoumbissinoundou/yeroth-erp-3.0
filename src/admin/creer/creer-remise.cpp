
bool YerothAdminCreateWindow::creer_remise()
{
    if (creer_remise_check_fields())
    {
    }

    return false;
}


void YerothAdminCreateWindow::populateRemiseComboBoxes()
{
    _logger->log("populateRemiseComboBoxes");
}


void YerothAdminCreateWindow::creer_remise_check_fields_entry()
{
    bool remise_nom = lineEdit_creer_remise_nom->checkField();
    bool produit = lineEdit_creer_remise_designation->checkField();
}


bool YerothAdminCreateWindow::creer_remise_check_fields()
{
    bool remise_nom = lineEdit_creer_remise_nom->checkField();
    bool produit = lineEdit_creer_remise_designation->checkField();

    bool check =  remise_nom 	&&
    			  produit 		;


    if (dateEdit_creer_remise_date_debut->date() < QDate::currentDate())
    {
    	dateEdit_creer_remise_date_debut->setPalette(YerothUtils::YEROTH_RED_PALETTE);
    	dateEdit_creer_remise_date_fin->setPalette(YerothUtils::YEROTH_RED_PALETTE);

    	QString pMsg(QObject::trUtf8("Le paramètre 'début' de la remise ne doit pas être dans le passé !"));

    	YerothQMessageBox::information(this,
    								   QObject::trUtf8("Yeroth-erp-3.0 - créer ~ alerte"),
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
    								   QObject::trUtf8("Yeroth-erp-3.0 - créer ~ alerte"),
									   pMsg);
    	return false;
    }

    return check;
}

void YerothAdminCreateWindow::clear_remise_all_fields()
{
    lineEdit_creer_remise_nom->clearField();
    lineEdit_creer_remise_designation->clearField();

    lineEdit_creer_remise_quantite_en_stock->clear();
    lineEdit_creer_remise_date_peremption->clear();

    textEdit_creer_remise_message->clearField();

    dateEdit_creer_remise_date_debut->reset();
    dateEdit_creer_remise_date_fin->reset();

    dateEdit_creer_remise_date_debut->setPalette(YerothUtils::YEROTH_WHITE_PALETTE);
    dateEdit_creer_remise_date_fin->setPalette(YerothUtils::YEROTH_WHITE_PALETTE);
}

