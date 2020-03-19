create table stocks_vendu (id int primary key NOT NULL, stocks_id int NOT NULL, clients_id int, date_vente date NOT NULL, heure_vente time NOT NULL, nom_entreprise_client varchar(256), reference varchar(256), categorie varchar(256) NOT NULL, designation varchar(256) NOT NULL, nom_entreprise_fournisseur varchar(256), prix_unitaire double NOT NULL, quantite_vendue double NOT NULL, montant_tva double NOT NULL, remise_prix double NOT NULL, remise_pourcentage double NOT NULL, montant_total_vente double NOT NULL, montant_recu double NOT NULL, montant_a_rembourser double NOT NULL, localisation varchar(256), date_peremption varchar(256) NOT NULL, nom_caissier varchar(256) NOT NULL, nom_utilisateur_caissier varchar(256) NOT NULL, type_de_vente varchar(256) NOT NULL, compte_client double, reference_recu_vendu varchar(256) NOT NULL, marge_beneficiaire double, is_service boolean NOT NULL);
