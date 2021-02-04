alter table achats_de_services ADD column `designation` varchar(256) NOT NULL UNIQUE AFTER id;

UPDATE achats_de_services SET designation=reference;
