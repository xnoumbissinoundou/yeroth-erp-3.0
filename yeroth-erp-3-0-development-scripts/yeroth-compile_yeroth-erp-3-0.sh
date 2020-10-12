#/bin/bash

USAGE="
       ------------------------------------------------------------
       'YEROTH-ERP-3.0' Build System
        @auteur: Xavier NOUMBISSI NOUNDOU, Dipl.-Inf., Ph.D. (ABD)
	@email:  xnoundou7@gmail.com
       ------------------------------------------------------------
       Usage: $(basename $0)
	[-h] : help
	[-t] : compile 'yeroth-erp-3.0' with QT Test library activated
					for unit tests	
	[-r] : generates an official release build-executable,
	       with 'LAST BUILD ID' set
	[-s] : simulate 'yeroth-erp-3.0' compilation
	[-t] : compile 'yeroth-erp-3.0' with QT Test library activated
					for unit tests
	[-l] : compile 'yeroth-erp-3.0' to use with virtual keyboard
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

qtTestLibFlag=
virtualKeyboardFlag=
simulationFlag=
jobsFlag=
yerothVersionFlag=
debugFlag=
yerothVersionFlag=
continueFlag=
releaseBuildFlag=


while getopts 'tlhsgv:fecj:r' OPTION
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

    l)	virtualKeyboardFlag=1
        echo "clavier virtuel activé."
		;;

    g)	debugFlag=1
     		debugVal="YEROTH_DEBUG_LOG"
		;;

    v)	yerothVersionFlag=1
     		yerothVersionVal="$OPTARG"
		;;

    f)	languageFlag=1
      	languageVal="YEROTH_FRANCAIS_LANGUAGE"
        echo "compilation pour la langue francaise"
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

YEROTH_BUILD_COMPUTER_TEXT="ON BUILD_COMPUTER: '${YEROTH_BUILD_COMPUTER}'.\"));"

if [ $releaseBuildFlag ]; then
		sed -i "s/YEROTH_GIT_PUSH_COMMIT_ID/${YEROTH_GIT_PUSH_COMMIT_ID}/g" src/utils/yeroth-erp-utils.cpp
		sed -i "s/YEROTH_BUILD_DATE_TIMESTAMP/${YEROTH_BUILD_DATE_TIMESTAMP}/g" src/utils/yeroth-erp-utils.cpp
		sed -i "s/YEROTH_BUILD_DATE/${YEROTH_BUILD_DATE}/g" src/utils/yeroth-erp-utils.cpp
		sed -i "s/LAST BUILD ID: .*/${YEROTH_GIT_PUSH_COMMIT_ID_TEXT}/g" src/utils/yeroth-erp-utils.cpp
		sed -i "s/ON BUILD_COMPUTER: .*/${YEROTH_BUILD_COMPUTER_TEXT}/g" src/utils/yeroth-erp-utils.cpp
fi


if [ $simulationFlag ]; then
		if [ $qtTestLibFlag ]; then
				if [ $continueFlag ]; then
						echo "make -j$jobsVal YEROTH_ERP_3_0_TEST=$qtTestLibVal YEROTH_VIRTUAL_KEYBOARD_OPTIONS=$virtualKeyboardVal YEROTH_DEBUG_LOG=$debugVal YEROTH_VERSION=$yerothVersionVal YEROTH_LANGUAGE=$languageVal"
				else
						echo "make clean &&  make -j$jobsVal YEROTH_ERP_3_0_TEST=$qtTestLibVal YEROTH_VIRTUAL_KEYBOARD_OPTIONS=$virtualKeyboardVal YEROTH_DEBUG_LOG=$debugVal YEROTH_VERSION=$yerothVersionVal YEROTH_LANGUAGE=$languageVal"
				fi
		else
				if [ $continueFlag ]; then
						echo "make -j$jobsVal YEROTH_VIRTUAL_KEYBOARD_OPTIONS=$virtualKeyboardVal YEROTH_DEBUG_LOG=$debugVal YEROTH_VERSION=$yerothVersionVal YEROTH_LANGUAGE=$languageVal"
				else
						echo "make clean && make -j$jobsVal YEROTH_VIRTUAL_KEYBOARD_OPTIONS=$virtualKeyboardVal YEROTH_DEBUG_LOG=$debugVal YEROTH_VERSION=$yerothVersionVal YEROTH_LANGUAGE=$languageVal"
				fi
		fi
		exit 3
fi

if [ ${languageVal} == "YEROTH_ENGLISH_LANGUAGE" ]; then
		cp yeroth-erp-3-0-english.qrc yeroth-erp-3-0.qrc
elif [ ${languageVal} == "YEROTH_FRANCAIS_LANGUAGE" ]; then
		cp yeroth-erp-3-0-french.qrc yeroth-erp-3-0.qrc
fi

qmake

if [ $qtTestLibFlag ]; then
		if [ $continueFlag ]; then
				make -j$jobsVal \
						YEROTH_ERP_3_0_TEST=$qtTestLibVal \
						YEROTH_VIRTUAL_KEYBOARD_OPTIONS=$virtualKeyboardVal \
						YEROTH_DEBUG_LOG=$debugVal \
						YEROTH_VERSION=$yerothVersionVal \
						YEROTH_LANGUAGE=$languageVal
		else
				make clean && \
				make -j$jobsVal \
						YEROTH_ERP_3_0_TEST=$qtTestLibVal \
						YEROTH_VIRTUAL_KEYBOARD_OPTIONS=$virtualKeyboardVal \
						YEROTH_DEBUG_LOG=$debugVal \
						YEROTH_VERSION=$yerothVersionVal \
						YEROTH_LANGUAGE=$languageVal
		fi
else
		if [ $continueFlag ]; then
				make -j$jobsVal \
						YEROTH_ERP_3_0_TEST=$qtTestLibVal \
						YEROTH_VIRTUAL_KEYBOARD_OPTIONS=$virtualKeyboardVal \
						YEROTH_DEBUG_LOG=$debugVal \
						YEROTH_VERSION=$yerothVersionVal \
						YEROTH_LANGUAGE=$languageVal
		else
				make clean && \
				make -j$jobsVal \
						YEROTH_ERP_3_0_TEST=$qtTestLibVal \
						YEROTH_VIRTUAL_KEYBOARD_OPTIONS=$virtualKeyboardVal \
						YEROTH_DEBUG_LOG=$debugVal \
						YEROTH_VERSION=$yerothVersionVal \
						YEROTH_LANGUAGE=$languageVal
		fi
fi


BUILD_SUCCESSFUL="$?"

if [ ${BUILD_SUCCESSFUL} -eq 0 ] && [ $releaseBuildFlag ]; then
		git checkout src/utils/yeroth-erp-utils.cpp
fi

