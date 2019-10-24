/*
 * yeroth-erp-user.hpp
 *
 *  Created on: Oct 22, 2015
 *      Author: Dipl.-Inf. Xavier NOUMBISSI NOUNDOU, Ph.D. (ABD)
 *      Email:  xnoundou7@gmail.com
 */

#ifndef SRC_YEROTH_ERP_USERS_HPP_
#define SRC_YEROTH_ERP_USERS_HPP_

#include <QtCore/QString>
#include <QtCore/QDate>

#include <QtSql/QSqlRecord>

#include "src/utils/yeroth-erp-utils.hpp"

class QString;
class QDate;

enum Role
{
	RoleAdministrateur			= 10000,
	RoleManager,
	RoleVendeur,
	RoleGestionaireDesStocks,
	RoleMagasinier,
	RoleCaissier,
	PasDeRole
};

class YerothERPWindows;
class YerothSqlTableModel;

class YerothPOSUserAdministrateur;
class YerothPOSUserManager;
class YerothERPVendeur;
class YerothPOSUserGestionaireDesStocks;
class YerothPOSUserMagasinier;
class YerothPOSUserCaissier;

class YerothPOSUser : public QObject
{
	Q_OBJECT

public:

	YEROTH_CLASS_OPERATORS

	inline YerothPOSUser(YerothERPWindows *allWindows)
	:_role(PasDeRole),
	 _allWindows(allWindows)
	{
	}

	inline virtual ~YerothPOSUser(){}

	enum Role role(){ return _role; }

	QString toString();

	inline void setRole(enum Role aRole)
	{
		_role = aRole;
	}

	QString getRole();

	inline QString prenom()
	{
		return _prenom;
	}

	inline QString nom()
	{
		return _nom;
	}

	inline QString nom_utilisateur()
	{
		return _nom_utilisateur;
	}

	inline QString nom_complet()
	{
		return _nom_complet;
	}

	inline QString nom_completTex()
	{
		return YerothUtils::handleForeignAccents(_nom_complet);
	}

	inline QString localisation()
	{
		return _localisation;
	}

	inline QString email()
	{
		return _email;
	}

	inline QString numero_telephone_1()
	{
		return _numero_telephone_1;
	}

	inline QString numero_telephone_2()
	{
		return _numero_telephone_2;
	}

	inline QString lieu_naissance()
	{
		return _lieu_naissance;
	}

	inline QString date_naissance()
	{
		return _date_naissance;
	}

	inline QString titre()
	{
		return _titre;
	}

	inline void set_prenom(QString prenom)
	{
		_prenom = prenom;
	}

	inline void set_nom(QString nom)
	{
		_nom = nom;
	}

	inline void set_nom_utilisateur(QString nom_utilisateur)
	{
		_nom_utilisateur = nom_utilisateur;
	}

	inline void set_nom_complet(QString nom_complet)
	{
		_nom_complet = nom_complet;
	}

	inline void set_localisation(QString localisation)
	{
		_localisation = localisation;
	}

	inline void set_email(QString email)
	{
		_email = email;
	}

	inline void set_numero_telephone_1(QString numero_telephone_1)
	{
		_numero_telephone_1 = numero_telephone_1;
	}

	inline void set_numero_telephone_2(QString numero_telephone_2)
	{
		_numero_telephone_2 = numero_telephone_2;
	}

	inline void set_lieu_naissance(QString lieu_naissance)
	{
		_lieu_naissance = lieu_naissance;
	}

	inline void set_date_naissance(QString date_naissance)
	{
		_date_naissance = date_naissance;
	}

	inline void set_titre(QString titre)
	{
		_titre = titre;
	}

	inline virtual bool isPasDeRole()
	{
		return _role == PasDeRole;
	}

	inline virtual bool isAdmin()
	{
		return _role == RoleAdministrateur;
	}

	inline virtual bool isManager()
	{
		return _role == RoleManager;
	}

	inline virtual bool isVendeur()
	{
		return _role == RoleVendeur;
	}

	inline virtual bool isMagasinier()
	{
		return _role == RoleMagasinier;
	}

	inline virtual bool isGestionaireDesStocks()
	{
		return _role == RoleGestionaireDesStocks;
	}

	inline virtual bool isCaissier()
	{
		return _role == RoleCaissier;
	}

protected:

	enum Role _role;

	QString _prenom;
	QString _nom;
	QString _nom_utilisateur;
	QString _localisation;
	QString _email;
	QString _numero_telephone_1;
	QString _numero_telephone_2;
	QString _lieu_naissance;
	QString _date_naissance;
	QString _titre;
	QString _nom_complet;

private:

	YerothERPWindows *_allWindows;
};

#endif /* SRC_YEROTH_ERP_USERS_HPP_ */
