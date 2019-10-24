/*
 * yeroth-erp-dialog-changer-utilisateur.cpp
 *      Author: Dipl.-Inf. Xavier NOUMBISSI NOUNDOU, Ph.D. (ABD)
 *      Email:  xnoundou7@gmail.com
 */

#include "src/yeroth-erp-windows.hpp"


#include "src/windows/yeroth-erp-main-window.hpp"

#include "yeroth-erp-changer-utilisateur-dialog.hpp"

#include "src/users/yeroth-erp-users.hpp"

#include "src/users/yeroth-erp-user-manager.hpp"

#include "src/users/yeroth-erp-user-vendeur.hpp"

#include "src/users/yeroth-erp-user-gestionaire-des-stocks.hpp"

#include "src/users/yeroth-erp-user-magasinier.hpp"

#include "src/users/yeroth-erp-user-caissier.hpp"

#include "src/users/yeroth-erp-user-administrateur.hpp"


#include <QtWidgets/QDesktopWidget>

#include <QtWidgets/QMainWindow>

#include <QtSql/QSqlQuery>

#include <QtSql/QSqlRecord>

#include <QtSql/QSqlError>



const QString YerothPOSChangerUtilisateurDialog::DIALOG_WINDOW_TITLE(QString("%1 - %2").
        arg(YEROTH_ERP_WINDOW_TITLE,
            QObject::trUtf8("changer d'utilisateur")));

YerothPOSChangerUtilisateurDialog::YerothPOSChangerUtilisateurDialog(YerothERPWindows * allWindows,
																   QMainWindow * parent)
:YerothPOSDialogCommons(YerothPOSChangerUtilisateurDialog::DIALOG_WINDOW_TITLE),
 _currentPosition(new QPoint),
 _user(0)
{
    setupUi(this);

    QDesktopWidget &desktopWidget = _allWindows->desktopWidget();

	YerothUtils::getCenterPosition(desktopWidget.width(),
								  desktopWidget.height(),
								  *this,
								  *_currentPosition);

    lineEdit_mot_passe->setEchoMode(QLineEdit::Password);

    setFixedSize(width(), height());

    connect(pushButton_annuler, SIGNAL(clicked()), this, SLOT(annuler()));
    connect(pushButton_valider, SIGNAL(clicked()), this, SLOT(valider()));
}

YerothPOSChangerUtilisateurDialog::~YerothPOSChangerUtilisateurDialog()
{
	delete _currentPosition;

    if (_user)
    {
        delete _user;

        _user = 0;
    }
}

void YerothPOSChangerUtilisateurDialog::valider()
{
    //qDebug() << "DepotFacileDialogChangerUtilisateur::valider()";
    QString mot_passe(lineEdit_mot_passe->text());

    QString nom_utilisateur(lineEdit_nom_utilisateur->text().toLower());

    if (!mot_passe.isEmpty())
    {
        mot_passe.append(nom_utilisateur);

        YerothSqlTableModel & usersSqlTableModel = _allWindows->getSqlTableModel_users();

        QString searchUserFilter;

        QByteArray md5Hash_mot_passe(MD5_HASH(mot_passe));

        searchUserFilter.append(GENERATE_SQL_IS_STMT("nom_utilisateur", nom_utilisateur));

        usersSqlTableModel.yerothSetFilter(searchUserFilter);

        int usersSqlTableModelRowCount = usersSqlTableModel.easySelect();

        if (usersSqlTableModelRowCount > 0)
        {
            QSqlRecord userRecord = usersSqlTableModel.record(0);
            QByteArray md5Hash(userRecord.value("mot_passe").toByteArray());

            if (md5Hash != md5Hash_mot_passe)
            {
                YerothQMessageBox::information(_allWindows->_mainWindow,
                							  QObject::tr("enregistrement de l'utilisateur"),
											  QObject::tr("Votre nom d'utilisateur "
													  	  "ou bien votre mot de passe "
													  	  "est incorrect !"));
                return;
            }

            QString role = GET_SQL_RECORD_DATA(userRecord, "role");

            YerothPOSUser *user = createUser(userRecord, role);

            if (!user)
            {
                YerothQMessageBox::information(_allWindows->_mainWindow,
                							  QObject::tr("enregistrement de l'utilisateur"),
											  QObject::tr("Votre nom d'utilisateur "
													  	  "ou bien votre mot de passe "
													  	  "est incorrect !"));
                return;
            }

            if (_user)
            {
                delete _user;
            }

            _user = user;

            //Check if the user has new alerts.
            checkCourriersAlertes();

            _allWindows->_mainWindow->cleanupAllOtherWindows();

            _allWindows->setUser(user);

            if (YerothUtils::isEqualCaseInsensitive(ROLE_YEROTH_ERP_3_0_ADMINISTRATEUR, role))
            {
            	_allWindows->_mainWindow->rendreInvisible();
                _allWindows->_mainWindow->definirAdministrateur();
            }
            else if (YerothUtils::isEqualCaseInsensitive(ROLE_YEROTH_ERP_3_0_CAISSIER, role))
            {
                _allWindows->_mainWindow->rendreInvisible();
                _allWindows->_ventesWindow->resetFilter(&_allWindows->getSqlTableModel_stocks_vendu());
                _allWindows->_pdVenteWindow->rendreVisible(&_allWindows->getSqlTableModel_stocks());
            }
            else if (YerothUtils::isEqualCaseInsensitive(ROLE_YEROTH_ERP_3_0_MAGASINIER, role))
            {
                _allWindows->_mainWindow->rendreVisible(&_allWindows->getSqlTableModel_stocks());
            }
            else if (YerothUtils::isEqualCaseInsensitive(ROLE_YEROTH_ERP_3_0_VENDEUR, role))
            {
                _allWindows->_mainWindow->rendreVisible(&_allWindows->getSqlTableModel_stocks());
            }
            else if (YerothUtils::isEqualCaseInsensitive(ROLE_YEROTH_ERP_3_0_GESTIONAIRE_DES_STOCKS, role))
            {
                _allWindows->_mainWindow->rendreVisible(&_allWindows->getSqlTableModel_stocks());
            }
            else		//RoleManager
            {
                //qDebug() << "++ RoleManager";
                //qDebug() << "++ _allWindows->_caisseWindow: " << _allWindows->_caisseWindow;
                _allWindows->_ventesWindow->resetFilter(&_allWindows->getSqlTableModel_stocks_vendu());
                //qDebug() << "++ RoleManager, _allWindows->_caisseWindow->resetFilter()";
                _allWindows->_mainWindow->rendreVisible(&_allWindows->getSqlTableModel_stocks());
            }

            close();
        }
        else
        {
            YerothQMessageBox::information(_allWindows->_mainWindow,
            							   QObject::tr("enregistrement de l'utilisateur"),
										   QObject::tr("Votre nom d'utilisateur "
												   	   "ou bien votre mot de passe "
												   	   "est incorrect !"));
            return;
        }
    }
    else
    {
        YerothQMessageBox::information(_allWindows->_mainWindow,
        							   QObject::tr("enregistrement de l'utilisateur"),
									   QObject::tr("Votre nom d'utilisateur "
											   	   "ou bien votre mot de passe "
											   	   "est incorrect !"));
    }
}

YerothPOSUser *YerothPOSChangerUtilisateurDialog::createUser(QSqlRecord & userRecord, QString & role)
{
    YerothPOSUser *user = 0;

    if (YerothUtils::isEqualCaseSensitive(ROLE_YEROTH_ERP_3_0_MANAGER, role))
    {
        user = new YerothPOSUserManager(_allWindows);
    }
    if (YerothUtils::isEqualCaseSensitive(ROLE_YEROTH_ERP_3_0_VENDEUR, role))
    {
        user = new YerothERPUserVendeur(_allWindows);
    }
    if (YerothUtils::isEqualCaseSensitive(ROLE_YEROTH_ERP_3_0_GESTIONAIRE_DES_STOCKS, role))
    {
        user = new YerothPOSUserGestionaireDesStocks(_allWindows);
    }
    else if (YerothUtils::isEqualCaseSensitive(ROLE_YEROTH_ERP_3_0_MAGASINIER, role))
    {
        user = new YerothPOSUserMagasinier(_allWindows);
    }
    else if (YerothUtils::isEqualCaseSensitive(ROLE_YEROTH_ERP_3_0_CAISSIER, role))
    {
        user = new YerothPOSUserCaissier(_allWindows);
    }
    else if (YerothUtils::isEqualCaseSensitive(ROLE_YEROTH_ERP_3_0_ADMINISTRATEUR, role))
    {

#ifdef YEROTH_CLIENT
        QString retMsg(QObject::trUtf8("La version cliente de YEROTH ne vous donne pas accès à l'administration!"));

        QMessageBox::information(this,
        						 QObject::trUtf8("pas d'accès à l'administration"),
                                 FROM_UTF8_STRING(retMsg));
        return 0;
#endif

        user = new YerothPOSUserAdministrateur(_allWindows);
    }

    user->set_nom_utilisateur(GET_SQL_RECORD_DATA(userRecord, "nom_utilisateur"));
    user->set_prenom(GET_SQL_RECORD_DATA(userRecord, "prenom"));
    user->set_nom(GET_SQL_RECORD_DATA(userRecord, "nom"));
    user->set_lieu_naissance(GET_SQL_RECORD_DATA(userRecord, "lieu_naissance"));
    user->set_date_naissance(GET_SQL_RECORD_DATA(userRecord, "date_naissance"));
    user->set_localisation(GET_SQL_RECORD_DATA(userRecord, "localisation"));
    user->set_email(GET_SQL_RECORD_DATA(userRecord, YerothDatabaseTableColumn::EMAIL));
    user->set_numero_telephone_1(GET_SQL_RECORD_DATA(userRecord, YerothDatabaseTableColumn::NUMERO_TELEPHONE_1));
    user->set_numero_telephone_2(GET_SQL_RECORD_DATA(userRecord, YerothDatabaseTableColumn::NUMERO_TELEPHONE_2));
    user->set_titre(GET_SQL_RECORD_DATA(userRecord, "titre"));
    user->set_nom_complet(user->prenom().append(" ").append(user->nom()));

    return user;
}

void YerothPOSChangerUtilisateurDialog::checkCourriersAlertes()
{
    YerothSqlTableModel & courriersAlertesTable = _allWindows->getSqlTableModel_courriers_alertes();

    QString courriersAlertesFilter(GENERATE_SQL_IS_STMT(YerothDatabaseTableColumn::DESTINATAIRE, lineEdit_nom_utilisateur->text()));

    courriersAlertesFilter.append(" AND ").append(GENERATE_SQL_IS_STMT(YerothDatabaseTableColumn::ALERTE_RESOLUE, "0"));
    courriersAlertesTable.yerothSetFilter(courriersAlertesFilter);

    int alertsNr = courriersAlertesTable.rowCount();

    if (courriersAlertesTable.select() && alertsNr > 0)
    {
        QApplication::beep();
        QString aMsg("Vous avez ");
        QString alertsNr_s;
        alertsNr_s.setNum(courriersAlertesTable.rowCount());
        aMsg.append(alertsNr_s);
        aMsg.append(" alertes non résolues!");

        YerothQMessageBox::information(_allWindows->_listerAlertesWindow,
                                      QObject::trUtf8
                                      ("changer d'utilisateur - alertes"),
									  FROM_UTF8_STRING(aMsg));
    }
}

void YerothPOSChangerUtilisateurDialog::show()
{
	move(*_currentPosition);

    lineEdit_nom_utilisateur->setFocus();

    YerothPOSDialogCommons::show();
}

void YerothPOSChangerUtilisateurDialog::closeEvent(QCloseEvent * closeEvent)
{
    lineEdit_nom_utilisateur->clear();
    lineEdit_mot_passe->clear();
    lineEdit_nom_utilisateur->setFocus();

    YerothPOSDialogCommons::closeEvent(closeEvent);
}
