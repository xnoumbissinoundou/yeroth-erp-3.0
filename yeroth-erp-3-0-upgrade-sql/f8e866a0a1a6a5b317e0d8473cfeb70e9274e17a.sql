alter table achats_de_services modify nom_departement_produit varchar(256) AFTER id;
alter table achats_de_services modify categorie varchar(256) AFTER nom_departement_produit;
alter table achats_de_services modify designation varchar(256) AFTER categorie;
alter table achats_de_services modify nom_entreprise_fournisseur varchar(256) AFTER designation;
