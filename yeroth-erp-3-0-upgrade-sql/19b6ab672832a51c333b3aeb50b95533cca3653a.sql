alter table programmes_de_fidelite_clients change column `nom_departement_produit` `localisation` varchar(256);

UPDATE programmes_de_fidelite_clients SET localisation='';
