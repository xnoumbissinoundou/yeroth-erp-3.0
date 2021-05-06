/*
 * yeroth-erp-select-db-qcheckbox.cpp
 *
 *      Author: PR. XAVIER NOUMBISSI NOUNDOU
 */


#include "yeroth-erp-select-db-qcheckbox.hpp"


#include <QtCore/QDebug>


YerothSelectDBQCheckBox::YerothSelectDBQCheckBox(QWidget 		*parent,
												 YerothQStringList	*visibleDBFieldColumnStrList)
:YerothERPQCheckBox(parent),
 _visibleDBFieldColumnStrList(visibleDBFieldColumnStrList)
{
}


void YerothSelectDBQCheckBox::handle_visible_db_field_checkBox(bool checkBoxClicked)
{
	if (0 != _visibleDBFieldColumnStrList)
	{
		if (checkBoxClicked)
		{
			if (!_visibleDBFieldColumnStrList->contains(objectName()))
			{
				_visibleDBFieldColumnStrList->append(objectName());
			}
		}
		else
		{
			_visibleDBFieldColumnStrList->removeAll(objectName());
		}
	}
}
