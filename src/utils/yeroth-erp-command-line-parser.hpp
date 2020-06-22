/*
 * yeroth-erp-command-line-parser.hpp
 *
 *      Author: Dipl.-Inf. XAVIER NOUMBISSI NOUNDOU
 */

#ifndef SRC_YEROTH_COMMAND_LINE_PARSER_HPP_
#define SRC_YEROTH_COMMAND_LINE_PARSER_HPP_

#include "src/include/yeroth-erp-3-0-software.text-configuration.hpp"

#include <QtCore/QFile>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QTextStream>

enum YerothCommandLineArgumentType
{
	HELP		= 0,
	GUI			= 1,
	VERSION		= 2,
	UNDEFINED	= 3
};

class YerothCommandLineParser
{
public:

	YEROTH_CLASS_OPERATORS

	inline YerothCommandLineParser()
	{
	}

	inline ~YerothCommandLineParser()
	{
	}

	enum YerothCommandLineArgumentType parseProgramArguments(int argc, char *argv[]);
};

#endif /* SRC_YEROTH_COMMAND_LINE_PARSER_HPP_ */
