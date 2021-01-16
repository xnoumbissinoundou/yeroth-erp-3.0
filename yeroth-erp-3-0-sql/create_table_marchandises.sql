create table marchandises  (id int primary key, reference varchar(256), designation varchar(256) NOT NULL, categorie varchar(256) NOT NULL, sous_categorie varchar(256) NOT NULL, prix_dachat_precedent double, prix_vente_precedent double, prix_vente_en_gros_precedent double, description_produit varchar(256), stock_dalerte double, stock_minimum double, stock_maximum double, quantite_totale double, is_service boolean NOT NULL);
