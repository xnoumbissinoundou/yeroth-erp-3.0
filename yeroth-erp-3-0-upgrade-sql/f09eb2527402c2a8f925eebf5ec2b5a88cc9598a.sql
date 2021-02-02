alter table entreprise_info ADD column `CARTE_DE_FIDELITE_CLIENT_nom_entreprise` varchar(256) AFTER reference_registre_du_commerce;

alter table entreprise_info ADD column `CARTE_DE_FIDELITE_CLIENT_siege` varchar(256) AFTER CARTE_DE_FIDELITE_CLIENT_nom_entreprise;

alter table entreprise_info ADD column `CARTE_DE_FIDELITE_CLIENT_service_de_gestion_clientele` varchar(256) AFTER CARTE_DE_FIDELITE_CLIENT_siege;

alter table entreprise_info ADD column `CARTE_DE_FIDELITE_CLIENT_service_du_programme_de_fidelite_client` varchar(256) AFTER CARTE_DE_FIDELITE_CLIENT_service_de_gestion_clientele;

alter table entreprise_info ADD column `CARTE_DE_FIDELITE_CLIENT_email` varchar(256) AFTER CARTE_DE_FIDELITE_CLIENT_service_du_programme_de_fidelite_client;

alter table entreprise_info ADD column `CARTE_DE_FIDELITE_CLIENT_telephone` varchar(256) AFTER CARTE_DE_FIDELITE_CLIENT_email;

alter table paiements ADD column `compte_fournisseur` double AFTER compte_client;

alter table paiements change column `compte_client` compte_client double;

alter table fournisseurs ADD column `compte_fournisseur` double NOT NULL AFTER email;
