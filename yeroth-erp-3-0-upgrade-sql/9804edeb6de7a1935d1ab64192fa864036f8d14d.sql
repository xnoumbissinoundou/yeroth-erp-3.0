alter table clients modify column `groupes_du_client` varchar(5000) AFTER refereur_client_ID;


create table departements_produits (id int primary key, nom_departement_produit varchar(256) NOT NULL UNIQUE, description_departement_produit varchar(256));

insert into departements_produits (id, nom_departement_produit) values (0, 'drugs_healthcare');

alter table categories ADD column `nom_departement_produit` varchar(256) NOT NULL AFTER nom_categorie;

UPDATE categories SET nom_departement_produit='drugs_healthcare';

