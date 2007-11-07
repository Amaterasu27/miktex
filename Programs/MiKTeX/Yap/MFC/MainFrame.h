/* MainFrame.h:							-*- C++ -*-

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

#pragma once

#define YAP_WND_CLASS "MiKTeX_yap"

#include "DviStatusBar.h"

class MainFrame : public CMDIFrameWnd
{
private:
  DECLARE_DYNAMIC(MainFrame);

protected:
  DECLARE_MESSAGE_MAP();

public:
  MainFrame();

public:
  virtual
  BOOL
  PreCreateWindow (/*[in,out]*/ CREATESTRUCT & cs);
  
protected:
  afx_msg
  int
  OnCreate(/*[in]*/ LPCREATESTRUCT pCreateStruct);
  
protected:
  afx_msg
  void
  OnCancel ();
  
protected:
  afx_msg
  void
  OnClose();
  
protected:
  afx_msg
  void
  OnContinuousDouble ();
  
protected:
  afx_msg
  void
  OnContinuousSingle ();
  
protected:
  afx_msg
  void
  OnDoublePage ();
  
protected:
  afx_msg
  void
  OnGetMinMaxInfo (/*[in]*/ MINMAXINFO * pMMI);
  
protected:
  afx_msg
  void
  OnHelpKeyboard ();
  
protected:
  afx_msg
  void
  OnSinglePage ();
  
protected:
  afx_msg
  void
  OnUpdateCommandPrefix (/*[in]*/ CCmdUI * pCmdUI);
  
protected:
  afx_msg
  void
  OnUpdateContinuousDouble (/*[in]*/ CCmdUI * pCmdUI);
  
protected:
  afx_msg
  void
  OnUpdateContinuousSingle (/*[in]*/ CCmdUI * pCmdUI);
  
protected:
  afx_msg
  void
  OnUpdateDoublePage (/*[in]*/ CCmdUI * pCmdUI);
  
protected:
  afx_msg
  void
  OnUpdatePageMofN (/*[in]*/ CCmdUI * pCmdUI);
  
protected:
  afx_msg
  void
  OnUpdatePoint (/*[in]*/ CCmdUI * pCmdUI);
  
protected:
  afx_msg
  void
  OnUpdateEffectivePageMode (/*[in]*/ CCmdUI * pCmdUI);
  
protected:
  afx_msg
  void
  OnUpdateSinglePage (/*[in]*/ CCmdUI * pCmdUI);
  
protected:
  afx_msg
  void
  OnUpdateSource (/*[in]*/ CCmdUI * pCmdUI);
  
protected:
  afx_msg
  void
  OnUpdateViewFullScreen (/*[in]*/ CCmdUI * pCmdUI);
  
protected:
  afx_msg
  void
  OnViewFullScreen ();
  
protected:
  afx_msg
  void
  OnViewOptions ();

protected:
  afx_msg
  void
  OnViewCentimeters ();

protected:
  afx_msg
  void
  OnUpdateViewCentimeters (/*[in]*/ CCmdUI *pCmdUI);

protected:
  afx_msg
  void
  OnViewInches ();

protected:
  afx_msg
  void
  OnUpdateViewInches (/*[in]*/ CCmdUI *pCmdUI);

protected:
  afx_msg
  void
  OnViewMillimeters ();

protected:
  afx_msg
  void
  OnUpdateViewMillimeters (/*[in]*/ CCmdUI *pCmdUI);

protected:
  afx_msg
  void
  OnViewPicas ();

protected:
  afx_msg
  void
  OnUpdateViewPicas (/*[in]*/ CCmdUI *pCmdUI);

protected:
  afx_msg
  void
  OnViewPoints ();

protected:
  afx_msg
  void
  OnUpdateViewPoints (/*[in]*/ CCmdUI *pCmdUI);

public:
  CStatusBar &
  GetStatusBar ()
  {
    return (statusBar);
  }

public:
  bool
  AddCommandPrefixChar (/*[in]*/ char ch);

public:
  CString
  GetCommandPrefix (/*[in]*/ bool clear = false);

public:
  bool
  IsFullScreen ()
    const
  {
    return (isFullScreen);
  }

protected:
  DviStatusBar statusBar;

protected:
  CToolBar toolBar;

private:
  CString commandPrefix;

private:
  bool isFullScreen;

private:
  CRect fullScreenWindowRect;

private:
  WINDOWPLACEMENT previousWindowPlacement;

private:
  CToolBar * pFullScreenBar;
};
