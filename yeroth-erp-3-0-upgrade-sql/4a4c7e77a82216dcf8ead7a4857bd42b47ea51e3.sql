alter table clients DROP column `personne_ayant_referer`;
alter table clients ADD column `refereur_client` varchar(256) AFTER compte_client;
alter table clients ADD column `refereur_client_ID` int AFTER refereur_client;
alter table clients ADD column `groupes_client_ID` varchar(5000) AFTER refereur_client_ID;
