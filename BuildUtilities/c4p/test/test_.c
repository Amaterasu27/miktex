/* generated from test.p by C4P version 2.8.3270 */
#define C4PEXTERN

#include "test.h"

#ifdef _MSC_VER
#pragma warning ( disable : 4102 4390)
#endif


void __cdecl int main (int argc, const char * * argv)

{
  C4P_BEGIN_PROGRAM("test", argc, argv);
  C4P_WRITELN_BEGIN () c4p_write_f  (output, output); c4p_write_s  ("hello, world!", output); C4P_WRITELN_END (output)
  ;
  C4P_END_PROGRAM();
}
