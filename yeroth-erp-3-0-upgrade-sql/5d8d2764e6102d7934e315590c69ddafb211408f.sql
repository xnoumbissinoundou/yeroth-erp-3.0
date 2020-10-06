alter table marchandises ADD column `prix_dachat_precedent` double AFTER categorie;
alter table marchandises ADD column `prix_vente_precedent` double AFTER prix_dachat_precedent;
