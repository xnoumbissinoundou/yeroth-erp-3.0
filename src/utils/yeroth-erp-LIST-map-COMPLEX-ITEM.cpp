/*
 * yeroth-erp-LIST-map-COMPLEX-ITEM.cpp
 *
 *      Author: PR. XAVIER NOUMBISSI NOUNDOU
 */

#include "yeroth-erp-LIST-map-COMPLEX-ITEM.hpp"


void Yeroth_LIST_map_COMPLEX_ITEM::clear()
{
    for (int k = 0; k < _my_list_complex_items.size(); ++k)
    {
    	YEROTH_DELETE_FREE_POINTER_NOW(_my_list_complex_items.value(k));
    }

    _my_list_complex_items.clear();
}


const Yeroth_MAP_COMPLEX_Item * Yeroth_LIST_map_COMPLEX_ITEM::find_item(const QString an_item_name)
{
	for(int i = 0; i < _my_list_complex_items.size(); ++i)
	{
		if (YerothUtils::isEqualCaseInsensitive(an_item_name,
												_my_list_complex_items.value(i)->_itemName))
		{
			return _my_list_complex_items.at(i);
		}
	}

	return 0;
}


void Yeroth_LIST_map_COMPLEX_ITEM::insert_item(const QString &an_item_name,
										  	   double aDOUBLE_value)
{
	if (an_item_name.isEmpty())
	{
		return ;
	}

	int idx = -1;

	Yeroth_MAP_COMPLEX_Item *a_item = 0;

	for(int i = 0; i < _my_list_complex_items.size(); ++i)
	{
		a_item = _my_list_complex_items.value(i);

		if (0 != a_item &&
			YerothUtils::isEqualCaseInsensitive(an_item_name, a_item->_itemName))
		{
			idx = i;
			break;
		}
	}

	if (-1 == idx)
	{
		Yeroth_MAP_COMPLEX_Item *aNewItem = new Yeroth_MAP_COMPLEX_Item(an_item_name,
													    				aDOUBLE_value);

		_my_list_complex_items.push_back(aNewItem);
	}
	else
	{
		a_item = _my_list_complex_items.value(idx);

		if (0 != a_item)
		{
			a_item->_itemValue = aDOUBLE_value;
		}
	}
}


void Yeroth_LIST_map_COMPLEX_ITEM::print(const QString &a_label /* = YerothUtils::EMPTY_STRING */)
{
	QString result_print_string;

	Yeroth_MAP_COMPLEX_Item *anItem = 0;

	for (uint i = 0; i < _my_list_complex_items.size(); ++i)
	{
		anItem = _my_list_complex_items.at(i);

		if (0 != anItem)
		{
			result_print_string.append("; ")
							   .append(anItem->toString());
		}
	}

	if (!a_label.isEmpty())
	{
		qDebug() << "++ " << a_label << ": "
				 << QString(" %2").arg(result_print_string);
	}
	else
	{
		qDebug() << result_print_string;
	}
}

