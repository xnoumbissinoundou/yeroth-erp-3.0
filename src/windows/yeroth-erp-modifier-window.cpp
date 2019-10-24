/*

   * yeroth-erp-modifier-window.cpp

   *

   *  Created on: Oct 20, 2015

   *      Author: Dipl.-Inf. Xavier NOUMBISSI NOUNDOU, Ph.D. (ABD)

   *      Email:  xnoundou7@gmail.com

   */


#include "src/windows/yeroth-erp-modifier-window.hpp"

#include "src/yeroth-erp-windows.hpp"

#include <unistd.h>

#include <QtSql/QSqlRecord>

#include <QtGui/QContextMenuEvent>


const unsigned int YerothModifierWindow::INDEX_ZERO(0);

const QString YerothModifierWindow::_WINDOW_TITLE(QString(QObject::trUtf8("%1 - %2")).
        arg(YEROTH_ERP_WINDOW_TITLE,
            QObject::trUtf8("modifier un stock")));

YerothModifierWindow::YerothModifierWindow()
:YerothWindowsCommons(YerothModifierWindow::_WINDOW_TITLE),
 _logger(new YerothLogger("YerothModifierWindow")),
 _montantTva(0.0),
 _tvaCheckBoxPreviousState(false),
 _tvaPercent(YerothUtils::getTvaStringWithPercent())
{
    setupUi(this);

    this->mySetupUi(this);

    QMESSAGE_BOX_STYLE_SHEET = QString("QMessageBox {background-color: rgb(%1);}"
                                       "QMessageBox QLabel {color: rgb(%2);}").
                               arg(COLOUR_RGB_STRING_YEROTH_DARK_GRAY_60_60_60, COLOUR_RGB_STRING_YEROTH_WHITE_255_255_255);

    this->setupLineEdits();
    this->setupDateTimeEdits();

    spinBox_lots->setEnabled(false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionActualiser, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEntrer, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionStocks, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionModifier, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimer, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSortir, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVendre, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);

    pushButton_entrer->disable(this);
    pushButton_stocks->disable(this);
    pushButton_menu_principal->disable(this);
    pushButton_supprimer->disable(this);
    pushButton_enregistrer->disable(this);
    pushButton_annuler->disable(this);
    pushButton_selectionner_image->disable(this);

    connect(lineEdit_quantite_par_lot, SIGNAL(textChanged(const QString &)), this,
            SLOT(display_quantite_restante(const QString &)));
    connect(spinBox_lots, SIGNAL(valueChanged(int)), this, SLOT(display_quantite_restante_by_spinbox(int)));
    connect(checkBox_tva, SIGNAL(clicked(bool)), this, SLOT(handleTVACheckBox(bool)));
    connect(lineEdit_prix_vente, SIGNAL(textEdited(const QString &)), this,
            SLOT(edited_prix_vente(const QString &)));
    connect(lineEdit_prix_vente, SIGNAL(editingFinished()), this, SLOT(display_prix_vente()));

    /** Menu actions */
    connect(actionChanger_utilisateur, SIGNAL(triggered()), this, SLOT(changer_utilisateur()));
    connect(actionAppeler_aide, SIGNAL(triggered()), this, SLOT(help()));
    connect(actionDeconnecter_utilisateur, SIGNAL(triggered()), this, SLOT(deconnecter_utilisateur()));
    connect(actionMenu, SIGNAL(triggered()), this, SLOT(menu()));
    connect(actionActualiser, SIGNAL(triggered()), this, SLOT(actualiser_article()));
    connect(actionEntrer, SIGNAL(triggered()), this, SLOT(entrer()));
    connect(actionStocks, SIGNAL(triggered()), this, SLOT(afficherStocks()));
    connect(actionSupprimer, SIGNAL(triggered()), this, SLOT(supprimer_ce_stock()));
    connect(actionSupprimerImage, SIGNAL(triggered()), this, SLOT(supprimer_image_stock()));
    connect(actionSortir, SIGNAL(triggered()), this, SLOT(sortir()));
    connect(actionFermeture, SIGNAL(triggered()), this, SLOT(fermeture()));
    connect(actionA_propos, SIGNAL(triggered()), this, SLOT(apropos()));
    connect(actionAlertes, SIGNAL(triggered()), this, SLOT(alertes()));
    connect(actionVendre, SIGNAL(triggered()), this, SLOT(vendre()));
    connect(actionInformationEntreprise, SIGNAL(triggered()), this, SLOT(infosEntreprise()));
    connect(actionQui_suis_je, SIGNAL(triggered()), this, SLOT(qui_suis_je()));

#ifdef YEROTH_CLIENT
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#else				//YEROTH_SERVER, YEROTH_STANDALONE, YEROTH_ACADEMIC_EVALUSATION_VERSION
    connect(actionAdministration, SIGNAL(triggered()), this, SLOT(administration()));
#endif

    this->setupShortcuts();
}

void YerothModifierWindow::setupShortcuts()
{
    this->setupShortcutActionMessageDaide 	(*actionAppeler_aide);
    this->setupShortcutActionQuiSuisJe		(*actionQui_suis_je);
}

void YerothModifierWindow::setupLineEdits()
{
    lineEdit_quantite_par_lot->setValidator(&YerothUtils::DoubleValidator);
    lineEdit_quantite_restante->setValidator(&YerothUtils::IntValidator);
    lineEdit_stock_minimum->setValidator(&YerothUtils::DoubleValidator);
    lineEdit_prix_vente->setValidator(&YerothUtils::DoubleValidator);

    lineEdit_localisation_produit->setEnabled(true);
    lineEdit_prix_dachat->setEnabled(true);
    lineEdit_prix_vente->setEnabled(true);
    lineEdit_reference_produit->setEnabled(false);
    lineEdit_designation->setEnabled(false);
    lineEdit_nom_entreprise_fournisseur->setEnabled(false);
    lineEdit_categorie_produit->setEnabled(false);
    lineEdit_quantite_par_lot->setEnabled(false);
    lineEdit_stock_minimum->setEnabled(true);
    lineEdit_tva->setEnabled(false);
    lineEdit_quantite_restante->setEnabled(false);
    lineEdit_tva->setText(YerothUtils::getTvaStringWithPercent());

}


void YerothModifierWindow::contextMenuEvent(QContextMenuEvent * event)
{
    QMenu menu(this);
    menu.setPalette(toolBar_modifierWindow->palette());
    menu.addAction(actionActualiser);
    menu.addAction(actionSupprimerImage);
    menu.exec(event->globalPos());
}

void YerothModifierWindow::deconnecter_utilisateur()
{
    this->clear_all_fields();
    YerothWindowsCommons::deconnecter_utilisateur();
}

void YerothModifierWindow::definirPasDeRole()
{
    _logger->log("definirPasDeRole");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionActualiser, false);

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEntrer, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionStocks, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionModifier, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimer, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSortir, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVendre, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);

    pushButton_entrer->disable(this);
    pushButton_stocks->disable(this);
    pushButton_annuler->disable(this);
    pushButton_supprimer->disable(this);
    pushButton_enregistrer->disable(this);
    pushButton_selectionner_image->disable(this);
}

void YerothModifierWindow::definirCaissier()
{
    _logger->log("definirCaissier");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);

    actionMenu->setDisabled(false);

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionActualiser, false);

    actionEntrer->setDisabled(true);
    actionStocks->setDisabled(false);

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimer, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSortir, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVendre, true);

    actionModifier->setDisabled(true);

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    pushButton_entrer->disable(this);
    pushButton_stocks->enable(this, SLOT(afficherStocks()));
    pushButton_menu_principal->enable(this, SLOT(menu()));
    pushButton_supprimer->disable(this);
    pushButton_enregistrer->disable(this);
    pushButton_selectionner_image->disable(this);
}

void YerothModifierWindow::definirManager()
{
    _logger->log("definirManager");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionActualiser, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEntrer, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionStocks, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionModifier, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimer, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSortir, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVendre, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);

#ifdef YEROTH_CLIENT
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    pushButton_entrer->enable(this, SLOT(entrer()));
    pushButton_stocks->enable(this, SLOT(afficherStocks()));
    pushButton_menu_principal->enable(this, SLOT(menu()));
    pushButton_annuler->enable(this, SLOT(afficherStocks()));
    pushButton_supprimer->enable(this, SLOT(supprimer_ce_stock()));
    pushButton_enregistrer->enable(this, SLOT(actualiser_article()));
    pushButton_selectionner_image->enable(this, SLOT(selectionner_image_produit()));
}


void YerothModifierWindow::definirVendeur()
{
    _logger->log("definirVendeur");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionActualiser, false);

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEntrer, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionStocks, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionModifier, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimer, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSortir, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVendre, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);

    pushButton_entrer->disable(this);
    pushButton_stocks->disable(this);
    pushButton_annuler->disable(this);
    pushButton_supprimer->disable(this);
    pushButton_enregistrer->disable(this);
    pushButton_selectionner_image->disable(this);
}


void YerothModifierWindow::definirGestionaireDesStocks()
{
    _logger->log("definirGestionaireDesStocks");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionActualiser, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEntrer, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionStocks, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionModifier, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimer, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSortir, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVendre, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);

#ifdef YEROTH_CLIENT
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    pushButton_entrer->enable(this, SLOT(entrer()));
    pushButton_stocks->enable(this, SLOT(afficherStocks()));
    pushButton_menu_principal->enable(this, SLOT(menu()));
    pushButton_annuler->enable(this, SLOT(afficherStocks()));
    pushButton_supprimer->enable(this, SLOT(supprimer_ce_stock()));
    pushButton_enregistrer->enable(this, SLOT(actualiser_article()));
    pushButton_selectionner_image->enable(this, SLOT(selectionner_image_produit()));
}

void YerothModifierWindow::definirMagasinier()
{
    _logger->log("definirMagasinier");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionActualiser, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEntrer, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionStocks, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionModifier, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimer, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSortir, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVendre, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);

    pushButton_entrer->disable(this);
    pushButton_stocks->disable(this);
    pushButton_menu_principal->disable(this);
    pushButton_annuler->disable(this);
    pushButton_supprimer->disable(this);
    pushButton_enregistrer->disable(this);
    pushButton_selectionner_image->disable(this);
}

bool YerothModifierWindow::check_fields()
{
    bool prix_vente = lineEdit_prix_vente->checkField();
    return prix_vente;
}

void YerothModifierWindow::clear_all_fields()
{
    lineEdit_reference_produit->clearField();
    lineEdit_designation->clearField();
    lineEdit_categorie_produit->clearField();
    spinBox_lots->clear();
    lineEdit_quantite_par_lot->clearField();
    lineEdit_nom_entreprise_fournisseur->clear();
    lineEdit_stock_minimum->clearField();
    lineEdit_prix_vente->clearField();
    textEdit_description->clear();
    lineEdit_localisation_produit->clear();
    label_image_produit->clear();
    label_image_produit->setAutoFillBackground(false);
    lineEdit_tva->clearField();
    checkBox_tva->setChecked(false);
}

void YerothModifierWindow::display_quantite_restante(const QString & quantite_par_lot)
{
    double qte_lot = quantite_par_lot.toDouble();
    double qte_restante = spinBox_lots->valueMultiplyBy(qte_lot);
    lineEdit_quantite_restante->setText(QString::number(qte_restante, 'f', 0));
}

void YerothModifierWindow::display_quantite_restante_by_spinbox(int lots)
{
    double qte_lot = lineEdit_quantite_par_lot->text().toDouble();
    double qte_restante = lots * qte_lot;
    lineEdit_quantite_restante->setText(QString::number(qte_restante, 'f', 0));
}

void YerothModifierWindow::display_prix_vente()
{
    if (_lastEditedPrixVente != lineEdit_prix_vente->text())
    {
        return;
    }
    if (checkBox_tva->isChecked())
    {
        double prix_vente = lineEdit_prix_vente->text().toDouble();
        _montantTva = prix_vente * YerothERPConfig::tva_value;
        prix_vente = prix_vente + _montantTva;
        lineEdit_prix_vente->setText(QString::number(prix_vente, 'f', 2));
    }
}

void YerothModifierWindow::handleTVACheckBox(bool clicked)
{
    double prix_vente = lineEdit_prix_vente->text().toDouble();
    if (clicked && checkBox_tva->isChecked())
    {
        if (false == _tvaCheckBoxPreviousState)
        {
            _tvaCheckBoxPreviousState = true;
        }
        else
        {
            _tvaCheckBoxPreviousState = false;
        }
        //qDebug() << "\t ++_montantTva: " << _montantTva;
        _montantTva = prix_vente * YerothERPConfig::tva_value;
        prix_vente = prix_vente + _montantTva;
    }
    else
    {
        //qDebug() << "\t ++_montantTva: " << _montantTva;
        _montantTva = 0;
        if (true == _tvaCheckBoxPreviousState)
        {
            _tvaCheckBoxPreviousState = false;
            prix_vente = prix_vente / (1 + YerothERPConfig::tva_value);
        }
    }
    lineEdit_prix_vente->setText(QString::number(prix_vente, 'f', 2));
}

void YerothModifierWindow::actualiser_article()
{
    if (check_fields())
    {
        bool correctDatePeremption = true;
        if (dateEdit_date_peremption->date() <= QDate::currentDate())
        {
            QString warnMsg("La date de péremption n'est pas postdatée!\n\n"
                            "Continuer avec l'actualisation des données de l'article ?");
            if (QMessageBox::Ok ==
                    YerothQMessageBox::question(this,
                                               QObject::trUtf8
                                               ("actualiser les détails d'un stock"),
                                               FROM_UTF8_STRING(warnMsg), QMessageBox::Cancel, QMessageBox::Ok))
            {
                // nothing here
            }
            else
            {
                /**
                 * The user doesn't want to continue with the current
                 * peremption date since it is not post-dated.
                 */
                correctDatePeremption = false;
            }
        }
        else
        {
            /**
             * This empty 'else' case is necessary to avoid that
             * the user gets twice the message.
             */
        }
        if (!correctDatePeremption)
        {
            return;
        }
        QString msgEnregistrer("Poursuivre avec la modification du stock '");
        msgEnregistrer.append(lineEdit_designation->text()).append("' ?");
        if (QMessageBox::Ok ==
                YerothQMessageBox::question(this, _windowName, msgEnregistrer, QMessageBox::Cancel, QMessageBox::Ok))
        {
        	YerothUtils::startTransaction();

            QSqlRecord record = _curStocksTableModel->record(_allWindows->getLastSelectedListerRow());
            //YerothSqlTableModel &stocksModificationsSqlTableModel = _allWindows->getSqlTableModel_stocks_modifications();
            //QSqlRecord  stocksModificationsRecord = stocksModificationsSqlTableModel.record();
            QString description_produit(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::DESCRIPTION_PRODUIT));
            if (!YerothUtils::isEqualCaseInsensitive(description_produit, textEdit_description->toPlainText()))
            {
                record.setValue(YerothDatabaseTableColumn::DESCRIPTION_PRODUIT, textEdit_description->toPlainText());
            }

            record.setValue(YerothDatabaseTableColumn::LOCALISATION_STOCK, lineEdit_localisation_produit->text());
            record.setValue(YerothDatabaseTableColumn::MONTANT_TVA, _montantTva);

            YerothPOSUser *currentUser = YerothUtils::getAllWindows()->getUser();

            if (0 != currentUser)
            {
            	if (currentUser->isManager() ||
            		currentUser->isGestionaireDesStocks())
            	{
            		record.setValue(YerothDatabaseTableColumn::REFERENCE_RECU_DACHAT, lineEdit_reference_recu_dachat->text());
            		record.setValue(YerothDatabaseTableColumn::PRIX_DACHAT, lineEdit_prix_dachat->text().toDouble());
            	}
            }

            record.setValue(YerothDatabaseTableColumn::PRIX_VENTE, lineEdit_prix_vente->text().toDouble());

            record.setValue(YerothDatabaseTableColumn::STOCK_MINIMUM, lineEdit_stock_minimum->text().toDouble());

            record.setValue(YerothDatabaseTableColumn::DATE_PEREMPTION, dateEdit_date_peremption->date());

            if (label_image_produit->pixmap())
            {
                QByteArray bytes;
                YerothUtils::savePixmapToByteArray(bytes, *label_image_produit->pixmap(), "JPG");
                record.setValue(YerothDatabaseTableColumn::IMAGE_PRODUIT, QVariant::fromValue(bytes));
            }

            bool success = _curStocksTableModel->updateRecord(_allWindows->getLastSelectedListerRow(), record);

            YerothUtils::commitTransaction();

            /*
             * To avoid having two message boxes shown at the same
             * time to the user.
             */
            sleep(0.5);


            QString retMsg("Les détails du stock '");

            retMsg.append(lineEdit_designation->text());

            if (success)
            {
                retMsg.append("' ont été actualisés avec succès!");
                YerothQMessageBox::information(this, QObject::trUtf8("succès"), FROM_UTF8_STRING(retMsg));
            }
            else
            {
                retMsg.append("' n'ont pas pu être actualisés avec succès!");
                YerothQMessageBox::warning(this, QObject::trUtf8("échec"), FROM_UTF8_STRING(retMsg));
            }

            _allWindows->_stocksWindow->rendreVisible(_curStocksTableModel);

            this->rendreInvisible();
        }
        else
        {
            msgEnregistrer.clear();
            msgEnregistrer.append("Vous avez annulé la modification des détails du stock '")
            .append(lineEdit_designation->text()).append("' !");
            YerothQMessageBox::information(this, tr("annulation"), tr(msgEnregistrer.toStdString().c_str()),
                                          QMessageBox::Ok);
        }
    }
    else
    {
    }
}

void YerothModifierWindow::supprimer_ce_stock()
{
    QSqlRecord record = _curStocksTableModel->record(_allWindows->getLastSelectedListerRow());
    QString msgSupprimer("Poursuivre avec la suppression du stock \"");
    msgSupprimer.append(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::DESIGNATION));
    msgSupprimer.append("\" ?");
    if (QMessageBox::Ok ==
            YerothQMessageBox::question(this, "suppression", msgSupprimer,
                                       QMessageBox::Cancel, QMessageBox::Ok))
    {
        bool resRemoved = _curStocksTableModel->removeRow(_allWindows->getLastSelectedListerRow());
        //qDebug() << "YerothModifierWindow::supprimer_ce_stock() " << resRemoved;
        afficherStocks();
        if (resRemoved)
        {
            msgSupprimer.clear();
            msgSupprimer.append("Le stock \"");
            msgSupprimer.append(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::DESIGNATION));
            msgSupprimer.append(QObject::trUtf8("\" a été supprimé."));
            YerothQMessageBox::information(this, "suppression d'un stock avec succès", msgSupprimer);
        }
        else
        {
            msgSupprimer.clear();
            msgSupprimer.append("Le stock \"");
            msgSupprimer.append(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::DESIGNATION));
            msgSupprimer.append("\" ne pouvait pas être supprimé.");
            YerothQMessageBox::information(this, "échec de la suppression d'un stock", msgSupprimer);
        }
    }
    else
    {
    }
}

void YerothModifierWindow::supprimer_image_stock()
{
    QSqlRecord record = _curStocksTableModel->record(_allWindows->getLastSelectedListerRow());
    QVariant image_produit(record.value(YerothDatabaseTableColumn::IMAGE_PRODUIT));
    if (image_produit.toByteArray().isEmpty())
    {
        QString msg("Le stock '");
        msg.append(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::DESIGNATION)).append("' n'a pas d'image !");
        YerothQMessageBox::information(this, QObject::trUtf8("suppression de l'image d'un stock"),
                                      msg);
        return;
    }
    QString msgSupprimer("Poursuivre avec la suppression de l'image du stock \"");
    msgSupprimer.append(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::DESIGNATION));
    msgSupprimer.append("\" ?");
    if (QMessageBox::Ok ==
            YerothQMessageBox::question(this, QObject::trUtf8("suppression de l'image d'un stock"),
                                       msgSupprimer, QMessageBox::Cancel, QMessageBox::Ok))
    {
        record.setValue(YerothDatabaseTableColumn::IMAGE_PRODUIT, QVariant(QVariant::ByteArray));
        bool resRemoved = _curStocksTableModel->updateRecord(_allWindows->getLastSelectedListerRow(), record);
        //qDebug() << "YerothModifierWindow::supprimer_ce_stock() " << resRemoved;
        label_image_produit->clear();
        label_image_produit->setAutoFillBackground(false);
        if (resRemoved)
        {
            msgSupprimer.clear();
            msgSupprimer.append("L'image du stock \"");
            msgSupprimer.append(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::DESIGNATION));
            msgSupprimer.append(QObject::trUtf8("\" a été supprimé."));
            YerothQMessageBox::information(this,
                                          QObject::trUtf8("suppression de l'image du stock avec succès"),
                                          msgSupprimer);
        }
        else
        {
            msgSupprimer.clear();
            msgSupprimer.append("L'image du stock \"");
            msgSupprimer.append(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::DESIGNATION));
            msgSupprimer.append("\" ne pouvait pas être supprimé!");
            YerothQMessageBox::information(this,
                                          QObject::trUtf8("échec de la suppression de l'image d'un stock"),
                                          msgSupprimer);
        }
    }
    else
    {
    }
}

void YerothModifierWindow::rendreInvisible()
{
    this->clear_all_fields();
    _lastEditedPrixVente.clear();
    _montantTva = 0;
    _tvaPercent = YerothUtils::getTvaStringWithPercent();
    _tvaCheckBoxPreviousState = false;
    dateEdit_date_peremption->reset();
    YerothWindowsCommons::rendreInvisible();
}

void YerothModifierWindow::rendreVisible(YerothSqlTableModel * stocksTableModel)
{
    _logger->log("rendreVisible(YerothSqlTableModel *)");
    _curStocksTableModel = stocksTableModel;
    this->showItem();
    this->setVisible(true);
}

void YerothModifierWindow::showItem()
{
    QSqlRecord record = _curStocksTableModel->record(_allWindows->getLastSelectedListerRow());

    lineEdit_reference_produit->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::REFERENCE));

    lineEdit_designation->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::DESIGNATION));

    spinBox_lots->setValue(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::LOTS_ENTRANT).toInt());

    lineEdit_quantite_par_lot->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::QUANTITE_PAR_LOT));

    double quantite_par_lot = GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::QUANTITE_PAR_LOT).toDouble();

    lineEdit_quantite_par_lot->setText(QString::number(quantite_par_lot, 'f', 0));

    lineEdit_stock_minimum->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::STOCK_MINIMUM));

    lineEdit_reference_recu_dachat->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::REFERENCE_RECU_DACHAT));

    lineEdit_prix_dachat->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::PRIX_DACHAT));

    YerothPOSUser *currentUser = YerothUtils::getAllWindows()->getUser();

    if (0 != currentUser)
    {
    	if (!currentUser->isManager() ||
    		!currentUser->isGestionaireDesStocks())
    	{
    		lineEdit_reference_recu_dachat->setEnabled(false);
    		lineEdit_prix_dachat->setEnabled(false);
    	}
    	else
    	{
    		lineEdit_reference_recu_dachat->setEnabled(true);
    		lineEdit_prix_dachat->setEnabled(true);
    	}
    }

    double prix_vente = GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::PRIX_VENTE).toDouble();

    _montantTva = GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::MONTANT_TVA).toDouble();
    //qDebug() << "\t++ showItem, _montantTva: " << _montantTva;
    if (_montantTva > 0)
    {
        checkBox_tva->setChecked(true);
        _tvaCheckBoxPreviousState = true;
    }

    lineEdit_tva->setText(YerothUtils::getTvaStringWithPercent());
    lineEdit_prix_vente->setText(QString::number(prix_vente, 'f', 2));

    double quantite_restante = GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::QUANTITE_TOTAL).toDouble();

    lineEdit_quantite_restante->setText(QString::number(quantite_restante, 'f', 0));
    textEdit_description->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::DESCRIPTION_PRODUIT));

    QString date_peremption(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::DATE_PEREMPTION));

    dateEdit_date_peremption->setYerothEnabled(true);
    dateEdit_date_peremption->setMyDate(GET_DATE_FROM_STRING(date_peremption));
    lineEdit_categorie_produit->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::CATEGORIE));
    lineEdit_localisation_produit->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::LOCALISATION_STOCK));
    lineEdit_nom_entreprise_fournisseur->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::NOM_ENTREPRISE_FOURNISSEUR));

    QVariant img(record.value(YerothDatabaseTableColumn::IMAGE_PRODUIT));

    if (!img.isNull())
    {
        YerothUtils::loadPixmapFromDB(*label_image_produit, img, "JPG");
    }
    else
    {
        label_image_produit->setAutoFillBackground(false);
    }
}
