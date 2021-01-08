alter table programmes_de_fidelite_clients ADD column `condition_du_benefice` varchar(2) AFTER pourcentage_du_rabais;
alter table programmes_de_fidelite_clients ADD column `valeur_de_la_condition_beneficiaire` varchar(256) AFTER condition_du_benefice;
