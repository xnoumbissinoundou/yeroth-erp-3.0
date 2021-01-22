/*
 * yeroth-erp-info-entreprise.hpp
 *
 *      Author: DR. XAVIER NOUMBISSI NOUNDOU
 */

#ifndef YEROTH_INFO_ENTREPRISE_HPP_
#define YEROTH_INFO_ENTREPRISE_HPP_

#include <QtCore/QString>

#include "src/include/yeroth-erp-3-0-software.text-configuration.hpp"

#include "src/utils/yeroth-erp-utils.hpp"

class YerothInfoEntreprise
{
public:

	YEROTH_CLASS_OPERATORS

	inline YerothInfoEntreprise()
	{
		updateInfoEntrepriseFromDB();
	}

	inline ~YerothInfoEntreprise()
	{
	}

	static bool load_COMPANY_INFO_LOGO_FROM_DB_TABLE_ENTREPRISE_INFO(QLabel *a_temporary_label_for_company_logo = 0);

	void updateInfoEntrepriseFromDB();

	QString toString();

	inline void setNomCommercial(QString nom_commercial)
	{
		_nom_commercial = nom_commercial;
	}

	inline void setAdresse(QString adresse)
	{
		_adresse = adresse;
	}

	inline void setVille(QString ville)
	{
		_ville = ville;
	}

	inline void setPays(QString pays)
	{
		_pays = pays;
	}

	inline void setEmail(QString email)
	{
		_email = email;
	}

	inline void setTelephone(QString telephone)
	{
		_telephone = telephone;
	}

	inline void setNumeroChambreContribuable(QString numero_contribuable)
	{
		_numero_contribuable = numero_contribuable;
	}

	inline void setReferenceRegistreChambreDuCommerce(QString reference_registre_chambre_du_commerce)
	{
		_reference_registre_chambre_du_commerce = reference_registre_chambre_du_commerce;
	}

	inline void setNumeroCompteBancaire(QString numero_comptebancaire)
	{
		_reference_comptebancaire = numero_comptebancaire;
	}

	inline void setActiviteEntreprise(QString activite_entreprise)
	{
		_secteurs_activites = activite_entreprise;
	}

	QString getNomCommercial_truncated_FOR_SMALL_RECEIPT();

	inline QString getNomCommercial() const
	{
		return _nom_commercial;
	}

	inline QString getNomCommercialTex() const
	{
		return YerothUtils::LATEX_IN_OUT_handleForeignAccents(_nom_commercial);
	}

	inline QString getNomCommercial_truncated_FOR_SMALL_RECEIPT_Tex()
	{
		return YerothUtils::LATEX_IN_OUT_handleForeignAccents
					(getNomCommercial_truncated_FOR_SMALL_RECEIPT());
	}

	inline QString getLocalisation() const
	{
		return _localisation;
	}

	inline QString getLocalisationTex() const
	{
		return YerothUtils::LATEX_IN_OUT_handleForeignAccents(_localisation);
	}

	inline QString getSiegeSocial() const
	{
		return _siege_social;
	}

	inline QString getBoitePostal() const
	{
		return _boite_postale;
	}

	inline QString getAdresse() const
	{
		return _adresse;
	}

	inline QString getVille() const
	{
		return _ville;
	}

	inline QString getVilleTex() const
	{
		return YerothUtils::LATEX_IN_OUT_handleForeignAccents(_ville);
	}

	inline QString getPays() const
	{
		return _pays;
	}

	inline QString getPaysTex() const
	{
		return YerothUtils::LATEX_IN_OUT_handleForeignAccents(_pays);
	}

	inline QString getEmail() const
	{
		return _email;
	}

	inline QString getEmailTex() const
	{
		return YerothUtils::LATEX_IN_OUT_handleForeignAccents(_email);
	}

	inline QString getTelephone() const
	{
		return _telephone;
	}

	inline QString getNumeroDeContribuable() const
	{
		return _numero_contribuable;
	}

	inline QString getReferenceRegistreDuCommerce() const
	{
		return _reference_registre_chambre_du_commerce;
	}

	inline QString getNumeroCompteBancaire() const
	{
		return _reference_comptebancaire;
	}

	inline QString getAgenceCompteBancaire() const
	{
		return _agence_comptebancaire;
	}

	inline QString getAgenceCompteBancaireTex() const
	{
		return YerothUtils::LATEX_IN_OUT_handleForeignAccents(_agence_comptebancaire);
	}

	inline QString getSecteursActivites() const
	{
		return _secteurs_activites;
	}

	inline QString getSecteursActivitesTex() const
	{
		return YerothUtils::LATEX_IN_OUT_handleForeignAccents(_secteurs_activites);
	}

private:

	QString _nom_commercial;
	QString _siege_social;
	QString _localisation;
	QString _boite_postale;
	QString _adresse;
	QString _ville;
	QString _pays;
	QString _email;
	QString _telephone;
	QString _numero_contribuable;
	QString _secteurs_activites;
	QString _agence_comptebancaire;
	QString _reference_comptebancaire;
	QString _reference_registre_chambre_du_commerce;
};


#endif //YEROTH_INFO_ENTREPRISE_HPP_
