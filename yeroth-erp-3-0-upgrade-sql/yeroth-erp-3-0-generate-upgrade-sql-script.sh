#!/bin/bash
# @file: yeroth-erp-3-0-generate-upgrade-sql-script.sh
# @author: Xavier NOUMBISSI NOUNDOU, Dipl.-Inf., Ph.D. (ABD)

APP="$(basename $0)"

SQL_UPGRADE_FILE=yeroth-result-last-sql-upgrade.sql

rm -f "${SQL_UPGRADE_FILE}"

CANDIDATE_GIT_COMMIT_TO_SQL_UPGRADE="$(git rev-parse origin/master)"

LAST_BUILD_ID="$1"

if [ -z "${LAST_BUILD_ID}" ]; then
		echo "Usage: $APP <last_build_id>"
		exit 1
fi

# THIS SHOULD BE THE GIT_COMMIT JUST BEFORE THE 'LAST BUILD ID' !
ANCESTOR_GIT_COMMIT="$(git rev-list --parents -n 1 ${LAST_BUILD_ID} | awk '//{print $2}')"
#ANCESTOR_GIT_COMMIT="b65ee8a3aba45d3e79c76dfcc38db5f584ddc49e"

# A result of '0' means first argument commit is an
# ancestor to second argument commit.
# Therefore a sql upgrade script is necessary.
git merge-base --is-ancestor ${ANCESTOR_GIT_COMMIT} ${CANDIDATE_GIT_COMMIT_TO_SQL_UPGRADE}

CANDIDATE_GIT_COMMIT_IS_ANCESTOR="$?"

if [ "${CANDIDATE_GIT_COMMIT_IS_ANCESTOR}" -eq 0 ]; then
		#echo "$APP | commit ${CANDIDATE_GIT_COMMIT_TO_SQL_UPGRADE} is a descendant of commit ${ANCESTOR_GIT_COMMIT}";
		#echo "$APP | find all sql upgrade scripts required from commit ${ANCESTOR_GIT_COMMIT}";
		
		ALL_RELEVANT_GIT_COMMITS="$(git rev-list --skip=1 --reverse ^${ANCESTOR_GIT_COMMIT} ${CANDIDATE_GIT_COMMIT_TO_SQL_UPGRADE})"

		COUNTER=0
		sql_upgrade_file_generated=

		for c in ${ALL_RELEVANT_GIT_COMMITS}; do

				SQL_FILE="${c}.sql"
				
				if [ -f ${SQL_FILE} ]; then					
						
						if [ $COUNTER -eq 0 ]; then
								sql_upgrade_file_generated=1
								echo -e "-- YEROTH-ERP-3.0: '.sql' file to upgrade database." >> ${SQL_UPGRADE_FILE}
								echo -e "-- @author: Xavier NOUMBISSI NOUNDOU, Dipl.-Inf., Ph.D. (ABD)." >> ${SQL_UPGRADE_FILE}
								echo -e "-- from LAST BUILD ID: '${LAST_BUILD_ID}' to BUILD ID: '${CANDIDATE_GIT_COMMIT_TO_SQL_UPGRADE}'." >> ${SQL_UPGRADE_FILE}
								echo -e "" >> ${SQL_UPGRADE_FILE}
								let COUNTER=COUNTER+1
						fi
						
						echo "$APP | applies sql upgrade script ${SQL_FILE}";
						echo -e "-- CONTENT OF FILE '${SQL_FILE}'" >> ${SQL_UPGRADE_FILE}
						cat ${SQL_FILE} >> ${SQL_UPGRADE_FILE}
				fi
	  done

		if [ $sql_upgrade_file_generated ]; then
				echo "$APP | generated FINAL sql upgrade script ${SQL_UPGRADE_FILE}";
		fi
fi 
