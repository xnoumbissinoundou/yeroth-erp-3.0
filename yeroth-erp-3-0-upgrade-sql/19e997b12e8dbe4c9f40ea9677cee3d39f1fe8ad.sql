alter table paiements change column `reference` justification_transaction_client varchar(256);

alter table paiements ADD column `reference_recu_paiement_client` varchar(256) AFTER intitule_du_compte_bancaire;
