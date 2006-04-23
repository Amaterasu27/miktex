/* MainFrame.h:							-*- C++ -*-

   Copyright (C) 2002-2006 Christian Schenk

   This file is part of MiKTeX Package Manager.

   MiKTeX Package Manager is free software; you can redistribute it
   and/or modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2, or
   (at your option) any later version.
   
   MiKTeX Package Manager is distributed in the hope that it will be
   useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with MiKTeX Package Manager; if not, write to the Free
   Software Foundation, 59 Temple Place - Suite 330, Boston, MA
   02111-1307, USA. */

#if defined(_MSC_VER)
#  pragma once
#endif

class MainFrame : public CFrameWnd
{
protected:
  MainFrame ();

protected:
  DECLARE_DYNCREATE(MainFrame);

protected:
  virtual
  BOOL
  PreCreateWindow (/*[in,out]*/ CREATESTRUCT & cs);

public:
  virtual
  ~MainFrame ();

protected:
  CStatusBar statusBar;

protected:
  CToolBar toolBar;

protected:
  CReBar reBar;

protected:
  CDialogBar dialogBar;

public:
  CWnd *
  GetDlgBarItem (/*[in]*/ int controlId)
  {
    CWnd * pWnd = dialogBar.GetDlgItem(controlId);
    if (pWnd == 0)
      {
	FATAL_WINDOWS_ERROR (T_("MainFrame::GetDlgItem"), 0);
      }
    return (pWnd);
  }

public:
  CString
  GetDlgBarText (/*[in]*/ int controlId)
  {
    CWnd * pWnd = GetDlgBarItem(controlId);
    CString text;
    pWnd->GetWindowText (text);
    return (text);
  }

protected:
  afx_msg
  int
  OnCreate (/*[in,out]*/ LPCREATESTRUCT pCreateStruct);

public:
  DECLARE_MESSAGE_MAP();

protected:
  afx_msg
  void
  OnUpdateFilterOut (/*[in]*/ CCmdUI * pCmdUI);

protected:
  afx_msg
  void
  OnEditPaste ();

protected:
  afx_msg
  void
  OnEditCut ();

protected:
  afx_msg
  void
  OnEditCopy ();

protected:
  afx_msg
  void
  OnEditUndo ();
};
