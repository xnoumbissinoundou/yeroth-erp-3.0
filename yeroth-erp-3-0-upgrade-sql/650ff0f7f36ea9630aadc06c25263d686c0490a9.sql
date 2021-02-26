alter table stocks_vendu ADD compte_client_PROGRAMME_DE_FIDELITE_CLIENTS double AFTER type_de_vente;

alter table services_completes ADD compte_client_PROGRAMME_DE_FIDELITE_CLIENTS double AFTER type_de_vente;

alter table paiements ADD compte_client_PROGRAMME_DE_FIDELITE_CLIENTS double AFTER montant_paye;

alter table clients modify dette_maximale_compte_client double NOT NULL AFTER compte_client;
