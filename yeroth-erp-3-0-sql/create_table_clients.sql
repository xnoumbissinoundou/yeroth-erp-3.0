create table clients (id int primary key, reference_client varchar(256), nom_entreprise varchar(256), dette_maximale_compte_client double NOT NULL, compte_client double NOT NULL, nom_representant varchar(256), description_client varchar(256), quartier varchar(256), ville varchar(256), province_etat varchar(256), pays varchar(256), boite_postale varchar(256), siege_social varchar(256), email varchar(256), numero_telephone_1 varchar(256), numero_telephone_2 varchar(256), reference_registre_du_commerce varchar(256), numero_contribuable varchar(256), refereur_client varchar(256), refereur_client_ID int, groupes_du_client varchar(3000), groupes_du_client_ID varchar(5000), image_compte_client mediumblob);
