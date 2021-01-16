-- 2021-01-15
alter table clients modify column `groupes_du_client` varchar(5000) AFTER refereur_client_ID;


alter table categories ADD column `is_sous_categorie` boolean AFTER nom_categorie;

alter table stocks change column `categorie` sous_categorie varchar(256);

alter table stocks_sorties change column `categorie` sous_categorie varchar(256);

alter table stocks_vendu change column `categorie` sous_categorie varchar(256);

alter table services_completes change column `categorie` sous_categorie varchar(256);

alter table marchandises change column `categorie` sous_categorie varchar(256);

alter table achats change column `categorie` sous_categorie varchar(256);

alter table achats_aux_fournisseurs change column `categorie` sous_categorie varchar(256);


UPDATE categories SET is_sous_categorie='1';


alter table stocks ADD column `categorie` varchar(256) AFTER designation;

alter table stocks_sorties ADD column `categorie` varchar(256) AFTER designation;

alter table stocks_vendu ADD column `categorie` varchar(256) AFTER reference;

alter table services_completes ADD column `categorie` varchar(256) AFTER reference;

alter table marchandises ADD column `categorie` varchar(256) AFTER designation;

alter table achats ADD column `categorie` varchar(256) AFTER reference;

alter table achats_aux_fournisseurs ADD column `categorie` varchar(256) AFTER reference;


set @TEMP_MAX_ID_INCREMENT=(select MAX(id) from categories);
set @NEW_ID=(@TEMP_MAX_ID_INCREMENT+1);
insert into categories (id, nom_categorie, is_sous_categorie, description_categorie) values (@NEW_ID, 'stock', '0', 'DUMMY UPPER CATEGORY FOR ALL PREVIOUS INSTALLATIONS OF YEROTH-ERP-3.0 !');


UPDATE stocks SET categorie='stock';

UPDATE stocks_sorties SET categorie='stock';

UPDATE stocks_vendu SET categorie='stock';

UPDATE services_completes SET categorie='stock';

UPDATE marchandises SET categorie='stock';

UPDATE achats SET categorie='stock';

UPDATE achats_aux_fournisseurs SET categorie='stock';





