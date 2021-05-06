/*
 * yeroth-erp-qstring-list.hpp
 *
 *      Author: PR. XAVIER NOUMBISSI NOUNDOU
 */

#ifndef SRC_WIDGETS_YEROTH_QSTRING_LIST_HPP_
#define SRC_WIDGETS_YEROTH_QSTRING_LIST_HPP_


#include "src/include/yeroth-erp-3-0-definition-oo-class-operators.hpp"


#include <QtCore/QStringList>


class YerothQStringList : public QStringList
{

public:

	YEROTH_CLASS_OPERATORS

	inline YerothQStringList(const QString &str)
	:QStringList(str)
	{
	}

	inline YerothQStringList(const QList<QString> &other)
	:QStringList(other)
	{
	}

	inline YerothQStringList(QList<QString> &&other)
	:QStringList(other)
	{
	}

	inline YerothQStringList(std::initializer_list<QString> args)
	:QStringList(args)
	{
	}

	inline YerothQStringList()
	:QStringList()
	{
	}

	inline virtual ~YerothQStringList()
	{
	}

	void remove_list(QStringList &a_list);
};


#endif /* SRC_WIDGETS_YEROTH_QSTRING_LIST_HPP_ */
