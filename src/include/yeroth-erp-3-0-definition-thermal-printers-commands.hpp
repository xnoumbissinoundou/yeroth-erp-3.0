/*
 * yeroth-erp-3-0-definition-thermal-printers-commands.hpp
 *
 *      Author: Dipl.-Inf. Xavier NOUMBISSI NOUNDOU, Ph.D. (ABD)
 *      E-mail:  xnoundou7@gmail.com
 */

#ifndef YEROTH_ERP_DEFINITION_THERMAL_PRINTERS_COMMANDS_HPP_
#define YEROTH_ERP_DEFINITION_THERMAL_PRINTERS_COMMANDS_HPP_



#define COMMAND_LINE_COMMAND_OPEN_CASH_DRAWER_EPSON_TM_T20II	"echo -e -n \x1b\x70\x00\x19\xfa > %1"


#define COMMAND_LINE_COMMAND_PRINT_EPSON_TM_T20II				"echo | cat %1 >> %2"



#endif /* YEROTH_ERP_DEFINITION_THERMAL_PRINTERS_COMMANDS_HPP_ */
