/* InverseSearchOptionsPage.cpp:

   Copyright (C) 1996-2006 Christian Schenk

   This file is part of Yap.

   Yap is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.
   
   Yap is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
   or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
   License for more details.
   
   You should have received a copy of the GNU General Public License
   along with Yap; if not, write to the Free Software Foundation, 59
   Temple Place - Suite 330, Boston, MA 02111-1307, USA. */

/* todo:

   Free
   ====

   texshell /l=%l %f
   "C:\Program Files\LEd\LEd.exe" %f:%l
   "C:\Program Files\wscite\SciTE.exe" "%f" -goto:%l

   Non-free
   ========

*/

#include "StdAfx.h"

#include "yap.h"

#include "InverseSearchOptionsPage.h"

#include "AddChangeEditorDialog.h"

/* _________________________________________________________________________

   LocateProgramFilesDir
   _________________________________________________________________________ */

bool
LocateProgramFilesDir (/*[out]*/ PathName &	path)
{
  HKEY hkey;
  if (RegOpenKey(HKEY_LOCAL_MACHINE,
		 T_("SOFTWARE\\Microsoft\\Windows\\CurrentVersion"),
		 &hkey)
      != ERROR_SUCCESS)
    {
      return (false);
    }
  AutoHKEY autoHKEY (hkey);
  unsigned long size =
    (static_cast<unsigned long>(path.GetCapacity()) * sizeof(char));
  long res =
    RegQueryValueEx(hkey,
		    T_("ProgramFilesDir"),
		    0,
		    0,
		    reinterpret_cast<LPBYTE>(path.GetBuffer()),
		    &size);
  if (res != ERROR_SUCCESS)
    {
      return (false);
    }
  return (true);
}

/* _________________________________________________________________________

   LocateNTEmacs
   _________________________________________________________________________ */

bool
LocateNTEmacs (/*[out]*/ PathName &		ntEmacs,
	       /*[in]*/ const char *	lpszName)
{
  HKEY hkey;
  long res =
    RegOpenKeyEx(HKEY_LOCAL_MACHINE,
		 T_("SOFTWARE\\GNU\\Emacs"),
		 0,
		 KEY_READ,
		 &hkey);
  if (res != ERROR_SUCCESS)
    {
      return (false);
    }
  AutoHKEY autoHKEY (hkey);
  unsigned long type;
  PathName emacsDir;
  unsigned long size =
    (static_cast<unsigned long>(emacsDir.GetCapacity()) * sizeof(char));
  res =
    RegQueryValueEx(hkey,
		    T_("emacs_dir"),
		    0,
		    &type,
		    reinterpret_cast<LPBYTE>(emacsDir.GetBuffer()),
		    &size);
  if (res != ERROR_SUCCESS || type != REG_SZ)
    {
      return (false);
    }
  PathName pathBinDir (emacsDir, T_("bin"));
  ntEmacs = pathBinDir;
  ntEmacs += lpszName;
  ntEmacs.SetExtension (T_(".exe"));
  return (File::Exists(ntEmacs));
}

/* _________________________________________________________________________

   MakeNTEmacsCommandLine
   _________________________________________________________________________ */

bool
MakeNTEmacsCommandLine (/*[out]*/ string &	program,
		        /*[out]*/ string &	arguments)
{
  PathName pathEmacs;
  if (! LocateNTEmacs(pathEmacs, T_("runemacs.exe"))
      && ! LocateNTEmacs(pathEmacs, T_("emacs.exe")))
    {
      return (false);
    }
  program = pathEmacs.Get();
  arguments = T_("+%l \"%f\"");
  return (true);
}

/* _________________________________________________________________________

   MakeNTEmacsClientCommandLine
   _________________________________________________________________________ */

bool
MakeNTEmacsClientCommandLine (/*[out]*/ string &	program,
			      /*[out]*/ string &	arguments)
{
  PathName pathEmacs;
  char * lpszFileName;
  if (! (LocateNTEmacs(pathEmacs, T_("gnuclientw.exe"))
	 || SearchPath(0,
		       T_("gnuclientw.exe"),
		       0,
		       static_cast<unsigned long>(pathEmacs.GetCapacity()),
		       pathEmacs.GetBuffer(),
		       &lpszFileName)))
    {
      return (false);
    }
  program = pathEmacs.Get();
  arguments = T_("-F +%l \"%f\"");
  return (true);
}

/* _________________________________________________________________________

   MakeXEmacsCommandLine
   _________________________________________________________________________ */

bool
MakeXEmacsCommandLine (/*[out]*/ string &	program,
		       /*[out]*/ string &	arguments)
{
  HKEY hkey;
  long res =
    RegOpenKeyEx(HKEY_LOCAL_MACHINE,
		 (T_("SOFTWARE\\Microsoft\\Windows\\CurrentVersion")
		  T_("\\App Paths\\xemacs.exe")),
		 0,
		 KEY_READ,
		 &hkey);
  if (res != ERROR_SUCCESS)
    {
      return (false);
    }
  AutoHKEY autoHKEY (hkey);
  unsigned long type;
  PathName pathEmacsDir;
  unsigned long size =
    (static_cast<unsigned long>(pathEmacsDir.GetCapacity() * sizeof(char)));
  res =
    RegQueryValueEx(hkey,
		    T_("Path"),
		    0,
		    &type,
		    reinterpret_cast<LPBYTE>
		    (pathEmacsDir.GetBuffer()),
		    &size);
  if (res != ERROR_SUCCESS || type != REG_SZ)
    {
      return (false);
    }
  PathName pathEmacs (pathEmacsDir, T_("runemacs.exe"));
  if (! File::Exists(pathEmacs))
    {
      pathEmacs.Set (pathEmacsDir, T_("xemacs.exe"));
      if (! File::Exists(pathEmacs))
	{
	  return (false);
	}
    }
  program = pathEmacs.Get();
  arguments = T_("+%l \"%f\"");
  return (true);
}

/* _________________________________________________________________________

   LocateTeXnicCenter
   _________________________________________________________________________ */

bool
LocateTeXnicCenter (/*[out]*/ PathName & tc)
{
  HKEY hkey;
  long res =
    RegOpenKeyEx(HKEY_LOCAL_MACHINE,
		 T_("SOFTWARE\\ToolsCenter\\TeXnicCenter"),
		 0,
		 KEY_READ,
		 &hkey);
  if (res != ERROR_SUCCESS)
    {
      return (false);
    }
  AutoHKEY autoHKEY (hkey);
  unsigned long type;
  PathName tcRoot;
  unsigned long size =
    (static_cast<unsigned long>(tcRoot.GetCapacity()) * sizeof(char));
  res =
    RegQueryValueEx(hkey,
		    T_("AppPath"),
		    0,
		    &type,
		    reinterpret_cast<LPBYTE>(tcRoot.GetBuffer()),
		    &size);
  if (res == ERROR_SUCCESS && type == REG_SZ)
    {
      tc.Set (tcRoot, T_("TEXCNTR.EXE"));
      if (File::Exists(tc))
	{
	  return (true);
	}
    }
  return (false);
}

/* _________________________________________________________________________

   MakeTeXnicCenterCommandLine
   _________________________________________________________________________ */

bool
MakeTeXnicCenterCommandLine (/*[out]*/ string &	program,
			     /*[out]*/ string &	arguments)
{
  PathName tc;
  if (! LocateTeXnicCenter(tc))
    {
      return (false);
    }
  program = tc.Get();
  arguments = T_("/ddecmd \"[goto('%f', '%l')]\"");
  return (true);
}

/* _________________________________________________________________________

   LocateVisualTeX
   _________________________________________________________________________ */

bool
LocateVisualTeX (/*[out]*/ PathName & path)
{
  PathName pathProgramFiles;
  if (! LocateProgramFilesDir(pathProgramFiles))
    {
      return (false);
    }
  path = pathProgramFiles;
  path += T_("Visual-TeX");
  path += T_("vtex.exe");
  return (File::Exists(path));
}

/* _________________________________________________________________________

   MakeVisualTeXCommandLine
   _________________________________________________________________________ */

bool
MakeVisualTeXCommandLine (/*[out]*/ string &	program,
			  /*[out]*/ string &	arguments)
{
  PathName path;
  if (! LocateVisualTeX(path))
    {
      return (false);
    }
  program = path.Get();
  arguments = T_("\"%f\" /line:%l");
  return (true);
}

/* _________________________________________________________________________

   LocateWinEdt
   _________________________________________________________________________ */

bool
LocateWinEdt (/*[out]*/ PathName & winEdt)
{
  HKEY hkey;
  long res =
    RegOpenKeyEx(HKEY_CURRENT_USER,
		 T_("Software\\WinEdt"),
		 0,
		 KEY_READ,
		 &hkey);
  if (res == ERROR_SUCCESS)
    {
      AutoHKEY autoHKEY (hkey);
      unsigned long type;
      PathName pathWinEdtDir;
      unsigned long size =
	(static_cast<unsigned long>(pathWinEdtDir.GetCapacity())
	 * sizeof(char));
      res =
	RegQueryValueEx(hkey,
			T_("Install Root"),
			0,
			&type,
			reinterpret_cast<LPBYTE>
			(pathWinEdtDir.GetBuffer()),
			&size);
      if (res == ERROR_SUCCESS && type == REG_SZ)
	{
	  winEdt = pathWinEdtDir;
	  winEdt += T_("winedt.exe");
	  if (File::Exists(winEdt))
	    {
	      return (true);
	    }
	}
    }
  res =
    RegOpenKeyEx(HKEY_LOCAL_MACHINE,
		 (T_("SOFTWARE\\Microsoft\\Windows\\CurrentVersion")
		  T_("\\App Paths\\WinEdt.exe")),
		 0,
		 KEY_READ,
		 &hkey);
  if (res != ERROR_SUCCESS)
    {
      return (false);
    }
  AutoHKEY autoHKEY (hkey);
  unsigned long type;
  unsigned long size =
    (static_cast<unsigned long>(winEdt.GetCapacity()) * sizeof(char));
  res =
    RegQueryValueEx(hkey,
		    0,
		    0,
		    &type,
		    reinterpret_cast<LPBYTE>(winEdt.GetBuffer()),
		    &size);
  if (res == ERROR_SUCCESS && type == REG_SZ)
    {
      return (true);
    }
  return (false);
}

/* _________________________________________________________________________

   MakeWinEdtCommandLine
   _________________________________________________________________________ */

bool
MakeWinEdtCommandLine (/*[out]*/ string &	program,
		       /*[out]*/ string &	arguments)
{
  PathName pathWinEdt;
  if (! LocateWinEdt(pathWinEdt))
    {
      return (false);
    }
  program = pathWinEdt.Get();
  arguments = T_("\"[Open(|%f|);SelPar(%l,8)]\"");
  return (true);
}

/* _________________________________________________________________________

   LocateLaTeXMng
   _________________________________________________________________________ */

bool
LocateLaTeXMng (/*[out]*/ PathName & path)
{
  PathName pathProgramFiles;
  if (! LocateProgramFilesDir(pathProgramFiles))
    {
      return (false);
    }
  path = pathProgramFiles;
  path += T_("LaTeXMng");
  path += T_("LaTeXMng.exe");
  return (File::Exists(path));
}

/* _________________________________________________________________________

   MakeLaTeXMngCommandLine
   _________________________________________________________________________ */

bool
MakeLaTeXMngCommandLine (/*[out]*/ string &	program,
			 /*[out]*/ string &	arguments)
{
  PathName path;
  if (! LocateLaTeXMng(path))
    {
      return (false);
    }
  program = path.Get();

  //latexmng -l%l %f


  arguments = T_("-l%l \"%f\"");
  return (true);
}

/* _________________________________________________________________________

   LocateWinTeXXP
   _________________________________________________________________________ */

bool
LocateWinTeXXP (/*[out]*/ PathName & path)
{
  PathName pathProgramFiles;
  if (! LocateProgramFilesDir(pathProgramFiles))
    {
      return (false);
    }
  path = pathProgramFiles;
  path += T_("WinTeX");
  path += T_("wintex.exe");
  return (File::Exists(path));
}

/* _________________________________________________________________________

   MakeWinTeXXPCommandLine
   _________________________________________________________________________ */

bool
MakeWinTeXXPCommandLine (/*[out]*/ string &	program,
			  /*[out]*/ string &	arguments)
{
  PathName path;
  if (! LocateWinTeXXP(path))
    {
      return (false);
    }
  program = path.Get();
  arguments = T_("-f \"%f\" -l %l");
  return (true);
}

/* _________________________________________________________________________

   LocateWinShell
   _________________________________________________________________________ */

bool
LocateWinShell (/*[out]*/ PathName & path)
{
  PathName pathProgramFiles;
  if (! LocateProgramFilesDir(pathProgramFiles))
    {
      return (false);
    }
  path = pathProgramFiles;
  path += T_("WinShell");
  path += T_("WinShell.exe");
  return (File::Exists(path));
}

/* _________________________________________________________________________

   MakeWinShellCommandLine
   _________________________________________________________________________ */

bool
MakeWinShellCommandLine (/*[out]*/ string &	program,
			 /*[out]*/ string &	arguments)
{
  PathName path;
  if (! LocateWinShell(path))
    {
      return (false);
    }
  program = path.Get();
  arguments = T_("-c \"%f\" -l %l");
  return (true);
}

/* _________________________________________________________________________

   LocateLaTeXWIDE
   _________________________________________________________________________ */

bool
LocateLaTeXWIDE (/*[out]*/ PathName & path)
{
  PathName pathProgramFiles;
  if (! LocateProgramFilesDir(pathProgramFiles))
    {
      return (false);
    }
  path = pathProgramFiles;
  path += T_("LaTeX WIDE");
  path += T_("LWide.exe");
  return (File::Exists(path));
}

/* _________________________________________________________________________

   MakeLaTeXWIDECommandLine
   _________________________________________________________________________ */

bool
MakeLaTeXWIDECommandLine (/*[out]*/ string &	program,
			  /*[out]*/ string &	arguments)
{
  PathName path;
  if (! LocateLaTeXWIDE(path))
    {
      return (false);
    }
  program = path.Get();
  arguments = T_("\"%f\" /l%l");
  return (true);
}

/* _________________________________________________________________________

   InverseSearchOptionsPage Message Map
   _________________________________________________________________________ */

BEGIN_MESSAGE_MAP(InverseSearchOptionsPage, CPropertyPage)
  ON_CBN_SELCHANGE(IDC_LISTBOX_EDITOR, OnSelChangeEditor)
  ON_BN_CLICKED(IDC_ADD, &InverseSearchOptionsPage::OnAddEditor)
  ON_BN_CLICKED(IDC_DELETE, &InverseSearchOptionsPage::OnDeleteEditor)
  ON_BN_CLICKED(IDC_CHANGE, &InverseSearchOptionsPage::OnChangeEditor)
END_MESSAGE_MAP();

/* _________________________________________________________________________

   InverseSearchOptionsPage::InverseSearchOptionsPage
   _________________________________________________________________________ */

InverseSearchOptionsPage::InverseSearchOptionsPage ()
  : CPropertyPage (IDD),
    firstCustomIdx (-1),
    commandLineIdx (-1),
    mustWrite (false)
{
}

/* _________________________________________________________________________

   InverseSearchOptionsPage::OnInitDialog
   _________________________________________________________________________ */

BOOL
InverseSearchOptionsPage::OnInitDialog ()
{
  BOOL ret = CPropertyPage::OnInitDialog();

  try
    {
      CollectEditors ();

      int idx = 0;

      for (vector<EditorInfo>::const_iterator it = editors.begin();
	   it != editors.end();
	   ++ it, ++ idx)
	{
	  if (g_pYapConfig->inverseSearchCommandLine
	      == editors[idx].GetCommandLine().c_str())
	    {
	      commandLineIdx = idx;
	    }
	  CString str = it->name.c_str();
	  if (idx < firstCustomIdx)
	    {
	      str += T_(" (auto-detected)");	      
	    }
	  if (editorListBox.AddString(str) < 0)
	    {
	      UNEXPECTED_CONDITION
		(T_("InverseSearchOptionsPage::OnInitDialog"));
	    }
	}

      if (commandLineIdx >= 0)
	{
	  if (editorListBox.SetCurSel(commandLineIdx) < 0)
	    {
	      UNEXPECTED_CONDITION
		(T_("InverseSearchOptionsPage::OnInitDialog"));
	    }
	  commandLineDisplay.SetWindowText
	    (editors[commandLineIdx].GetCommandLine().c_str());
	}
      
      EnableButtons ();
    }

  catch (const MiKTeXException & e)
    {
      ErrorDialog::DoModal (0, e);
    }

  catch (const exception & e)
    {
      ErrorDialog::DoModal (0, e);
    }

  return (ret);
}

/* _________________________________________________________________________

   InverseSearchOptionsPage::DoDataExchange
   _________________________________________________________________________ */

void
InverseSearchOptionsPage::DoDataExchange (/*[in,out]*/ CDataExchange * pDX)
{
  CPropertyPage::DoDataExchange (pDX);
  DDX_LBIndex (pDX, IDC_LISTBOX_EDITOR, commandLineIdx);
  DDX_Control (pDX, IDC_LISTBOX_EDITOR, editorListBox);
  DDX_Control (pDX, IDC_COMMAND_LINE, commandLineDisplay);
  DDX_Control (pDX, IDC_DELETE, deleteButton);
  DDX_Control (pDX, IDC_CHANGE, changeButton);
}

/* _________________________________________________________________________

   InverseSearchOptionsPage::OnSelChangeEditor
   _________________________________________________________________________ */

void
InverseSearchOptionsPage::OnSelChangeEditor ()
{
  try
    {
      int idx = editorListBox.GetCurSel();
      if (idx >= static_cast<int>(editors.size()))
	{
	  UNEXPECTED_CONDITION
	    (T_("InverseSearchOptionsPage::OnSelChangeEditor"));
	}
      commandLineIdx = idx;
      if (idx >= 0)
	{
	  commandLineDisplay.SetWindowText
	    (editors[idx].GetCommandLine().c_str());
	}
      else
	{
	  commandLineDisplay.SetWindowText (T_(""));
	}
      SetModified (TRUE);
      EnableButtons ();
    }
  catch (const MiKTeXException & e)
    {
      ErrorDialog::DoModal (this, e);
    }
  catch (const exception & e)
    {
      ErrorDialog::DoModal (this, e);
    }
}

/* _________________________________________________________________________

   InverseSearchOptionsPage::OnAddEditor
   _________________________________________________________________________ */

void
InverseSearchOptionsPage::OnAddEditor ()
{
  try
    {
      AddChangeEditorDialog dlg (this, editors, firstCustomIdx);
      if (dlg.DoModal() == IDOK)
	{
	  EditorInfo editorInfo = dlg.GetEditorInfo();
	  editors.push_back (editorInfo);
	  if (editorListBox.AddString(editorInfo.name.c_str()) < 0)
	    {
	      UNEXPECTED_CONDITION
		(T_("InverseSearchOptionsPage::OnAddEditor"));
	    }
	  mustWrite = true;
	}
    }
  catch (const MiKTeXException & e)
    {
      ErrorDialog::DoModal (this, e);
    }
  catch (const exception & e)
    {
      ErrorDialog::DoModal (this, e);
    }
}

/* _________________________________________________________________________

   InverseSearchOptionsPage::OnChangeEditor
   _________________________________________________________________________ */

void
InverseSearchOptionsPage::OnChangeEditor ()
{
  try
    {
      AddChangeEditorDialog
	dlg (this, editors, firstCustomIdx, commandLineIdx);
      if (dlg.DoModal() == IDOK)
	{
	  editors[commandLineIdx] = dlg.GetEditorInfo();
	  OnSelChangeEditor ();
	  mustWrite = true;
	}
    }
  catch (const MiKTeXException & e)
    {
      ErrorDialog::DoModal (this, e);
    }
  catch (const exception & e)
    {
      ErrorDialog::DoModal (this, e);
    }
}

/* _________________________________________________________________________

   InverseSearchOptionsPage::OnDeleteEditor
   _________________________________________________________________________ */

void
InverseSearchOptionsPage::OnDeleteEditor ()
{
  try
    {
      if (editorListBox.DeleteString(commandLineIdx) < 0)
	{
	  UNEXPECTED_CONDITION
	    (T_("InverseSearchOptionsPage::OnDeleteEditor"));
	}
      editors.erase (editors.begin() + commandLineIdx);
      commandLineIdx = -1;
      OnSelChangeEditor ();
      mustWrite = true;
    }
  catch (const MiKTeXException & e)
    {
      ErrorDialog::DoModal (this, e);
    }
  catch (const exception & e)
    {
      ErrorDialog::DoModal (this, e);
    }
}

/* _________________________________________________________________________

   InverseSearchOptionsPage::OnApply
   _________________________________________________________________________ */

BOOL
InverseSearchOptionsPage::OnApply ()
{
  try
    {
      PathName pathEditorsIni
	(SessionWrapper(true)->GetSpecialPath(SpecialPath::ConfigRoot),
	 MIKTEX_PATH_EDITORS_INI);
      if (mustWrite)
	{
	  SmartPointer<Cfg> pCfg (Cfg::Create());
	  for (int idx = firstCustomIdx;
	       idx < static_cast<int>(editors.size());
	       ++ idx)
	    {
	      pCfg->PutValue (editors[idx].name.c_str(),
			      T_("program"),
			      editors[idx].program.c_str());
	      pCfg->PutValue (editors[idx].name.c_str(),
			      T_("arguments"),
			      editors[idx].arguments.c_str());
	    }
	  Directory::Create (PathName(pathEditorsIni).RemoveFileSpec());
	  pCfg->Write (pathEditorsIni);
	  mustWrite = false;
	  pCfg.Release ();
	  if (! Fndb::FileExists(pathEditorsIni))
	    {
	      Fndb::Add (pathEditorsIni);
	    }
	}
      if (commandLineIdx >= 0)
	{
	  g_pYapConfig->inverseSearchCommandLine =
	    editors[commandLineIdx].GetCommandLine().c_str();
	}
      else
	{
	  g_pYapConfig->inverseSearchCommandLine = T_("");
	}
      return (CPropertyPage::OnApply());
    }
  catch (const MiKTeXException & e)
    {
      ErrorDialog::DoModal (this, e);
      return (FALSE);
    }
  catch (const exception & e)
    {
      ErrorDialog::DoModal (this, e);
      return (FALSE);
    }
}

/* _________________________________________________________________________

   InverseSearchOptionsPage::EnableButtons
   _________________________________________________________________________ */

void
InverseSearchOptionsPage::EnableButtons ()
{
  deleteButton.EnableWindow
    (commandLineIdx >= 0 && commandLineIdx >= firstCustomIdx);
  changeButton.EnableWindow
    (commandLineIdx >= 0 && commandLineIdx >= firstCustomIdx);
}

/* _________________________________________________________________________

   InverseSearchOptionsPage::CollectEditors
   _________________________________________________________________________ */

void
InverseSearchOptionsPage::CollectEditors ()
{
  string program;
  string arguments;

  /// free editors

  if (MakeXEmacsCommandLine(program, arguments))
    {
      editors.push_back (EditorInfo(T_("XEmacs"), program, arguments));
    }
  
  if (MakeNTEmacsCommandLine(program, arguments))
    {
      editors.push_back (EditorInfo(T_("NTEmacs"), program, arguments));
    }

  if (MakeNTEmacsClientCommandLine(program, arguments))
    {
      editors.push_back (EditorInfo(T_("NTEmacs (Single Instance)"),
				    program,
				    arguments));
    }

  if (MakeTeXnicCenterCommandLine(program, arguments))
    {
      editors.push_back (EditorInfo(T_("TeXnicCenter"), program, arguments));
    }
  
  if (MakeVisualTeXCommandLine(program, arguments))
    {
      editors.push_back (EditorInfo(T_("Visual TeX"), program, arguments));
    }
  
  /// non-free editors

  if (MakeWinEdtCommandLine(program, arguments))
    {
      editors.push_back (EditorInfo(T_("WinEdt"), program, arguments));
    }

  if (MakeLaTeXMngCommandLine(program, arguments))
    {
      editors.push_back (EditorInfo(T_("LaTeXMng"), program, arguments));
    }

  if (MakeWinTeXXPCommandLine(program, arguments))
    {
      editors.push_back (EditorInfo(T_("WinTeX XP"), program, arguments));
    }

  if (MakeWinShellCommandLine(program, arguments))
    {
      editors.push_back (EditorInfo(T_("WinShell"), program, arguments));
    }

  if (MakeLaTeXWIDECommandLine(program, arguments))
    {
      editors.push_back (EditorInfo(T_("LaTeX WIDE"), program, arguments));
    }
  
  /// user-defined

  firstCustomIdx = static_cast<int>(editors.size());
  
  for (unsigned r = SessionWrapper(true)->GetNumberOfTEXMFRoots();
       r > 0;
       -- r)
    {
      PathName editorsIni = SessionWrapper(true)->GetRootDirectory(r - 1);
      editorsIni += MIKTEX_PATH_EDITORS_INI;
      if (File::Exists(editorsIni))
	{
	  SmartPointer<Cfg> pCfg (Cfg::Create());
	  pCfg->Read (editorsIni);
	  char szName[BufferSizes::MaxCfgName];
	  for (char * lpszName =
		 pCfg->FirstKey(szName, BufferSizes::MaxCfgName);
	       lpszName != 0;
	       lpszName = pCfg->NextKey(szName, BufferSizes::MaxCfgName))
	    {
	      editors.push_back
		(EditorInfo(lpszName,
			    pCfg->GetValue(lpszName, T_("program")),
			    pCfg->GetValue(lpszName, T_("arguments"))));
	    }
	}
    }
}
