/* miktex/Core/test.h: test framework				-*- C++ -*-

   Copyright (C) 1996-2009 Christian Schenk

   This file is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published
   by the Free Software Foundation; either version 2, or (at your
   option) any later version.

   This file is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this file; if not, write to the Free Software
   Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307,
   USA. */

#if defined(_MSC_VER)
#  pragma once
#endif

#if ! defined(F3C19AE9_7A4B_49AC_9E96_B6A8293DC1A7)
#define F3C19AE9_7A4B_49AC_9E96_B6A8293DC1A7

#include <cstdio>
#include <string>
#include <vector>

#if defined(_MSC_VER)
#  include <direct.h>
#  include <io.h>
#  include <fcntl.h>
#  include <process.h>
#endif

#include <miktex/Core/Text>
#include <miktex/Core/Core>
#include <miktex/Core/Paths>
#include <popt-miktex.h>

#define MIKTEXTEST_BEGIN_NAMESPACE		\
  namespace MiKTeX {				\
    namespace Test {

#define MIKTEXTEST_END_NAMESPACE		\
    }						\
  }

MIKTEXTEST_BEGIN_NAMESPACE

#define T_(x) MIKTEXTEXT(x)

#define FPrintF fprintf
#define PrintF printf

#if defined(MIKTEX_WINDOWS)
const char PATH_DELIMITER = ';';
#define PATH_DELIMITER_STRING ";"
#else
const char PATH_DELIMITER = ':';
#define PATH_DELIMITER_STRING ":"
#endif

class OptionEnum
{
public:
  enum EnumType {
    Verbose = 1000,
    RootDirectories,
    InstallRoot,
    DataRoot,
    Trace,
  };
};

typedef MiKTeX::Core::EnumWrapper<OptionEnum> Option;

/* _________________________________________________________________________

   FATAL
   _________________________________________________________________________ */

#define FATAL()						\
  FPrintF (stderr,					\
           T_("fatal error in %s, line %d\n"),		\
	   __FILE__,					\
	   __LINE__);					\
  throw (1)

/* _________________________________________________________________________

   PRINT
   _________________________________________________________________________ */

#define PRINT ! verbose ? 0 : PrintF

/* _________________________________________________________________________

   PRINT
   _________________________________________________________________________ */

#define PRINT_FAILURE(f)						\
  PrintF (T_("%s:%d: TEST FAILED\n"), f.strFile.c_str(), f.iLine);
  
/* _________________________________________________________________________

   TestScript
   _________________________________________________________________________ */

class TestScript
{
public:
  TestScript ()
    : verbose (false)
  {
#if defined(TRACEFLAGS)
    traceFlags = TRACEFLAGS;
#endif
  }

public:
  virtual
  ~TestScript ()
  {
  }

public:
  int
  Main (/*[in]*/ int argc,
	/*[in]*/ const char **argv)
  {
#if defined(_MSC_VER)
    _setmode (_fileno(stdin), _O_BINARY);
    _setmode (_fileno(stdout), _O_BINARY);
    _setmode (_fileno(stderr), _O_BINARY);
#endif
    int rc = 0;
    int chOption;
    Cpopt popt (argc, argv, optionTable);
    while ((chOption = popt.GetNextOpt()) >= 0)
      {
	switch (chOption)
	  {
  	  case Option::InstallRoot:
	    installRoot = popt.GetOptArg();
	    break;
  	  case Option::DataRoot:
	    dataRoot = popt.GetOptArg();
	    break;
  	  case Option::RootDirectories:
	    rootDirectories = popt.GetOptArg();
	    break;
	  case Option::Verbose:
	    verbose = true;
	    break;
	  case Option::Trace:
	    traceFlags = popt.GetOptArg();
	    break;
          default:
            ;
	  }
      }
    const char ** leftovers = popt.GetArgs();
    if (leftovers != 0)
      {
	for (; *leftovers != 0; ++ leftovers)
	  {
	    vecArgs.push_back (*leftovers);
	  }
      }
    PRINT (T_("initializing...\n"));
    try
      {
	MiKTeX::Core::Session::InitInfo initInfo (argv[0]);
	MiKTeX::Core::StartupConfig startupConfig;

#if defined(INSTALLROOT) && defined(DATAROOT)
	startupConfig.userDataRoot = DATAROOT;
	startupConfig.userInstallRoot = INSTALLROOT;
	initInfo.SetStartupConfig (startupConfig);
#else
	startupConfig.userRoots = rootDirectories;
	startupConfig.userDataRoot = dataRoot;
	startupConfig.userInstallRoot = installRoot;
	initInfo.SetStartupConfig (startupConfig);
#endif

#if defined(INITFLAGS)
	initInfo.SetFlags (INITFLAGS);
#endif

#if defined(BINDIR)
#  error BINDIR is deprecated
#endif

	pSession.CreateSession (initInfo);

	PRINT (T_("Starting tests...\n"));
	if (traceFlags.length() > 0)
	  {
	    MiKTeX::Core::TraceStream::SetTraceFlags (traceFlags.c_str());
	  }
	Run ();
	pSession.Reset ();
      }
    catch (const FAILURE & f)
      {
	PRINT_FAILURE (f);
	rc = 1;
      }
    catch (const MiKTeX::Core::MiKTeXException & e)
      {
	MiKTeX::Core::Utils::PrintException (e);
#if 0
	PrintF (T_("%s, %d\n"), e.GetSourceFile().c_str(), e.GetSourceLine());
#endif
	rc = 1;
      }
    catch (int)
      {
	FPrintF (stderr, T_("check test functions\n"));
	rc = 1;
      }
    return (rc);
  }

protected:
  struct FAILURE
  {
    int iNum;
    std::string strFile;
    int iLine;
    FAILURE (/*[in]*/ int i)
      : iNum (i)
    {
    }
  };
  
protected:
  virtual
  void
  Run ()
    = 0;

protected:
  bool
  FindFile (/*[in]*/ const char *	lpszFileName,
	    /*[in]*/ const char *	lpszPathList)
  {
    MiKTeX::Core::PathName path;
    return (pSession->FindFile(lpszFileName, lpszPathList, path));
  }

protected:
  int
  RunSystemCommand (/*[in]*/ const char * lpszProgramName)
  {
    MiKTeX::Core::PathName binDir = pSession->GetMyLocation();
    return (system(MiKTeX::Core::PathName(binDir, lpszProgramName).Get()));
  }

protected:
  void
  Touch (/*[in]*/ const char * lpszPath)
  {
    FILE * pFile;
#if defined(_MSC_VER) && _MSC_VER >= 1400
    if (fopen_s(&pFile, lpszPath, "wb") != 0)
      {
	pFile = 0;
      }
#else
    pFile = fopen(lpszPath, "wb");
#endif
    if (pFile == 0)
      {
	FATAL ();
      }
    fclose (pFile);
  }

protected:
  MiKTeX::Core::SessionWrapper pSession;

protected:
  std::vector<std::string> vecArgs;

protected:
  bool verbose;

protected:
  std::string traceFlags;

protected:
  std::string rootDirectories;

protected:
  std::string installRoot;

protected:
  std::string dataRoot;

private:
  static
  const struct poptOption
  optionTable[];
};

/* _________________________________________________________________________

   TestScript::optionTable
   _________________________________________________________________________ */

const struct poptOption TestScript::optionTable[] = {
  { "install-root", 0, POPT_ARG_STRING, 0, Option::InstallRoot,
    T_("Set the install root directory."), 0},
  { "data-root", 0, POPT_ARG_STRING, 0, Option::DataRoot,
    T_("Set the data root directory."), 0},
  { "root-directories", 0, POPT_ARG_STRING, 0, Option::RootDirectories,
    T_("Set the root directories."), 0},
  { "trace", 0, POPT_ARG_STRING, 0, Option::Trace,
    T_("Set trace flags."), 0},
  { "verbose", 0, POPT_ARG_NONE, 0, Option::Verbose,
    T_("Print information about what is being done."), 0},
  POPT_TABLEEND
};

/* _________________________________________________________________________

   ProcessOutput
   _________________________________________________________________________ */

class
ProcessOutput : public MiKTeX::Core::IRunProcessCallback
{
public:
  virtual
  bool
  MIKTEXTHISCALL
  OnProcessOutput (/*[in]*/ const void *	ptr,
		   /*[in]*/ size_t		n)
  {
    output.append (reinterpret_cast<const char *>(ptr), n);
    return (true);
  }
private:
  std::string output;
public:
  std::string
  GetOutput ()
    const
  {
    return (output);
  }
};


/* _________________________________________________________________________

   FAIL_
   _________________________________________________________________________ */

#define FAIL() f.iLine = __LINE__; f.strFile = __FILE__; throw (f)

/* _________________________________________________________________________

   TEST
   _________________________________________________________________________ */

#define TEST(exp)				\
  PRINT (T_("Test: %s\n"), #exp);		\
  if (! (exp)) { FAIL(); }

/* _________________________________________________________________________

   TESTX
   _________________________________________________________________________ */

#define TESTX(exp)				\
  PRINT (T_("Test: %s\n"), #exp);		\
  try						\
    {						\
      exp;					\
    }						\
  catch (const MiKTeX::Core::MiKTeXException &)	\
    {						\
      FAIL();					\
    }

/* _________________________________________________________________________

   BEGIN_TEST_FUNCTION
   _________________________________________________________________________ */

#define BEGIN_TEST_FUNCTION(num)		\
  void						\
  TestFunc##num ()				\
  {						\
    FAILURE f (num);				\
    PRINT (T_("Testfunc %d\n"), num);

/* _________________________________________________________________________

   END_TEST_FUNCTION
   _________________________________________________________________________ */

#define END_TEST_FUNCTION() \
  }

/* _________________________________________________________________________

   CALL_TEST_FUNCTION
   _________________________________________________________________________ */

#define CALL_TEST_FUNCTION(num) TestFunc##num ()

/* _________________________________________________________________________

   BEGIN_TEST_SCRIPT
   _________________________________________________________________________ */

#define BEGIN_TEST_SCRIPT()			\
  class MyTestScript : public TestScript	\
  {						\
  protected:
  
/* _________________________________________________________________________

   END_TEST_SCRIPT
   _________________________________________________________________________ */

#define END_TEST_SCRIPT()			\
  };

/* _________________________________________________________________________

   BEGIN_TEST_PROGRAM
   _________________________________________________________________________ */

#define BEGIN_TEST_PROGRAM()			\
protected:					\
virtual						\
void						\
Run ()						\
{

/* _________________________________________________________________________

   END_TEST_PROGRAM
   _________________________________________________________________________ */

#define END_TEST_PROGRAM()			\
}

/* _________________________________________________________________________

   RUN_TEST_SCRIPT
   _________________________________________________________________________ */

#define RUN_TEST_SCRIPT()			\
int						\
main (/*[in]*/ int argc,			\
      /*[in]*/ const char **argv)		\
{						\
  MyTestScript script;				\
  return (script.Main(argc, argv));		\
}
 
/* -- */

MIKTEXTEST_END_NAMESPACE;

using namespace MiKTeX::Core;
using namespace MiKTeX::Test;
using namespace std;

#endif
