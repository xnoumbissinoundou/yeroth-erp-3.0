alter table clients ADD column `groupes_du_client` varchar(3000) AFTER dette_maximale_compte_client;
alter table clients ADD column `personne_ayant_referer` varchar(256) AFTER id;
alter table clients DROP column `personne_ayant_referer`;
alter table clients ADD column `refereur_client` varchar(256) AFTER compte_client;
alter table clients ADD column `refereur_client_ID` int AFTER refereur_client;
alter table clients ADD column `groupes_client_ID` varchar(5000) AFTER refereur_client_ID;
alter table clients change column `groupes_client_ID` groupes_du_client_ID varchar(5000);

alter table alertes change column `quantite` quantite double;

alter table courriers_alertes change column `quantite` quantite double;

UPDATE init_configurations SET description_configuration='Intervalle de temps de vérification du daemon pour les alertes sur la période de temps (en secondes)' WHERE nom_configuration='ALERT_PERIOD_TIME_INTERVAL';

UPDATE init_configurations SET description_configuration='Intervalle de temps de vérification du daemon pour les alertes sur la quantité en stock (en secondes)' WHERE nom_configuration='ALERT_QUANTITY_TIME_INTERVAL';

UPDATE configurations SET description_configuration='Intervalle de temps de vérification du daemon pour les alertes sur la période de temps (en secondes)' WHERE nom_configuration='ALERT_PERIOD_TIME_INTERVAL';

UPDATE configurations SET description_configuration='Intervalle de temps de vérification du daemon pour les alertes sur la quantité en stock (en secondes)' WHERE nom_configuration='ALERT_QUANTITY_TIME_INTERVAL';

alter table programmes_de_fidelite_clients ADD column `pourcentage_du_rabais_refereur` int(2) AFTER pourcentage_du_rabais;
alter table programmes_de_fidelite_clients ADD column `rabais_en_cascade_tous_refereurs_par_le_haut` boolean AFTER pourcentage_du_rabais_refereur;

alter table programmes_de_fidelite_clients ADD column `condition_du_benefice` varchar(2) AFTER pourcentage_du_rabais;
alter table programmes_de_fidelite_clients ADD column `valeur_de_la_condition_beneficiaire` varchar(256) AFTER condition_du_benefice;


alter table groupes_de_clients ADD column `programme_de_fidelite_clients` varchar(256) AFTER description_groupe;
