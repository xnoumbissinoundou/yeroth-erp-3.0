
RC_ICONS = yeroth-erp-3-0-icon.ico

YEROTH_ERP_3_0_CONFIG_BUILD = "NO_YEROTH_ERP_3_0_TESTING_UNIT_TEST"

YEROTH_ERP_3_0_TOUCH_SCREEN = "NO_YEROTH_ERP_3_0_TOUCH_SCREEN_SET"

DEFINES += $${YEROTH_ERP_3_0_CONFIG_BUILD}
DEFINES += $${YEROTH_ERP_3_0_TOUCH_SCREEN}
DEFINES += ${YEROTH_VIRTUAL_KEYBOARD_OPTIONS}
DEFINES += ${YEROTH_LANGUAGE}
DEFINES += ${YEROTH_DEBUG_LOG}
DEFINES += ${YEROTH_VERSION}
#DEFINES += ${YEROTH_CHECK_MEMORY}

#QML_IMPORT_PATH =

#include(yeroth-erp-3-0-qtvirtualkeyboard-deployment.pri)

CONFIG += precompile_header 
CONFIG += qt 
CONFIG += moc 

contains(YEROTH_ERP_3_0_CONFIG_BUILD, "NO_YEROTH_ERP_3_0_TESTING_UNIT_TEST") {
	CONFIG -= testcase
  
	QT -= testlib
  
	LIBS -= -lgcov
	
	QMAKE_CXXFLAGS_RELEASE -= --coverage
} else {
	CONFIG += testcase
  
	QT += testlib
  
	LIBS += -lgcov
	
	QMAKE_CXXFLAGS_RELEASE += --coverage
}

contains(YEROTH_ERP_3_0_TOUCH_SCREEN, "NO_YEROTH_ERP_3_0_TOUCH_SCREEN_SET") {
  CONFIG -= disable-desktop
} else {
  CONFIG += disable-desktop
}

#CONFIG += debug

CONFIG -= import_plugins

QT += widgets
QT += sql
QT += network
QT += dbus
QT += quick

#disable-desktop {
#    DEFINES += MAIN_QML=\\\"basic-b2qt.qml\\\"
#} 

#static {
#  QTPLUGIN += qtvirtualkeyboardplugin
#	QT += svg
#}

#LIBS += -L/usr/lib/i386-linux-gnu/qt5/plugins/styles -lqmotifstyle -lqplastiquestyle

QMAKE_CXXFLAGS_RELEASE -= -O2
#QMAKE_CXXFLAGS_RELEASE += -Wunused-parameter

QMAKE_CFLAGS_RELEASE -= -O2

DESTDIR = bin

MOC_DIR  = moc

OBJECTS_DIR = obj

FORMS = ui/yeroth-erp-main-window.ui \
			ui/yeroth-erp-configuration-comptabilite-window.ui \
			ui/stocks/yeroth-erp-sortir-window.ui \
		  ui/yeroth-erp-entrer-window.ui \
		  ui/stocks/yeroth-erp-mouvements_de_stocks-window.ui \
		  ui/stocks/yeroth-erp-marchandises-window.ui \
		  ui/stocks/yeroth-erp-modifier-window.ui \
			ui/stocks/yeroth-erp-historique-du-stock-window.ui \
			ui/stocks/yeroth-erp-stocks-window.ui \
		  ui/stocks/yeroth-erp-sortir-liststocks-window.ui \
		  ui/stocks/yeroth-erp-stock-detail-window.ui \
			ui/pos/yeroth-erp-pointdevente-liststocks-window.ui \
		  ui/pos/yeroth-erp-pointdevente-window.ui \
		  ui/pos/yeroth-erp-pointdevente-methode-paiement-carte-credit-dialog.ui \
		  ui/pos/yeroth-erp-pointdevente-methode-paiement-comptant-entree-dialog.ui \
		  ui/pos/yeroth-erp-pointdevente-methode-paiement-dialog.ui \
			ui/purchase/yeroth-erp-achats-detail-window.ui \
			ui/purchase/yeroth-erp-achats-window.ui \
			ui/purchase/yeroth-erp-achats-aux-fournisseurs-window.ui \
			ui/supplier/yeroth-erp-payer-fournisseur-window.ui \
			ui/supplier/yeroth-erp-tableau-des-transactions-du-fournisseur-window.ui \
			ui/supplier/yeroth-erp-modifier-fournisseur-window.ui \
			ui/supplier/yeroth-erp-creer-fournisseur-window.ui \
			ui/supplier/yeroth-erp-fournisseur-details-window.ui \
		  ui/supplier/yeroth-erp-fournisseurs-window.ui \
			ui/crm/creer/yeroth-erp-creer-un-programme-de-fidelite-clients-window.ui \
			ui/crm/creer/yeroth-erp-creer-compte-client-window.ui \
			ui/crm/creer/yeroth-erp-creer-groupe-de-clients-window.ui \
			ui/crm/details/yeroth-erp-details-dun-groupe-de-clients-window.ui \
			ui/crm/details/yeroth-erp-clients-detail-window.ui \
			ui/crm/details/yeroth-erp-details-dun-programme-de-fidelite-clients-window.ui \
			ui/crm/yeroth-erp-tableau-des-transactions-du-client-window.ui \
			ui/crm/modifier/yeroth-erp-modifier-compte-client-window.ui \
			ui/crm/modifier/yeroth-erp-modifier-groupe-de-clients-window.ui \
			ui/crm/modifier/yeroth-erp-modifier-un-programme-de-fidelite-clients-window.ui \
			ui/crm/yeroth-erp-payer-compte-client-window.ui \
			ui/crm/yeroth-erp-programmes-de-fidelite-clients-window.ui \
			ui/crm/yeroth-erp-groupes-de-clients-window.ui \
			ui/crm/yeroth-erp-groupes-dun-client-window.ui \
			ui/crm/yeroth-erp-clients-window.ui \
			ui/yeroth-erp-paiements-window.ui \
		  ui/yeroth-erp-ventes-window.ui \
		  ui/yeroth-erp-tableaux-de-bord-window.ui \
			ui/yeroth-erp-alertes-window.ui \
		  ui/yeroth-erp-changer-utilisateur-dialog.ui \
		  ui/yeroth-erp-admin-window.ui \
		  ui/yeroth-erp-admin-search-form.ui \
		  ui/yeroth-erp-admin-create-window.ui \
		  ui/yeroth-erp-admin-lister-window.ui \
		  ui/yeroth-erp-admin-detail-window.ui \
			ui/yeroth-erp-admin-modifier-window.ui

PRECOMPILED_HEADER  = src/include/yeroth-erp-3-0-precompiled-header.hpp

HEADERS = src/include/yeroth-erp-3-0-precompiled-header.hpp \
			src/yeroth-erp-windows.hpp \
		  src/imports/yeroth-erp-database-table-import-info.hpp \
			src/imports/yeroth-erp-marchandise-import.hpp \
			src/imports/yeroth-erp-stock-import.hpp \
		  src/include/yeroth-erp-3-0-definition-special-qt5.7-macro.hpp \
		  src/include/yeroth-erp-3-0-definition-colour-rgb.hpp \
		  src/include/yeroth-erp-3-0-definition-user-manual.hpp \
		  src/include/yeroth-erp-3-0-definition-file-folder.hpp \
		  src/include/yeroth-erp-3-0-definition-format-date-time.hpp \
		  src/include/yeroth-erp-3-0-definition-initial-focus-searchbar.hpp \
		  src/include/yeroth-erp-3-0-definition-month.hpp \
		  src/include/yeroth-erp-3-0-definition-shortcut.hpp \
		  src/include/yeroth-erp-3-0-software.text-configuration.hpp \
		  src/widgets/yeroth-erp-progress-bar.hpp \
	  	src/widgets/yeroth-erp-qcolumn-view.hpp \
	  	src/widgets/yeroth-erp-qtabwidget.hpp \
			src/widgets/table-view/yeroth-erp-table-view-with-pagination.hpp \
			src/widgets/table-view/yeroth-erp-table-view.hpp \
		  src/widgets/table-view/yeroth-erp-paiements-table-view.hpp \
		  src/widgets/table-view/yeroth-erp-stocks-table-view.hpp \
		  src/widgets/table-view/yeroth-erp-ventes-table-view.hpp \
		  src/widgets/table-view/yeroth-erp-mouvements_de_stocks-table-view.hpp \
		  src/widgets/table-view/yeroth-erp-fournisseurs-table-view.hpp \
			src/widgets/table-view/yeroth-erp-programmes-de-fidelite-clients-table-view.hpp \
			src/widgets/table-view/yeroth-erp-groupes-de-clients-table-view.hpp \
		  src/widgets/table-view/yeroth-erp-clients-table-view.hpp \
		  src/widgets/table-view/yeroth-erp-alertes-table-view.hpp \
		  src/widgets/table-view/yeroth-erp-achats-aux-fournisseurs-table-view.hpp \
		  src/widgets/table-view/yeroth-erp-achats-table-view.hpp \
		  src/widgets/table-view/yeroth-erp-marchandises-table-view.hpp \
			src/widgets/yeroth-erp-qtable-widget-item.hpp \
		  src/widgets/yeroth-erp-line-edit.hpp \
		  src/widgets/yeroth-erp-qmessage-box.hpp \
		  src/widgets/yeroth-erp-push-button.hpp \
		  src/widgets/yeroth-erp-text-edit.hpp \
		  src/widgets/yeroth-erp-combo-box.hpp \
		  src/widgets/yeroth-erp-qstandard-item-model.hpp \
		  src/widgets/yeroth-erp-qstandard-item.hpp \
		  src/widgets/yeroth-erp-spinbox.hpp \
		  src/widgets/yeroth-erp-datetime-edit.hpp \
		  src/widgets/yeroth-erp-qcheckbox.hpp \
		  src/widgets/yeroth-erp-select-db-qcheckbox.hpp \
			src/widgets/yeroth-erp-creer-un-groupe-de-clients-table-widget.hpp \
			src/widgets/yeroth-erp-groupes-dun-client-table-widget.hpp \
			src/widgets/yeroth-erp-point-de-vente-table-widget.hpp \
		  src/widgets/yeroth-erp-table-widget.hpp \
		  src/process/yeroth-erp-process-info.hpp \
		  src/process/yeroth-erp-process.hpp \
		  src/dialogs/yeroth-erp-dialog-commons.hpp \
		  src/dialogs/yeroth-erp-generic-select-db-field-dialog.hpp \
		  src/dialogs/yeroth-erp-changer-utilisateur-dialog.hpp \
		  src/dialogs/yeroth-erp-pointdevente-methode-paiement-carte-credit-dialog.hpp \
		  src/dialogs/yeroth-erp-pointdevente-methode-paiement-comptant-entree-dialog.hpp \
		  src/dialogs/yeroth-erp-pointdevente-methode-paiement-dialog.hpp \
			src/windows/crm/creer/yeroth-erp-creer-groupe-de-clients-window.hpp \
			src/windows/crm/creer/yeroth-erp-creer-un-programme-de-fidelite-clients-window.hpp \
		  src/windows/crm/creer/yeroth-erp-creer-compte-client-window.hpp \
			src/windows/crm/modifier/yeroth-erp-modifier-compte-client-window.hpp \
			src/windows/crm/details/yeroth-erp-details-dun-groupe-de-clients-window.hpp \
			src/windows/crm/details/yeroth-erp-clients-detail-window.hpp \
			src/windows/crm/details/yeroth-erp-details-dun-programme-de-fidelite-clients-window.hpp \
			src/windows/crm/yeroth-erp-programmes-de-fidelite-clients-window.hpp \
		  src/windows/crm/yeroth-erp-groupes-de-clients-window.hpp \
		  src/windows/crm/yeroth-erp-groupes-dun-client-window.hpp \
		  src/windows/crm/yeroth-erp-clients-window.hpp \
			src/windows/crm/yeroth-erp-payer-compte-client-window.hpp \
		  src/windows/crm/yeroth-erp-tableau-des-transactions-du-client-window.hpp \
		  src/windows/yeroth-erp-entrer-window.hpp \
		  src/windows/stocks/yeroth-erp-mouvements_de_stocks-window.hpp \
		  src/windows/stocks/yeroth-erp-modifier-window.hpp \
		  src/windows/stocks/yeroth-erp-marchandises-window.hpp \
		  src/windows/stocks/yeroth-erp-sortir-liststocks-window.hpp \
		  src/windows/stocks/yeroth-erp-sortir-window.hpp \
		  src/windows/stocks/yeroth-erp-historique-du-stock-window.hpp \
		  src/windows/stocks/yeroth-erp-stock-detail-window.hpp \
		  src/windows/stocks/yeroth-erp-stocks-window.hpp \
			src/windows/purchase/yeroth-erp-achats-aux-fournisseurs-window.hpp \
		  src/windows/purchase/yeroth-erp-achats-detail-window.hpp \
		  src/windows/purchase/yeroth-erp-achats-window.hpp \
		  src/windows/pos/yeroth-erp-pointdevente-window.hpp \
		  src/windows/pos/yeroth-erp-pointdevente-liststocks-window.hpp \
		  src/windows/supplier/yeroth-erp-tableau-des-transactions-du-fournisseur-window.hpp \
			src/windows/supplier/yeroth-erp-payer-fournisseur-window.hpp \
			src/windows/supplier/yeroth-erp-modifier-fournisseur-window.hpp \
			src/windows/supplier/yeroth-erp-fournisseurs-window.hpp \
			src/windows/supplier/yeroth-erp-creer-fournisseur-window.hpp \
			src/windows/supplier/yeroth-erp-fournisseur-details-window.hpp \
		  src/windows/yeroth-erp-configuration-comptabilite-window.hpp \
		  src/windows/yeroth-erp-window-commons.hpp \
		  src/windows/yeroth-erp-main-window.hpp \
		  src/windows/yeroth-erp-alertes-window.hpp \
		  src/windows/yeroth-erp-tableaux-de-bord-window.hpp \
		  src/windows/yeroth-erp-paiements-window.hpp \
		  src/windows/yeroth-erp-ventes-window.hpp \
		  src/tests/yeroth-erp-test-yeroth-erp-table-view.hpp \
			src/tests/yeroth-erp-test-yeroth-erp-windows.hpp \
		  src/tests/utils/yeroth-erp-test-import-csv-file-data.hpp \
			src/tests/utils/yeroth-erp-test-utils.hpp \
			src/users/yeroth-erp-users.hpp \
		  src/users/yeroth-erp-user-vendeur.hpp \		  
		  src/users/yeroth-erp-user-gestionaire-des-stocks.hpp \		  
		  src/users/yeroth-erp-user-manager.hpp \
		  src/users/yeroth-erp-user-magasinier.hpp \
		  src/users/yeroth-erp-user-caissier.hpp \
		  src/users/yeroth-erp-user-administrateur.hpp \
		  src/utils/credit-card/yeroth-erp-carte-credit-info.hpp \
			src/utils/windows/yeroth-erp-abstract-class-detail-view-window.hpp \
			src/utils/windows/yeroth-erp-abstract-class-yerothsearch-window.hpp \
		  src/utils/virtual-keyboard/yeroth-erp-key-emitter.hpp \
		  src/utils/yeroth-erp-sqltable-model.hpp \
		  src/utils/yeroth-erp-config.hpp \
		  src/utils/yeroth-erp-service-stock-marchandise-data.hpp \
			src/utils/yeroth-erp-utils.hpp \
		  src/utils/yeroth-erp-database.hpp \
		  src/utils/yeroth-erp-logger.hpp \
		  src/utils/yeroth-erp-historique-stock.hpp \
		  src/utils/yeroth-erp-info-entreprise.hpp \
		  src/utils/yeroth-erp-article-vente-info.hpp \
		  src/utils/yeroth-erp-database-table-column.hpp \
		  src/utils/yeroth-erp-command-line-parser.hpp \
		  src/utils/yeroth-erp-style.hpp \
		  src/utils/print_latex_pdf/yeroth-erp-print_yeroth_latex_util_pdf.hpp \
			src/dbus/yeroth-erp-dbus-client.hpp \
		  src/dbus/yeroth-erp-dbus-client-adaptor.hpp \
		  src/dbus/yeroth-erp-dbus-server.hpp \
		  src/dbus/yeroth-erp-dbus-server-adaptor.hpp \
		  src/admin/yeroth-erp-admin-windows-commons.hpp \
		  src/admin/yeroth-erp-admin-window.hpp \
		  src/admin/yeroth-erp-admin-search-form.hpp \		  
		  src/admin/widgets/table-view/yeroth-erp-product-department-table-view.hpp \
			src/admin/widgets/table-view/yeroth-erp-alert-table-view.hpp \
		  src/admin/widgets/table-view/yeroth-erp-site-table-view.hpp \
		  src/admin/widgets/table-view/yeroth-erp-user-table-view.hpp \
			src/admin/widgets/table-view/yeroth-erp-category-table-view.hpp \
			src/admin/widgets/table-view/yeroth-erp-bank-account-table-view.hpp \
		  src/admin/creer/yeroth-erp-admin-create-window.hpp \
		  src/admin/lister/yeroth-erp-admin-lister-window.hpp \
		  src/admin/detail/yeroth-erp-admin-detail-window.hpp \		
		  src/admin/modifier/yeroth-erp-admin-modifier-window.hpp
 
SOURCES = src/yeroth-erp-3-0.cpp \
		  src/yeroth-erp-windows.cpp \
			src/imports/yeroth-erp-marchandise-import.cpp \
		  src/imports/yeroth-erp-stock-import.cpp \
		  src/widgets/yeroth-erp-progress-bar.cpp \
	  	src/widgets/yeroth-erp-qcolumn-view.cpp \
		  src/widgets/yeroth-erp-qtabwidget.cpp \
			src/widgets/table-view/yeroth-erp-table-view-with-pagination.cpp \
			src/widgets/table-view/yeroth-erp-table-view.cpp \
		  src/widgets/table-view/yeroth-erp-paiements-table-view.cpp \
		  src/widgets/table-view/yeroth-erp-stocks-table-view.cpp \
			src/widgets/table-view/yeroth-erp-ventes-table-view.cpp \
		  src/widgets/table-view/yeroth-erp-mouvements_de_stocks-table-view.cpp \
		  src/widgets/table-view/yeroth-erp-fournisseurs-table-view.cpp \
			src/widgets/table-view/yeroth-erp-groupes-de-clients-table-view.cpp \
		  src/widgets/table-view/yeroth-erp-clients-table-view.cpp \
		  src/widgets/table-view/yeroth-erp-alertes-table-view.cpp \
		  src/widgets/table-view/yeroth-erp-achats-aux-fournisseurs-table-view.cpp \
		  src/widgets/table-view/yeroth-erp-achats-table-view.cpp \
		  src/widgets/table-view/yeroth-erp-marchandises-table-view.cpp \
			src/widgets/table-view/yeroth-erp-programmes-de-fidelite-clients-table-view.cpp \
			src/widgets/yeroth-erp-qtable-widget-item.cpp \
		  src/widgets/yeroth-erp-line-edit.cpp \
		  src/widgets/yeroth-erp-push-button.cpp \
		  src/widgets/yeroth-erp-text-edit.cpp \
		  src/widgets/yeroth-erp-combo-box.cpp \
		  src/widgets/yeroth-erp-qmessage-box.cpp \
		  src/widgets/yeroth-erp-qstandard-item-model.cpp \		  
		  src/widgets/yeroth-erp-qstandard-item.cpp \		  
		  src/widgets/yeroth-erp-spinbox.cpp \
		  src/widgets/yeroth-erp-datetime-edit.cpp \
		  src/widgets/yeroth-erp-qcheckbox.cpp \
		  src/widgets/yeroth-erp-select-db-qcheckbox.cpp \
			src/widgets/yeroth-erp-creer-un-groupe-de-clients-table-widget.cpp \
			src/widgets/yeroth-erp-groupes-dun-client-table-widget.cpp \
			src/widgets/yeroth-erp-point-de-vente-table-widget.cpp \
			src/widgets/yeroth-erp-table-widget.cpp \
		  src/process/yeroth-erp-process-info.cpp \
		  src/process/yeroth-erp-process.cpp \
		  src/dialogs/yeroth-erp-dialog-commons.cpp \
		  src/dialogs/yeroth-erp-generic-select-db-field-dialog.cpp \
		  src/dialogs/yeroth-erp-changer-utilisateur-dialog.cpp \
		  src/dialogs/yeroth-erp-pointdevente-methode-paiement-carte-credit-dialog.cpp \
		  src/dialogs/yeroth-erp-pointdevente-methode-paiement-comptant-entree-dialog.cpp \
		  src/dialogs/yeroth-erp-pointdevente-methode-paiement-dialog.cpp \
			src/windows/yeroth-erp-configuration-comptabilite-window.cpp \
		  src/windows/crm/creer/yeroth-erp-creer-groupe-de-clients-window.cpp \
			src/windows/crm/creer/yeroth-erp-creer-un-programme-de-fidelite-clients-window.cpp \
		  src/windows/crm/creer/yeroth-erp-creer-compte-client-window.cpp \
			src/windows/crm/details/yeroth-erp-details-dun-groupe-de-clients-window.cpp \
			src/windows/crm/details/yeroth-erp-clients-detail-window.cpp \
			src/windows/crm/details/yeroth-erp-details-dun-programme-de-fidelite-clients-window.cpp \
			src/windows/crm/modifier/yeroth-erp-modifier-compte-client-window.cpp \
			src/windows/crm/yeroth-erp-programmes-de-fidelite-clients-window.cpp \
		  src/windows/crm/yeroth-erp-groupes-de-clients-window.cpp \
		  src/windows/crm/yeroth-erp-groupes-dun-client-window.cpp \
			src/windows/crm/yeroth-erp-clients-window.cpp \
			src/windows/crm/yeroth-erp-payer-compte-client-window.cpp \
		  src/windows/crm/yeroth-erp-tableau-des-transactions-du-client-window.cpp \
		  src/windows/yeroth-erp-entrer-window.cpp \
		  src/windows/stocks/yeroth-erp-mouvements_de_stocks-window.cpp \
		  src/windows/stocks/yeroth-erp-modifier-window.cpp \
		  src/windows/stocks/yeroth-erp-marchandises-window.cpp \
			src/windows/stocks/yeroth-erp-sortir-liststocks-window.cpp \
		  src/windows/stocks/yeroth-erp-sortir-window.cpp \
			src/windows/stocks/yeroth-erp-historique-du-stock-window.cpp \
		  src/windows/stocks/yeroth-erp-stock-detail-window.cpp \
		  src/windows/stocks/yeroth-erp-stocks-window.cpp \
		  src/windows/purchase/yeroth-erp-achats-aux-fournisseurs-window.cpp \
		  src/windows/purchase/yeroth-erp-achats-detail-window.cpp \
		  src/windows/purchase/yeroth-erp-achats-window.cpp \
		  src/windows/pos/yeroth-erp-pointdevente-window.cpp \
		  src/windows/pos/yeroth-erp-pointdevente-liststocks-window.cpp \
			src/windows/supplier/yeroth-erp-tableau-des-transactions-du-fournisseur-window.cpp \
			src/windows/supplier/yeroth-erp-payer-fournisseur-window.cpp \
			src/windows/supplier/yeroth-erp-modifier-fournisseur-window.cpp \
			src/windows/supplier/yeroth-erp-fournisseurs-window.cpp \
			src/windows/supplier/yeroth-erp-creer-fournisseur-window.cpp \
			src/windows/supplier/yeroth-erp-fournisseur-details-window.cpp \
		  src/windows/yeroth-erp-window-commons.cpp \
		  src/windows/yeroth-erp-main-window.cpp \
		  src/windows/yeroth-erp-alertes-window.cpp \
		  src/windows/yeroth-erp-tableaux-de-bord-window.cpp \
		  src/windows/yeroth-erp-paiements-window.cpp \
		  src/windows/yeroth-erp-ventes-window.cpp \
		  src/tests/yeroth-erp-test-yeroth-erp-table-view.cpp \
			src/tests/yeroth-erp-test-yeroth-erp-windows.cpp \
		  src/tests/utils/yeroth-erp-test-import-csv-file-data.cpp \
			src/tests/utils/yeroth-erp-test-utils.cpp \
		  src/users/yeroth-erp-users.cpp \
		  src/users/yeroth-erp-user-vendeur.cpp \		  
		  src/users/yeroth-erp-user-gestionaire-des-stocks.cpp \
		  src/users/yeroth-erp-user-manager.cpp \
		  src/users/yeroth-erp-user-magasinier.cpp \
		  src/users/yeroth-erp-user-caissier.cpp \
		  src/users/yeroth-erp-user-administrateur.cpp \
		  src/utils/credit-card/yeroth-erp-carte-credit-info.cpp \
			src/utils/windows/yeroth-erp-abstract-class-detail-view-window.cpp \
		  src/utils/windows/yeroth-erp-abstract-class-yerothsearch-window.cpp \
		  src/utils/virtual-keyboard/yeroth-erp-key-emitter.cpp \
		  src/utils/yeroth-erp-sqltable-model.cpp \
		  src/utils/yeroth-erp-config.cpp \
		  src/utils/yeroth-erp-utils.cpp \
		  src/utils/yeroth-erp-database.cpp \		  
		  src/utils/yeroth-erp-logger.cpp \
		  src/utils/yeroth-erp-historique-stock.cpp \
		  src/utils/yeroth-erp-info-entreprise.cpp \
		  src/utils/yeroth-erp-article-vente-info.cpp \
		  src/utils/yeroth-erp-database-table-column.cpp \
		  src/utils/yeroth-erp-command-line-parser.cpp \
			src/utils/yeroth-erp-style.cpp \
			src/utils/print_latex_pdf/yeroth-erp-print_yeroth_latex_util_pdf.cpp \
		  src/dbus/yeroth-erp-dbus-client.cpp \
		  src/dbus/yeroth-erp-dbus-server.cpp \
		  src/dbus/yeroth-erp-dbus-client-adaptor.cpp \
		  src/dbus/yeroth-erp-dbus-server-adaptor.cpp \
		  src/admin/yeroth-erp-admin-windows-commons.cpp \
		  src/admin/yeroth-erp-admin-window.cpp \
		  src/admin/yeroth-erp-admin-search-form.cpp \		  
		  src/admin/widgets/table-view/yeroth-erp-product-department-table-view.cpp \
			src/admin/widgets/table-view/yeroth-erp-alert-table-view.cpp \
		  src/admin/widgets/table-view/yeroth-erp-site-table-view.cpp \
		  src/admin/widgets/table-view/yeroth-erp-user-table-view.cpp \
		  src/admin/widgets/table-view/yeroth-erp-category-table-view.cpp \
			src/admin/widgets/table-view/yeroth-erp-bank-account-table-view.cpp \
		  src/admin/creer/yeroth-erp-admin-create-window.cpp \
		  src/admin/lister/yeroth-erp-admin-lister-window.cpp \
		  src/admin/detail/yeroth-erp-admin-detail-window.cpp \
		  src/admin/modifier/yeroth-erp-admin-modifier-window.cpp

RESOURCES    = yeroth-erp-3-0.qrc
TRANSLATIONS = yeroth-erp-3-0_english.ts

# install
target.path = yeroth-erp-3-0
sources.files = $$SOURCES $$HEADERS $$RESOURCES $$FORMS *.pro
sources.path = .
INSTALLS += target sources
