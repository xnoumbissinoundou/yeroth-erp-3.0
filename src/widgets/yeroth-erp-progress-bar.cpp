/*
 * yeroth-erp-progress-bar.cpp
 *
 *      Author: Dipl.-Inf. XAVIER NOUMBISSI NOUNDOU
 */

#include "yeroth-erp-progress-bar.hpp"


YerothProgressBar::YerothProgressBar(QMainWindow *aParentWindow)
:QProgressBar(aParentWindow),
 _setuped(false)
{
	setVisible(false);

	setStyleSheet(QString("QProgressBar "
			"{text-align: center; "
			"border: 2px solid white; "
			"border-radius: 1px;}"
			"QProgressBar::chunk {"
			"background-color: rgb(%1); "
			"color: rgb(%2); "
			"width: 3px;}")
			.arg(COLOUR_RGB_STRING_YEROTH_FIREBRICK_RED_255_48_48,
				 COLOUR_RGB_STRING_YEROTH_DARK_GREEN_47_67_67));

	int width = qCeil( ((QMainWindow *)parent())->width() / 2.0 );

	int X = qCeil(width / 2.0);

	int Y = qCeil( ((QMainWindow *)parent())->height() / 2.0 );

	setFixedWidth(width);

	move(X, Y);
}
