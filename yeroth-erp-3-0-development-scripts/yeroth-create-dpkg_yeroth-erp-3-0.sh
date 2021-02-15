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

USR_SHARE="/usr/share"

YEROTH_ERP_3_0_DEB_FILE_FOLDER="${YEROTH_ERP_3_0_HOME_FOLDER}/$yeroth-erp-3-0-deb-file-repository"

YEROTH_ERP_3_0_LICENSE_TYPE="${yerothpos7VersionVal}"

YEROTH_ERP_3_0_BINARY_NAME="yeroth-erp-3.0-${YEROTH_ERP_3_0_LICENSE_TYPE}"

YEROTH_ERP_3_0_BINARY_NAME_FOR_FILE="yeroth-erp-3-0-${YEROTH_ERP_3_0_LICENSE_TYPE}"

YEROTH_ERP_3_0_ENGLISH_TRANSLATION_FILE="${YEROTH_ERP_3_0_HOME_FOLDER}/bin/yeroth-erp-3-0_english.qm"

YEROTH_ERP_3_0_DEB_FILE_NAME="${YEROTH_ERP_3_0_HOME_FOLDER}/${YEROTH_ERP_3_0_BINARY_NAME}.deb"

YEROTH_ERP_3_0_BINARY_FILE_PATH="${YEROTH_ERP_3_0_HOME_FOLDER}/bin/${YEROTH_ERP_3_0_BINARY_NAME}"


TARGET_INSTALLATION_FOLDER="${YEROTH_ERP_3_0_HOME_FOLDER}/${YEROTH_ERP_3_0_BINARY_NAME}"

if [ ! -d "${TARGET_INSTALLATION_FOLDER}" ]; then
    echo "creating installation directory: ${TARGET_INSTALLATION_FOLDER}"
    mkdir -p "${TARGET_INSTALLATION_FOLDER}"
fi


OPT_INSTALLATION_TARGET_DIR="${TARGET_INSTALLATION_FOLDER}/opt/${YEROTH_ERP_3_0_BINARY_NAME}"

mkdir -p "${OPT_INSTALLATION_TARGET_DIR}"

YEROTH_SQL_BACKUP_FOLDER_NAME="YEROTH_sql_backup"

TARGET_INSTALLATION_SQL_BACKUP_FOLDER="${OPT_INSTALLATION_TARGET_DIR}/${YEROTH_SQL_BACKUP_FOLDER_NAME}"

mkdir -p "${TARGET_INSTALLATION_SQL_BACKUP_FOLDER}"

TARGET_INSTALLATION_FOLDER_BIN="${OPT_INSTALLATION_TARGET_DIR}/bin"

mkdir -p "${TARGET_INSTALLATION_FOLDER_BIN}"

TARGET_INSTALLATION_FOLDER_DOC="${TARGET_INSTALLATION_FOLDER}${USR_SHARE}/doc/yeroth-erp-3.0"

mkdir -p "${TARGET_INSTALLATION_FOLDER_DOC}"

YEROTH_ERP_3_0_PROPERTIES_FILE_DIR=${OPT_INSTALLATION_TARGET_DIR}

touch "$OPT_INSTALLATION_TARGET_DIR/yeroth-erp-3-0.log"

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


echo -e "${YEROTH_ERP_3_0_DEB_PACKAGE_INFORMATION}$(cat ${TARGET_INSTALLATION_FOLDER}/DEBIAN/control)" > ${TARGET_INSTALLATION_FOLDER}/DEBIAN/control


if [ ! "$qsqlite3Flag" ]; then
	${CP} ${YEROTH_ERP_3_0_HOME_FOLDER}/yeroth-erp-3-0-deployment-configuration-scripts/yeroth-erp-3-0.properties \
			"${YEROTH_ERP_3_0_PROPERTIES_FILE_DIR}"

	${CP} ${YEROTH_ERP_3_0_HOME_FOLDER}/yeroth-erp-3-0-deployment-configuration-scripts/yeroth-erp-3-0-system-local-configuration.properties \
			"${YEROTH_ERP_3_0_PROPERTIES_FILE_DIR}"
else
	${CP} ${YEROTH_ERP_3_0_HOME_FOLDER}/yeroth-erp-3-0-deployment-configuration-scripts/yeroth-erp-3-0-qsqlite3.properties \
			"${YEROTH_ERP_3_0_PROPERTIES_FILE_DIR}/yeroth-erp-3-0.properties"

	${CP} ${YEROTH_ERP_3_0_HOME_FOLDER}/yeroth-erp-3-0-deployment-configuration-scripts/yeroth-erp-3-0-system-local-configuration.properties \
			"${YEROTH_ERP_3_0_PROPERTIES_FILE_DIR}"
fi

YEROTH_ERP_3_0_POSTRM_STR="#!/bin/bash
sed -i '/YEROTH_ERP_3_0_HOME_FOLDER/d' /etc/environment
sed -i '/YEROTH_ERP_3_0_PROPERTIES_CONFIGURATION_FOLDER/d' /etc/environment
sed -i '/YEROTH_ERP_3_0_SYSTEM_DAEMON_HOME_FOLDER/d' /etc/environment
"

echo -e "${YEROTH_ERP_3_0_POSTRM_STR}$(cat ${TARGET_INSTALLATION_FOLDER}/DEBIAN/postrm)" > ${TARGET_INSTALLATION_FOLDER}/DEBIAN/postrm

chmod 755 ${TARGET_INSTALLATION_FOLDER}/DEBIAN/postrm


YEROTH_ERP_3_0_POSTINST_STR="#!/bin/bash
echo -e \"export YEROTH_ERP_3_0_HOME_FOLDER=/opt/${YEROTH_ERP_3_0_BINARY_NAME}\" >> /etc/environment
echo -e \"export YEROTH_ERP_3_0_PROPERTIES_CONFIGURATION_FOLDER=/opt/${YEROTH_ERP_3_0_BINARY_NAME}\" >> /etc/environment
echo -e \"export YEROTH_ERP_3_0_SYSTEM_DAEMON_HOME_FOLDER=/opt/yeroth-erp-3-0-system-daemon\" >> /etc/environment
chmod go+rwx /opt/${YEROTH_ERP_3_0_BINARY_NAME}/${YEROTH_SQL_BACKUP_FOLDER_NAME}
chmod go+w /opt/${YEROTH_ERP_3_0_BINARY_NAME}/yeroth-erp-3-0.log
chmod go+w /opt/${YEROTH_ERP_3_0_BINARY_NAME}/yeroth-erp-3-0-system-local-configuration.properties"

echo -e "${YEROTH_ERP_3_0_POSTINST_STR}$(cat ${TARGET_INSTALLATION_FOLDER}/DEBIAN/postinst)" > ${TARGET_INSTALLATION_FOLDER}/DEBIAN/postinst

chmod 755 ${TARGET_INSTALLATION_FOLDER}/DEBIAN/postinst

${CP} ${YEROTH_ERP_3_0_ENGLISH_TRANSLATION_FILE} "${TARGET_INSTALLATION_FOLDER_BIN}"

${CP} ${YEROTH_ERP_3_0_BINARY_FILE_PATH} "${TARGET_INSTALLATION_FOLDER_BIN}"

${CP} ${YEROTH_ERP_3_0_BINARY_FILE_PATH}.sh ${TARGET_INSTALLATION_FOLDER_BIN}

# ********************************************************************************** #

YEROTH_ERP_3_0_DOCUMENTATION_ENGLISH_ROOT_DIR="${YEROTH_ERP_3_0_HOME_FOLDER}/doc/english"
YEROTH_ERP_3_0_DOCUMENTATION_FRANCAIS_ROOT_DIR="${YEROTH_ERP_3_0_HOME_FOLDER}/doc/francais"

SOURCE_DOCUMENTATION_ENGLISH_PRODUCT_SHEET_DIR="${YEROTH_ERP_3_0_DOCUMENTATION_ENGLISH_ROOT_DIR}/yeroth-product-sheet"
SOURCE_DOCUMENTATION_FRANCAIS_FICHE_DE_DONNEES_DIR="${YEROTH_ERP_3_0_DOCUMENTATION_FRANCAIS_ROOT_DIR}/yeroth-fiche-de-donnees"

ENGLISH_ERP_PRODUCT_COMPARISON_DIR="${YEROTH_ERP_3_0_DOCUMENTATION_ENGLISH_ROOT_DIR}/yeroth-erp-document-comparison"
FRANCAIS_COMPARAISON_ERP_DIR="${YEROTH_ERP_3_0_DOCUMENTATION_FRANCAIS_ROOT_DIR}/yeroth-erp-document-comparaisons"

SOURCE_DOCUMENTATION_ENGLISH_WHITE_PAPERS_DIR="${YEROTH_ERP_3_0_DOCUMENTATION_ENGLISH_ROOT_DIR}/yeroth-whitepapers"
SOURCE_DOCUMENTATION_FRANCAIS_WHITE_PAPERS_DIR="${YEROTH_ERP_3_0_DOCUMENTATION_FRANCAIS_ROOT_DIR}/yeroth-whitepapers"

SOURCE_DOCUMENTATION_ENGLISH_BROCHURE_DIR="${YEROTH_ERP_3_0_DOCUMENTATION_ENGLISH_ROOT_DIR}/yeroth-brochure"
SOURCE_DOCUMENTATION_FRANCAIS_BROCHURE_DIR="${YEROTH_ERP_3_0_DOCUMENTATION_FRANCAIS_ROOT_DIR}/yeroth-brochure"

ENGLISH_ERP_PRODUCT_COMPARISON_SHEET_PDF_FILE_PATH="${ENGLISH_ERP_PRODUCT_COMPARISON_DIR}/yeroth-erp-document-comparison.pdf"
FRANCAIS_COMPARAISON_ERP_PDF_CHEMIN="${FRANCAIS_COMPARAISON_ERP_DIR}/yeroth-erp-3.0-document-comparaisons.pdf"

ENGLISH_PRODUCT_SHEET_PDF_FILE_PATH="${SOURCE_DOCUMENTATION_ENGLISH_PRODUCT_SHEET_DIR}/yeroth-erp-3.0-product-sheet.pdf"
FRANCAIS_FICHE_DE_DONNEES_PDF_CHEMIN="${SOURCE_DOCUMENTATION_FRANCAIS_FICHE_DE_DONNEES_DIR}/yeroth-erp-3.0-fiche-de-donnees.pdf"

ENGLISH_WHITE_PAPERS_PDF_FILE_PATH="${SOURCE_DOCUMENTATION_ENGLISH_WHITE_PAPERS_DIR}/yeroth-erp-*.pdf"
FRANCAIS_WHITE_PAPERS_PDF_CHEMIN="${SOURCE_DOCUMENTATION_FRANCAIS_WHITE_PAPERS_DIR}/yeroth-erp-*.pdf"

ENGLISH_BROCHURE_PDF_FILE_PATH="${SOURCE_DOCUMENTATION_ENGLISH_BROCHURE_DIR}/yeroth-erp-3.0-brochure-english.pdf"
FRANCAIS_BROCHURE_PDF_CHEMIN="${SOURCE_DOCUMENTATION_FRANCAIS_BROCHURE_DIR}/yeroth-erp-3.0-brochure.pdf"

ENGLISH_USER_MANUAL_FILE_PATH="${YEROTH_ERP_3_0_DOCUMENTATION_ENGLISH_ROOT_DIR}/yeroth-erp-3.0-users-guide.pdf"
FRANCAIS_MANUEL_DE_LUTILISATEUR_CHEMIN="${YEROTH_ERP_3_0_DOCUMENTATION_FRANCAIS_ROOT_DIR}/guide-*/*.pdf"

mkdir -p "${TARGET_INSTALLATION_FOLDER_DOC}/english"
mkdir -p "${TARGET_INSTALLATION_FOLDER_DOC}/francais"

${CP} ${ENGLISH_PRODUCT_SHEET_PDF_FILE_PATH} "${TARGET_INSTALLATION_FOLDER_DOC}/english"
${CP} ${FRANCAIS_FICHE_DE_DONNEES_PDF_CHEMIN} "${TARGET_INSTALLATION_FOLDER_DOC}/francais"

${CP} ${ENGLISH_ERP_PRODUCT_COMPARISON_SHEET_PDF_FILE_PATH} "${TARGET_INSTALLATION_FOLDER_DOC}/english"
${CP} ${FRANCAIS_COMPARAISON_ERP_PDF_CHEMIN} "${TARGET_INSTALLATION_FOLDER_DOC}/francais"

${CP} ${ENGLISH_WHITE_PAPERS_PDF_FILE_PATH} "${TARGET_INSTALLATION_FOLDER_DOC}/english"
${CP} ${FRANCAIS_WHITE_PAPERS_PDF_CHEMIN} "${TARGET_INSTALLATION_FOLDER_DOC}/francais"

${CP} ${ENGLISH_BROCHURE_PDF_FILE_PATH} "${TARGET_INSTALLATION_FOLDER_DOC}/english"
${CP} ${FRANCAIS_BROCHURE_PDF_CHEMIN} "${TARGET_INSTALLATION_FOLDER_DOC}/francais"

${CP} ${ENGLISH_USER_MANUAL_FILE_PATH} "${TARGET_INSTALLATION_FOLDER_DOC}/english"
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

${CP} ${DESKTOP_FILE_YEROTH_ERP_3_0} ${TARGET_INSTALLATION_FOLDER_USR_SHARE_APPLICATIONS}

${CP} ${LOGO_YEROTH_ERP_3_0} ${TARGET_INSTALLATION_FOLDER_USR_SHARE_PIXMAPS}

rm -f ${YEROTH_ERP_3_0_DEB_FILE_NAME}

TARGET_INSTALLATION_FOLDER_DEBIAN="${TARGET_INSTALLATION_FOLDER}/DEBIAN"

TARGET_INSTALLATION_FOLDER_OPT="${TARGET_INSTALLATION_FOLDER}/opt"

TARGET_INSTALLATION_FOLDER_USR="${TARGET_INSTALLATION_FOLDER}/usr"

chown_and_grp_for_lintian "${TARGET_INSTALLATION_FOLDER_DEBIAN}"

chown_and_grp_for_lintian "${TARGET_INSTALLATION_FOLDER_OPT}"

chown_and_grp_for_lintian "${TARGET_INSTALLATION_FOLDER_USR}"

dpkg-deb --verbose --build ${YEROTH_ERP_3_0_HOME_FOLDER}/${YEROTH_ERP_3_0_BINARY_NAME}
