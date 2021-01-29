create table programmes_de_fidelite_clients (id int primary key, date_creation date, reference_programme_de_fidelite_clients varchar(256), designation varchar(256) NOT NULL UNIQUE, nom_departement_produit varchar(256) NOT NULL, description_programme_de_fidelite_clients varchar(256) NOT NULL, montant_du_rabais double, pourcentage_du_rabais double, pourcentage_du_rabais_refereur int(2), rabais_en_cascade_tous_refereurs_par_le_haut boolean, condition_du_benefice varchar(2), valeur_de_la_condition_beneficiaire varchar(256));
