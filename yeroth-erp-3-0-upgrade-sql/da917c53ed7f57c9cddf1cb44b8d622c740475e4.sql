update services_completes set historique_stock = REPLACE(historique_stock,';','_');
update stocks_sorties set historique_stock = REPLACE(historique_stock,';','_');
update stocks_vendu set historique_stock = REPLACE(historique_stock,';','_');
update stocks set historique_stock = REPLACE(historique_stock,';','_');
