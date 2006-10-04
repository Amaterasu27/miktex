@echo off

setlocal

if "%MIKTEX_J2000%" == "" call setenv.cmd

set sdkname=miktex-sdk-%MIKTEX_SERIES%.%MIKTEX_J2000%
set sdkdir=C:\MiKTeX-SDKs\%sdkname%

set libdirs=%libdirs% Libraries\MiKTeX\App
set libdirs=%libdirs% Libraries\MiKTeX\Core
set libdirs=%libdirs% Libraries\MiKTeX\PackageManager
set libdirs=%libdirs% Libraries\3rd\libmd5

if exist "%sdkdir%" rmdir /s /q "%sdkdir%"
mkdir "%sdkdir%"
mkdir "%sdkdir%\include"
mkdir "%sdkdir%\lib"
echo .svn>excl
echo version.h.in>>excl
xcopy /EXCLUDE:excl /S SDK "%sdkdir%"

for %%d in (%libdirs%) do (
  xcopy /EXCLUDE:excl /S %%d\include "%sdkdir%\include"
  copy %%d\Release\*.lib "%sdkdir%\lib"
)
del excl

pushd %sdkdir%
cd ..
zip -9r %sdkname%.zip %sdkname%
popd

endlocal
