alter table imprimantereseau_recus_petits change column `nom_utilisateur_denvoi` nom_utilisateur varchar(256);
alter table imprimantereseau_recus_petits change column `message_recu_petit` message_PDV_recu_petit varchar(256);
alter table imprimantereseau_recus_petits change column `message_PDV_recu_petit` message_PDV_recu_petit varchar(256) NOT NULL;
alter table imprimantereseau_recus_petits change column `nom_utilisateur` nom_utilisateur varchar(256) NOT NULL;
alter table imprimantereseau_recus_petits ADD column `nom_complet` varchar(256) NOT NULL AFTER nom_utilisateur;
alter table imprimantereseau_recus_petits ADD column `adresse_ip_reseau_imprimante_thermique` varchar(15) NOT NULL AFTER message_PDV_recu_petit;
