alter table clients modify column `groupes_du_client` varchar(5000) AFTER refereur_client_ID;


create table departements_produits (id int primary key, nom_departement_produit varchar(256) NOT NULL UNIQUE, description_departement_produit varchar(256));

insert into departements_produits (id, nom_departement_produit) values (0, 'drugs_healthcare');

alter table categories ADD column `nom_departement_produit` varchar(256) NOT NULL AFTER nom_categorie;

UPDATE categories SET nom_departement_produit='drugs_healthcare';

alter table stocks ADD column `nom_departement_produit` varchar(256) NOT NULL AFTER designation;

alter table stocks_sorties ADD column `nom_departement_produit` varchar(256) NOT NULL AFTER designation;

alter table stocks_vendu ADD column `nom_departement_produit` varchar(256) NOT NULL AFTER reference;

alter table services_completes ADD column `nom_departement_produit` varchar(256) NOT NULL AFTER reference;

alter table marchandises ADD column `nom_departement_produit` varchar(256) NOT NULL AFTER designation;

alter table achats ADD column `nom_departement_produit` varchar(256) NOT NULL AFTER reference;

alter table achats_aux_fournisseurs ADD column `nom_departement_produit` varchar(256) NOT NULL AFTER reference;


alter table programmes_de_fidelite_clients ADD column `nom_departement_produit` varchar(256) NOT NULL AFTER designation;


UPDATE stocks SET nom_departement_produit='drugs_healthcare';

UPDATE stocks_sorties SET nom_departement_produit='drugs_healthcare';

UPDATE stocks_vendu SET nom_departement_produit='drugs_healthcare';

UPDATE services_completes SET nom_departement_produit='drugs_healthcare';

UPDATE marchandises SET nom_departement_produit='drugs_healthcare';

UPDATE achats SET nom_departement_produit='drugs_healthcare';

UPDATE achats_aux_fournisseurs SET nom_departement_produit='drugs_healthcare';



