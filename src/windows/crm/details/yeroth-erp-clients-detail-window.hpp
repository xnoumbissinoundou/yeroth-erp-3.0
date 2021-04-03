/*
 * yeroth-erp-clients-detail-window.hpp
 *
 *      Author: PR. XAVIER NOUMBISSI NOUNDOU
 */

#ifndef SRC_WINDOWS_YEROTH_CLIENTS_DETAIL_WINDOW_HPP_
#define SRC_WINDOWS_YEROTH_CLIENTS_DETAIL_WINDOW_HPP_

#include "../../../../ui_yeroth-erp-clients-detail-window.h"

#include "src/utils/yeroth-erp-logger.hpp"

#include "src/windows/yeroth-erp-window-commons.hpp"

#include <QtWidgets/QMessageBox>


class QContextMenuEvent;

class YerothERPWindows;
class YerothSqlTableModel;
class YerothLogger;

class YerothClientsDetailWindow : public YerothWindowsCommons,
								  private Ui_YerothClientsDetailWindow
{
    Q_OBJECT

public:

	YEROTH_CLASS_OPERATORS

	YerothClientsDetailWindow();

	inline ~YerothClientsDetailWindow()
	{
		delete _logger;
	}

	inline QToolBar * getQMainWindowToolBar()
	{
		return toolBar_clientsDetailWindow;
	}

	virtual void definirCaissier();

    virtual void definirManager();

    virtual void definirVendeur();

    virtual void definirGestionaireDesStocks();

    virtual void definirMagasinier();

    virtual void definirPasDeRole();

    virtual void rendreInvisible();

    virtual void rendreVisible(YerothSqlTableModel *clientTableModel,
					   	   	   YerothSqlTableModel *stocksTableModel);

public slots:

	virtual bool imprimer_pdf_document();

	inline virtual void apropos()
	{
		YerothWindowsCommons::apropos(this);
	}

	inline virtual void help()
	{
		YerothQMessageBox::information(this,
						 QObject::trUtf8("aide"),
						 QObject::trUtf8("Vous pouvez modifier les détails de ce compte client "
										 "en cliquant sur 'Modifier' !"));
	}

protected:

	virtual void setupShortcuts();

private slots:

	inline virtual void imprimer_pdf_document_WITH_A_YEROTH_PROGRESS_BAR()
	{
		YerothProgressBar(this)(this,
								&YerothClientsDetailWindow::imprimer_pdf_document);
	}

	inline void YEROTH_PROGRESS_BAR_generer_la_carte_de_fidelite_du_client()
	{
		YerothProgressBar(this)(this,
						  	    &YerothClientsDetailWindow::generer_la_carte_de_fidelite_du_client);
	}

	bool generer_la_carte_de_fidelite_du_client();

	void afficher_groupes_dun_client();

	void private_payer_au_compteclient();

	void modifierCompteClient();

	void supprimerCompteClient();

private:

    void setupLineEdits();

    void showClientDetail();

    void checkCourrierAlerts();


    static const int TRUNCATE_POS_FOR_NOM_ENTREPRISE;


    YerothLogger			*_logger;

    QStringList				_curClientGroups;

    YerothSqlTableModel 	*_curClientTableModel;
};


#endif /* SRC_WINDOWS_YEROTH_CLIENTS_DETAIL_WINDOW_HPP_ */
