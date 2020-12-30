UPDATE init_configurations SET description_configuration='Intervalle de temps de vérification du daemon pour les alertes sur la période de temps (en secondes)' WHERE nom_configuration='ALERT_PERIOD_TIME_INTERVAL';

UPDATE init_configurations SET description_configuration='Intervalle de temps de vérification du daemon pour les alertes sur la quantité en stock (en secondes)' WHERE nom_configuration='ALERT_QUANTITY_TIME_INTERVAL';

UPDATE configurations SET description_configuration='Intervalle de temps de vérification du daemon pour les alertes sur la période de temps (en secondes)' WHERE nom_configuration='ALERT_PERIOD_TIME_INTERVAL';

UPDATE configurations SET description_configuration='Intervalle de temps de vérification du daemon pour les alertes sur la quantité en stock (en secondes)' WHERE nom_configuration='ALERT_QUANTITY_TIME_INTERVAL';
