/*
 * yeroth-erp-user.hpp
 *
 *      Author: PR. XAVIER NOUMBISSI NOUNDOU
 */

#ifndef SRC_YEROTH_ERP_USERS_HPP_
#define SRC_YEROTH_ERP_USERS_HPP_

#include <QtCore/QString>

#include <QtCore/QDate>

#include <QtSql/QSqlRecord>

#include "src/utils/yeroth-erp-utils.hpp"

class QString;
class QDate;

class YerothERPWindows;
class YerothSqlTableModel;

class YerothERPUserSettings;
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
	:_role(YerothUtils::ROLE_INDEFINI),
	 _user_personal_settings(0),
	 _allWindows(allWindows)
	{
	}

	inline virtual ~YerothPOSUser()
	{
		YEROTH_DELETE_FREE_POINTER_NOW(_user_personal_settings);
	}

	enum YerothUtils::USER_ROLE role(){ return _role; }

	QString toString();

	inline void setRole(enum YerothUtils::USER_ROLE aRole)
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

	QString nom_complet_truncated_FOR_SMALL_RECEIPT();

	inline QString nom_completTex()
	{
		return YerothUtils::LATEX_IN_OUT_handleForeignAccents(_nom_complet);
	}

	inline QString nom_complet_truncated_FOR_SMALL_RECEIPT_Tex()
	{
		return YerothUtils::LATEX_IN_OUT_handleForeignAccents
				(nom_complet_truncated_FOR_SMALL_RECEIPT());
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

	inline QString titreTex()
	{
		return YerothUtils::LATEX_IN_OUT_handleForeignAccents(_titre);
	}

	inline void set_prenom(const QString &prenom)
	{
		_prenom = prenom;
	}

	inline void set_nom(const QString &nom)
	{
		_nom = nom;
	}

	inline void set_nom_utilisateur(const QString &nom_utilisateur)
	{
		_nom_utilisateur = nom_utilisateur;
	}

	inline void set_nom_complet(const QString &nom_complet)
	{
		_nom_complet = nom_complet;
	}

	inline void set_localisation(const QString &localisation)
	{
		_localisation = localisation;
	}

	inline void set_email(const QString &email)
	{
		_email = email;
	}

	inline void set_numero_telephone_1(const QString &numero_telephone_1)
	{
		_numero_telephone_1 = numero_telephone_1;
	}

	inline void set_numero_telephone_2(const QString &numero_telephone_2)
	{
		_numero_telephone_2 = numero_telephone_2;
	}

	inline void set_lieu_naissance(const QString &lieu_naissance)
	{
		_lieu_naissance = lieu_naissance;
	}

	inline void set_date_naissance(const QString &date_naissance)
	{
		_date_naissance = date_naissance;
	}

	inline void set_titre(const QString &titre)
	{
		_titre = titre;
	}

	inline virtual bool isPasDeRole()
	{
		return _role == YerothUtils::ROLE_INDEFINI;
	}

	inline virtual bool isAdmin()
	{
		return _role == YerothUtils::ROLE_ADMINISTRATEUR;
	}

	inline virtual bool isManager()
	{
		return _role == YerothUtils::ROLE_MANAGER;
	}

	inline virtual bool isVendeur()
	{
		return _role == YerothUtils::ROLE_VENDEUR;
	}

	inline virtual bool isMagasinier()
	{
		return _role == YerothUtils::ROLE_MAGASINIER;
	}

	inline virtual bool isGestionaireDesStocks()
	{
		return _role == YerothUtils::ROLE_GESTIONNAIREDESTOCK;
	}

	inline virtual bool isCaissier()
	{
		return _role == YerothUtils::ROLE_CAISSIER;
	}

	void create_user_personal_settings_file();

protected:

	enum YerothUtils::USER_ROLE _role;

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

	YerothERPUserSettings *_user_personal_settings;

private:

	YerothERPWindows *_allWindows;
};

#endif /* SRC_YEROTH_ERP_USERS_HPP_ */
