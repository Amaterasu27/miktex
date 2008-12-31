/* config.cpp: MiKTeX configuration settings

   Copyright (C) 1996-2007 Christian Schenk

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

   SessionImpl::FindStartupConfigFile

   Try to find the MiKTeX startup file.
   _________________________________________________________________________ */

bool
SessionImpl::FindStartupConfigFile (/*[out]*/ PathName & path)
{
  if ((initInfo.GetFlags() & InitFlags::NoConfigFiles) != 0)
    {
      return (false);
    }

  string str;

  if (Utils::GetEnvironmentString(MIKTEX_ENV_STARTUP_FILE, str))
    {
      path = str;
      return (true);
    }

#if defined(MIKTEX_WINDOWS)
  if (winRegistry::TryGetRegistryValue(TriState::Undetermined,
				       MIKTEX_REGKEY_CORE,
				       MIKTEX_REGVAL_STARTUP_FILE,
				       str,
				       0))
    {
      path = str;
      return (true);
    }
#endif

  PathName bindir = GetMyLocation();

#if defined(MIKTEX_WINDOWS)

  path = bindir;

#  if defined(MIKTEX_WINDOWS_32)
  path += PARENT_PARENT_DIRECTORY;
#  else
  path += PARENT_PARENT_PARENT_DIRECTORY;
#  endif

  path += MIKTEX_PATH_STARTUP_CONFIG_FILE;
  if (File::Exists(path))
    {
      path.MakeAbsolute ();
      return (true);
    }
  else
    {
      return (false);
    }

#elif defined(MIKTEX_UNIX)

  RemoveDirectoryDelimiter (bindir.GetBuffer());

  if (PathName::Compare(bindir, "/usr/local/bin") == 0
      || PathName::Compare(bindir, "/usr/bin") == 0)
    {
      PathName root (bindir);

      // /usr/local/bin => /usr/local
      // /usr/bin => /usr
      root.CutOffLastComponent ();

      // /usr/local/share/texmf/miktex/config/paths.ini
      // /usr/share/texmf/miktex/config/paths.ini
      path = root;
      path += "share/texmf";
      path += MIKTEX_PATH_STARTUP_CONFIG_FILE;

      if (File::Exists(path))
	{
	  return (true);
	}

      // /usr/local/texmf/miktex/config/paths.ini
      // /usr/texmf/miktex/config/paths.ini
      path = root;
      path += "texmf";
      path += MIKTEX_PATH_STARTUP_CONFIG_FILE;

      if (File::Exists(path))
	{
	  return (true);
	}

      return (false);
    }
  else
    {
      return (false);
    }

#else

#  error Unimplemented: SessionImpl::FindStartupConfigFile()

#endif
}

/* _________________________________________________________________________

   SessionImpl::ReadStartupConfigFile

   Read the contents of the startup config file.
   _________________________________________________________________________ */

StartupConfig
SessionImpl::ReadStartupConfigFile ()
{
  MIKTEX_ASSERT (! IsMiKTeXDirect());

  PathName path;

  StartupConfig ret;

  if (FindStartupConfigFile(path))
    {
      haveStartupConfigFile = true;
      startupConfigFile = path;

      SmartPointer<Cfg> pcfg (Cfg::Create());
      
      pcfg->Read (path);

      string str;
      
      if (pcfg->TryGetValue("Paths",
			    MIKTEX_REGVAL_ROOTS,
			    str))
	{
	  ret.roots = str;
	}
      
      if (pcfg->TryGetValue("Paths",
			    MIKTEX_REGVAL_COMMON_INSTALL,
			    str))
	{
	  ret.commonInstallRoot = str;
	}
 
      if (pcfg->TryGetValue("Paths",
			    MIKTEX_REGVAL_USER_INSTALL,
			    str))
	{
	  ret.userInstallRoot = str;
	}

      if (pcfg->TryGetValue("Paths",
			    MIKTEX_REGVAL_COMMON_DATA,
			    str))
	{
	  ret.commonDataRoot = str;
	}
      
      if (pcfg->TryGetValue("Paths",
			    MIKTEX_REGVAL_USER_DATA,
			    str))
	{
	  ret.userDataRoot = str;
	}
      
      if (pcfg->TryGetValue("Paths",
			    MIKTEX_REGVAL_COMMON_CONFIG,
			    str))
	{
	  ret.commonConfigRoot = str;
	}
      
      if (pcfg->TryGetValue("Paths",
			    MIKTEX_REGVAL_USER_CONFIG,
			    str))
	{
	  ret.userConfigRoot = str;
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
(/*[in]*/ const StartupConfig & startupConfig)
{
  MIKTEX_ASSERT (! IsMiKTeXDirect());

  StartupConfig defaultConfig = DefaultConfig(true);

  SmartPointer<Cfg> pcfg (Cfg::Create());

  pcfg->PutValue ("Paths",
		  MIKTEX_REGVAL_ROOTS,
		  startupConfig.roots.c_str());

  if (! startupConfig.commonInstallRoot.Empty()
      && startupConfig.commonInstallRoot != defaultConfig.commonInstallRoot)
    {
      pcfg->PutValue ("Paths",
		      MIKTEX_REGVAL_COMMON_INSTALL,
		      startupConfig.commonInstallRoot.Get());
    }

  if (! startupConfig.userInstallRoot.Empty()
      && startupConfig.userInstallRoot != defaultConfig.userInstallRoot)
    {
      pcfg->PutValue ("Paths",
		      MIKTEX_REGVAL_USER_INSTALL,
		      startupConfig.userInstallRoot.Get());
    }

  if (! startupConfig.commonDataRoot.Empty()
      && startupConfig.commonDataRoot != defaultConfig.commonDataRoot)
    {
      pcfg->PutValue ("Paths",
		      MIKTEX_REGVAL_COMMON_DATA,
		      startupConfig.commonDataRoot.Get());
    }

  if (! startupConfig.userDataRoot.Empty()
      && startupConfig.userDataRoot != defaultConfig.userDataRoot)
    {
      pcfg->PutValue ("Paths",
		      MIKTEX_REGVAL_USER_DATA,
		      startupConfig.userDataRoot.Get());
    }

  if (! startupConfig.commonConfigRoot.Empty()
      && startupConfig.commonConfigRoot != defaultConfig.commonConfigRoot)
    {
      pcfg->PutValue ("Paths",
		      MIKTEX_REGVAL_COMMON_CONFIG,
		      startupConfig.commonConfigRoot.Get());
    }

  if (! startupConfig.userConfigRoot.Empty()
      && startupConfig.userConfigRoot != defaultConfig.userConfigRoot)
    {
      pcfg->PutValue ("Paths",
		      MIKTEX_REGVAL_USER_CONFIG,
		      startupConfig.userConfigRoot.Get());
    }

  if (startupConfigFile.Empty())
    {
      UNEXPECTED_CONDITION ("SessionImpl::WriteStartupConfigFile");
    }

  pcfg->Write (startupConfigFile.Get());
}

/* _________________________________________________________________________

   SessionImpl::ReadEnvironment
   _________________________________________________________________________ */

StartupConfig
SessionImpl::ReadEnvironment ()
{
  MIKTEX_ASSERT (! IsMiKTeXDirect());

  StartupConfig ret;

  string str;

  if (Utils::GetEnvironmentString (MIKTEX_ENV_ROOTS, str))
    {
      ret.roots = str;
    }

  if (Utils::GetEnvironmentString(MIKTEX_ENV_USER_INSTALL, str))
    {
      ret.userInstallRoot = str;
    }

  if (Utils::GetEnvironmentString(MIKTEX_ENV_COMMON_INSTALL, str))
    {
      ret.commonInstallRoot = str;
    }

  if (Utils::GetEnvironmentString(MIKTEX_ENV_USER_DATA, str))
    {
      ret.userDataRoot = str;
    }

  if (Utils::GetEnvironmentString(MIKTEX_ENV_COMMON_DATA, str))
    {
      ret.commonDataRoot = str;
    }

  if (Utils::GetEnvironmentString(MIKTEX_ENV_USER_CONFIG, str))
    {
      ret.userConfigRoot = str;
    }

  if (Utils::GetEnvironmentString(MIKTEX_ENV_COMMON_CONFIG, str))
    {
      ret.commonConfigRoot = str;
    }

  return (ret);
}

/* _________________________________________________________________________

   SessionImpl::IsMiKTeXDirect

   Return true, if we are running from a MiKTeXDirect medium, i.e., if
   the running program is located in:

   D:\texmf\miktex\bin\				(Windows 32-bit)
   D:\texmf\miktex\bin\amd64			(Windows 64-bit)	
   /cdrom/texmf/miktex/bin/i386-linux		(Linux 32-bit)
   _________________________________________________________________________ */

bool
SessionImpl::IsMiKTeXDirect ()
{
  if (triMiKTeXDirect == TriState::Undetermined)
    {
      PathName path;
      path = GetMyLocation();
#if defined(MIKTEX_WINDOWS_32)
      path += PARENT_PARENT_DIRECTORY;
#else
      path += PARENT_PARENT_PARENT_DIRECTORY;
#endif
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
#if defined(MIKTEX_PATH_BIN_DIR)
	  ret = GetRootDirectory(GetInstallRoot());
	  ret += MIKTEX_PATH_BIN_DIR;
#else
	  ret = GetMyLocation();
#endif
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

   Set a configuration parameter.
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

  if (File::Exists(pathConfigFile))
  {
    pCfg->Read (pathConfigFile);
  }

#if defined(MIKTEX_WINDOWS)
  else
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
The configuration value could not be changed. Possibly an \
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

   SessionImpl::SharedMiKTeXSetup
   _________________________________________________________________________ */

void
SessionImpl::SharedMiKTeXSetup (/*[in]*/ bool shared)
{
  SharedMiKTeXSetup (shared, false);
}

/* _________________________________________________________________________

   SessionImpl::SharedMiKTeXSetup
   _________________________________________________________________________ */

void
SessionImpl::SharedMiKTeXSetup (/*[in]*/ bool shared,
				/*[in]*/ bool sessionOnly)
{
  if (! sessionOnly)
    {
#if defined(MIKTEX_WINDOWS)
      winRegistry::SetRegistryValue (TriState::True,
				     MIKTEX_REGKEY_CORE,
				     MIKTEX_REGVAL_SHARED_SETUP,
				     NUMTOSTR(shared));
#else
#  warning Unimplemented: SessionImpl::SharedMiKTeXSetup()
      UNIMPLEMENTED ("SessionImpl::SharedMiKTeXSetup");
#endif
    }
  sharedSetup = (shared ? TriState::True : TriState::False);
}

/* _________________________________________________________________________

   SessionImpl::IsSharedMiKTeXSetup
   _________________________________________________________________________ */

TriState
SessionImpl::IsSharedMiKTeXSetup ()
{
  if (sharedSetup == TriState::Undetermined)
    {
#if defined(MIKTEX_WINDOWS)
      if ((initInfo.GetFlags() & InitFlags::NoConfigFiles) == 0)
	{
	  string value;
	  if (winRegistry::TryGetRegistryValue(TriState::True,
					       MIKTEX_REGKEY_CORE,
					       MIKTEX_REGVAL_SHARED_SETUP,
					       value,
					       0))
	    {
	      sharedSetup =
		((value == "0") ? TriState::False : TriState::True);
	    }
	}
#else
#  warning Unimplemented: SessionImpl::IsSharedMiKTeXSetup()
#endif
    }
  return (sharedSetup);
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
