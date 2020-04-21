#!/bin/bash
# @file: yeroth-erp-3-0-generate-upgrade-sql-script.sh
# @author: Xavier NOUMBISSI NOUNDOU, Dipl.-Inf., Ph.D. (ABD)

APP="$(basename $0)"

SQL_UPGRADE_FILE=yeroth-result-last-sql-upgrade.sql

rm -f "${SQL_UPGRADE_FILE}"

CANDIDATE_COMMIT_TO_SQL_UPGRADE="$(git rev-parse origin/master)"

LAST_BUILD_ID="f892fef6686bc05c60801ed5eca49f6c99a968fd"

# THIS SHOULD BE THE COMMIT JUST BEFORE THE 'LAST BUILD ID' !
ANCESTOR_COMMIT="$(git rev-list --parents -n 1 ${LAST_BUILD_ID} | awk '//{print $2}')"
#ANCESTOR_COMMIT="b65ee8a3aba45d3e79c76dfcc38db5f584ddc49e"

git merge-base --is-ancestor ${ANCESTOR_COMMIT} ${CANDIDATE_COMMIT_TO_SQL_UPGRADE}

# A result of '0' means first commit is an ancestor
# to second commit.
# Therefore a sql upgrade script is necessary.

CANDIDATE_COMMIT_IS_ANCESTOR="$?"

if [ "${CANDIDATE_COMMIT_IS_ANCESTOR}" -eq 0 ]; then
		#echo "$APP | commit ${CANDIDATE_COMMIT_TO_SQL_UPGRADE} is a descendant of commit ${ANCESTOR_COMMIT}";
		#echo "$APP | find all sql upgrade scripts required from commit ${ANCESTOR_COMMIT}";
		
		ALL_RELEVANT_COMMITS="$(git rev-list --skip=1 --reverse ^${ANCESTOR_COMMIT} ${CANDIDATE_COMMIT_TO_SQL_UPGRADE})"

		sql_upgrade_file_generated= 
		for c in ${ALL_RELEVANT_COMMITS}; do
				SQL_FILE="${c}.sql"
				if [ -f ${SQL_FILE} ]; then
						echo "$APP | applies sql upgrade script ${SQL_FILE}";
						cat ${SQL_FILE} >> ${SQL_UPGRADE_FILE}
						sql_upgrade_file_generated=1
				fi
	  done

		if [ $sql_upgrade_file_generated ]; then
				echo "$APP | generated FINAL sql upgrade script ${SQL_UPGRADE_FILE}";
		fi
fi 
