create table comptes_bancaires (id int auto_increment primary key, reference_du_compte_bancaire varchar(256), intitule_du_compte_bancaire varchar(256) UNIQUE NOT NULL, institut_bancaire varchar(256), description_du_compte_bancaire varchar(256));
