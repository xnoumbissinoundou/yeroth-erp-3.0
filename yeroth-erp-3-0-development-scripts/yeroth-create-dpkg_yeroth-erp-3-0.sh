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

YEROTH_ERP_3_0_DEB_FILE_FOLDER="yeroth-erp-3-0-deb-file-repository"

YEROTH_ERP_3_0_LICENSE_TYPE="${yerothpos7VersionVal}"

YEROTH_ERP_3_0_BINARY_NAME="yeroth-erp-3.0-${YEROTH_ERP_3_0_LICENSE_TYPE}"

YEROTH_ERP_3_0_BINARY_NAME_FOR_FILE="yeroth-erp-3-0-${YEROTH_ERP_3_0_LICENSE_TYPE}"

YEROTH_ERP_3_0_ENGLISH_TRANSLATION_FILE="./bin/yeroth-erp-3-0_english.qm"

YEROTH_ERP_3_0_DEB_FILE_NAME="${YEROTH_ERP_3_0_BINARY_NAME}.deb"

YEROTH_ERP_3_0_BINARY_FILE_PATH="./bin/${YEROTH_ERP_3_0_BINARY_NAME}"


TARGET_INSTALLATION_DIR="${YEROTH_ERP_3_0_BINARY_NAME}"

if [ ! -d "${TARGET_INSTALLATION_DIR}" ]; then
    echo "creating installation directory: ${TARGET_INSTALLATION_DIR}"
    mkdir -p "${TARGET_INSTALLATION_DIR}"
fi


OPT_INSTALLATION_TARGET_DIR="${TARGET_INSTALLATION_DIR}/opt/${YEROTH_ERP_3_0_BINARY_NAME}"

mkdir -p "${OPT_INSTALLATION_TARGET_DIR}"

TARGET_INSTALLATION_DIR_BIN="${OPT_INSTALLATION_TARGET_DIR}/bin"

mkdir -p "${TARGET_INSTALLATION_DIR_BIN}"

TARGET_INSTALLATION_DIR_DOC="${TARGET_INSTALLATION_DIR}${USR_SHARE}/doc/yeroth-erp-3.0"

mkdir -p "${TARGET_INSTALLATION_DIR_DOC}"

YEROTH_ERP_3_0_PROPERTIES_FILE_DIR=${OPT_INSTALLATION_TARGET_DIR}

touch "$OPT_INSTALLATION_TARGET_DIR/yeroth-erp-3-0.log"

CP="cp"

YEROTH_ERP_3_0_DEB_PACKAGE_INFORMATION="Package: ${YEROTH_ERP_3_0_BINARY_NAME}"

mkdir "${TARGET_INSTALLATION_DIR}/DEBIAN"


if [ ! "$qsqlite3Flag" ]; then
	${CP} DEBIAN/control "${TARGET_INSTALLATION_DIR}/DEBIAN/control"
	${CP} DEBIAN/postinst "${TARGET_INSTALLATION_DIR}/DEBIAN/postinst"
	${CP} DEBIAN/postrm "${TARGET_INSTALLATION_DIR}/DEBIAN/postrm"
else
	${CP} DEBIAN/control_academic "${TARGET_INSTALLATION_DIR}/DEBIAN/control"
fi


echo -e "${YEROTH_ERP_3_0_DEB_PACKAGE_INFORMATION}$(cat ${TARGET_INSTALLATION_DIR}/DEBIAN/control)" > ${TARGET_INSTALLATION_DIR}/DEBIAN/control


if [ ! "$qsqlite3Flag" ]; then
	${CP} yeroth-erp-3-0-deployment-configuration-scripts/yeroth-erp-3-0.properties "${YEROTH_ERP_3_0_PROPERTIES_FILE_DIR}"
else
	${CP} yeroth-erp-3-0-deployment-configuration-scripts/yeroth-erp-3-0-qsqlite3.properties "${YEROTH_ERP_3_0_PROPERTIES_FILE_DIR}/yeroth-erp-3-0.properties"
fi

YEROTH_ERP_3_0_POSTRM_STR="#!/bin/bash
sed -i '/YEROTH_ERP_3_0_HOME_FOLDER/d' /etc/environment
sed -i '/YEROTH_ERP_3_0_PROPERTIES_CONFIGURATION_FOLDER/d' /etc/environment
sed -i '/YEROTH_ERP_ALERT_3_0_HOME_FOLDER/d' /etc/environment
"

echo -e "${YEROTH_ERP_3_0_POSTRM_STR}$(cat ${TARGET_INSTALLATION_DIR}/DEBIAN/postrm)" > ${TARGET_INSTALLATION_DIR}/DEBIAN/postrm

chmod 755 ${TARGET_INSTALLATION_DIR}/DEBIAN/postrm


if [ ! "$qsqlite3Flag" ]; then
		YEROTH_ERP_3_0_POSTINST_STR="#!/bin/bash
echo -e \"export YEROTH_ERP_3_0_HOME_FOLDER=/opt/${YEROTH_ERP_3_0_BINARY_NAME}\" >> /etc/environment
echo -e \"export YEROTH_ERP_3_0_PROPERTIES_CONFIGURATION_FOLDER=/opt/${YEROTH_ERP_3_0_BINARY_NAME}\" >> /etc/environment
echo -e \"export YEROTH_ERP_ALERT_3_0_HOME_FOLDER=/opt/yeroth-erp-alert-3-0\" >> /etc/environment
MAINDB=\"yeroth_erp_3\"
USER_NAME=\"yeroth_erp_3\"
PASSWDDB=\"1234567\"
echo \"Enter MariaDB administrative password YEROTH-ERP-3.0 database and user!\"
mysql -uroot -p -e \"CREATE DATABASE \${MAINDB} CHARACTER SET = 'utf8'; \
CREATE USER \${USER_NAME}@'%' IDENTIFIED BY '\${PASSWDDB}'; \
GRANT ALL PRIVILEGES ON *.* TO '\${USER_NAME}'@'%'; \
FLUSH PRIVILEGES;\"
mysql_ret_code=\"\$?\"
exit 0"
else
		YEROTH_ERP_3_0_POSTINST_STR="#!/bin/bash
echo -e \"export YEROTH_ERP_3_0_HOME_FOLDER=/opt/${YEROTH_ERP_3_0_BINARY_NAME}\" >> /etc/environment
echo -e \"export YEROTH_ERP_3_0_PROPERTIES_CONFIGURATION_FOLDER=/opt/${YEROTH_ERP_3_0_BINARY_NAME}\" >> /etc/environment
echo -e \"export YEROTH_ERP_ALERT_3_0_HOME_FOLDER=/opt/yeroth-erp-alert-3-0\" >> /etc/environment"
fi


echo -e "${YEROTH_ERP_3_0_POSTINST_STR}$(cat ${TARGET_INSTALLATION_DIR}/DEBIAN/postinst)" > ${TARGET_INSTALLATION_DIR}/DEBIAN/postinst

chmod 755 ${TARGET_INSTALLATION_DIR}/DEBIAN/postinst

${CP} ${YEROTH_ERP_3_0_ENGLISH_TRANSLATION_FILE} "${TARGET_INSTALLATION_DIR_BIN}"

${CP} ${YEROTH_ERP_3_0_BINARY_FILE_PATH} "${TARGET_INSTALLATION_DIR_BIN}"

${CP} ${YEROTH_ERP_3_0_BINARY_FILE_PATH}.sh ${TARGET_INSTALLATION_DIR_BIN}

# ********************************************************************************** #

YEROTH_ERP_3_0_DOCUMENTATION_ENGLISH_ROOT_DIR="./doc/english"
YEROTH_ERP_3_0_DOCUMENTATION_FRANCAIS_ROOT_DIR="./doc/francais"

SOURCE_DOCUMENTATION_ENGLISH_PRODUCT_SHEET_DIR="${YEROTH_ERP_3_0_DOCUMENTATION_ENGLISH_ROOT_DIR}/yeroth-product-sheet"
SOURCE_DOCUMENTATION_FRANCAIS_FICHE_DE_DONNEES_DIR="${YEROTH_ERP_3_0_DOCUMENTATION_FRANCAIS_ROOT_DIR}/yeroth-fiche-de-donnees"

SOURCE_DOCUMENTATION_ENGLISH_WHITE_PAPERS_DIR="${YEROTH_ERP_3_0_DOCUMENTATION_ENGLISH_ROOT_DIR}/yeroth-whitepapers"
SOURCE_DOCUMENTATION_FRANCAIS_WHITE_PAPERS_DIR="${YEROTH_ERP_3_0_DOCUMENTATION_FRANCAIS_ROOT_DIR}/yeroth-whitepapers"

SOURCE_DOCUMENTATION_ENGLISH_BROCHURE_DIR="${YEROTH_ERP_3_0_DOCUMENTATION_ENGLISH_ROOT_DIR}/yeroth-brochure"
SOURCE_DOCUMENTATION_FRANCAIS_BROCHURE_DIR="${YEROTH_ERP_3_0_DOCUMENTATION_FRANCAIS_ROOT_DIR}/yeroth-brochure"

ENGLISH_PRODUCT_SHEET_PDF_FILE_PATH="${SOURCE_DOCUMENTATION_ENGLISH_PRODUCT_SHEET_DIR}/yeroth-erp-3.0-product-sheet.pdf"
FRANCAIS_FICHE_DE_DONNEES_PDF_CHEMIN="${SOURCE_DOCUMENTATION_FRANCAIS_FICHE_DE_DONNEES_DIR}/yeroth-erp-3.0-fiche-de-donnees.pdf"

ENGLISH_WHITE_PAPERS_PDF_FILE_PATH="${SOURCE_DOCUMENTATION_ENGLISH_WHITE_PAPERS_DIR}/yeroth-erp-*.pdf"
FRANCAIS_WHITE_PAPERS_PDF_CHEMIN="${SOURCE_DOCUMENTATION_FRANCAIS_WHITE_PAPERS_DIR}/yeroth-erp-*.pdf"

ENGLISH_BROCHURE_PDF_FILE_PATH="${SOURCE_DOCUMENTATION_ENGLISH_BROCHURE_DIR}/yeroth-erp-3.0-brochure-english.pdf"
FRANCAIS_BROCHURE_PDF_CHEMIN="${SOURCE_DOCUMENTATION_FRANCAIS_BROCHURE_DIR}/yeroth-erp-3.0-brochure.pdf"

ENGLISH_USER_MANUAL_FILE_PATH="${YEROTH_ERP_3_0_DOCUMENTATION_ENGLISH_ROOT_DIR}/yeroth-erp-3.0-user-manual.pdf"
FRANCAIS_MANUEL_DE_LUTILISATEUR_CHEMIN="${YEROTH_ERP_3_0_DOCUMENTATION_FRANCAIS_ROOT_DIR}/manuel-*/*.pdf"

mkdir -p "${TARGET_INSTALLATION_DIR_DOC}/english"
mkdir -p "${TARGET_INSTALLATION_DIR_DOC}/francais"

${CP} ${ENGLISH_PRODUCT_SHEET_PDF_FILE_PATH} "${TARGET_INSTALLATION_DIR_DOC}/english"
${CP} ${FRANCAIS_FICHE_DE_DONNEES_PDF_CHEMIN} "${TARGET_INSTALLATION_DIR_DOC}/francais"

${CP} ${ENGLISH_WHITE_PAPERS_PDF_FILE_PATH} "${TARGET_INSTALLATION_DIR_DOC}/english"
${CP} ${FRANCAIS_WHITE_PAPERS_PDF_CHEMIN} "${TARGET_INSTALLATION_DIR_DOC}/francais"

${CP} ${ENGLISH_BROCHURE_PDF_FILE_PATH} "${TARGET_INSTALLATION_DIR_DOC}/english"
${CP} ${FRANCAIS_BROCHURE_PDF_CHEMIN} "${TARGET_INSTALLATION_DIR_DOC}/francais"

${CP} ${ENGLISH_USER_MANUAL_FILE_PATH} "${TARGET_INSTALLATION_DIR_DOC}/english"
${CP} ${FRANCAIS_MANUEL_DE_LUTILISATEUR_CHEMIN} "${TARGET_INSTALLATION_DIR_DOC}/francais"

# ********************************************************************************** #

USR_SHARE_PIXMAPS="${USR_SHARE}/pixmaps"

USR_SHARE_APPLICATIONS="${USR_SHARE}/applications"

TARGET_INSTALLATION_DIR_USR_SHARE_PIXMAPS="${TARGET_INSTALLATION_DIR}${USR_SHARE_PIXMAPS}"

mkdir -p "${TARGET_INSTALLATION_DIR_USR_SHARE_PIXMAPS}"

TARGET_INSTALLATION_DIR_USR_SHARE_APPLICATIONS="${TARGET_INSTALLATION_DIR}${USR_SHARE_APPLICATIONS}"

mkdir -p "${TARGET_INSTALLATION_DIR_USR_SHARE_APPLICATIONS}"

LOGO_YEROTH_ERP_3_0=${YEROTH_ERP_3_0_HOME_FOLDER}/yeroth-erp-3-0-images/yeroth-erp-3-0.svg

DESKTOP_FILE_YEROTH_ERP_3_0=${YEROTH_ERP_3_0_HOME_FOLDER}/yeroth-erp-3-0-gnome-files-desktop/${YEROTH_ERP_3_0_BINARY_NAME_FOR_FILE}.desktop

${CP} ${DESKTOP_FILE_YEROTH_ERP_3_0} ${TARGET_INSTALLATION_DIR_USR_SHARE_APPLICATIONS}

${CP} ${LOGO_YEROTH_ERP_3_0} ${TARGET_INSTALLATION_DIR_USR_SHARE_PIXMAPS}

rm -f ${YEROTH_ERP_3_0_DEB_FILE_NAME}

TARGET_INSTALLATION_DIR_DEBIAN="${TARGET_INSTALLATION_DIR}/DEBIAN"

TARGET_INSTALLATION_DIR_OPT="${TARGET_INSTALLATION_DIR}/opt"

TARGET_INSTALLATION_DIR_USR="${TARGET_INSTALLATION_DIR}/usr"

chown_and_grp_for_lintian "${TARGET_INSTALLATION_DIR_DEBIAN}"

chown_and_grp_for_lintian "${TARGET_INSTALLATION_DIR_OPT}"

chown_and_grp_for_lintian "${TARGET_INSTALLATION_DIR_USR}"

dpkg-deb --verbose --build ${YEROTH_ERP_3_0_BINARY_NAME}
