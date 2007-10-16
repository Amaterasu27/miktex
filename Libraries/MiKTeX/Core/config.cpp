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

#include "MiKTeX/Core/Environment"
#include "miktex/paths.h"
#include "miktex/reg.h"
#include "miktex/version.h"

#if defined(MIKTEX_WINDOWS)
#  include "win/winRegistry.h"
#endif

/* _________________________________________________________________________

   ConfigMapping
   _________________________________________________________________________ */

#if 0
struct ConfigMapping
{
  const MIKTEXCHAR *	lpszConfigSection;
  const MIKTEXCHAR *	lpszConfigValueName;
  const MIKTEXCHAR *	lpszEnvVarName;
#if defined(MIKTEX_WINDOWS)
  const MIKTEXCHAR *	lpszRegKey;
  const MIKTEXCHAR *	lpszRegValueName;
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
FindConfigMapping (/*[in]*/ const MIKTEXCHAR *	lpszConfigSection,
		   /*[in]*/ const MIKTEXCHAR *	lpszConfigValueName)
{
  for (size_t i = 0;
       i < sizeof(configMappings) / sizeof(configMappings);
       ++ i)
    {
      if ((StringCompare(configMappings[i].lpszConfigSection,
			 lpszConfigSection,
			 true)
	   == 0)
	  && (StringCompare(configMappings[i].lpszConfigValueName,
			    lpszConfigValueName,
			    true))
	  == 0)
	{
	  return (&configMappings[i]);
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

  tstring str;

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

  if (PathName::Compare(bindir, T_("/usr/local/bin")) == 0
      || PathName::Compare(bindir, T_("/usr/bin")) == 0)
    {
      PathName root (bindir);

      // /usr/local/bin => /usr/local
      // /usr/bin => /usr
      root.CutOffLastComponent ();

      // /usr/local/share/texmf/miktex/config/paths.ini
      // /usr/share/texmf/miktex/config/paths.ini
      path = root;
      path += T_("share/texmf");
      path += MIKTEX_PATH_STARTUP_CONFIG_FILE;

      if (File::Exists(path))
	{
	  return (true);
	}

      // /usr/local/texmf/miktex/config/paths.ini
      // /usr/texmf/miktex/config/paths.ini
      path = root;
      path += T_("texmf");
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

      tstring str;
      
      if (pcfg->TryGetValue(T_("Paths"),
			    MIKTEX_REGVAL_ROOTS,
			    str))
	{
	  ret.roots = str;
	}
      
      if (pcfg->TryGetValue(T_("Paths"),
			    MIKTEX_REGVAL_INSTALL,
			    str))
	{
	  ret.installRoot = str;
	}
      
      if (pcfg->TryGetValue(T_("Paths"),
			    MIKTEX_REGVAL_COMMON_DATA,
			    str))
	{
	  ret.commonDataRoot = str;
	}
      
      if (pcfg->TryGetValue(T_("Paths"),
			    MIKTEX_REGVAL_USER_DATA,
			    str))
	{
	  ret.userDataRoot = str;
	}
      
      if (pcfg->TryGetValue(T_("Paths"),
			    MIKTEX_REGVAL_COMMON_CONFIG,
			    str))
	{
	  ret.commonConfigRoot = str;
	}
      
      if (pcfg->TryGetValue(T_("Paths"),
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

  pcfg->PutValue (T_("Paths"),
		  MIKTEX_REGVAL_ROOTS,
		  startupConfig.roots.c_str());

  if (! startupConfig.installRoot.Empty()
      && startupConfig.installRoot != defaultConfig.installRoot)
    {
      pcfg->PutValue (T_("Paths"),
		      MIKTEX_REGVAL_INSTALL,
		      startupConfig.installRoot.Get());
    }

  if (! startupConfig.commonDataRoot.Empty()
      && startupConfig.commonDataRoot != defaultConfig.commonDataRoot)
    {
      pcfg->PutValue (T_("Paths"),
		      MIKTEX_REGVAL_COMMON_DATA,
		      startupConfig.commonDataRoot.Get());
    }

  if (! startupConfig.userDataRoot.Empty()
      && startupConfig.userDataRoot != defaultConfig.userDataRoot)
    {
      pcfg->PutValue (T_("Paths"),
		      MIKTEX_REGVAL_USER_DATA,
		      startupConfig.userDataRoot.Get());
    }

  if (! startupConfig.commonConfigRoot.Empty()
      && startupConfig.commonConfigRoot != defaultConfig.commonConfigRoot)
    {
      pcfg->PutValue (T_("Paths"),
		      MIKTEX_REGVAL_COMMON_CONFIG,
		      startupConfig.commonConfigRoot.Get());
    }

  if (! startupConfig.userConfigRoot.Empty()
      && startupConfig.userConfigRoot != defaultConfig.userConfigRoot)
    {
      pcfg->PutValue (T_("Paths"),
		      MIKTEX_REGVAL_USER_CONFIG,
		      startupConfig.userConfigRoot.Get());
    }

  if (startupConfigFile.Empty())
    {
      UNEXPECTED_CONDITION (T_("SessionImpl::WriteStartupConfigFile"));
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

  tstring str;

  if (Utils::GetEnvironmentString (MIKTEX_ENV_ROOTS, str))
    {
      ret.roots = str;
    }

  if (Utils::GetEnvironmentString(MIKTEX_ENV_INSTALL, str))
    {
      ret.installRoot = str;
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
  tstring str;
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
SessionImpl::ReadAllConfigFiles (/*[in]*/ const MIKTEXCHAR *	lpszBaseName,
				 /*[in,out]*/ Cfg *		pCfg)
{
  PathName fileName (MIKTEX_PATH_MIKTEX_CONFIG_DIR,
		     lpszBaseName,
		     T_(".ini"));

  // read all configuration files in reverse order
  for (unsigned i = GetNumberOfTEXMFRoots(); i > 0; -- i)
    {
      if (! IsManagedRoot(i - 1))
	{
	  continue;
	}
      PathName pathConfigFile (GetRootDirectory(i - 1), fileName);
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
AppendToEnvVarName (/*[in,out]*/ tstring &	name,
		    /*[in]*/ const MIKTEXCHAR *	lpszPart)
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
SessionImpl::GetSessionValue (/*[in]*/ const MIKTEXCHAR * lpszSectionName,
			      /*[in]*/ const MIKTEXCHAR * lpszValueName,
			      /*[out]*/ tstring &	  value,
			      /*[in]*/const MIKTEXCHAR *  lpszDefaultValue)
{
  bool haveValue = false;

  for (CSVList app (applicationNames.c_str(), PATH_DELIMITER);
       ! haveValue && app.GetCurrent() != 0;
       ++ app)
    {
      Cfg * pCfg = 0;

      // read configuration file
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

      const MIKTEXCHAR * lpszSectionName2 = lpszSectionName;
	  
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
      
      {
	tstring envVarName;
	envVarName.reserve (100);
	
	// try environment variable
	// MIKTEX_<APPLICATIONNAME>_<SECTIONNAME>_<VALUENAME>
	envVarName = MIKTEX_ENV_PREFIX_;
	AppendToEnvVarName (envVarName, app.GetCurrent());
	envVarName += T_('_');
	AppendToEnvVarName (envVarName, lpszSectionName2);
	envVarName += T_('_');
	AppendToEnvVarName (envVarName, lpszValueName);
	if (Utils::GetEnvironmentString(envVarName.c_str(), value))
	  {
	    haveValue = true;
	    break;
	  }
      }
      
#if defined(MIKTEX_WINDOWS)
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
      tstring envVarName (MIKTEX_ENV_PREFIX_);
      AppendToEnvVarName (envVarName, lpszSectionName);
      envVarName += T_('_');
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
      tstring envVarName (MIKTEX_ENV_PREFIX_);
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
	  trace_values->WriteFormattedLine (T_("core"),
					    T_("[%s]%s => %s"),
					    lpszSectionName,
					    lpszValueName,
					    (haveValue
					     ? value.c_str()
					     : T_("null")));
	}
      else
	{
	  trace_values->WriteFormattedLine (T_("core"),
					    T_("%s => %s"),
					    lpszValueName,
					    (haveValue
					     ? value.c_str()
					     : T_("null")));
	}
    }
      
  return (haveValue);
}

/* _________________________________________________________________________

   SessionImpl::TryGetConfigValue

   Get a configuration parameter.
   _________________________________________________________________________ */

bool
SessionImpl::TryGetConfigValue (/*[in]*/ const MIKTEXCHAR * lpszSectionName,
				/*[in]*/ const MIKTEXCHAR * lpszValueName,
				/*[out]*/ tstring &		value)
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

tstring
SessionImpl::GetConfigValue (/*[in]*/ const MIKTEXCHAR * lpszSectionName,
			     /*[in]*/ const MIKTEXCHAR * lpszValueName,
			     /*[in]*/ const MIKTEXCHAR * lpszDefaultValue)
{
  MIKTEX_ASSERT_STRING_OR_NIL (lpszSectionName);
  MIKTEX_ASSERT_STRING (lpszValueName);
  MIKTEX_ASSERT_STRING (lpszDefaultValue);

  tstring value;

  if (! GetSessionValue(lpszSectionName,
			lpszValueName,
			value,
			lpszDefaultValue))
    {
      INVALID_ARGUMENT (T_("SessionImpl::GetConfigValue"), 0);
    }

  return (value);
}

/* _________________________________________________________________________

   SessionImpl::GetConfigValue

   Get a configuration parameter.
   _________________________________________________________________________ */

int
SessionImpl::GetConfigValue (/*[in]*/ const MIKTEXCHAR * lpszSectionName,
			     /*[in]*/ const MIKTEXCHAR *	lpszValueName,
			     /*[in]*/ int			defaultValue)
{
  MIKTEX_ASSERT_STRING_OR_NIL (lpszSectionName);
  MIKTEX_ASSERT_STRING (lpszValueName);

  tstring value;

  if (! GetSessionValue(lpszSectionName,
			lpszValueName,
			value,
			0))
    {
      return (defaultValue);
    }

  return (AToI(value.c_str()));
}

/* _________________________________________________________________________

   SessionImpl::GetConfigValue

   Get a configuration parameter.
   _________________________________________________________________________ */

bool
SessionImpl::GetConfigValue (/*[in]*/ const MIKTEXCHAR * lpszSectionName,
			     /*[in]*/ const MIKTEXCHAR *	lpszValueName,
			     /*[in]*/ bool			defaultValue)
{
  MIKTEX_ASSERT_STRING_OR_NIL (lpszSectionName);
  MIKTEX_ASSERT_STRING (lpszValueName);

  tstring value;

  if (! GetSessionValue(lpszSectionName,
			lpszValueName,
			value,
			0))
    {
      return (defaultValue);
    }

  if (value == T_("0")
      || value == T_("disable")
      || value == T_("off")
      || value == T_("f")
      || value == T_("false")
      || value == T_("n")
      || value == T_("no"))
    {
      return (false);
    }
  else if (! (value == T_("1")
	      || value == T_("enable")
	      || value == T_("on")
	      || value == T_("t")
	      || value == T_("true")
	      || value == T_("y")
	      || value == T_("yes")))
    {
      FATAL_MIKTEX_ERROR (T_("SessionImpl::GetConfigValue"),
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
SessionImpl::GetConfigValue (/*[in]*/ const MIKTEXCHAR * lpszSectionName,
			     /*[in]*/ const MIKTEXCHAR *	lpszValueName,
			     /*[in]*/ TriState			defaultValue)
{
  MIKTEX_ASSERT_STRING_OR_NIL (lpszSectionName);
  MIKTEX_ASSERT_STRING (lpszValueName);

  tstring value;

  if (! GetSessionValue(lpszSectionName,
			lpszValueName,
			value,
			0))
    {
      return (defaultValue);
    }

  if (value == T_("0")
      || value == T_("disable")
      || value == T_("off")
      || value == T_("f")
      || value == T_("false")
      || value == T_("n")
      || value == T_("no"))
    {
      return (TriState::False);
    }
  else if (value == T_("1")
	   || value == T_("enable")
	   || value == T_("on")
	   || value == T_("t")
	   || value == T_("true")
	   || value == T_("y")
	   || value == T_("yes"))
    {
      return (TriState::True);
    }
  else if (! (value == T_("")
	      || value == T_("2")
	      || value == T_("?")
	      || value == T_("undetermined")))
    {
      tostringstream str;
      str << T_("Invalid configuration data (") << value << T_(")")
	  << T_(" for value ") << lpszValueName << T_(".");
      FATAL_MIKTEX_ERROR (T_("SessionImpl::GetConfigValue"),
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
SessionImpl::SetUserConfigValue (/*[in]*/ const MIKTEXCHAR * lpszSectionName,
				 /*[in]*/ const MIKTEXCHAR * lpszValueName,
				 /*[in]*/ const MIKTEXCHAR * lpszValue)
{
#if defined(MIKTEX_WINDOWS)
  winRegistry::SetRegistryValue (TriState::False,
				 lpszSectionName,
				 lpszValueName,
				 lpszValue);
  tstring newValue;
  if (GetSessionValue(lpszSectionName, lpszValueName, newValue, 0))
    {
      if (newValue != lpszValue)
	{
	  FATAL_MIKTEX_ERROR
	    (T_("SessionImpl::SetUserConfigValue"),
	     T_("The configuration value could not be changed. Possibly an \
environment variable definition is in the way."),
	     lpszValueName);
	}
    }
#else
  UNUSED_ALWAYS (lpszSectionName);
  trace_error->WriteFormattedLine (T_("core"),
				   T_("cannot set %s to %s"),
				   lpszValueName,
				   lpszValue);
#  warning Unimplemented: SessionImpl::SetUserConfigValue()
  UNIMPLEMENTED (T_("SessionImpl::SetUserConfigValue"));
#endif
}

/* _________________________________________________________________________

   SessionImpl::SetUserConfigValue

   Set a configuration parameter.
   _________________________________________________________________________ */

void
SessionImpl::SetUserConfigValue (/*[in]*/ const MIKTEXCHAR * lpszSectionName,
				 /*[in]*/ const MIKTEXCHAR * lpszValueName,
				 /*[in]*/ bool			value)
{
  SetUserConfigValue (lpszSectionName,
		      lpszValueName,
		      value ? T_("t") : T_("f"));
}

/* _________________________________________________________________________

   SessionImpl::SetUserConfigValue

   Set a configuration parameter.
   _________________________________________________________________________ */

void
SessionImpl::SetUserConfigValue (/*[in]*/ const MIKTEXCHAR * lpszSectionName,
				 /*[in]*/ const MIKTEXCHAR * lpszValueName,
				 /*[in]*/ int			value)
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
      UNIMPLEMENTED (T_("SessionImpl::SharedMiKTeXSetup"));
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
	  tstring value;
	  if (winRegistry::TryGetRegistryValue(TriState::True,
					       MIKTEX_REGKEY_CORE,
					       MIKTEX_REGVAL_SHARED_SETUP,
					       value,
					       0))
	    {
	      sharedSetup =
		((value == T_("0")) ? TriState::False : TriState::True);
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
  pathIn.AppendExtension (T_(".in"));
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
  tstring name;
  while (streamIn.Read(&chr, 1) == 1)
    {
      if (chr == '@')
	{
	  if (readingName)
	    {
	      tstring value;
	      readingName = false;
	      if (name == MIKTEX_ENV_INSTALL)
		{
		  value = GetSpecialPath(SpecialPath::InstallRoot).Get();
		}
	      else if (name == T_("MIKTEX_CONFIG"))
		{
		  value = GetSpecialPath(SpecialPath::ConfigRoot).Get();
		}
	      else if (name == T_("MIKTEX_DATA"))
		{
		  value = GetSpecialPath(SpecialPath::DataRoot).Get();
		}
	      else
		{
		  FATAL_MIKTEX_ERROR (T_("SessionImpl::ConfigureFile"),
				      T_("Unknown variable."),
				      name.c_str());
		}
	      streamOut.Write (value.c_str(), value.length());
	    }
	  else
	    {
	      readingName = true;
	      name = T_("");
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

MIKTEXAPI(MIKTEXCHAR *)
miktex_get_config_value (/*[in]*/ const MIKTEXCHAR *	lpszSectionName,
			 /*[in]*/ const MIKTEXCHAR *	lpszValueName,
			 /*[out]*/ MIKTEXCHAR *		lpszBuf,
			 /*[in]*/ size_t		bufSize,
			 /*[in]*/ const MIKTEXCHAR *	lpszDefaultValue)
{
  C_FUNC_BEGIN ();
  tstring value;
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
