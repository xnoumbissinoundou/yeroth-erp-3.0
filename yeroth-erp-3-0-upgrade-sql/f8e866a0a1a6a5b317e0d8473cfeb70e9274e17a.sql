alter table charges_financieres modify nom_departement_produit varchar(256) AFTER id;
alter table charges_financieres modify categorie varchar(256) AFTER nom_departement_produit;
alter table charges_financieres modify designation varchar(256) AFTER categorie;
alter table charges_financieres modify nom_entreprise_fournisseur varchar(256) AFTER designation;
