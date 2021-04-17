#!/bin/sh

SCRIPTBASENAME=$(basename $0)

USAGE="
------------------------------------------------------------
'YEROTH-ERP-3.0' Build System
 @auteur: DR. XAVIER NOUMBISSI NOUNDOU
 @email:  yerotherp30@gmail.com
------------------------------------------------------------
 Usage: "$SCRIPTBASENAME"
 [-h] : help
 [-i] : input file containing user enabling actions in FORM
				'action,false', or 'action,true'
 [-o] : generates an official release build-executable,
	      with 'LAST BUILD ID' set
------------------------------------------------------------"

NUMBER_OF_JOBS=4

rawFlag=
inputFlag=
outputFlag=

while getopts 'rhi:o:' OPTION
do
  case $OPTION in

    h)	echo "$USAGE"
				exit 1;
		;;

		r)	rawFlag=1
		;;
	
	  i)	inputFlag=1
      	inputVal="$OPTARG"
		;;
	
		o)	outputFlag=1
      	outputVal="$OPTARG"
		;;

    ?)	printf "$USAGE\n" >&2
        exit 2
		;;

  esac
done
shift $(($OPTIND - 1))

YEROTH_FILE_INPUT=""

YEROTH_FILE_OUTPUT=""

if [ ! $inputFlag ]; then
	echo "$USAGE"
	echo "... MISSING INPUT FILE NAME ..."
	exit 1
else
	YEROTH_FILE_INPUT="${inputVal}"
fi

if [ ! $outputFlag ]; then
	echo "$USAGE"
	echo "... MISSING OUTPUT FILE NAME ..."
	exit 3
else
	YEROTH_FILE_OUTPUT="${outputVal}"
fi


echo "${SCRIPTBASENAME} | WORKING WITH INPUT FILE: ${YEROTH_FILE_INPUT}"

echo "${SCRIPTBASENAME} | WORKING WITH OUTPUT FILE: ${YEROTH_FILE_OUTPUT}"


# FOLLOWING LINE ISOLATE A LINE PATTERN AS: 'YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEntrer, false);'
# TO RESULTING ACTION USER WORD: 'actionEntrer, false'.

# OPTION '-F' OF AWK ENABLES US TO SPECIFY A SEPARATING PATTERN FOR
# WORD OTHER THAN EMPTY SPACE (' ')!

YEROTH_TEXT_TO_PLACE_INSIDE_CODE=""

if [ $rawFlag ]; then
	YEROTH_TEMPORARY_USER_ACTION_FILE_CONTENT="${YEROTH_FILE_INPUT%%".txt"}-raw.txt"
	cat "${YEROTH_FILE_INPUT}" | awk -F'(' '//{print $2}' | awk -F')' '//{print $1}' > "${YEROTH_TEMPORARY_USER_ACTION_FILE_CONTENT}"
	
	YEROTH_TEXT_TO_PLACE_INSIDE_CODE=`cat "${YEROTH_TEMPORARY_USER_ACTION_FILE_CONTENT}" | \
	awk '//{print "YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(" $0 ");" }'`

	exit 7
else
	YEROTH_TEXT_TO_PLACE_INSIDE_CODE=`cat "${YEROTH_FILE_INPUT}" | \
	awk '//{print "YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(" $0 "); " }'`
fi

YEROTH_TEMP_OUTPUT=""

counter=0
for i in ${YEROTH_TEXT_TO_PLACE_INSIDE_CODE}; do
	if [ $counter -gt 0	]; then
		YEROTH_TEMP_OUTPUT="${YEROTH_TEMP_OUTPUT}\\\\n    $i"
	else
		YEROTH_TEMP_OUTPUT="$i"
	fi
	#echo "$counter: ${YEROTH_TEMP_OUTPUT}"
	counter=$((counter+1))
done

YEROTH_TEXT_TO_PLACE_INSIDE_CODE="${YEROTH_TEMP_OUTPUT}"
#echo "${YEROTH_TEXT_TO_PLACE_INSIDE_CODE}"

echo "${YEROTH_TEXT_TO_PLACE_INSIDE_CODE}" > "${YEROTH_FILE_OUTPUT}"


