#include <miktex/Core/Core>

#include "luatex.h"

using namespace MiKTeX::Core;

int
miktex_luatex_renew_format_file(/*[in]*/ const char * lpszName)
{
  PathName path;
  return (SessionWrapper(true)->FindFile(lpszName, FileType::FMT, Session::FindFileFlags::Renew, path) ? 1 : 0);
}
