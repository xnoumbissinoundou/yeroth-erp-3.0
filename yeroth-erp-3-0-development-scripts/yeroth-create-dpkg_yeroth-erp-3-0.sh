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
       	echo "Creating .deb file, for ${yerothpos7VersionVal} version of Yeroth-erp-3-0"
        ;;

    c)	yerothpos7VersionFlag=1
			  yerothpos7VersionVal="$OPTARG"
       	echo "Creating .deb file, for ${yerothpos7VersionVal} version of Yeroth-erp-3-0"
        ;;

    v)	yerothpos7VersionFlag=1
			  yerothpos7VersionVal="$OPTARG"
       	echo "Creating .deb file, for ${yerothpos7VersionVal} version of Yeroth-erp-3-0"
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
	echo "$(basename $0) | please, give a correct Yeroth-erp-3-0 version"
	echo "${USAGE}"	
	exit 4
fi

set -x

USR_SHARE="/usr/share"

YEROTH_ERP_3_0_USER_LOCAL_SETTINGS_FOLDER=".yeroth_erp_3_0"

YEROTH_ERP_3_0_DEB_FILE_FOLDER="${YEROTH_ERP_3_0_HOME_FOLDER}/$yeroth-erp-3-0-deb-file-repository"

YEROTH_ERP_3_0_LICENSE_TYPE="${yerothpos7VersionVal}"

YEROTH_ERP_3_0_BINARY_NAME="yeroth-erp-3-0-${YEROTH_ERP_3_0_LICENSE_TYPE}"

YEROTH_ERP_3_0_BINARY_NAME_FOR_FILE="yeroth-erp-3-0-${YEROTH_ERP_3_0_LICENSE_TYPE}"

YEROTH_ERP_3_0_DEB_FILE_NAME="${YEROTH_ERP_3_0_HOME_FOLDER}/${YEROTH_ERP_3_0_BINARY_NAME}.deb"

YEROTH_ERP_3_0_BINARY_FILE_PATH="${YEROTH_ERP_3_0_HOME_FOLDER}/bin/${YEROTH_ERP_3_0_BINARY_NAME}"


TARGET_INSTALLATION_FOLDER="${YEROTH_ERP_3_0_HOME_FOLDER}/${YEROTH_ERP_3_0_BINARY_NAME}"

if [ ! -d "${TARGET_INSTALLATION_FOLDER}" ]; then
    echo "creating installation directory: ${TARGET_INSTALLATION_FOLDER}"
    mkdir -p "${TARGET_INSTALLATION_FOLDER}"
fi


OPT_INSTALLATION_TARGET_DIR="${TARGET_INSTALLATION_FOLDER}/opt/${YEROTH_ERP_3_0_BINARY_NAME}"

mkdir -p "${OPT_INSTALLATION_TARGET_DIR}"

TARGET_INSTALLATION_FOLDER_BIN="${OPT_INSTALLATION_TARGET_DIR}/bin"

mkdir -p "${TARGET_INSTALLATION_FOLDER_BIN}"

TARGET_INSTALLATION_USER_LOCAL_SETTINGS_FOLDER="${OPT_INSTALLATION_TARGET_DIR}/${YEROTH_ERP_3_0_USER_LOCAL_SETTINGS_FOLDER}"

mkdir -p --mode=777 "${TARGET_INSTALLATION_USER_LOCAL_SETTINGS_FOLDER}"

TARGET_INSTALLATION_FOLDER_DOC="${TARGET_INSTALLATION_FOLDER}${USR_SHARE}/doc/yeroth-erp-3-0"

mkdir -p "${TARGET_INSTALLATION_FOLDER_DOC}"

YEROTH_ERP_3_0_PROPERTIES_FILE_DIR=${OPT_INSTALLATION_TARGET_DIR}

touch "$OPT_INSTALLATION_TARGET_DIR/yeroth-erp-3-0.log"

ECHO="echo"

CP="cp"

YEROTH_ERP_3_0_DEB_PACKAGE_INFORMATION="Package: ${YEROTH_ERP_3_0_BINARY_NAME}"

mkdir "${TARGET_INSTALLATION_FOLDER}/DEBIAN"


if [ ! "$qsqlite3Flag" ]; then
	${CP} ${YEROTH_ERP_3_0_HOME_FOLDER}/DEBIAN/control "${TARGET_INSTALLATION_FOLDER}/DEBIAN/control"
	${CP} ${YEROTH_ERP_3_0_HOME_FOLDER}/DEBIAN/postinst "${TARGET_INSTALLATION_FOLDER}/DEBIAN/postinst"
	${CP} ${YEROTH_ERP_3_0_HOME_FOLDER}/DEBIAN/postrm "${TARGET_INSTALLATION_FOLDER}/DEBIAN/postrm"
else
	${CP} ${YEROTH_ERP_3_0_HOME_FOLDER}/DEBIAN/control_academic "${TARGET_INSTALLATION_FOLDER}/DEBIAN/control"
fi


${ECHO} -e "${YEROTH_ERP_3_0_DEB_PACKAGE_INFORMATION}$(cat ${TARGET_INSTALLATION_FOLDER}/DEBIAN/control)" > ${TARGET_INSTALLATION_FOLDER}/DEBIAN/control


YEROTH_ERP_3_0_POSTRM_STR="#!/bin/bash
sed -i '/YEROTH_ERP_3_0_HOME_FOLDER/d' /etc/environment
sed -i '/YEROTH_ERP_3_0_SYSTEM_DAEMON_HOME_FOLDER/d' /etc/environment"

${ECHO} -e "${YEROTH_ERP_3_0_POSTRM_STR}$(cat ${TARGET_INSTALLATION_FOLDER}/DEBIAN/postrm)" > ${TARGET_INSTALLATION_FOLDER}/DEBIAN/postrm

chmod 755 ${TARGET_INSTALLATION_FOLDER}/DEBIAN/postrm


YEROTH_ERP_3_0_POSTINST_STR="#!/bin/bash
echo -e \"export YEROTH_ERP_3_0_HOME_FOLDER=/opt/${YEROTH_ERP_3_0_BINARY_NAME}\" >> /etc/environment
echo -e \"export YEROTH_ERP_3_0_SYSTEM_DAEMON_HOME_FOLDER=/opt/yeroth-erp-3-0-system-daemon\" >> /etc/environment
chmod go+w /opt/${YEROTH_ERP_3_0_BINARY_NAME}/yeroth-erp-3-0.log"

${ECHO} -e "${YEROTH_ERP_3_0_POSTINST_STR}$(cat ${TARGET_INSTALLATION_FOLDER}/DEBIAN/postinst)" > ${TARGET_INSTALLATION_FOLDER}/DEBIAN/postinst

chmod 755 ${TARGET_INSTALLATION_FOLDER}/DEBIAN/postinst

mount -t tmpfs -o size=256K yerotherpthreedotzero_ramdisk "/opt/${YEROTH_ERP_3_0_BINARY_NAME}/${YEROTH_ERP_3_0_USER_LOCAL_SETTINGS_FOLDER}"

${ECHO} "*** YEROTH *** ${YEROTH_ERP_3_0_BINARY_FILE_PATH}"

${CP} ${YEROTH_ERP_3_0_BINARY_FILE_PATH} ${TARGET_INSTALLATION_FOLDER_BIN}

${CP} ${YEROTH_ERP_3_0_BINARY_FILE_PATH}.sh ${TARGET_INSTALLATION_FOLDER_BIN}

# ********************************************************************************** #

YEROTH_ERP_3_0_DOCUMENTATION_FRANCAIS_ROOT_DIR="${YEROTH_ERP_3_0_HOME_FOLDER}/doc/francais"

SOURCE_DOCUMENTATION_FRANCAIS_FICHE_DE_DONNEES_DIR="${YEROTH_ERP_3_0_DOCUMENTATION_FRANCAIS_ROOT_DIR}/yeroth-fiche-de-donnees"

FRANCAIS_COMPARAISON_ERP_DIR="${YEROTH_ERP_3_0_DOCUMENTATION_FRANCAIS_ROOT_DIR}/yeroth-erp-document-comparaisons"

SOURCE_DOCUMENTATION_FRANCAIS_WHITE_PAPERS_DIR="${YEROTH_ERP_3_0_DOCUMENTATION_FRANCAIS_ROOT_DIR}/yeroth-whitepapers"

SOURCE_DOCUMENTATION_FRANCAIS_BROCHURE_DIR="${YEROTH_ERP_3_0_DOCUMENTATION_FRANCAIS_ROOT_DIR}/yeroth-brochure"

FRANCAIS_COMPARAISON_ERP_PDF_CHEMIN="${FRANCAIS_COMPARAISON_ERP_DIR}/yeroth-erp-3-0-document-comparaisons.pdf"

FRANCAIS_FICHE_DE_DONNEES_PDF_CHEMIN="${SOURCE_DOCUMENTATION_FRANCAIS_FICHE_DE_DONNEES_DIR}/yeroth-erp-3-0-fiche-de-donnees.pdf"

FRANCAIS_WHITE_PAPERS_PDF_CHEMIN="${SOURCE_DOCUMENTATION_FRANCAIS_WHITE_PAPERS_DIR}/yeroth-erp-*.pdf"

FRANCAIS_BROCHURE_PDF_CHEMIN="${SOURCE_DOCUMENTATION_FRANCAIS_BROCHURE_DIR}/yeroth-erp-3-0-brochure.pdf"

FRANCAIS_MANUEL_DE_LUTILISATEUR_CHEMIN="${YEROTH_ERP_3_0_DOCUMENTATION_FRANCAIS_ROOT_DIR}/guide-*/*.pdf"

mkdir -p "${TARGET_INSTALLATION_FOLDER_DOC}/francais"

${CP} ${FRANCAIS_FICHE_DE_DONNEES_PDF_CHEMIN} "${TARGET_INSTALLATION_FOLDER_DOC}/francais"

${CP} ${FRANCAIS_COMPARAISON_ERP_PDF_CHEMIN} "${TARGET_INSTALLATION_FOLDER_DOC}/francais"

${CP} ${FRANCAIS_WHITE_PAPERS_PDF_CHEMIN} "${TARGET_INSTALLATION_FOLDER_DOC}/francais"

${CP} ${FRANCAIS_BROCHURE_PDF_CHEMIN} "${TARGET_INSTALLATION_FOLDER_DOC}/francais"

${CP} ${FRANCAIS_MANUEL_DE_LUTILISATEUR_CHEMIN} "${TARGET_INSTALLATION_FOLDER_DOC}/francais"

# ********************************************************************************** #

USR_SHARE_PIXMAPS="${USR_SHARE}/pixmaps"

USR_SHARE_APPLICATIONS="${USR_SHARE}/applications"

TARGET_INSTALLATION_FOLDER_USR_SHARE_PIXMAPS="${TARGET_INSTALLATION_FOLDER}${USR_SHARE_PIXMAPS}"

mkdir -p "${TARGET_INSTALLATION_FOLDER_USR_SHARE_PIXMAPS}"

TARGET_INSTALLATION_FOLDER_USR_SHARE_APPLICATIONS="${TARGET_INSTALLATION_FOLDER}${USR_SHARE_APPLICATIONS}"

mkdir -p "${TARGET_INSTALLATION_FOLDER_USR_SHARE_APPLICATIONS}"

LOGO_YEROTH_ERP_3_0=${YEROTH_ERP_3_0_HOME_FOLDER}/yeroth-erp-3-0-images/yeroth-erp-3-0.svg

DESKTOP_FILE_YEROTH_ERP_3_0=${YEROTH_ERP_3_0_HOME_FOLDER}/yeroth-erp-3-0-gnome-files-desktop/${YEROTH_ERP_3_0_BINARY_NAME_FOR_FILE}.desktop

sudo ${CP} ${DESKTOP_FILE_YEROTH_ERP_3_0} ${TARGET_INSTALLATION_FOLDER_USR_SHARE_APPLICATIONS}

sudo ${CP} ${LOGO_YEROTH_ERP_3_0} ${TARGET_INSTALLATION_FOLDER_USR_SHARE_PIXMAPS}

rm -f ${YEROTH_ERP_3_0_DEB_FILE_NAME}

TARGET_INSTALLATION_FOLDER_DEBIAN="${TARGET_INSTALLATION_FOLDER}/DEBIAN"

TARGET_INSTALLATION_FOLDER_OPT="${TARGET_INSTALLATION_FOLDER}/opt"

TARGET_INSTALLATION_FOLDER_USR="${TARGET_INSTALLATION_FOLDER}/usr"

chown_and_grp_for_lintian "${TARGET_INSTALLATION_FOLDER_DEBIAN}"

chown_and_grp_for_lintian "${TARGET_INSTALLATION_FOLDER_OPT}"

chown_and_grp_for_lintian "${TARGET_INSTALLATION_FOLDER_USR}"

dpkg-deb --verbose --build ${YEROTH_ERP_3_0_HOME_FOLDER}/${YEROTH_ERP_3_0_BINARY_NAME}
