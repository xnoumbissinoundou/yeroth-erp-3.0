/*
 * yeroth-erp-number-word-translation.cpp
 *
 *      Author: DIPL.-INF. XAVIER NOUMBISSI NOUNDOU
 */

#include "yeroth-erp-number-word-translation.hpp"


#include <QtCore/QDebug>


#include <cstring>
#include <cstdlib>


char *YerothERPNumberWordTranslation::single_digits[] =
{
		"zero",
		"one",
		"two",
		"three",
		"four",
		"five",
		"six",
		"seven",
		"eight",
		"nine"
};


char *YerothERPNumberWordTranslation::two_digits[] =
{
		"",
		"ten",
		"eleven",
		"twelve",
		"thirteen",
		"fourteen",
		"fifteen",
		"sixteen",
		"seventeen",
		"eighteen",
		"nineteen"
};


char *YerothERPNumberWordTranslation::tens_multiple[] =
{
		"",
		"",
		"twenty",
		"thirty",
		"forty",
		"fifty",
		"sixty",
		"seventy",
		"eighty",
		"ninety"
};


char *YerothERPNumberWordTranslation::tens_power[] =
{
		"hundred",
		"thousand"
};


void YerothERPNumberWordTranslation::yeroth_CONVERT_NUMBER_TO_WORDS(char *num)
{
	int len = strlen(num);

	if (len == 0)
	{
		qDebug() << "empty string";
		return;
	}

	if (len > 4)
	{
		qDebug() << "Length more than 4 is not supported";
		return;
	}

	qDebug() << QString("%1: ").arg(num);

	if (len == 1)
	{
		qDebug() << QString("%1 : ").arg(single_digits[*num - '0']);
		return;
	}

	while (*num != '\0')
	{
		if (len >= 3)
		{
			if (*num -'0' != 0)
			{
				qDebug() << QString("%1 %2 ")
								.arg(single_digits[*num - '0'],
									 tens_power[len-3]);
			}
			--len;
		}
		else
		{
			if (*num == '1')
			{
				int sum = *num - '0' + *(num + 1)- '0';

				qDebug() << QString("%1 ").arg(two_digits[sum]);

				return;
			}
			else if (*num == '2' && *(num + 1) == '0')
			{
				qDebug() << QString("twenty ");
				return;
			}
			else
			{
				int i = *num - '0';

				qDebug() << QString("%1 ").arg(i ? tens_multiple[i]: "");

				++num;

				if (*num != '0')
				{
					qDebug() << QString("%1 ").arg(single_digits[*num - '0']);
				}
			}
		}
		++num;
	}
}

