alter table operations_comptables ADD column `numero_du_compte_doperation_comptable` varchar(256) NOT NULL AFTER numero_piece_justificative_doperation_comptable;
alter table comptes_doperations_comptables DROP column `type_doperation_comptable`;
alter table comptes_doperations_comptables change column `raison_doperation_comptable` raison_doperation_comptable varchar(256);
