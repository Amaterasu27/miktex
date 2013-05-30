#pragma once

#if defined(__cplusplus)
#define open_input(f_ptr, filefmt, fopen_mode) \
  MiKTeX::Web2C::OpenInput(nameoffile + 1, f_ptr, filefmt, fopen_mode)
#else
#define open_input(f_ptr, filefmt, fopen_mode) \
  UNIMPLEMENTED_miktex_web2c_open_input(f_ptr, filefmt, fopen_mode)
#endif

#define close_file(f) ((f) != 0 ? (void)fclose(f) : (void)0)

#define versionstring miktex_web2c_version_string

#define recorder_change_filename(new_name)

#define recorder_record_input(fname)

#define recorder_record_output(fname)

#define recorder_enabled miktex_web2c_recorder_enabled

#if defined(THEAPP)
#  define output_directory (THEAPP.GetOutputDirectory().Empty() ? 0 : THEAPP.GetOutputDirectory().Get())
#else
#  define output_directory miktex_web2c_output_directory
#endif

#define fullnameoffile miktex_web2c_fullnameoffile

#define setupboundvariable(var, var_name, dflt) \
  miktex_setupboundvariable(var, var_name, dflt)
