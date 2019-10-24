/*
 * yeroth-erp-qmessage-box.cpp
 *
 *  Created on: Mar 17, 2018
 *      Author: Dipl.-Inf. Xavier NOUMBISSI NOUNDOU, Ph.D. (ABD)
 *      Email:  xnoundou7@gmail.com
 */

#include "src/widgets/yeroth-erp-qmessage-box.hpp"

#include "src/admin/yeroth-erp-admin-windows-commons.hpp"

# include <QtCore/QDebug>

#include <QtWidgets/QToolBar>

#include <QtWidgets/QMenuBar>

#include "../windows/yeroth-erp-window-commons.hpp"


void YerothQMessageBox::about(YerothPOSAdminWindowsCommons *aCallingWindow,
        					  QString transLatedPurposeStr,
							  QString translatedMsg)
{
    QToolBar *aCallingWindowToolbar = aCallingWindow->getQMainWindowToolBar();

    aCallingWindowToolbar->setStyleSheet(aCallingWindow->qMessageBoxStyleSheet());

    QMessageBox::about(aCallingWindowToolbar,
                              QString("%1 - %2")
                                    .arg(aCallingWindow->getWindowName(),
                                         transLatedPurposeStr),
                              translatedMsg);
}


QMessageBox::StandardButton YerothQMessageBox::information(YerothPOSAdminWindowsCommons *aCallingWindow,
        												   QString transLatedPurposeStr,
														   QString translatedMsg,
														   StandardButtons buttons /* = Ok*/,
														   StandardButton defaultButton /*= NoButton*/)
{
	QToolBar *aCallingWindowToolbar = aCallingWindow->getQMainWindowToolBar();

	aCallingWindowToolbar->setStyleSheet(aCallingWindow->qMessageBoxStyleSheet());

	return QMessageBox::information(aCallingWindowToolbar,
									QString("%1 - %2")
										.arg(aCallingWindow->getWindowName(),
											 transLatedPurposeStr),
											 translatedMsg,
											 buttons,
											 defaultButton);
}


QMessageBox::StandardButton YerothQMessageBox::question(YerothPOSAdminWindowsCommons *aCallingWindow,
        												QString transLatedPurposeStr,
														QString translatedMsg,
														StandardButtons buttons /* = Ok*/,
														StandardButton defaultButton /*= NoButton*/)
{
    /*QString stylesheet(QString("QMessageBox {background-color: rgb(%1);}")
    						.arg(COLOUR_RGB_STRING_YEROTH_ORANGE_243_162_0));
    qDebug() << "++ stylesheet: " << stylesheet;*/

    QToolBar *aCallingWindowToolbar = aCallingWindow->getQMainWindowToolBar();

    aCallingWindowToolbar->setStyleSheet(aCallingWindow->qMessageBoxStyleSheet());

    return QMessageBox::question(aCallingWindowToolbar,
                                 QString("%1 - %2")
                                 .arg(aCallingWindow->getWindowName(),
                                      transLatedPurposeStr),
                                 translatedMsg,
                                 buttons,
                                 defaultButton);
}


QMessageBox::StandardButton YerothQMessageBox::warning(YerothPOSAdminWindowsCommons *aCallingWindow,
        											   QString transLatedPurposeStr,
													   QString translatedMsg,
													   StandardButton defaultButton /* = Ok */)
{
    QToolBar *aCallingWindowToolbar = aCallingWindow->getQMainWindowToolBar();

    aCallingWindowToolbar->setStyleSheet(aCallingWindow->qMessageBoxStyleSheet());

    return QMessageBox::warning(aCallingWindowToolbar,
                                QString("%1 - %2")
                                .arg(aCallingWindow->getWindowName(),
                                     transLatedPurposeStr),
                                translatedMsg);
}


void YerothQMessageBox::about(YerothWindowsCommons *aCallingWindow,
        											 QString transLatedPurposeStr,
													 QString translatedMsg)
{
    /*QString stylesheet(QString("QMessageBox {background-color: rgb(%1);}")
    						.arg(COLOUR_RGB_STRING_YEROTH_ORANGE_243_162_0));
    qDebug() << "++ stylesheet: " << stylesheet;*/

    QToolBar *aCallingWindowToolbar = aCallingWindow->getQMainWindowToolBar();

    aCallingWindowToolbar->setStyleSheet(aCallingWindow->qMessageBoxStyleSheet());

    QMessageBox::about(aCallingWindowToolbar,
                              QString("%1 - %2")
                                    .arg(aCallingWindow->getWindowName(),
                                         transLatedPurposeStr),
                              translatedMsg);
}


QMessageBox::StandardButton YerothQMessageBox::information(YerothWindowsCommons *aCallingWindow,
        QString transLatedPurposeStr,
        QString translatedMsg,
        StandardButtons buttons /* = Ok*/,
        StandardButton defaultButton /*= NoButton*/)
{
    /*QString stylesheet(QString("QMessageBox {background-color: rgb(%1);}")
    						.arg(COLOUR_RGB_STRING_YEROTH_ORANGE_243_162_0));
    qDebug() << "++ stylesheet: " << stylesheet;*/

    QToolBar *aCallingWindowToolbar = aCallingWindow->getQMainWindowToolBar();

    aCallingWindowToolbar->setStyleSheet(aCallingWindow->qMessageBoxStyleSheet());

    return QMessageBox::information(aCallingWindowToolbar,
                                    QString("%1 - %2")
                                    .arg(aCallingWindow->getWindowName(),
                                         transLatedPurposeStr),
                                    translatedMsg,
                                    buttons,
                                    defaultButton);
}


QMessageBox::StandardButton YerothQMessageBox::question(YerothWindowsCommons *aCallingWindow,
        QString transLatedPurposeStr,
        QString translatedMsg,
        StandardButtons buttons /* = Ok*/,
        StandardButton defaultButton /*= NoButton*/)
{
    /*QString stylesheet(QString("QMessageBox {background-color: rgb(%1);}")
    						.arg(COLOUR_RGB_STRING_YEROTH_ORANGE_243_162_0));
    qDebug() << "++ stylesheet: " << stylesheet;*/

    QToolBar *aCallingWindowToolbar = aCallingWindow->getQMainWindowToolBar();

    aCallingWindowToolbar->setStyleSheet(aCallingWindow->qMessageBoxStyleSheet());

    return QMessageBox::question(aCallingWindowToolbar,
                                 QString("%1 - %2")
                                 .arg(aCallingWindow->getWindowName(),
                                      transLatedPurposeStr),
                                 translatedMsg,
                                 buttons,
                                 defaultButton);
}


QMessageBox::StandardButton YerothQMessageBox::warning(YerothWindowsCommons *aCallingWindow,
        QString transLatedPurposeStr,
        QString translatedMsg)
{
    /*QString stylesheet(QString("QMessageBox {background-color: rgb(%1);}")
    						.arg(COLOUR_RGB_STRING_YEROTH_ORANGE_243_162_0));
    qDebug() << "++ stylesheet: " << stylesheet;*/

    QToolBar *aCallingWindowToolbar = aCallingWindow->getQMainWindowToolBar();

    aCallingWindowToolbar->setStyleSheet(aCallingWindow->qMessageBoxStyleSheet());

    return QMessageBox::warning(aCallingWindowToolbar,
                                QString("%1 - %2").arg(aCallingWindow->getWindowName(), transLatedPurposeStr),
                                translatedMsg);
}

