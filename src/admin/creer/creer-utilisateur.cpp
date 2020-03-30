
bool YerothAdminCreateWindow::creer_utilisateur()
{
    //qDebug() << "YerothAdminCreateWindow::creer_utilisateur()";
    if (creer_utilisateur_check_fields())
    {
        QString retMsg(QObject::tr("Un compte utilisateur '"));

        YerothSqlTableModel &usersTableModel = _allWindows->getSqlTableModel_users();

        QString nom_utilisateur(lineEdit_creer_utilisateur_id->text().toLower());

        usersTableModel.yerothSetFilter(QString("LOWER(nom_utilisateur) = LOWER('%1')")
                                  .arg(nom_utilisateur));

        //qDebug() << "++ filter: " << usersTableModel.filter();

        int usersTableModelRowCount = usersTableModel.easySelect();

        //qDebug() << "++ result usersTableModelRowCount : " << usersTableModelRowCount ;

        if (usersTableModelRowCount > 0)
        {
            QSqlRecord record = usersTableModel.record(0);
            QString duplicateUtilisateur(GET_SQL_RECORD_DATA(record, "nom_utilisateur"));

            //qDebug() << "++ duplicate utilisateur: " << duplicateUtilisateur;

            retMsg.append(QString(QObject::trUtf8("%1' existe déjà !"))
            				.arg(duplicateUtilisateur));

            lineEdit_creer_utilisateur_id->setPalette(YerothUtils::YEROTH_RED_PALETTE);

            YerothQMessageBox::warning(this,
                                 QObject::trUtf8("Yeroth-erp-3.0 ~ administration ~ créer ~ utilisateur"),
                                 retMsg);

            usersTableModel.resetFilter();

            return false;
        }

        usersTableModel.resetFilter();

        QString mot_passe(lineEdit_creer_utilisateur_mot_passe_1->text());

        if (YerothUtils::isEqualCaseInsensitive(mot_passe, lineEdit_creer_utilisateur_id->text()))
        {
            QString msg(QObject::trUtf8("Choisissez un mot de passe différent de votre id !"));

            YerothQMessageBox::warning(this,
            						   QObject::trUtf8("Yeroth-erp-3.0 ~ administration ~ créer ~ utilisateur"),
									   msg,
									   QMessageBox::Ok);
            return false;
        }

        QSqlRecord record = usersTableModel.record();

        record.setValue(YerothDatabaseTableColumn::ID, _allWindows->getNextIdSqlTableModel_users());
        record.setValue(YerothDatabaseTableColumn::PRENOM, lineEdit_creer_utilisateur_prenom->text());
        record.setValue(YerothDatabaseTableColumn::NOM, lineEdit_creer_utilisateur_nom->text());

        comboBox_creer_utilisateur_titre->saveCurrentValueToDatabase(YerothDatabaseTableColumn::TITRE, record);

        record.setValue(YerothDatabaseTableColumn::LIEU_NAISSANCE, lineEdit_creer_utilisateur_lieu_naissance->text());
        record.setValue(YerothDatabaseTableColumn::DATE_NAISSANCE, dateEdit_creer_utilisateur_date_naissance->date());
        record.setValue(YerothDatabaseTableColumn::EMAIL, lineEdit_creer_utilisateur_email->text());
        record.setValue(YerothDatabaseTableColumn::PAYS, lineEdit_creer_utilisateur_pays->text());
        record.setValue(YerothDatabaseTableColumn::VILLE, lineEdit_creer_utilisateur_ville->text());
        record.setValue(YerothDatabaseTableColumn::PROVINCE_ETAT, lineEdit_creer_utilisateur_province_etat->text());
        record.setValue(YerothDatabaseTableColumn::BOITE_POSTALE, lineEdit_creer_utilisateur_boite_postale->text());
        record.setValue(YerothDatabaseTableColumn::NUMERO_TELEPHONE_1, lineEdit_creer_utilisateur_numero_telephone_1->text());
        record.setValue(YerothDatabaseTableColumn::NUMERO_TELEPHONE_2, lineEdit_creer_utilisateur_numero_telephone_2->text());

        comboBox_creer_utilisateur_role->saveCurrentValueToDatabase(YerothDatabaseTableColumn::ROLE, record);

        record.setValue(YerothDatabaseTableColumn::LOCALISATION, _allWindows->getInfoEntreprise().getLocalisation());
        record.setValue(YerothDatabaseTableColumn::NOM_UTILISATEUR, nom_utilisateur);

        QString nom_complet(lineEdit_creer_utilisateur_prenom->text());
        nom_complet.append(" ").append(lineEdit_creer_utilisateur_nom->text());

        record.setValue(YerothDatabaseTableColumn::NOM_COMPLET, nom_complet);

        mot_passe.append(nom_utilisateur);

        QByteArray md5Hash_mot_passe(MD5_HASH(mot_passe));

        record.setValue("mot_passe", md5Hash_mot_passe);

        retMsg.append(QString(QObject::tr("%1' pour '%2 %3"))
        				.arg(nom_utilisateur,
        					 lineEdit_creer_utilisateur_prenom->text(),
							 lineEdit_creer_utilisateur_nom->text()));

        bool success = usersTableModel.insertNewRecord(record);

        if (!success)
        {
            retMsg.append(QObject::trUtf8("' n'a pas pu être créer!"));

            YerothQMessageBox::warning(this,
                                 QObject::trUtf8("Yeroth-erp-3.0 - créer"),
                                 retMsg);
            return false;
        }

        retMsg.append(QObject::trUtf8("' a été créer !"));

        YerothQMessageBox::information(this,
        							   QObject::trUtf8("Yeroth-erp-3.0 - créer"),
                                 	   retMsg);

        this->clear_utilisateur_all_fields();

        _allWindows->_adminListerWindow->rendreVisible(SUJET_ACTION_COMPTE_UTILISATEUR);
        this->rendreInvisible();
    }

    return true;
}

void YerothAdminCreateWindow::populateUtilisateurComboBoxes()
{
    _logger->log("populateUtilisateurComboBoxes");

    comboBox_creer_utilisateur_titre->setupPopulateNORawString(_allWindows->TITRES,
    															YerothDatabaseTableColumn::APPELATION_TITRE,
																&YerothUtils::_titreToUserViewString);

    comboBox_creer_utilisateur_role->setupPopulateNORawString(_allWindows->ROLES,
    															YerothDatabaseTableColumn::NOM_ROLE,
																&YerothUtils::_roleToUserViewString);

    comboBox_creer_utilisateur_titre->populateComboBox();

    comboBox_creer_utilisateur_role->populateComboBox();
}


void YerothAdminCreateWindow::creer_utilisateur_check_fields_entry()
{
    bool nom = lineEdit_creer_utilisateur_nom->checkField();
    bool prenom = lineEdit_creer_utilisateur_prenom->checkField();
    bool titre = comboBox_creer_utilisateur_titre->checkField();
    bool user_id = lineEdit_creer_utilisateur_id->checkField();
    bool mot_passe_1 = lineEdit_creer_utilisateur_mot_passe_1->checkField();
    bool verification_mot_de_passe = lineEdit_creer_utilisateur_verification->checkField();
    bool role = comboBox_creer_utilisateur_role->checkField();

    //qDebug() << "++ verification_mot_de_passe" << BOOL_TO_STRING(verification_mot_de_passe);

    if (mot_passe_1 && verification_mot_de_passe)
    {
        if (lineEdit_creer_utilisateur_mot_passe_1->text() !=
                lineEdit_creer_utilisateur_verification->text())
        {
            lineEdit_creer_utilisateur_mot_passe_1->setPalette(YerothUtils::YEROTH_RED_PALETTE);
            lineEdit_creer_utilisateur_verification->setPalette(YerothUtils::YEROTH_RED_PALETTE);
        }
    }
}


bool YerothAdminCreateWindow::creer_utilisateur_check_fields()
{
    bool nom = lineEdit_creer_utilisateur_nom->checkField();
    bool prenom = lineEdit_creer_utilisateur_prenom->checkField();
    bool titre = comboBox_creer_utilisateur_titre->checkField();
    bool user_id = lineEdit_creer_utilisateur_id->checkField();
    bool mot_passe_1 = lineEdit_creer_utilisateur_mot_passe_1->checkField();
    bool verification_mot_de_passe = lineEdit_creer_utilisateur_verification->checkField();
    bool role = comboBox_creer_utilisateur_role->checkField();

    //qDebug() << "++ verification_mot_de_passe" << BOOL_TO_STRING(verification_mot_de_passe);

    if (mot_passe_1 && verification_mot_de_passe)
    {
        if (lineEdit_creer_utilisateur_mot_passe_1->text() !=
                lineEdit_creer_utilisateur_verification->text())
        {
            QString msg(QObject::trUtf8("Les deux mot de passe sont différents !"));

            YerothQMessageBox::warning(this,
            						   QObject::trUtf8("Yeroth-erp-3.0 ~ administration ~ créer ~ utilisateur"),
									   msg, QMessageBox::Ok);

            lineEdit_creer_utilisateur_mot_passe_1->setPalette(YerothUtils::YEROTH_RED_PALETTE);
            lineEdit_creer_utilisateur_verification->setPalette(YerothUtils::YEROTH_RED_PALETTE);

            return false;
        }
    }

    return nom 				&&
           prenom  			&&
           user_id 			&&
           mot_passe_1		&&
           verification_mot_de_passe		&&
           role				&&
           titre;
}

void YerothAdminCreateWindow::clear_utilisateur_all_fields()
{
    lineEdit_creer_utilisateur_nom->clearField();
    lineEdit_creer_utilisateur_prenom->clearField();
    lineEdit_creer_utilisateur_lieu_naissance->clearField();
    dateEdit_creer_utilisateur_date_naissance->reset();
    lineEdit_creer_utilisateur_numero_telephone_1->clearField();
    lineEdit_creer_utilisateur_numero_telephone_2->clearField();
    lineEdit_creer_utilisateur_id->clearField();
    lineEdit_creer_utilisateur_email->clearField();
    lineEdit_creer_utilisateur_ville->clearField();
    lineEdit_creer_utilisateur_province_etat->clearField();
    lineEdit_creer_utilisateur_pays->clearField();
    lineEdit_creer_utilisateur_boite_postale->clearField();
    lineEdit_creer_utilisateur_mot_passe_1->clearField();
    lineEdit_creer_utilisateur_verification->clearField();
    lineEdit_creer_utilisateur_localisation->clearField();

    comboBox_creer_utilisateur_titre->clearField();
    comboBox_creer_utilisateur_role->clearField();
}
