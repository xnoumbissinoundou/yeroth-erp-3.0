alter table stocks ADD column `prix_unitaire_en_gros` double AFTER prix_unitaire;
alter table stocks ADD column `montant_tva_en_gros` double AFTER montant_tva;
alter table stocks ADD column `prix_vente_en_gros` double AFTER prix_vente;

alter table stocks_vendu ADD column `is_vente_en_gros` boolean NOT NULL AFTER historique_stock;

alter table services_completes ADD column `is_vente_en_gros` boolean NOT NULL AFTER historique_stock;

alter table achats ADD column `marge_beneficiaire_en_gros` double AFTER marge_beneficiaire;
alter table achats ADD column `prix_unitaire_en_gros` double AFTER prix_unitaire;
alter table achats ADD column `montant_tva_en_gros` double AFTER montant_tva;
alter table achats ADD column `prix_vente_en_gros` double AFTER prix_vente;

alter table marchandises ADD column `prix_vente_en_gros_precedent` double AFTER prix_vente_precedent;

update stocks set prix_unitaire_en_gros=`prix_unitaire`;
update stocks set prix_vente_en_gros=`prix_vente`;
update stocks set montant_tva_en_gros=`montant_tva`;


update stocks_vendu set is_vente_en_gros='0';

update services_completes set is_vente_en_gros='0';
