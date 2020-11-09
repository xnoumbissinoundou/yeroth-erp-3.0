alter table stocks ADD column `prix_vente_en_gros` double AFTER prix_vente;
alter table achats ADD column `prix_vente_en_gros` double AFTER prix_vente;
