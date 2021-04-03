/*
 * yeroth-erp-users.cpp
 *
 *      Author: PR. XAVIER NOUMBISSI NOUNDOU
 */

#include <QtCore/QDate>

#include <QtCore/QString>

#include "src/include/yeroth-erp-3-0-software.text-configuration.hpp"

#include "yeroth-erp-users.hpp"

#include "src/yeroth-erp-windows.hpp"

#include "src/utils/yeroth-erp-utils.hpp"

#include "src/utils/yeroth-erp-sqltable-model.hpp"


QString YerothPOSUser::toString()
{
    QString userInfo;

# ifdef YEROTH_ENGLISH_LANGUAGE
    userInfo.append(QString("1) Given names: %1\n"
    						"2) Last names: %2\n"
    						"3) Title: %3\n"
    						"4) User ID: %4\n"
    						"5) Email: %5\n"
    						"6) Phone number 1: %6\n"
    						"7) Phone number 2: %7\n"
    						"8) Role: %8\n"
    						"9) Localisation: %9\n")
    					.arg(prenom(),
    						 nom(),
							 titre(),
							 nom_utilisateur(),
							 email(),
							 numero_telephone_1(),
							 numero_telephone_2(),
							 getRole(),
							 localisation()));
# endif

# ifdef YEROTH_FRANCAIS_LANGUAGE
    userInfo.append(QString("1) Prénoms: %1\n"
    						"2) Noms: %2\n"
    						"3) Titre: %3\n"
    						"4) Identifiant de l'utilisateur: %4\n"
    						"5) Émail: %5\n"
    						"6) Numéro de téléphone 1: %6\n"
    						"7) Numéro de téléphone 2: %7\n"
    						"8) Rôle: %8\n"
    						"9) Localisation: %9\n")
    					.arg(prenom(),
    						 nom(),
							 titre(),
							 nom_utilisateur(),
							 email(),
							 numero_telephone_1(),
							 numero_telephone_2(),
							 getRole(),
							 localisation()));
# endif

    return userInfo;
}


QString YerothPOSUser::getRole()
{
    QString role;

    switch(_role)
    {
    case YerothUtils::ROLE_ADMINISTRATEUR:
        role.append(YerothUtils::_roleToUserViewString.value(YerothUtils::ROLE_ADMINISTRATEUR));
        break;

    case YerothUtils::ROLE_MANAGER:
        role.append(YerothUtils::_roleToUserViewString.value(YerothUtils::ROLE_MANAGER));
        break;

    case YerothUtils::ROLE_MAGASINIER:
        role.append(YerothUtils::_roleToUserViewString.value(YerothUtils::ROLE_MAGASINIER));
        break;

    case YerothUtils::ROLE_VENDEUR:
        role.append(YerothUtils::_roleToUserViewString.value(YerothUtils::ROLE_VENDEUR));
        break;

    case YerothUtils::ROLE_GESTIONNAIREDESTOCK:
        role.append(YerothUtils::_roleToUserViewString.value(YerothUtils::ROLE_GESTIONNAIREDESTOCK));
        break;

    case YerothUtils::ROLE_CAISSIER:
        role.append(YerothUtils::_roleToUserViewString.value(YerothUtils::ROLE_CAISSIER));
        break;

    case YerothUtils::ROLE_INDEFINI:
        role.append(YerothUtils::_roleToUserViewString.value(YerothUtils::ROLE_INDEFINI));
        break;

    default:
        break;
    }

    return role;
}


QString YerothPOSUser::nom_complet_truncated_FOR_SMALL_RECEIPT()
{
    QString nom_complet_truncated(nom_complet().trimmed());

    if (nom_complet_truncated.length() > 23)
     {
    	nom_complet_truncated.truncate(23);
    	nom_complet_truncated.append(".");
     }

	return nom_complet_truncated;
}

