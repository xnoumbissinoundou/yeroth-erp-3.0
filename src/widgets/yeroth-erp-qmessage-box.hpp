/*
 * yeroth-erp-qmessage-box.hpp
 *
 *      Author: Xavier NOUMBISSI NOUNDOU, Dipl.-Inf., Ph.D. (ABD)
 */

#ifndef YEROTH_ERP_QMESSAGE_BOX_HPP_
#define YEROTH_ERP_QMESSAGE_BOX_HPP_

#include "src/include/yeroth-erp-3-0-software.text-configuration.hpp"

#include <QtWidgets/QMessageBox>

class QToolBar;
class YerothPOSAdminWindowsCommons;
class YerothWindowsCommons;

class YerothQMessageBox : public QMessageBox
{

public:

	YEROTH_CLASS_OPERATORS

	inline YerothQMessageBox(QWidget * parent = 0)
	:QMessageBox(parent)
	{
	}

	inline ~YerothQMessageBox(){}

	static void about(YerothPOSAdminWindowsCommons *aCallingWindow,
	        		  QString transLatedPurposeStr,
					  QString translatedMsg);

	static QMessageBox::StandardButton information(YerothPOSAdminWindowsCommons *aCallingWindow,
									  	  	  	   QString transLatedPurposeStr,
												   QString translatedMsg,
												   StandardButtons buttons = Ok,
												   StandardButton defaultButton = NoButton);

	static QMessageBox::StandardButton question(YerothPOSAdminWindowsCommons *aCallingWindow,
	        									QString transLatedPurposeStr,
												QString translatedMsg,
												StandardButtons buttons = Ok,
												StandardButton defaultButton = NoButton);

	static QMessageBox::StandardButton warning(YerothPOSAdminWindowsCommons *aCallingWindow,
	        								   QString transLatedPurposeStr,
											   QString translatedMsg,
											   StandardButton defaultButton = Ok);


	static void about(YerothWindowsCommons *aCallingWindow,
									  	  	  	   QString transLatedPurposeStr,
												   QString translatedMsg);

	static QMessageBox::StandardButton information(YerothWindowsCommons *aCallingWindow,
									  	  	  	   QString transLatedPurposeStr,
												   QString translatedMsg,
												   StandardButtons buttons = Ok,
												   StandardButton defaultButton = NoButton);

	static QMessageBox::StandardButton question(YerothWindowsCommons *aCallingWindow,
									  	  	  	   QString transLatedPurposeStr,
												   QString translatedMsg,
												   StandardButtons buttons = Ok,
												   StandardButton defaultButton = NoButton);

	static QMessageBox::StandardButton warning(YerothWindowsCommons *aCallingWindow,
									  	  	  	   QString transLatedPurposeStr,
												   QString translatedMsg);
};


#endif /* YEROTH_ERP_QMESSAGE_BOX_HPP_ */
