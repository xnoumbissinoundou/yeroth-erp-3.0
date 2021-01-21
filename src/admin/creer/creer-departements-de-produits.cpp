
bool YerothAdminCreateWindow::creer_departements_de_produits()
{
    if (creer_departements_de_produits_check_fields())
    {
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
