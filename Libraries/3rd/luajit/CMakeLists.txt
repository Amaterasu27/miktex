## CMakeLists.txt					-*- CMake -*-
##
## Copyright (C) 2015 Christian Schenk
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

add_definitions(-DLUA_BUILD_AS_DLL -DLUAJIT_ENABLE_LUA52COMPAT)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/src/host)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR}/src)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/src/host)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/host)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/dynasm)

set(generated_headers
  ${CMAKE_CURRENT_BINARY_DIR}/src/host/buildvm_arch.h
  ${CMAKE_CURRENT_BINARY_DIR}/src/lj_bcdef.h
  ${CMAKE_CURRENT_BINARY_DIR}/src/lj_ffdef.h
  ${CMAKE_CURRENT_BINARY_DIR}/src/lj_libdef.h
  ${CMAKE_CURRENT_BINARY_DIR}/src/lj_recdef.h
  ${CMAKE_CURRENT_BINARY_DIR}/src/lj_vmdef.h
  ${CMAKE_CURRENT_BINARY_DIR}/src/lj_folddef.h
)

set_source_files_properties(
  ${generated_headers}
  PROPERTIES GENERATED TRUE
)

set_source_files_properties(
  ${CMAKE_CURRENT_BINARY_DIR}/src/lj_vm.obj
  PROPERTIES
    EXTERNAL_OBJECT TRUE
    GENERATED TRUE
)

add_executable(minilua EXCLUDE_FROM_ALL
  src/host/minilua.c
)

merge_trustinfo_manifest(minilua asInvoker)

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
  set(dasmflags -D WIN -D JIT -D FFI -D P64)
else(CMAKE_SIZEOF_VOID_P EQUAL 8)
  set(dasmflags -D WIN -D JIT -D FFI)
endif(CMAKE_SIZEOF_VOID_P EQUAL 8)

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/src/host/buildvm_arch.h
  COMMAND $<TARGET_FILE:minilua> ${CMAKE_CURRENT_SOURCE_DIR}/dynasm/dynasm.lua
	-LN ${dasmflags}
	-o ${CMAKE_CURRENT_BINARY_DIR}/src/host/buildvm_arch.h
	${CMAKE_CURRENT_SOURCE_DIR}/src/vm_x86.dasc
  DEPENDS
	minilua
	${CMAKE_CURRENT_SOURCE_DIR}/dynasm/dynasm.lua
	${CMAKE_CURRENT_SOURCE_DIR}/src/vm_x86.dasc
  VERBATIM
)

set(buildvm_sources
  ${CMAKE_CURRENT_BINARY_DIR}/src/host/buildvm_arch.h
  src/host/buildvm.c
  src/host/buildvm_asm.c
  src/host/buildvm_fold.c
  src/host/buildvm_lib.c
  src/host/buildvm_peobj.c
)

add_executable(buildvm EXCLUDE_FROM_ALL ${buildvm_sources})

merge_trustinfo_manifest(buildvm asInvoker)

set(all_lib
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lbitlib.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lib_base.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lib_bit.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lib_debug.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lib_ffi.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lib_io.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lib_jit.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lib_math.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lib_os.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lib_package.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lib_string.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lib_table.c
)

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/src/lj_vm.obj
  COMMAND $<TARGET_FILE:buildvm>
	-m peobj
	-o ${CMAKE_CURRENT_BINARY_DIR}/src/lj_vm.obj
  DEPENDS
	buildvm
  VERBATIM
)

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/src/lj_bcdef.h
  COMMAND ${buildvm_exe}
	-m bcdef
	-o ${CMAKE_CURRENT_BINARY_DIR}/src/lj_bcdef.h
	${all_lib}
  DEPENDS
	buildvm
	${all_lib}
  VERBATIM
)

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/src/lj_ffdef.h
  COMMAND ${buildvm_exe}
	-m ffdef
	-o ${CMAKE_CURRENT_BINARY_DIR}/src/lj_ffdef.h
	${all_lib}
  DEPENDS
	buildvm
	${all_lib}
  VERBATIM
)

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/src/lj_libdef.h
  COMMAND ${buildvm_exe}
	-m libdef
	-o ${CMAKE_CURRENT_BINARY_DIR}/src/lj_libdef.h
	${all_lib}
  DEPENDS
	buildvm
	${all_lib}
  VERBATIM
)

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/src/lj_recdef.h
  COMMAND ${buildvm_exe}
	-m recdef
	-o ${CMAKE_CURRENT_BINARY_DIR}/src/lj_recdef.h
	${all_lib}
  DEPENDS
	buildvm
	${all_lib}
  VERBATIM
)

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/src/lj_vmdef.h
  COMMAND ${buildvm_exe}
	-m vmdef
	-o ${CMAKE_CURRENT_BINARY_DIR}/src/lj_vmdef.h
	${all_lib}
  DEPENDS
	buildvm
	${all_lib}
  VERBATIM
)

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/src/lj_folddef.h
  COMMAND ${buildvm_exe}
	-m folddef
	-o ${CMAKE_CURRENT_BINARY_DIR}/src/lj_folddef.h
	${CMAKE_CURRENT_SOURCE_DIR}/src/lj_opt_fold.c
  DEPENDS
	buildvm
	${CMAKE_CURRENT_SOURCE_DIR}/src/lj_opt_fold.c
  VERBATIM
)

set(lib_sources
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lauxlib.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lib_aux.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lib_init.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lj_alloc.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lj_alloc.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lj_api.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lj_arch.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lj_asm.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lj_asm.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lj_asm_arm.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lj_asm_mips.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lj_asm_ppc.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lj_asm_x86.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lj_bc.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lj_bc.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lj_bcdump.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lj_bcread.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lj_bcwrite.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lj_carith.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lj_carith.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lj_ccall.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lj_ccall.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lj_ccallback.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lj_ccallback.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lj_cconv.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lj_cconv.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lj_cdata.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lj_cdata.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lj_char.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lj_char.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lj_clib.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lj_clib.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lj_cparse.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lj_cparse.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lj_crecord.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lj_crecord.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lj_ctype.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lj_ctype.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lj_debug.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lj_debug.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lj_def.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lj_dispatch.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lj_dispatch.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lj_emit_arm.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lj_emit_mips.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lj_emit_ppc.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lj_emit_x86.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lj_err.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lj_err.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lj_errmsg.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lj_ff.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lj_ffrecord.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lj_ffrecord.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lj_frame.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lj_func.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lj_func.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lj_gc.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lj_gc.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lj_gdbjit.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lj_gdbjit.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lj_ir.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lj_ir.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lj_ircall.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lj_iropt.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lj_jit.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lj_lex.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lj_lex.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lj_lib.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lj_lib.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lj_load.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lj_mcode.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lj_mcode.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lj_meta.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lj_meta.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lj_obj.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lj_obj.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lj_opt_dce.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lj_opt_fold.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lj_opt_loop.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lj_opt_mem.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lj_opt_narrow.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lj_opt_sink.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lj_opt_split.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lj_parse.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lj_parse.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lj_record.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lj_record.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lj_snap.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lj_snap.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lj_state.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lj_state.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lj_str.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lj_str.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lj_strscan.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lj_strscan.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lj_tab.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lj_tab.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lj_target.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lj_target_arm.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lj_target_mips.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lj_target_ppc.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lj_target_x86.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lj_trace.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lj_trace.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lj_traceerr.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lj_udata.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lj_udata.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lj_vm.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lj_vmevent.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lj_vmevent.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lj_vmmath.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lua.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/luaconf.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/luajit.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lualib.h
)

set(${luajit_dll_name}_sources
  ${generated_headers}
  ${all_lib}
  ${lib_sources}
  ${CMAKE_CURRENT_BINARY_DIR}/src/lj_vm.obj
)

if(NATIVE_WINDOWS)
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/libluajit.rc.in
    ${CMAKE_CURRENT_BINARY_DIR}/libluajit.rc
  )
  set(${luajit_dll_name}_sources ${${luajit_dll_name}_sources}
    ${CMAKE_CURRENT_BINARY_DIR}/libluajit.rc
  )
endif(NATIVE_WINDOWS)

add_library(${luajit_dll_name} SHARED ${${luajit_dll_name}_sources})

set_source_files_properties(
  ${lib_sources} ${all_lib}
  PROPERTIES OBJECT_DEPENDS ${generated_headers}
)
  
#target_link_libraries(${luajit_dll_name}
#  ${utf8wrap_dll_name}
#)

set_target_properties(${luajit_dll_name}
  PROPERTIES
    VERSION "${MIKTEX_SERIES_STR}"
    SOVERSION "1"
)

rebase(${luajit_dll_name})

install(TARGETS ${luajit_dll_name}
  RUNTIME DESTINATION "${bindir}"
  LIBRARY DESTINATION "${libdir}"
  ARCHIVE DESTINATION "${libdir}"
)

set(luajit_sources
  src/luajit.c
  luajit-version.h
)

if(NATIVE_WINDOWS)
  set(luajit_sources ${luajit_sources}
    luajit.rc
  )
endif(NATIVE_WINDOWS)

add_executable(${miktex_prefix}luajit ${luajit_sources})

merge_trustinfo_manifest(${miktex_prefix}luajit asInvoker)

target_link_libraries(${miktex_prefix}luajit ${luajit_dll_name})

install(TARGETS ${miktex_prefix}luajit DESTINATION ${bindir})

set(includes
  lauxlib.h
  lj_arch.h
  lj_def.h
  lj_obj.h
  lj_state.h
  lua.h
  luaconf.h
  luajit.h
  lualib.h
)

foreach(i ${includes})
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/src/${i}
    ${CMAKE_BINARY_DIR}/${rel_luajit_include_dir}/${i}
    COPYONLY
  )
endforeach(i ${includes})
