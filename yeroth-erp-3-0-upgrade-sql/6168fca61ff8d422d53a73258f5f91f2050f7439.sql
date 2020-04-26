update services_completes set type_de_vente='350' where type_de_vente='achat-comptant';
update services_completes set type_de_vente='360' where type_de_vente='achat-compte-client';

alter table services_completes change column `type_de_vente` type_de_vente int(12);


update stocks_vendu set type_de_vente='350' where type_de_vente='achat-comptant';
update stocks_vendu set type_de_vente='360' where type_de_vente='achat-compte-client';

alter table stocks_vendu change column `type_de_vente` type_de_vente int(12);
