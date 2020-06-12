#/bin/bash
#@author: Xavier NOUMBISSI NOUNDOU, Dipl.-Inf., Ph.D. (ABD)

USAGE="
       ------------------------------------------------------------
       'YEROTH-ERP-3.0' Unit Testing Running System
        @auteur: Xavier NOUMBISSI NOUNDOU, DIPL.-INF., PH.D. (ABD)
       ------------------------------------------------------------
       Usage: $(basename $0)
	[-h] : help
	[-s] : simulate 'yeroth-erp-3.0' unit testing run
	[-c] : run 'yeroth-erp-3.0' unit test and also generates
	        coverage information with 'gcov'
       ------------------------------------------------------------"

jobsVal=2
jobsFlag=
buildFlag=
simulationFlag=
coverageGenFlag=

while getopts 'cbj:sh' OPTION
do
  case $OPTION in

		h)	echo "$USAGE"
				exit 1;
		;;

		s)	simulationFlag=1
        echo "simulation activée."
		;;

		j)	jobsFlag=1
      	jobsVal="$OPTARG"
        echo "compilation activée avec '$jobsVal' jobs."
		;;


		b)	buildFlag=1
		;;

    c)	coverageGenFlag=1
		;;

    ?)	printf "$USAGE\n" >&2
        exit 2
		;;

  esac
done
shift $(($OPTIND - 1))


if [ $buildFlag ]; then
		if [ $simulationFlag ]; then
				echo "yeroth-compile_yeroth-erp-3-0-standalone.sh -j${jobsVal} -c"
		else
				yeroth-compile_yeroth-erp-3-0-standalone.sh -j${jobsVal} -c
		fi
fi


if [ $simulationFlag ]; then
		echo "${YEROTH_ERP_3_0_HOME_FOLDER}/bin/yeroth-erp-3.0-standalone > test-results-yeroth-erp-3-0.txt 2>&1"
else
		${YEROTH_ERP_3_0_HOME_FOLDER}/bin/yeroth-erp-3.0-standalone > test-results-yeroth-erp-3-0.txt 2>&1
fi


if [ $coverageGenFlag ]; then
		if [ $simulationFlag ]; then
				echo "run-test-yeroth-erp-3-0-coverage-gen-html.sh"
		else
				run-test-yeroth-erp-3-0-coverage-gen-html.sh
		fi
fi
