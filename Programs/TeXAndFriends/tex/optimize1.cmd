:# optimize1.cmd: test optimized function
:# 
:# Copyright (C) 1991-2006 Christian Schenk
:# 
:# This file is free software; you can redistribute it and/or modify
:# it under the terms of the GNU General Public License as published
:# by the Free Software Foundation; either version 2, or (at your
:# option) any later version.
:# 
:# This file is distributed in the hope that it will be useful, but
:# WITHOUT ANY WARRANTY; without even the implied warranty of
:# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
:# General Public License for more details.
:# 
:# You should have received a copy of the GNU General Public License
:# along with this file; if not, write to the Free Software
:# Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307,
:# USA.
@echo off

echo testing optimized function %1...
echo testing optimized function %1...>>optimize.log

del /f WinRel\tex.obj>nul
del /f WinRel\tex.exe>nul

nmake -f miktex.mak optvars="-DC4P_OPT_%1=\\\"gty\\\" -Od -Oi -Gs" WinRel\tex.exe >>nul 2>>&1
goto test2

:test1
WinRel\tex --halt-on-error --ini plain \dump >>nul 2>>&1
if errorlevel 1 goto badfunction
goto end

:test2
WinRel\tex --job-time=tex.web --halt-on-error story \bye >>nul 2>>&1
if errorlevel 1 goto badfunction
diff story.dvi story-good.dvi
if errorlevel 1 goto badfunction
goto end

:badfunction
echo error: optimized function %1 causes problems
echo error: optimized function %1 causes problems>>optimize.log
goto end

:end
