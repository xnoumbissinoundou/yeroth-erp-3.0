/*
 * yeroth-erp-qcheckbox.hpp
 *
 *      Author: Dipl.-Inf. XAVIER NOUMBISSI NOUNDOU
 */

#ifndef SRC_WIDGETS_YEROTH_QCHECKBOX_HPP_
#define SRC_WIDGETS_YEROTH_QCHECKBOX_HPP_


#include "src/utils/yeroth-erp-utils.hpp"


#include <QtWidgets/QCheckBox>


class YerothERPQCheckBox : public QCheckBox
{
	Q_OBJECT

public:

	YEROTH_CLASS_OPERATORS

	YerothERPQCheckBox(QWidget *parent);

	virtual ~YerothERPQCheckBox(){}

	void setReadOnly(bool isReadOnly);

};

#endif /* SRC_WIDGETS_YEROTH_QCHECKBOX_HPP_ */
