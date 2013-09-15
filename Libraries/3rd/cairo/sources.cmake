## sources.cmake					-*- CMake -*-
##
## Copyright (C) 2013 Christian Schenk
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

set(cairo_headers
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo.h
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-deprecated.h
)

set(cairo_private
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairoint.h
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-analysis-surface-private.h
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-arc-private.h
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-array-private.h
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-atomic-private.h
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-backend-private.h
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-box-inline.h
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-boxes-private.h
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-cache-private.h
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-clip-inline.h
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-clip-private.h
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-combsort-inline.h
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-compiler-private.h
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-compositor-private.h
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-contour-inline.h
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-contour-private.h
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-composite-rectangles-private.h
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-damage-private.h
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-default-context-private.h
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-device-private.h
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-error-inline.h
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-error-private.h
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-fixed-private.h
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-fixed-type-private.h
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-freelist-private.h
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-freelist-type-private.h
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-freed-pool-private.h
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-fontconfig-private.h
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-gstate-private.h
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-hash-private.h
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-image-info-private.h
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-image-surface-inline.h
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-image-surface-private.h
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-list-inline.h
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-list-private.h
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-malloc-private.h
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-mempool-private.h
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-mutex-impl-private.h
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-mutex-list-private.h
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-mutex-private.h
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-mutex-type-private.h
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-output-stream-private.h
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-paginated-private.h
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-paginated-surface-private.h
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-path-fixed-private.h
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-path-private.h
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-pattern-inline.h
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-pattern-private.h
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-private.h
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-recording-surface-inline.h
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-recording-surface-private.h
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-reference-count-private.h
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-region-private.h
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-rtree-private.h
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-scaled-font-private.h
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-slope-private.h
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-spans-private.h
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-spans-compositor-private.h
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-stroke-dash-private.h
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-surface-inline.h
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-surface-private.h
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-surface-backend-private.h
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-surface-clipper-private.h
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-surface-fallback-private.h
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-surface-observer-inline.h
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-surface-observer-private.h
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-surface-offset-private.h
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-surface-subsurface-inline.h
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-surface-subsurface-private.h
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-surface-snapshot-inline.h
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-surface-snapshot-private.h
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-surface-wrapper-private.h
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-time-private.h
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-types-private.h
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-traps-private.h
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-tristrip-private.h
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-user-font-private.h
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-wideint-private.h
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-wideint-type-private.h
)

set(cairo_sources
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-analysis-surface.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-arc.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-array.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-atomic.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-base64-stream.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-base85-stream.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-bentley-ottmann.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-bentley-ottmann-rectangular.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-bentley-ottmann-rectilinear.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-botor-scan-converter.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-boxes.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-boxes-intersect.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-cache.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-clip.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-clip-boxes.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-clip-polygon.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-clip-region.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-clip-surface.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-color.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-composite-rectangles.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-compositor.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-contour.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-damage.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-debug.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-default-context.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-device.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-error.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-fallback-compositor.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-fixed.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-font-face.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-font-face-twin.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-font-face-twin-data.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-font-options.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-freelist.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-freed-pool.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-gstate.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-hash.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-hull.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-image-compositor.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-image-info.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-image-source.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-image-surface.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-lzw.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-matrix.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-mask-compositor.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-mesh-pattern-rasterizer.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-mempool.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-misc.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-mono-scan-converter.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-mutex.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-no-compositor.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-observer.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-output-stream.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-paginated-surface.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-path-bounds.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-path.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-path-fill.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-path-fixed.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-path-in-fill.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-path-stroke.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-path-stroke-boxes.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-path-stroke-polygon.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-path-stroke-traps.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-path-stroke-tristrip.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-pattern.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-pen.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-polygon.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-polygon-intersect.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-polygon-reduce.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-raster-source-pattern.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-recording-surface.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-rectangle.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-rectangular-scan-converter.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-region.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-rtree.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-scaled-font.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-shape-mask-compositor.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-slope.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-spans.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-spans-compositor.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-spline.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-stroke-dash.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-stroke-style.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-surface.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-surface-clipper.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-surface-fallback.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-surface-observer.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-surface-offset.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-surface-snapshot.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-surface-subsurface.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-surface-wrapper.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-time.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-tor-scan-converter.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-tor22-scan-converter.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-clip-tor-scan-converter.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-toy-font-face.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-traps.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-tristrip.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-traps-compositor.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-unicode.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-user-font.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-version.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-wideint.c
)

set(_cairo_font_subset_private
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-scaled-font-subsets-private.h
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-truetype-subset-private.h
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-type1-private.h
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-type3-glyph-surface-private.h
)

set(_cairo_font_subset_sources
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-cff-subset.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-scaled-font-subsets.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-truetype-subset.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-type1-fallback.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-type1-glyph-names.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-type1-subset.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-type3-glyph-surface.c
)

set(cairo_private
  ${cairo_private}
  ${_cairo_font_subset_private}
)

set(cairo_sources
  ${cairo_sources}
  ${_cairo_font_subset_sources}
)

#cairo_egl_sources =
#cairo_glx_sources =
#cairo_wgl_sources =

set(_cairo_pdf_operators_private
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-pdf-operators-private.h
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-pdf-shading-private.h
)

set(_cairo_pdf_operators_sources
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-pdf-operators.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-pdf-shading.c
)

set(cairo_private
  ${cairo_private}
  ${_cairo_pdf_operators_private}
)

set(cairo_sources
  ${cairo_sources}
  ${_cairo_pdf_operators_sources}
)

set(cairo_png_sources
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-png.c
)

set(cairo_ps_headers
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-ps.h
)

set(cairo_ps_private
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-ps-surface-private.h
)

set(cairo_ps_sources
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-ps-surface.c
)

set(_cairo_deflate_stream_sources
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-deflate-stream.c
)

set(cairo_sources
  ${cairo_sources}
  ${_cairo_deflate_stream_sources}
)

set(cairo_pdf_headers
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-pdf.h
)

set(cairo_pdf_private
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-pdf-surface-private.h
)

set(cairo_pdf_sources
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-pdf-surface.c
)

set(cairo_svg_headers
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-svg.h
)

set(cairo_svg_private
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-svg-surface-private.h
)

set(cairo_svg_sources
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-svg-surface.c
)

set(cairo_ft_headers
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-ft.h
)

set(cairo_ft_private
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-ft-private.h
)

set(cairo_ft_sources
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-ft-font.c
)

set(cairo_test_surfaces_private
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/test-compositor-surface.h
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/test-compositor-surface-private.h
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/test-null-compositor-surface.h
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/test-paginated-surface.h
)

set(cairo_test_surfaces_sources
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/test-compositor-surface.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/test-null-compositor-surface.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/test-base-compositor-surface.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/test-paginated-surface.c
)

set(cairo_xlib_headers
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-xlib.h
)

set(cairo_xlib_private
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-xlib-private.h
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-xlib-surface-private.h
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-xlib-xrender-private.h
)

set(cairo_xlib_sources
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-xlib-display.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-xlib-core-compositor.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-xlib-fallback-compositor.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-xlib-render-compositor.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-xlib-screen.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-xlib-source.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-xlib-surface.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-xlib-surface-shm.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-xlib-visual.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-xlib-xcb-surface.c
)

set(cairo_xlib_xrender_headers
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-xlib-xrender.h
)

set(cairo_xcb_headers
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-xcb.h
)

set(cairo_xcb_private
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-xcb-private.h
)

set(cairo_xcb_sources
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-xcb-connection.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-xcb-connection-core.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-xcb-connection-render.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-xcb-connection-shm.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-xcb-screen.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-xcb-shm.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-xcb-surface.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-xcb-surface-core.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-xcb-surface-render.c
)

set(cairo_qt_headers
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-qt.h
)

set(cairo_qt_cxx_sources
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-qt-surface.cpp
)

set(cairo_quartz_headers
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-quartz.h
)

set(cairo_quartz_private
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-quartz-private.h
)

set(cairo_quartz_sources
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-quartz-surface.c
)

set(cairo_quartz_image_headers
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-quartz-image.h
)

set(cairo_quartz_image_sources
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-quartz-image-surface.c
)

set(cairo_quartz_font_sources
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-quartz-font.c
)

set(cairo_win32_headers
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-win32.h
)

set(cairo_win32_private
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/win32/cairo-win32-private.h
)

set(cairo_win32_sources
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/win32/cairo-win32-debug.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/win32/cairo-win32-device.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/win32/cairo-win32-gdi-compositor.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/win32/cairo-win32-system.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/win32/cairo-win32-surface.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/win32/cairo-win32-display-surface.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/win32/cairo-win32-printing-surface.c
)

set(cairo_win32_font_sources
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/win32/cairo-win32-font.c
)

set(cairo_skia_headers
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-skia.h
)

set(cairo_skia_private
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/skia/cairo-skia-private.h
)

set(cairo_skia_cxx_sources
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/skia/cairo-skia-context.cpp
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/skia/cairo-skia-surface.cpp
)

set(cairo_os2_headers
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-os2.h
)

set(cairo_os2_private
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-os2-private.h
)

set(cairo_os2_sources
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-os2-surface.c
)

set(cairo_beos_headers
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-beos.h
)

set(cairo_beos_cxx_sources
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-beos-surface.cpp
)

set(cairo_gl_headers
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-gl.h
)

set(cairo_gl_private
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-gl-private.h
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-gl-dispatch-private.h
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-gl-ext-def-private.h
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-gl-gradient-private.h
)

set(cairo_gl_sources
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-gl-composite.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-gl-device.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-gl-dispatch.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-gl-glyphs.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-gl-gradient.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-gl-info.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-gl-operand.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-gl-shaders.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-gl-msaa-compositor.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-gl-spans-compositor.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-gl-traps-compositor.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-gl-source.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-gl-surface.c
)

set(cairo_glesv2_headers
  ${cairo_gl_headers}
)

set(cairo_glesv2_private
  ${cairo_gl_private}
)

set(cairo_glesv2_sources
  ${cairo_gl_sources}
)

set(cairo_egl_sources
  ${cairo_egl_sources}
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-egl-context.c
)

set(cairo_glx_sources
  ${cairo_glx_sources}
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-glx-context.c
)

set(cairo_wgl_sources
  ${cairo_wgl_sources}
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-wgl-context.c
)

set(cairo_directfb_headers
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-directfb.h
)

set(cairo_directfb_sources
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-directfb-surface.c
)

set(cairo_drm_headers
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-drm.h
)

set(cairo_drm_private
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/drm/cairo-drm-private.h
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/drm/cairo-drm-ioctl-private.h
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/drm/cairo-drm-intel-private.h
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/drm/cairo-drm-intel-brw-defines.h
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/drm/cairo-drm-intel-brw-structs.h
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/drm/cairo-drm-intel-brw-eu.h
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/drm/cairo-drm-intel-command-private.h
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/drm/cairo-drm-intel-ioctl-private.h
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/drm/cairo-drm-i915-private.h
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/drm/cairo-drm-i965-private.h
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/drm/cairo-drm-radeon-private.h
)

set(cairo_drm_sources
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/drm/cairo-drm.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/drm/cairo-drm-bo.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/drm/cairo-drm-surface.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/drm/cairo-drm-intel.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/drm/cairo-drm-intel-debug.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/drm/cairo-drm-intel-surface.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/drm/cairo-drm-i915-surface.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/drm/cairo-drm-i915-glyphs.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/drm/cairo-drm-i915-shader.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/drm/cairo-drm-i915-spans.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/drm/cairo-drm-i965-surface.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/drm/cairo-drm-i965-glyphs.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/drm/cairo-drm-i965-shader.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/drm/cairo-drm-i965-spans.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/drm/cairo-drm-intel-brw-eu.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/drm/cairo-drm-intel-brw-eu-emit.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/drm/cairo-drm-intel-brw-eu-util.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/drm/cairo-drm-radeon.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/drm/cairo-drm-radeon-surface.c
)

set(cairo_gallium_sources
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/drm/cairo-drm-gallium-surface.c
)

set(cairo_script_headers
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-script.h
)

set(cairo_script_private
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-script-private.h
)

set(cairo_script_sources
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-script-surface.c
)

set(cairo_tee_headers
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-tee.h
)

set(cairo_tee_private
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-tee-surface-private.h
)

set(cairo_tee_sources
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-tee-surface.c
)

set(cairo_xml_headers
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-xml.h
)

set(cairo_xml_sources
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-xml-surface.c
)

set(cairo_vg_headers
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-vg.h
)

set(cairo_vg_sources
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-vg-surface.c
)

set(cairo_cogl_headers
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-cogl.h
)

set(cairo_cogl_private
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-cogl-private.h
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/ cairo-cogl-gradient-private.h
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/ cairo-cogl-context-private.h
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/ cairo-cogl-utils-private.h
)

set(cairo_cogl_sources
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-cogl-surface.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-cogl-gradient.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-cogl-context.c
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-cogl-utils.c
)
