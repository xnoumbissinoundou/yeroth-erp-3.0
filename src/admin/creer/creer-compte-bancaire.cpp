
bool YerothAdminCreateWindow::creer_compte_bancaire()
{
    if (creer_compte_bancaire_check_fields())
    {
        QString retMsg(QObject::trUtf8("Le compte bancaire avec pour référence '"));

        YerothSqlTableModel &compteBancaireSqlTableModel = _allWindows->getSqlTableModel_comptes_bancaires();

        compteBancaireSqlTableModel.yerothSetFilter(QString("LOWER(%1) = LOWER('%2')")
                                       .arg(YerothDatabaseTableColumn::REFERENCE_DU_COMPTE_BANCAIRE,
                                    		lineEdit_creer_compte_bancaire_reference_du_compte_bancaire->text()));

        int compteBancaireTableModelRowCount = compteBancaireSqlTableModel.easySelect();

        if (compteBancaireTableModelRowCount > 0)
        {
            QSqlRecord record = compteBancaireSqlTableModel.record(0);
            QString duplicateCompteBancaire(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::REFERENCE_DU_COMPTE_BANCAIRE));

            retMsg.append(QString(QObject::trUtf8("%1' existe déjà ! "))
            				.arg(duplicateCompteBancaire));

            YerothQMessageBox::warning(this,
                                 QObject::trUtf8("Yeroth-erp-3.0 ~ administration ~ créer ~ compte bancaire"),
                                 retMsg);

            compteBancaireSqlTableModel.resetFilter();

            return false;
        }

        compteBancaireSqlTableModel.resetFilter();

        QSqlRecord record = compteBancaireSqlTableModel.record();

        record.setValue(YerothDatabaseTableColumn::ID, YerothERPWindows::getNextIdSqlTableModel_comptes_bancaires());
        record.setValue(YerothDatabaseTableColumn::REFERENCE_DU_COMPTE_BANCAIRE, lineEdit_creer_compte_bancaire_reference_du_compte_bancaire->text());
        record.setValue(YerothDatabaseTableColumn::INTITULE_DU_COMPTE_BANCAIRE, lineEdit_creer_compte_bancaire_intitule_du_compte_bancaire->text());
        record.setValue(YerothDatabaseTableColumn::INSTITUT_BANCAIRE, lineEdit_creer_compte_bancaire_institut_bancaire->text());
        record.setValue(YerothDatabaseTableColumn::DESCRIPTION_DU_COMPTE_BANCAIRE, textEdit_creer_compte_bancaire_description_du_compte->toPlainText());

        bool success = compteBancaireSqlTableModel.insertNewRecord(record);

        if (!success)
        {
            retMsg.append(QString(QObject::trUtf8("%1' n'a pas pu être créer ! "))
            				.arg(lineEdit_creer_compte_bancaire_reference_du_compte_bancaire->text()));

            YerothQMessageBox::warning(this,
                                 QObject::trUtf8("Yeroth-erp-3.0 ~ admin-créer-compte bancaire"),
                                 retMsg);
            return false;
        }

        retMsg.append(QString(QObject::trUtf8("%1' a été créer avec succès ! "))
        				.arg(lineEdit_creer_compte_bancaire_reference_du_compte_bancaire->text()));

        YerothQMessageBox::information(this,
                                 QObject::trUtf8("Yeroth-erp-3.0 ~ admin-créer-compte bancaire"),
                                 retMsg);

        clear_compte_bancaire_all_fields();

        rendreInvisible();

        _allWindows->_adminListerWindow->rendreVisible(SUJET_ACTION_COMPTE_BANCAIRE);

        return true;
    }

    return false;
}

bool YerothAdminCreateWindow::creer_compte_bancaire_check_fields()
{
    bool reference_du_compte_bancaire = lineEdit_creer_compte_bancaire_reference_du_compte_bancaire->checkField();
    bool intitule_du_compte_bancaire = lineEdit_creer_compte_bancaire_intitule_du_compte_bancaire->checkField();
    bool institut_bancaire = lineEdit_creer_compte_bancaire_institut_bancaire->checkField();

    return reference_du_compte_bancaire &&
    	   intitule_du_compte_bancaire  &&
		   institut_bancaire;
}

void YerothAdminCreateWindow::clear_compte_bancaire_all_fields()
{
	lineEdit_creer_compte_bancaire_reference_du_compte_bancaire->clearField();
	lineEdit_creer_compte_bancaire_intitule_du_compte_bancaire->clearField();
	lineEdit_creer_compte_bancaire_institut_bancaire->clearField();
	textEdit_creer_compte_bancaire_description_du_compte->clear();
}
