/*
 * yeroth-erp-ui-loader.hpp
 *
 *  Created on: August 28, 2017
 *      Author: Dipl.-Inf. Xavier NOUMBISSI NOUNDOU, Ph.D. (ABD)
 *      Email:  xnoundou7@gmail.com
 */

#ifndef SRC_YEROTH_UI_LOADER_HPP_
#define SRC_YEROTH_UI_LOADER_HPP_

#include <QtUiTools/QUiLoader>

class YerothUiLoader : public QUiLoader
{

public:

	inline YerothUiLoader(QObject *parent = 0)
	{
	}

	inline virtual ~YerothUiLoader()
	{
	}

    virtual QWidget *createWidget(const QString &className,
    							  QWidget *parent = 0,
    							  const QString & name = QString());

};

#endif /* SRC_YEROTH_UI_LOADER_HPP_ */
