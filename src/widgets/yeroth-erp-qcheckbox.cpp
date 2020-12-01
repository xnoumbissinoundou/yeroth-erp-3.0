/*
 * yeroth-erp-qcheckbox.cpp
 *
 *      Author: DR. XAVIER NOUMBISSI NOUNDOU
 */


#include "yeroth-erp-qcheckbox.hpp"


#include <QtCore/QDebug>


YerothERPQCheckBox::YerothERPQCheckBox(QWidget *parent)
:QCheckBox(parent)
{
}


void YerothERPQCheckBox::setReadOnly(bool isReadOnly)
{
	if (isReadOnly)
	{
		setAttribute(Qt::WA_TransparentForMouseEvents, true);
		setFocusPolicy(Qt::NoFocus);
	}
	else
	{
		setAttribute(Qt::WA_TransparentForMouseEvents, false);
		setFocusPolicy(Qt::StrongFocus);
	}
}
