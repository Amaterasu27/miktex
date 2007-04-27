@echo off

setlocal

set prodlog=%CD%\Admin\Log\produce.log

mkdir %CD%\Admin\Log

if exist "%prodlog%" del "%prodlog%"
echo -*- Compilation -*->> "%prodlog%"
echo %CD% %DATE% %TIME%>> "%prodlog%"
echo.>> "%prodlog%"

nmake instal 1>>"%prodlog%" 2>>&1

endlocal
