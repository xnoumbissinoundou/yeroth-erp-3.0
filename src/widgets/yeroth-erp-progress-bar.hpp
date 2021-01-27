/*
 * yeroth-erp-progress-bar.hpp
 *
 *      Author: DR. XAVIER NOUMBISSI NOUNDOU
 */

#ifndef SRC_WIDGETS_YEROTH_ERP_PROGRESS_BAR_HPP_
#define SRC_WIDGETS_YEROTH_ERP_PROGRESS_BAR_HPP_

#include "src/utils/yeroth-erp-utils.hpp"

#include <QtCore/qmath.h>

#include <QtWidgets/QProgressBar>


class YerothWindowsCommons;


class YerothProgressBar : public QProgressBar
{
	Q_OBJECT

public:

	YEROTH_CLASS_OPERATORS

	YerothProgressBar(YerothPOSAdminWindowsCommons *aYerothParentAdminWindow);

	YerothProgressBar(YerothWindowsCommons *aYerothParentWindow);

	inline virtual ~YerothProgressBar()
	{
	}

	template <class classType, typename parameterType, typename returnType>
	inline void operator()(classType *erpStockImport,
						   parameterType *aParamValue,
						   returnType (classType::*func)(parameterType *),
						   returnType *aRetValue = 0,
						   unsigned int progressBarMaximum = 100)
	{
		call_funtion_with_progress_bar_updates(erpStockImport,
											   aParamValue,
											   func,
											   aRetValue,
											   progressBarMaximum);
	}

	template <class classType, typename returnType>
	inline void operator()(classType *erpStockImport,
						   returnType (classType::*func)(),
						   returnType *aRetValue = 0,
						   unsigned int progressBarMaximum = 100)
	{
		call_funtion_with_progress_bar_updates(erpStockImport,
											   func,
											   aRetValue,
											   progressBarMaximum);
	}

public slots:

	inline void SLOT_UPDATE_PROGRESS_BAR_VALUE(int v = 1)
	{
		setValue(v + 2);
	}

protected:

	template <class classType, typename parameterType, typename returnType>
	void call_funtion_with_progress_bar_updates(classType *erpStockImport,
												parameterType *aParamValue,
											    returnType (classType::*func)(parameterType *),
												returnType *aRetValue,
												unsigned int progressBarMaximum);

	template <class classType, typename returnType>
	void call_funtion_with_progress_bar_updates(classType *erpStockImport,
											    returnType (classType::*func)(),
												returnType *aRetValue,
												unsigned int progressBarMaximum);

private:

	bool _setup;
};


/*
 * MUST STAY IN HEADER FILE !
 */
template <class classType, typename parameterType, typename returnType>
void YerothProgressBar::call_funtion_with_progress_bar_updates(classType *aClassInstanceRef,
															   parameterType *aParamValue,
														       returnType (classType::*func)(parameterType *),
															   returnType *aRetValue,
															   unsigned int progressBarMaximum)
{
	if (!_setup)
	{
		connect(aClassInstanceRef,
				SIGNAL(SIGNAL_INCREMENT_PROGRESS_BAR(int)),
				this,
				SLOT(SLOT_UPDATE_PROGRESS_BAR_VALUE(int)));

		_setup = true;
	}

	setMaximum(progressBarMaximum);

	setVisible(true);

    returnType retValue = (aClassInstanceRef->*func)(aParamValue);

	setVisible(false);

	if (0 != aRetValue)
	{
		*aRetValue = retValue;
	}
}


/*
 * MUST STAY IN HEADER FILE !
 */
template <class classType, typename returnType>
void YerothProgressBar::call_funtion_with_progress_bar_updates(classType *aClassInstanceRef,
														       returnType (classType::*func)(),
															   returnType *aRetValue,
															   unsigned int progressBarMaximum)
{
	if (!_setup)
	{
		connect(aClassInstanceRef,
				SIGNAL(SIGNAL_INCREMENT_PROGRESS_BAR(int)),
				this,
				SLOT(SLOT_UPDATE_PROGRESS_BAR_VALUE(int)));

		_setup = true;
	}

	setMaximum(progressBarMaximum);

	setVisible(true);

    returnType retValue = (aClassInstanceRef->*func)();

	setVisible(false);

	if (0 != aRetValue)
	{
		*aRetValue = retValue;
	}
}


#endif /* SRC_WIDGETS_YEROTH_ERP_PROGRESS_BAR_HPP_ */
