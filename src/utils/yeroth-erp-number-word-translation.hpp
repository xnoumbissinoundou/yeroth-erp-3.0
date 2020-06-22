/*
 * yeroth-erp-number-word-translation.hpp
 *
 *      Author: Dipl.-Inf. XAVIER NOUMBISSI NOUNDOU
 */

#ifndef SRC_YEROTH_ERP_NUMBER_WORD_TRANSLATION_HPP_
#define SRC_YEROTH_ERP_NUMBER_WORD_TRANSLATION_HPP_


#include "src/include/yeroth-erp-3-0-software.text-configuration.hpp"


#include <QtCore/QObject>


class YerothERPNumberWordTranslation : public QObject
{
	Q_OBJECT

public:

	YEROTH_CLASS_OPERATORS

	virtual void yeroth_CONVERT_NUMBER_TO_WORDS(char *num);

protected:

	static char *single_digits[];

	static char *two_digits[];

	static char *tens_multiple[];

	static char *tens_power[];
};


#endif /* SRC_YEROTH_ERP_NUMBER_WORD_TRANSLATION_HPP_ */
