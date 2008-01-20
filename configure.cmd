@echo off

setlocal

set major=
set minor=
set j2000=
set state=
set num=

set find=%ComSpec:cmd.exe=find.exe%

if not exist "%find%" (
  echo find.exe could not be found!
  exit /B 1
)

mkdir tmp

echo s/AC_INIT(\[.*\],\[\([0-9]\+\)\.\([0-9]\+\)\.\([0-9]\+\)\(.*\)\])/\1/ > tmp\getmaj
echo s/AC_INIT(\[.*\],\[\([0-9]\+\)\.\([0-9]\+\)\.\([0-9]\+\)\(.*\)\])/\2/ > tmp\getmin
echo s/AC_INIT(\[.*\],\[\([0-9]\+\)\.\([0-9]\+\)\.\([0-9]\+\)\(.*\)\])/\3/ > tmp\getj2000
echo s/AC_INIT(\[.*\],\[\([0-9]\+\)\.\([0-9]\+\)\.\([0-9]\+\)-\([a-z]\+\)-\([0-9]\+\)\])/\5/ > tmp\getnum

for /F "usebackq delims=!" %%x in (`%%find%% "AC_INIT" ^< configure.ac`) do (
   for /F "usebackq" %%y in (`echo %%x ^| sed -f tmp\getmaj`) do (
     set major=%%y
   )
   for /F "usebackq" %%y in (`echo %%x ^| sed -f tmp\getmin`) do (
     set minor=%%y
   )
   for /F "usebackq" %%y in (`echo %%x ^| sed -f tmp\getj2000`) do (
     set j2000=%%y
   )
   for /F "usebackq" %%y in (`echo %%x ^| %%find%% "-rc"`) do (
     set state=1
     for /F "usebackq" %%z in (`echo %%x ^| sed -f tmp\getnum`) do (
       set num=%%z
     )
   )
   for /F "usebackq" %%y in (`echo %%x ^| %%find%% "-beta-"`) do (
     set state=2
     for /F "usebackq" %%z in (`echo %%x ^| sed -f tmp\getnum`) do (
       set num=%%z
     )
   )
   for /F "usebackq" %%y in (`echo %%x ^| %%find%% "-snapshot"`) do (
     set state=4
   )
)

if not defined state set state=0
if not defined num set num=0

if "%state%" == "0" set verstr=%major%.%minor%
if "%state%" == "1" set verstr=%major%.%minor% RC %num%
if "%state%" == "2" set verstr=%major%.%minor% Beta %num%
if "%state%" == "4" set verstr=%major%.%minor%.%j2000% snapshot

echo ## created by configure.cmd > msvc\config.inc
echo MIKTEX_MAJOR_VERSION=%major% >> msvc\config.inc
echo MIKTEX_MINOR_VERSION=%minor% >> msvc\config.inc
echo MIKTEX_J2000_VERSION=%j2000% >> msvc\config.inc
echo MIKTEX_RELEASE_STATE=%state% >> msvc\config.inc
echo MIKTEX_RELEASE_NUM=%num% >> msvc\config.inc
echo MIKTEX_VERSION_STR=%verstr% >> msvc\config.inc
echo miktexseries=%major%.%minor% >> msvc\config.inc
echo miktexsrcdir=%CD% >> msvc\config.inc

echo s/@PACKAGE_NAME@/MiKTeX/ > tmp\scriptfile
echo s/@MIKTEX_MAJOR_VERSION@/%major%/ >> tmp\scriptfile
echo s/@MIKTEX_MINOR_VERSION@/%minor%/ >> tmp\scriptfile
echo s/@MIKTEX_J2000_VERSION@/%j2000%/ >> tmp\scriptfile
echo s/@MIKTEX_RELEASE_STATE@/%state%/ >> tmp\scriptfile
echo s/@MIKTEX_RELEASE_NUMBER@/%num%/ >> tmp\scriptfile
echo s/@MIKTEX_VERSION_STR@/%verstr%/ >> tmp\scriptfile
echo s/@MIKTEX_SERIES_STR@/%major%.%minor%/ >> tmp\scriptfile
echo s;@top_srcdir@;%CD:\=/%; >> tmp\scriptfile
for /F "usebackq delims=" %%d in (`bash -c date`) do (
  echo s;@MIKTEX_DATETIME_STR@;%%d; >> tmp\scriptfile
)

sed -f tmp\scriptfile < Doxyfile.in > Doxyfile
sed -f tmp\scriptfile < Libraries\MiKTeX\Core\include\miktex\version.h.in > Libraries\MiKTeX\Core\include\miktex\version.h

sed -f tmp\scriptfile < Documentation\version.ent.in > Documentation\version.ent
sed -f tmp\scriptfile < Documentation\Styles\db2latex-common.xsl.in > Documentation\Styles\db2latex-common.xsl

sed -f tmp\scriptfile < BuildUtilities\docbook\catalog.in > BuildUtilities\docbook\catalog

setlocal enabledelayedexpansion
for %%f in (Admin\TPM\*.tpm.in) do (
  set tpmfile=%%~ff
  sed -f tmp\scriptfile < %%f > "!tpmfile:.tpm.in=-%major%.%minor%.tpm!"
)
endlocal

rmdir /s /q tmp

set INCLUDE=%CD%\msvc;%INCLUDE%

nmake -nologo -f miktex.mak make-miktexstartup-ini 1>nul
nmake -nologo -f miktex.mak make-setenv-cmd 1>nul
nmake -nologo -f miktex.mak init-texmf 1>nul

endlocal
