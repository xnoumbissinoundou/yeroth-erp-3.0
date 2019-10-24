create table dbms (id int primary key, nom_dbms varchar(256), url_dbms varchar(256), port_dbms int(6), magicNr mediumblob);
insert into dbms (id, nom_dbms, port_dbms) values (0, "MySQL", 3306);