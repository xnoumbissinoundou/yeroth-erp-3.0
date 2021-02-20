alter table clients modify column `dette_maximale_compte_client` double NOT NULL AFTER nom_entreprise;

alter table clients modify column `compte_client` double NOT NULL AFTER dette_maximale_compte_client;
