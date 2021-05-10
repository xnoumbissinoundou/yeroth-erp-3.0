alter table charges_financieres ADD column `designation` varchar(256) NOT NULL UNIQUE AFTER id;

UPDATE charges_financieres SET designation=reference;

alter table charges_financieres modify column `nom_entreprise_fournisseur` varchar(256) AFTER designation;
