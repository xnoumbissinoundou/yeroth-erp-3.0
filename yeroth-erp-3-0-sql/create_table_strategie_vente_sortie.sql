create table strategies_vente_sortie (id int primary key, nom_role varchar(256));
insert into strategies_vente_sortie values (0, "ALL");
insert into strategies_vente_sortie values (1, "FIFO");
insert into strategies_vente_sortie values (2, "DEF_DEO");