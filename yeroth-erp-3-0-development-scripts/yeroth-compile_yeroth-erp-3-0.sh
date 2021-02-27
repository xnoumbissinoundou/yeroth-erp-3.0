#/bin/bash

SCRIPTBASENAME=$(basename $0)

USAGE="
       ------------------------------------------------------------
       'YEROTH-ERP-3.0' Build System
        @auteur: Xavier NOUMBISSI NOUNDOU, Dipl.-Inf., Ph.D. (ABD)
	@email:  xnoundou7@gmail.com
       ------------------------------------------------------------
       Usage: "$SCRIPTBASENAME"
	[-h] : help
	[-t] : compile 'yeroth-erp-3.0' with QT Test library activated
					for unit tests	
	[-r] : generates an official release build-executable,
	       with 'LAST BUILD ID' set
	[-s] : simulate 'yeroth-erp-3.0' compilation
	[-t] : compile 'yeroth-erp-3.0' with QT Test library activated
					for unit tests
	[-k] : compile 'yeroth-erp-3.0' to use with virtual keyboard
	[-c] : continue previous stopped compilation
	[-g] : compiles 'yeroth-erp-3.0' with debug information
	[-v <'yeroth-erp-3.0'-version>] :
	       'yeroth-erp-3.0'-version := YEROTH_CLIENT     | 
	       		        YEROTH_SERVER     | 
			        YEROTH_STANDALONE |
			        YEROTH_ACADEMIC_EVALUATION_VERSION
	[-e] : compiles 'yeroth-erp-3.0' in English
	[-f] : compiles 'yeroth-erp-3.0' in French
	[-j <n>] : compiles 'yeroth-erp-3.0' using 'n' number of jobs
       ------------------------------------------------------------"

NUMBER_OF_JOBS=4

yerothCheckMemoryFlag=
qtTestLibFlag=
virtualKeyboardFlag=
simulationFlag=
jobsFlag=
yerothVersionFlag=
debugFlag=
yerothVersionFlag=
continueFlag=
releaseBuildFlag=


while getopts 'mtkhsgv:fecj:r' OPTION
do
  case $OPTION in

    h)	echo "$USAGE"
				exit 1;
		;;

		s)	simulationFlag=1
      	simulationVal="$OPTARG"
        echo "simulation activée."
		;;

		t)	qtTestLibFlag=1
        echo "QT testlib incluse."
		;;

    k)	virtualKeyboardFlag=1
        echo "clavier virtuel activé."
		;;

		m)	yerothCheckMemoryFlag=1
     		yerothCheckMemoryVal="YEROTH_CHECK_MEMORY"
		;;

    g)	debugFlag=1
     		debugVal="YEROTH_DEBUG_LOG"
		;;

    v)	yerothVersionFlag=1
     		yerothVersionVal="$OPTARG"
		;;

    f)	languageFlag=1
      	languageVal="YEROTH_FRANCAIS_LANGUAGE"
		;;

    e)	languageFlag=1
      	languageVal="YEROTH_ENGLISH_LANGUAGE"
        echo "compilation pour la langue anglaise"
		;;

    c)	continueFlag=1
        echo "continue la compilation"
		;;

		r)	releaseBuildFlag=1
        echo "BUILD OFFICIEL: $(git rev-parse origin/master)"
		;;

    j)	jobsFlag=1
      	jobsVal="$OPTARG"
        echo "compilation activée avec '$jobsVal' jobs."
		;;

    ?)	printf "$USAGE\n" >&2
        exit 2
		;;

  esac
done
shift $(($OPTIND - 1))

set +x


if [ ! $yerothCheckMemoryFlag ]; then
  yerothCheckMemoryVal="NO_YEROTH_CHECK_MEMORY"
fi

if [ ! $debugFlag ]; then
  debugVal="YEROTH_NO_DEBUG"
fi

if [ $yerothVersionFlag ]; then
  if [ "YEROTH_CLIENT" = $yerothVersionVal ]; then
    echo "configure to compile 'yeroth-erp-3.0' client [YEROTH_CLIENT]"
    yerothVersionVal="YEROTH_CLIENT"
  elif [ "YEROTH_SERVER" = $yerothVersionVal ]; then
    echo "configure to compile 'yeroth-erp-3.0' server [YEROTH_SERVER]"
    yerothVersionVal="YEROTH_SERVER"
  elif [ "YEROTH_ACADEMIC_EVALUATION_VERSION" = $yerothVersionVal ]; then
    echo "configure to compile 'yeroth-erp-3.0' academic version [YEROTH_ACADEMIC_EVALUATION_VERSION]"
    yerothVersionVal="YEROTH_ACADEMIC_EVALUATION_VERSION"
  elif [ "YEROTH_STANDALONE" = $yerothVersionVal ]; then
    echo "configure to compile 'yeroth-erp-3.0' standalone version [YEROTH_STANDALONE]"
    yerothVersionVal="YEROTH_STANDALONE"
  else
    echo "No version configuration information from user"
    echo "configure to compile 'yeroth-erp-3.0' standalone version [YEROTH_STANDALONE]"
    yerothVersionVal="YEROTH_STANDALONE"
  fi
fi

if [ ! $yerothVersionFlag ]; then
    yerothVersionVal="YEROTH_STANDALONE"
fi

if [ ! $jobsFlag ]; then
  jobsVal="$NUMBER_OF_JOBS"
fi

if [ ! $languageFlag ]; then
  languageVal="YEROTH_FRANCAIS_LANGUAGE"
fi

if [ $qtTestLibFlag ]; then
		echo "["$SCRIPTBASENAME"] WORKING WITH FILE: ${YEROTH_ERP_3_0_HOME_FOLDER}/yeroth-erp-3-0.pro !"
		
		sed -i "5s/NO_YEROTH_ERP_3_0_TESTING_UNIT_TEST/YEROTH_ERP_3_0_TESTING_UNIT_TEST/" ${YEROTH_ERP_3_0_HOME_FOLDER}/yeroth-erp-3-0.pro
		
		qtTestLibVal="YEROTH_ERP_3_0_TESTING_UNIT_TEST"

	else

    qtTestLibVal="NO_YEROTH_ERP_3_0_TESTING_UNIT_TEST"
fi

if [ $virtualKeyboardFlag ]; then
    virtualKeyboardVal="YEROTH_ERP_3_0_TOUCH_SCREEN"
	else
    virtualKeyboardVal="NO_YEROTH_ERP_3_0_TOUCH_SCREEN"
fi


YEROTH_BUILD_DATE_TIMESTAMP="$(date '+%H:%M:%S-%s')"

YEROTH_BUILD_DATE="$(date '+%d-%m-%Y')"

YEROTH_GIT_PUSH_COMMIT_ID="$(git rev-parse origin/master)"

YEROTH_GIT_PUSH_COMMIT_ID_TEXT="LAST BUILD ID: '${YEROTH_GIT_PUSH_COMMIT_ID}'.\\\n\\\n\""

YEROTH_BUILD_COMPUTER="$(uname -srm)"

YEROTH_BUILD_COMPUTER_TEXT="ON BUILD_COMPUTER: '${YEROTH_BUILD_COMPUTER}'.\\\n\\\n\""

YEROTH_CURRENT_LOCALISATION_FOR_RELEASE_BUILD="${YEROTH_ERP_3_0_HOME_FOLDER}/yeroth-erp-3-0-development-scripts/YEROTH_ERP_3_0_CURRENT_LOCALISATION_FOR_RELEASE_BUILD"

YEROTH_ERP_3_0_STANDALONE_CURRENT_LOCALISATION_FOR_BUILD_RELEASE="CLIENT DEDICATED COMPANY LOCALISATION SITE: '$(sed -n '1,1p;1q' ${YEROTH_CURRENT_LOCALISATION_FOR_RELEASE_BUILD})'.\"));";

YEROTH_ERP_3_0_STANDALONE_CURRENT_LOCALISATION_FOR_BUILD_RELEASE_CONFIG_CPP="$(sed -n '1,1p;1q' ${YEROTH_CURRENT_LOCALISATION_FOR_RELEASE_BUILD})"


if [ $releaseBuildFlag ]; then
		sed -i "s/YEROTH_GIT_PUSH_COMMIT_ID/${YEROTH_GIT_PUSH_COMMIT_ID}/g" src/utils/yeroth-erp-utils.cpp
		sed -i "s/YEROTH_BUILD_DATE_TIMESTAMP/${YEROTH_BUILD_DATE_TIMESTAMP}/g" src/utils/yeroth-erp-utils.cpp
		sed -i "s/YEROTH_BUILD_DATE/${YEROTH_BUILD_DATE}/g" src/utils/yeroth-erp-utils.cpp
		sed -i "s/LAST BUILD ID: .*/${YEROTH_GIT_PUSH_COMMIT_ID_TEXT}/g" src/utils/yeroth-erp-utils.cpp
		sed -i "s/ON BUILD_COMPUTER: .*/${YEROTH_BUILD_COMPUTER_TEXT}/g" src/utils/yeroth-erp-utils.cpp
			src/utils/yeroth-erp-utils.cpp
		sed -i "s/CLIENT DEDICATED COMPANY LOCALISATION SITE: .*/${YEROTH_ERP_3_0_STANDALONE_CURRENT_LOCALISATION_FOR_BUILD_RELEASE}/g" src/utils/yeroth-erp-utils.cpp
		sed -i "s/THIS_SITE_LOCALISATION_HARD_CODED_BY_DEVELOPER_FOR_MAXIMUM_BENEFIT_FOR_CLIENT/${YEROTH_ERP_3_0_STANDALONE_CURRENT_LOCALISATION_FOR_BUILD_RELEASE_CONFIG_CPP}/g" src/utils/yeroth-erp-config.cpp
fi


if [ $continueFlag ]; then
	echo "make -j$jobsVal YEROTH_ERP_3_0_TEST=$qtTestLibVal YEROTH_VIRTUAL_KEYBOARD_OPTIONS=$virtualKeyboardVal YEROTH_DEBUG_LOG=$debugVal YEROTH_VERSION=$yerothVersionVal YEROTH_LANGUAGE=$languageVal YEROTH_CHECK_MEMORY=$yerothCheckMemoryVal > ${yerothVersionVal}.out.log	2> ${yerothVersionVal}.error.log"
else
	echo "make clean &&  make -j$jobsVal YEROTH_ERP_3_0_TEST=$qtTestLibVal YEROTH_VIRTUAL_KEYBOARD_OPTIONS=$virtualKeyboardVal YEROTH_DEBUG_LOG=$debugVal YEROTH_VERSION=$yerothVersionVal YEROTH_LANGUAGE=$languageVal YEROTH_CHECK_MEMORY=$yerothCheckMemoryVal > ${yerothVersionVal}.out.log	2> ${yerothVersionVal}.error.log"
fi


if [ $simulationFlag ]; then
	exit 3
fi


if [ ${languageVal} == "YEROTH_ENGLISH_LANGUAGE" ]; then
		cp yeroth-erp-3-0-english.qrc yeroth-erp-3-0.qrc
elif [ ${languageVal} == "YEROTH_FRANCAIS_LANGUAGE" ]; then
		cp yeroth-erp-3-0-french.qrc yeroth-erp-3-0.qrc
fi

qmake

if [ $continueFlag ]; then
	make -j$jobsVal \
		YEROTH_ERP_3_0_TEST=$qtTestLibVal \
		YEROTH_VIRTUAL_KEYBOARD_OPTIONS=$virtualKeyboardVal \
		YEROTH_CHECK_MEMORY=$yerothCheckMemoryVal \
		YEROTH_DEBUG_LOG=$debugVal \
		YEROTH_VERSION=$yerothVersionVal \
		YEROTH_LANGUAGE=$languageVal > "${yerothVersionVal}.out.log" \
		2> "${yerothVersionVal}.error.log" 
else
	make clean && \
	make -j$jobsVal \
		YEROTH_ERP_3_0_TEST=$qtTestLibVal \
		YEROTH_VIRTUAL_KEYBOARD_OPTIONS=$virtualKeyboardVal \
		YEROTH_CHECK_MEMORY=$yerothCheckMemoryVal \
		YEROTH_DEBUG_LOG=$debugVal \
		YEROTH_VERSION=$yerothVersionVal \
		YEROTH_LANGUAGE=$languageVal > "${yerothVersionVal}.out.log" \
		2> "${yerothVersionVal}.error.log" 
fi

BUILD_SUCCESSFUL="$?"

if [ ${BUILD_SUCCESSFUL} -eq 0 ] && [ $releaseBuildFlag ]; then
		git checkout src/utils/yeroth-erp-utils.cpp
fi

