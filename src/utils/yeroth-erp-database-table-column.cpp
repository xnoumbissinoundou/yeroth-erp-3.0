/*
 * yeroth-erp-database-table-column.cpp
 *
 *      Author: Xavier NOUMBISSI NOUNDOU, Dipl.-Inf., Ph.D. (ABD)
 */

#include "yeroth-erp-database-table-column.hpp"

#include "src/include/yeroth-erp-3-0-software.text-configuration.hpp"

#include "src/utils/yeroth-erp-utils.hpp"


QMap<QString, QString> YerothDatabaseTableColumn::_tableColumnToUserViewString;

const QString YerothDatabaseTableColumn::ID								("id");
const QString YerothDatabaseTableColumn::HISTORIQUE_STOCK				("historique_stock");
const QString YerothDatabaseTableColumn::CLIENTS_ID						("clients_id");
const QString YerothDatabaseTableColumn::VENTE_ID						("vente_id");
const QString YerothDatabaseTableColumn::ENREGISTREUR_STOCK				("enregistreur_stock");
const QString YerothDatabaseTableColumn::REFERENCE						("reference");
const QString YerothDatabaseTableColumn::COMPTE_CLIENT					("compte_client");
const QString YerothDatabaseTableColumn::DETTE_MAXIMALE_COMPTE_CLIENT	("dette_maximale_compte_client");
const QString YerothDatabaseTableColumn::CATEGORIE						("categorie");
const QString YerothDatabaseTableColumn::DESIGNATION					("designation");
const QString YerothDatabaseTableColumn::QUANTITE						("quantite");
const QString YerothDatabaseTableColumn::STOCK_MINIMUM					("stock_minimum");
const QString YerothDatabaseTableColumn::STOCK_MAXIMUM					("stock_maximum");
const QString YerothDatabaseTableColumn::QUANTITE_VENDUE				("quantite_vendue");
const QString YerothDatabaseTableColumn::QUANTITE_TOTAL					("quantite_total");
const QString YerothDatabaseTableColumn::QUANTITE_SORTIE				("quantite_sortie");
const QString YerothDatabaseTableColumn::DESCRIPTION_FOURNISSEUR		("description_fournisseur");
const QString YerothDatabaseTableColumn::DESCRIPTION_PRODUIT			("description_produit");
const QString YerothDatabaseTableColumn::PRIX_UNITAIRE					("prix_unitaire");
const QString YerothDatabaseTableColumn::REFERENCE_RECU_DACHAT			("reference_recu_dachat");
const QString YerothDatabaseTableColumn::PRIX_DACHAT					("prix_dachat");
const QString YerothDatabaseTableColumn::PRIX_VENTE						("prix_vente");
const QString YerothDatabaseTableColumn::DESCRIPTION_CATEGORIE			("description_categorie");
const QString YerothDatabaseTableColumn::NOM_CATEGORIE					("nom_categorie");
const QString YerothDatabaseTableColumn::MONTANT_TVA					("montant_tva");
const QString YerothDatabaseTableColumn::TVA							("tva");
const QString YerothDatabaseTableColumn::MARGE_BENEFICIAIRE				("marge_beneficiaire");
const QString YerothDatabaseTableColumn::MONTANT_RECU					("montant_recu");
const QString YerothDatabaseTableColumn::MONTANT_A_REMBOURSER			("montant_a_rembourser");
const QString YerothDatabaseTableColumn::MONTANT_TOTAL_VENTE			("montant_total_vente");
const QString YerothDatabaseTableColumn::REMISE_PRIX					("remise_prix");
const QString YerothDatabaseTableColumn::REMISE_POURCENTAGE				("remise_pourcentage");
const QString YerothDatabaseTableColumn::LOCALISATION					("localisation");
const QString YerothDatabaseTableColumn::LOCALISATION_STOCK				("localisation_stock");
const QString YerothDatabaseTableColumn::QUANTITE_PAR_LOT				("quantite_par_lot");
const QString YerothDatabaseTableColumn::LOTS_ENTRANT					("lots_entrant");
const QString YerothDatabaseTableColumn::IMAGE_PRODUIT					("image_produit");
const QString YerothDatabaseTableColumn::DATE_PEREMPTION				("date_peremption");
const QString YerothDatabaseTableColumn::DATE_ENTREE					("date_entree");
const QString YerothDatabaseTableColumn::PRENOM							("prenom");
const QString YerothDatabaseTableColumn::NOM							("nom");
const QString YerothDatabaseTableColumn::NOM_RECEPTEUR					("nom_recepteur");
const QString YerothDatabaseTableColumn::NOM_MAGASINIER					("nom_magasinier");
const QString YerothDatabaseTableColumn::NOM_UTILISATEUR_MAGASINIER		("nom_utilisateur_magasinier");
const QString YerothDatabaseTableColumn::NOM_UTILISATEUR_CAISSIER		("nom_utilisateur_caissier");
const QString YerothDatabaseTableColumn::NOM_CAISSIER					("nom_caissier");
const QString YerothDatabaseTableColumn::NOM_COMPLET					("nom_complet");
const QString YerothDatabaseTableColumn::NOM_LOCALISATION				("nom_localisation");
const QString YerothDatabaseTableColumn::VALEUR_DIVENTAIRE				("valeur_dinventaire");
const QString YerothDatabaseTableColumn::NOM_UTILISATEUR				("nom_utilisateur");
const QString YerothDatabaseTableColumn::STOCKS_ID						("stocks_id");
const QString YerothDatabaseTableColumn::STOCK_MANAGER					("stock_manager");
const QString YerothDatabaseTableColumn::MOT_PASSE						("mot_passe");
const QString YerothDatabaseTableColumn::TITRE							("titre");
const QString YerothDatabaseTableColumn::APPELATION_TITRE				("appelation_titre");
const QString YerothDatabaseTableColumn::ROLE							("role");
const QString YerothDatabaseTableColumn::EMAIL							("email");
const QString YerothDatabaseTableColumn::REFERENCE_CLIENT				("reference_client");
const QString YerothDatabaseTableColumn::NUMERO_TELEPHONE_1				("numero_telephone_1");
const QString YerothDatabaseTableColumn::NUMERO_TELEPHONE_2				("numero_telephone_2");
const QString YerothDatabaseTableColumn::LIEU_NAISSANCE					("lieu_naissance");
const QString YerothDatabaseTableColumn::DATE_NAISSANCE					("date_naissance");
const QString YerothDatabaseTableColumn::VILLE							("ville");
const QString YerothDatabaseTableColumn::QUARTIER						("quartier");
const QString YerothDatabaseTableColumn::PROVINCE_ETAT					("province_etat");
const QString YerothDatabaseTableColumn::PAYS							("pays");
const QString YerothDatabaseTableColumn::ADRESSE						("adresse");
const QString YerothDatabaseTableColumn::BOITE_POSTALE					("boite_postale");
const QString YerothDatabaseTableColumn::SORTIE_ID						("sortie_id");
const QString YerothDatabaseTableColumn::STATUT_RESOLU					("statut_resolu");
const QString YerothDatabaseTableColumn::SIEGE_SOCIAL					("siege_social");
const QString YerothDatabaseTableColumn::NOTES							("notes");
const QString YerothDatabaseTableColumn::NOM_ENTREPRISE					("nom_entreprise");
const QString YerothDatabaseTableColumn::NUMERO_DE_COMPTE_BANCAIRE		("numero_de_compte_bancaire");
const QString YerothDatabaseTableColumn::AGENCE_DE_COMPTE_BANCAIRE		("agence_de_compte_bancaire");
const QString YerothDatabaseTableColumn::SECTEURS_DACTIVITES			("secteurs_dactivites");
const QString YerothDatabaseTableColumn::NUMERO_CONTRIBUABLE			("numero_contribuable");
const QString YerothDatabaseTableColumn::NOM_REPRESENTANT				("nom_representant");
const QString YerothDatabaseTableColumn::NOM_ENTREPRISE_CLIENT			("nom_entreprise_client");
const QString YerothDatabaseTableColumn::NOM_ENTREPRISE_FOURNISSEUR		("nom_entreprise_fournisseur");
const QString YerothDatabaseTableColumn::DESCRIPTION_CLIENT				("description_client");
const QString YerothDatabaseTableColumn::DESIGNATION_ALERTE				("designation_alerte");
const QString YerothDatabaseTableColumn::CONDITION_ALERTE				("condition_alerte");
const QString YerothDatabaseTableColumn::NOM_ENCAISSEUR					("nom_encaisseur");
const QString YerothDatabaseTableColumn::NOM_PAYEUR						("nom_payeur");
const QString YerothDatabaseTableColumn::DATE_PAIEMENT					("date_paiement");
const QString YerothDatabaseTableColumn::MONTANT_PAYE					("montant_paye");
const QString YerothDatabaseTableColumn::DATE_VENTE						("date_vente");
const QString YerothDatabaseTableColumn::DATE_DEBUT						("date_debut");
const QString YerothDatabaseTableColumn::DATE_FIN						("date_fin");
const QString YerothDatabaseTableColumn::DATE_SORTIE					("date_sortie");
const QString YerothDatabaseTableColumn::HEURE_PAIEMENT					("heure_paiement");
const QString YerothDatabaseTableColumn::HEURE_VENTE					("heure_vente");
const QString YerothDatabaseTableColumn::HEURE_SORTIE					("heure_sortie");
const QString YerothDatabaseTableColumn::DESTINATAIRE					("destinataire");
const QString YerothDatabaseTableColumn::NOM_COMPLET_DESTINATAIRE		("nom_complet_destinataire");
const QString YerothDatabaseTableColumn::MESSAGE_ALERTE					("message_alerte");
const QString YerothDatabaseTableColumn::DATE_CREATION					("date_creation");
const QString YerothDatabaseTableColumn::TYPE_DE_VENTE					("type_de_vente");
const QString YerothDatabaseTableColumn::NOTIFICATIONS					("notifications");
const QString YerothDatabaseTableColumn::ALERTE_RESOLUE					("alerte_resolue");
const QString YerothDatabaseTableColumn::LOCALISATION_SORTIE			("localisation_sortie");
const QString YerothDatabaseTableColumn::LOCALISATION_ENTREE			("localisation_entree");
const QString YerothDatabaseTableColumn::LOCALISATION_SORTIE_ID			("localisation_sortie_id");
const QString YerothDatabaseTableColumn::LOCALISATION_ENTREE_ID			("localisation_entree_id");



YerothDatabaseTableColumn::YerothDatabaseTableColumn()
{
	_tableColumnToUserViewString.insert(YerothDatabaseTableColumn::REMISE_PRIX, 	QString("Remise (%1)").arg(YerothERPConfig::currency));

	_tableColumnToUserViewString.insert(YerothDatabaseTableColumn::ID, 				QObject::tr("ID"));

	_tableColumnToUserViewString.insert(YerothDatabaseTableColumn::HISTORIQUE_STOCK,	QObject::tr("Historique stock"));

	_tableColumnToUserViewString.insert(YerothDatabaseTableColumn::VENTE_ID,			QObject::tr("Nr. Fac."));

	_tableColumnToUserViewString.insert(YerothDatabaseTableColumn::ENREGISTREUR_STOCK,			QObject::tr("Gestionnaire stock"));

	_tableColumnToUserViewString.insert(YerothDatabaseTableColumn::REFERENCE,			QObject::tr("Référence"));

	_tableColumnToUserViewString.insert(YerothDatabaseTableColumn::COMPTE_CLIENT,			QObject::tr("Compte client"));

	_tableColumnToUserViewString.insert(YerothDatabaseTableColumn::DETTE_MAXIMALE_COMPTE_CLIENT,		QObject::tr("Dette maximale"));

	_tableColumnToUserViewString.insert(YerothDatabaseTableColumn::CATEGORIE,		QObject::trUtf8("Catégorie"));

	_tableColumnToUserViewString.insert(YerothDatabaseTableColumn::DESIGNATION,		QObject::trUtf8("Désignation"));

	_tableColumnToUserViewString.insert(YerothDatabaseTableColumn::QUANTITE,			QObject::trUtf8("Quantité"));

	_tableColumnToUserViewString.insert(YerothDatabaseTableColumn::STOCK_MAXIMUM,	QObject::trUtf8("stock maximum"));

	_tableColumnToUserViewString.insert(YerothDatabaseTableColumn::STOCK_MINIMUM,	QObject::trUtf8("stock minimum"));

	_tableColumnToUserViewString.insert(YerothDatabaseTableColumn::QUANTITE_VENDUE,	QObject::trUtf8("Qté"));

	_tableColumnToUserViewString.insert(YerothDatabaseTableColumn::QUANTITE_TOTAL,	QObject::trUtf8("Qté total"));

	_tableColumnToUserViewString.insert(YerothDatabaseTableColumn::QUANTITE_SORTIE,	QObject::trUtf8("Qté sortie"));

	_tableColumnToUserViewString.insert(YerothDatabaseTableColumn::DESCRIPTION_PRODUIT, QObject::tr("Description produit"));

	_tableColumnToUserViewString.insert(YerothDatabaseTableColumn::PRIX_UNITAIRE, 	QObject::tr("Prix unitaire"));

	_tableColumnToUserViewString.insert(YerothDatabaseTableColumn::REFERENCE_RECU_DACHAT, 		QObject::trUtf8("Référence achat"));

	_tableColumnToUserViewString.insert(YerothDatabaseTableColumn::PRIX_DACHAT, 		QObject::tr("Prix d'achat"));

	_tableColumnToUserViewString.insert(YerothDatabaseTableColumn::PRIX_VENTE, 		QObject::tr("Prix vente"));

	_tableColumnToUserViewString.insert(YerothDatabaseTableColumn::DESCRIPTION_CATEGORIE, 		QObject::trUtf8("Description"));

	_tableColumnToUserViewString.insert(YerothDatabaseTableColumn::NOM_CATEGORIE, 		QObject::trUtf8("Nom"));

	_tableColumnToUserViewString.insert(YerothDatabaseTableColumn::MONTANT_TVA, 		QObject::tr("Montant TVA"));

	_tableColumnToUserViewString.insert(YerothDatabaseTableColumn::TVA,				QObject::tr("TVA"));

	_tableColumnToUserViewString.insert(YerothDatabaseTableColumn::MARGE_BENEFICIAIRE,		QObject::trUtf8("Marge bénéficiaire"));

	_tableColumnToUserViewString.insert(YerothDatabaseTableColumn::MONTANT_RECU,		QObject::trUtf8("Montant reçu"));

	_tableColumnToUserViewString.insert(YerothDatabaseTableColumn::MONTANT_A_REMBOURSER,	QObject::trUtf8("Montant à rembourser"));

	_tableColumnToUserViewString.insert(YerothDatabaseTableColumn::MONTANT_TOTAL_VENTE,	QObject::tr("Total vente"));

	_tableColumnToUserViewString.insert(YerothDatabaseTableColumn::REMISE_PRIX,			QObject::tr("Remise prix"));

	_tableColumnToUserViewString.insert(YerothDatabaseTableColumn::REMISE_POURCENTAGE,	QObject::tr("Remise (%)"));

	_tableColumnToUserViewString.insert(YerothDatabaseTableColumn::LOCALISATION,			QObject::tr("Localisation"));

	_tableColumnToUserViewString.insert(YerothDatabaseTableColumn::LOCALISATION_STOCK,	QObject::tr("Localisation stock"));

	_tableColumnToUserViewString.insert(YerothDatabaseTableColumn::LOTS_ENTRANT,		QObject::tr("Lots entrant"));

	_tableColumnToUserViewString.insert(YerothDatabaseTableColumn::IMAGE_PRODUIT,	QObject::tr("Image produit"));

	_tableColumnToUserViewString.insert(YerothDatabaseTableColumn::DATE_PEREMPTION,	QObject::trUtf8("Date péremption"));

	_tableColumnToUserViewString.insert(YerothDatabaseTableColumn::DATE_ENTREE,		QObject::trUtf8("Date entrée"));

	_tableColumnToUserViewString.insert(YerothDatabaseTableColumn::PRENOM,			QObject::trUtf8("Prénom"));

	_tableColumnToUserViewString.insert(YerothDatabaseTableColumn::NOM,				QObject::tr("Nom"));

	_tableColumnToUserViewString.insert(YerothDatabaseTableColumn::NOM_RECEPTEUR,	QObject::trUtf8("Nom récepteur"));

	_tableColumnToUserViewString.insert(YerothDatabaseTableColumn::NOM_MAGASINIER,	QObject::tr("Nom magasinier"));

	_tableColumnToUserViewString.insert(YerothDatabaseTableColumn::NOM_UTILISATEUR_MAGASINIER,	QObject::tr("ID magasinier"));

	_tableColumnToUserViewString.insert(YerothDatabaseTableColumn::NOM_UTILISATEUR_CAISSIER,	QObject::tr("ID caissier"));

	_tableColumnToUserViewString.insert(YerothDatabaseTableColumn::NOM_CAISSIER,			QObject::tr("Nom caissier"));

	_tableColumnToUserViewString.insert(YerothDatabaseTableColumn::NOM_COMPLET,			QObject::tr("Nom complet"));

	_tableColumnToUserViewString.insert(YerothDatabaseTableColumn::NOM_LOCALISATION,	QObject::tr("Nom"));

	_tableColumnToUserViewString.insert(YerothDatabaseTableColumn::VALEUR_DIVENTAIRE,	QObject::tr("Valeur d'inventaire"));

	_tableColumnToUserViewString.insert(YerothDatabaseTableColumn::NOM_UTILISATEUR,		QObject::tr("ID"));

	_tableColumnToUserViewString.insert(YerothDatabaseTableColumn::STOCKS_ID,			QObject::tr("Stocks ID"));

	_tableColumnToUserViewString.insert(YerothDatabaseTableColumn::STOCK_MANAGER,		QObject::tr("Stock manager"));

	_tableColumnToUserViewString.insert(YerothDatabaseTableColumn::MOT_PASSE,			QObject::tr("Mot de passe"));

	_tableColumnToUserViewString.insert(YerothDatabaseTableColumn::TITRE,				QObject::tr("Titre"));

	_tableColumnToUserViewString.insert(YerothDatabaseTableColumn::APPELATION_TITRE,		QObject::tr("Titre appelation"));

	_tableColumnToUserViewString.insert(YerothDatabaseTableColumn::ROLE,					QObject::trUtf8("rôle"));

	_tableColumnToUserViewString.insert(YerothDatabaseTableColumn::EMAIL,				QObject::trUtf8("Émail"));

	_tableColumnToUserViewString.insert(YerothDatabaseTableColumn::REFERENCE_CLIENT,	QObject::trUtf8("Référence client"));

	_tableColumnToUserViewString.insert(YerothDatabaseTableColumn::NUMERO_TELEPHONE_1,	QObject::trUtf8("Numéro téléphone 1"));

	_tableColumnToUserViewString.insert(YerothDatabaseTableColumn::NUMERO_TELEPHONE_2,	QObject::trUtf8("Numéro téléphone 2"));

	_tableColumnToUserViewString.insert(YerothDatabaseTableColumn::LIEU_NAISSANCE,		QObject::tr("Lieu naissance"));

	_tableColumnToUserViewString.insert(YerothDatabaseTableColumn::DATE_NAISSANCE,		QObject::tr("Date naissance"));

	_tableColumnToUserViewString.insert(YerothDatabaseTableColumn::VILLE,				QObject::tr("Ville"));

	_tableColumnToUserViewString.insert(YerothDatabaseTableColumn::QUARTIER,				QObject::tr("Quartier"));

	_tableColumnToUserViewString.insert(YerothDatabaseTableColumn::PROVINCE_ETAT,		QObject::tr("Province/État"));

	_tableColumnToUserViewString.insert(YerothDatabaseTableColumn::PAYS,					QObject::tr("Pays"));

	_tableColumnToUserViewString.insert(YerothDatabaseTableColumn::ADRESSE,				QObject::tr("Adresse"));

	_tableColumnToUserViewString.insert(YerothDatabaseTableColumn::BOITE_POSTALE,		QObject::trUtf8("Boîte postale"));

	_tableColumnToUserViewString.insert(YerothDatabaseTableColumn::SORTIE_ID,			QObject::tr("Sortie ID"));

	_tableColumnToUserViewString.insert(YerothDatabaseTableColumn::SIEGE_SOCIAL,			QObject::trUtf8("Siège social"));

	_tableColumnToUserViewString.insert(YerothDatabaseTableColumn::NOTES,				QObject::tr("Notes"));

	_tableColumnToUserViewString.insert(YerothDatabaseTableColumn::NOM_ENTREPRISE,		QObject::tr("Nom entreprise"));

	_tableColumnToUserViewString.insert(YerothDatabaseTableColumn::NUMERO_DE_COMPTE_BANCAIRE,	QObject::trUtf8("Numéro compte bancaire"));

	_tableColumnToUserViewString.insert(YerothDatabaseTableColumn::AGENCE_DE_COMPTE_BANCAIRE,	QObject::tr("Agence compte bancaire"));

	_tableColumnToUserViewString.insert(YerothDatabaseTableColumn::SECTEURS_DACTIVITES,	QObject::trUtf8("Secteurs d'activités"));

	_tableColumnToUserViewString.insert(YerothDatabaseTableColumn::NUMERO_CONTRIBUABLE,	QObject::trUtf8("Numéro contribuable"));

	_tableColumnToUserViewString.insert(YerothDatabaseTableColumn::NOM_REPRESENTANT,		QObject::trUtf8("Nom représentant"));

	_tableColumnToUserViewString.insert(YerothDatabaseTableColumn::NOM_ENTREPRISE_CLIENT,	QObject::tr("Entreprise client"));

	_tableColumnToUserViewString.insert(YerothDatabaseTableColumn::NOM_ENTREPRISE_FOURNISSEUR,	QObject::tr("Entreprise fournisseur"));

	_tableColumnToUserViewString.insert(YerothDatabaseTableColumn::DESCRIPTION_CLIENT,	QObject::tr("Description client"));

	_tableColumnToUserViewString.insert(YerothDatabaseTableColumn::DESIGNATION_ALERTE,	QObject::trUtf8("Désignation alerte"));

	_tableColumnToUserViewString.insert(YerothDatabaseTableColumn::CONDITION_ALERTE,		QObject::tr("Condition alerte"));

	_tableColumnToUserViewString.insert(YerothDatabaseTableColumn::NOM_ENCAISSEUR,			QObject::tr("Encaisseur"));

	_tableColumnToUserViewString.insert(YerothDatabaseTableColumn::NOM_PAYEUR,				QObject::tr("Payeur"));

	_tableColumnToUserViewString.insert(YerothDatabaseTableColumn::DATE_PAIEMENT,			QObject::tr("Date paiement"));

	_tableColumnToUserViewString.insert(YerothDatabaseTableColumn::MONTANT_PAYE,			QObject::trUtf8("Montant payé"));

	_tableColumnToUserViewString.insert(YerothDatabaseTableColumn::DATE_VENTE,			QObject::tr("Date vente"));

	_tableColumnToUserViewString.insert(YerothDatabaseTableColumn::DATE_DEBUT,			QObject::trUtf8("Date début"));

	_tableColumnToUserViewString.insert(YerothDatabaseTableColumn::DATE_FIN,				QObject::tr("Date fin"));

	_tableColumnToUserViewString.insert(YerothDatabaseTableColumn::DATE_SORTIE,			QObject::tr("Date sortie"));

	_tableColumnToUserViewString.insert(YerothDatabaseTableColumn::HEURE_PAIEMENT,			QObject::tr("Heure paiement"));

	_tableColumnToUserViewString.insert(YerothDatabaseTableColumn::HEURE_VENTE,			QObject::tr("Heure vente"));

	_tableColumnToUserViewString.insert(YerothDatabaseTableColumn::HEURE_SORTIE,			QObject::tr("Heure sortie"));

	_tableColumnToUserViewString.insert(YerothDatabaseTableColumn::DESTINATAIRE,			QObject::tr("Destinataire"));

	_tableColumnToUserViewString.insert(YerothDatabaseTableColumn::NOM_COMPLET_DESTINATAIRE,	QObject::tr("Nom complet destinataire"));

	_tableColumnToUserViewString.insert(YerothDatabaseTableColumn::MESSAGE_ALERTE,		QObject::tr("Message d'alerte"));

	_tableColumnToUserViewString.insert(YerothDatabaseTableColumn::DATE_CREATION	,		QObject::trUtf8("Date de création"));

	_tableColumnToUserViewString.insert(YerothDatabaseTableColumn::NOTIFICATIONS,		QObject::tr("Notifications"));

	_tableColumnToUserViewString.insert(YerothDatabaseTableColumn::ALERTE_RESOLUE,		QObject::trUtf8("Alerte résolue"));

	_tableColumnToUserViewString.insert(YerothDatabaseTableColumn::LOCALISATION_SORTIE,	QObject::tr("Local. sortie"));

	_tableColumnToUserViewString.insert(YerothDatabaseTableColumn::LOCALISATION_ENTREE,	QObject::trUtf8("Local. entrée"));

	_tableColumnToUserViewString.insert(YerothDatabaseTableColumn::LOCALISATION_SORTIE_ID,	QObject::tr("ID local. sortie"));

	_tableColumnToUserViewString.insert(YerothDatabaseTableColumn::LOCALISATION_ENTREE_ID,	QObject::trUtf8("ID local. entrée"));

	//TODO: add an assertion to make sure that all keys, and values are unique
}
