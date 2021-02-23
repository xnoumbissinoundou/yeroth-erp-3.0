#!/bin/bash

USAGE="$(basename $0) [-e] <-v [qsqlite3 | standalone | academic | client | server]>"


function chown_and_grp_for_lintian ()
{
		FILE_OR_FOLDER="$1"

		sudo chown -R root "${FILE_OR_FOLDER}"
		sudo chgrp -R root "${FILE_OR_FOLDER}"
}


# We don't want to have temporary files "*.bak"
# within our '.deb' packages
find . -name "*.bak" -delete

if [ $# -lt 2 ]; then
 echo "${USAGE}"
 exit 1
fi 

yerothpos7VersionFlag=
qsqlite3Flag=

while getopts 's:c:ev:' OPTION
do
  case $OPTION in
    s)	yerothpos7VersionFlag=1
			  yerothpos7VersionVal="$OPTARG"
       	echo "Creating .deb file, for ${yerothpos7VersionVal} version of Yeroth-erp-3.0"
        ;;

    c)	yerothpos7VersionFlag=1
			  yerothpos7VersionVal="$OPTARG"
       	echo "Creating .deb file, for ${yerothpos7VersionVal} version of Yeroth-erp-3.0"
        ;;

    v)	yerothpos7VersionFlag=1
			  yerothpos7VersionVal="$OPTARG"
       	echo "Creating .deb file, for ${yerothpos7VersionVal} version of Yeroth-erp-3.0"
        ;;
    e)	qsqlite3Flag=1
       	echo "Creating evaluation .deb file, using qsqlite3"
        ;;
    ?)	printf "$USAGE" >&2
        exit 2
	;;
  esac
done
shift $(($OPTIND - 1))

if [ ! "$yerothpos7VersionFlag" ]; then
  echo "${USAGE}"
	exit 3
fi

if [ "$yerothpos7VersionVal" == "client" ]; then
	echo "yerothpos7VersionVal: client"
elif [ "$yerothpos7VersionVal" == "server" ]; then
	echo "yerothpos7VersionVal: server"
elif [ "$yerothpos7VersionVal" == "standalone" ]; then
	echo "yerothpos7VersionVal: standalone"
elif [ "$yerothpos7VersionVal" == "academic" ]; then
	echo "yerothpos7VersionVal: academic"
else
	echo "$(basename $0) | please, give a correct Yeroth-erp-3.0 version"
	echo "${USAGE}"	
	exit 4
fi

set -x

YEROTH_ERP_3_0_LICENSE_TYPE="${yerothpos7VersionVal}"

YEROTH_ERP_3_0_BINARY_NAME="yeroth-erp-3.0-${YEROTH_ERP_3_0_LICENSE_TYPE}-configurations-data-ENGLISH"

YEROTH_ERP_3_0_DEB_FILE_NAME="${YEROTH_ERP_3_0_HOME_FOLDER}/${YEROTH_ERP_3_0_BINARY_NAME}.deb"

TARGET_INSTALLATION_FOLDER="${YEROTH_ERP_3_0_HOME_FOLDER}/${YEROTH_ERP_3_0_BINARY_NAME}"

if [ ! -d "${TARGET_INSTALLATION_FOLDER}" ]; then
    echo "creating installation directory: ${TARGET_INSTALLATION_FOLDER}"
    mkdir -p "${TARGET_INSTALLATION_FOLDER}"
fi


OPT_INSTALLATION_BASE_TARGET_DIR="${TARGET_INSTALLATION_FOLDER}/opt"

OPT_INSTALLATION_TARGET_DIR="${OPT_INSTALLATION_BASE_TARGET_DIR}/${YEROTH_ERP_3_0_BINARY_NAME}"

mkdir -p "${OPT_INSTALLATION_TARGET_DIR}"

YEROTH_SQL_BACKUP_FOLDER_NAME="yeroth-erp-3.0-sql-backup-restore"

TARGET_INSTALLATION_SQL_BACKUP_FOLDER="${OPT_INSTALLATION_BASE_TARGET_DIR}/${YEROTH_SQL_BACKUP_FOLDER_NAME}"

mkdir -p "${TARGET_INSTALLATION_SQL_BACKUP_FOLDER}"

CP="cp"

YEROTH_ERP_3_0_DEB_PACKAGE_INFORMATION="Package: ${YEROTH_ERP_3_0_BINARY_NAME}"

mkdir "${TARGET_INSTALLATION_FOLDER}/DEBIAN"


if [ ! "$qsqlite3Flag" ]; then
	${CP} ${YEROTH_ERP_3_0_HOME_FOLDER}/DEBIAN/control_configurations_data-ENGLISH "${TARGET_INSTALLATION_FOLDER}/DEBIAN/control"
	${CP} ${YEROTH_ERP_3_0_HOME_FOLDER}/DEBIAN/postinst "${TARGET_INSTALLATION_FOLDER}/DEBIAN/postinst"
else
	${CP} ${YEROTH_ERP_3_0_HOME_FOLDER}/DEBIAN/control_academic "${TARGET_INSTALLATION_FOLDER}/DEBIAN/control"
fi


echo -e "${YEROTH_ERP_3_0_DEB_PACKAGE_INFORMATION}$(cat ${TARGET_INSTALLATION_FOLDER}/DEBIAN/control)" > ${TARGET_INSTALLATION_FOLDER}/DEBIAN/control


if [ ! "$qsqlite3Flag" ]; then
	${CP} ${YEROTH_ERP_3_0_HOME_FOLDER}/yeroth-erp-3-0-deployment-configuration-scripts/yeroth-erp-3-0.properties \
		"${OPT_INSTALLATION_TARGET_DIR}"

	${CP} ${YEROTH_ERP_3_0_HOME_FOLDER}/yeroth-erp-3-0-deployment-configuration-scripts/yeroth-erp-3-0-system-local-configuration.properties \
			"${OPT_INSTALLATION_TARGET_DIR}"
else
	${CP} ${YEROTH_ERP_3_0_HOME_FOLDER}/yeroth-erp-3-0-deployment-configuration-scripts/yeroth-erp-3-0-qsqlite3.properties \
			"${OPT_INSTALLATION_TARGET_DIR}"

	${CP} ${YEROTH_ERP_3_0_HOME_FOLDER}/yeroth-erp-3-0-deployment-configuration-scripts/yeroth-erp-3-0-system-local-configuration.properties \
			"${OPT_INSTALLATION_TARGET_DIR}"
fi

YEROTH_ERP_3_0_POSTRM_STR="#!/bin/bash
sed -i '/YEROTH_ERP_3_0_PROPERTIES_CONFIGURATION_FOLDER/d' /etc/environment"

echo -e "${YEROTH_ERP_3_0_POSTRM_STR}$(cat ${TARGET_INSTALLATION_FOLDER}/DEBIAN/postrm)" > ${TARGET_INSTALLATION_FOLDER}/DEBIAN/postrm

chmod 755 ${TARGET_INSTALLATION_FOLDER}/DEBIAN/postrm

YEROTH_ERP_3_0_POSTINST_STR="#!/bin/bash
echo -e \"export YEROTH_ERP_3_0_PROPERTIES_CONFIGURATION_FOLDER=/opt/${YEROTH_ERP_3_0_BINARY_NAME}\" >> /etc/environment
chmod go+rw /opt/${YEROTH_SQL_BACKUP_FOLDER_NAME}
chmod go+rw /opt/${YEROTH_ERP_3_0_BINARY_NAME}/yeroth-erp-3-0-system-local-configuration.properties"

echo -e "${YEROTH_ERP_3_0_POSTINST_STR}$(cat ${TARGET_INSTALLATION_FOLDER}/DEBIAN/postinst)" > ${TARGET_INSTALLATION_FOLDER}/DEBIAN/postinst

chmod 755 ${TARGET_INSTALLATION_FOLDER}/DEBIAN/postinst

# ********************************************************************************** #

rm -f ${YEROTH_ERP_3_0_DEB_FILE_NAME}

TARGET_INSTALLATION_FOLDER_DEBIAN="${TARGET_INSTALLATION_FOLDER}/DEBIAN"

TARGET_INSTALLATION_FOLDER_OPT="${TARGET_INSTALLATION_FOLDER}/opt"

chown_and_grp_for_lintian "${TARGET_INSTALLATION_FOLDER_DEBIAN}"

chown_and_grp_for_lintian "${TARGET_INSTALLATION_FOLDER_OPT}"

dpkg-deb --verbose --build ${YEROTH_ERP_3_0_HOME_FOLDER}/${YEROTH_ERP_3_0_BINARY_NAME}
