/*
 * yeroth-erp-process-info.hpp
 *
 *      Author: Dipl.-Inf. Xavier NOUMBISSI NOUNDOU, Ph.D. (ABD)
 *      Email:  xnoundou7@gmail.com
 */

#ifndef SRC_UTILS_YEROTH_PROCESS_INFO_HPP_
#define SRC_UTILS_YEROTH_PROCESS_INFO_HPP_

#include "src/include/yeroth-erp-3-0-software.text-configuration.hpp"

#include <QtCore/QString>

#include "src/utils/yeroth-erp-utils.hpp"

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
