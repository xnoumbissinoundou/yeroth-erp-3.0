/*
 * yeroth-erp-qstring-list.cpp
 *
 *      Author: PR. XAVIER NOUMBISSI NOUNDOU
 */


#include "src/widgets/yeroth-erp-qstring-list.hpp"


void YerothQStringList::remove_list(QStringList &a_list)
{
	for (uint i = 0; i < a_list.size(); ++i)
	{
		removeOne(a_list.at(i));
	}
}
