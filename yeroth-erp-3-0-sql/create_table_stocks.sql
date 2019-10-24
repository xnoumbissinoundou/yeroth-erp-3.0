create table stocks (id int primary key NOT NULL, reference varchar(256), designation varchar(256) NOT NULL, categorie varchar(256) NOT NULL, description_produit varchar(256), prix_unitaire double NOT NULL, montant_tva double NOT NULL, prix_vente double NOT NULL,  quantite_total double NOT NULL, stock_minimum double, localisation_stock varchar(256), image_produit mediumblob, date_entree date NOT NULL, date_peremption date NOT NULL, localisation varchar(256) NOT NULL, nom_entreprise_fournisseur varchar(256), lots_entrant int NOT NULL, quantite_par_lot double NOT NULL, historique_stock varchar(6500), prix_dachat double, reference_recu_dachat varchar(256), enregistreur_stock varchar(256));
