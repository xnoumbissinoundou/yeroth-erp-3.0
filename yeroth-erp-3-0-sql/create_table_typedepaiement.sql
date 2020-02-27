create table type_de_paiement (id int primary key, type_de_paiement varchar(256));
insert into type_de_paiement (id, type_de_paiement) values (0, "versement (comptant)");
insert into type_de_paiement (id, type_de_paiement) values (1, "versement (cheque)");
insert into type_de_paiement (id, type_de_paiement) values (2, "versement (téléphone)");
insert into type_de_paiement (id, type_de_paiement) values (3, "versement (bancaire)");
insert into type_de_paiement (id, type_de_paiement) values (4, "versement (virement bancaire)");
