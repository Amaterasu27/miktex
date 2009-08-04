/* config.cpp: MiKTeX configuration settings

   Copyright (C) 1996-2009 Christian Schenk

   This file is part of the MiKTeX Core Library.

   The MiKTeX Core Library is free software; you can redistribute it
   and/or modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2, or
   (at your option) any later version.
   
   The MiKTeX Core Library is distributed in the hope that it will be
   useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with the MiKTeX Core Library; if not, write to the Free
   Software Foundation, 59 Temple Place - Suite 330, Boston, MA
   02111-1307, USA. */

#include "StdAfx.h"

#include "internal.h"

#include "miktex/Core/Environment"
#include "miktex/Core/Paths"
#include "miktex/Core/Registry"
#include "miktex/Core/Version"

#if defined(MIKTEX_WINDOWS)
#  include "win/winRegistry.h"
#endif

/* _________________________________________________________________________

   ConfigMapping
   _________________________________________________________________________ */

#if 0
struct ConfigMapping
{
  const char *	lpszConfigSection;
  const char *	lpszConfigValueName;
  const char *	lpszEnvVarName;
#if defined(MIKTEX_WINDOWS)
  const char *	lpszRegKey;
  const char *	lpszRegValueName;
#endif
};
#endif

/* _________________________________________________________________________

   configMappings
   _________________________________________________________________________ */

#if 0
namespace {
  const ConfigMapping configMappings[] = {
    {
      MIKTEX_REGKEY_PACKAGE_MANAGER,
      MIKTEX_REGVAL_REMOTE_REPOSITORY,
      MIKTEX_ENV_REPOSITORY,
    },
  };
}
#endif

/* _________________________________________________________________________

   FindConfigMapping
   _________________________________________________________________________ */

#if 0
MIKTEXSTATICFUNC(const ConfigMapping *)
FindConfigMapping (/*[in]*/ const char *	lpszConfigSection,
		   /*[in]*/ const char *	lpszConfigValueName)
{
  for (size_t idx = 0;
       idx < sizeof(configMappings) / sizeof(configMappings);
       ++ idx)
    {
      if ((StringCompare(configMappings[idx].lpszConfigSection,
			 lpszConfigSection,
			 true)
	   == 0)
	  && (StringCompare(configMappings[idx].lpszConfigValueName,
			    lpszConfigValueName,
			    true))
	  == 0)
	{
	  return (&configMappings[idx]);
	}
    }
  return (0);
}
#endif

/* _________________________________________________________________________

   SessionImpl::GetMyPrefix
   _________________________________________________________________________ */

PathName
SessionImpl::GetMyPrefix ()
{
  PathName bindir = GetMyLocation(true);

  RemoveDirectoryDelimiter (bindir.GetBuffer());

  PathName prefix (bindir);

  // /usr/local/bin => /usr/local
  // /usr/bin => /usr
  prefix.CutOffLastComponent ();

  return (prefix);
}

/* _________________________________________________________________________

   SessionImpl::FindStartupConfigFile

   Try to find the MiKTeX startup file in the usual places.
   _________________________________________________________________________ */

bool
SessionImpl::FindStartupConfigFile (/*[in]*/ bool	  common,
				    /*[out]*/ PathName &  path)
{
  if ((initInfo.GetFlags() & InitFlags::NoConfigFiles) != 0)
    {
      return (false);
    }

  string str;

  if (Utils::GetEnvironmentString((common
				   ? MIKTEX_ENV_COMMON_STARTUP_FILE
				   : MIKTEX_ENV_USER_STARTUP_FILE),
				  str))
    {
      path = str;
      // don't check for existence; it's a fatal error (detected later)
      // if the environment value is incorrect
      return (true);
    }

#if ! NO_REGISTRY
  if (winRegistry::TryGetRegistryValue((common
					? TriState::True
					: TriState::False),
				       MIKTEX_REGKEY_CORE,
				       MIKTEX_REGVAL_STARTUP_FILE,
				       str,
				       0))
    {
      // don't check for existence; it's a fatal error (detected later)
      // if the registry value is incorrect
      path = str;
      return (true);
    }
#endif

  StartupConfig defaultStartupConfig = DefaultConfig();

  if (common)
    {
      // try the prefix of the bin directory
      PathName myloc = GetMyLocation(true);
      RemoveDirectoryDelimiter (myloc.GetBuffer());
      PathName internalBindir (MIKTEX_PATH_INTERNAL_BIN_DIR);
      RemoveDirectoryDelimiter (internalBindir.GetBuffer());
      PathName prefix;
      if (Utils::GetPathNamePrefix(myloc, internalBindir, prefix))
	{
	  path = prefix;
	  path += MIKTEX_PATH_STARTUP_CONFIG_FILE;
	  if (File::Exists(path))
	    {
	      return (true);
	    }
	}
      PathName bindir (MIKTEX_PATH_BIN_DIR);
      RemoveDirectoryDelimiter (bindir.GetBuffer());
      if (Utils::GetPathNamePrefix(myloc, bindir, prefix))
	{
	  path = prefix;
	  path += MIKTEX_PATH_STARTUP_CONFIG_FILE;
	  if (File::Exists(path))
	    {
	      return (true);
	    }
	}
      // try /var/lib/miktex-texmf/miktex/config/miktexstartup.ini
      path = defaultStartupConfig.commonConfigRoot;
      path += MIKTEX_PATH_STARTUP_CONFIG_FILE;
      if (File::Exists(path))
	{
	  return (true);
	}
#if MIKTEX_UNIX
      // try /usr/share/miktex-texmf/miktex/config/miktexstartup.ini
      prefix = GetMyPrefix();
      path = prefix;
      path += MIKTEX_TEXMF;
      path += MIKTEX_PATH_STARTUP_CONFIG_FILE;
      if (File::Exists(path))
	{
	  return (true);
	}
#endif
    }
  else
    {
      // try $HOME/.miktex/miktex/config/miktexstartup.ini
      path = defaultStartupConfig.userConfigRoot;
      path += MIKTEX_PATH_STARTUP_CONFIG_FILE;
      if (File::Exists(path))
	{
	  return (true);
	}
    }

  return (false);
}

/* _________________________________________________________________________

   Absolutize
   _________________________________________________________________________ */

void
Absolutize (/*[in,out]*/ string &     paths,
	    /*[in]*/ const PathName & relativeFrom)
{
#if MIKTEX_WINDOWS
  string result;
  for (CSVList path (paths.c_str(), PATH_DELIMITER);
       path.GetCurrent() != 0;
       ++ path)
  {
    if (! result.empty())
    {
      result += PATH_DELIMITER;
    }
    if (Utils::IsAbsolutePath(path.GetCurrent()))
    {
      result += path.GetCurrent();
    }
    else
    {
      MIKTEX_ASSERT (Utils::IsAbsolutePath(relativeFrom.Get()));
      PathName absPath (relativeFrom);
      absPath += path.GetCurrent();
      PathName absPath2;
      MIKTEX_ASSERT (absPath2.GetCapacity() >= MAX_PATH);
      if (! PathCanonicalizeA(absPath2.GetBuffer(), absPath.Get()))
      {
	absPath2 = absPath;
      }
      result += absPath2.Get();
    }
  }
  paths = result;
#else
  UNIMPLEMENTED ("Absolutize");
#endif
}
/* _________________________________________________________________________

   SessionImpl::ReadStartupConfigFile

   Read the contents of the startup config file.
   _________________________________________________________________________ */

StartupConfig
SessionImpl::ReadStartupConfigFile (/*[in]*/ bool common,
				    /*[in]*/ const PathName & path)
{
  StartupConfig ret;

  SmartPointer<Cfg> pcfg (Cfg::Create());

  pcfg->Read (path);

  string str;

  if (pcfg->TryGetValue("Auto", "Config", str))
  {
    if (common)
    {
      ret.commonConfigRoot = "";
      ret.commonDataRoot = "";
      ret.commonInstallRoot = "";
    }
    else
    {
      ret.userConfigRoot = "";
      ret.userDataRoot = "";
      ret.userInstallRoot = "";
    }
    if (str == "Regular")
    {
      ret.config = MiKTeXConfiguration::Regular;
    }
    else if (str == "Portable")
    {
      ret.config = MiKTeXConfiguration::Portable;
    }
    else if (str == "Direct")
    {
      ret.config = MiKTeXConfiguration::Direct;
    }
    else
    {
      FATAL_MIKTEX_ERROR ("SessionImpl::ReadStartupConfigFile",
	T_("Invalid configuration value."),
	str.c_str());
    }
  }

  PathName relativeFrom (path);
  relativeFrom.RemoveFileSpec ();
  
  if (common)
  {      
    if (pcfg->TryGetValue("Paths",
      MIKTEX_REGVAL_COMMON_ROOTS,
      str))
    {
      Absolutize (str, relativeFrom);
      ret.commonRoots = str;
    }
    if (pcfg->TryGetValue("Paths",
      MIKTEX_REGVAL_COMMON_INSTALL,
      str))
    {
      Absolutize (str, relativeFrom);
      ret.commonInstallRoot = str;
    }
    if (pcfg->TryGetValue("Paths",
      MIKTEX_REGVAL_COMMON_DATA,
      str))
    {
      Absolutize (str, relativeFrom);
      ret.commonDataRoot = str;
    }
    if (pcfg->TryGetValue("Paths",
      MIKTEX_REGVAL_COMMON_CONFIG,
      str))
    {
      Absolutize (str, relativeFrom);
      ret.commonConfigRoot = str;
    }
  }
  else
  {
    if (pcfg->TryGetValue("Paths",
      MIKTEX_REGVAL_USER_ROOTS,
      str))
    {
      Absolutize (str, relativeFrom);
      ret.userRoots = str;
    }
    if (pcfg->TryGetValue("Paths",
      MIKTEX_REGVAL_USER_INSTALL,
      str))
    {
      Absolutize (str, relativeFrom);
      ret.userInstallRoot = str;
    }
    if (pcfg->TryGetValue("Paths",
      MIKTEX_REGVAL_USER_DATA,
      str))
    {
      Absolutize (str, relativeFrom);
      ret.userDataRoot = str;
    }
    if (pcfg->TryGetValue("Paths",
      MIKTEX_REGVAL_USER_CONFIG,
      str))
    {
      Absolutize (str, relativeFrom);
      ret.userConfigRoot = str;
    }
  }      
  pcfg.Release ();

  return (ret);
}

/* _________________________________________________________________________

   Relativize
   _________________________________________________________________________ */

void
Relativize (/*[in,out]*/ string &     paths,
	    /*[in]*/ const PathName & relativeFrom)
{
#if MIKTEX_WINDOWS
  string result;
  for (CSVList path (paths.c_str(), PATH_DELIMITER);
       path.GetCurrent() != 0;
       ++ path)
  {
    if (! result.empty())
    {
      result += PATH_DELIMITER;
    }
    PathName relPath;
    MIKTEX_ASSERT (relPath.GetCapacity() >= MAX_PATH);
    if (PathRelativePathToA(relPath.GetBuffer(),
			    relativeFrom.Get(),
			    FILE_ATTRIBUTE_DIRECTORY,
			    path.GetCurrent(),
			    FILE_ATTRIBUTE_DIRECTORY))
    {
      result += relPath.Get();
    }
    else
    {
      result += path.GetCurrent();
    }
  }
  paths = result;
#else
  UNIMPLEMENTED ("Relativize");
#endif
}

/* _________________________________________________________________________

   SessionImpl::WriteStartupConfigFile

   Write the startup config file.
   _________________________________________________________________________ */

void
SessionImpl::WriteStartupConfigFile
  (/*[in]*/ bool		  common,
   /*[in]*/ const StartupConfig & startupConfig)
{
  MIKTEX_ASSERT (! IsMiKTeXDirect());

  string str;

  StartupConfig defaultConfig = DefaultConfig(startupConfig.config, "", "");

  PathName userStartupConfigFile;

  if (Utils::GetEnvironmentString(MIKTEX_ENV_USER_STARTUP_FILE, str))
  {
    userStartupConfigFile = str;
  }
#if ! NO_REGISTRY
  else if (winRegistry::TryGetRegistryValue(TriState::False,
					    MIKTEX_REGKEY_CORE,
					    MIKTEX_REGVAL_STARTUP_FILE,
					    str,
					    0))
  {
    userStartupConfigFile = str;
  }
#endif
  else
  {
    userStartupConfigFile= defaultConfig.userConfigRoot;
    userStartupConfigFile += MIKTEX_PATH_STARTUP_CONFIG_FILE;
  }

  PathName commonStartupConfigFile;

  if (Utils::GetEnvironmentString(MIKTEX_ENV_COMMON_STARTUP_FILE, str))
  {
    commonStartupConfigFile = str;
  }
#if ! NO_REGISTRY
  else if (winRegistry::TryGetRegistryValue(TriState::True,
					    MIKTEX_REGKEY_CORE,
					    MIKTEX_REGVAL_STARTUP_FILE,
					    str,
					    0))
  {
    commonStartupConfigFile = str;
  }
#endif
  else
  {
    PathName myloc = GetMyLocation(true);
    RemoveDirectoryDelimiter (myloc.GetBuffer());
    PathName internalBindir (MIKTEX_PATH_INTERNAL_BIN_DIR);
    RemoveDirectoryDelimiter (internalBindir.GetBuffer());
    PathName bindir (MIKTEX_PATH_BIN_DIR);
    RemoveDirectoryDelimiter (bindir.GetBuffer());
    PathName prefix;
    if (Utils::GetPathNamePrefix(myloc, internalBindir, prefix))
    {
      commonStartupConfigFile = prefix;
    }
    else if (Utils::GetPathNamePrefix(myloc, bindir, prefix))
    {
      commonStartupConfigFile = prefix;
    }
    else
    {
      UNEXPECTED_CONDITION ("SessionImpl::WriteStartupConfigFile");
    }
    commonStartupConfigFile += MIKTEX_PATH_STARTUP_CONFIG_FILE;
  }

  SmartPointer<Cfg> pcfg (Cfg::Create());

  bool showAllValues = false;
  bool relativize = false;
  PathName relativeFrom;

  if (startupConfig.config == MiKTeXConfiguration::Portable)
  {
    pcfg->PutValue("Auto", "Config", "Portable");
    relativize = (userStartupConfigFile == commonStartupConfigFile);
    if (relativize)
    {
      relativeFrom = commonStartupConfigFile;
      relativeFrom.RemoveFileSpec ();
    }
  }

  if (common || commonStartupConfigFile == userStartupConfigFile)
  {
    if (startupConfig.commonRoots != "" || showAllValues)
    {
      string val = startupConfig.commonRoots;
      if (relativize)
      {
	Relativize (val, relativeFrom);
      };
      pcfg->PutValue ("Paths",
	MIKTEX_REGVAL_COMMON_ROOTS,
	val.c_str(),
	T_("common TEXMF root directories"),
	startupConfig.commonRoots == "");
    }
    if (! startupConfig.commonInstallRoot.Empty()
	&& (startupConfig.commonInstallRoot != defaultConfig.commonInstallRoot || showAllValues))
    {
      string val = startupConfig.commonInstallRoot.Get();
      if (relativize)
      {
	Relativize (val, relativeFrom);
      };
      pcfg->PutValue ("Paths",
	MIKTEX_REGVAL_COMMON_INSTALL,
	val.c_str(),
	T_("common install root"),
	(startupConfig.commonInstallRoot
	== defaultConfig.commonInstallRoot));
    }
    if (! startupConfig.commonDataRoot.Empty()
        && (startupConfig.commonDataRoot != defaultConfig.commonDataRoot || showAllValues))
    {
      string val = startupConfig.commonDataRoot.Get();
      if (relativize)
      {
	Relativize (val, relativeFrom);
      };
      pcfg->PutValue ("Paths",
	MIKTEX_REGVAL_COMMON_DATA,
	val.c_str(),
	T_("common data root"),
	(startupConfig.commonDataRoot
	== defaultConfig.commonDataRoot));
    }
    if (! startupConfig.commonConfigRoot.Empty()
        && (startupConfig.commonConfigRoot != defaultConfig.commonConfigRoot || showAllValues))
    {
      string val = startupConfig.commonConfigRoot.Get();
      if (relativize)
      {
	Relativize (val, relativeFrom);
      };
      pcfg->PutValue ("Paths",
	MIKTEX_REGVAL_COMMON_CONFIG,
	val.c_str(),
	T_("common config root"),
	(startupConfig.commonConfigRoot
	== defaultConfig.commonConfigRoot));
    }
  }
  
  if (! common || commonStartupConfigFile == userStartupConfigFile)
  {
    if (startupConfig.userRoots != "" || showAllValues)
    {
      string val = startupConfig.userRoots;
      if (relativize)
      {
	Relativize (val, relativeFrom);
      };
      pcfg->PutValue ("Paths",
	MIKTEX_REGVAL_USER_ROOTS,
	val.c_str(),
	T_("user TEXMF root directories"),
	startupConfig.userRoots == "");
    }
    if (! startupConfig.userInstallRoot.Empty()
        && (startupConfig.userInstallRoot != defaultConfig.userInstallRoot || showAllValues))
    {
      string val = startupConfig.userInstallRoot.Get();
      if (relativize)
      {
	Relativize (val, relativeFrom);
      };
      pcfg->PutValue ("Paths",
	MIKTEX_REGVAL_USER_INSTALL,
	val.c_str(),
	T_("user install root"),
	(startupConfig.userInstallRoot
	== defaultConfig.userInstallRoot));
    }
    if (! startupConfig.userDataRoot.Empty()
        && (startupConfig.userDataRoot != defaultConfig.userDataRoot || showAllValues))
    {
      string val = startupConfig.userDataRoot.Get();
      if (relativize)
      {
	Relativize (val, relativeFrom);
      };
      pcfg->PutValue ("Paths",
	MIKTEX_REGVAL_USER_DATA,
	val.c_str(),
	T_("user data root"),
	(startupConfig.userDataRoot
	== defaultConfig.userDataRoot));
    }
    if (! startupConfig.userConfigRoot.Empty()
        && (startupConfig.userConfigRoot != defaultConfig.userConfigRoot || showAllValues))
    {
      string val = startupConfig.userConfigRoot.Get();
      if (relativize)
      {
	Relativize (val, relativeFrom);
      };
      pcfg->PutValue ("Paths",
	MIKTEX_REGVAL_USER_CONFIG,
	val.c_str(),
	T_("user config root"),
	(startupConfig.userConfigRoot
	== defaultConfig.userConfigRoot));
    }
  }

  PathName startupConfigFile;
  if (common)
  {
    startupConfigFile = commonStartupConfigFile;
  }
  else
  {
    startupConfigFile = userStartupConfigFile;
  }

  PathName dir;
  dir = startupConfigFile;
  dir.RemoveFileSpec ();
  Directory::Create (dir);
  
  pcfg->Write (startupConfigFile.Get(), T_("MiKTeX startup information"));
}

/* _________________________________________________________________________

   SessionImpl::ReadEnvironment
   _________________________________________________________________________ */

StartupConfig
SessionImpl::ReadEnvironment (/*[in]*/ bool common)
{
  MIKTEX_ASSERT (! IsMiKTeXDirect());

  StartupConfig ret;

  string str;

  if (common)
  {
    if (Utils::GetEnvironmentString(MIKTEX_ENV_COMMON_ROOTS, str))
    {
      ret.commonRoots = str;
    }
    if (Utils::GetEnvironmentString(MIKTEX_ENV_COMMON_INSTALL, str))
    {
      ret.commonInstallRoot = str;
    }
    if (Utils::GetEnvironmentString(MIKTEX_ENV_COMMON_DATA, str))
    {
      ret.commonDataRoot = str;
    }
    if (Utils::GetEnvironmentString(MIKTEX_ENV_COMMON_CONFIG, str))
    {
      ret.commonConfigRoot = str;
    }
  }
  else
  {
    if (Utils::GetEnvironmentString(MIKTEX_ENV_USER_ROOTS, str))
    {
      ret.userRoots = str;
    }
    if (Utils::GetEnvironmentString(MIKTEX_ENV_USER_INSTALL, str))
    {
      ret.userInstallRoot = str;
    }
    if (Utils::GetEnvironmentString(MIKTEX_ENV_USER_DATA, str))
    {
      ret.userDataRoot = str;
    }
    if (Utils::GetEnvironmentString(MIKTEX_ENV_USER_CONFIG, str))
    {
      ret.userConfigRoot = str;
    }
  }

  return (ret);
}

/* _________________________________________________________________________

   SessionImpl::IsMiKTeXDirect

   Return true, if we are running from a MiKTeXDirect medium, i.e., if
   the running program is located in:

   D:\texmf\miktex\bin\				(Windows 32-bit)
   D:\texmf\miktex\amd64-windows\bin		(Windows 64-bit)	
   /cdrom/texmf/miktex/i386-linux\bin		(Linux 32-bit)
   _________________________________________________________________________ */

bool
SessionImpl::IsMiKTeXDirect ()
{
  return (startupConfig.config == MiKTeXConfiguration::Direct);
}

/* _________________________________________________________________________

   SessionImpl::IsMiKTeXPortable
   _________________________________________________________________________ */

bool
SessionImpl::IsMiKTeXPortable ()
{
  return (startupConfig.config == MiKTeXConfiguration::Portable);
}

/* _________________________________________________________________________

   SessionImpl::GetBinDirectory

   Get the fully qualified path to the MiKTeX bin directory.
   _________________________________________________________________________ */

PathName
SessionImpl::GetBinDirectory ()
{
  PathName ret = GetRootDirectory(GetInstallRoot());
  ret += MIKTEX_PATH_BIN_DIR;
  return (ret);
}

/* _________________________________________________________________________

   SessionImpl::ReadAllConfigFiles
   _________________________________________________________________________ */

void
SessionImpl::ReadAllConfigFiles (/*[in]*/ const char *	lpszBaseName,
				 /*[in,out]*/ Cfg *	pCfg)
{
  PathName fileName (MIKTEX_PATH_MIKTEX_CONFIG_DIR,
		     lpszBaseName,
		     ".ini");

  // read all configuration files in reverse order
  for (unsigned idx = GetNumberOfTEXMFRoots(); idx > 0; -- idx)
    {
      if (! IsManagedRoot(idx - 1))
	{
	  continue;
	}
      PathName pathConfigFile (GetRootDirectory(idx - 1), fileName);
      if (File::Exists(pathConfigFile))
	{
	  pCfg->Read (pathConfigFile);
	}
    }
}

/* _________________________________________________________________________

   AppendToEnvVarName
   _________________________________________________________________________ */

MIKTEXSTATICFUNC(void)
AppendToEnvVarName (/*[in,out]*/ string &	name,
		    /*[in]*/ const char *	lpszPart)
{
  for (; *lpszPart != 0; ++ lpszPart)
    {
      if (IsAlpha(*lpszPart))
	{
	  name += ToUpper(*lpszPart);
	}
      else if (IsDigit(*lpszPart))
	{
	  name += *lpszPart;
	}
    }
}

/* _________________________________________________________________________

   SessionImpl::GetSessionValue
   _________________________________________________________________________ */

bool
SessionImpl::GetSessionValue (/*[in]*/ const char *	lpszSectionName,
			      /*[in]*/ const char *	lpszValueName,
			      /*[out]*/ string &	value,
			      /*[in]*/const char *	lpszDefaultValue)
{
  bool haveValue = false;

  // iterate over application names, e.g.: miktex;latex;tex
  for (CSVList app (applicationNames.c_str(), PATH_DELIMITER);
       ! haveValue && app.GetCurrent() != 0;
       ++ app)
    {
      Cfg * pCfg = 0;

      // read configuration files
      if ((initInfo.GetFlags() & InitFlags::NoConfigFiles) == 0)
	{
	  ConfigurationSettings::iterator it =
	    configurationSettings.find(app.GetCurrent());
	  if (it != configurationSettings.end())
	    {
	      pCfg = it->second.Get();
	    }
	  else
	    {
	      pair<ConfigurationSettings::iterator, bool> p =
		configurationSettings.insert(ConfigurationSettings::value_type
					     (app.GetCurrent(),
					      Cfg::Create()));
	      pCfg = p.first->second.Get();
	      ReadAllConfigFiles (app.GetCurrent(), pCfg);
	    }
	}

      const char * lpszSectionName2 = lpszSectionName;
	  
      // section name defaults to application name
      if (lpszSectionName2 == 0)
	{
	  lpszSectionName2 = app.GetCurrent();
	}
      
#if 0
      const ConfigMapping * pMapping =
	FindConfigMapping(lpszSectionName2, lpszValueName);
      
      if (pMapping != 0
	  && pMapping->lpszEnvVarName != 0
	  && Utils::GetEnvironmentString(pMapping->lpszEnvVarName, value))
	{
	  haveValue = true;
	  break;
	}
#endif
      
      // try environment variable
      // MIKTEX_<APPLICATIONNAME>_<SECTIONNAME>_<VALUENAME>
      {
	string envVarName;
	envVarName.reserve (100);
	
	envVarName = MIKTEX_ENV_PREFIX_;
	AppendToEnvVarName (envVarName, app.GetCurrent());
	envVarName += '_';
	AppendToEnvVarName (envVarName, lpszSectionName2);
	envVarName += '_';
	AppendToEnvVarName (envVarName, lpszValueName);
	if (Utils::GetEnvironmentString(envVarName.c_str(), value))
	  {
	    haveValue = true;
	    break;
	  }
      }
      
#if defined(MIKTEX_WINDOWS)
      // try registry value
      if (! IsMiKTeXPortable()
	  && winRegistry::TryGetRegistryValue(TriState::Undetermined,
					   lpszSectionName2,
					   lpszValueName,
					   value,
					   0))
	{
	  haveValue = true;
	  break;
	}
#endif
      
      // try configuration file
      if (pCfg != 0
	  && pCfg->TryGetValue(lpszSectionName2, lpszValueName, value))
	{
	  haveValue = true;
	  break;
	}
    }
  
  // try environment variable
  // MIKTEX_<SECTIONNAME>_<VALUENAME>
  if (! haveValue && lpszSectionName != 0)
    {
      string envVarName (MIKTEX_ENV_PREFIX_);
      AppendToEnvVarName (envVarName, lpszSectionName);
      envVarName += '_';
      AppendToEnvVarName (envVarName, lpszValueName);
      if (Utils::GetEnvironmentString(envVarName.c_str(), value))
	{
	  haveValue = true;
	}
    }
  
  // try environment variable
  // MIKTEX_<VALUENAME>
  if (! haveValue)
    {
      string envVarName (MIKTEX_ENV_PREFIX_);
      AppendToEnvVarName (envVarName, lpszValueName);
      if (Utils::GetEnvironmentString(envVarName.c_str(), value))
	{
	  haveValue = true;
	}
    }
  
  // return the default value
  if (! haveValue && lpszDefaultValue != 0)
    {
      value = lpszDefaultValue;
      haveValue = true;
    }
  
  if (trace_values->IsEnabled())
    {
      if (lpszSectionName != 0)
	{
	  trace_values->WriteFormattedLine ("core",
					    "[%s]%s => %s",
					    lpszSectionName,
					    lpszValueName,
					    (haveValue
					     ? value.c_str()
					     : "null"));
	}
      else
	{
	  trace_values->WriteFormattedLine ("core",
					    "%s => %s",
					    lpszValueName,
					    (haveValue
					     ? value.c_str()
					     : "null"));
	}
    }
      
  return (haveValue);
}

/* _________________________________________________________________________

   SessionImpl::TryGetConfigValue

   Get a configuration parameter.
   _________________________________________________________________________ */

bool
SessionImpl::TryGetConfigValue (/*[in]*/ const char *	lpszSectionName,
				/*[in]*/ const char *	lpszValueName,
				/*[out]*/ string &	value)
{
  MIKTEX_ASSERT_STRING_OR_NIL (lpszSectionName);
  MIKTEX_ASSERT_STRING (lpszValueName);

  return (GetSessionValue(lpszSectionName,
			  lpszValueName,
			  value,
			  0));
}

/* _________________________________________________________________________

   SessionImpl::GetConfigValue

   Get a configuration parameter.
   _________________________________________________________________________ */

string
SessionImpl::GetConfigValue (/*[in]*/ const char * lpszSectionName,
			     /*[in]*/ const char * lpszValueName,
			     /*[in]*/ const char * lpszDefaultValue)
{
  MIKTEX_ASSERT_STRING_OR_NIL (lpszSectionName);
  MIKTEX_ASSERT_STRING (lpszValueName);
  MIKTEX_ASSERT_STRING (lpszDefaultValue);

  string value;

  if (! GetSessionValue(lpszSectionName,
			lpszValueName,
			value,
			lpszDefaultValue))
    {
      INVALID_ARGUMENT ("SessionImpl::GetConfigValue", 0);
    }

  return (value);
}

/* _________________________________________________________________________

   SessionImpl::GetConfigValue

   Get a configuration parameter.
   _________________________________________________________________________ */

char
SessionImpl::GetConfigValue (/*[in]*/ const char * lpszSectionName,
			     /*[in]*/ const char * lpszValueName,
			     /*[in]*/ char	   defaultValue)
{
  MIKTEX_ASSERT_STRING_OR_NIL (lpszSectionName);
  MIKTEX_ASSERT_STRING (lpszValueName);

  string value;

  string defaultValueString;
  defaultValueString = defaultValue;

  if (! GetSessionValue(lpszSectionName,
			lpszValueName,
			value,
			defaultValueString.c_str()))			
    {
      INVALID_ARGUMENT ("SessionImpl::GetConfigValue", 0);
    }

  if (value.length() != 1)
  {
    FATAL_MIKTEX_ERROR ("SessionImpl::GetConfigValue",
      T_("Invalid configuration value."),
      value.c_str());
  }

  return (value[0]);
}

/* _________________________________________________________________________

   SessionImpl::GetConfigValue

   Get a configuration parameter.
   _________________________________________________________________________ */

int
SessionImpl::GetConfigValue (/*[in]*/ const char *	lpszSectionName,
			     /*[in]*/ const char *	lpszValueName,
			     /*[in]*/ int		defaultValue)
{
  MIKTEX_ASSERT_STRING_OR_NIL (lpszSectionName);
  MIKTEX_ASSERT_STRING (lpszValueName);

  string value;

  if (! GetSessionValue(lpszSectionName,
			lpszValueName,
			value,
			0))
    {
      return (defaultValue);
    }

  return (atoi(value.c_str()));
}

/* _________________________________________________________________________

   SessionImpl::GetConfigValue

   Get a configuration parameter.
   _________________________________________________________________________ */

bool
SessionImpl::GetConfigValue (/*[in]*/ const char *	lpszSectionName,
			     /*[in]*/ const char *	lpszValueName,
			     /*[in]*/ bool		defaultValue)
{
  MIKTEX_ASSERT_STRING_OR_NIL (lpszSectionName);
  MIKTEX_ASSERT_STRING (lpszValueName);

  string value;

  if (! GetSessionValue(lpszSectionName,
			lpszValueName,
			value,
			0))
    {
      return (defaultValue);
    }

  if (value == "0"
      || value == "disable"
      || value == "off"
      || value == "f"
      || value == "false"
      || value == "n"
      || value == "no")
    {
      return (false);
    }
  else if (! (value == "1"
	      || value == "enable"
	      || value == "on"
	      || value == "t"
	      || value == "true"
	      || value == "y"
	      || value == "yes"))
    {
      FATAL_MIKTEX_ERROR ("SessionImpl::GetConfigValue",
			  T_("Invalid configuration value."),
			  value.c_str());
    }

  return (true);    
}

/* _________________________________________________________________________

   SessionImpl::GetConfigValue

   Get a configuration parameter.
   _________________________________________________________________________ */

TriState
SessionImpl::GetConfigValue (/*[in]*/ const char *	lpszSectionName,
			     /*[in]*/ const char *	lpszValueName,
			     /*[in]*/ TriState		defaultValue)
{
  MIKTEX_ASSERT_STRING_OR_NIL (lpszSectionName);
  MIKTEX_ASSERT_STRING (lpszValueName);

  string value;

  if (! GetSessionValue(lpszSectionName,
			lpszValueName,
			value,
			0))
    {
      return (defaultValue);
    }

  if (value == "0"
      || value == "disable"
      || value == "off"
      || value == "f"
      || value == "false"
      || value == "n"
      || value == "no")
    {
      return (TriState::False);
    }
  else if (value == "1"
	   || value == "enable"
	   || value == "on"
	   || value == "t"
	   || value == "true"
	   || value == "y"
	   || value == "yes")
    {
      return (TriState::True);
    }
  else if (! (value == ""
	      || value == "2"
	      || value == "?"
	      || value == "undetermined"))
    {
      tostringstream str;
      str << T_("Invalid configuration data (") << value << ")"
	  << T_(" for value ") << lpszValueName << ".";
      FATAL_MIKTEX_ERROR ("SessionImpl::GetConfigValue",
			  str.str().c_str(),
			  0);
    }

  return (TriState::Undetermined);
}

/* _________________________________________________________________________

   SessionImpl::SetConfigValue

   Set a per-user configuration parameter.
   _________________________________________________________________________ */

void
SessionImpl::SetConfigValue (/*[in]*/ const char * lpszSectionName,
			     /*[in]*/ const char * lpszValueName,
			     /*[in]*/ const char * lpszValue)
{
  MIKTEX_ASSERT_STRING (lpszSectionName);

  PathName pathConfigFile = GetSpecialPath(SpecialPath::ConfigRoot);
  pathConfigFile += MIKTEX_PATH_MIKTEX_CONFIG_DIR;
  pathConfigFile += MIKTEX_INI_FILE;

  SmartPointer<Cfg> pCfg (Cfg::Create());

  bool haveConfigFile = File::Exists(pathConfigFile);

  if (haveConfigFile)
    {
      pCfg->Read (pathConfigFile);
    }

#if defined(MIKTEX_WINDOWS)
  if (! haveConfigFile
      && ! IsMiKTeXPortable()
      && ! GetConfigValue(MIKTEX_REGKEY_CORE,
			  MIKTEX_REGVAL_NO_REGISTRY,
			  NO_REGISTRY ? true : false))
    {
      winRegistry::SetRegistryValue (IsAdminMode() ? TriState::True : TriState::False,
				     lpszSectionName,
				     lpszValueName,
				     lpszValue);
      string newValue;
      if (GetSessionValue(lpszSectionName, lpszValueName, newValue, 0))
	{
	  if (newValue != lpszValue)
	    {
	      FATAL_MIKTEX_ERROR
		("SessionImpl::SetConfigValue",
		 T_("\
The configuration value could not be changed. Possible reason: an \
environment variable definition is in the way."),
		 lpszValueName);
	    }
	}
      return;
    }
#endif

  pCfg->PutValue (lpszSectionName, lpszValueName, lpszValue);
  pCfg->Write (pathConfigFile);
  configurationSettings.clear ();
}

/* _________________________________________________________________________

   SessionImpl::SetConfigValue

   Set a configuration parameter.
   _________________________________________________________________________ */

void
SessionImpl::SetConfigValue (/*[in]*/ const char *	lpszSectionName,
			     /*[in]*/ const char *	lpszValueName,
			     /*[in]*/ bool		value)
{
  SetConfigValue (lpszSectionName,
		  lpszValueName,
		  value ? "t" : "f");
}

/* _________________________________________________________________________

   SessionImpl::SetConfigValue

   Set a configuration parameter.
   _________________________________________________________________________ */

void
SessionImpl::SetConfigValue (/*[in]*/ const char *	lpszSectionName,
			     /*[in]*/ const char *	lpszValueName,
			     /*[in]*/ int		value)
{
  SetConfigValue (lpszSectionName, lpszValueName, NUMTOSTR(value));
}

/* _________________________________________________________________________

   SessionImpl::SetAdminMode
   _________________________________________________________________________ */

void
SessionImpl::SetAdminMode (/*[in]*/ bool adminMode)
{
  if (this->adminMode == adminMode)
  {
    return;
  }
  trace_config->WriteFormattedLine ("core",
    T_("turning %s administrative mode"),
    (adminMode ? "on" : "off"));
  fileTypes.clear ();
  UnloadFilenameDatabase ();
  this->adminMode = adminMode;
  if (rootDirectories.size() > 0)
  {
    // reinitialize
    InitializeRootDirectories ();
  }
}

/* _________________________________________________________________________

   SessionImpl::IsAdminMode
   _________________________________________________________________________ */

bool
SessionImpl::IsAdminMode ()
{
  return (adminMode);
}

/* _________________________________________________________________________

   SessionImpl::ConfigureFile
   _________________________________________________________________________ */

void
SessionImpl::ConfigureFile (/*[in]*/ const PathName & pathRel)
{
  PathName pathOut (GetSpecialPath(SpecialPath::ConfigRoot));
  pathOut += pathRel;
  PathName pathIn (GetSpecialPath(SpecialPath::UserInstallRoot));
  pathIn += pathRel;
  pathIn.AppendExtension (".in");
  if (! File::Exists(pathIn))
  {
    pathIn = GetSpecialPath(SpecialPath::CommonInstallRoot);
    pathIn += pathRel;
    pathIn.AppendExtension (".in");
  }
  ConfigureFile (pathIn, pathOut);
}

/* _________________________________________________________________________

   SessionImpl::ConfigureFile
   _________________________________________________________________________ */

void
SessionImpl::ConfigureFile (/*[in]*/ const PathName & pathIn,
			    /*[in]*/ const PathName & pathOut)
{
  Directory::Create (PathName(pathOut).RemoveFileSpec());
  if (File::Exists(pathOut))
    {
      FileAttributes attr = File::GetAttributes(pathOut);
      attr &= ~ FileAttributes(FileAttributes::ReadOnly);
      File::SetAttributes (pathOut, attr);
    }
  FileStream streamIn
    (OpenFile(pathIn.Get(), FileMode::Open, FileAccess::Read, false));
  FileStream streamOut
    (OpenFile(pathOut.Get(), FileMode::Create, FileAccess::Write, false));
  char chr;
  bool readingName = false;
  string name;
  while (streamIn.Read(&chr, 1) == 1)
    {
      if (chr == '@')
	{
	  if (readingName)
	    {
	      string value;
	      readingName = false;
	      if (name == "MIKTEX_INSTALL")
		{
		  value = GetSpecialPath(SpecialPath::InstallRoot).Get();
		}
	      else if (name == "MIKTEX_CONFIG")
		{
		  value = GetSpecialPath(SpecialPath::ConfigRoot).Get();
		}
	      else if (name == "MIKTEX_DATA")
		{
		  value = GetSpecialPath(SpecialPath::DataRoot).Get();
		}
	      else
		{
		  FATAL_MIKTEX_ERROR ("SessionImpl::ConfigureFile",
				      T_("Unknown variable."),
				      name.c_str());
		}
	      streamOut.Write (value.c_str(), value.length());
	    }
	  else
	    {
	      readingName = true;
	      name = "";
	    }
	}
      else if (readingName)
	{
	  name += chr;
	}
      else
	{
	  streamOut.Write (&chr, 1);
	}
    }
  streamIn.Close ();
  streamOut.Close ();
  FileAttributes attr = File::GetAttributes(pathOut);
  attr |= FileAttributes::ReadOnly;
  File::SetAttributes (pathOut, attr);
  if (! Fndb::FileExists(pathOut))
    {
      Fndb::Add (pathOut);
    }
}
  
/* _________________________________________________________________________

   miktex_get_config_value

   Get a configuration parameter.
   _________________________________________________________________________ */

MIKTEXCEEAPI(char *)
miktex_get_config_value (/*[in]*/ const char *	lpszSectionName,
			 /*[in]*/ const char *	lpszValueName,
			 /*[out]*/ char *	lpszBuf,
			 /*[in]*/ size_t	bufSize,
			 /*[in]*/ const char *	lpszDefaultValue)
{
  C_FUNC_BEGIN ();
  string value;
  if (! SessionImpl::GetSession()->TryGetConfigValue(lpszSectionName,
						     lpszValueName,
						     value))
    {
      if (lpszDefaultValue == 0)
	{
	  return (0);
	}
      value = lpszDefaultValue;
    }
  Utils::CopyString (lpszBuf, bufSize, value.c_str());
  return (lpszBuf);
  C_FUNC_END ();
}
