## DelayLoad.cmake
##
## Copyright (C) 2007-2014 Christian Schenk
## 
## This file is free software; you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published
## by the Free Software Foundation; either version 2, or (at your
## option) any later version.
## 
## This file is distributed in the hope that it will be useful, but
## WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
## General Public License for more details.
## 
## You should have received a copy of the GNU General Public License
## along with this file; if not, write to the Free Software
## Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307,
## USA.

macro(delay_load _target)
  if(NATIVE_WINDOWS)
    foreach(_dll_name ${ARGN})
      add_link_flags (${_target} "/DELAYLOAD:${_dll_name}.dll")
    endforeach(_dll_name ${ARGN})
    target_link_libraries(${_target} PRIVATE delayimp)
  endif(NATIVE_WINDOWS)
endmacro(delay_load)
