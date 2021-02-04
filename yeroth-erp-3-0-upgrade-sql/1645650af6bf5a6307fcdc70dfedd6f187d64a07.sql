alter table achats_de_services ADD column `designation` varchar(256) NOT NULL UNIQUE AFTER id;

UPDATE achats_de_services SET designation=reference;

alter table achats_de_services modify column `nom_entreprise_fournisseur` varchar(256) AFTER designation;
