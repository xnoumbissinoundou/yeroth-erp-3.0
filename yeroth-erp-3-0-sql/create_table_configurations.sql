create table configurations (id int primary key, nom_configuration varchar(256), description_configuration varchar(256), valeur_configuration varchar(256));
insert into configurations (id, nom_configuration, description_configuration, valeur_configuration) values (0, "ALERT_PERIOD_TIME_INTERVAL", "Intervalle de temps de vérification du daemon pour les alertes sur la période de temps", "20");
insert into configurations (id, nom_configuration, description_configuration, valeur_configuration) values (1, "ALERT_QUANTITY_TIME_INTERVAL", "Intervalle de temps de vérification du daemon pour les alertes sur la quantité de stock", "10");
insert into configurations (id, nom_configuration, description_configuration, valeur_configuration) values (2, "CONFIG_TVA_VALUE", "Valeur de la TVA", "19.25");
insert into configurations (id, nom_configuration, description_configuration, valeur_configuration) values (3, "CONFIG_SALES_STRATEGY", "Configuration de la stratégie de vente/sortie", "FIFO");
insert into configurations (id, nom_configuration, description_configuration, valeur_configuration) values (4, "CONFIG_CURRENCY", "Configuration de la devise", "$");
