/*
 * yeroth-erp-select-db-qcheckbox.hpp
 *
 *      Author: Xavier NOUMBISSI NOUNDOU, Dipl.-Inf., Ph.D. (ABD)
 */

#ifndef SRC_WIDGETS_YEROTH_SELECT_DB_QCHECKBOX_HPP_
#define SRC_WIDGETS_YEROTH_SELECT_DB_QCHECKBOX_HPP_


#include "src/utils/yeroth-erp-utils.hpp"


#include <QtWidgets/QCheckBox>


class YerothSelectDBQCheckBox : public QCheckBox
{
	Q_OBJECT

public:

	YEROTH_CLASS_OPERATORS

	YerothSelectDBQCheckBox(QWidget 		*parent,
							QStringList		*visibleDBFieldColumnStrList);

	virtual ~YerothSelectDBQCheckBox(){}


public slots:

	void handle_visible_db_field_checkBox(bool clicked);


private:

	QStringList			*_visibleDBFieldColumnStrList;

};

#endif /* SRC_WIDGETS_YEROTH_SELECT_DB_QCHECKBOX_HPP_ */