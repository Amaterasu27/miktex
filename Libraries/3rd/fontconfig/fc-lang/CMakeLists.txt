## CMakeLists.txt						-*- CMake -*-
##
## Copyright (C) 2008-2014 Christian Schenk
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
  ak.orth
  am.orth
  an.orth
  ar.orth
  as.orth
  ast.orth
  av.orth
  ay.orth
  az_az.orth
  az_ir.orth
  ba.orth
  be.orth
  ber_dz.orth
  ber_ma.orth
  bg.orth
  bh.orth
  bho.orth
  bi.orth
  bin.orth
  bm.orth
  bn.orth
  bo.orth
  br.orth
  brx.orth
  bs.orth
  bua.orth
  byn.orth
  ca.orth
  ce.orth
  ch.orth
  chm.orth
  chr.orth
  co.orth
  crh.orth
  cs.orth
  csb.orth
  cu.orth
  cv.orth
  cy.orth
  da.orth
  de.orth
  doi.orth
  dv.orth
  dz.orth
  ee.orth
  el.orth
  en.orth
  eo.orth
  es.orth
  et.orth
  eu.orth
  fa.orth
  fat.orth
  ff.orth
  fi.orth
  fil.orth
  fj.orth
  fo.orth
  fr.orth
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
  hne.orth
  ho.orth
  hr.orth
  hsb.orth
  ht.orth
  hu.orth
  hy.orth
  hz.orth
  ia.orth
  id.orth
  ie.orth
  ig.orth
  ii.orth
  ik.orth
  io.orth
  is.orth
  it.orth
  iu.orth
  ja.orth
  jv.orth
  ka.orth
  kaa.orth
  kab.orth
  ki.orth
  kj.orth
  kk.orth
  kl.orth
  km.orth
  kn.orth
  ko.orth
  kok.orth
  kr.orth
  ks.orth
  ku_am.orth
  ku_iq.orth
  ku_ir.orth
  ku_tr.orth
  kum.orth
  kv.orth
  kw.orth
  kwm.orth
  ky.orth
  la.orth
  lah.orth
  lb.orth
  lez.orth
  lg.orth
  li.orth
  ln.orth
  lo.orth
  lt.orth
  lv.orth
  mai.orth
  mg.orth
  mh.orth
  mi.orth
  mk.orth
  ml.orth
  mn_cn.orth
  mn_mn.orth
  mni.orth
  mo.orth
  mr.orth
  ms.orth
  mt.orth
  my.orth
  na.orth
  nb.orth
  nds.orth
  ne.orth
  ng.orth
  nl.orth
  nn.orth
  no.orth
  nqo.orth
  nr.orth
  nso.orth
  nv.orth
  ny.orth
  oc.orth
  om.orth
  or.orth
  os.orth
  ota.orth
  pa.orth
  pa_pk.orth
  pap_an.orth
  pap_aw.orth
  pl.orth
  ps_af.orth
  ps_pk.orth
  pt.orth
  qu.orth
  rm.orth
  rn.orth
  ro.orth
  ru.orth
  rw.orth
  sa.orth
  sah.orth
  sat.orth
  sc.orth
  sco.orth
  sd.orth
  se.orth
  sel.orth
  sg.orth
  sh.orth
  shs.orth
  si.orth
  sid.orth
  sk.orth
  sl.orth
  sm.orth
  sma.orth
  smj.orth
  smn.orth
  sms.orth
  sn.orth
  so.orth
  sq.orth
  sr.orth
  ss.orth
  st.orth
  su.orth
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
  ty.orth
  tyv.orth
  ug.orth
  uk.orth
  ur.orth
  uz.orth
  ve.orth
  vi.orth
  vo.orth
  vot.orth
  wa.orth
  wal.orth
  wen.orth
  wo.orth
  xh.orth
  yap.orth
  yi.orth
  yo.orth
  za.orth
  zh_cn.orth
  zh_hk.orth
  zh_mo.orth
  zh_sg.orth
  zh_tw.orth
  zu.orth
)

add_definitions(
  -DHAVE_CONFIG_H
  -DDONT_USE_MIKTEX_EXIT
  -D_UNICODE -DUNICODE
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
  ${utf8wrap_dll_name}
)

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/fcaliastail.h
  COMMAND ${CMAKE_COMMAND} -E touch ${CMAKE_CURRENT_BINARY_DIR}/fcaliastail.h
  VERBATIM
)

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/fclang.h
  COMMAND $<TARGET_FILE:fc-lang> -d ${CMAKE_CURRENT_SOURCE_DIR} ${orth_files}
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
