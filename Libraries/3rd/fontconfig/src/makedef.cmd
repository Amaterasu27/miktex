@echo off

set headers=../fontconfig/fontconfig.h 
set headers=%headers% ../fontconfig/fcfreetype.h
set headers=%headers% ../fontconfig/fcprivate.h

echo EXPORTS

( cat %headers% & echo FcError ( ) ^
  | grep '^^Fc[^^ ]* *(' ^
  | sed -e 's/ *(.*$//' -e 's/^^/	/' ^
  | sort
