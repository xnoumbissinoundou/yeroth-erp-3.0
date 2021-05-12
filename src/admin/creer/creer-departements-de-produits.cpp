
bool YerothAdminCreateWindow::creer_departements_de_produits()
{
    if (creer_departements_de_produits_check_fields())
    {
        QString retMsg(QObject::trUtf8("Le département '"));

        YerothSqlTableModel &departements_produitsTableModel =
        		_allWindows->getSqlTableModel_departements_produits();

        departements_produitsTableModel.
			yerothSetFilter_WITH_where_clause(QString("LOWER(%1) = LOWER('%2')")
                                       .arg(YerothDatabaseTableColumn::NOM_DEPARTEMENT_PRODUIT,
                                    		lineEdit_creer_departements_de_produits_nom->text()));

        //qDebug() << "++ filter: " << departements_produitsTableModel.filter();

        int departements_produitsTableModelRowCount = departements_produitsTableModel.easySelect();

        //qDebug() << "++ result departements_produitsTableModelRowCount : " << departements_produitsTableModelRowCount ;

        if (departements_produitsTableModelRowCount > 0)
        {
            QSqlRecord record = departements_produitsTableModel.record(0);
            QString duplicateDepartement_de_produits(
            		GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::NOM_DEPARTEMENT_PRODUIT));

            //qDebug() << "++ duplicate departement_de_produits: " << duplicateDepartement_de_produits;

            retMsg.append(QObject::trUtf8("%1' existe déjà ! ")
            				.arg(duplicateDepartement_de_produits));

            YerothQMessageBox::warning(this,
                                 QObject::trUtf8("Yeroth-pgi-3.0 ~ administration ~ créer ~ département de produits"),
                                 retMsg);

            departements_produitsTableModel.resetFilter();

            return false;
        }

        departements_produitsTableModel.resetFilter();

        QSqlRecord record = departements_produitsTableModel.record();

        record.setValue(YerothDatabaseTableColumn::ID, YerothERPWindows::getNextIdSqlTableModel_departements_produits());
        record.setValue(YerothDatabaseTableColumn::NOM_DEPARTEMENT_PRODUIT, lineEdit_creer_departements_de_produits_nom->text());
        record.setValue(YerothDatabaseTableColumn::DESCRIPTION_DEPARTEMENT_PRODUIT, textEdit_creer_departements_de_produits_description->toPlainText());

        bool success = departements_produitsTableModel.insertNewRecord(record);

        if (!success)
        {
            retMsg.append(QObject::trUtf8("%1' n'a pas pu être créer ! ")
            				.arg(lineEdit_creer_departements_de_produits_nom->text()));

            YerothQMessageBox::warning(this,
                                 QObject::trUtf8("Yeroth-pgi-3.0 ~ admin-créer-département de produits"),
                                 retMsg);
            return false;
        }

        retMsg.append(QObject::trUtf8("%1' a été créer avec succès ! ")
        				.arg(lineEdit_creer_departements_de_produits_nom->text()));

        YerothQMessageBox::information(this,
                                 QObject::trUtf8("Yeroth-pgi-3.0 ~ admin-créer-département de produits"),
                                 retMsg);

        clear_departements_de_produits_all_fields();

         rendreInvisible();

        _allWindows->_adminListerWindow->rendreVisible(SUJET_ACTION_DEPARTEMENTS_DE_PRODUITS);

        return true;
    }

    return false;
}

bool YerothAdminCreateWindow::creer_departements_de_produits_check_fields()
{
    bool nom = lineEdit_creer_departements_de_produits_nom->checkField();
    return nom;
}

void YerothAdminCreateWindow::clear_departements_de_produits_all_fields()
{
	lineEdit_creer_departements_de_produits_nom->clearField();
    textEdit_creer_departements_de_produits_description->clear();
}
