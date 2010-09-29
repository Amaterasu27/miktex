/* runtexlua.cpp: run a texlua script

   Copyright (C) 2010 Christian Schenk

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
   USA.  */

#include <miktex/App/Application>
#include <miktex/Core/Paths>

using namespace MiKTeX::App;
using namespace MiKTeX::Core;
using namespace std;

#define FATAL_MIKTEX_ERROR(miktexFunction, traceMessage, lpszInfo)	\
  Session::FatalMiKTeXError (miktexFunction,				\
			     traceMessage,				\
			     lpszInfo,					\
			     __FILE__,					\
			     __LINE__)

extern "C"
int
MIKTEXCEECALL
Main (/*[in]*/ int	argc,
      /*[in]*/ char **	argv);

// Keep the application object in the global scope (C functions might
// call exit())
MiKTeX::App::Application app;

#if MTXRUN
#  define CFGKEY "mtxrun"
#else
#  define CFGKEY "texlua"
#endif

/* _________________________________________________________________________

   main
   _________________________________________________________________________ */

int
main (/*[in]*/ int	argc,
      /*[in]*/ char **	argv)
{
  try
    {
      app.Init (argv[0]);

      MIKTEX_ASSERT (argc > 0);

      // determine script name
      char szProgramName[BufferSizes::MaxPath];
      PathName::Split (argv[0],
		       0, 0,
		       szProgramName, BufferSizes::MaxPath,
		       0, 0);

      std::string scriptName;

#if MTXRUN
      bool isLuatools = (PathName::Compare(szProgramName, "luatools") == 0);
      bool isMtxrun = (PathName::Compare(szProgramName, "mtxrun") == 0);
      bool isTexmfstart = (PathName::Compare(szProgramName, "texmfstart") == 0);
      if (isLuatools)
      {
	scriptName = "luatools";
      }
      else
      {
	scriptName = "mtxrun";
      }
#else
      scriptName = szProgramName;
#endif

      // get relative script path
      PathName scriptsIni = app.GetSession()->GetSpecialPath(SpecialPath::DistRoot);
      scriptsIni += MIKTEX_PATH_SCRIPTS_INI;
      SmartPointer<Cfg> pConfig (Cfg::Create());
      pConfig->Read (scriptsIni);
      std::string relScriptPath;
      if (! pConfig->TryGetValue(CFGKEY, scriptName.c_str(), relScriptPath))
      {
	FATAL_MIKTEX_ERROR ("runtexlua",
	  MIKTEXTEXT("The Lua script is not registered."),
	  szProgramName);
      }
      pConfig.Release ();
      
      // find script
      PathName scriptPath;
      if (! app.GetSession()->FindFile(relScriptPath.c_str(), MIKTEX_PATH_TEXMF_PLACEHOLDER, scriptPath))
      {
	FATAL_MIKTEX_ERROR ("runtexlua",
	  MIKTEXTEXT("The Lua script could not be found."),
	  relScriptPath.c_str());
      }
      
      // build new argv
      std::vector<char *> args;
      args.reserve (argc + 2);
      PathName argv0 = argv[0];
      args.push_back (argv0.GetBuffer());
      args.push_back ("--luaonly");
      args.push_back (scriptPath.GetBuffer());
#if MTXRUN
      if (! (isLuatools || isMtxrun || isTexmfstart))
      {
	args.push_back ("--script");
	args.push_back (szProgramName);
      }
#endif
      for (int idx = 1; idx < argc; ++ idx)
	{
	  args.push_back (argv[idx]);
	}
      args.push_back (0);

      // run texlua
      int exitCode = Main(args.size() - 1, &args[0]);

      app.Finalize ();

      return (exitCode);
    }
  catch (const MiKTeXException & e)
    {
      Utils::PrintException (e);
      return (1);
    }
  catch (const std::exception & e)
    {
      Utils::PrintException (e);
      return (1);
    }
  catch (int exitCode)
    {
      return (exitCode);
    }
}
