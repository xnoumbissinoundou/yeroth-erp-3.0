alter table marchandises ADD column `stock_dalerte` double AFTER description_produit;
alter table marchandises ADD column `stock_minimum` double AFTER stock_dalerte;
alter table marchandises ADD column `stock_maximum` double AFTER stock_minimum;
