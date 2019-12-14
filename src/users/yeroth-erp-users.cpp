/*
 * yeroth-erp-users.cpp
 *
 *      Author: Xavier NOUMBISSI NOUNDOU, Dipl.-Inf., Ph.D. (ABD)
 */

#include <QtCore/QDate>

#include <QtCore/QString>

#include "src/include/yeroth-erp-3-0-software.text-configuration.hpp"

#include "yeroth-erp-users.hpp"

#include "src/yeroth-erp-windows.hpp"

#include "src/utils/yeroth-erp-utils.hpp"

#include "src/utils/yeroth-erp-sqltable-model.hpp"

QString YerothPOSUser::getRole()
{
    QString role;

#ifdef YEROTH_FRANCAIS_LANGUAGE

    switch(_role)
    {
    case RoleAdministrateur:
        role.append(QObject::trUtf8(ROLE_FRANCAIS_UI_NAME_YEROTH_ERP_3_0_ADMINISTRATEUR));
        break;

    case RoleManager:
        role.append(QObject::trUtf8(ROLE_FRANCAIS_UI_NAME_YEROTH_ERP_3_0_MANAGER));
        break;

    case RoleMagasinier:
        role.append(QObject::trUtf8(ROLE_FRANCAIS_UI_NAME_YEROTH_ERP_3_0_MAGASINIER));
        break;

    case RoleVendeur:
        role.append(QObject::trUtf8(ROLE_FRANCAIS_UI_NAME_YEROTH_ERP_3_0_VENDEUR));
        break;

    case RoleGestionaireDesStocks:
        role.append(QObject::trUtf8(ROLE_FRANCAIS_UI_NAME_YEROTH_ERP_3_0_GESTIONAIRE_DES_STOCKS));
        break;

    case RoleCaissier:
        role.append(QObject::trUtf8(ROLE_FRANCAIS_UI_NAME_YEROTH_ERP_3_0_CAISSIER));
        break;

    case PasDeRole:
        role.append(QObject::trUtf8(ROLE_FRANCAIS_UI_NAME_YEROTH_ERP_3_0_INDEFINI));
        break;

    default:
        break;
    }

#elif YEROTH_ENGLISH_LANGUAGE

    switch(_role)
    {
    case RoleAdministrateur:
        role.append(QObject::trUtf8(ROLE_ENGLISH_UI_NAME_YEROTH_ERP_3_0_ADMINISTRATOR));
        break;

    case RoleManager:
        role.append(QObject::trUtf8(ROLE_ENGLISH_UI_NAME_YEROTH_ERP_3_0_MANAGER));
        break;

    case RoleMagasinier:
        role.append(QObject::trUtf8(ROLE_ENGLISH_UI_NAME_YEROTH_ERP_3_0_STORE_KEEPER));
        break;

    case RoleVendeur:
        role.append(QObject::trUtf8(ROLE_ENGLISH_UI_NAME_YEROTH_ERP_3_0_SELLER));
        break;

    case RoleGestionaireDesStocks:
        role.append(QObject::trUtf8(ROLE_ENGLISH_UI_NAME_YEROTH_ERP_3_0_INVENTORY_STOCK_MANAGER));
        break;

    case RoleCaissier:
        role.append(QObject::trUtf8(ROLE_ENGLISH_UI_NAME_YEROTH_ERP_3_0_CASHIER));
        break;

    case PasDeRole:
        role.append(QObject::trUtf8(ROLE_ENGLISH_UI_NAME_YEROTH_ERP_3_0_UNDEFINED));
        break;

    default:
        break;
    }

#endif


    return role;
}

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
