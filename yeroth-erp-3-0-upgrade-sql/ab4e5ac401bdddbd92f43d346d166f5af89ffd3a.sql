alter table stocks_vendu ADD column `montant_rabais_programme_de_fidelite_clients` double AFTER remise_prix;

alter table services_completes ADD column `montant_rabais_programme_de_fidelite_clients` double AFTER remise_prix;
