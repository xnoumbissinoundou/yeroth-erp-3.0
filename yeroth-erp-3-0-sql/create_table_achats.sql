create table achats (id int primary key NOT NULL, stocks_id int NOT NULL, designation varchar(256) NOT NULL, reference varchar(256), nom_departement_produit varchar(256) NOT NULL, categorie varchar(256) NOT NULL, reference_recu_dachat varchar(256), prix_dachat double NOT NULL, prix_unitaire double NOT NULL, prix_unitaire_en_gros double NOT NULL, montant_tva double NOT NULL, montant_tva_en_gros double NOT NULL, prix_vente double NOT NULL, prix_vente_en_gros double NOT NULL, marge_beneficiaire double, marge_beneficiaire_en_gros double, date_entree date NOT NULL, date_peremption date NOT NULL, localisation varchar(256), localisation_stock varchar(256), nom_entreprise_fournisseur varchar(256), quantite_totale double NOT NULL, enregistreur_stock varchar(256));
