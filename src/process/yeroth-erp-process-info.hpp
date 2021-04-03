/*
 * yeroth-erp-process-info.hpp
 *
 *      Author: PR. XAVIER NOUMBISSI NOUNDOU
 */

#ifndef SRC_UTILS_YEROTH_PROCESS_INFO_HPP_
#define SRC_UTILS_YEROTH_PROCESS_INFO_HPP_

#include "src/include/yeroth-erp-3-0-software.text-configuration.hpp"


#include "src/process/yeroth-erp-process.hpp"


#include <QtCore/QString>


class YerothUtils;
class YerothLogger;

class YerothProcessInfo
{
public:

	YEROTH_CLASS_OPERATORS

	inline YerothProcessInfo()
	{
	}

	inline ~YerothProcessInfo()
	{
	}

	static bool checkYerothERPAlertDaemonRunning(YerothLogger *logger = 0);
};


#endif /* SRC_UTILS_YEROTH_PROCESS_INFO_HPP_ */
