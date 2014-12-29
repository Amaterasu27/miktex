#define MIKTEX_UTF8_WRAP_PUTC 1
#include <miktex/utf8wrap.h>

int wmain()
{
  const char * test = "ö\n";
  for (const char * lpsz = test; *lpsz != 0; ++ lpsz)
  {
    putc(*lpsz, stdout);
  }
  puts(test);
  return 0;
}
