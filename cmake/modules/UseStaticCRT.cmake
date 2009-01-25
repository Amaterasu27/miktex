## UseStaticCRT.cmake
##
## Copyright (C) 2006-2008 Christian Schenk
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

###############################################################################
# add_cxx_flag
# 
# Add the specified C++ compiler flag.
###############################################################################

macro(add_cxx_flag _flag)
  foreach(c "" "_DEBUG" "_RELEASE" "_MINSIZEREL" "_RELWITHDEBINFO")
    set(CMAKE_CXX_FLAGS${c} "${CMAKE_CXX_FLAGS${c}} ${_flag}")
  endforeach(c)
endmacro(add_cxx_flag)

###############################################################################
# add_c_flag
# 
# Add the specified C compiler flag.
###############################################################################

macro(add_c_flag _flag)
  foreach(c "" "_DEBUG" "_RELEASE" "_MINSIZEREL" "_RELWITHDEBINFO")
    set(CMAKE_C_FLAGS${c} "${CMAKE_C_FLAGS${c}} ${_flag}")
  endforeach(c)
endmacro(add_c_flag)

###############################################################################
# remove_cxx_flag
# 
# Remove the specified C++ compiler flag (a regular expression).
###############################################################################

macro(remove_cxx_flag _flag_regex)
  foreach(c "" "_DEBUG" "_RELEASE" "_MINSIZEREL" "_RELWITHDEBINFO")
    string(REGEX REPLACE
           "${_flag_regex}( |$)" ""
           CMAKE_CXX_FLAGS${c}
           "${CMAKE_CXX_FLAGS${c}}")
  endforeach(c)
endmacro(remove_cxx_flag)

###############################################################################
# remove_c_flag
# 
# Remove the specified C compiler flag (a regular expression).
###############################################################################

macro(remove_c_flag _flag_regex)
  foreach(c "" "_DEBUG" "_RELEASE" "_MINSIZEREL" "_RELWITHDEBINFO")
    string(REGEX REPLACE
           "${_flag_regex}( |$)" ""
           CMAKE_C_FLAGS${c}
           "${CMAKE_C_FLAGS${c}}")
  endforeach(c)
endmacro(remove_c_flag)

###############################################################################
# hide_symbols
###############################################################################

macro(hide_symbols)
  if(HAVE_CXX_VISIBILITY_OPTION)
    add_c_flag(-fvisibility=hidden)
    add_cxx_flag(-fvisibility=hidden)
    add_cxx_flag(-fvisibility-inlines-hidden)
  endif(HAVE_CXX_VISIBILITY_OPTION)
endmacro(hide_symbols)

###############################################################################
# use_static_crt
# 
# Prepare to link against the static C runtime library.
###############################################################################

macro(use_static_crt)
  if(MSVC)
    foreach(c "" "_DEBUG" "_RELEASE" "_MINSIZEREL" "_RELWITHDEBINFO")
      string(REPLACE "/MD" "/MT" CMAKE_C_FLAGS${c} "${CMAKE_C_FLAGS${c}}")
      string(REPLACE "/MD" "/MT" CMAKE_CXX_FLAGS${c} "${CMAKE_CXX_FLAGS${c}}")
    endforeach(c)
  endif(MSVC)
  remove_definitions(
    -D_AFXDLL
    -D_ATL_DLL
  )
endmacro(use_static_crt)
