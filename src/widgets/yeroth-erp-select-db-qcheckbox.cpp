/*
 * yeroth-erp-select-db-qcheckbox.cpp
 *
 *      Author: PR. XAVIER NOUMBISSI NOUNDOU
 */


#include "yeroth-erp-select-db-qcheckbox.hpp"


#include <QtCore/QDebug>


YerothSelectDBQCheckBox::YerothSelectDBQCheckBox(QWidget 		*parent,
												 QStringList	*visibleDBFieldColumnStrList)
:YerothERPQCheckBox(parent),
 _visibleDBFieldColumnStrList(visibleDBFieldColumnStrList)
{
}


void YerothSelectDBQCheckBox::handle_visible_db_field_checkBox(bool checkBoxClicked)
{
	if (0 != _visibleDBFieldColumnStrList)
	{
		QString yeroth_qt_THIS_object_name = YEROTH_QT_OBJECT_NAME(this);

		if (checkBoxClicked)
		{
			if (!_visibleDBFieldColumnStrList->contains(yeroth_qt_THIS_object_name))
			{
				_visibleDBFieldColumnStrList->append(yeroth_qt_THIS_object_name);
			}
		}
		else
		{
			_visibleDBFieldColumnStrList->removeAll(yeroth_qt_THIS_object_name);
		}
	}
}
