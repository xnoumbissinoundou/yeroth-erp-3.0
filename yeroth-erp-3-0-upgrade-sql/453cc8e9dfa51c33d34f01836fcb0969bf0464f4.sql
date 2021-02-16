alter table marchandises ADD column `nom_entreprise_client` varchar(256) AFTER quantite_totale;

alter table marchandises DROP column valeur_dinventaire;
