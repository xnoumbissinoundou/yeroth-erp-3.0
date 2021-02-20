alter table services_completes ADD column `is_vente_en_gros` boolean NOT NULL AFTER historique_stock;

update services_completes set is_vente_en_gros='0';
