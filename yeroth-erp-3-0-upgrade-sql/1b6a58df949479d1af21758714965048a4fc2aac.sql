alter table comptes_bancaires modify column `reference_du_compte_bancaire` varchar(256) UNIQUE NOT NULL;
alter table comptes_bancaires modify column `intitule_du_compte_bancaire` varchar(256) UNIQUE NOT NULL;
