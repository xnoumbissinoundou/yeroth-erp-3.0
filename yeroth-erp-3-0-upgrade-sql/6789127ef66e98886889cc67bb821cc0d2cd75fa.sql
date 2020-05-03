DELETE FROM configurations WHERE nom_configuration='PDF_READER';
DELETE FROM configurations WHERE nom_configuration='RECEIPT_FORMAT';
DELETE FROM configurations WHERE nom_configuration='CONFIG_TEMPORARY_FILES_DIR';
DELETE FROM configurations WHERE nom_configuration='CONFIG_PRINTER';
DELETE FROM configurations WHERE nom_configuration='CONFIG_OPEN_CASH_DRAWER';
DELETE FROM configurations WHERE nom_configuration='CONFIG_LATEX_SYSTEM_ROOT_FOLDER';
DELETE FROM configurations WHERE nom_configuration='CONFIG_THERMAL_PRINTER_DEVICE_FILE_FULL_PATH';
DELETE FROM configurations WHERE nom_configuration='CONFIG_MAX_STRING_DISPLAY_LENGTH';
DELETE FROM configurations WHERE nom_configuration='CONFIG_ANNEE_DEPART_RAPPORTS_CHIFFRE_AFFAIRE';
update configurations SET id='0' where nom_configuration='ALERT_PERIOD_TIME_INTERVAL';
update configurations SET id='1' where nom_configuration='ALERT_QUANTITY_TIME_INTERVAL';
update configurations SET id='2' where nom_configuration='CONFIG_TVA_VALUE';
update configurations SET id='3' where nom_configuration='CONFIG_SALES_STRATEGY';
update configurations SET id='4' where nom_configuration='CONFIG_CURRENCY';
DELETE FROM init_configurations WHERE nom_configuration='PDF_READER';
DELETE FROM init_configurations WHERE nom_configuration='RECEIPT_FORMAT';
DELETE FROM init_configurations WHERE nom_configuration='CONFIG_TEMPORARY_FILES_DIR';
DELETE FROM init_configurations WHERE nom_configuration='CONFIG_PRINTER';
DELETE FROM init_configurations WHERE nom_configuration='CONFIG_OPEN_CASH_DRAWER';
DELETE FROM init_configurations WHERE nom_configuration='CONFIG_LATEX_SYSTEM_ROOT_FOLDER';
DELETE FROM init_configurations WHERE nom_configuration='CONFIG_THERMAL_PRINTER_DEVICE_FILE_FULL_PATH';
DELETE FROM init_configurations WHERE nom_configuration='CONFIG_MAX_STRING_DISPLAY_LENGTH';
DELETE FROM init_configurations WHERE nom_configuration='CONFIG_ANNEE_DEPART_RAPPORTS_CHIFFRE_AFFAIRE';
update init_configurations SET id='0' where nom_configuration='ALERT_PERIOD_TIME_INTERVAL';
update init_configurations SET id='1' where nom_configuration='ALERT_QUANTITY_TIME_INTERVAL';
update init_configurations SET id='2' where nom_configuration='CONFIG_TVA_VALUE';
update init_configurations SET id='3' where nom_configuration='CONFIG_SALES_STRATEGY';
update init_configurations SET id='4' where nom_configuration='CONFIG_CURRENCY';
