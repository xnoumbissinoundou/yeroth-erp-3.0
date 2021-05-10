create table charges_financieres (id int primary key NOT NULL, designation varchar(256) NOT NULL UNIQUE, nom_entreprise_fournisseur varchar(256), statut_de_lachat_au_fournisseur int(12), reference varchar(256), nom_departement_produit varchar(256) NOT NULL, categorie varchar(256) NOT NULL, reference_recu_dachat varchar(256), prix_dachat double NOT NULL, prix_unitaire double NOT NULL, montant_tva double NOT NULL, date_de_commande date NOT NULL, date_de_reception date, localisation varchar(256), quantite_totale double NOT NULL, nom_utilisateur_du_commandeur_de_lachat varchar(256));
