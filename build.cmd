:# build.cmd: build MiKTeX
:# 
:# Copyright (C) 1996-2006 Christian Schenk
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
:#
:# See file win/README.win for build instructions.
@echo off

setlocal

if "%ARCH%" == "" (
  set buildlog=build-x86.log
) else (
  set buildlog=build-%ARCH%.log
)

set TEMP=\MiKTeXTemp
set TMP=%TEMP%
if not exist "%TEMP%" mkdir "%TEMP%"

:build
if not "%1" == "--rebuild" goto build2
echo cleaning MiKTeX ...
nmake /NOLOGO /f miktex.mak clean 1>nul 2>&1

:build2
echo building MiKTeX ...
if exist %buildlog% del %buildlog%
echo -*- Compilation -*->> %buildlog%
echo %CD% %DATE% %TIME%>> %buildlog%
echo.>> %buildlog%
nmake /NOLOGO /f miktex.mak install 1>>%buildlog% 2>>&1
goto end

:usage
echo Usage: build [--rebuild]
goto end

:end

endlocal
