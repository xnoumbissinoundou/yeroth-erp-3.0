/*
 * yeroth-erp-push-button.hpp
 *
 *      Author: Dipl.-Inf. XAVIER NOUMBISSI NOUNDOU
 */

#ifndef SRC_WIDGETS_YEROTH_PUSH_BUTTON_HPP_
#define SRC_WIDGETS_YEROTH_PUSH_BUTTON_HPP_

#include "src/include/yeroth-erp-3-0-software.text-configuration.hpp"

#include <QtWidgets/QPushButton>

class YerothPushButton : public QPushButton
{
public:

	YEROTH_CLASS_OPERATORS

	inline YerothPushButton(QWidget *w)
	:QPushButton(w)
	{
	}

	inline YerothPushButton(const QString &s, QWidget *w)
	:QPushButton(s, w)
	{
	}

	inline YerothPushButton(const QIcon &i, const QString &s, QWidget *w)
	:QPushButton(i, s, w)
	{
	}

	inline ~YerothPushButton(){}

	void enable(const QObject *receiver, const char *f);

	void disable(const QObject *receiver, bool buttonVisible = false);

	inline void emptyEnable()
	{
		this->setEnabled(true);
	}
};


#endif /* SRC_WIDGETS_YEROTH_PUSH_BUTTON_HPP_ */
