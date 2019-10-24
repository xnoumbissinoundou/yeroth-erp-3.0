#ifndef YEROTH_ERP_KEY_EMITTER_HPP
#define YEROTH_ERP_KEY_EMITTER_HPP

#include "src/include/yeroth-erp-3-0-software.text-configuration.hpp"

#include <QtCore/QObject>

class YerothPOSKeyEmitter : public QObject
{
	Q_OBJECT

public:

	inline YerothPOSKeyEmitter() {}

	inline ~YerothPOSKeyEmitter() {}

public slots:

	void emitKey(Qt::Key key);
};

#endif //YEROTH_ERP_KEY_EMITTER_HPP
