

void YerothAdminModifierWindow::setupEditDepartementsDeProduits()
{
}

void YerothAdminModifierWindow::modifier_departements_de_produits()
{
	if (modifier_departements_de_produits_check_fields())
	{
		YerothUtils::startTransaction();
		modifier_departements_de_produits_main();
		YerothUtils::commitTransaction();
	}
}

void YerothAdminModifierWindow::modifier_departements_de_produits_main()
{
}

bool YerothAdminModifierWindow::modifier_departements_de_produits_check_fields()
{
	return false;
}

void YerothAdminModifierWindow::clear_departements_de_produits_all_fields()
{

}
