@echo off

setlocal

if not exist configure.cmd (
  echo This script must be invoked in the top source directory.
  exit /B 1
)

set VCPROFILE_PATH=%CD%\pgo
mkdir "%VCPROFILE_PATH%"

set prodlog=%CD%\Admin\Log\produce.log

if exist "%prodlog%" del "%prodlog%"
echo -*- Compilation -*->> "%prodlog%"
echo %CD% %DATE% %TIME%>> "%prodlog%"
echo.>> "%prodlog%"

call configure.cmd 1>>"%prodlog%" 2>>&1
if errorlevel 1 goto error

call build.cmd --rebuild --pginstrument 1>>"%prodlog%" 2>>&1
if errorlevel 1 goto error

echo Training... 1>>"%prodlog%" 2>>&1
initexmf --quiet --dump=latex 1>>"%prodlog%" 2>>&1
rmdir /s /q "%VCPROFILE_PATH%"
mkdir "%VCPROFILE_PATH%"
mkdir scratch
cd scratch
latex sample2e 1>>"%prodlog%" 2>>&1
dvips sample2e 1>>"%prodlog%" 2>>&1
cd ..
rmdir /s /q scratch

del Libraries\MiKTeX\Core\Release\*.dll
del Programs\TeXAndFriends\pdfetex\Release\*.dll
del Programs\DviWare\dvips\Release\*.exe
call build.cmd --pgoptimize 1>>"%prodlog%" 2>>&1
if errorlevel 1 goto error

goto end

:error
echo Something went wrong!
goto end

:end

rmdir /s /q "%VCPROFILE_PATH%"

endlocal
