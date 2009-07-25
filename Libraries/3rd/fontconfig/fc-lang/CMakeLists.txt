## CMakeLists.txt						-*- CMake -*-
##
## Copyright (C) 2008-2009 Christian Schenk
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

set(orth_files
  aa.orth
  ab.orth
  af.orth
  am.orth
  ar.orth
  as.orth
  ast.orth
  ava.orth
  ay.orth
  az.orth
  az_ir.orth
  ba.orth
  bam.orth
  be.orth
  bg.orth
  bh.orth
  bho.orth
  bi.orth
  bin.orth
  bn.orth
  bo.orth
  br.orth
  bs.orth
  bua.orth
  ca.orth
  ce.orth
  ch.orth
  chm.orth
  chr.orth
  co.orth
  cs.orth
  cu.orth
  cv.orth
  cy.orth
  da.orth
  de.orth
  dz.orth
  el.orth
  en.orth
  eo.orth
  es.orth
  et.orth
  eu.orth
  fa.orth
  fi.orth
  fj.orth
  fo.orth
  fr.orth
  ful.orth
  fur.orth
  fy.orth
  ga.orth
  gd.orth
  gez.orth
  gl.orth
  gn.orth
  gu.orth
  gv.orth
  ha.orth
  haw.orth
  he.orth
  hi.orth
  ho.orth
  hr.orth
  hu.orth
  hy.orth
  ia.orth
  ibo.orth
  id.orth
  ie.orth
  ik.orth
  io.orth
  is.orth
  it.orth
  iu.orth
  ja.orth
  ka.orth
  kaa.orth
  ki.orth
  kk.orth
  kl.orth
  km.orth
  kn.orth
  ko.orth
  kok.orth
  ks.orth
  ku.orth
  ku_ir.orth
  kum.orth
  kv.orth
  kw.orth
  ky.orth
  la.orth
  lb.orth
  lez.orth
  ln.orth
  lo.orth
  lt.orth
  lv.orth
  mg.orth
  mh.orth
  mi.orth
  mk.orth
  ml.orth
  mn.orth
  mo.orth
  mr.orth
  mt.orth
  my.orth
  nb.orth
  nds.orth
  ne.orth
  nl.orth
  nn.orth
  no.orth
  nr.orth
  nso.orth
  ny.orth
  oc.orth
  om.orth
  or.orth
  os.orth
  pa.orth
  pl.orth
  ps_af.orth
  ps_pk.orth
  pt.orth
  rm.orth
  ro.orth
  ru.orth
  sa.orth
  sah.orth
  sco.orth
  se.orth
  sel.orth
  sh.orth
  shs.orth
  si.orth
  sk.orth
  sl.orth
  sm.orth
  sma.orth
  smj.orth
  smn.orth
  sms.orth
  so.orth
  sq.orth
  sr.orth
  ss.orth
  st.orth
  sv.orth
  sw.orth
  syr.orth
  ta.orth
  te.orth
  tg.orth
  th.orth
  ti_er.orth
  ti_et.orth
  tig.orth
  tk.orth
  tl.orth
  tn.orth
  to.orth
  tr.orth
  ts.orth
  tt.orth
  tw.orth
  tyv.orth
  ug.orth
  uk.orth
  ur.orth
  uz.orth
  ven.orth
  vi.orth
  vo.orth
  vot.orth
  wa.orth
  wen.orth
  wo.orth
  xh.orth
  yap.orth
  yi.orth
  yo.orth
  zh_cn.orth
  zh_hk.orth
  zh_mo.orth
  zh_sg.orth
  zh_tw.orth
  zu.orth
)

add_definitions(
  -DHAVE_CONFIG_H
)

include_directories(BEFORE
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_SOURCE_DIR}/${rel_fontconfig_dir}/src
)

set(fc-lang_sources
  ${CMAKE_CURRENT_BINARY_DIR}/fcaliastail.h
  fc-lang.c
)

set_source_files_properties(
  ${CMAKE_CURRENT_BINARY_DIR}/fcaliastail.h
  PROPERTIES GENERATED TRUE
)

add_executable(fc-lang ${fc-lang_sources})

add_dependencies(fc-lang fccase_h)

merge_trustinfo_manifest(fc-lang asInvoker)

target_link_libraries(fc-lang
  ${app_dll_name}
  $(core_dll_name)
  ${getopt_dll_name}
)

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/fcaliastail.h
  COMMAND ${CMAKE_COMMAND} -E touch ${CMAKE_CURRENT_BINARY_DIR}/fcaliastail.h
  VERBATIM
)

get_target_property(fc-lang_exe fc-lang LOCATION)

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/fclang.h
  COMMAND ${fc-lang_exe} -d ${CMAKE_CURRENT_SOURCE_DIR} ${orth_files}
	< ${CMAKE_CURRENT_SOURCE_DIR}/fclang.tmpl.h
	> ${CMAKE_CURRENT_BINARY_DIR}/fclang.h
  MAIN_DEPENDENCY fclang.tmpl.h
  DEPENDS
	fc-lang
	${orth_files}
	${CMAKE_CURRENT_SOURCE_DIR}/fclang.tmpl.h
  VERBATIM
)

add_custom_target(fclang_h ALL
    DEPENDS
	${CMAKE_CURRENT_BINARY_DIR}/fclang.h
)
