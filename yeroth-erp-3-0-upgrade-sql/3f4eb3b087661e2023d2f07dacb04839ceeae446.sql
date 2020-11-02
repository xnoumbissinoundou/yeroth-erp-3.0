alter table fournisseurs ADD column `image_fournisseur` mediumblob AFTER numero_contribuable;
alter table fournisseurs ADD column `compte_fournisseur` double AFTER numero_contribuable;
