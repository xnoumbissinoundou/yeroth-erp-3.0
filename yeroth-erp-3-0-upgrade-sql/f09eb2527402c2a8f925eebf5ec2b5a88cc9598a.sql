alter table entreprise_info ADD column `CARTE_DE_FIDELITE_CLIENT_nom_entreprise` varchar(256) AFTER reference_registre_du_commerce;

alter table entreprise_info ADD column `CARTE_DE_FIDELITE_CLIENT_siege` varchar(256) AFTER CARTE_DE_FIDELITE_CLIENT_nom_entreprise;

alter table entreprise_info ADD column `CARTE_DE_FIDELITE_CLIENT_service_de_gestion_clientele` varchar(256) AFTER CARTE_DE_FIDELITE_CLIENT_siege;

alter table entreprise_info ADD column `CARTE_DE_FIDELITE_CLIENT_service_du_programme_de_fidelite_client` varchar(256) AFTER CARTE_DE_FIDELITE_CLIENT_service_de_gestion_clientele;

alter table entreprise_info ADD column `CARTE_DE_FIDELITE_CLIENT_email` varchar(256) AFTER CARTE_DE_FIDELITE_CLIENT_service_du_programme_de_fidelite_client;

alter table entreprise_info ADD column `CARTE_DE_FIDELITE_CLIENT_telephone` varchar(256) AFTER CARTE_DE_FIDELITE_CLIENT_email;

