/*
 * yeroth-erp-progress-bar.hpp
 *
 *      Author: Dipl.-Inf. XAVIER NOUMBISSI NOUNDOU
 */

#ifndef SRC_WIDGETS_YEROTH_ERP_PROGRESS_BAR_HPP_
#define SRC_WIDGETS_YEROTH_ERP_PROGRESS_BAR_HPP_

#include "src/utils/yeroth-erp-utils.hpp"


#include <QtWidgets/QMainWindow>

#include <QtCore/qmath.h>

#include <QtWidgets/QProgressBar>


class YerothProgressBar : public QProgressBar
{
	Q_OBJECT

public:

	YEROTH_CLASS_OPERATORS

	YerothProgressBar(QMainWindow *aParentWindow);

	inline virtual ~YerothProgressBar()
	{
	}

	template <class classType, typename returnType>
	inline void operator()(classType *erpStockImport,
						   returnType (classType::*func)(),
						   returnType *aRetValue,
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

	template <class classType, typename returnType>
	void call_funtion_with_progress_bar_updates(classType *erpStockImport,
											    returnType (classType::*func)(),
												returnType *aRetValue,
												unsigned int progressBarMaximum);

private:

	bool _setuped;
};


/*
 * MUST STAY IN HEADER FILE !
 */
template <class classType, typename returnType>
void YerothProgressBar::call_funtion_with_progress_bar_updates(classType *aClassInstanceRef,
														       returnType (classType::*func)(),
															   returnType *aRetValue,
															   unsigned int progressBarMaximum)
{
	if (!_setuped)
	{
		connect(aClassInstanceRef,
				SIGNAL(SIGNAL_INCREMENT_PROGRESS_BAR(int)),
				this,
				SLOT(SLOT_UPDATE_PROGRESS_BAR_VALUE(int)));

		_setuped = true;
	}

	setMaximum(progressBarMaximum);

	setVisible(true);

    returnType retValue = (aClassInstanceRef->*func)();

	setVisible(false);

	*aRetValue = retValue;
}


#endif /* SRC_WIDGETS_YEROTH_ERP_PROGRESS_BAR_HPP_ */
