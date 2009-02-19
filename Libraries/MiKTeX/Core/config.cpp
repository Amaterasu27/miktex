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
      PathName bindir (MIKTEX_PATH_BIN_DIR);
      RemoveDirectoryDelimiter (bindir.GetBuffer());
      PathName prefix;
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

   SessionImpl::ReadStartupConfigFile

   Read the contents of the startup config file.
   _________________________________________________________________________ */

StartupConfig
SessionImpl::ReadStartupConfigFile (/*[in]*/ bool common)
{
  MIKTEX_ASSERT (! IsMiKTeXDirect());

  PathName path;

  StartupConfig ret;

  if (FindStartupConfigFile(common, path))
    {
      if (common)
      {
	haveCommonStartupConfigFile = true;
      }
      else
      {
	haveUserStartupConfigFile = true;
      }

      startupConfigFile = path;

      SmartPointer<Cfg> pcfg (Cfg::Create());
      
      pcfg->Read (path);

      string str;

      if (common)
      {      
	if (pcfg->TryGetValue("Paths",
	  MIKTEX_REGVAL_COMMON_ROOTS,
	  str))
	{
	  ret.commonRoots = str;
	}
	if (pcfg->TryGetValue("Paths",
	  MIKTEX_REGVAL_COMMON_INSTALL,
	  str))
	{
	  ret.commonInstallRoot = str;
	}
	if (pcfg->TryGetValue("Paths",
	  MIKTEX_REGVAL_COMMON_DATA,
	  str))
	{
	  ret.commonDataRoot = str;
	}
	if (pcfg->TryGetValue("Paths",
	  MIKTEX_REGVAL_COMMON_CONFIG,
	  str))
	{
	  ret.commonConfigRoot = str;
	}
      }
      else
      {
	if (pcfg->TryGetValue("Paths",
	  MIKTEX_REGVAL_USER_ROOTS,
	  str))
	{
	  ret.userRoots = str;
	}
	if (pcfg->TryGetValue("Paths",
	  MIKTEX_REGVAL_USER_INSTALL,
	  str))
	{
	  ret.userInstallRoot = str;
	}
	if (pcfg->TryGetValue("Paths",
	  MIKTEX_REGVAL_USER_DATA,
	  str))
	{
	  ret.userDataRoot = str;
	}
	if (pcfg->TryGetValue("Paths",
	  MIKTEX_REGVAL_USER_CONFIG,
	  str))
	{
	  ret.userConfigRoot = str;
	}
      }      
      pcfg.Release ();
    }

  return (ret);
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

  StartupConfig defaultConfig = DefaultConfig();

  SmartPointer<Cfg> pcfg (Cfg::Create());

  if (common)
  {
    pcfg->PutValue ("Paths",
      MIKTEX_REGVAL_COMMON_ROOTS,
      startupConfig.commonRoots.c_str(),
      T_("common TEXMF root directories"),
      false);
    if (! startupConfig.commonInstallRoot.Empty())
    {
      pcfg->PutValue ("Paths",
	MIKTEX_REGVAL_COMMON_INSTALL,
	startupConfig.commonInstallRoot.Get(),
	T_("common install root"),
	(startupConfig.commonInstallRoot
	== defaultConfig.commonInstallRoot));
    }
    if (! startupConfig.commonDataRoot.Empty())
    {
      pcfg->PutValue ("Paths",
	MIKTEX_REGVAL_COMMON_DATA,
	startupConfig.commonDataRoot.Get(),
	T_("common data root"),
	(startupConfig.commonDataRoot
	== defaultConfig.commonDataRoot));
    }
    if (! startupConfig.commonConfigRoot.Empty())
    {
      pcfg->PutValue ("Paths",
	MIKTEX_REGVAL_COMMON_CONFIG,
	startupConfig.commonConfigRoot.Get(),
	T_("common config root"),
	(startupConfig.commonConfigRoot
	== defaultConfig.commonConfigRoot));
    }
    if (! startupConfig.commonConfigRoot.Empty())
    {
      startupConfigFile = startupConfig.commonConfigRoot;
    }
    else
    {
      startupConfigFile = defaultConfig.commonConfigRoot;
    }
  }
  else
  {
    pcfg->PutValue ("Paths",
      MIKTEX_REGVAL_USER_ROOTS,
      startupConfig.userRoots.c_str(),
      T_("user TEXMF root directories"),
      false);
    if (! startupConfig.userInstallRoot.Empty())
    {
      pcfg->PutValue ("Paths",
	MIKTEX_REGVAL_USER_INSTALL,
	startupConfig.userInstallRoot.Get(),
	T_("user install root"),
	(startupConfig.userInstallRoot
	== defaultConfig.userInstallRoot));
    }
    if (! startupConfig.userDataRoot.Empty())
    {
      pcfg->PutValue ("Paths",
	MIKTEX_REGVAL_USER_DATA,
	startupConfig.userDataRoot.Get(),
	T_("user data root"),
	(startupConfig.userDataRoot
	== defaultConfig.userDataRoot));
    }
    if (! startupConfig.userConfigRoot.Empty())
    {
      pcfg->PutValue ("Paths",
	MIKTEX_REGVAL_USER_CONFIG,
	startupConfig.userConfigRoot.Get(),
	T_("user config root"),
	(startupConfig.userConfigRoot
	== defaultConfig.userConfigRoot));
    }
    if (! startupConfig.userConfigRoot.Empty())
    {
      startupConfigFile = startupConfig.userConfigRoot;
    }
    else
    {
      startupConfigFile = defaultConfig.userConfigRoot;
    }
  }

  if (startupConfigFile.Empty())
    {
      UNEXPECTED_CONDITION ("SessionImpl::WriteStartupConfigFile");
    }

  startupConfigFile += MIKTEX_PATH_STARTUP_CONFIG_FILE;

  PathName dir;
  dir = startupConfigFile;
  dir.RemoveFileSpec ();
  Directory::Create (dir);
  
  pcfg->Write (startupConfigFile.Get());
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
    if (Utils::GetEnvironmentString (MIKTEX_ENV_COMMON_ROOTS, str))
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
    if (Utils::GetEnvironmentString (MIKTEX_ENV_USER_ROOTS, str))
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
  if (triMiKTeXDirect == TriState::Undetermined)
  {
    PathName myloc = GetMyLocation(false);
    RemoveDirectoryDelimiter (myloc.GetBuffer());
    PathName bindir = MIKTEX_PATH_BIN_DIR;
    RemoveDirectoryDelimiter (bindir.GetBuffer());
    PathName prefix;
    if (! Utils::GetPathNamePrefix(myloc, bindir, prefix))
    {
      return (false);
    }
    PathName path (prefix);
    path += MIKTEX_PATH_MD_INI;
    triMiKTeXDirect =
      ((File::Exists(path)
      && ((File::GetAttributes(path) & FileAttributes::ReadOnly) != 0))
      ? TriState::True
      : TriState::False);
    if (triMiKTeXDirect == TriState::True)
    {
      trace_config->WriteLine (0, T_("*** MiKTeXDirect setup ***"));
    }
  }
  return (triMiKTeXDirect == TriState::True);
}

/* _________________________________________________________________________

   SessionImpl::GetBinDirectory

   Get the fully qualified path to the MiKTeX bin directory.  Consider
   the MIKTEX_BINDIR environment variable.
   _________________________________________________________________________ */

PathName
SessionImpl::GetBinDirectory ()
{
  string str;
  PathName ret;
  if (! IsMiKTeXDirect()
      && Utils::GetEnvironmentString(MIKTEX_ENV_BIN_DIR, str))
    {
      ret = str;
    }
  else
    {
      ret = initInfo.GetBinDirectory();
      if (ret.Empty())
	{
	  ret = GetRootDirectory(GetInstallRoot());
	  ret += MIKTEX_PATH_BIN_DIR;
	}
    }
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
      if (winRegistry::TryGetRegistryValue(TriState::Undetermined,
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

   SessionImpl::SetUserConfigValue

   Set a per-user configuration parameter.
   _________________________________________________________________________ */

void
SessionImpl::SetUserConfigValue (/*[in]*/ const char * lpszSectionName,
				 /*[in]*/ const char * lpszValueName,
				 /*[in]*/ const char * lpszValue)
{
  MIKTEX_ASSERT_STRING (lpszSectionName);

  PathName pathConfigFile = GetSpecialPath(SpecialPath::UserConfigRoot);
  pathConfigFile += MIKTEX_PATH_MIKTEX_CONFIG_DIR;
  pathConfigFile += MIKTEX_INI_FILE;

  SmartPointer<Cfg> pCfg (Cfg::Create());

  bool haveConfigFile = File::Exists(pathConfigFile);

  if (haveConfigFile)
    {
      pCfg->Read (pathConfigFile);
    }

#if ! NO_REGISTRY
  if (! haveConfigFile)
    {
      winRegistry::SetRegistryValue (TriState::False,
				     lpszSectionName,
				     lpszValueName,
				     lpszValue);
      string newValue;
      if (GetSessionValue(lpszSectionName, lpszValueName, newValue, 0))
	{
	  if (newValue != lpszValue)
	    {
	      FATAL_MIKTEX_ERROR
		("SessionImpl::SetUserConfigValue",
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
}

/* _________________________________________________________________________

   SessionImpl::SetUserConfigValue

   Set a configuration parameter.
   _________________________________________________________________________ */

void
SessionImpl::SetUserConfigValue (/*[in]*/ const char *	lpszSectionName,
				 /*[in]*/ const char *	lpszValueName,
				 /*[in]*/ bool		value)
{
  SetUserConfigValue (lpszSectionName,
		      lpszValueName,
		      value ? "t" : "f");
}

/* _________________________________________________________________________

   SessionImpl::SetUserConfigValue

   Set a configuration parameter.
   _________________________________________________________________________ */

void
SessionImpl::SetUserConfigValue (/*[in]*/ const char *	lpszSectionName,
				 /*[in]*/ const char *	lpszValueName,
				 /*[in]*/ int		value)
{
  SetUserConfigValue (lpszSectionName, lpszValueName, NUMTOSTR(value));
}

/* _________________________________________________________________________

   SessionImpl::SetAdminMode
   _________________________________________________________________________ */

void
SessionImpl::SetAdminMode (/*[in]*/ bool adminMode)
{
  this->adminMode = adminMode;
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
  PathName pathIn (GetSpecialPath(SpecialPath::InstallRoot));
  pathIn += pathRel;
  pathIn.AppendExtension (".in");
  PathName pathOut (GetSpecialPath(SpecialPath::ConfigRoot));
  pathOut += pathRel;
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
